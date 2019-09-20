#include "draw_unit.h"
#include "capture_def.h"
#include <cmath>

DrawUnit::DrawUnit(int x, int y, const RECT& rc_valid)
:line_width_(knDefaultLineWidth), color_(kdwDefaultColor), rc_valid_(rc_valid),
left_(x), top_(y), right_(x), bottom_(y), blur_range_(0), blur_type_(BLUR_TYPE_NONE), need_save_draw_(false)
{

}
DrawUnit::~DrawUnit()
{
}

void DrawUnit::Render(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	// ��������PEN������DrawUnit��Pen�Ĺ��õģ�����Brush����DrawUnit��ͬ
	HPEN current_pen = CreatePen(PS_SOLID, line_width_, color_);
	HPEN old_pen = (HPEN)SelectObject(hdc, current_pen);
	RenderSelf(hdc, bitmap, is_continue); // ���ദ��
	SelectObject(hdc, old_pen);
	DeleteObject(current_pen);
}

void DrawUnit::MeasurePoint(int &x,int &y)
{
	x = (x < rc_valid_.left ? rc_valid_.left : x);
	x = (x > rc_valid_.right ? rc_valid_.right : x);
	y = (y < rc_valid_.top ? rc_valid_.top : y);
	y = (y > rc_valid_.bottom ? rc_valid_.bottom : y);
}


//////////////////////////////////����////////////////////////////////////////
void DrawUnitRectangle::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	LOGBRUSH lb = {BS_NULL};
	HBRUSH current_brush = CreateBrushIndirect(&lb);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);
    Rectangle(hdc, left_, top_, right_, bottom_);
	SelectObject(hdc, old_brush);
	DeleteObject(current_brush);
}

//////////////////////////////////ֱ��////////////////////////////////////////
void DrawUnitLine::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	MoveToEx(hdc, left_, top_, NULL);
	LineTo(hdc, right_, bottom_);
}

//////////////////////////////////��ˢ////////////////////////////////////////

// ��д����MouseMove�����У��ռ����о����ĵ�
void DrawUnitBrush::SetEndPoint(int x,int y)
{
	ui::CPoint point = m_vtPoints.back();
	if (x != point.x || y != point.y)
	{
		m_vtPoints.push_back(ui::CPoint(x, y));
	}
}

void DrawUnitBrush::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	// ��������MouseMove�����ĵ�
	UINT draw_pos = is_continue ? draw_pos_ : 0;
	need_save_draw_ = false;
	if (blur_type_ != DrawUnit::BLUR_TYPE_NONE)
	{
		if (bitmap && draw_pos < m_vtPoints.size())
		{
			Gdiplus::Graphics graphics(hdc);
			Gdiplus::Bitmap bmp(bitmap, NULL);
			Gdiplus::TextureBrush brush(&bmp);
			Gdiplus::Pen pen(&brush, line_width_);
			if (draw_pos == 0)
			{
				graphics.FillEllipse(&brush, GetRectByPt(m_vtPoints[0]));
				draw_pos = 1;
				need_save_draw_ = true;
			}
			for (size_t i = draw_pos; i < m_vtPoints.size(); i++)
			{
				DrawSingleLine(graphics, brush, pen, m_vtPoints[i - 1], m_vtPoints[i]);
				need_save_draw_ = true;
			}
		}
	} 
	else
	{
		HBRUSH current_brush = CreateSolidBrush(color_);
		HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);
		if (draw_pos == 0)
		{
			draw_pos = 1;
		}
		for (size_t i = draw_pos; i < m_vtPoints.size(); i++)
		{
			DrawSingleLine(hdc, m_vtPoints[i - 1], m_vtPoints[i]);
			need_save_draw_ = true;
		}
		SelectObject(hdc, old_brush);
		DeleteObject(current_brush);
	}
	draw_pos_ = m_vtPoints.size();
}

void DrawUnitBrush::DrawSingleLine(HDC hdc, const ui::CPoint& pt_start, const ui::CPoint& pt_end)
{
	int start_x	= pt_start.x;
	int start_y	= pt_start.y;
	int end_x = pt_end.x;
	int end_y = pt_end.y;

	MeasurePoint(start_x, start_y);
	MeasurePoint(end_x, end_y);

	MoveToEx(hdc, start_x, start_y, NULL);
	LineTo(hdc, end_x, end_y);
}
void DrawUnitBrush::DrawSingleLine(Gdiplus::Graphics &graphics, Gdiplus::Brush &brush, Gdiplus::Pen &pen, const ui::CPoint &pt_start, const ui::CPoint &pt_end)
{
	int start_x = pt_start.x;
	int start_y = pt_start.y;
	int end_x = pt_end.x;
	int end_y = pt_end.y;

	MeasurePoint(start_x, start_y);
	MeasurePoint(end_x, end_y);

	graphics.DrawLine(&pen, Gdiplus::Point(pt_start.x, pt_start.y), Gdiplus::Point(pt_end.x, pt_end.y));
	graphics.FillEllipse(&brush, GetRectByPt(pt_end));
}
Gdiplus::Rect DrawUnitBrush::GetRectByPt(const ui::CPoint& pt)
{
	int x = pt.x;
	int y = pt.y;
	MeasurePoint(x, y);
	return Gdiplus::Rect(x - line_width_/2, y - line_width_/2, line_width_, line_width_);
}

//////////////////////////////////��Բ////////////////////////////////////////
void DrawUnitEllipse::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	LOGBRUSH lb = {BS_NULL};
	HBRUSH current_brush = CreateBrushIndirect(&lb);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);
	Ellipse(hdc, left_, top_, right_, bottom_);
	SelectObject(hdc, old_brush);
	DeleteObject(current_brush);
}

//////////////////////////////////��ͷ////////////////////////////////////////

