#include "capture_wnd.h"
#include "capture_manager.h"
#include "ui_capture_edit.h"
#include "shared/modal_wnd/file_dialog_ex.h"
#include "base/file/file_util.h"
#include "base/util/string_util.h"
#include "base/time/time.h"
#include "base/win32/platform_string_util.h"

#include <atlimage.h>
#include <ShlObj.h>

using namespace nbase;
using namespace ui;

#define BORDER_SIZE	3
#define DOT_SIZE	9
#define TEXT_INFO_LENGTH	64

//#define MAX_IMAGE_PIXELS	4000000
//#define MAX_RATIO			3

static ui::Control* CaptureWindowCreateControl( const std::wstring& pstrClass )
{
	// ����XMLʱ�������Զ���Ľڵ�
	if (pstrClass == _T("CaptureEdit"))
	{
		return new CCaptureEdit();
	}
	else if(pstrClass == L"ColorBlock")
	{
		return new CColorBlock;
	}
	else if(pstrClass == L"BrushBlock")
	{
		return new CBrushBlock;
	}
	return NULL;
}

CaptureWindow::CaptureWindow(const std::wstring& user_data_dir)
{
	track_state_ = NOTRACK;
	track_rect_.left = 0;
	track_rect_.right = 0;
	track_rect_.top = 0;
	track_rect_.bottom = 0;
	bitmap_bg_ = NULL;
	track_image_path_ = L"";
	toolbar_min_width_ = 180;
	toolbar_min_width2_ = 180;
	capture_edit_ = NULL;
	capture_edit_mode_ = EM_NONE;
	color_palette_ = NULL;
	is_show_color_palette_ = false;
	desktop_scale_ = 100;
	user_data_dir_ = user_data_dir;
}

CaptureWindow::~CaptureWindow()
{
	if (bitmap_bg_)
	{
		DeleteObject(bitmap_bg_);
	}
}

//std::wstring CaptureWindow::GetWindowId(void) const
//{
//	return _T("CaptureWindow");
//}

std::wstring CaptureWindow::GetSkinFolder()
{
	return L"capture_image";
}

std::wstring CaptureWindow::GetSkinFile()
{
	return L"capture_wnd.xml";
}

void CaptureWindow::OnFinalMessage(HWND hWnd)
{
	CaptureManager::GetInstance()->EndCapture(/*csid_, */track_image_path_);
	__super::OnFinalMessage(hWnd);
}

// ������ʾ�ı�
void CaptureWindow::SetSendInfo(std::wstring send_info)
{
	if (send_info.size() == 0)
	{
		return;
	}
	ui::MutiLanSupport *multilan = ui::MutiLanSupport::GetInstance();
	if(multilan)
	{
		std::wstring send_info_temp;
		nbase::StringPrintf(send_info_temp, multilan->GetStringViaID(L"STRID_CAPTUREFORM_SENDINFO").c_str(), send_info.c_str());
		send_info = send_info_temp;
	}
	send_info_ = send_info;
	std::string check_string;
	check_string = nbase::UTF16ToUTF8(send_info.c_str(), send_info.size());
	if (check_string.size() + send_info_.length() > TEXT_INFO_LENGTH*2)
	{
		send_info_ = send_info_.substr(0, TEXT_INFO_LENGTH);
		while (check_string.size() + send_info_.length() > TEXT_INFO_LENGTH*2)
		{
			send_info_ = send_info_.substr(0, send_info_.length()-1);
			check_string = nbase::UTF16ToUTF8(send_info.c_str(), send_info_.length());
		}
		send_info_ += L"...";
	}
}

