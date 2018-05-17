#pragma once
#include "module/video/video_frame_mng.h"

namespace nim_comp
{
struct CaptureWindowInfo 
{
	HWND id;
	std::wstring title;
};
typedef std::vector<CaptureWindowInfo> CaptureWindowInfoList;

/** @class ScreenCapture
  * @brief ��Ļ¼�ƿؼ�
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/09/21
  */
class ScreenCapture : public virtual nbase::SupportWeakCallback
{
public:
	/**
	* ö�����п�¼�ƵĴ��ڵ��б�
	* @param[out] windows ��ȡ�����б�
	* @return bool true ö�ٳɹ���false ö��ʧ��
	*/
	static bool GetCaptureWindowList(CaptureWindowInfoList* windows);

	/**
	* ��ȡϵͳDPI����ֵ
	* @return int DPIֵ
	*/
	static int GetDesktopScale();

	ScreenCapture();
	~ScreenCapture();

	/**
	* ��ʼ¼��
	* @param[in] id ��¼�ƵĴ��ڵľ��
	* @param[in] 
	* @return void	�޷���ֵ
	*/
	void StartCapture(HWND id, int time_ms=60);

	/**
	* ����¼�Ʒ�Χ
	* @param[in] cut �Ƿ�¼�ƴ����е�һ����
	* @param[in] rect ¼�Ʒ�Χ
	* @return void	�޷���ֵ
	*/
	void SetCutMode(bool cut, RECT rect);

	/**
	* ��ȡһ֡λͼ
	* @return void	�޷���ֵ
	*/
	void CustomFrame();

	/**
	* ��ȡ��Ƶ֡������
	* @return VideoFrameMng* ��Ƶ֡������
	*/
	VideoFrameMng* GetFrame() { return &frame_; }
private:
	/**
	* ��ʼ���ڴ�λͼ
	* @param[in] width λͼ���
	* @param[in] height λͼ�߶�
	* @return void	�޷���ֵ
	*/
	void InitHBitmap(int width, int height);
private:
	HBITMAP capture_bitmap_;
	HWND	capture_hwnd_;
	char* capture_data_;
	int capture_width_;
	int capture_height_;
	RECT cut_rect_;
	bool cut_screen_;
	VideoFrameMng frame_;

};
}