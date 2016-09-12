#ifndef SYNCHRONIZECLOCKDEF_H
#define SYNCHRONIZECLOCKDEF_H

#include "ffmpeg.h"
#include "AudioDef.h"

typedef struct Clock {
    double pts;           /* clock base����ǰ֡PTS,��λ��*/
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed;
    int serial;           /* clock is based on a packet with this serial */
    int paused;
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
} Clock;

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20
/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

typedef struct SynchronizeClockState;

//��Ƶ����ʱ���ڼ���ͬ���Ĳ���
typedef struct AudioSynchronize
{
	double audio_diff_cum; /* used for AV difference average computation */
	double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;

	int64_t audio_current_pos;
	SynchronizeClockState *sc;
}AudioSynchronize;

//��Ƶ����ʱ���ڼ���ͬ���Ĳ���
typedef struct VideoSynchronize
{
	//��Ƶ
	double frame_timer;			//��ǰ֡����ʱ���,Ĭ��0
	double max_frame_duration;	//֡�������,�����㷽ʽ:(AVFormatContext->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;
	int step;					//Ĭ��0,
	int decoder_reorder_pts;	//Ĭ�� -1;���Ž���pts

	int64_t video_current_pos;
	double frame_last_pts;
	double frame_last_duration;

	double frame_last_filter_delay;
	int64_t frame_last_dropped_pos;
	double frame_last_dropped_pts;//AV_NOPTS_VALUE
	int frame_last_dropped_serial;
	int frame_drops_early;

	SynchronizeClockState *sc;
}VideoSynchronize;

enum {
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

typedef void (*PPosCallback)(void *usr,int64_t pos,int64_t max);

typedef struct SynchronizeClockState
{
	int av_sync_type;			//ʱ��ͬ����ʽ,Ĭ��0
	int show_mode;

	Clock audclk;
    Clock vidclk;
    Clock extclk;
	//���ڱ�ʾ�Ƿ������Щ����Ĭ��:-1
	int audio_stream;
	int subtitle_stream;
	int video_stream;

	//��Ƶ
	double audio_clock;			//Ĭ��0
	int audio_clock_serial;		//Ĭ��-1

	int audio_write_buf_size;	//��Ƶ���ݴ�С,Ĭ��0
	AudioSampleArray sample_array;//��Ƶ����,������Ƶ��Ⱦ����Ƶ��


	AudioSynchronize *as;		//������Ƶͬ��������ʱ��
	VideoSynchronize *vs;		//������Ƶ�����¼ͬ������

	//���ڻص���ǰ��Ⱦλ��
	PPosCallback cb;
	void *usr;
}SynchronizeClockState;

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
#define EXTERNAL_CLOCK_SPEED_MIN  0.900
#define EXTERNAL_CLOCK_SPEED_MAX  1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001


#endif