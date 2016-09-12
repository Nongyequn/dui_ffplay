#ifndef AUDIOSAMPLEDRAW_H
#define AUDIOSAMPLEDRAW_H

/*
����:������Ƶ����Ƶ������ͼ
*/

#include "ffmpeg.h"
#include "MSDL.h"
#include "AudioDef.h"
#include "SDLRenderParam.h"

//
//audio_tgt:��Ƶ����������
//audio_callback_time:��Ƶ���ݻص�ʱ��
//s:����
//channels:ͨ����
//audio_write_buf_size:��Ƶд��ĳ���
//bShowWAVE���Ƿ���WAVE��ʽ��ʾ
int Get_I_Start(ImageRenderInfor *iri,AudioSampleSpikes *pSampleSpikes,
	struct AudioParams audio_tgt,int channels,int64_t audio_callback_time,
	AudioSampleArray *s,int audio_write_buf_size,bool bShowWAVE);

int AudioSampleWAVE(ImageRenderInfor *iri,AudioSampleSpikes *pSampleSpikes,int channels,AudioSampleArray *s,int audio_write_buf_size);

int AudioSampleFFTS(ImageRenderInfor *iri,AudioSampleSpikes *pSampleSpikes,FFTSRenderInfor *fri,int channels,AudioSampleArray *s,int audio_write_buf_size);

//������Ƶ�����������ڻ���Ƶ��
/* copy samples for viewing in editor window */
void update_sample_display(AudioSampleArray *asa, short *samples, int samples_size);

#endif