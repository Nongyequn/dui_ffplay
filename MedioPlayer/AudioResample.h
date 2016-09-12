#ifndef AUDIORESAMPLE_H
#define AUDIORESAMPLE_H

#include "AudioDef.h"

typedef struct AudioDataBuffer
{
	uint8_t *audio_buf;
	unsigned int audio_buf_size;
}AudioDataBuffer;

typedef struct AudioSampleParam
{
	struct AudioParams audio_src;//��ǰ�ز���������Ƶ����,
	struct SwrContext *swr_ctx;//�ز�����
	uint8_t *audio_buf1;
    unsigned int audio_buf1_size;
}AudioSampleParam;

//��Ƶ�ز���
//����:��Ƶ��������ʹ�õĲ���
struct AudioSampleParam * AR_InitAudioSample(struct AudioParams audio_src);
void AR_UninitAudioSample(struct AudioSampleParam **pasp);

struct AudioParams AR_GetAudioParams(struct AudioSampleParam *pasp);
//����Ƿ���Ҫ�ز���
//audio_tgt:��Ҫ�ز����ﵽ��Ŀ����Ƶ����
//-1:����0 ���裬1 ��Ҫ�����
int AR_ChechAudioSamle(struct AudioSampleParam *pasp,AVFrame *frame,struct AudioParams audio_tgt,int wanted_nb_samples);
//��Ƶ�ز���
int AR_AudioResample(struct AudioSampleParam *pasp,AVFrame *frame,struct AudioParams audio_tgt,int wanted_nb_samples,AudioDataBuffer *pout);

#endif