void CaptureWindow::InitWindow()
{
	m_pRoot->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&CaptureWindow::Notify, this, std::placeholders::_1));

	RECT rcWindow;

	screen_width_ = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	screen_height_ = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	screen_x_ = GetSystemMetrics(SM_XVIRTUALSCREEN);
	screen_y_ = GetSystemMetrics(SM_YVIRTUALSCREEN);

	rcWindow.left = screen_x_;
	rcWindow.top = screen_y_;
	rcWindow.right = screen_x_ + screen_width_;
	rcWindow.bottom = screen_y_ + screen_height_;

	if (!ui::DpiManager::GetInstance()->IsAdaptDPI())
	{
		int dpi_value = ui::DpiManager::GetInstance()->GetDPIFromRegistry();
		desktop_scale_ = dpi_value * 100 / 96;
	}

	// ��ͼ�༭�ؼ�
	capture_edit_ = static_cast<CCaptureEdit *>(FindControl(L"capture_edit"));
	ASSERT(capture_edit_);
	// 6��optionһ��
	vec_option_names_.push_back(L"rectangle_option");
	vec_option_names_.push_back(L"ellipse_option");
	vec_option_names_.push_back(L"line_option");
	vec_option_names_.push_back(L"brush_option");
	vec_option_names_.push_back(L"arrow_option");
	vec_option_names_.push_back(L"text_option");
	vec_option_names_.push_back(L"mosaic_option");
	vec_option_names_.push_back(L"gauss_option");
	toolbar_ = static_cast<Box *>(FindControl(L"toolbar_button"));

	// ��̬������ɫ��
	color_palette_ = new CColorPalette(); // ������һ������
	toolbar_->Add(color_palette_->CreateColorPaletteUI(nbase::Bind(&CaptureWindowCreateControl, std::placeholders::_1)));
	color_palette_->InitChildControls();
	color_palette_->SetVisible(false); // ��ʼ���ɼ�
	capture_edit_->SetColorPalette(color_palette_);

	Label *label = static_cast<Label *>(FindControl(L"send_info"));
	int label_width = 0;
	if (label && send_info_.size()>0)
	{
		label->SetText(send_info_.c_str());
		ui::CSize max_size(9999, 9999);
		ui::CSize sz = label->EstimateSize(max_size);
		label_width = sz.cx + 24;
		label->SetFixedWidth(label_width + 16);
	}
	Control *control = static_cast<Control *>(FindControl(L"toolbar_button"));
	if (control)
	{
		toolbar_min_width_ = label_width + control->GetFixedWidth();
	}
	Label *label2 = static_cast<Label *>(FindControl(L"send_info2"));
	if (label2 && send_info_.size()>0)
	{
		label2->SetText(send_info_.c_str());
		ui::CSize max_size(9999, 9999);
		ui::CSize sz = label2->EstimateSize(max_size);
		int label_width = sz.cx;
		toolbar_min_width2_ = label_width + 16;
	}
	Label *text = static_cast<Label *>(FindControl(L"tip_head"));
	if (text)
	{                   
		ui::MutiLanSupport *multilan = ui::MutiLanSupport::GetInstance();
		if(multilan)
		{
			text->SetText(multilan->GetStringViaID(L"STRID_CAPTUREFORM_SIZEINFO").c_str());
		}
		else
		{
			text->SetVisible(false);
		}
	}
	text = static_cast<Label *>(FindControl(L"tip_size"));
	if (text)
	{
		text->SetText(_T("(0*0)"));
	}
	
	InitBitmaps();
	ShowToolBar();
	ShowSizeTip();

	LONG windowLongEx = GetWindowLong( m_hWnd, GWL_EXSTYLE );
#ifdef DEBUG
	SetWindowLong( m_hWnd, GWL_EXSTYLE, windowLongEx | WS_EX_TOOLWINDOW );

	// ����ʱ��  HWND_NOTOPMOST�����껻���� HWND_TOPMOST
	SetPos(ui::UiRect(screen_x_, screen_y_, screen_x_ + screen_width_, screen_y_+screen_height_), false, SWP_NOACTIVATE, NULL);
#else
	SetWindowLong(m_hWnd, GWL_EXSTYLE, windowLongEx | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);

	// ����ʱ��  HWND_NOTOPMOST�����껻���� HWND_TOPMOST
	SetPos(ui::UiRect(screen_x_, screen_y_, screen_x_ + screen_width_, screen_y_ + screen_height_), false, SWP_NOACTIVATE, HWND_TOPMOST);
#endif

	DrawTrack(track_rect_);

	SetForegroundWindow(m_hWnd);
	SetActiveWindow(m_hWnd);
}
ui::Control* CaptureWindow::CreateControl(const std::wstring& pstrClass)
{
	return CaptureWindowCreateControl(pstrClass);
}

bool CaptureWindow::Notify(ui::EventArgs* msg)
{
	std::wstring name = msg->pSender->GetName();
	if (msg->Type == ui::kEventClick)
	{
		if (msg->pSender->GetName() == L"send_btn")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsDone");

			SaveTrackImage();
			Close();
			return true;
		}
		else if (msg->pSender->GetName() == L"cancel_btn")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsQuit");

			Close();
			return true;
		}
		else if(msg->pSender->GetName() == L"save_btn")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsSave");

			Save(); // ���浽�����ļ����Զ��˳�
		}
		else if(msg->pSender->GetName() == L"undo_btn")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsRevoke");

			capture_edit_->Undo(); // ����
		}
		else if(msg->pSender->GetName() == L"redo_btn")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsRegain");

			capture_edit_->Redo(); // ����
		}
	}
	else if(msg->Type == ui::kEventSelect)
	{
		bool start_paint = true;
		if(msg->pSender->GetName() == L"rectangle_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsBox");

			SetCaptureEditMode(EM_RECTANGLE); // ���ƾ���
		}
		else if(msg->pSender->GetName() == L"ellipse_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsRound");

			SetCaptureEditMode(EM_ELLIPSE); // ������Բ
		}
		else if(msg->pSender->GetName() == L"brush_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsBrush");

			SetCaptureEditMode(EM_BRUSH); // ���ƻ�ˢ
		}
		else if(msg->pSender->GetName() == L"line_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsLine");

			SetCaptureEditMode(EM_LINE); // ����ֱ��
		}
		else if(msg->pSender->GetName() == L"arrow_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsArrow");

			SetCaptureEditMode(EM_ARROW); // ���Ƽ�ͷ
		}
		else if (msg->pSender->GetName() == L"mosaic_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsMosaics");

			SetCaptureEditMode(EM_MOSAIC); // ����������
		}
		else if (msg->pSender->GetName() == L"gauss_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsGauss");

			SetCaptureEditMode(EM_GAUSS); // ����ë����
		}
		else if(msg->pSender->GetName() == L"text_option")
		{
			//da::DaSupport::GetInstance()->AddDaEvent(YX_TYPE_SESSION, YX_SESSIONCLICKSSTOOLBOX, L"SsText");

			SetCaptureEditMode(EM_TEXT); // �����ı�
		}
		else
			start_paint = false;

		// ���ý�ͼ������
		if (start_paint)
			capture_edit_->SetTrackRect(track_rect_); 
	}
	else if (msg->Type == ui::kEventMouseMenu)
	{
		switch (DeTrackState(track_state_))
		{
		case NOTRACK:
			Close();
			break;
		case TRACKED:
			ResetCaptureEditMode(); // ���ý�ͼ�༭״̬���ͷ��ϴεĻ��Ƶ�Ԫ
			SetTrackState(NOTRACK);
			ShowToolBar();
			AdjustOffset(track_rect_, point_offset_, point_offset_, NOTRACK);
			DrawTrack(track_rect_);
			SetHitTestCursor(NOTRACK);
			break;
		}
	}

	return true;
}

