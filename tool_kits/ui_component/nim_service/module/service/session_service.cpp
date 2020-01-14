#include "session_service.h"
#include "nim_service/module/subscribe_event/subscribe_event_manager.h"

namespace nim_comp
{
void SessionService::InvokeLoadSessionList()
{
	nim::Session::QueryAllRecentSessionAsync(nbase::Bind(&nim_comp::SessionService::OnQuerySessionListCallback, std::placeholders::_1, std::placeholders::_2));
}

void SessionService::InvokeLoadOnlineSessionList()
{
	nim::SessionOnLineService::QuerySessionList(0, online_session_latest_timestamp_, true, 20, nbase::Bind(&nim_comp::SessionService::OnQuerySessionOnlineListCallback, std::placeholders::_1));
}

nim::SessionData SessionService::GetSessionItem(const std::string &session_id)
{
	auto iter = sessions_.find(session_id);
	if (iter != sessions_.end())
		return iter->second;
	else
		return nim::SessionData();
}

void SessionService::OnQuerySessionList(const std::list<nim::SessionData>& sessions)
{
	if (sessions.empty())
		return;

	// ����Ự�б���ڵ�¼�������յ�����ʱ��������ȡ�漰�����û���Ϣ����Щ�û���ӵ���ѯ������
	// ���ⴴ���Ự�б���ʱ���б���ؼ���ѯ�û���Ϣ������Ƶ�������û���Ϣ��ȡ�ӿ�
	std::list<std::string> user_ids;
	for each (auto& session in sessions)
	{
		if (session.type_ == nim::kNIMSessionTypeP2P)
		{
			user_ids.push_back(session.id_);
		}
		else
		{
			user_ids.push_back(session.msg_sender_accid_);
		}
	}
	nim_comp::UserService::GetInstance()->DoQueryUserInfos(user_ids);
	nim_comp::SubscribeEventManager::GetInstance()->SubscribeAllSessionEvent(sessions);

	for each (auto& session in sessions)
	{
		auto iter = sessions_.find(session.id_);
		if (iter != sessions_.end())
		{
			if (nim::kNIMSessionCommandMsgDeleted != session.command_ && iter->second.msg_timetag_ > session.msg_timetag_)
			{
				sessions_[session.id_] = session;
			}
			break;
		}
		else
		{
			sessions_[session.id_] = session;
		}
	}

	if (session_data_changed_callback_ != nullptr)
		(*session_data_changed_callback_)(sessions);
}

void SessionService::OnQueryOnlineSessionList(int res_code, bool has_more, const nim::SessionOnLineServiceHelper::SessionList& sessions)
{
	// �����ѯ�����ķ�������Ϣ
	for (auto& session : sessions)
	{
		auto iter = online_sessions_.find(session.id_);
		if (iter != online_sessions_.end())
		{
			// ������ظ��ģ���ʱ����£���ʹ�÷�������ȡ����������
			if (session.update_time_ > iter->second.update_time_)
				online_sessions_[session.id_] = session;
		}
		else
		{
			online_sessions_[session.id_] = session;
		}

		QLOG_APP(L"[SessionService] On query online session list: session id: {0}, update timestamp: {1}")
			<< session.id_ << session.update_time_;
		online_session_latest_timestamp_ = session.update_time_;
	}

	if (received_online_session_data_callback_ != nullptr)
		(*received_online_session_data_callback_)(has_more, sessions);
}

void SessionService::OnQuerySessionListCallback(int unread_count, const nim::SessionDataList& session_list)
{
	QLOG_PRO(L"local session list: count :{0} - unread :{1}") << session_list.count_ << session_list.unread_count_;
	SessionService::GetInstance()->OnQuerySessionList(session_list.sessions_);
}

void SessionService::OnQuerySessionOnlineListCallback(const nim::SessionOnLineServiceHelper::QuerySessionListResult& online_session_list)
{
	QLOG_APP(L"online session list size: {0}, result code: {1}, has more: {2}") 
		<< online_session_list.session_list_.size()
		<< online_session_list.res_code
		<< online_session_list.has_more_;

	SessionService::GetInstance()->OnQueryOnlineSessionList(
		online_session_list.res_code,
		online_session_list.has_more_,
		online_session_list.session_list_
	);
}

}