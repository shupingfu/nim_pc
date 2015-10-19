#ifndef _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_TEXT_EDIT_H_
#define _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_TEXT_EDIT_H_
#pragma once

#include "duilib/UIlib.h"

class CCaptureTextEdit : public ui::RichEdit
{
public:
	CCaptureTextEdit(const ui::UiRect &rect);
	~CCaptureTextEdit();
	
	virtual void HandleMessage(ui::EventArgs& event);
	virtual void Paint(HDC hDC, const ui::UiRect& rcPaint) override; 
	ui::UiRect GetCaptureTextEditRect(){ return rc_valid_; };
	void SetDotPenColor(DWORD color) { color_pen_ = color; };
private:
	void DrawRect(HDC hdc);
private:
	ui::UiRect rc_valid_;		// ��Чλ��(���ڻ��Ƶ�λ��)
	HBRUSH			 white_brush_;  // ���ڻ��ư׵ױ���
	DWORD			 color_pen_;	// ������ɫ
};
#endif  // _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_TEXT_EDIT_H_