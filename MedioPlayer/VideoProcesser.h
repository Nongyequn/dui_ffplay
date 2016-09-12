#ifndef VIDEOPROCESSER_H
#define VIDEOPROCESSER_H

#include "ffmpeg.h"
#include "MSDL.h"
#include "VideoState.h"
#include "ScreenRender.h"
#include "SynchronizeClockDef.h"

/*
����:����������Ƶ������ʾ���ݻ�����
*/

typedef struct VideoProcesser
{
	VideoState *is;
	SynchronizeClockState *scs;
	VideoPictureBuffer *vpb;
	VideoSynchronize *vs;
}VideoProcesser;

VideoProcesser *CreateVideoProcesser(double max_frame_duration);
void InitVideoProcesser(VideoProcesser *vp,VideoState *is,SynchronizeClockState *scs);
void UninitVideoProcesser(VideoProcesser * vp);
void FreeVideoProcesser(VideoProcesser ** pvp);

/*
/����:������Ƶ��������
vp��VideoProcesser. �ļ�������
sr����Ⱦ���ߣ������ڲ�
��ע���ڲ�����סһֱ�����ļ������������ݣ������Ҫ�ⲿ���̴߳���
*/
int video_process_packet(VideoProcesser *vp,ScreenRender *sr);

#endif