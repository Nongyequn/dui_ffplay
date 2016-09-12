#ifndef VIDEOSTATE_H
#define VIDEOSTATE_H

#include <string>
#include "ffmpeg.h"
#include "packet_queue.h"
#include "SynchronizeClockDef.h"

#include "Streamcomponent.h"

//���л������ֽ����ռ�
#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
//��С��ȡ����
#define MIN_FRAMES 5

//��Ƶ������ʾģʽ
enum ShowMode {
	SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
};

typedef struct VideoState{
	enum ShowMode show_mode;

	int64_t audio_callback_time;//��Ƶ���ݻص�ʱ��
	int realtime;				//�Ƿ�Ϊʵʱ��

	int abort_request;			//��ֹ
	int paused;					//��ͣ
	int last_paused;			//���һ�ε���ͣ��ֵ
	int read_pause_return;		//��ͣ�������	

	int seek_req;				//�����л���λ��
	int64_t seek_pos;			//��ȡλ�ã�PTS
	int64_t seek_rel;			//��ʵ��ȡ��λ�ã��������ʵ�¼�
	int seek_flags;				//ƫ���ļ�ʹ�õ�ģʽ��AVSEEK_FLAG_BYTE
	int eof;					//�ļ���ȡ����β
	AVFormatContext *ic;		//�ļ�������

	//����
	//���ݻ�����
	PacketQueue videoq;			//��Ƶ���ݻ�����
	PacketQueue audioq;			//��Ƶ���ݻ�����
	PacketQueue subtitleq;		//��Ļ���ݻ�����

	AVStream *audio_st;//�������
	AVStream *video_st;//�������
	AVStream *subtitle_st;//�������

	int audio_stream;			//�Ƿ�����Ƶ����-1Ϊû�У�>= 0Ϊ��
	int subtitle_stream;		//�Ƿ�����Ļ����-1Ϊû�У�>= 0Ϊ��
	int video_stream;			//�Ƿ�����Ƶ����-1Ϊû�У�>= 0Ϊ��
}VideoState;

typedef struct StreamConfigure
{
	int seek_by_bytes;		//���ö�ȡ�ֽڷ�ʽ
	int64_t start_time;		//����ʱ��
	int st_index[AVMEDIA_TYPE_NB];
	int video_disable;		//��ֹ��Ƶ
	int audio_disable;		//��ֹ��Ƶ
	int subtitle_disable;	//��ֹ��Ļ
	Audio_Param audiopm;	//��Ƶ����

	int64_t duration;	//

	int loop;			//ѭ������
	int autoexit;		//���������Զ���ֹ
	int infinite_buffer;	//�����ڴ�ģʽ
}StreamConfigure;

//��ȡ������
void InitStreamConfigure(StreamConfigure *is);
void UnitStreamConfigure(StreamConfigure **pis);

//��ʼ����Ƶ����
VideoState *CreateVideoState();
//�ͷŽṹ��
void FreeVideoState(VideoState **is);
//���ļ�����ʼ��VideoState
int OpenStream(VideoState *is,std::string strName,StreamConfigure *cf);
//��ʼ�����ļ���
int LoadStreams(VideoState *is,SynchronizeClockState *scs,StreamConfigure *cf);
//��ֹ��
int AbortStream(VideoState *is);
//��������ȡλ�ã�nPos:��λ��
void seek_stream_pos(VideoState *is,int64_t nPos);
//ǰ��POS
void seek_pos(VideoState *is,SynchronizeClockState *scs,double incr);

#endif