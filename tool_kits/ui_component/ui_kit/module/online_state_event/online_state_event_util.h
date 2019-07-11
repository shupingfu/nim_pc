#pragma once
#include "module/subscribe_event/online_state_event_helper.h"
#include "module/subscribe_event/subscribe_event_manager.h"

namespace nim_comp
{
	class OnlineStateEventUtil
	{
	public:
		/**
		* �����û��¼���Ϣ��ȡ����״̬ժҪ��Ϣ
		* @param[in] online_client_type ���߿ͻ���
		* @param[in] multi_config ���������Ϣ
		* @param[in] is_simple �Ƿ�Ҫ��Ҫ��Ϣ���������б�չʾ��
		* @return std::wstring ժҪ��Ϣ
		*/
		static std::wstring GetOnlineState(const std::string& accid, const EventDataEx& data, bool is_simple);

	};

}