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
		
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void Selected(bool bSelected, bool bTriggerEvent = false) override;
	virtual void Activate() override;

	virtual  std::wstring GetGroup() const;
	virtual void SetGroup(const std::wstring& strGroupName);

protected:
	std::wstring	m_sGroupName;
};

#include "OptionImpl.h"

typedef OptionTemplate<Control> Option;
typedef OptionTemplate<Box> OptionBox;

}

#endif // UI_CONTROL_OPTION_H_
