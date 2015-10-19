#ifndef _NIM_GUI_CAPTURE_FORM_CAPTURE_DEF_H_
#define _NIM_GUI_CAPTURE_FORM_CAPTURE_DEF_H_
#pragma once

#include "duilib/UIlib.h"

// �༭ģʽ
enum EditMode
{	
	EM_NONE,			// δѡ��
	EM_RECTANGLE,		// ����
	EM_ELLIPSE,			// ��Բ
	EM_LINE,			// ֱ��
	EM_ARROW,			// ��ͷ
	EM_TEXT,			// ����
	EM_BRUSH,			// ��ˢ (ʵ����N���߶�)
	EM_MOSAIC,			// ��ˢ���͵ļ�ǿ����ʾΪ������
	EM_GAUSS,			// ��ˢ���͵ļ�ǿ����ʾΪë����
};

// ���Ƶ�Ԫ��������
enum DrawUnitParamType
{
	DUPT_NONE,
	DUPT_LINEWIDTH,		// �߿�
	DUPT_COLOR,			// ��ɫ
	DUPT_FONTSIZE,		// �����С
	DUPT_BOLD,			// �Ӵ�
	DUPT_UNDERLINE,		// �»���
	DUPT_ITALIC,		// б��
};

// �߿�
enum LineWidth		
{
	LW_1POUND		=	1,
	LW_1_5POUNDS	=	2,
	LW_3POUNDS		=	3,
	LW_4_5POUNDS	=	4,
	LW_6POUNDS		=	5,
};

const DWORD	kdwDefaultColor		=	RGB(0xe6,0x00,0x13);	// Ĭ����ɫ
const int	knDefaultFontSize	=	18;						// Ĭ�������С
const int	knDefaultLineWidth	=	LW_1POUND;				// Ĭ���ߴ�
const int	knFontSizeLength	=	14;
const int	vtFontSize[knFontSizeLength] = {8,9,10,11,knDefaultFontSize,14,16,18,20,22,24,26,28,36};
const int	knDefaultIndex = 4;
const int	knEditOffset = 3;

// ������������
template <typename FloatType>
inline int __roundToInt (const FloatType value) throw()
{
	union { int asInt[2]; double asDouble; } n;
	n.asDouble = ((double) value) + 6755399441055744.0;

	return n.asInt [0];
}

// ��������, ������
#define   ROUND(X)  __roundToInt(X)

#endif  // _NIM_GUI_CAPTURE_FORM_CAPTURE_DEF_H_