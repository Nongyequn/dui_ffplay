#ifndef VIDEOPICTUREREFRESH_H
#define VIDEOPICTUREREFRESH_H

/*
����:��Ļ����ˢ�º���
*/

#include "ffmpeg.h"
#include "MSDL.h"
#include "VideoState.h"
#include "ScreenRender.h"
#include "VideoProcesser.h"

ScreenRender *CreateScreenRender(void *Render,RenderFunc func);
void InitScreenRender(ScreenRender * is,void *Render,RenderFunc func);
void FreeScreenRender(ScreenRender **pis);

//ˢ����ʾ����
void video_refresh(VideoProcesser *vp,ScreenRender *sr,
	SubtitleBuffer *sb,	//��Ļ������
	double *remaining_time);

void toggle_pause(VideoState *is,VideoSynchronize *vs,SynchronizeClockState *scs);
void step_to_next_frame(VideoState *is,VideoSynchronize *vs,SynchronizeClockState *scs);

#endif
