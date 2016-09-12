#ifndef SDLDRAWVIDEO_H
#define SDLDRAWVIDEO_H 

#include "ScreenRender.h"
#include "VideoState.h"
#include "SynchronizeClockDef.h"

//��src_frame���ݱ�����VideoPictureBuffer��������
int fill_picture(ScreenRender *sr,VideoPictureBuffer *vpr, AVFrame *src_frame, double pts, int64_t pos, int serial);

//����ͼ��
/* display the current picture, if any */
void video_display(VideoState *is,
	ScreenRender *sr,
	SubtitleBuffer *sb,		//��Ļ������
	VideoPictureBuffer *vpb, //��Ƶͼ�񻺳���
	SynchronizeClockState *scs);

//��ƵƵ�׻���
void audio_sample_display(VideoState *is,
	ScreenRender *sr,
	SynchronizeClockState *scs);


void toggle_audio_display(VideoState *is,ScreenRender *sr);
//ȫ���л�
void toggle_full_screen(ScreenRender *is);


static RenderFunc g_sdlRender = 
{
	fill_picture,
	video_display,
	audio_sample_display
};

#include "SDLRenderParam.h"

UserScreenRender *CreateUserScreenRender(int width,int height);
void InitUserScreenRender(UserScreenRender * usr,int width,int height);
void FreeUserScreenRender(UserScreenRender** usr);
#endif