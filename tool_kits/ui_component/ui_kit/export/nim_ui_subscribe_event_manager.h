#pragma once

#include "nim_ui_dll.h"
#include "module/subscribe_event/subscribe_event_manager.h"
#include "tool_kits/base/memory/singleton.h"

namespace nim_ui
{

/** @class SubscribeEventManager
  * @brief �ṩ�����¼��йصĽӿ�
  * @copyright (c) 2017, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2017/4/6
  */
class NIM_UI_DLL_API SubscribeEventManager
{
public:
	SINGLETON_DEFINE(SubscribeEventManager);

public:
	/**
	* ����/�����¼�����ģ��
	* @param[in] enable	�Ƿ������¼�����ģ��
	* @return void	�޷���ֵ
	*/
	void SetEnabled(bool enable);

	/**
	* �Ƿ��������¼�����ģ��
	* @return bool	true:����, false : ����
	*/
	bool IsEnabled() const;

	/**
	* ��ȡ�¼���Ϣ
	* @param[in] event_type �¼�����
	* @param[in] accid �û�id
	* @param[out] data �¼�
	* @return void	�޷���ֵ
	*/
	bool GetEventData(int event_type, const std::string &accid, nim_comp::EventDataEx &data);

	/**
	* ���ĳ�ʱ���Զ����¶����¼�
	* @return void	�޷���ֵ
	*/
	void StartAutoSubscribe();

	/**
	* ����ָ���û����¼�
	* @param[in] event_type �¼�����
	* @param[in] subscribe_account �����û�uid
	* @return void	�޷���ֵ
	*/
	void SubscribeEvent(int event_type, const std::string &subscribe_account);

	/**
	* ����ָ���û����¼�
	* @param[in] event_type �¼�����
	* @param[in] subscribe_list �����б�
	* @return void	�޷���ֵ
	*/
	void SubscribeEvent(int event_type, const std::list<std::string> &subscribe_list);

	/**
	* ȡ������ָ���û����¼�
	* @param[in] event_type �¼�����
	* @param[in] unsubscribe_account ȡ�������û�uid
	* @return void	�޷���ֵ
	*/
	void UnSubscribeEvent(int event_type, const std::string &unsubscribe_account);

	/**
	* ȡ������ָ���û����¼�
	* @param[in] event_type �¼�����
	* @param[in] unsubscribe_list ȡ�������б�
	* @return void	�޷���ֵ
	*/
	void UnSubscribeEvent(int event_type, const std::list<std::string> &unsubscribe_list);

	/**
	* ����æµ״̬�¼�
	* @param[in] busy �Ƿ�æµ
	* @return EventData	�¼�����
	*/
	static nim::EventData CreateBusyEvent(bool busy);

private:
	SubscribeEventManager() {}
	~SubscribeEventManager() {}
};

}