static std::wstring GetGUID()
{
	GUID guid;
	HRESULT hr = ::CoCreateGuid(&guid);
	if( SUCCEEDED(hr) )
	{
		return nbase::StringPrintf(L"%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x", guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
	else
	{
		assert(0);
		return L"";
	}
}

//�����ȡ������
//typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
BOOL CaptureWindow::SaveTrackImage(bool save_to_clipboard)
{
	// ����ı��༭��δ����
	if(capture_edit_mode_ == EM_TEXT && capture_edit_)
	{
		capture_edit_->OnTextEditFinished();
	}
	BOOL bResult = FALSE;
	if (bitmap_bg_ && DeTrackState(track_state_) == TRACKED)
	{
		RECT track_rect = ClipTrackRect(track_rect_);
		int width = track_rect.right - track_rect.left + 1;
		int height = track_rect.bottom - track_rect.top + 1;
		RECT client_rect;
		GetClientRect(m_hWnd, &client_rect);
		int x = track_rect.left - client_rect.left;
		int y = track_rect.top - client_rect.top;
		x = x * desktop_scale_ / 100;
		y = y * desktop_scale_ / 100;
		width = width * desktop_scale_ / 100;
		height = height * desktop_scale_ / 100;

		int image_save_width = width;
		int image_save_height = height;

		// ��ĻDC 
		HDC hScrDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		// ��Ļ����DC��ѡ���ͼ��ʼʱ�������Ļͼ��
		HDC hScrDC2 = CreateCompatibleDC(hScrDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hScrDC2,bitmap_bg_);
		// ���ϱ༭������
		if(capture_edit_)
			capture_edit_->DrawExtraUnits(hScrDC2);
		// ��Ļ����DC��Ŀ�����DC����ѡ���ͼ����ļ���λͼ
		HDC hMemDC = CreateCompatibleDC(hScrDC);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScrDC, image_save_width, image_save_height);
		HBITMAP hOldBitmap2 = (HBITMAP)SelectObject(hMemDC,hBitmap);
		BitBlt(hMemDC,0,0,width, height, hScrDC2, x, y,SRCCOPY);
		hBitmap = (HBITMAP)SelectObject(hMemDC,hOldBitmap2);
		(HBITMAP)SelectObject(hScrDC2,hOldBitmap);
		//���
		DeleteDC(hScrDC);
		DeleteDC(hScrDC2);
		DeleteDC(hMemDC);
		if(hBitmap)
		{
			//�����image\\uuid.png�ļ�
			std::wstring capture_path = user_data_dir_;
			capture_path += L"tmp\\";
			if (false == nbase::FilePathIsExist(capture_path, true))
			{
				CreateDirectory(capture_path);
			}
			
			capture_path += GetGUID() + L".png";
			bResult = SaveBitmapToFile(hBitmap, capture_path);
			if (bResult)
			{
				track_image_path_ = capture_path;
			}
			
			if (save_to_clipboard && OpenClipboard(NULL))
			{
				//��ռ�����
				EmptyClipboard();
				SetClipboardData(CF_BITMAP, hBitmap);

				if (track_image_path_.size() > 0)
				{
					std::string image_path;
					nbase::win32::UnicodeToMBCS(track_image_path_, image_path, CP_ACP);
					HGLOBAL hGblEffect;
					LPDWORD lpdDropEffect;
					DROPFILES stDrop;

					HGLOBAL hGblFiles;
					LPSTR lpData;

					hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,sizeof(DWORD));
					lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
					*lpdDropEffect = DROPEFFECT_COPY;//����; ��������DROPEFFECT_MOVE
					GlobalUnlock(hGblEffect);

					stDrop.pFiles = sizeof(DROPFILES);
					stDrop.pt.x = 0;
					stDrop.pt.y = 0;
					stDrop.fNC = FALSE;
					stDrop.fWide = FALSE;

					hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,\
						sizeof(DROPFILES)+strlen(image_path.c_str())+2);
					lpData = (LPSTR)GlobalLock(hGblFiles);
					memcpy(lpData,&stDrop,sizeof(DROPFILES));
					strcpy(lpData+sizeof(DROPFILES),image_path.c_str());
					GlobalUnlock(hGblFiles);

					SetClipboardData(CF_HDROP,hGblFiles);
				}
				//�رռ�����
				CloseClipboard();
			}

			DeleteObject(hBitmap);
		}
	}
	return bResult;
}

