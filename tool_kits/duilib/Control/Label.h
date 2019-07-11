#ifndef UI_CONTROL_LABEL_H_
#define UI_CONTROL_LABEL_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API LabelTemplate : public InheritType
{
public:
	LabelTemplate();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual std::wstring GetText() const;
	virtual std::string GetUTF8Text() const;
	virtual void SetText(const std::wstring& strText);
	virtual void SetUTF8Text(const std::string& strText);
	virtual void SetTextId(const std::wstring& strTextId);
	virtual void SetUTF8TextId(const std::string& strTextId);
	virtual bool HasHotState();
	virtual CSize EstimateText(CSize szAvailable, bool& bReEstimateSize) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintText(IRenderContext* pRender) override;

	/**
	 * @brief �����ı���ʽ
	 * @param[in] uStyle Ҫ���õ���ʽ
	 * @return ��
	 */
	void SetTextStyle(UINT uStyle);

	/**
	 * @brief ��ȡ�ı���ʽ
	 * @return �����ı���ʽ
	 */
	UINT GetTextStyle() const;

	/**
	 * @brief ��ȡָ��״̬�µ��ı���ɫ
	 * @param[in] stateType Ҫ��ȡ��״̬��־
	 * @return ����ָ��״̬�µ��ı���ɫ
	 */
	std::wstring GetStateTextColor(ControlStateType stateType);

	/**
	 * @brief ����ָ��״̬�µ��ı���ɫ
	 * @param[in] stateType Ҫ���õ�״̬��־
	 * @param[in] dwTextColor Ҫ���õ�״̬��ɫ�ַ�������ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

	/**
	 * @brief ��ȡ��ǰ������
	 * @return ���������ţ��ñ���� global.xml �б�ʶ
	 */
	int GetFont() const;

	/**
	 * @brief ���õ�ǰ����
	 * @param[in] index Ҫ���õ������ţ��ñ�ű����� global.xml �д���
	 * @return ��
	 */
	void SetFont(int index);

	/**
	 * @brief ��ȡ���ֱ߾�
	 * @return �������ֵı߾���Ϣ
	 */
	UiRect GetTextPadding() const;

	/**
	 * @brief �������ֱ߾���Ϣ
	 * @param[in] rc �߾���Ϣ
	 * @return ��
	 */
	void SetTextPadding(UiRect rc);

	/**
	 * @brief �ж��Ƿ��ǵ���ģʽ
	 * @return ���� true ��ʾ����ģʽ������Ϊ false
	 */
	bool IsSingleLine();

	/**
	 * @brief ����Ϊ��������ģʽ
	 * @param[in] bSingleLine Ϊ true ʱΪ����ģʽ������Ϊ false
	 * @return ��
	 */
	void SetSingleLine(bool bSingleLine);

	/**
	 * @brief �Ƿ������������
	 * @return ���� true Ϊ���ƣ�false Ϊ������
	 */
	bool IsLineLimit();

	/**
	 * @brief �����������
	 * @param[in] bLineLimit ���� true Ϊ���ƣ�false Ϊ������
	 * @return ��
	 */
	void SetLineLimit(bool bLineLimit);

protected:
	int		m_iFont;
	UINT	m_uTextStyle;
	bool    m_bSingleLine;
	bool    m_bLineLimit;
	int		m_hAlign;
	int		m_vAlign;
	UiRect	m_rcTextPadding;
	std::wstring	m_sText;
	std::wstring	m_sTextId;
	StateColorMap	m_textColorMap;
};

#include "LabelImpl.h"

typedef LabelTemplate<Control> Label;
typedef LabelTemplate<Box> LabelBox;

}

#endif // UI_CONTROL_LABEL_H_