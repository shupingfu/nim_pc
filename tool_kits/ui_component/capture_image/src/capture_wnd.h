#ifndef NIM_WIN_GUI_CAPTURE_WND_H_
#define NIM_WIN_GUI_CAPTURE_WND_H_

#include "capture_def.h"
#include "ui_capture_edit.h"
#include "ui_color_palette.h"

#define DeTrackState(n) ((n) & 0x03)
#define DeSizeState(n) ((n) & 0xFFFFFFFC)

class CCaptureEdit;
class CaptureWindow : public ui::WindowImplBase
{
public:
	CaptureWindow(const std::wstring& user_data_dir);
	virtual ~CaptureWindow();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	//virtual std::wstring GetWindowId(void) const;
	virtual std::wstring GetWindowClassName() const override { return _T("CaptureWindow"); };
	virtual UINT GetClassStyle() const override { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	virtual void OnFinalMessage(HWND hWnd) override;
	virtual void InitWindow() override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	bool Notify(ui::EventArgs* msg);

	//�����¼�
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	std::wstring GetImagePath(){return track_image_path_;}

	void SetSendInfo(std::wstring send_info); // ������ʾ�ı�

	static BOOL SaveBitmapToFile(HBITMAP hBitmap, const std::wstring &file_path);

private:
	int GetTrackState() { return track_state_; }

	//����
	void DrawTrack(RECT track_rect);

	//��ʾ������
	void ShowToolBar(bool edit = false);

	//��ʾ��ͼ��С��Ϣ
	void ShowSizeTip();

	//�����ȡ������
	BOOL SaveTrackImage(bool save_to_clipboard = true);

	//�ղؽ�ȡ������

	// ��ʼ��λͼ
	BOOL InitBitmaps();

	void NormalizeRect(RECT& rect);

	void SetTrackState(int track_state) { track_state_ = track_state; } // ����Tracker��״̬

	RECT ClipTrackRect(RECT rect);//�ü��ڿͻ����Ŀɽ�ͼ����

	RECT GetToolBarRect();//����������
	RECT GetToolBar2Rect();//����������

	RECT GetSizeTipRect();//��С��Ϣ��ʾ����

	RECT GetDrawTrackRect(int hit_test);//������Ԫ������

	// ��Tracker����HitTest
	int TrackerHitTest(RECT rect, POINT point);

	// ���ڵ��ڵ㣨�ߣ��ϵ��ڵ�ʱ������λ��
	void AdjustOffset(RECT& rcTracker, POINT point_dest, POINT point_offset, int hit_test);

	// ����HitTest���ȡ����ȷ�Ĺ����
	void SetHitTestCursor(int hit_test);

	// ���õ�ǰ��ͼ�༭��״̬
	void SetCaptureEditMode(EditMode mode);
	EditMode GetCaptureEditMode() { return this->capture_edit_mode_; }
	void ResetCaptureEditMode(); // ���ñ༭״̬������ϴα༭����

	// �����ͼ���û�ָ���ĵ�ַ
	void Save();

	void SaveTrackImageDlgCallback(BOOL ret, std::wstring file_path);

private:
	enum
	{
		NOTRACK = 0,
		TRACKING = 1,
		TRACKED = 3,
		SIZEALL			= 1<<3,
		SIZELEFT		= 1<<4,
		SIZERIGHT		= 1<<5,
		SIZETOP			= 1<<6,
		SIZEBOTTOM		= 1<<7,
		SIZENE			= 1<<8,
		SIZENW			= 1<<9,
		SIZESE			= 1<<10,
		SIZESW			= 1<<11,
		SIZELEFT_LINE	= 1<<12,
		SIZERIGHT_LINE	= 1<<13,
		SIZETOP_LINE	= 1<<14,
		SIZEBOTTOM_LINE	= 1<<15
	};
	RECT track_rect_;
	//��ͼ״̬
	int track_state_;

	//��ͼ����
	int screen_width_;
	int screen_height_;
	int screen_x_;
	int screen_y_;

	POINT point_offset_;

	//����ͼ
	HBITMAP bitmap_bg_;
	std::wstring track_image_path_;
	std::wstring send_info_;

	int toolbar_min_width_;
	int toolbar_min_width2_;

	ui::Box* toolbar_;
	// ��ͼ�༭״̬
	EditMode capture_edit_mode_;
	CCaptureEdit *capture_edit_;
	std::vector<std::wstring> vec_option_names_;
	CColorPalette* color_palette_;
	bool is_show_color_palette_;
	int32_t desktop_scale_;//�ٷ���
	std::wstring user_data_dir_;
};

#endif // NIM_WIN_GUI_CAPTURE_WND_H_
