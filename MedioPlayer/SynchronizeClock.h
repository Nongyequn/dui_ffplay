#ifndef SYNCHRONIZECLOCK_H
#define SYNCHRONIZECLOCK_H

#include "ffmpeg.h"
#include "SynchronizeClockDef.h"
#include "AudioDef.h"
#include "VideoState.h"

//��ʼ��ͬ����
SynchronizeClockState *Create_Syschronize_Clock(int av_sync_type,VideoState *vs,PPosCallback cb,void *usr);
void Free_Syschronize_Clock(SynchronizeClockState** is);

//��ʼ����Ƶͬ����,������Ƶͬ��������ʱ��
AudioSynchronize *CreateAudioSynchronize(SynchronizeClockState *sc,double audio_diff_threshold);
void InitAudioSynchronize(AudioSynchronize * is,SynchronizeClockState *sc,double audio_diff_threshold);
//ȡ����
void UninitAudioSynchronize(AudioSynchronize *is);
void FreeAudioSynchronize(AudioSynchronize **pis);
//��ʼ����Ƶͬ����
VideoSynchronize *CreateVideoSynchronize(SynchronizeClockState *sc,double max_frame_duration);
void InitVideoSynchronize(VideoSynchronize * vs,SynchronizeClockState *sc,double max_frame_duration);
void UninitVideoSynchronize(VideoSynchronize *is);
void FreeVideoSynchronize(VideoSynchronize **pis);

//��ȡͬ����ʽ
int get_master_sync_type(SynchronizeClockState *is);

//��ȡ��ʱ��ʱ��
/* get the current master clock value */
double get_master_clock(SynchronizeClockState *is);

/* return the wanted number of samples to get better sync if sync_type is video
 * or external master clock */
//ͬ����Ƶ������ʱ��
int synchronize_audio(SynchronizeClockState *scs,int nb_samples,struct AudioParams audio_src);
//������ƵPTS��
void update_video_pts(SynchronizeClockState *scs, double pts, int serial);
//��ͣ��������Ƶ
//����ֵΪ�Ƿ���ͣ
int sc_stream_toggle_pause(SynchronizeClockState *is,int paused,int read_pause_return);

//ƥ����Ƶ�ӳ�,���Լ�����Ƶ��ʾ��ȴ�ʱ��
double compute_target_delay(double delay,double max_frame_duration, SynchronizeClockState *scs);
//������Ƶͬ��ʱ��
/*
������Ƶ֡ʱ��
tb:����ʱ��
audio_serial:�Ƿ���������Ƶ����
*/
void updata_audio_frame_clock(SynchronizeClockState * scs,AVFrame *frame,AVRational tb,int audio_serial);
//����ͬ����Ƶʱ��
void update_audio_clock(SynchronizeClockState *scs,int audio_write_buf_size,int audio_hw_buf_size,int bytes_per_sec,int64_t audio_callback_time);
//��鲢������ʱ���ٶ�
void check_external_clock_speed(VideoState *is,SynchronizeClockState *scs);
//�����ⲿʱ������
void set_extclk_clock(int seek_flags,int64_t seek_target,SynchronizeClockState *scs);

#endif