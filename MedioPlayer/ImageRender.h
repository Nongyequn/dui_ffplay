#ifndef IMAGERENDER_H
#define IMAGERENDER_H

/*
����:����VideoPicture->bmpͼ����SDL��Ļ������
*/


#include "ffmpeg.h"
#include "SDLRenderParam.h"
#include "VideoDef.h"

//��ʼ������ģ��
int init_image_render(ImageRenderInfor *iri,int w,int h,int flags,int force_set_video_mode);
//����ͼƬ�ڴ�
int alloc_videopicture(ImageRenderInfor *iri,VideoPicture *vp);
//������Ƶ����Ļ
void video_image_draw(ImageRenderInfor *iri,VideoPicture *vp,SubPicture *sp,int force_refresh);
//ͨ��ת��ģ�齫AVFrameͼ�����ݸ��Ƶ�VideoPicture��
int video_copy_picture(struct SwsContext **pimg_convert_ctx,AVFrame *src_frame,VideoPicture *vp);

#endif