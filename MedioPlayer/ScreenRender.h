#ifndef SCREENRENDER_H
#define SCREENRENDER_H

#include "VideoDef.h"
#include "VideoState.h"

typedef struct ScreenRender;

//��仺����
typedef int (*PFill_Picture)(ScreenRender *sr,VideoPictureBuffer *vpr, AVFrame *src_frame, double pts, int64_t pos, int serial);

//��Ƶ���ƺ���
typedef void (*PVideo_Display)(VideoState *is,
	ScreenRender *sr,
	SubtitleBuffer *sb,		//��Ļ������
	VideoPictureBuffer *vpb, //��Ƶͼ�񻺳���
	SynchronizeClockState *scs);

//��ƵƵ�׻���
typedef void (*PAudio_Sample_Display)(VideoState *is,
	ScreenRender *sr,
	SynchronizeClockState *scs);

typedef struct RenderFunc
{
	PFill_Picture fill_picture;
	PVideo_Display video_display;
	PAudio_Sample_Display audio_sample_display;
}RenderFunc;

//��Ļ��Ⱦ
typedef struct ScreenRender
{
	PFill_Picture fill_picture;
	PVideo_Display video_display;
	PAudio_Sample_Display audio_sample_display;
	void *Render;			//�û���Ⱦ����
	
	double last_vis_time;	//������Ƶ��������ʱ��,0.0
	int force_refresh;		//ǿ��ˢ��,0
	int display_disable;	//�ر���ʾ,0
}ScreenRender;

#endif