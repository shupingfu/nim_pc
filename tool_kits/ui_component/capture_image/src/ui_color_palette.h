#ifndef _NIM_GUI_CAPTURE_FORM_UI_COLOR_PALETTE_H_
#define _NIM_GUI_CAPTURE_FORM_UI_COLOR_PALETTE_H_
#pragma once


#include "duilib/UIlib.h"

// ��ɫ��
class CColorBlock : public ui::Option
{
public:
	CColorBlock();
	virtual ~CColorBlock();
	virtual void Paint(HDC hDC, const ui::UiRect& rcPaint) override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;
	DWORD GetColor() { return color_; };
	std::wstring GetColorName() { return color_name_; }

private:
	void DrawColorBlock(HDC hdc, const ui::UiRect &rect);

private:
	HBRUSH fill_color_brush_;
	HBRUSH select_rect_brush_;
	HPEN select_rect_pen_;
	DWORD color_;
	std::wstring color_name_;
};

// ��ˢ��
class CBrushBlock : public ui::Option
{
public:
	CBrushBlock();
	virtual ~CBrushBlock();
	virtual void HandleMessage(ui::EventArgs& event);
	virtual void Paint(HDC hDC, const ui::UiRect& rcPaint) override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;
	int GetBrushWidth() { return brush_width_; };
	int GetBrushWidth2() { return brush_width2_; };

private:
	int brush_width_; // ��ˢ���
	int brush_width2_; // ģ������ֱ��
	int brush_paint_width_;//��ʾ��ʱ��Բ��ֱ��
	void DrawCircleBlock(HDC hdc, const ui::UiRect &rect);
	std::unique_ptr<Gdiplus::SolidBrush> brush_;
};

// ��ɫ��
class CColorPalette : public ui::Box
{
public:
	CColorPalette();
	virtual ~CColorPalette();
	ui::Box* CreateColorPaletteUI(ui::CreateControlCallback callback, bool visible = true);
	void ShowRange(bool show_brush_width, bool show_font_size, bool show_range, bool show_color);
	void InitChildControls();
	DWORD GetSelectedColorRGB();
	std::wstring GetSelectedColorName();
	int GetSelectedBrushWidth();
	int GetSelectedBrushWidth2();
	int GetRangePos();
	int GetToolbarHeight();
	int GetSelectedFontIndex();

private:
	bool OnClick(ui::EventArgs* param);
	bool OnSelect(ui::EventArgs *msg);
	ui::Box* color_palette_;
	CColorBlock* selected_color_;
	int selected_brush_width_;
	int selected_brush_width2_;
	DWORD color_;
	std::wstring color_name_;
	ui::Combo* font_size_combo_;
};
#endif  // _NIM_GUI_CAPTURE_FORM_UI_COLOR_PALETTE_H_