DrawUnitArrow::DrawUnitArrow(int x,int y,const RECT &rc_valid)
	:DrawUnit(x,y,rc_valid), arrow_tag_(line_width_ * 5 + 5 / line_width_, 0.65f)
{
}

void DrawUnitArrow::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	HBRUSH current_brush = CreateSolidBrush(color_);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);
	arrow_tag_.arrow_width_ = line_width_ * 5 + 5 / line_width_; 	// reset
	DrawArrow(hdc);
	SelectObject(hdc, old_brush);
	DeleteObject(current_brush);
}

// point ��(x,y)תalpha��(����)
ui::CPoint Transform(ui::CPoint point, float x, float y, float alpha)
{
	ui::CPoint ptResult;
	ptResult.x = ROUND(point.x * cos(alpha) - sin(alpha) * point.y + x - x * cos(alpha) + y * sin(alpha));
	ptResult.y = ROUND(point.x * sin(alpha) + cos(alpha) * point.y + y - x * sin(alpha) - y * cos(alpha));
	return ptResult;
}

void DrawUnitArrow::DrawArrow(HDC hdc)
{
	// �ݴ�����������յ�����С�Ͳ�����
	const static int knMinDiff = 3;
	if (abs(left_ - right_) <= knMinDiff &&
		abs(top_ - bottom_) <= knMinDiff)
	{
		return;
	}

	ui::CPoint	pt_start(left_, top_);
	ui::CPoint	pt_end(right_, bottom_);
	ui::CPoint	pt_polys[3]; // �����ε�������

	// ��ֱ��
	MoveToEx(hdc, pt_start.x, pt_start.y, NULL);
	LineTo(hdc, pt_end.x, pt_end.y);

	//�����ˮƽʱ��ͷ�����������
	float diff_y = pt_end.y - pt_start.y;
	float diff_x = pt_end.x - pt_start.x;
	float radius = atan(diff_y/diff_x); // arc_tan(y/x)��Ƕ�
	diff_x = (fabs(diff_x) <= 0.00001) ? (diff_x < 0 ? -0.00001 : 0.00001) :diff_x;
	float half_arrow_width = arrow_tag_.arrow_width_ / 2.0f;
	float f_length = (float)sqrt(diff_y * diff_y + diff_x * diff_x); 
	float arrow_x_length = diff_x >= 0 ? f_length : -f_length;
	float arrow_x_offset =  half_arrow_width / tan(arrow_tag_.arrow_theta_/2) ;
	if (diff_x >= 0)
	{
		arrow_x_offset = - arrow_x_offset;
	}
	ui::CPoint pt_arrow = ui::CPoint(ROUND(pt_start.x + arrow_x_length), pt_start.y);				
	ui::CPoint pt_high = ui::CPoint(ROUND(pt_arrow.x + arrow_x_offset), ROUND (pt_arrow.y - half_arrow_width));
	ui::CPoint pt_low = ui::CPoint(ROUND(pt_arrow.x + arrow_x_offset), ROUND(pt_arrow.y + half_arrow_width));

	//������ת
	float x = pt_start.x;
	float y = pt_start.y;
	pt_polys[0] = Transform(pt_arrow,x,y,radius);
	pt_polys[1] = Transform(pt_high,x,y,radius);
	pt_polys[2] = Transform(pt_low,x,y,radius);

	//���ƶ����
	Polygon(hdc, pt_polys, 3);
}

//////////////////////////////////���ֻ�����////////////////////////////////////////
DrawUnitText::DrawUnitText(int x, int y, const RECT& rcValid, BOOL is_generated) :DrawUnit(x,y,rcValid)
{
	is_text_has_generated_	=	is_generated;
	font_index_ = 3;
}

void DrawUnitText::RenderSelf(HDC hdc, HBITMAP bitmap, bool is_continue)
{
	if (is_text_has_generated_)	
	{
		DrawText(hdc); // ������������֣������ı�
	}
	else	
	{
		DrawRect(hdc); // ��û���ɿؼ�����������л�������ʾ
	}
}

void DrawUnitText::DrawRect(HDC hdc)
{
	// ʹ���ı�����͸�����봰�ڱ���һ��
	int old_bk_mode = SetBkMode(hdc, TRANSPARENT); 
	// ���߻���
	HPEN dot_pen = CreatePen(PS_DOT, 1, color_);
	HPEN old_pen = (HPEN)SelectObject(hdc, dot_pen);
	// ��ˢ
	LOGBRUSH lb = {BS_NULL};
	HBRUSH current_brush = CreateBrushIndirect(&lb);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, current_brush);
	// ������
	Rectangle(hdc, left_, top_, right_, bottom_);
	// �ָ�
	SelectObject(hdc, old_pen);
	SelectObject(hdc, old_brush);
	SetBkMode(hdc, old_bk_mode);
	// �ͷž��
	DeleteObject(current_brush);
	DeleteObject(dot_pen);
}

void DrawUnitText::DrawText(HDC hdc)
{
	int old_bk_mode = SetBkMode(hdc, TRANSPARENT); // ʹ���ı�����͸�����봰�ڱ���һ��
	COLORREF old_color = ::SetTextColor(hdc, color_); //������ɫ
	HFONT font = ui::GlobalManager::GetFont(font_index_);
	HFONT old_font = nullptr;
	if (font)
		old_font = (HFONT)SelectObject(hdc, font); //���������С
	ui::UiRect rc_text(left_, top_, right_, bottom_);
	::DrawText(hdc, str_text_.c_str(), str_text_.size(), &rc_text, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	
	//��ԭ
	::SetBkMode(hdc, old_bk_mode);
	::SetTextColor(hdc, old_color);
	if (old_font)
		::SelectObject(hdc, old_font);
}