// ��ʼ��λͼ
BOOL CaptureWindow::InitBitmaps()
{
	BOOL bResult = FALSE;
	int x = screen_x_ * desktop_scale_ / 100;
	int y = screen_y_ * desktop_scale_ / 100;
	int width = screen_width_ * desktop_scale_ / 100;
	int height = screen_height_ * desktop_scale_ / 100;

	HDC hScrDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	HDC hMemDC = CreateCompatibleDC(hScrDC);
	// ����һ������Ļ�豸��������ݵ�λͼ
	bitmap_bg_ = CreateCompatibleBitmap(hScrDC, width, height);
	// ����λͼѡ���ڴ��豸��������
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC,bitmap_bg_);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC, 0, 0, width, height, hScrDC, x, y, SRCCOPY | CAPTUREBLT);
	//�õ���Ļλͼ�ľ��
	bitmap_bg_ = (HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//���
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	if(bitmap_bg_)
	{
		std::wstring capture_path = user_data_dir_;
		if (!capture_path.empty())
		{
			capture_path += L"tmp\\";
			if (false == nbase::FilePathIsExist(capture_path, true))
			{
				CreateDirectory(capture_path);
			}
			capture_path += L"capture.png";
			bResult = SaveBitmapToFile(bitmap_bg_, capture_path);
			if (bResult)
			{
				VBox* verLayout = static_cast<VBox *>(FindControl(L"bg_panel"));
				if (verLayout)
				{
					verLayout->SetBkImage(capture_path);
				}
			}
		}
	}

	return bResult;
}

BOOL CaptureWindow::SaveBitmapToFile(HBITMAP hBitmap, const std::wstring &file_path)
{    
	CImage image;
	image.Attach(hBitmap);
	image.Save(file_path.c_str());
	image.Detach();
	return TRUE;
}

void CaptureWindow::NormalizeRect(RECT& rect)
{
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
}

//�ü��ڿͻ����Ŀɽ�ͼ����
RECT CaptureWindow::ClipTrackRect(RECT rect)
{
	RECT client_rect;
	GetClientRect(m_hWnd, &client_rect);
	NormalizeRect(rect);
	if(rect.left < client_rect.left)
		rect.left = client_rect.left;

	if(rect.right >= client_rect.right)
		rect.right = client_rect.right;

	if(rect.top < client_rect.top)
		rect.top = client_rect.top;

	if(rect.bottom >= client_rect.bottom)
		rect.bottom = client_rect.bottom;

	return rect;
}

RECT CaptureWindow::GetToolBarRect()
{
	RECT rect = ClipTrackRect(track_rect_);
	int width = rect.right-rect.left;
	int height = toolbar_->GetMinHeight();
	// ��ɫ��ɼ�ʱҪ���ϵ�ɫ��ĸ߶�
	if(is_show_color_palette_)
	{
		color_palette_->SetVisible(true);
		if (capture_edit_mode_ == EM_TEXT)
			color_palette_->ShowRange(false, true, false, true);
		else if (capture_edit_mode_ == EM_MOSAIC || capture_edit_mode_ == EM_GAUSS)
			color_palette_->ShowRange(true, false, true, false);
		else
			color_palette_->ShowRange(true, false, false, true);
		height += color_palette_->GetToolbarHeight();
	}
	else
	{
		color_palette_->SetVisible(false);
	}
	int boarder = 4;
	DpiManager::GetInstance()->ScaleInt(boarder);

	if (width < toolbar_min_width_)
	{
		width = toolbar_min_width_;
	}
	int x = rect.right - width;
	if (x < 0)
		x = 0;
	int y = rect.bottom + boarder*2;
	if (y + height + boarder > screen_height_)
	{
		y = rect.top - height - boarder*2;
		if (y < 0)
		{
			y = rect.bottom - height - boarder*2;
		}
	}
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;
	return rect;
}

RECT CaptureWindow::GetToolBar2Rect()
{
	RECT rect = ClipTrackRect(track_rect_);
	int height = 30;
	int boarder = 4;
	DpiManager::GetInstance()->ScaleInt(height);
	DpiManager::GetInstance()->ScaleInt(boarder);

	int x = rect.left;
	if (x + toolbar_min_width2_ > screen_width_)
	{
		x = screen_width_ - toolbar_min_width2_;
	}
	int y = rect.top - height - boarder;
	if (y < 0)
	{
		y = rect.top + boarder;
	}
	rect.left = x;
	rect.top = y;
	rect.right = x + toolbar_min_width2_;
	rect.bottom = y + height;
	return rect;
}

