#ifndef SDLRENDERPARAM_H
#define SDLRENDERPARAM_H

#include "MSDL.h"
#include "ffmpeg.h"
#include "AudioDef.h"

typedef struct FFTSRenderInfor{
	RDFTContext *rdft;
	int rdft_bits;
    FFTSample *rdft_data;
	int xpos;
}FFTSRenderInfor;

typedef struct AudioSampleSpikes
{
	int i_start;
	int h;
}AudioSampleSpikes;

//��Ƶ������Ⱦ
typedef struct AudioSampleParams
{
	//��Ƶ���
	struct AudioParams audio_tgt;
	FFTSRenderInfor fftsri;//��Ƶffts��Ⱦģ��
	AudioSampleSpikes ass; //��Ƶ��Ⱦ����
}AudioSampleParams;


typedef struct ImageRenderInfor
{
	int flush_screen;
	int width, height, xleft, ytop;
	SDL_Surface *screen;
	SDL_Rect last_display_rect;
}ImageRenderInfor;

typedef struct UserScreenRender
{
	int fs_screen_width;
	int fs_screen_height;
	int default_width;
	int default_height;
	int screen_width;
	int screen_height;
	int is_full_screen;

	ImageRenderInfor iri;//������Ⱦģ��

#if !CONFIG_AVFILTER
    struct SwsContext *img_convert_ctx;
#endif

	AudioSampleParams asr;  //��Ƶ�������Ʋ���
}UserScreenRender;


#endif