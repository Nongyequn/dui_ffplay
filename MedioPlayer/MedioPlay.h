#ifndef MEDIOPLAY_H
#define MEDIOPLAY_H
#include <string>
#include "ffmpeg.h"
#include "Streamcomponent.h"
#include "packet_queue.h"
#include "VideoState.h"
#include "AudioRender.h"
#include "VideoRender.h"

class MedioPlay
{
public:
	MedioPlay();
	~MedioPlay();
	int Open(const char* FileName);
	void Play();
	void Pause();
	void Stop();
	bool IsPlaying();
	bool IsPause();
	bool IsStop();
	//��ȡ��Ƶʱ��
	int64_t GetTimes();//��λ��
	//������Ծ����POS
	void AdjustPost(int64_t nPos);
	//���ڵ���ָ��POS
	void SetPos(int64_t nPos);
	void SetPosCallback(PPosCallback cb,void *usr);
	//������Ļ��С
	void Resize(int width,int height);
	//�󶨴���
	void BindWindow(HWND id);
	//�������
	AudioRender *GetAudioRender(){return m_pRender;}

	int ThreadReadFile();
	int PosCall(int64_t pos);
private:
	void RePlay();
private:
	std::string m_strName;
	bool m_isPlaying;
	bool m_isPause;
	bool m_isStop;
private:
	VideoState *m_is;
	SynchronizeClockState *m_scs;
	StreamConfigure m_sc;

	AudioRender *m_pRender;
	VideoRender *m_pVideo;
	HANDLE m_hThread;

	int64_t m_nCurPos;
	HWND m_hwnd;
	PPosCallback m_pcb;
	void *m_usr;
};



#endif