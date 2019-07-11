#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
	CheckBoxTemplate();

	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual void Activate() override;
	virtual Image* GetEstimateImage() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusColor(IRenderContext* pRender) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void PaintText(IRenderContext* pRender) override;
	virtual bool HasHotState();

	/**
	 * @brief �жϵ�ǰ�Ƿ���ѡ��״̬
	 * @return ���� true Ϊѡ��״̬������Ϊ false
	 */
	bool IsSelected() const { return m_bSelected; }

	/**
	 * @brief ���ÿؼ��Ƿ�ѡ��״̬
	 * @param[in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
	 * @param[in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
	 * @return ��
	 */
	virtual void Selected(bool bSelected, bool bTriggerEvent = false);

	/**
	 * @brief ��ȡ��ѡ��ʱ��ͼƬ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
	 * @return ����ͼƬλ��
	 */
	std::wstring GetSelectedStateImage(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ��ͼƬ
	 * @param[in] stateType Ҫ��������״̬�µ�ͼƬ
	 * @param[in] strImage ͼƬ��ַ
	 * @return ��
	 */
	void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief ��ȡ��ѡ��ʱ���ı���ɫ
	 * @return ���ر�ѡ��ʱ���ı���ɫ
	 */
	std::wstring GetSelectedTextColor();

	/**
	 * @brief ���ñ�ѡ��ʱ���ı���ɫ
	 * @param[in] dwTextColor Ҫ���õ���ɫ�ַ���������ɫ������ global.xml �д���
	 * @return ��
	 */
	void SetSelectedTextColor(const std::wstring& dwTextColor);

	/**
	 * @brief ��ȡ��ѡ��ʱ�Ŀؼ���ɫ 
	 * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
	 * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
	 */
	std::wstring GetSelectStateColor(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ�Ŀؼ���ɫ
	 * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
	 * @param[in] stateColor Ҫ���õ���ɫ
	 * @return ��
	 */
	void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

	/**
	 * @brief ��ȡ��ѡ��ʱ��ǰ��ͼƬ
	 * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
	 * @return ����ͼƬλ��
	 */
	std::wstring GetSelectedForeStateImage(ControlStateType stateType);

	/**
	 * @brief ���ñ�ѡ��ʱ��ǰ��ͼƬ
	 * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
	 * @param[in] pStrImage ͼƬλ��
	 * @return ��
	 */
	void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage);

	/**
	 * @brief ������ѡ��ʱ���¼�
	 * @param[in] callback ��ѡ��ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback; }

	/**
	 * @brief ����ȡ��ѡ��ʱ���¼�
	 * @param[in] callback ȡ��ѡ��ʱ�����Ļص�����
	 * @return ��
	 */
	void AttachUnSelect(const EventCallback& callback) { OnEvent[kEventUnSelect] += callback; }

protected:
	bool			m_bSelected;
	std::wstring	m_dwSelectedTextColor;
	StateColorMap	m_selectedColorMap;
};

#include "CheckBoxImpl.h"

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_#ifndef UI_CONTROL_CHECKBOX_H_