#ifndef AUDIODECODER_H
#define AUDIODECODER_H

/*
����:��Ƶ���뺯��
*/

#include "ffmpeg.h"
#include "AudioDef.h"

typedef struct AudioPacketDecode
{
	AVCodecContext *codec;
	AVRational time_base;//��ʱ��

						//ͬ������Ĳ���
	AVRational tb;		//0
	int64_t audio_frame_next_pts;//��һ��PTS
	

	int audio_buf_frames_pending;//��Ƶ������������0
}AudioPacketDecode;

struct AudioPacketDecode *InitDecoder(AVStream *audio_st);
void UninitDecoder(struct AudioPacketDecode **pdec);
/*
����:����������
*/
int BeginDecoder(struct AudioPacketDecode *dec,int64_t pts);
/*
����:������һ֡�ʵ�PTS
*/
int SetNextPts(struct AudioPacketDecode *dec,int64_t pts);
/*
����:������Ƶ��
����ֵ:<0 �˳�,==0������>0��ȷ���
*/
int DecodePacket(AudioPacketDecode *is,AVPacket *pkt_temp,AVFrame *frame,struct AudioParams audio_src);

#endif