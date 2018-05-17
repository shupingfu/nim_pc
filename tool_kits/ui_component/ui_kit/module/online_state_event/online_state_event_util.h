#pragma once
#include "nim_service/module/subscribe_event/online_state_event_helper.h"

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
		static std::wstring GetOnlineState(const nim::EventOnlineClientType& online_client_type, const EventMultiConfig& multi_config, bool is_simple);

	};

}