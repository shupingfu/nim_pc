﻿#include "session_list.h"
#include "module/session/session_manager.h"
#include "gui/main/team_event_form.h"

namespace nim_comp
{

using namespace ui;

SessionList::SessionList(ui::ListBox* session_list)
{
	ASSERT(session_list != NULL);
	session_list_ = session_list;
	sys_msg_unread_ = 0;
	custom_msg_unread_ = 0;

	Box* multispot_and_events = (Box*)GlobalManager::CreateBox(L"main/main_session_multispot_and_event.xml");
	session_list->AddAt(multispot_and_events, 0);

	ButtonBox* btn_events = (ButtonBox*)multispot_and_events->FindSubControl(L"btn_events");
	ButtonBox* btn_multispot_info = (ButtonBox*)multispot_and_events->FindSubControl(L"multispot_info");
	btn_events->AttachClick(nbase::Bind(&SessionList::OnEventsClick, this, std::placeholders::_1));
	btn_multispot_info->AttachClick(nbase::Bind(&SessionList::OnMultispotClick, this, std::placeholders::_1));

	box_unread_sysmsg_ = (Box*)session_list->GetWindow()->FindControl(L"box_unread_sysmsg");
	label_unread_sysmsg_ = (Label*)session_list->GetWindow()->FindControl(L"label_unread_sysmsg");
	box_unread_sysmsg_->SetVisible(false);

	nim::Session::RegChangeCb(nbase::Bind(&SessionList::OnChangeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	
	OnUserInfoChangeCallback cb1 = nbase::Bind(&SessionList::OnUserInfoChange, this, std::placeholders::_1);
	unregister_cb.Add(UserService::GetInstance()->RegUserInfoChange(cb1));
	OnUserPhotoReadyCallback cb2 = nbase::Bind(&SessionList::OnUserPhotoReady, this, std::placeholders::_1, std::placeholders::_2);
	unregister_cb.Add(UserService::GetInstance()->RegUserPhotoReady(cb2));
}

SessionList::~SessionList()
{

}

int SessionList::AdjustMsg(const SessionMsgData &msg)
{
	for (int i = 0; i < session_list_->GetCount(); i++)
	{
		SessionItem* item = dynamic_cast<SessionItem*>(session_list_->GetItemAt(i));
		if (item)
		{
			if (msg.msg_time > item->GetMsgTime())
				return i;
		}

	}
	return -1;
}

SessionItem* SessionList::AddSessionItem(const SessionMsgData &item_data)
{
	std::string id = item_data.session_id;
	auto control = session_list_->FindSubControl(nbase::UTF8ToUTF16(id));
	if (control)
	{
		session_list_->Remove(control);
	}
	int index = AdjustMsg(item_data);

	SessionItem* item = new SessionItem;
	GlobalManager::FillBoxWithCache(item, L"main/session_item.xml");
	if (index >= 0)
		session_list_->AddAt(item, index);
	else
		session_list_->Add(item);

	item->Init(item_data);

	item->AttachAllEvents(nbase::Bind(&SessionList::OnItemNotify, this, std::placeholders::_1));
	
	return item;
}

void SessionList::RemoveAllSessionItem()
{
	//回话列表的第一项是多端同步和消息中心，所以要专门写个清除函数
	int count = session_list_->GetCount();
	for (int i = count - 1; i > 0; --i)
	{
		session_list_->RemoveAt(i);
	}
}

void SessionList::AddUnreadCount(const std::string &id)
{
	std::wstring wid = nbase::UTF8ToUTF16(id);
	SessionItem* item = dynamic_cast<SessionItem*>(session_list_->FindSubControl(wid));
	if (item)
	{
		item->AddUnread();
	}
}

void SessionList::ResetSessionUnread(const std::string &id)
{
	std::wstring wid = nbase::UTF8ToUTF16(id);
	SessionItem* item = dynamic_cast<SessionItem*>(session_list_->FindSubControl(wid));
	if (item)
	{
		item->ResetUnread();
	}
}

void SessionList::UISysmsgUnread(int count)
{
	sys_msg_unread_ = count;
	int all_count = sys_msg_unread_ + custom_msg_unread_;
	if (all_count > 0)
	{
		label_unread_sysmsg_->SetText(nbase::StringPrintf(L"%d", all_count));
		box_unread_sysmsg_->SetVisible(true);
	}
	else
	{
		box_unread_sysmsg_->SetVisible(false);
	}
}

void SessionList::UICustomSysmsgUnread(bool add)
{
	if (add)
	{
		custom_msg_unread_++;
	}
	else
	{
		custom_msg_unread_ = 0;
	}
	int all_count = sys_msg_unread_ + custom_msg_unread_;
	if (all_count > 0)
	{
		label_unread_sysmsg_->SetText(nbase::StringPrintf(L"%d", all_count));
		box_unread_sysmsg_->SetVisible(true);
	}
	else
	{
		box_unread_sysmsg_->SetVisible(false);
	}
}

void SessionList::UpdateSessionInfo(const UserInfo& user_info)
{
	SessionItem* session_item = (SessionItem*)session_list_->FindSubControl(nbase::UTF8ToUTF16(user_info.account));
	if (session_item != NULL)
		session_item->UpdateInfo();
}

void SessionList::DeleteSessionItem(SessionItem* item)
{
	assert(item);
	session_list_->Remove(item);
}

//多端登陆
//[{"app_account":"abc","client_os":"***","client_type":1,"device_id":"***","mac":"***"}]
void SessionList::OnMultispotChange(bool online, Json::Value& json)
{
	if (json.isArray())
	{
		int len = json.size();
		for (int i = 0; i < len; i++)
		{
			MultispotInfo info;
			info.client_type_ = json[i][nim::kNIMPresClientType].asInt();
			info.client_os_ = json[i][nim::kNIMPresOS].asString();
			info.device_id_ = json[i][nim::kNIMPresDeviceID].asString();
			info.mac_ = json[i][nim::kNIMPresMac].asString();
			info.login_time_ = json[i][nim::kNIMPresLoginTime].asInt64();
			if (online)
			{
				map_multispot_infos_[info.client_type_] = info;
			}
			else
			{
				auto it = map_multispot_infos_.find(info.client_type_);
				if (it != map_multispot_infos_.end())
				{
					map_multispot_infos_.erase(it);
				}
			}
		}
	}
	ShowMultispotUI();
}

void SessionList::OnMultispotKickout(const std::list<std::string> &client_ids)
{
	for (auto it : client_ids)
	{
		for (auto it2 = map_multispot_infos_.begin(); it2 != map_multispot_infos_.end(); it2++)
		{
			if (it2->second.device_id_ == it)
			{
				map_multispot_infos_.erase(it2);
				break;
			}
		}
	}
	ShowMultispotUI();
}

void SessionList::ShowMultispotUI()
{
	ui::ButtonBox* show_btn = (ui::ButtonBox*)session_list_->GetWindow()->FindControl(L"multispot_info");
	if (show_btn)
	{
		if (map_multispot_infos_.size() > 0)
		{
			//std::wstring show_tip = L"登陆帐号设备";
			//for (auto& it : map_multispot_infos_)
			//{
			//	show_tip.append(L"\r\n");
			//	show_tip.append(nbase::UTF8ToUTF16(it.second.client_os_));
			//	show_tip.append(L"\r\n");
			//	show_tip.append(GetMessageTime(it.second.login_time_, false));
			//}
			show_btn->SetVisible(true);
			//show_btn->SetToolTipText(show_tip);
		}
		else
		{
			show_btn->SetVisible(false);
		}
	}

	MultispotForm* form = dynamic_cast<MultispotForm*>(WindowsManager::GetInstance()->GetWindow(MultispotForm::kClassName, MultispotForm::kClassName));
	if (form)
	{
		if (map_multispot_infos_.size() > 0)
		{
			form->OnMultispotChange(map_multispot_infos_);
		}
		else
		{
			form->Close();
		}
	}
}

void SessionList::OnChangeCallback(nim::NIMResCode rescode, const std::string& result, int total_unread_counts)
{
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(result, value))
	{
		nim::NIMSessionCommand command = nim::NIMSessionCommand(value[nim::kNIMSessionCommand].asInt());
		switch (command)
		{
		case nim::kNIMSessionCommandAdd:
		case nim::kNIMSessionCommandUpdate:
		case nim::kNIMSessionCommandMsgDeleted:
		{
			SessionMsgData data;
			data.session_id = value[nim::kNIMSessionId].asString();
			data.from_account = value[nim::kNIMSessionMsgFromAccount].asString();
			data.to_type = value[nim::kNIMSessionType].asString() == "0" ? nim::kNIMSessionTypeP2P : nim::kNIMSessionTypeTeam;
			data.msg_body = value[nim::kNIMSessionMsgBody].asString();
			data.msg_attach = value[nim::kNIMSessionMsgAttach].asString();
			data.msg_type = value[nim::kNIMSessionMsgType].asInt();
			data.msg_time = value[nim::kNIMSessionMsgTime].asInt64();
			data.unread_count = value[nim::kNIMSessionUnreadCount].asInt();
			data.msg_status = value[nim::kNIMSessionMsgStatus].asInt();
			AddSessionItem(data);
			if (SessionManager::GetInstance()->IsSessionWndActive(data.session_id))
			{
				ResetSessionUnread(data.session_id);
			}
		}
		break;
		case nim::kNIMSessionCommandRemoveAll:
			RemoveAllSessionItem();
			break;
		case nim::kNIMSessionCommandRemoveAllP2P:
		case nim::kNIMSessionCommandRemoveAllTeam:
		{
			for (int i = session_list_->GetCount() - 1; i >= 0; i--)
			{
				SessionItem* item = dynamic_cast<SessionItem*>(session_list_->GetItemAt(i));
				if (item && (item->GetIsTeam() == (command == nim::kNIMSessionCommandRemoveAllTeam)))
				{
					session_list_->RemoveAt(i);
				}
			}
		}
		break;
		case nim::kNIMSessionCommandRemove:
			break;
		case nim::kNIMSessionCommandAllMsgDeleted:
		case nim::kNIMSessionCommandAllP2PMsgDeleted:
		case nim::kNIMSessionCommandAllTeamMsgDeleted:
		{
			for (int i = session_list_->GetCount() - 1; i >= 0; i--)
			{
				SessionItem* item = dynamic_cast<SessionItem*>(session_list_->GetItemAt(i));
				if (item)
				{
					if (command == nim::kNIMSessionCommandAllMsgDeleted || (item->GetIsTeam() == (command == nim::kNIMSessionCommandAllTeamMsgDeleted)))
					{
						item->ClearMsg();
					}
				}
			}
		}
		break;
		}
	}
}

void SessionList::OnUserInfoChange(const std::list<UserInfo>& uinfos)
{
	for (auto iter = uinfos.cbegin(); iter != uinfos.cend(); iter++)
		UpdateSessionInfo(*iter);
}

void SessionList::OnUserPhotoReady(const std::string& accid, const std::wstring &photo_path)
{
	SessionItem* item = (SessionItem*)session_list_->FindSubControl(nbase::UTF8ToUTF16(accid));
	if (item != NULL)
		item->FindSubControl(L"head_image")->SetBkImage(photo_path);
}

bool SessionList::OnEventsClick(ui::EventArgs* param)
{
	if (param->pSender->GetName() == L"btn_events")
	{
		TeamEventForm* f = WindowsManager::SingletonShow<TeamEventForm>(TeamEventForm::kClassName);
		f->ReloadEvents();
	}

	return true;
}

bool SessionList::OnMultispotClick(ui::EventArgs* param)
{
	if (param->pSender->GetName() == L"multispot_info")
	{
		MultispotForm* f = WindowsManager::SingletonShow<MultispotForm>(MultispotForm::kClassName);
		f->OnMultispotChange(map_multispot_infos_);
	}
	return true;
}

bool SessionList::OnItemNotify(ui::EventArgs* msg)
{
	if(msg->Type == kEventNotify)
	{		
	 	SessionItem* item = dynamic_cast<SessionItem*>(msg->pSender);
	 	assert(item);
	 	if(msg->wParam == SET_DELETE)
	 	{
	 		this->DeleteSessionItem(item);
	 	}
	}
	return true;
}

}