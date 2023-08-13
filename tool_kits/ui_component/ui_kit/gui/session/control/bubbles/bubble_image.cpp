#include "stdafx.h"
#include "bubble_image.h"
#include "image_view/src/image_view_manager.h"
#include "util/user.h"
#include "shared/zoom_image.h"

using namespace ui;

namespace nim_comp
{
void MsgBubbleImage::InitControl(bool bubble_right)
{
	__super::InitControl(bubble_right);

	msg_image_ = new ButtonBox;
	if(bubble_right)
		GlobalManager::FillBoxWithCache(msg_image_, L"session/image_right.xml");
	else
		GlobalManager::FillBoxWithCache(msg_image_, L"session/image_left.xml");
	bubble_box_->Add( msg_image_ );

	image_ = msg_image_->FindSubControl(L"image");

	msg_image_->AttachClick(nbase::Bind(&MsgBubbleImage::OnClicked, this, std::placeholders::_1));
	msg_image_->AttachMenu(nbase::Bind(&MsgBubbleImage::OnMenu, this, std::placeholders::_1));
}

void MsgBubbleImage::InitInfo(const nim::IMMessage &msg)
{
	__super::InitInfo(msg);

	SetCanView(false);
	InitResPath(msg_);

	if (nbase::FilePathIsExist(thumb_, false)) //thumb_图片存在
	{
		if (CheckThumbImage())
		{
			image_->SetFixedWidth(thumb_image_width_, false); 
			image_->SetFixedHeight(thumb_image_height_);
			image_->SetBkImage(thumb_);
			if (!my_msg_ || !(msg_.status_ == nim::kNIMMsgLogStatusSendFailed))
				SetLoadStatus(RS_LOAD_OK);
			if (my_msg_ && msg_.status_ == nim::kNIMMsgLogStatusSending)
				SetMsgStatus(nim::kNIMMsgLogStatusSending);
			SetCanView(true);
		}
		else //图片有错误
			SetLoadStatus(RS_LOAD_NO);
	}
	else if (nbase::FilePathIsExist(path_, false)) //缩略图不存在，而原图存在
	{
		DoZoom();
	}
	else //thumb_图片和原图都不存在
	{
		if (msg_.rescode_ == 0) //读取消息历史
		{
			SetLoadStatus(RS_LOADING);
		}
		else //接收
		{
			if (msg_.rescode_ == nim::kNIMResSuccess
				|| msg_.rescode_ == nim::kNIMLocalResExist)
				SetLoadStatus(RS_LOADING);
			else if (msg_.rescode_ == nim::kNIMLocalResParameterError)
				SetLoadStatus(RS_LOAD_NO);
			else
			{
				SetLoadStatus(RS_LOAD_NO);
				QLOG_WAR(L"unknown receive msg code {0}") << msg_.rescode_;
			}
		}
	}
}

bool MsgBubbleImage::OnClicked( ui::EventArgs* arg )
{
	if( nbase::FilePathIsExist(path_, false) )
	{
		ImageViewManager::GetInstance()->StartViewPic(path_, L"", true);
	}
	else //原图不存在
	{
		ImageViewManager::GetInstance()->StartViewPic(path_, L"", true);
		nim::NOS::DownloadMediaCallback cb = [](nim::NIMResCode res_code, const std::string& file_path, const std::string&, const std::string&)
		{
			if (res_code == nim::kNIMResSuccess)
			{
				ImageViewManager* image_view_mgr = ImageViewManager::GetInstance();
				if (image_view_mgr)
					image_view_mgr->OnImageReady(nbase::UTF8ToUTF16(file_path), true);
			}
		};
		nim::NOS::FetchMedia(msg_, cb, nim::NOS::ProgressCallback());
	}
	return true;
}

void MsgBubbleImage::SetCanView(bool can)
{
	msg_image_->SetEnabled(can);
}

void MsgBubbleImage::OnDownloadCallback( bool success, const std::string& file_path )
{
	if (image_checked_)
		return;

	ASSERT(!file_path.empty());
	std::wstring wpath = nbase::UTF8ToUTF16(file_path);
	if (wpath == thumb_) //下载的是缩略图
	{
		if (success)
		{
			ListBox* lbx = dynamic_cast<ListBox*>(this->GetParent());
			ASSERT(lbx);
			bool at_end = lbx->IsAtEnd();

			SetLoadStatus(RS_LOAD_OK);
			if (CheckThumbImage())
			{
				image_->SetFixedWidth(thumb_image_width_, false); 
				image_->SetFixedHeight(thumb_image_height_);
				image_->SetBkImage(thumb_);
			}
			SetCanView(true);
			
			if (at_end)
				lbx->EndDown(true, false);
		}
		else
		{
			SetLoadStatus(RS_LOAD_NO);
		}
	}
	else if (wpath == path_) //下载的是原图
	{
		if (success)
		{
			ListBox* lbx = dynamic_cast<ListBox*>(this->GetParent());
			ASSERT(lbx);
			bool at_end = lbx->IsAtEnd();

			SetLoadStatus(RS_LOAD_OK);

			DoZoom();

			if (at_end)
				lbx->EndDown(true, false);
		}
		else
		{
			SetLoadStatus(RS_LOAD_NO);
		}
	}
}

void MsgBubbleImage::DoZoom()
{
	if (image_checked_)
		return;

	if( !nbase::FilePathIsExist(path_, false) )
	{
		InitResPath(msg_);
		if (!nbase::FilePathIsExist(path_, false))
		{
			QLOG_ERR(L"Image not exist: {0}") << path_;
			return;
		}
	}

	const int cx = 270, cy = 180, xf = 78 - 5, yf = 57 - 5;

	std::wstring mime_type = ZoomImage::GetMimeType(path_);
	if( mime_type.empty() )
	{
		QLOG_ERR(L"Image wrong: {0}") <<path_;
		return;
	}
	SetCanView(true);

	SIZE sz = { 0, 0 };
	bool resize = CalculateImageSize(path_, sz, cx, cy);

	image_->SetFixedWidth(sz.cx, false);
	image_->SetFixedHeight(sz.cy);

	if(mime_type == kImageGIF)
	{
		image_->SetBkImage(path_);
	}
	else
	{
		if(resize)
		{
			ZoomImageF(path_, thumb_, sz.cx, sz.cy);
			image_->SetBkImage(thumb_);
		}
		else
		{
			nbase::CopyFile(path_, thumb_);
			image_->SetBkImage(thumb_);
			if(sz.cx < xf && sz.cy < yf)
			{
				image_->SetForeStateImage(kControlStateNormal, L"");
			}
		}
	}
	image_checked_ = true;
}

bool MsgBubbleImage::OnMenu( ui::EventArgs* arg )
{
	bool show_retweet = true;
	if (msg_.type_ == nim::kNIMMessageTypeCustom)
	{
		Json::Value json;
		if (StringToJson(msg_.attach_, json) && json.isObject())
		{
			int sub_type = json["type"].asInt();
			if (sub_type == CustomMsgType_SnapChat)
				show_retweet = false;
		}
	}
	PopupMenu(true, true, show_retweet);
	return false;
}

bool MsgBubbleImage::NeedDownloadResource()
{
	//if( nbase::FilePathIsExist(thumb_, false) )
		return false;
	//else
	//	return true;
}
bool MsgBubbleImage::NeedDownloadResourceEx()
{
	if( nbase::FilePathIsExist(path_, false) )
		return false;
	else
		return true;
}

void MsgBubbleImage::OnMenuCopy() {
    HBITMAP hBitmap = NULL;
    //背景色设置为白色。
    Gdiplus::Color backColor = Gdiplus::Color(255, 255, 255, 255);
    // Status sts
    if (PathFileExists(path_.c_str())) {
        Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(path_.c_str());
        Gdiplus::Status sts = pBitmap->GetLastStatus();
        sts = pBitmap->GetHBITMAP(backColor, &hBitmap);
        if (sts == Gdiplus::Ok) {
            int k = 0;
        }
    }
    int error = GetLastError();

    if (!::OpenClipboard(NULL))
        return;
    ::EmptyClipboard();
    BITMAP bm;
    ::GetObject(hBitmap, sizeof(bm), &bm);

    BITMAPINFOHEADER bi;
    ::ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = bm.bmBitsPixel;
    bi.biCompression = BI_RGB;
    if (bi.biBitCount <= 1)  // make sure bits per pixel is valid
        bi.biBitCount = 1;
    else if (bi.biBitCount <= 4)
        bi.biBitCount = 4;
    else if (bi.biBitCount <= 8)
        bi.biBitCount = 8;
    else  // if greater than 8-bit, force to 24-bit
        bi.biBitCount = 24;

    // Get size of color table.
    SIZE_T dwColTableLen = (bi.biBitCount <= 8) ? (1 << bi.biBitCount) * sizeof(RGBQUAD) : 0;

    // Create a device context with palette
    HDC hDC = ::GetDC(NULL);
    HPALETTE hPal = static_cast<HPALETTE>(::GetStockObject(DEFAULT_PALETTE));
    HPALETTE hOldPal = ::SelectPalette(hDC, hPal, FALSE);
    ::RealizePalette(hDC);

    // Use GetDIBits to calculate the image size.
    ::GetDIBits(hDC, hBitmap, 0, static_cast<UINT>(bi.biHeight), NULL, reinterpret_cast<LPBITMAPINFO>(&bi), DIB_RGB_COLORS);
    // If the driver did not fill in the biSizeImage field, then compute it.
    // Each scan line of the image is aligned on a DWORD (32bit) boundary.
    if (0 == bi.biSizeImage)
        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;

    // Allocate memory
    HGLOBAL hDIB = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + dwColTableLen + bi.biSizeImage);
    if (hDIB) {
        union tagHdr_u {
            LPVOID p;
            LPBYTE pByte;
            LPBITMAPINFOHEADER pHdr;
            LPBITMAPINFO pInfo;
        } Hdr;

        Hdr.p = ::GlobalLock(hDIB);
        // Copy the header
        ::CopyMemory(Hdr.p, &bi, sizeof(BITMAPINFOHEADER));
        // Convert/copy the image bits and create the color table
        int nConv = ::GetDIBits(hDC, hBitmap, 0, static_cast<UINT>(bi.biHeight), Hdr.pByte + sizeof(BITMAPINFOHEADER) + dwColTableLen, Hdr.pInfo,
                                DIB_RGB_COLORS);
        ::GlobalUnlock(hDIB);
        if (!nConv) {
            ::GlobalFree(hDIB);
            hDIB = NULL;
        }
    }
    if (hDIB)
        ::SetClipboardData(CF_DIB, hDIB);
    ::CloseClipboard();
    ::SelectPalette(hDC, hOldPal, FALSE);
    ::ReleaseDC(NULL, hDC);
}

}