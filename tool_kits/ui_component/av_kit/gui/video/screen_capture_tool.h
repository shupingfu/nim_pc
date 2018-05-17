#pragma once
#include "./helper/screen_capture.h"
#include "./helper/yuv_image.h"

namespace nim_comp
{
#define kScreenDefWidth		1280
#define kScreenDefHeight	720
#define kCameraDefWidth		640
#define kCameraDefHeight	480
	enum CaptureType
	{
		kCaptureTypeCamera = 0,
		kCaptureTypeScreen,
		kCaptureTypeScreenAndCamera,
	};
	//��ͼ����
	enum
	{
		NOTRACK = 0,
		SIZEALL = 1 << 3,
		SIZELEFT = 1 << 4,
		SIZERIGHT = 1 << 5,
		SIZETOP = 1 << 6,
		SIZEBOTTOM = 1 << 7,
		SIZENE = 1 << 8,
		SIZENW = 1 << 9,
		SIZESE = 1 << 10,
		SIZESW = 1 << 11,
	};

	/** @class ScreenCaptureTool
	  * @brief ¼������
	  * @copyright (c) 2016, NetEase Inc. All rights reserved
	  * @date 2016/09/21
	  */
	class ScreenCaptureTool : public ui::WindowImplBase
	{
	public:
		ScreenCaptureTool();
		~ScreenCaptureTool();

		//�����麯��
		virtual std::wstring GetSkinFolder() override;
		virtual std::wstring GetSkinFile() override;
		virtual std::wstring GetWindowClassName() const override;
		virtual UINT GetClassStyle() const override;

		/**
		* ���ڳ�ʼ������
		* @return void	�޷���ֵ
		*/
		virtual void InitWindow() override;

		/**
		* ���ز�����ײ㴰����Ϣ
		* @param[in] uMsg ��Ϣ����
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @return LRESULT ������
		*/
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		/**
		* ���ز������ڹر���Ϣ
		* @param[in] uMsg ��Ϣ����
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in] bHandled �Ƿ���Ҫ����������Ϣ��true�򲻴���
		* @return LRESULT ������
		*/
		virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;

		/**
		* ���ز���������������
		* @param[in] uMsg ��Ϣ����
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in] bHandled �Ƿ���Ҫ����������Ϣ��true�򲻴���
		* @return LRESULT ������
		*/
		virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

		/**
		* ���ز���������ɿ���Ϣ
		* @param[in] uMsg ��Ϣ����
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in] bHandled �Ƿ���Ҫ����������Ϣ��true�򲻴���
		* @return LRESULT ������
		*/
		virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

		/**
		* ���ز���������ƶ���Ϣ
		* @param[in] uMsg ��Ϣ����
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in] bHandled �Ƿ���Ҫ����������Ϣ��true�򲻴���
		* @return LRESULT ������
		*/
		virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

		/**
		* �������пؼ���������Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool Notify(ui::EventArgs* msg);

		/**
		* �������пؼ�������Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnClicked(ui::EventArgs* msg);

		/**
		* ������Ƶ֡������
		* @param[in] mng ��Ƶ֡������ָ��
		* @return void	�޷���ֵ
		*/
		void SetVideoMng(VideoFrameMng* mng) { video_frame_mng_ = mng; }

		/**
		* ��ʼ¼��
		* @param[in] type ¼������
		* @param[in] id ¼��Ŀ�괰�ڵľ��
		* @param[in] time_ms ¼�Ƽ��ʱ��
		* @return void	�޷���ֵ
		*/
		void StartCapture(CaptureType type, HWND id, int time_ms);

		/**
		* ����¼������
		* @param[in] type ¼������
		* @return void	�޷���ֵ
		*/
		void SetCaptureType(CaptureType type);

		/**
		* ����¼��Ŀ�괰��
		* @param[in] id �������ڵľ��
		* @param[in] cut �Ƿ�¼��Ŀ�괰���е�һ����
		* @return void	�޷���ֵ
		*/
		void SetCaptureWnd(HWND id, bool cut);

		/**
		* ����logo����
		* @param[in] text logo����
		* @return void	�޷���ֵ
		*/
		void SetTextLogo(std::wstring text);

	private:
		/**
		* �������в��Խ�����ù��
		* @param[in] hit_type ���в��Խ��
		* @return void	�޷���ֵ
		*/
		void SetHitTestCursor(int hit_type);

		/**
		* ������������ж����в��Խ��
		* @param[in] pt �������
		* @return int ���в��Խ��
		*/
		int CheckPtType(POINT pt);

		/**
		* ����ѡ����С
		* @param[in] pt ������ƫ����
		* @return void	�޷���ֵ
		*/
		void ResizeTrackRect(POINT& pt);

		/**
		* ֹͣ¼��
		* @return void	�޷���ֵ
		*/
		void StopCapture();

		/**
		* ��ȡһ֡ͼ��
		* @return void	�޷���ֵ
		*/
		void MakeFrame();

		void CopyI420Data(int type, std::string& dest, const std::string& src, int width, int height, ImageYuvDataType image_type = kYuvDataTypeNormal, POINT pt = { 0, 0 });

	public:
		static const LPTSTR kClassName;

	private:
		nbase::NLock lock_;
		nbase::WeakCallbackFlag capture_timer_;
		int width_;
		int height_;
		RECT track_rect_;
		//��ͼ״̬
		int track_state_;
		ui::Box* track_box_;
		ui::Control* track_NW_;
		ui::Control* track_T_;
		ui::Control* track_NE_;
		ui::Control* track_L_;
		ui::Control* track_R_;
		ui::Control* track_SW_;
		ui::Control* track_B_;
		ui::Control* track_SE_;

		//��ͼ����
		int screen_width_;
		int screen_height_;
		int screen_x_;
		int screen_y_;

		POINT point_offset_;

		ScreenCapture capture_;
		std::vector<YuvImage> yuv_image_list_;
		CaptureType capture_type_;
		nim_comp::VideoFrameMng* video_frame_mng_;
	};
}