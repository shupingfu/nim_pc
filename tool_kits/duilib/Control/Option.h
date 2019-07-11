#ifndef UI_CONTROL_OPTION_H_
#define UI_CONTROL_OPTION_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API OptionTemplate : public CheckBoxTemplate<InheritType>
{
public:
	OptionTemplate();
	~OptionTemplate();
		
	/// ��д���෽�����ṩ���Ի����ܣ���ο���������
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void Selected(bool bSelected, bool bTriggerEvent = false) override;
	virtual void Activate() override;

	/**
	 * @brief ��ȡ����������
	 * @return ����������
	 */
	virtual  std::wstring GetGroup() const;

	/**
	 * @brief ����������
	 * @param[in] strGroupName ������
	 * @return ��
	 */
	virtual void SetGroup(const std::wstring& strGroupName);

protected:
	std::wstring	m_sGroupName;
};

#include "OptionImpl.h"

typedef OptionTemplate<Control> Option;
typedef OptionTemplate<Box> OptionBox;

}

#endif // UI_CONTROL_OPTION_H_
