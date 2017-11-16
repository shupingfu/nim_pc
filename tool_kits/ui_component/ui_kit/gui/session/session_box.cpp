﻿#include "session_box.h"
#include "session_form.h"
#include "gui/session/control/audio_capture.h"
#include "gui/session/control/atme_view.h"
#include "gui/session/atlist/at_list_form.h"
#include "gui/session/taskbar/taskbar_manager.h"
#include "module/session/session_util.h"
#include "module/session/session_manager.h"
#include "module/session/force_push_manager.h"
#include "module/audio/audio_manager.h"
#include "callback/session/session_callback.h"
#include "export/nim_ui_window_manager.h"
#include "util/user.h"

using namespace ui;

namespace
{
	const int kMsgLogNumberShow = 20;
}

namespace nim_comp
{

const LPCTSTR SessionBox::kClassName = L"SessionForm";

SessionBox::SessionBox(std::string id, nim::NIMSessionType type)
{
	btn_max_restore_ = NULL;
	input_edit_ = NULL;

	session_id_ = id;
	session_type_ = type;

	is_header_enable_ = false;

	first_show_msg_ = true;
	last_msg_time_ = 0;
	farst_msg_time_ = 0;

	has_writing_cell_ = false;
	writing_time_ = 0;
	is_team_valid_ = true;

	mute_all_ = false;

	taskbar_item_ = NULL;

	mark_receipt_when_load_msgs_ = false;
}

SessionBox::~SessionBox()
{

}

SessionForm* SessionBox::GetSessionForm() const
{
	ASSERT(NULL != session_form_);
	ASSERT(::IsWindow(session_form_->GetHWND()));
	return session_form_;
}

void SessionBox::InitSessionBox()
{
	ASSERT(session_form_ != NULL);

	UserService::GetInstance()->GetRobotInfo(session_id_, robot_info_);
	is_robot_session_ = !robot_info_.GetAccid().empty();

	this->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&SessionBox::Notify, this, std::placeholders::_1));
	this->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&SessionBox::OnClicked, this, std::placeholders::_1));
	this->AttachBubbledEvent(ui::kEventSelect, nbase::Bind(&SessionBox::OnSelChanged, this, std::placeholders::_1));
	this->AttachBubbledEvent(ui::kEventUnSelect, nbase::Bind(&SessionBox::OnSelChanged, this, std::placeholders::_1));

	label_title_ = (Label*)FindSubControl(L"label_title");
	label_tid_ = (Label*)FindSubControl(L"label_tid");
	label_online_state_ = (Label*)FindSubControl(L"label_online_state");

	btn_header_ = (Button*)FindSubControl(L"btn_header");
	btn_header_->AttachClick(nbase::Bind(&SessionBox::OnBtnHeaderClick, this, std::placeholders::_1));

	btn_invite_ = (Button*)FindSubControl(L"btn_invite");
	btn_invite_->SetVisible(false);
	btn_invite_->AttachClick(nbase::Bind(&SessionBox::OnBtnInvite, this, std::placeholders::_1));

	btn_capture_audio_ = (Option*)FindSubControl(L"btn_capture_audio");
	Button* btn_audio = (Button*)FindSubControl(L"btn_audio");
	Button* btn_video = (Button*)FindSubControl(L"btn_video");
	Button* btn_rts = (Button*)FindSubControl(L"btn_rts");
	if (session_type_ == nim::kNIMSessionTypeP2P && !IsFileTransPhone())
	{
		btn_audio->SetVisible(true);
		btn_video->SetVisible(true);
		btn_rts->SetVisible(true);
	}

	btn_max_restore_ = (Button*)FindSubControl(L"btn_max_restore");

	msg_content_ = (Box*)FindSubControl(L"msg_content");
	msg_list_ = (ListBox*)FindSubControl(L"msg_list");
	btn_face_ = (CheckBox*)FindSubControl(L"btn_face");
	input_edit_ = (RichEdit*)FindSubControl(L"input_edit");
	input_edit_->SetLimitText(5000);
	input_edit_->SetNoCaretReadonly();
	input_edit_->AttachReturn(nbase::Bind(&SessionBox::OnInputEditEnter, this, std::placeholders::_1));
	btn_send_ = (Button*)FindSubControl(L"btn_send");

	btn_new_broad_ = (Button*)FindSubControl(L"btn_new_broad");
	btn_new_broad_->SetVisible(false);
	edit_broad_ = (RichEdit*)FindSubControl(L"re_broad");
	edit_broad_->SetNoCaretReadonly();
	label_member_ = (Label*)FindSubControl(L"label_member");
	btn_refresh_member_ = (Button*)FindSubControl(L"btn_refresh_member");
	btn_refresh_member_->SetEnabled(false);
	member_list_ = (ListBox*)FindSubControl(L"member_list");

	ITextServices * text_services = input_edit_->GetTextServices();
	richedit_ole_callback_ = new IRichEditOleCallbackEx(text_services, std::function<void()>());
	text_services->Release();
	input_edit_->SetOleCallback(richedit_ole_callback_);
	richedit_ole_callback_->SetCustomMode(true);

	IDropTarget *pdt = input_edit_->GetTxDropTarget();
	if (pdt)
	{
		input_edit_droptarget_ = pdt;
	}

	atme_view_ = new AtMeView;
	atme_view_->InitControl();
	msg_content_->Add(atme_view_);
	atme_view_->AttachBubbledEvent(kEventClick, nbase::Bind(&SessionBox::OnAtMeViewClick, this, std::placeholders::_1));
	Button* button_close_at_me = static_cast<Button*>(atme_view_->FindSubControl(L"btn_close_at_me"));
	button_close_at_me->AttachClick(nbase::Bind(&SessionBox::OnCloseAtMeView, this, std::placeholders::_1));

	// 语音录制界面显示优先级高，放到最上层
	audio_capture_view_ = new AudioCaptureView;
	audio_capture_view_->InitControl(session_id_, nbase::Bind(&SessionBox::OnAudioCaptureComplete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	msg_content_->Add(audio_capture_view_);

	// 初始化任务栏缩略图
	{
		taskbar_item_ = new TaskbarTabItem(this);
		if (taskbar_item_)
			taskbar_item_->Init(nbase::UTF8ToUTF16(session_id_));
	}

	if (is_robot_session_)
	{
		btn_audio->SetVisible(false);
		btn_video->SetVisible(false);
		btn_rts->SetVisible(false);
		btn_face_->SetVisible(false);
		btn_capture_audio_->SetVisible(false);
		FindSubControl(L"btn_image")->SetVisible(false);
		FindSubControl(L"btn_snapchat")->SetVisible(false);
		FindSubControl(L"btn_file")->SetVisible(false);
		FindSubControl(L"btn_jsb")->SetVisible(false);
		FindSubControl(L"btn_tip")->SetVisible(false);
		FindSubControl(L"btn_clip")->SetVisible(false);
		FindSubControl(L"btn_custom_msg")->SetVisible(false);
	}
	else
	{
		OnSelectAtItem cb = nbase::Bind(&SessionBox::OnSelectAtItemCallback, this, std::placeholders::_1, std::placeholders::_2);
		AtlistForm *at_list_form = new AtlistForm(session_id_, session_type_, ToWeakCallback(cb));
		at_list_form->Create(this->GetWindow()->GetHWND(), L"", WS_POPUPWINDOW, 0L);
		nim::RobotInfos infos;
		UserService::GetInstance()->GetAllRobotInfo(infos);
		at_list_form->InitRobotInfos(infos);
	}

	CheckHeader();
	CheckTeamType(nim::kNIMTeamTypeNormal);
	OnWndSizeMax(TRUE == IsZoomed(this->GetWindow()->GetHWND()));

	if (session_type_ == nim::kNIMSessionTypeTeam)
	{
		FindSubControl(L"btn_snapchat")->SetVisible(false);
		InvokeGetTeamMember(); 
		InvokeGetTeamInfo();
	}
	else
	{
		is_header_enable_ = true;
	}

	unregister_cb.Add(NotifyCenter::GetInstance()->RegNotify(NT_LINK, nbase::Bind(&SessionBox::OnRelink, this, std::placeholders::_1)));
	unregister_cb.Add(UserService::GetInstance()->RegUserInfoChange(nbase::Bind(&SessionBox::OnUserInfoChange, this, std::placeholders::_1)));
	unregister_cb.Add(PhotoService::GetInstance()->RegPhotoReady(nbase::Bind(&SessionBox::OnUserPhotoReady, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	if (session_type_ == nim::kNIMSessionTypeTeam)
	{
		unregister_cb.Add(TeamService::GetInstance()->RegAddTeamMember(nbase::Bind(&SessionBox::OnTeamMemberAdd, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegRemoveTeamMember(nbase::Bind(&SessionBox::OnTeamMemberRemove, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamMember(nbase::Bind(&SessionBox::OnTeamMemberChange, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegSetTeamAdmin(nbase::Bind(&SessionBox::OnTeamAdminSet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamOwner(nbase::Bind(&SessionBox::OnTeamOwnerChange, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamName(nbase::Bind(&SessionBox::OnTeamNameChange, this, std::placeholders::_1)));
		unregister_cb.Add(TeamService::GetInstance()->RegRemoveTeam(nbase::Bind(&SessionBox::OnTeamRemove, this, std::placeholders::_1)));
		unregister_cb.Add(TeamService::GetInstance()->RegAddTeam(nbase::Bind(&SessionBox::OnTeamAdd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		unregister_cb.Add(TeamService::GetInstance()->RegMuteMember(nbase::Bind(&SessionBox::OnTeamMuteMember, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	}
	else
	{
		auto receive_event_cb = nbase::Bind(&SessionBox::OnReceiveEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		unregister_cb.Add(SubscribeEventManager::GetInstance()->RegReceiveEventCallback(receive_event_cb));
	}

	auto robot_list_change_cb = nbase::Bind(&SessionBox::OnRobotChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	unregister_cb.Add(UserService::GetInstance()->RegRobotListChange(robot_list_change_cb));
}

void SessionBox::UninitSessionBox()
{
	AudioManager::GetInstance()->StopPlayAudio(session_id_);
	SessionManager::GetInstance()->RemoveSessionBox(session_id_, this);

	if (taskbar_item_)
	{
		taskbar_item_->UnInit();
	}

	AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::UTF8ToUTF16(session_id_));
	if (at_list_form)
		at_list_form->CloseForm();

	if (input_edit_droptarget_)
	{
		input_edit_droptarget_->Release();
		input_edit_droptarget_ = NULL;
	}
}

void SessionBox::InvokeShowMsgs(bool first_show_msg)
{
	first_show_msg_ = first_show_msg;

	QLOG_APP(L"query begin: id={0} type={1} farst_time={2}") << session_id_ << session_type_ << farst_msg_time_;

	nim::MsgLog::QueryMsgAsync(session_id_, session_type_, kMsgLogNumberShow, farst_msg_time_,
		nbase::Bind(&TalkCallback::OnQueryMsgCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void SessionBox::AddNewMsg(const nim::IMMessage &msg, bool create)
{
	bool at_end = msg_list_->IsAtEnd();

	bool show_time = false;
	if (first_show_msg_)
	{
		show_time = true;
		farst_msg_time_ = msg.timetag_;
	}
	else
	{
		show_time = CheckIfShowTime(last_msg_time_, msg.timetag_);
	}
	ShowMsg(msg, false, show_time);

	if(!IsNoticeMsg(msg))
		last_msg_time_ = msg.timetag_;

	if (first_show_msg_)
	{
		first_show_msg_ = false;
		AddTip(STT_LOAD_MORE_MSG);
	}

	RemoveTip(STT_WRITING);
	writing_time_ = 0;

	bool flash = true;
	if (msg.feature_ == nim::kNIMMessageFeatureSyncMsg || msg.type_ == nim::kNIMMessageTypeNotification)
		flash = false;
	session_form_->OnNewMsg(*this, create, flash);
	
	if(at_end)
	{
		msg_list_->EndDown(true, false);
	}

	SendReceiptIfNeeded(true);
	AddAtMessage(msg);
}

MsgBubbleItem* SessionBox::ShowMsg(const nim::IMMessage &msg, bool first, bool show_time)
{
	const std::string &bubble_id = msg.client_msg_id_;
	if(bubble_id.empty())
	{
		QLOG_WAR(L"msg id empty");
		return nullptr;
	}

	IdBubblePair::iterator it = id_bubble_pair_.find(bubble_id);
	if(it != id_bubble_pair_.end())
	{
		QLOG_WAR(L"repeat msg: {0}") <<bubble_id;
		return nullptr;
	}

	MsgBubbleItem* item = NULL;

	if (msg.type_ == nim::kNIMMessageTypeText 
		|| IsNetCallMsg(msg.type_, msg.attach_))
	{
		Json::Value values;
		Json::Reader reader;
		if (reader.parse(msg.attach_, values) 
			&& values.isObject() 
			&& values.isMember("comment")
			&& values["comment"].asString() == "is_recall_notification")
		{
			nim::IMMessage notify_msg = msg;
			if (values.isMember("notify_from"))
			{
				std::string from_id = values["notify_from"].asString();
				std::string from_nick = values["from_nick"].asString();
				notify_msg.content_ = nbase::UTF16ToUTF8(GetRecallNotifyText(from_id, from_nick));
			}

			MsgBubbleNotice* cell = new MsgBubbleNotice;
			GlobalManager::FillBoxWithCache(cell, L"session/cell_notice.xml");
			if (first)
				msg_list_->AddAt(cell, 0);
			else
				msg_list_->Add(cell);
			cell->InitControl();
			cell->InitInfo(notify_msg, session_id_, true);
			return nullptr;
		}
		else
		{
			item = new MsgBubbleText;
		}		
	}
	else if (msg.type_ == nim::kNIMMessageTypeImage)
		item = new MsgBubbleImage;
	else if (msg.type_ == nim::kNIMMessageTypeAudio)
		item = new MsgBubbleAudio;
	else if (msg.type_ == nim::kNIMMessageTypeLocation)
		item = new MsgBubbleLocation;
	else if (msg.type_ == nim::kNIMMessageTypeFile)
		item = new MsgBubbleFile;
	else if (msg.type_ == nim::kNIMMessageTypeVideo)
		item = new MsgBubbleVideo;
	else if (msg.type_ == nim::kNIMMessageTypeNotification || msg.type_ == nim::kNIMMessageTypeTips)
	{
		id_bubble_pair_[bubble_id] = NULL;

		MsgBubbleNotice* cell = new MsgBubbleNotice;
		GlobalManager::FillBoxWithCache(cell, L"session/cell_notice.xml");
		if(first)
			msg_list_->AddAt(cell, 0);
		else
			msg_list_->Add(cell);
		cell->InitControl();
		cell->InitInfo(msg, session_id_);
		return nullptr;
	}
	else if (msg.type_ == nim::kNIMMessageTypeCustom)
	{
		Json::Value json;
		if (StringToJson(msg.attach_, json) && json.isObject())
		{
			int sub_type = json["type"].asInt();
			if (sub_type == CustomMsgType_Jsb) //finger
			{
				item = new MsgBubbleFinger;
			}
			else if (sub_type == CustomMsgType_SnapChat)
			{
				item = new MsgBubbleSnapChat;
			}
			else if (sub_type == CustomMsgType_Sticker)
			{
				item = new MsgBubbleSticker;
			}
			else if (sub_type == CustomMsgType_Rts)
			{
				if (json["data"].isObject())
				{
					int flag = json["data"]["flag"].asInt();
					if (flag == 0)
					{
						item = new MsgBubbleText;
					}
					else if (flag == 1)
					{
						id_bubble_pair_[bubble_id] = NULL;

						MsgBubbleNotice* cell = new MsgBubbleNotice;
						GlobalManager::FillBoxWithCache(cell, L"session/cell_notice.xml");
						if (first)
							msg_list_->AddAt(cell, 0);
						else
							msg_list_->Add(cell);
						cell->InitControl();
						cell->InitInfo(msg, session_id_);
						return nullptr;
					}
				}
			}
		}
	}
	else if (msg.type_ == nim::kNIMMessageTypeRobot)
	{
		item = new MsgBubbleRobot;
	}
	if (item == nullptr)
	{
		QLOG_WAR(L"unknown msg: cid={0} msg_type={1}") << bubble_id << msg.type_;
		item = new MsgBubbleUnknown;
	}

	bool bubble_right = IsBubbleRight(msg);
	if(bubble_right)
		GlobalManager::FillBoxWithCache( item, L"session/bubble_right.xml" );
	else
		GlobalManager::FillBoxWithCache( item, L"session/bubble_left.xml" );

	if(first)
		msg_list_->AddAt(item, 0);
	else
		msg_list_->Add(item);

	item->InitControl(bubble_right);
	item->InitInfo(msg);
	item->SetSessionId(session_id_);
	item->SetSessionType(session_type_);
	item->SetShowTime(show_time);
	if (bubble_right || msg.session_type_ == nim::kNIMSessionTypeP2P)
		item->SetShowName(false, "");
	else
	{
		auto iter = team_member_info_list_.find(msg.sender_accid_);
		if (iter != team_member_info_list_.cend() && !iter->second.GetNick().empty())
			item->SetShowName(true, iter->second.GetNick()); //显示群名片
		else
		{
			std::string show_name = nbase::UTF16ToUTF8(UserService::GetInstance()->GetUserName(msg.sender_accid_));
			item->SetShowName(true, show_name); //显示备注名或昵称
		}
	}

	id_bubble_pair_[bubble_id] = item;
	if (first)//第一次打开，顺序倒序
		cached_msgs_bubbles_.insert(cached_msgs_bubbles_.begin(), 1, item);
	else
		cached_msgs_bubbles_.push_back(item);

	return item;
}
void SessionBox::InvokeShowSpecifiedCountMsgs(unsigned count)
{
	QLOG_APP(L"query begin: id={0} type={1} farst_time={2}") << session_id_ << session_type_ << farst_msg_time_;

	nim::MsgLog::QueryMsgAsync(session_id_, session_type_, count, farst_msg_time_,
		nbase::Bind(&TalkCallback::OnQueryMsgCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}
void SessionBox::ShowMsgs(const std::vector<nim::IMMessage> &msg)
{
	int pos = msg_list_->GetScrollRange().cy - msg_list_->GetScrollPos().cy;

	bool show_time = false;
	bool show_fast_message = false;	
	//msg倒序排列
	size_t len = msg.size();
	for (size_t i = 0; i < len; i++)
	{
		if (i == len - 1)
		{
			//如果最后一条是提示消息被拒收的消息，要把被拒收的消息加载出来
			show_fast_message = (msg[i].type_ == nim::kNIMMessageTypeTips && msg[i].status_ == nim::kNIMMsgLogStatusRefused);
		}
		if (len == 1 || i == len - 1)
		{
			show_time = true;
		}
		else
		{
			long long older_time = 0;
			for (size_t j = i + 1; j < len; j++)
			{
				if (!IsNoticeMsg(msg[j]))
				{
					older_time = msg[j].timetag_;
					break;
				}
			}
			if (msg[i].type_ == nim::kNIMMessageTypeTips && msg[i].status_ == nim::kNIMMsgLogStatusRefused)
				show_time = false;
			else
				show_time = CheckIfShowTime(older_time, msg[i].timetag_);
		}
		ShowMsg(msg[i], true, show_time);		
	}

	//加载更多历史消息
	AddTip(STT_LOAD_MORE_MSG);
	if (len < kMsgLogNumberShow)
	{
		Box* box = (Box*)msg_list_->FindSubControl(CELL_LOAD_MORE_MSG);
		assert(box);
		Button* btn = (Button*)box->FindSubControl(CELL_BTN_LOAD_MORE_MSG);
		assert(btn);
		btn->SetText(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_ALL_MSGS_SHOWN"));
		btn->SetEnabled(false);
	}
	//修正最近时间
	if (first_show_msg_)
	{
		first_show_msg_ = false;

		msg_list_->EndDown(true, false);

		if (len > 0 && last_msg_time_ == 0)
		{
			for (const auto &i : msg)
			{
				if (!IsNoticeMsg(i))
				{
					last_msg_time_ = i.timetag_;
					break;
				}
			}
		}

		SendReceiptIfNeeded(false);
	}
	else
	{
		msg_list_->SetPos(msg_list_->GetPos());

		ui::CSize sz = msg_list_->GetScrollPos();
		sz.cy = msg_list_->GetScrollRange().cy - pos - 50;
		msg_list_->SetScrollPos(sz);
	}

	if (session_type_ == nim::kNIMSessionTypeP2P && !mark_receipt_when_load_msgs_)
		ResetLastMsgNeedMarkReceipt();
	//修正最远时间
	if (len > 0)
	{
		farst_msg_time_ = msg[len - 1].timetag_;
	}
	if(show_fast_message)
		InvokeShowSpecifiedCountMsgs(1);
}

void SessionBox::AddWritingMsg(const nim::IMMessage &msg)
{
	bool at_end = msg_list_->IsAtEnd();

	Json::Value json;
	if (StringToJson(msg.attach_, json) && json.isObject())
	{
		std::string id = json["id"].asString();
		if (id == "1")
		{
			ShowMsgWriting(msg);
		}
	}

	if (at_end)
	{
		msg_list_->EndDown(true, false);
	}
}

void SessionBox::ShowMsgWriting(const nim::IMMessage &msg)
{
	cancel_writing_timer_.Cancel();
	StdClosure cb = nbase::Bind(&SessionBox::CancelWriting, this);
	auto weak_cb = cancel_writing_timer_.ToWeakCallback(cb);
	nbase::ThreadManager::PostDelayedTask(weak_cb, nbase::TimeDelta::FromSeconds(kCellCancelWriting));

	if(has_writing_cell_)
		return;

	MsgBubbleWriting* item = new MsgBubbleWriting;
	GlobalManager::FillBoxWithCache(item, L"session/bubble_left.xml");
	msg_list_->Add(item);

	item->InitControl(false);
	item->InitInfo(msg);
	item->SetShowTime(false);
	item->SetShowName(false, "");

	item->SetName(CELL_WRITING);

	has_writing_cell_ = true;
}

void SessionBox::CancelWriting()
{
	bool at_end = msg_list_->IsAtEnd();

	RemoveTip(STT_WRITING);

	if(at_end)
		msg_list_->EndDown(true, false);
}

void SessionBox::AddRetweetMsg(const nim::IMMessage &msg)
{
	// 如果这个消息对应的资源文件被清理掉，就重新下载
	if (!IsResourceExist(msg))
	{
		nim::NOS::FetchMedia(msg, nbase::Bind(&SessionBox::OnRetweetResDownloadCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nim::NOS::ProgressCallback());
	}
	this->AddSendingMsg(msg);
}

void SessionBox::OnSendMsgCallback(const std::string &cid, int code, __int64 msg_timetag)
{
	IdBubblePair::iterator it = id_bubble_pair_.find(cid);
	if (it != id_bubble_pair_.end())
	{
		MsgBubbleItem* item = it->second;
		if (item)
		{
			if (code == nim::kNIMResSuccess)
			{
				item->SetMsgStatus(nim::kNIMMsgLogStatusSent);
				item->UpdateMsgTime(msg_timetag);
			}
			else if (code == nim::kNIMLocalResMsgFileNotExist)
				item->SetMsgStatus(nim::kNIMMsgLogStatusSendFailed);
			else if (code == nim::kNIMLocalResMsgNosUploadCancel)
				item->SetMsgStatus(nim::kNIMMsgLogStatusSendCancel);
			else if (code == nim::kNIMResInBlack)
			{
				MutiLanSupport* mls = MutiLanSupport::GetInstance();
				SendRefusedTip(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_MSG_SENT_BUT_REJECTED"));
				item->SetMsgStatus(nim::kNIMMsgLogStatusRefused);
			}
			else
			{
				QLOG_WAR(L"unknown send msg callback code {0}") << code;
				item->SetMsgStatus(nim::kNIMMsgLogStatusSendFailed);
			}
		}
	}
}

void SessionBox::OnMsgStatusChangedCallback(const std::string &from_accid, const __int64 timetag, nim::NIMMsgLogStatus status)
{
	if (!last_receipt_msg_id_.empty())
	{
		auto it = id_bubble_pair_.find(last_receipt_msg_id_);
		if (it != id_bubble_pair_.end())
		{
			MsgBubbleItem* item = it->second;
			if (item)
				item->SetMsgStatus(nim::kNIMMsgLogStatusSent);
		}
	}

	std::string my_id = LoginManager::GetInstance()->GetAccount();
	auto iter = cached_msgs_bubbles_.rbegin();
	for (; iter != cached_msgs_bubbles_.rend(); ++iter)
	{
		MsgBubbleItem* item = (MsgBubbleItem*)(*iter);
		if (item)
		{
			nim::IMMessage msg = item->GetMsg();
			if (msg.sender_accid_ == my_id && item->IsMyMsg() && msg.status_ == nim::kNIMMsgLogStatusSent && nim::MsgLog::QuerySentMessageBeReaded(msg))
			{
				item->SetMsgStatus(nim::kNIMMsgLogStatusReceipt);
				last_receipt_msg_id_ = msg.client_msg_id_;
				break;
			}
		}
	}
}

void SessionBox::OnSnapchatReadCallback(const std::string& client_msg_id)
{
	RemoveMsgItem(client_msg_id);
}

void SessionBox::OnDownloadCallback(const std::string &res_id, bool success, const std::string& file_path)
{
	IdBubblePair::iterator it = id_bubble_pair_.find(res_id);
	if (it != id_bubble_pair_.end())
	{
		MsgBubbleItem* item = it->second;
		if (item)
		{
			item->OnDownloadCallback(success, file_path);
		}
	}
}

void SessionBox::OnUploadCallback(const std::string &res_id, bool success, const std::string& url)
{
	IdBubblePair::iterator it = id_bubble_pair_.find(res_id);
	if (it != id_bubble_pair_.end())
	{
		MsgBubbleItem* item = it->second;
		if (item)
		{
			item->OnUploadCallback(success, url);
		}
	}
}

void SessionBox::OnRecallMsgCallback(nim::NIMResCode code, const nim::RecallMsgNotify &notify)
{
	if (code != nim::kNIMResSuccess)
	{
		std::wstring toast = nbase::StringPrintf(L"recall msg error, code:%d, id:%s", code, nbase::UTF8ToUTF16(notify.msg_id_).c_str());
		nim_ui::ShowToast(toast, 5000, this->GetWindow()->GetHWND());
		return;
	}

	bool is_at_end = msg_list_->IsAtEnd();
	int index = RemoveMsgItem(notify.msg_id_);
	if (is_at_end)
		msg_list_->EndDown(true, false);

// 	if (index < 0)
// 	{
// 		QLOG_APP(L"SessionBox::OnRecallMsgCallback RemoveMsgItem faild: code:{0} from_id:{1} to_id:{2} msg_time:{3} msglog_createtime:{4} msg_id:{5}")
// 			<< code << notify.from_id_ << notify.to_id_ << notify.notify_timetag_ << notify.msglog_timetag_ << notify.msg_id_;

		index = FindIndexByMsgTime(notify.msglog_timetag_);
// 		if (index < 0)
// 		{
// 			QLOG_APP(L"SessionBox::OnRecallMsgCallback FindIndexByMsgTime faild: code:{0} from_id:{1} to_id:{2} msg_time:{3} msglog_createtime:{4} msg_id:{5}")
// 				<< code << notify.from_id_ << notify.to_id_ << notify.notify_timetag_ << notify.msglog_timetag_ << notify.msg_id_;
// 		}
// 	}

	if (index > -1)
	{
		std::wstring notify_text = GetRecallNotifyText(notify.from_id_, notify.from_nick_);

		nim::IMMessage msg;
		msg.timetag_ = notify.msglog_timetag_;
		msg.client_msg_id_ = QString::GetGUID();
		msg.receiver_accid_ = session_id_;
		msg.session_type_ = session_type_;
		msg.sender_accid_ = notify.from_id_;
		msg.status_ = nim::kNIMMsgLogStatusSent;
		msg.type_ = nim::kNIMMessageTypeText;
		Json::Value values;
		values["comment"] = "is_recall_notification";
		values["notify_from"] = notify.from_id_;
		values["from_nick"] = notify.from_nick_;
		msg.attach_ = values.toStyledString();
		msg.content_ = nbase::UTF16ToUTF8(notify_text);
		msg.msg_setting_.push_need_badge_ = nim::BS_FALSE; //设置会话列表不需要计入未读数
		nim::MsgLog::WriteMsglogToLocalAsync(session_id_, msg, true, nbase::Bind(&SessionBox::WriteMsglogCallback, this, std::placeholders::_1, std::placeholders::_2, msg, index, is_at_end));
	}
}

void SessionBox::OnRetweetResDownloadCallback(nim::NIMResCode code, const std::string& file_path, const std::string& sid, const std::string& cid)
{
	IdBubblePair::iterator it = id_bubble_pair_.find(cid);
	if (it != id_bubble_pair_.end())
	{
		MsgBubbleItem* item = it->second;
		if (item)
		{
			item->OnDownloadCallback(code == nim::kNIMResSuccess, file_path);
		}
	}
}

void SessionBox::SendText( const std::string &text )
{
	nim::IMMessage msg;
	PackageMsg(msg);
	nim::MessageSetting setting;
	std::string robot_accid;
	//判断是否包含@某人的消息
	if (!uid_at_someone_.empty())
	{
		//检查文本消息中是否存在“@xxx ”的文本
		for (auto it = uid_at_someone_.begin(); it != uid_at_someone_.end(); ++it)
		{
			std::string nick_name = it->first;
			std::string at_str = "@";
			at_str.append(nick_name);
			at_str.append(" ");

			int index = text.find(at_str);
			if (index != std::string::npos)
			{
				if (!it->second.is_robot_)
				{
					setting.force_push_ids_list_.push_back(it->second.uid_);
				}
				else
				{
					msg.type_ = nim::kNIMMessageTypeRobot;
					if (robot_accid.empty())//只允许第一个
						robot_accid = it->second.uid_;
				}
			}
		}

		if (!setting.force_push_ids_list_.empty())
		{
			setting.is_force_push_ = nim::BS_TRUE;
			setting.force_push_content_ = text;
		}

		uid_at_someone_.clear();
	}
	if (is_robot_session_)
		msg.type_ = nim::kNIMMessageTypeRobot;

	std::string json_msg;
	msg.content_ = text;
	if (msg.type_ != nim::kNIMMessageTypeRobot)
	{
		msg.type_ = nim::kNIMMessageTypeText;
		json_msg = nim::Talk::CreateTextMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, msg.content_, setting, msg.timetag_);
	}
	else
	{
		nim::IMBotRobot bot;
		bot.robot_accid_ = is_robot_session_ ? session_id_ : robot_accid;
		std::wstring robot_content = nbase::UTF8ToUTF16(text);
		int index = robot_content.find(L"@");
		if (index != -1)
		{
			int space_index = 0;
			while (space_index != -1 && index != -1)
			{
				space_index = robot_content.find(L" ", index);
				if (space_index != -1)
				{
					robot_content = robot_content.replace(index, space_index - index + 1, L"");
					index = robot_content.find(L"@", index);
				}
			}
		}
		bot.sent_param_["content"] = robot_content.empty() ? " " : nbase::UTF16ToUTF8(robot_content);
		bot.sent_param_["type"] = "01";
		msg.attach_ = bot.ToJsonString();
		json_msg = nim::Talk::CreateBotRobotMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, text, bot, setting, msg.timetag_);
	}

	AddSendingMsg(msg);
	nim::Talk::SendMsg(json_msg);
}

void SessionBox::SendImage( const std::wstring &src )
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeImage;

	//先以消息id为图片名，生成用于上传的图片
	std::wstring image_dir = GetUserImagePath();
	if (!nbase::FilePathIsExist(image_dir, true))
		nbase::CreateDirectory(image_dir);
	std::wstring zoom_path = image_dir + nbase::UTF8ToUTF16(msg.client_msg_id_);
	if (!nbase::FilePathIsExist(zoom_path, false))
	{
		GenerateUploadImage(src, zoom_path);
		if (!nbase::FilePathIsExist(zoom_path, false))
		{
			QLOG_ERR(L"Zoomed image does not exist.");
			return;
		}	
	}

	//再计算用于上传的图片的md5，以md5重命名之
	std::string image_md5 = GetFileMD5(zoom_path);
	std::wstring dest = image_dir + nbase::UTF8ToUTF16(image_md5);
	if (!nbase::FilePathIsExist(dest, false))
	{
		if (!::MoveFileEx(zoom_path.c_str(), dest.c_str(), 0))
		{
			QLOG_ERR(L"Rename image error: {0}.") << ::GetLastError();
			return;
		}
	}
	else
		nbase::DeleteFile(zoom_path);
	
	msg.local_res_path_ = nbase::UTF16ToUTF8(dest);

	nim::IMImage img;
	img.md5_ = image_md5;
	img.size_ = (long)nbase::GetFileSize(dest);

	Gdiplus::Image image(dest.c_str());
	if (image.GetLastStatus() != Gdiplus::Ok)
	{
		assert(0);
	}
	else
	{
		img.width_ = image.GetWidth();
		img.height_ = image.GetHeight();
	}

	msg.attach_ = img.ToJsonString();

	AddSendingMsg(msg);
	std::string json_msg = nim::Talk::CreateImageMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, img, msg.local_res_path_, nim::MessageSetting(), msg.timetag_);
	nim::Talk::SendMsg(json_msg);
}

void SessionBox::SendAudio(const std::string& file_path, const std::string& file_ext, long file_size, int audio_duration)
{
	std::wstring wfile_path = nbase::UTF8ToUTF16(file_path);
	if (!nbase::FilePathIsExist(wfile_path, false))
		return;

	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeAudio;
	msg.local_res_path_ = file_path;

	nim::IMAudio audio;
	audio.duration_ = audio_duration;
	auto token_list = nbase::StringTokenize(wfile_path.c_str(), L"\\");
	audio.md5_ = GetFileMD5(token_list.back());
	audio.size_ = file_size;
	audio.file_extension_ = file_ext;
	msg.attach_ = audio.ToJsonString();

	AddSendingMsg(msg);
	std::string json_msg = nim::Talk::CreateAudioMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, audio, msg.local_res_path_, nim::MessageSetting(), msg.timetag_);
	nim::Talk::SendMsg(json_msg);
}

void SessionBox::SendSnapChat(const std::wstring &src)
{
	auto weak_flag = this->GetWeakFlag();
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.msg_setting_.server_history_saved_ = nim::BS_FALSE;
	msg.msg_setting_.roaming_ = nim::BS_FALSE;
	msg.msg_setting_.self_sync_ = nim::BS_FALSE;
	//TODO(litianyi)
	std::wstring zoom_path = GetUserOtherResPath() + nbase::UTF8ToUTF16(msg.client_msg_id_);
	if (!nbase::FilePathIsExist(zoom_path, false))
	{
		GenerateUploadImage(src, zoom_path);
		if (!nbase::FilePathIsExist(zoom_path, false))
		{
			QLOG_ERR(L"Zoomed image does not exist.");
			return;
		}
	}
	std::string image_md5 = GetFileMD5(zoom_path);
	std::wstring dest = GetUserOtherResPath() + nbase::UTF8ToUTF16(image_md5);
	if (!nbase::FilePathIsExist(dest, false))
	{
		if (!::MoveFileEx(zoom_path.c_str(), dest.c_str(), 0))
		{
			QLOG_ERR(L"Rename image error: {0}.") << ::GetLastError();
			return;
		}
	}
	else
		nbase::DeleteFile(zoom_path);

	msg.local_res_path_ = nbase::UTF16ToUTF8(dest);

	nim::NOS::UploadResource(msg.local_res_path_, [this, msg, image_md5, weak_flag](int res_code, const std::string& url) {
		if (!weak_flag.expired() && res_code == nim::kNIMResSuccess)
		{
			nim::IMMessage new_msg = msg;
			int file_size = (int)nbase::GetFileSize(nbase::UTF8ToUTF16(new_msg.local_res_path_));
			new_msg.type_ = nim::kNIMMessageTypeCustom;
			Json::Value json;
			Json::Value json_data;
			Json::FastWriter writer;
			json_data["size"] = file_size;
			json_data["md5"] = image_md5;
			json_data["url"] = url;
			json["type"] = CustomMsgType_SnapChat;
			json["data"] = json_data;
			new_msg.content_ = nbase::UTF16ToUTF8(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_SNAPCHAT"));
			new_msg.attach_ = writer.write(json);
			AddSendingMsg(new_msg);

			nim::Talk::SendMsg(new_msg.ToJsonString(true));
		}
	});
}

bool SessionBox::CheckFileSize(const std::wstring &src)
{
	int64_t sz = nbase::GetFileSize(src);
	if (sz > LoginManager::GetInstance()->GetFileSizeLimit()*1024*1024 || sz <= 0)
	{
		return false;
	}
	return true;
}

void SessionBox::SendFile(const std::wstring &src)
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeFile;
	msg.local_res_path_ = nbase::UTF16ToUTF8(src);

	nim::IMFile file;
	file.md5_ = GetFileMD5(src);
	file.size_ = (long)nbase::GetFileSize(src);

	nbase::PathString file_name;
	nbase::FilePathApartFileName(src, file_name);
	std::wstring file_exten;
	nbase::FilePathExtension(file_name, file_exten);
	file.display_name_ = nbase::UTF16ToUTF8(file_name);
	file.file_extension_ = nbase::UTF16ToUTF8(file_exten);
	msg.attach_ = file.ToJsonString();

	AddSendingMsg(msg);

	nim::Talk::FileUpPrgCallback* cb_pointer = nullptr;
	MsgBubbleFile* bubble = dynamic_cast<MsgBubbleFile*>(msg_list_->FindSubControl(nbase::UTF8ToUTF16(msg.client_msg_id_)));
	if (!msg.local_res_path_.empty() && nbase::FilePathIsExist(nbase::UTF8ToUTF16(msg.local_res_path_), false) && bubble)
	{
		cb_pointer = new nim::Talk::FileUpPrgCallback(bubble->GetFileUpPrgCallback());
	}
	std::string json_msg = nim::Talk::CreateFileMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, file, msg.local_res_path_, nim::MessageSetting(), msg.timetag_);
	nim::Talk::SendMsg(json_msg, msg.client_msg_id_, cb_pointer);
}

void SessionBox::SendJsb( const std::string &attach )
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeCustom;

	msg.content_ = nbase::UTF16ToUTF8(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_THIS_IS_A_JSB"));
	msg.attach_ = attach;
	
	AddSendingMsg(msg);

	nim::Talk::SendMsg(msg.ToJsonString(true));
}

void SessionBox::SendSticker(const std::string &catalog, const std::string &name)
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeCustom;

	Json::Value json;
	Json::FastWriter writer;
	json["type"] = CustomMsgType_Sticker;
	json["data"]["catalog"] = catalog;
	json["data"]["chartlet"] = name;

	msg.content_ = nbase::UTF16ToUTF8(MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_CHARLET"));
	msg.attach_ = writer.write(json);

	AddSendingMsg(msg);

	nim::Talk::SendMsg(msg.ToJsonString(true));
}

void SessionBox::SendTip(const std::wstring &tip)
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeTips;
	msg.content_ = nbase::UTF16ToUTF8(tip);
	msg.msg_setting_.need_push_ = nim::BS_FALSE;
	msg.status_ = nim::kNIMMsgLogStatusSent;

	AddSendingMsg(msg);
	nim::Talk::SendMsg(msg.ToJsonString(true));
}
void SessionBox::SendRefusedTip(const std::wstring &tip)
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.msg_setting_.server_history_saved_ = nim::BS_FALSE;//不存云端
	msg.msg_setting_.roaming_ = nim::BS_FALSE;//不漫游
	msg.msg_setting_.self_sync_ = nim::BS_FALSE;//不进行多端同步
	msg.msg_setting_.need_push_ = nim::BS_FALSE;//不推送
	msg.msg_setting_.push_need_badge_ = nim::BS_FALSE;//不计数
	msg.msg_setting_.need_offline_ = nim::BS_FALSE;//不需要支持离线
	msg.msg_setting_.routable_ = nim::BS_FALSE;//不需要抄送
	msg.type_ = nim::kNIMMessageTypeTips;
	msg.content_ = nbase::UTF16ToUTF8(tip);
	msg.msg_setting_.need_push_ = nim::BS_FALSE;
	msg.status_ = nim::kNIMMsgLogStatusSent;
	AddSendingMsg(msg);
	nim::Talk::SendMsg(msg.ToJsonString(true));
}
void SessionBox::AddSendingMsg(const nim::IMMessage &msg)
{
	writing_time_ = 0;
	RemoveTip(STT_WRITING);

	bool show_time = false;
	if(last_msg_time_ == 0)
	{
		show_time = true;
		farst_msg_time_ = msg.timetag_;
	}
	else
	{
		show_time = CheckIfShowTime(last_msg_time_, msg.timetag_);
	}
	
	if (!IsNoticeMsg(msg))
		last_msg_time_ = msg.timetag_;

	MsgBubbleItem* item = ShowMsg(msg, false, show_time);
	msg_list_->EndDown(true, false);
}

void SessionBox::ReSendMsg(nim::IMMessage &msg)
{
	msg.msg_setting_.resend_flag_ = nim::BS_TRUE;
	msg.status_ = nim::kNIMMsgLogStatusSending;
	msg.timetag_= 1000 * nbase::Time::Now().ToTimeT();

	AddSendingMsg(msg);

	nim::Talk::SendMsg(msg.ToJsonString(true));
}

void SessionBox::PackageMsg(nim::IMMessage &msg)
{
	msg.session_type_			= session_type_;
	msg.receiver_accid_		= session_id_;
	msg.sender_accid_	= LoginManager::GetInstance()->GetAccount();
	msg.client_msg_id_   = QString::GetGUID();
	msg.msg_setting_.resend_flag_ = nim::BS_FALSE;

	//base获取的时间单位是s，服务器的时间单位是ms
	msg.timetag_ = 1000 * nbase::Time::Now().ToTimeT();

	msg.status_ = nim::kNIMMsgLogStatusSending;
}

void SessionBox::CheckLastReceiptMsg()
{
	if (!last_receipt_msg_id_.empty())
		return;

	std::string my_id = LoginManager::GetInstance()->GetAccount();
	auto iter = cached_msgs_bubbles_.rbegin();
	for (; iter != cached_msgs_bubbles_.rend(); ++iter)
	{
		MsgBubbleItem* item = (MsgBubbleItem*)(*iter);
		if (item)
		{
			nim::IMMessage msg = item->GetMsg();
			if (msg.sender_accid_ == my_id && item->IsMyMsg() && msg.status_ == nim::kNIMMsgLogStatusReceipt)
			{
				item->SetMsgStatus(nim::kNIMMsgLogStatusReceipt);
				last_receipt_msg_id_ = msg.client_msg_id_;
				break;
			}
		}
	}
}

bool SessionBox::GetLastNeedSendReceiptMsg(nim::IMMessage &msg)
{
	std::string my_id = LoginManager::GetInstance()->GetAccount();
	auto iter = cached_msgs_bubbles_.rbegin();
	for (; iter != cached_msgs_bubbles_.rend(); ++iter)
	{
		MsgBubbleItem* item = (MsgBubbleItem*)(*iter);
		if (item)
		{
			nim::IMMessage message = item->GetMsg();
			if (message.sender_accid_ != my_id || !item->IsMyMsg())
			{
				if (!nim::MsgLog::QueryReceivedMsgReceiptSent(message))
				{
					msg = message;
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

bool SessionBox::ResetLastMsgNeedMarkReceipt()
{
	std::string my_id = LoginManager::GetInstance()->GetAccount();
	auto iter = cached_msgs_bubbles_.rbegin();
	for (; iter != cached_msgs_bubbles_.rend(); ++iter)
	{
		MsgBubbleItem* item = (MsgBubbleItem*)(*iter);
		if (item)
		{
			nim::IMMessage message = item->GetMsg();
			if (message.sender_accid_ == my_id && item->IsMyMsg())
			{
				if (nim::MsgLog::QuerySentMessageBeReaded(message))
				{
					item->SetMsgStatus(nim::kNIMMsgLogStatusReceipt);
					last_receipt_msg_id_ = message.client_msg_id_;
					mark_receipt_when_load_msgs_ = true;
					return true;
				}
			}
		}
	}
	return false;
}

int SessionBox::RemoveMsgItem(const std::string& client_msg_id)
{
	int index = -1;
	MsgBubbleItem* msg_item = NULL;
	MsgBubbleNotice* msg_cell = NULL;
	for (int i = 0; i < msg_list_->GetCount(); i++)
	{
		if (msg_list_->GetItemAt(i)->GetUTF8Name() == client_msg_id)
		{
			msg_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
			msg_cell = dynamic_cast<MsgBubbleNotice*>(msg_list_->GetItemAt(i));
			index = i;
			break;
		}
	}
	if (!msg_item)
	{
		if (msg_cell) //要删除的是一个通知消息，删除后直接return
			msg_list_->Remove(msg_cell);
		return index;
	}
	
	auto iter2 = std::find(cached_msgs_bubbles_.begin(), cached_msgs_bubbles_.end(), msg_item);
	if (iter2 != cached_msgs_bubbles_.end())
	{
		if (last_receipt_msg_id_ == client_msg_id) //本条要被删除的消息是显示已读的消息，则把上一条自己发的消息显示为已读。
		{
			auto iter3 = std::reverse_iterator<decltype(iter2)>(iter2); //iter3现在指向iter2的上一个元素
			std::string my_id = LoginManager::GetInstance()->GetAccount();
			while (iter3 != cached_msgs_bubbles_.rend())
			{
				MsgBubbleItem* item = dynamic_cast<MsgBubbleItem*>(*iter3);
				if (item)
				{
					nim::IMMessage msg = item->GetMsg();
					if (msg.sender_accid_ == my_id && item->IsMyMsg())
					{
						item->SetMsgStatus(nim::kNIMMsgLogStatusReceipt);
						last_receipt_msg_id_ = msg.client_msg_id_;
						break;
					}
				}
				iter3++;
			}
			if (iter3 == cached_msgs_bubbles_.rend())
				last_receipt_msg_id_ = "";
		}
		cached_msgs_bubbles_.erase(iter2); //从cached_msgs_bubbles_中删除
	}

	auto iter1 = id_bubble_pair_.find(client_msg_id);
	if (iter1 != id_bubble_pair_.end())
		id_bubble_pair_.erase(client_msg_id); //从id_bubble_pair_删除

	msg_list_->Remove(msg_item); //最后从msg_list_中删除并销毁该MsgBubbleItem

	return index;
}

int SessionBox::FindIndexByMsgTime(int64_t msg_time)
{
	int index = -1;
	MsgBubbleItem* msg_item = NULL;
	MsgBubbleNotice* msg_cell = NULL;

	for (int i = 0; i < msg_list_->GetCount(); i++)
	{
		msg_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
		if (msg_item)
		{
			if (msg_time <= msg_item->GetMsgTimeTag())
			{
				index = i;
				break;
			}
		}
		else
		{
			msg_cell = dynamic_cast<MsgBubbleNotice*>(msg_list_->GetItemAt(i));
			if (msg_cell && msg_time <= msg_cell->GetMsgTimeTag())
			{
				index = i;
				break;
			}
		}
	}

	if (-1 == index)
		return msg_list_->GetCount();
	else
		return index;
}

std::wstring SessionBox::GetRecallNotifyText(const std::string& msg_from_id, const std::string& msg_from_nick)
{
	ui::MutiLanSupport* mls = ui::MutiLanSupport::GetInstance();
	std::wstring notify_text;
	if (msg_from_id == LoginManager::GetInstance()->GetAccount())
	{
		notify_text = mls->GetStringViaID(L"STRID_SESSION_ITEM_MSG_I_RECALL_MSG");
	}
	else
	{
		if (session_type_ == nim::kNIMSessionTypeP2P)
		{
			notify_text = mls->GetStringViaID(L"STRID_SESSION_ITEM_MSG_OTHER_RECALL_MSG");
		}
		else
		{
			UTF8String name;
			if (!msg_from_nick.empty())
			{
				name = msg_from_nick;
			}
			else
			{
				auto info = GetTeamMemberInfo(msg_from_id);
				name = info.GetNick();
				if (name.empty())
				{
					nim::UserNameCard name_card;
					UserService::GetInstance()->GetUserInfo(msg_from_id, name_card);
					name = name_card.GetName();
				}
				if (name.empty())
					name = msg_from_id;
			}

			notify_text = nbase::UTF8ToUTF16(name) + L" " + mls->GetStringViaID(L"STRID_SESSION_ITEM_MSG_RECALL_MSG");
		}
	}

	return notify_text;
}

void SessionBox::CheckHeader()
{
	std::wstring name, photo;
	bool is_team = (session_type_ == nim::kNIMSessionTypeTeam);

	if (is_team)
	{
		label_tid_->SetVisible(true);
		label_tid_->SetUTF8Text(session_id_);
		label_online_state_->SetVisible(false);
		name = TeamService::GetInstance()->GetTeamName(session_id_);
		photo = PhotoService::GetInstance()->GetTeamPhoto(session_id_, true);
	}
	else
	{
		if (SubscribeEventManager::GetInstance()->IsEnabled())
		{
			label_online_state_->SetVisible(true);
			EventDataEx data;
			if (!is_robot_session_)
				SubscribeEventManager::GetInstance()->GetEventData(nim::kNIMEventTypeOnlineState, session_id_, data);
			else
				data.online_client_.online_client_type_.insert(nim::kNIMClientTypeDefault);
			SetOnlineState(data);
		}

		if (!is_robot_session_)
		{
			btn_invite_->SetVisible(true);
			label_tid_->SetVisible(false);
			if (IsFileTransPhone())
				name = MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_SESSION_MY_MOBILEPHONE");
			else
				name = UserService::GetInstance()->GetUserName(session_id_);
		}
		else
		{
			name = nbase::UTF8ToUTF16(robot_info_.GetName());
		}
		photo = PhotoService::GetInstance()->GetUserPhoto(session_id_, is_robot_session_);
	}

	SetTitleName(name);
	SetHeaderPhoto(photo);
}

void SessionBox::OnRelink(const Json::Value &json)
{
	bool link = json["link"].asBool();
	if (link)
	{
		RemoveTip(STT_LINK);
	}
}

bool SessionBox::IsFileTransPhone()
{
	if (session_type_ == nim::kNIMSessionTypeP2P && LoginManager::GetInstance()->IsEqual(session_id_))
	{
		return true;
	}
	return false;
}

void SessionBox::SetOnlineState(const EventDataEx &data)
{
	if (session_type_ != nim::kNIMSessionTypeTeam)
	{
		label_online_state_->SetText(OnlineStateEventHelper::GetOnlineState(data.online_client_, data.multi_config_, false));
	}
}

void SessionBox::SetTitleName(const std::wstring &name)
{
	label_title_->SetText(name);
	session_form_->SetMergeItemName(nbase::UTF8ToUTF16(session_id_), name);

	if (taskbar_item_)
		taskbar_item_->SetTaskbarTitle(name);

	SetTaskbarTitle(name);
}

void SessionBox::SetHeaderPhoto(const std::wstring &photo)
{
	btn_header_->SetBkImage(photo);
	session_form_->SetMergeItemHeaderPhoto(nbase::UTF8ToUTF16(session_id_), photo);
	SetTaskbarIcon(photo);
}

void SessionBox::SetTaskbarTitle(const std::wstring &title)
{
	if (session_form_->GetSelectedSessionBox() == this)
	{
		session_form_->SetTaskbarTitle(title);
	}
}

void SessionBox::SetTaskbarIcon(const std::wstring &icon)
{
	if (session_form_->GetSelectedSessionBox() == this)
	{
		session_form_->SetTaskbarIcon(icon);
	}
}

void SessionBox::WriteMsglogCallback(nim::NIMResCode res_code, const std::string& msg_id, const nim::IMMessage& msg, int index, bool is_keep_end)
{
	if (res_code == nim::kNIMResSuccess)
	{
		MsgBubbleNotice* cell = new MsgBubbleNotice;
		GlobalManager::FillBoxWithCache(cell, L"session/cell_notice.xml");
		bool succeed = false;
		if (index > -1)
			succeed = msg_list_->AddAt(cell, index);
		else
			succeed = msg_list_->Add(cell);

		cell->InitControl();
		cell->InitInfo(msg, session_id_, true);

		if (is_keep_end)
			msg_list_->EndDown(true, false);

		if (!succeed)
		{
			QLOG_APP(L"SessionBox::WriteMsglogCallback Add faild: code:{0} index:{1} msg_id:{2} sender:{3} reveiver:{4}")
				<< res_code << index << msg_id << msg.sender_accid_ << msg.receiver_accid_;
		}
	}
	else
	{
		QLOG_APP(L"SessionBox::WriteMsglogCallback Faild: code:{0} index:{1} msg_id:{2} sender:{3} reveiver:{4}")
			<< res_code << index << msg_id << msg.sender_accid_ << msg.receiver_accid_;
	}
}

void SessionBox::OnRobotChange(nim::NIMResCode rescode, nim::NIMRobotInfoChangeType type, const nim::RobotInfos& robots)
{
	if (rescode == nim::kNIMResSuccess)
	{
		for (auto &robot : robots)
		{
			if (robot.GetAccid() == session_id_)
			{
				is_robot_session_ = true;
				robot_info_ = robot;
				FindSubControl(L"btn_audio")->SetVisible(false);
				FindSubControl(L"btn_video")->SetVisible(false);
				FindSubControl(L"btn_rts")->SetVisible(false);
				btn_capture_audio_->SetVisible(false);
				btn_face_->SetVisible(false);
				FindSubControl(L"btn_image")->SetVisible(false);
				FindSubControl(L"btn_snapchat")->SetVisible(false);
				FindSubControl(L"btn_file")->SetVisible(false);
				FindSubControl(L"btn_jsb")->SetVisible(false);
				FindSubControl(L"btn_tip")->SetVisible(false);
				FindSubControl(L"btn_clip")->SetVisible(false);
				FindSubControl(L"btn_custom_msg")->SetVisible(false);
				CheckHeader();			
				break;
			}
		}
		auto form = (AtlistForm *)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::UTF8ToUTF16(session_id_));
		if (form)
			form->InitRobotInfos(robots);
	}
}

void SessionBox::OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos)
{
	auto refresh_msglist = [this](const nim::UserNameCard& info) //更新消息列表中消息气泡的头像和名称
	{
		bool refresh_icon = info.ExistValue(nim::kUserNameCardKeyIconUrl);
		bool refresh_show_name = info.ExistValue(nim::kUserNameCardKeyName) && session_type_ == nim::kNIMSessionTypeTeam;
		if (refresh_show_name)
		{
			auto iter = team_member_info_list_.find(info.GetAccId());
			if (iter != team_member_info_list_.cend() && !iter->second.GetNick().empty()) //设置了群名片，不更新
				refresh_show_name = false;
		}

		if (refresh_icon)
		{
			int msg_count = msg_list_->GetCount();
			for (int i = 0; i < msg_count; i++)
			{
				MsgBubbleItem* bubble_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
				if (bubble_item)
				{
					bool refresh_header = bubble_item->msg_.sender_accid_ == info.GetAccId();
					if (bubble_item->GetMsg().type_ == nim::kNIMMessageTypeRobot)
					{
						nim::IMBotRobot robot;
						nim::Talk::ParseBotRobotMessageAttach(bubble_item->GetMsg(), robot);
						refresh_header = !robot.out_msg_;
					}
					if (refresh_header)
						bubble_item->SetShowHeader();
				}
			}
		}

		if (refresh_show_name)
			RefreshMsglistShowname(info.GetAccId());
	};

	for (auto info : uinfos)
	{
		if (session_type_ == nim::kNIMSessionTypeP2P)
		{
			if (info.GetAccId() == session_id_)
				CheckHeader();
		}
		else if (session_type_ == nim::kNIMSessionTypeTeam)
		{
			if (team_info_.GetType() == nim::kNIMTeamTypeAdvanced)
			{
				auto iter = team_member_info_list_.find(info.GetAccId());
				if (iter != team_member_info_list_.end())
				{
					//更新群成员列表信息
					TeamItem* item = (TeamItem*)member_list_->FindSubControl(nbase::UTF8ToUTF16(info.GetAccId()));
					if (item != NULL)
						item->InitInfo(iter->second);
				}
			}
		}

		refresh_msglist(info);
	}
}

void SessionBox::OnUserPhotoReady(PhotoType type, const std::string& accid, const std::wstring &photo_path)
{
	auto refresh_msglist_photo = [=]() //更新消息列表头像
	{
		int msg_count = msg_list_->GetCount();
		for (int i = 0; i < msg_count; i++)
		{
			MsgBubbleItem* bubble_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
			if (bubble_item != NULL)
			{
				if (bubble_item->msg_.sender_accid_ == accid)
				{
					bool refresh_header = true;
					if (bubble_item->msg_.type_ == nim::kNIMMessageTypeRobot)
					{
						nim::IMBotRobot robot_attach;
						nim::Talk::ParseBotRobotMessageAttach(bubble_item->msg_, robot_attach);
						refresh_header = !robot_attach.out_msg_;
					}
					if (refresh_header)
					{
						bubble_item->SetShowHeader();
					}
				}
			}
		}
	};

	auto refresh_msglist_robot_photo = [=]() //更新消息列表头像
	{
		int msg_count = msg_list_->GetCount();
		for (int i = 0; i < msg_count; i++)
		{
			MsgBubbleItem* bubble_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
			if (bubble_item != NULL)
			{
				bool is_robot_response = false;
				std::string robot_id;
				if (bubble_item->msg_.type_ == nim::kNIMMessageTypeRobot)
				{
					nim::IMBotRobot robot_attach;
					nim::Talk::ParseBotRobotMessageAttach(bubble_item->msg_, robot_attach);
					is_robot_response = robot_attach.out_msg_;
					robot_id = robot_attach.robot_accid_;
				}
				if (robot_id == accid && is_robot_response)
				{
					((MsgBubbleRobot *)bubble_item)->SetShowHeader();
				}
			}
		}
	};

	if (accid == session_id_)
	{
		SetHeaderPhoto(photo_path);
	}

	if (type == kUser)
	{
		// 群聊时，更新群成员列表和消息列表中用户头像
		if (session_type_ == nim::kNIMSessionTypeTeam)
		{
			if (team_info_.GetType() == nim::kNIMTeamTypeAdvanced && team_member_info_list_.find(accid) != team_member_info_list_.cend())
			{
				TeamItem* item = (TeamItem*)member_list_->FindSubControl(nbase::UTF8ToUTF16(accid));
				if (item != NULL)
					item->FindSubControl(L"member_icon")->SetBkImage(photo_path);

				refresh_msglist_photo();
			}
			else if (team_info_.GetType() == nim::kNIMTeamTypeNormal)
				refresh_msglist_photo();
		}
		else
			refresh_msglist_photo();
	}
	else if (type == kRobot)
		refresh_msglist_robot_photo();
}

void SessionBox::OnReceiveEvent(int event_type, const std::string &accid, const EventDataEx &data)
{
	if (event_type == nim::kNIMEventTypeOnlineState)
	{
		if (session_type_ == nim::kNIMSessionTypeP2P && session_id_ == accid)
		{
			SetOnlineState(data);
		}
	}
}

}