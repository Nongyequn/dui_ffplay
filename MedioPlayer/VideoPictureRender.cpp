#include "VideoPictureRender.h"
#include "VideoProcesser.h"
#include "SynchronizeClock.h"
#include "MClock.h"
#include "SDLVideoRender.h"
#include "SubtitleProcesser.h"

/* pause or resume the video */
int stream_toggle_pause(VideoSynchronize *vs,SynchronizeClockState *is,int paused,int read_pause_return)
{
    if (paused) {
        vs->frame_timer += av_gettime() / 1000000.0 + is->vidclk.pts_drift - is->vidclk.pts;
	}
	return sc_stream_toggle_pause(is,paused,read_pause_return);
}

/* pause or resume the video */
void toggle_pause(VideoState *is,VideoSynchronize *vs,SynchronizeClockState *scs)
{
	is->paused = stream_toggle_pause(vs,scs,is->paused,is->read_pause_return);
    vs->step = 0;
}

void step_to_next_frame(VideoState *is,VideoSynchronize *vs,SynchronizeClockState *scs)
{
    /* if the stream is paused unpause it, then step */
    if (is->paused)
		is->paused = stream_toggle_pause(vs,scs,is->paused,is->read_pause_return);
    vs->step = 1;
}

void pictq_next_picture(VideoPictureBuffer *is) 
 {
    /* update queue size and signal for next picture */
    if (++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
        is->pictq_rindex = 0;

    SDL_LockMutex(is->pictq_mutex);
    is->pictq_size--;
    SDL_CondSignal(is->pictq_cond);
    SDL_UnlockMutex(is->pictq_mutex);
}

 int pictq_prev_picture(VideoPictureBuffer *is,VideoState *vs)
 {
    VideoPicture *prevvp;
    int ret = 0;
    /* update queue size and signal for the previous picture */
    prevvp = &is->pictq[(is->pictq_rindex + VIDEO_PICTURE_QUEUE_SIZE - 1) % VIDEO_PICTURE_QUEUE_SIZE];
    if (prevvp->allocated && prevvp->serial == vs->videoq.serial) {
        SDL_LockMutex(is->pictq_mutex);
        if (is->pictq_size < VIDEO_PICTURE_QUEUE_SIZE) {
            if (--is->pictq_rindex == -1)
                is->pictq_rindex = VIDEO_PICTURE_QUEUE_SIZE - 1;
            is->pictq_size++;
            ret = 1;
        }
        SDL_CondSignal(is->pictq_cond);
        SDL_UnlockMutex(is->pictq_mutex);
    }
    return ret;
}
/* called to display each frame */
void video_refresh(VideoState *is,ScreenRender *sr,
	SubtitleBuffer *sb,		//��Ļ������
	VideoPictureBuffer *vpb, //��Ƶͼ�񻺳���
	VideoSynchronize *vs,SynchronizeClockState *scs,
	double *remaining_time)
{
    VideoPicture *vp;
    double time;
	int framedrop = -1;
    SubPicture *sp, *sp2;

	if (!is->paused && get_master_sync_type(scs) == AV_SYNC_EXTERNAL_CLOCK && is->realtime)
        check_external_clock_speed(is,scs);

	if (!sr->display_disable && is->show_mode != SHOW_MODE_VIDEO && is->audio_st) {
        time = av_gettime() / 1000000.0;
		//ǿ��ˢ��
		if (sr->force_refresh || sr->last_vis_time + 0.02 < time) {
			if(sr->audio_sample_display != NULL){
				sr->audio_sample_display(is,sr,scs);//��ʾͼ��
			}
            //audio_sample_display(is,sr,scs);
            sr->last_vis_time = time;
        }
        *remaining_time = FFMIN(*remaining_time, sr->last_vis_time + 0.02 - time);
    }

    if (is->video_st) {
        int redisplay = 0;
        if (sr->force_refresh)
			redisplay = pictq_prev_picture(vpb,is);
retry:
        if (vpb->pictq_size == 0) {
			SDL_LockMutex(vpb->pictq_mutex);
			//����������������ݣ���ʹ�ö��������ݰ�PTS���µ�ǰPTS
            if (vs->frame_last_dropped_pts != AV_NOPTS_VALUE && vs->frame_last_dropped_pts > vs->frame_last_pts) {
                update_video_pts(scs, vs->frame_last_dropped_pts, vs->frame_last_dropped_serial);
				vs->video_current_pos = vs->frame_last_dropped_pos;
                vs->frame_last_dropped_pts = AV_NOPTS_VALUE;
            }
            SDL_UnlockMutex(vpb->pictq_mutex);
            // nothing to do, no picture to display in the queue
        } else {
            double last_duration, duration, delay = 0.0;
            /* dequeue the picture */
            vp = &vpb->pictq[vpb->pictq_rindex];
			//�����ǰͼ����Ч,����ʾ��һ��
            if (vp->serial != is->videoq.serial) {
				pictq_next_picture(vpb);
                redisplay = 0;
                goto retry;
            }
			//�����ͣ��ֱ����ʾ
            if (is->paused)
                goto display;

			//���㵱ǰͼ������һ�ŵ�PTS��ֵ,�õ�ǰ֡��pts��ȥ��һ֡��pts���Ӷ������һ�����Ƶ�delayֵ
			/* compute nominal last_duration */
            last_duration = vp->pts - vs->frame_last_pts;
			//�����ǰPTS��ֵ���޶���Χ�ڣ����¼
            if (!isnan(last_duration) && last_duration > 0 && last_duration < vs->max_frame_duration) {
                /* if duration of the last frame was sane, update last_duration in video state */
                vs->frame_last_duration = last_duration;
            }

            if (redisplay)//�ػ���
                delay = 0.0;
            else
                delay = compute_target_delay(vs->frame_last_duration,vs->max_frame_duration, scs);

			time= av_gettime()/1000000.0;
			//�����ǰʱ��С��֡ʱ����ӳ�ʱ��,��ȴ�
            if (time < vs->frame_timer + delay && !redisplay) {
                *remaining_time = FFMIN(vs->frame_timer + delay - time, *remaining_time);
                return;
            }
			//��֡ʱ������ӳټ��㵱ǰ֡��ʾʱ��
            vs->frame_timer += delay;
			//����ӳٴ���0��ͬʱ��ǰʱ�����֡����ʱ��֮�����AV_SYNC_THRESHOLD_MAX,��ǰ֡����ʱ��Ϊtime
            if (delay > 0 && time - vs->frame_timer > AV_SYNC_THRESHOLD_MAX)
                vs->frame_timer = time;

			//����ʱ��
            SDL_LockMutex(vpb->pictq_mutex);
            if (!redisplay && !isnan(vp->pts)){
                update_video_pts(scs, vp->pts, vp->serial);

				vs->video_current_pos = vp->pos;
				vs->frame_last_pts = vp->pts;
			}
            SDL_UnlockMutex(vpb->pictq_mutex);
			//�����һ֡�����Ƿ����ӳ���
            if (vpb->pictq_size > 1) {
                VideoPicture *nextvp = &vpb->pictq[(vpb->pictq_rindex + 1) % VIDEO_PICTURE_QUEUE_SIZE];
                duration = nextvp->pts - vp->pts;
				//�����֡ͬʱ���ػ���߶�֡��ͬʱ��ǰʱ����ڵ�ǰ֡��ʾʱ�����һ֡�ӳ�ʱ�䣬��ֱ���е���һ֡
				if(time > vs->frame_timer + duration){
					if(!vs->step && (redisplay || framedrop>0 || (framedrop && get_master_sync_type(scs) != AV_SYNC_VIDEO_MASTER))){
						pictq_next_picture(vpb);
						redisplay = 0;
						goto retry;
					}
				}
            }

			if (is->subtitle_st) {
                    while (sb->subpq_size > 0) {
                        sp = &sb->subpq[sb->subpq_rindex];

                        if (sb->subpq_size > 1)
                            sp2 = &sb->subpq[(sb->subpq_rindex + 1) % SUBPICTURE_QUEUE_SIZE];
                        else
                            sp2 = NULL;

                        if (sp->serial != is->subtitleq.serial
                                || (scs->vidclk.pts > (sp->pts + ((float) sp->sub.end_display_time / 1000)))
                                || (sp2 && scs->vidclk.pts > (sp2->pts + ((float) sp2->sub.start_display_time / 1000))))
                        {
                            free_subpicture(sp);

                            /* update queue size and signal for next picture */
                            if (++sb->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
                                sb->subpq_rindex = 0;

                            SDL_LockMutex(sb->subpq_mutex);
                            sb->subpq_size--;
                            SDL_CondSignal(sb->subpq_cond);
                            SDL_UnlockMutex(sb->subpq_mutex);
                        } else {
                            break;
                        }
                    }
            }
display:
			/* display picture */
            if (!sr->display_disable && is->show_mode == SHOW_MODE_VIDEO){
				if(sr->video_display != NULL){
					sr->video_display(is,sr,sb,vpb,scs);//��ʾͼ��
				}
				//video_display(is,sr,sb,vpb,scs);//��ʾͼ��
			}

			pictq_next_picture(vpb);

            if (vs->step && !is->paused){
				is->paused = stream_toggle_pause(vs,scs,is->paused,is->read_pause_return);
				vs->step = 0;
			}

        }
    }
	sr->force_refresh = 0;
}

void video_refresh(VideoProcesser *vp,ScreenRender *sr,
	SubtitleBuffer *sb,	//��Ļ������
	double *remaining_time)
{
	if(vp == NULL) return;
	video_refresh(vp->is,sr,sb,vp->vpb,vp->vs,vp->scs,remaining_time);
}


ScreenRender *CreateScreenRender(void *Render,RenderFunc func)
{
	ScreenRender *is = (ScreenRender*)av_malloc(sizeof(ScreenRender));
	memset(is,0,sizeof(ScreenRender));
	
	is->fill_picture = func.fill_picture;
	is->audio_sample_display = func.audio_sample_display;
	is->video_display = func.video_display;
	is->Render = Render;
	return is;
}
void InitScreenRender(ScreenRender * is,void *Render,RenderFunc func)
{
	if(is == NULL) return;
	is->fill_picture = func.fill_picture;
	is->audio_sample_display = func.audio_sample_display;
	is->video_display = func.video_display;
	is->Render = Render;

	is->display_disable = 0;
	is->force_refresh = 0;
	is->last_vis_time = 0.0;
}
void FreeScreenRender(ScreenRender **pis)
{
	if(pis == NULL) return;
	if(*pis != NULL)
	{
		av_free(*pis);
	}
	*pis = NULL;
}