//��С��Ϣ��ʾ����
RECT CaptureWindow::GetSizeTipRect()
{
	int width = 75;
	int height = 40;
	int boarder = 10;
	DpiManager::GetInstance()->ScaleInt(width);
	DpiManager::GetInstance()->ScaleInt(height);
	DpiManager::GetInstance()->ScaleInt(boarder);

	RECT rect = ClipTrackRect(track_rect_);
	POINT point;
	GetCursorPos(&point);
	point.x -= screen_x_;
	point.y -= screen_y_;
	int x = 0;
	int y = 0;

	if (point.x < rect.right - BORDER_SIZE)
	{
		x = point.x - width - boarder;
		if (x < 0)
		{
			x = point.x + boarder;
		}
	} 
	else
	{
		x = point.x + boarder;
		if (x + width > screen_width_)
		{
			x = point.x - width - boarder;
		}
	}

	if (point.y < rect.bottom - BORDER_SIZE)
	{
		y = point.y - height - boarder;
		if (y < 0)
		{
			y = point.y + boarder;
		}
	} 
	else
	{
		y = point.y + boarder;
		if (y + height > screen_height_)
		{
			y = point.y - height - boarder;
		}
	}

	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;
	return rect;
}

//������Ԫ������
RECT CaptureWindow::GetDrawTrackRect(int hit_test)
{
	RECT rect_ret = ClipTrackRect(track_rect_);

	int width = rect_ret.right - rect_ret.left;
	int height = rect_ret.bottom - rect_ret.top;
	switch (hit_test)
	{
	case SIZETOP://����
		{
			rect_ret.left += width/2 - DOT_SIZE/2;
			rect_ret.top -= DOT_SIZE/2;
		}
		break;
	case SIZEBOTTOM://����
		{
			rect_ret.left += width/2 - DOT_SIZE/2;
			rect_ret.top = rect_ret.bottom - DOT_SIZE/2;
		}
		break;
	case SIZELEFT://����
		{
			rect_ret.left -= DOT_SIZE/2;
			rect_ret.top += height/2 - DOT_SIZE/2;
		}
		break;
	case SIZERIGHT://����
		{
			rect_ret.left = rect_ret.right - DOT_SIZE/2;
			rect_ret.top += height/2 - DOT_SIZE/2;
		}
		break;
	case SIZESW://����
		{
			rect_ret.left -= DOT_SIZE/2;
			rect_ret.top -= DOT_SIZE/2;
		}
		break;
	case SIZESE://����
		{
			rect_ret.left = rect_ret.right - DOT_SIZE/2;
			rect_ret.top -= DOT_SIZE/2;
		}
		break;
	case SIZENW://����
		{
			rect_ret.left -= DOT_SIZE/2;
			rect_ret.top = rect_ret.bottom - DOT_SIZE/2;
		}
		break;
	case SIZENE://����
		{
			rect_ret.left = rect_ret.right - DOT_SIZE/2;
			rect_ret.top = rect_ret.bottom - DOT_SIZE/2;
		}
		break;
	case SIZELEFT_LINE://��
		{
			rect_ret.right = rect_ret.left + 1;
		}
		break;
	case SIZERIGHT_LINE://��
		{
			rect_ret.left = rect_ret.right;
			rect_ret.right = rect_ret.left + 1;
		}
		break;
	case SIZETOP_LINE://��
		{
			rect_ret.bottom = rect_ret.top + 1;
		}
		break;
	case SIZEBOTTOM_LINE://��
		{
			rect_ret.top = rect_ret.bottom;
			rect_ret.bottom = rect_ret.top + 1;
		}
		break;
	}
	if (hit_test < SIZELEFT_LINE)
	{
		rect_ret.right = rect_ret.left + DOT_SIZE;
		rect_ret.bottom = rect_ret.top + DOT_SIZE;
	}
	return rect_ret;
}

