#ifndef _NIM_GUI_CAPTURE_FORM_DRAW_UNIT_H_
#define _NIM_GUI_CAPTURE_FORM_DRAW_UNIT_H_

#pragma once

#include "duilib/UIlib.h"

//////////////////////////////////���ƻ���////////////////////////////////////////

class DrawUnit
{
public:
	enum BlurType
	{
		BLUR_TYPE_NONE = 0,	// û��ģ��Ч��
		BLUR_TYPE_MOSAIC,	// ������
		BLUR_TYPE_GAUSS,	// ë����
	};
	DrawUnit(int x, int y, const RECT& rc_valid);
	virtual ~DrawUnit();
	void Render(HDC hdc, HBITMAP bitmap, bool is_continue = false);  // ���ƽӿ�
	void SetLineWidth(int line_width)	{ line_width_ = line_width; }
	void SetColor(COLORREF dw_color)	{ color_ = dw_color; }
	ui::CPoint	GetStartPoint()		{ return ui::CPoint(left_, top_); }
	virtual void SetEndPoint(int x,int y)	{ right_ = x; bottom_ = y; }
	ui::UiRect GetWindowRect()	{ return ui::UiRect(left_, top_, right_, bottom_); }
	void SetBlur(BlurType blur_type, int blur_range) { blur_type_ = blur_type; blur_range_ = blur_range; }
	//bool IsMosaic() { return mosaic_; }
	int GetBlurRange() { return blur_range_; }
	BlurType GetBlurType() { return blur_type_; }
	bool NeedSaveDraw() { return need_save_draw_; }

protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue) = 0;
	void MeasurePoint(int &x, int &y);	// ȷ��Ҫ���Ƶĵ��ڽ�ͼ����Χ�� 

protected:
	int left_, top_, right_, bottom_;	//��ʼ���꣬�յ�����
	int line_width_;					//�ߴ�
	COLORREF color_;					//��ɫ
	ui::UiRect rc_valid_;			//�Ϸ���Χ
	int blur_range_;
	BlurType blur_type_;
	bool need_save_draw_;
};

//////////////////////////////////����////////////////////////////////////////

class DrawUnitRectangle : public DrawUnit
{
public:
	DrawUnitRectangle(int x,int y,const RECT& rc_vaild)
		:DrawUnit(x,y,rc_vaild){}
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
};

//////////////////////////////////ֱ��////////////////////////////////////////

class DrawUnitLine	: public DrawUnit
{
public:
	DrawUnitLine(int x,int y,const RECT& rc_valid)
		:DrawUnit(x,y,rc_valid){}
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
};

//////////////////////////////////��ˢ////////////////////////////////////////

class DrawUnitBrush	: public DrawUnit
{
public:
	DrawUnitBrush(int x,int y,const RECT& rc_valid)
		:DrawUnit(x,y,rc_valid),
		draw_pos_(0)
	{	
		m_vtPoints.push_back(ui::CPoint(x,y));	
	}
	virtual	void SetEndPoint(int x, int y) override;
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
private:
	void DrawSingleLine(HDC hdc, const ui::CPoint &pt_start, const ui::CPoint &pt_end);
	void DrawSingleLine(Gdiplus::Graphics &graphics, Gdiplus::Brush &brush, Gdiplus::Pen &pen, const ui::CPoint &pt_start, const ui::CPoint &pt_end);
	Gdiplus::Rect GetRectByPt(const ui::CPoint& pt);
private:
	std::vector<ui::CPoint>	m_vtPoints;
	int draw_pos_;	//���ƶȣ����ڸ��ӵ�һ������������ɶȣ�����Ҫÿ�ζ����»���
};

//////////////////////////////////��Բ////////////////////////////////////////

class DrawUnitEllipse : public DrawUnit
{
public:
	DrawUnitEllipse(int x,int y,const RECT& rc_valid)
		:DrawUnit(x,y,rc_valid){}
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
};


//////////////////////////////////��ͷ////////////////////////////////////////

// ��ͷ��Ϣ
struct ArrowInfo 
{
	ArrowInfo(int width, float theta)
	{
		arrow_width_ = width;
		arrow_theta_ = theta;
	}
	int arrow_width_;	// ��ͷ������
	float arrow_theta_; // ��ͷ�Ƕ� 
};

class DrawUnitArrow : public DrawUnit
{
public:
	DrawUnitArrow(int x, int y, const RECT &rc_valid);
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
private:
	void DrawArrow(HDC hdc);
private:
	ArrowInfo arrow_tag_;
};

////////////////////////////////���ֻ�����////////////////////////////////////////

class DrawUnitText : public	DrawUnit
{
public:
	DrawUnitText(int x, int y, const RECT& rcValid, DWORD font_color, BOOL is_generated);

	void SetText(const std::wstring& str_text)	{	str_text_ = str_text; }
	void SetTextColor(COLORREF color)			{	font_color_ = color;}
	void SetItalic(BOOL is_italic)				{	is_font_italic_ = is_italic;}
	void SetBold(BOOL is_bold)					{	is_bold_ = is_bold;}
	void SetUnderLine(BOOL is_underLine)		{	is_font_underline_ = is_underLine;}
	void SetFontSize(int font_height)			{	font_height_ = font_height;}
protected:
	virtual	void RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue);
private:
	void DrawText(HDC hdc);
	void DrawRect(HDC hdc);
private:
	BOOL				is_text_has_generated_;		// �Ƿ��Ѿ����
	std::wstring		str_text_;					// ����
	COLORREF			font_color_;				// ������ɫ
	BOOL				is_font_italic_;			// �Ƿ���б
	BOOL				is_font_underline_;			// �Ƿ���»���
	int					font_height_;				// �����С
	BOOL				is_bold_;					// �Ӵ�
};

#endif  // _NIM_GUI_CAPTURE_FORM_DRAW_UNIT_H_