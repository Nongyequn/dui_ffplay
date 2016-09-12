#ifndef AUDIOPROCESSER_H
#define AUDIOPROCESSER_H

#include "VideoState.h"
#include "AudioDecoder.h"
#include "AudioResample.h"

typedef struct DecodeBuffer
{
	int64_t pts;
	uint8_t *audio_buf;
	unsigned int audio_buf_size;
}DecodeBuffer;

/* SDL audio buffer size, in samples. Should be small to have precise
   A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

typedef struct AudioQueueDecode
{
	AVPacket audio_pkt_temp;
	AVPacket audio_pkt;

	int audio_pkt_temp_serial;

	AVFrame *frame;
}AudioQueueDecode;

typedef struct AudioProcesser{
	VideoState *vs;
	SynchronizeClockState *scs;

	AudioPacketDecode *dec;
	AudioQueueDecode *qdec;
	AudioSampleParam *pasp;//�ز���

	DecodeBuffer outbuffer;
	int audio_buf_index;

	uint8_t silence_buf[SDL_AUDIO_BUFFER_SIZE];
}AudioProcesser;

//audio_params:�򿪵���Ƶ�豸����
AudioProcesser *CreateAudioProcesser(AVStream *audio_st,struct AudioParams audio_params);
void FreeAudioProcesser(AudioProcesser **is);

//�����Ƶ�豸������
void FillAudioDeviceBuffer(AudioProcesser *ap,struct AudioParams ap_render,int audio_buf_size,Uint8 *stream, int len,int volume);
//void FillAudioDeviceBuffer(AudioProcesser *ap,struct AudioParams ap_render,int audio_buf_size,Uint8 *stream, int len);


#endif