//����
void CaptureWindow::DrawTrack(RECT track_rect)
{
	RECT client_rect;
	GetClientRect(m_hWnd, &client_rect);
	track_rect = ClipTrackRect(track_rect);

	int left = track_rect.left - client_rect.left;
	int top = track_rect.top - client_rect.top;
	int right = client_rect.right - track_rect.right;
	int bottom = client_rect.bottom - track_rect.bottom;

	int width = track_rect.right - track_rect.left;
	int height = track_rect.bottom - track_rect.top;

	bool show_track = width > 0 || height > 0;
	if (show_track)
	{
		right--;
		bottom--;
	}

	Control *control = static_cast<Control *>(FindControl(L"left_panel"));
	if (control)
	{
		control->SetFixedWidth(left, true, false);
		control->SetVisible(left > 0);
	}
	control = static_cast<Control *>(FindControl(L"top_panel"));
	if (control)
	{
		control->SetFixedHeight(top, false);
		control->SetVisible(top > 0);
	}
	control = static_cast<Control *>(FindControl(L"right_panel"));
	if (control)
	{
		control->SetFixedWidth(right, true, false);
		control->SetVisible(right > 0);
	}
	control = static_cast<Control *>(FindControl(L"bottom_panel"));
	if (control)
	{
		control->SetFixedHeight(bottom, false);
		control->SetVisible(bottom > 0);
	}

	//������
	control = static_cast<Control *>(FindControl(L"track_left_line"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZELEFT_LINE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		control->SetFixedWidth(track_board.right - track_board.left, true, false);
		control->SetFixedHeight(track_board.bottom - track_board.top, false);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_right_line"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZERIGHT_LINE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		control->SetFixedWidth(track_board.right - track_board.left, true, false);
		control->SetFixedHeight(track_board.bottom - track_board.top, false);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_top_line"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZETOP_LINE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		control->SetFixedWidth(track_board.right - track_board.left, true, false);
		control->SetFixedHeight(track_board.bottom - track_board.top, false);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_bottom_line"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZEBOTTOM_LINE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		control->SetFixedWidth(track_board.right - track_board.left, true, false);
		control->SetFixedHeight(track_board.bottom - track_board.top, false);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}

	//�Ķ���
	control = static_cast<Control *>(FindControl(L"track_topleft"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZESW);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_topright"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZESE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_bottomleft"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZENW);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_bottomright"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZENE);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(show_track);
	}

	//���е�
	control = static_cast<Control *>(FindControl(L"track_left"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZELEFT);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(height > DOT_SIZE*2 && show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_right"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZERIGHT);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(height > DOT_SIZE*2 && show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_top"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZETOP);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(width > DOT_SIZE*2 && show_track);
	}
	control = static_cast<Control *>(FindControl(L"track_bottom"));
	if (control)
	{
		RECT track_board = GetDrawTrackRect(SIZEBOTTOM);

		RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
		control->SetMargin(margin, false);
		//control->SetFixedWidth(track_board.right - track_board.left);
		//control->SetFixedHeight(track_board.bottom - track_board.top);

		//control->SetPos(track_board);
		control->SetVisible(width > DOT_SIZE*2 && show_track);
	}
}

//��ʾ������
void CaptureWindow::ShowToolBar(bool edit)
{
	bool show = DeTrackState(track_state_) == TRACKED;
	Control *control = static_cast<Control *>(FindControl(L"toolbar"));
	if (control)
	{
		bool show_toolbar = show;
		RECT track_board = GetToolBarRect();
		if (show_toolbar && edit)
		{
			RECT rect = ClipTrackRect(track_rect_);
			if (track_board.top > rect.top && track_board.bottom < rect.bottom)
			{
				show_toolbar = false;
			}
		}

		if (show_toolbar)
		{
			RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
			control->SetMargin(margin, false);
			control->SetFixedWidth(track_board.right - track_board.left, true, false);
			//control->SetFixedHeight(track_board.bottom - track_board.top);

			//control->SetPos(track_board);
		}
		control->SetVisible(show_toolbar);
	}
	Control *control2 = static_cast<Control *>(FindControl(L"toolbar2"));
	if (control2 && send_info_.size() > 0)
	{
		bool show_toolbar = show;
		RECT track_board = GetToolBar2Rect();
		if (show_toolbar && edit)
		{
			RECT rect = ClipTrackRect(track_rect_);
			if (track_board.top > rect.top && track_board.bottom < rect.bottom)
			{
				show_toolbar = false;
			}
		}
		if (show_toolbar)
		{
			RECT margin = {track_board.left - control2->GetParent()->GetPos().left, track_board.top - control2->GetParent()->GetPos().top, 0, 0};
			control2->SetMargin(margin, false);
			control2->SetFixedWidth(track_board.right - track_board.left, true, false);
			control2->SetFixedHeight(track_board.bottom - track_board.top, false);

			//control2->SetPos(track_board);
		}
		control2->SetVisible(show_toolbar);
	}
}

//��ʾ��ͼ��С��Ϣ
void CaptureWindow::ShowSizeTip()
{
	Label *control = static_cast<Label *>(FindControl(L"size_tip"));
	if (control)
	{
		bool show = DeTrackState(track_state_) == TRACKING;
		if (show)
		{
			RECT track_board = GetSizeTipRect();

			RECT margin = {track_board.left - control->GetParent()->GetPos().left, track_board.top - control->GetParent()->GetPos().top, 0, 0};
			control->SetMargin(margin, false);
			control->SetFixedWidth(track_board.right - track_board.left, true, false);
			control->SetFixedHeight(track_board.bottom - track_board.top, false);

			//control->SetPos(track_board);
			Label *control = static_cast<Label *>(FindControl(L"tip_size"));
			if (control)
			{
				RECT track_rect = ClipTrackRect(track_rect_);
				std::wstring size;
				size = nbase::StringPrintf(_T("(%d*%d)"), track_rect.right - track_rect.left + 1, track_rect.bottom - track_rect.top + 1);
				control->SetText(size.c_str());
			}
		}
		control->SetVisible(show);
	}
}

