#pragma once

#include "nim_ui_dll.h"
#include "module/service/notify_center.h"
#include "tool_kits/base/memory/singleton.h"

namespace nim_ui
{

/** @class NotifyCenter
  * @brief �¼�֪ͨ����
  * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2017/4/20
  */
class NIM_UI_DLL_API NotifyCenter
{
public:
	SINGLETON_DEFINE(NotifyCenter);

public:
	/**
	* ע���¼�֪ͨ�Ļص�����
	* @param[in] type �¼�����
	* @param[in] notify �ص�����
	* @return UnregisterCallback ��ע�����
	*/
	UnregisterCallback RegNotify(NotifyType type, OnNotify notify);

private:

	NotifyCenter() {}
	~NotifyCenter() {}
};

}