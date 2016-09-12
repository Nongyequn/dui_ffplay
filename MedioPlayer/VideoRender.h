#ifndef VIDEORENDER_H
#define VIDEORENDER_H

#include "VideoPictureRender.h"
#include "SDLRenderParam.h"

class VideoRender
{
public:
	VideoRender(HWND hwnd);
	~VideoRender();
	int Pause(int nPause);		//��ͣ����
	void Stop();				//ֹͣ����
	void WaitStop();
	int Play(VideoState *is,SynchronizeClockState *scs);//������Ƶ
	void Resize(int width,int height);//�������Ŵ�С
	//��ȡ���Ż��������ݴ�С
	int GetVideoPQSize();
	//��ȡ��Ļ���������ݴ�С
	int GetSubtitlePQSize();
public:
	//�̻߳ص�����
	int SubtitleCallback();
	int VideoCallback();
	int VideoRenderCallback();
private:
	VideoState *m_is;
	SynchronizeClockState *m_scs;

	SubtitleBuffer* m_sb;
	ScreenRender *m_sr;
	VideoProcesser *m_vp;
	UserScreenRender * m_usr;

	SDL_Thread *m_subtitle_tid;
	SDL_Thread *m_video_tid;
	SDL_Thread *m_render_tid;
	HWND m_hwnd;
};

void RegisterWindow(HWND id);

#endif