// ��Tracker����HitTest
int CaptureWindow::TrackerHitTest(RECT rect, POINT point)
{
	int x = point.x;// -screen_x_;
	int y = point.y;// -screen_y_;
	Control *control = static_cast<Control *>(FindControl(L"toolbar_button"));
	if (control && control->IsVisible())
	{
		RECT rect_button = control->GetPos();
		if (x >= rect_button.left && x <= rect_button.right && y >= rect_button.top && y <= rect_button.bottom)
		{
			return NOTRACK;
		}
	}

	NormalizeRect(rect);

	// ����
	if(x >= rect.left - BORDER_SIZE && x <= rect.left + BORDER_SIZE &&  y >= rect.top - BORDER_SIZE &&  y <= rect.top + BORDER_SIZE)
	{
		return SIZENW;
	}

	// ����
	if(x >= rect.right - BORDER_SIZE && x <= rect.right + BORDER_SIZE &&  y >= rect.top - BORDER_SIZE &&  y <= rect.top + BORDER_SIZE)
	{
		return SIZENE;
	}

	// ����
	if(x >= rect.left - BORDER_SIZE && x <= rect.left + BORDER_SIZE &&  y >= rect.bottom - BORDER_SIZE &&  y <= rect.bottom + BORDER_SIZE)
	{
		return SIZESW;
	}

	// ����
	if(x >= rect.right - BORDER_SIZE && x <= rect.right + BORDER_SIZE &&  y >= rect.bottom - BORDER_SIZE &&  y <= rect.bottom + BORDER_SIZE)
	{
		return SIZESE;
	}

	// ��
	if(x >= rect.left - BORDER_SIZE && x <= rect.left + BORDER_SIZE &&  y >= rect.top &&  y <= rect.bottom)
	{
		return SIZELEFT;
	}

	// ��
	if(x >= rect.left && x <= rect.right &&  y >= rect.top - BORDER_SIZE &&  y <= rect.top + BORDER_SIZE)
	{
		return SIZETOP;
	}

	// ��
	if(x >= rect.right - BORDER_SIZE && x <= rect.right + BORDER_SIZE &&  y >= rect.top &&  y <= rect.bottom)
	{
		return SIZERIGHT;
	}

	// ��
	if(x >= rect.left && x <= rect.right &&  y >= rect.bottom - BORDER_SIZE &&  y <= rect.bottom + BORDER_SIZE)
	{
		return SIZEBOTTOM;
	}

	// ��
	if(x >= rect.left && x <= rect.right &&  y >= rect.top &&  y <= rect.bottom)
	{
		return SIZEALL;
	}

	return 0;
}

// ���ڵ��ڵ㣨�ߣ��ϵ��ڵ�ʱ������λ��
void CaptureWindow::AdjustOffset(RECT& rect_track, POINT point_dest, POINT point_offset, int hit_test)
{
	//point_dest.x -= screen_x_;
	//point_dest.y -= screen_y_;
	//point_offset.x -= screen_x_;
	//point_offset.y -= screen_y_;
	switch(hit_test)
	{
	case NOTRACK:
		{
			rect_track.left = point_dest.x;
			rect_track.top = point_dest.y;
			rect_track.right = point_dest.x;
			rect_track.bottom = point_dest.y;
		}
		break;
	case SIZEALL:
		{
			int x = point_dest.x - point_offset.x;
			int y = point_dest.y - point_offset.y;
			rect_track.left += x;
			rect_track.top += y;
			rect_track.right += x;
			rect_track.bottom += y;
		}
		break;
	case SIZELEFT:		rect_track.left = point_dest.x; break;
	case SIZERIGHT:		rect_track.right = point_dest.x; break;
	case SIZETOP:		rect_track.top = point_dest.y; break;
	case SIZEBOTTOM:	rect_track.bottom = point_dest.y; break;
	case SIZENW:		rect_track.left = point_dest.x; rect_track.top = point_dest.y; break;
	case SIZENE:		rect_track.right = point_dest.x; rect_track.top = point_dest.y; break;
	case SIZESW:		rect_track.left = point_dest.x; rect_track.bottom = point_dest.y; break;
	case SIZESE:		rect_track.right = point_dest.x; rect_track.bottom = point_dest.y; break;
	}
}

// ����HitTest���ȡ����ȷ�Ĺ����
void CaptureWindow::SetHitTestCursor(int hit_test)
{
	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	switch(hit_test)
	{
	case SIZEALL:		cursor = LoadCursor(NULL, IDC_SIZEALL); break;
	case SIZETOP:
	case SIZEBOTTOM:	cursor = LoadCursor(NULL, IDC_SIZENS); break;
	case SIZELEFT:
	case SIZERIGHT:		cursor = LoadCursor(NULL, IDC_SIZEWE); break;
	case SIZESE:
	case SIZENW:		cursor = LoadCursor(NULL, IDC_SIZENWSE); break;
	case SIZESW:
	case SIZENE:		cursor = LoadCursor(NULL, IDC_SIZENESW); break;
	}
	SetCursor(cursor);
}

