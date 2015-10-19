#ifndef _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_EDIT_H_
#define _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_EDIT_H_
#pragma once

#include "capture_def.h"
#include "draw_unit.h"
#include "ui_capture_text_edit.h"
#include "ui_color_palette.h"

class CCaptureEdit : public ui::Control
{
public:
	CCaptureEdit();
	~CCaptureEdit();
	virtual void Paint(HDC hDC, const ui::UiRect& rcPaint) override;
	
public:
	// ��������ͼ
	void SetTrackRect(RECT track_rect){ track_rect_ = track_rect; };
	void SetEditMode(EditMode mode){ edit_mode_ = mode; };
	void SetColorPalette(CColorPalette* color_palette) { color_palette_ = color_palette; };
	BOOL OnLButtonDownInCaptureEditMode(POINT point, RECT tool_bar_rect); // �༭ʱ��갴��
	void OnMouseMoveInCaptureEditMode(POINT point, RECT tool_bar_rect); // �༭ʱ����϶�
	void OnLButtonUpInCaptureEditMode(POINT point); // �༭ʱ��굯��
	void DrawExtraUnits(HDC hdc); // �ؼ��ػ桢�����ͼʱ����
	void OnTextEditFinished(); // ���ı��༭����ʱ
	void Undo(); // ����
	void Redo(); // ����
	void ReleaseAllDrawUnits(); // �ͷ����л��Ƶ�Ԫ

protected:
	// �������
	void OnDrawUnitStart(const ui::CPoint &point);
	void OnDrawUnitProcess(const ui::CPoint &point);
	void OnDrawUnitEnd(const ui::CPoint &point);
	void CreateDrawUnit(const ui::CPoint& point);
	void SetEditModeCursor(const ui::CPoint& point);
	void SetNormalCursor();
	void CalcValidEndPoint(ui::CPoint& pt_end);
	void SubmitDrawUnit();
	void ClearHistoryVector();
	void PaintContent();
	//��ȡ����
	void GetExtraUnitsBimap(HDC hdc, HBITMAP& bitmap, DrawUnit::BlurType blur_type = DrawUnit::BLUR_TYPE_NONE, int range = 0);
	void ReleaseHBitmap(HBITMAP& bitmap);
	bool CheckBlurBitmap(HDC hdc, DrawUnit* draw_units_);
	//��ȡָ��ͼƬ����
	void CheckCursor(HCURSOR &cursor, std::wstring image_name);

private:
	std::vector<DrawUnit*>			vec_draw_units_;			// ��ǰ���Ƶ�Ԫ�б�
	std::vector<DrawUnit*>			vec_history_draw_units_;	// ����/�ع����Ƶ�Ԫ�б�
	DrawUnit*						current_draw_unit_;         // ��ǰ���ڻ��Ƶĵ�Ԫ
	BOOL							is_begin_draw_;				// �Ƿ�ʼ����
	ui::UiRect				track_rect_;				// ��ǰ��ͼ������Ҳ�Ǳ༭�Ŀ�����
	EditMode						edit_mode_;					// ��ǰ�༭��ģʽ��ȷ�����Ƶ�ͼ�Σ�
	TCHAR*							str_cursor_;				// ��ǰ�༭����Ҫ��ʾ�����ָ��
	CCaptureTextEdit*				text_edit_;					// �ı��༭ָ��
	CColorPalette*					color_palette_;				// ��ɫ��ָ��
	HBITMAP							extra_bitmap_;
	HBITMAP							blur_bitmap_;
	int								blur_range_;
	DrawUnit::BlurType				blur_type_;
	HCURSOR							cursor_blur_big_;
	HCURSOR							cursor_blur_mid_;
	HCURSOR							cursor_blur_small_;
};
#endif  // _NIM_GUI_CAPTURE_FORM_UI_CAPTURE_EDIT_H_