void CaptureWindow::Save()
{
	std::wstring file_type = MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_CAPTUREFORM_IMAGE_FORMAT");
	std::wstring text = nbase::StringPrintf(L"%s(*.png)", file_type.c_str());
	// Ҫ������ļ���
	std::wstring file_name;
	nbase::Time::TimeStruct now = nbase::Time::Now().ToTimeStruct(true);
	nbase::StringPrintf(file_name, MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_CAPTUREFORM_CLIP").c_str(),
		now.year(), now.month(), now.day_of_month(), now.hour(), now.minute(), now.second());
	CFileDialogEx* file_dlg = new CFileDialogEx();
	std::map<LPCTSTR,LPCTSTR> filters;
	filters[text.c_str()] = L"*.png";
	file_dlg->SetFilter(filters);
	file_dlg->SetFileName(file_name.c_str());
	file_dlg->SetDefExt(L"png");
	file_dlg->SetParentWnd(m_hWnd);
	// ������ģ̬�Ի���
	CFileDialogEx::FileDialogCallback2 callback2 = nbase::Bind(&CaptureWindow::SaveTrackImageDlgCallback, this, std::placeholders::_1, std::placeholders::_2);
	file_dlg->AyncShowSaveFileDlg(callback2);
}

void CaptureWindow::SaveTrackImageDlgCallback( BOOL ret, std::wstring file_path )
{
	if(!ret)
		return;

	// ����ı��༭��δ����
	if(capture_edit_mode_ == EM_TEXT && capture_edit_)
	{
		capture_edit_->OnTextEditFinished();
	}

	if (bitmap_bg_ && DeTrackState(track_state_) == TRACKED)
	{
		RECT track_rect = ClipTrackRect(track_rect_);
		int width = track_rect.right - track_rect.left + 1;
		int height = track_rect.bottom - track_rect.top + 1;
		RECT client_rect;
		GetClientRect(m_hWnd, &client_rect);
		int x = track_rect.left - client_rect.left;
		int y = track_rect.top - client_rect.top;
		x = x * desktop_scale_ / 100;
		y = y * desktop_scale_ / 100;
		width = width * desktop_scale_ / 100;
		height = height * desktop_scale_ / 100;

		int image_save_width = width;
		int image_save_height = height;

		// ��ĻDC 
		HDC hScrDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		// ��Ļ����DC��ѡ���ͼ��ʼʱ�������Ļͼ��
		HDC hScrDC2 = CreateCompatibleDC(hScrDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hScrDC2,bitmap_bg_);
		// ���ϱ༭������
		if(capture_edit_)
			capture_edit_->DrawExtraUnits(hScrDC2);
		// ��Ļ����DC��Ŀ�����DC����ѡ���ͼ����ļ���λͼ
		HDC hMemDC = CreateCompatibleDC(hScrDC);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScrDC, image_save_width, image_save_height);
		HBITMAP hOldBitmap2 = (HBITMAP)SelectObject(hMemDC,hBitmap);

		BitBlt(hMemDC,0,0,width, height, hScrDC2, x, y,SRCCOPY);

		hBitmap = (HBITMAP)SelectObject(hMemDC,hOldBitmap2);
		(HBITMAP)SelectObject(hScrDC2,hOldBitmap);
		//���
		DeleteDC(hScrDC);
		DeleteDC(hScrDC2);
		DeleteDC(hMemDC);
		if(hBitmap)
		{
			// �û�ѡ�񱣴��λ��
			SaveBitmapToFile(hBitmap, file_path);
			DeleteObject(hBitmap);
			// �رմ���
			this->Close();
		}
	}
}

void CaptureWindow::ResetCaptureEditMode()
{
	SetCaptureEditMode(EM_NONE); // �ָ�δ�༭״̬
	capture_edit_->ReleaseAllDrawUnits(); // �ͷ��ϴα༭�Ļ��Ƶ�Ԫ
	// ����Option��Selected״̬
	Option* option_ui = NULL;
	for(size_t i = 0; i < vec_option_names_.size(); i++)
	{
		option_ui = static_cast<Option *>(FindControl(vec_option_names_[i].c_str()));
		if(option_ui)
			option_ui->Selected(false);
	}
	is_show_color_palette_ = false;
}

void CaptureWindow::SetCaptureEditMode( EditMode mode )
{
	if(capture_edit_mode_ == mode && mode > EM_NONE)
	{
		// �ظ������ͬ�ı༭��Ԫ����ô�����ɫ��
		is_show_color_palette_ = false;
	}
	else
	{
		capture_edit_mode_ = mode;
		if(mode > EM_NONE)
		{
			is_show_color_palette_ = true;
		}
	}
	capture_edit_->SetEditMode(capture_edit_mode_);
	ShowToolBar(); // ��������߶ȸı䣬�ػ�
}
