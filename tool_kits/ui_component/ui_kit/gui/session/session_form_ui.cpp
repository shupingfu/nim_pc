﻿#include "session_form.h"
#include "gui/link/link_form.h"
#include "gui/contact_select_form/contact_select_form.h"
#include "gui/team_info/team_info.h"
#include "gui/broad/broad_form.h"
#include "gui/session/msg_record.h"
#include "gui/profile_form/profile_form.h"
#include "gui/session/control/atme_view.h"
#include "gui/session/atlist/at_list_form.h"
#include "callback/session/session_callback.h"
#include "callback/http/http_callback.h"
#include "module/session/session_manager.h"
#include "module/video/video_manager.h"
#include "module/rts/rts_manager.h"
#include "module/service/user_service.h"
#include "capture_image/src/capture_manager.h"
#include "shared/modal_wnd/file_dialog_ex.h"
#include "callback/team/team_callback.h"
#include <commdlg.h>
#include "export/nim_ui_user_config.h"

using namespace ui;

namespace
{
	const int kMaxRecordLastPeople = 5; //最多记录5个最近发言的人
}

namespace nim_comp
{
const LPCTSTR SessionForm::kClassName = L"SessionForm";

SessionForm::SessionForm(std::string id, nim::NIMSessionType type)
{
	icon_handle_ = NULL;

	btn_max_restore_ = NULL;
	input_edit_ = NULL;

	session_id_ = id;
	session_type_ = type;

	first_show_msg_ = true;
	last_msg_time_ = 0;
	farst_msg_time_ = 0;

	has_writing_cell_ = false;
	writing_time_ = 0;
	is_valid_ = true;
}

SessionForm::~SessionForm()
{

}

std::wstring SessionForm::GetSkinFolder()
{
	return L"session";
}

std::wstring SessionForm::GetSkinFile()
{
	return L"session.xml";
}

ui::UILIB_RESOURCETYPE SessionForm::GetResourceType() const
{
	return ui::UILIB_FILE;
}

std::wstring SessionForm::GetZIPFileName() const
{
	return L"session.zip";
}

std::wstring SessionForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring SessionForm::GetWindowId() const
{
	std::wstring id = nbase::UTF8ToUTF16(session_id_);
	return id;
}

UINT SessionForm::GetClassStyle() const
{
	return (UI_CLASSSTYLE_FRAME | CS_DBLCLKS);
}

void SessionForm::OnFinalMessage(HWND hWnd)
{
	if (AudioCallback::GetPlaySid() == session_id_)
	{
		AudioCallback::SetPlaySid("");
		AudioCallback::SetPlayCid("");
		nim_audio::Audio::StopPlayAudio();
	}

	SessionManager::GetInstance()->RemoveForm(session_id_, this);

	if (droptarget_)
	{
		droptarget_->Release();
		droptarget_ = NULL;
	}

	if (icon_handle_)
	{
		::DestroyIcon(icon_handle_);
	}

	__super::OnFinalMessage(hWnd);
}

LRESULT SessionForm::OnClose(UINT u, WPARAM w, LPARAM l, BOOL& bHandled)
{
	return __super::OnClose(u, w, l, bHandled);
}

LRESULT SessionForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == 'V')
		{
			if (::GetKeyState(VK_CONTROL) < 0 && input_edit_->IsFocused())
			{
				input_edit_->PasteSpecial(CF_TEXT);
				return 1;
			}
		}
		else if (wParam == VK_DOWN || wParam == VK_UP || wParam == VK_RETURN || wParam == VK_BACK)
		{
			if (HandleAtMsg(wParam, lParam))
				return 0;
		}
	}
	if (uMsg == WM_CHAR)
	{
		if (wParam != VK_BACK)
		{
			//先让RichEdit处理完，然后再处理@消息
			LRESULT res = __super::HandleMessage(uMsg, wParam, lParam);
			HandleAtMsg(wParam, lParam);
			return res;
		}
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		//处理@列表的滚动
		if (HandleAtMouseWheel(wParam, lParam))
			return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_NCLBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_KILLFOCUS)
	{
		HideAtListForm();
	}
	if (uMsg == WM_SIZE)
	{
		if (wParam == SIZE_RESTORED)
			OnWndSizeMax(false);
		else if (wParam == SIZE_MAXIMIZED)
			OnWndSizeMax(true);
	}
	if (uMsg == WM_SETFOCUS)
	{
		if (input_edit_)
			input_edit_->SetFocus();
	}
	if (uMsg == WM_ACTIVATE)
	{
		if (wParam != WA_INACTIVE)
		{
			SessionManager::GetInstance()->ResetUnread(session_id_);
			SendReceiptIfNeeded();
		}
	}
	if (uMsg == WM_NOTIFY)  // 超链接消息
	{
		if (CheckRichEditLink(wParam, lParam))
			return 0;
	}
	//else if( uMsg == WM_DROPFILES )
	//{
	//	HDROP hDrop = (HDROP) wParam;
	//	if( hDrop )
	//		OnDropFile(hDrop);
	//}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void SessionForm::SendReceiptIfNeeded(bool auto_detect/* = false*/)
{
	//发送已读回执目前只支持P2P会话
	if (session_type_ != nim::kNIMSessionTypeP2P)
		return;

	if (auto_detect)
	{
		auto top_hwnd = GetForegroundWindow();
		if (top_hwnd != this->GetHWND() || msg_list_ == nullptr || !msg_list_->IsAtEnd())
		{
			receipt_need_send_ = true;
			return;
		}
	}

	nim::IMMessage msg;
	if (GetLastNeedSendReceiptMsg(msg))
	{
		receipt_need_send_ = false;
		nim::MsgLog::SendReceiptAsync(msg.ToJsonString(false), [](const nim::MessageStatusChangedResult& res) {
			auto iter = res.results_.begin();
			QLOG_APP(L"mark receipt id:{0} time:{1} rescode:{2}") << iter->talk_id_ << iter->msg_timetag_ << res.rescode_;
		});
	}
}

void SessionForm::OnEsc(BOOL &bHandled)
{
	std::wstring wstr = GetRichText(input_edit_);
	StringHelper::Trim(wstr);
	if (!wstr.empty())
	{
		bHandled = TRUE;

		MsgboxCallback cb = nbase::Bind(&SessionForm::OnCloseInputMsgBoxCallback, this, std::placeholders::_1);
		ShowMsgBox(m_hWnd, L"输入框有未发送内容，确定要关闭窗口吗？", cb);
	}
}

void SessionForm::InitWindow()
{
	if (nim_ui::UserConfig::GetInstance()->GetIcon() > 0)
	{
		SetIcon(nim_ui::UserConfig::GetInstance()->GetIcon());
	}

	m_pRoot->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&SessionForm::Notify, this, std::placeholders::_1));
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&SessionForm::OnClicked, this, std::placeholders::_1));
	m_pRoot->AttachBubbledEvent(ui::kEventSelect, nbase::Bind(&SessionForm::OnSelChanged, this, std::placeholders::_1));
	m_pRoot->AttachBubbledEvent(ui::kEventUnSelect, nbase::Bind(&SessionForm::OnSelChanged, this, std::placeholders::_1));

	label_title_ = (Label*)FindControl(L"label_title");
	label_tid_ = (Label*)FindControl(L"label_tid");

	btn_header_ = (Button*)FindControl(L"btn_header");
	btn_header_->SetEnabled(false);
	btn_header_->AttachClick(nbase::Bind(&SessionForm::OnBtnHeaderClick, this, std::placeholders::_1));

	btn_invite_ = (Button*)FindControl(L"btn_invite");
	btn_invite_->SetVisible(false);
	btn_invite_->AttachClick(nbase::Bind(&SessionForm::OnBtnInvite, this, std::placeholders::_1));

	Button* btn_audio = (Button*)FindControl(L"btn_audio");
	Button* btn_video = (Button*)FindControl(L"btn_video");
	Button* btn_rts = (Button*)FindControl(L"btn_rts");
	if (session_type_ == nim::kNIMSessionTypeP2P && !IsFileTransPhone())
	{
		btn_audio->SetVisible(true);
		btn_video->SetVisible(true);
		btn_rts->SetVisible(true);
	}

	btn_max_restore_ = (Button*)FindControl(L"btn_max_restore");

	msg_content_ = (Box*)FindControl(L"msg_content");
	msg_list_ = (ListBox*)FindControl(L"msg_list");
	atme_view_ = new AtMeView;
	GlobalManager::FillBoxWithCache(atme_view_, L"session/at_me_view.xml");
	msg_content_->Add(atme_view_);
	atme_view_->InitControl();
	atme_view_->AttachBubbledEvent(kEventClick, nbase::Bind(&SessionForm::OnBtnAtMe, this, std::placeholders::_1));
	Button* button_close_at_me = static_cast<Button*>(atme_view_->FindSubControl(L"btn_close_at_me"));
	button_close_at_me->AttachClick(nbase::Bind(&SessionForm::OnCloseAtMeTip, this, std::placeholders::_1));

	btn_face_ = (CheckBox*)FindControl(L"btn_face");
	input_edit_ = (RichEdit*)FindControl(L"input_edit");
	input_edit_->SetLimitText(5000);
	input_edit_->SetNoCaretReadonly();
	input_edit_->AttachReturn(nbase::Bind(&SessionForm::OnEditEnter, this, std::placeholders::_1));
	btn_send_ = (Button*)FindControl(L"btn_send");

	btn_new_broad_ = (Button*)FindControl(L"btn_new_broad");
	btn_new_broad_->SetVisible(false);
	re_broad_ = (RichEdit*)FindControl(L"re_broad");
	re_broad_->SetNoCaretReadonly();
	label_member_ = (Label*)FindControl(L"label_member");
	btn_refresh_member_ = (Button*)FindControl(L"btn_refresh_member");
	btn_refresh_member_->SetEnabled(false);
	member_list_ = (ListBox*)FindControl(L"member_list");

	ITextServices * text_services = input_edit_->GetTextServices();
	richeditolecallback_ = new IRichEditOleCallbackEx(text_services, std::function<void()>());
	text_services->Release();
	input_edit_->SetOleCallback(richeditolecallback_);
	richeditolecallback_->SetCustomMode(true);

	IDropTarget *pdt = input_edit_->GetTxDropTarget();
	if (pdt)
	{
		droptarget_ = pdt;
		::RegisterDragDrop(m_hWnd, this);
	}

	//DragAcceptFiles(m_hWnd, TRUE);

	CheckHeader();
	CheckTeamType(nim::kNIMTeamTypeNormal);

	if (session_type_ == nim::kNIMSessionTypeTeam)
	{
		FindControl(L"btn_snapchat")->SetVisible(false);
		InvokeGetTeamInfo();

		OnSelectAtItem cb = nbase::Bind(&SessionForm::OnSelectAtItemCallback, this, std::placeholders::_1);
		AtlistForm *at_list_fomr = new AtlistForm(session_id_, ToWeakCallback(cb));
		at_list_fomr->Create(m_hWnd, L"", WS_POPUPWINDOW, 0L);
	}
	else
	{
		btn_header_->SetEnabled(true);
		btn_invite_->SetVisible(true);
	}

	unregister_cb.Add(NotifyCenter::GetInstance()->RegNotify(NT_LINK, nbase::Bind(&SessionForm::OnRelink, this, std::placeholders::_1)));
	unregister_cb.Add(UserService::GetInstance()->RegUserInfoChange(nbase::Bind(&SessionForm::OnUserInfoChange, this, std::placeholders::_1)));
	unregister_cb.Add(PhotoService::GetInstance()->RegPhotoReady(nbase::Bind(&SessionForm::OnUserPhotoReady, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	if (session_type_ == nim::kNIMSessionTypeTeam)
	{
		unregister_cb.Add(TeamService::GetInstance()->RegAddTeamMember(nbase::Bind(&SessionForm::OnTeamMemberAdd, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegRemoveTeamMember(nbase::Bind(&SessionForm::OnTeamMemberRemove, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamMember(nbase::Bind(&SessionForm::OnTeamMemberChange, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegSetTeamAdmin(nbase::Bind(&SessionForm::OnTeamAdminSet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamOwner(nbase::Bind(&SessionForm::OnTeamOwnerChange, this, std::placeholders::_1, std::placeholders::_2)));
		unregister_cb.Add(TeamService::GetInstance()->RegChangeTeamName(nbase::Bind(&SessionForm::OnTeamNameChange, this, std::placeholders::_1)));
		unregister_cb.Add(TeamService::GetInstance()->RegRemoveTeam(nbase::Bind(&SessionForm::OnTeamRemove, this, std::placeholders::_1)));
		unregister_cb.Add(TeamService::GetInstance()->RegMuteMember(nbase::Bind(&SessionForm::OnTeamMuteMember, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	}
}

bool SessionForm::Notify(ui::EventArgs* param)
{
	std::wstring name = param->pSender->GetName();
	if (param->Type == kEventScrollChange)
	{
		if (name == L"msg_list")
		{
			if (receipt_need_send_)
			{
				bool list_last = msg_list_->IsAtEnd();
				// 这里要判断当前窗口是否是激活状态，因为在非激活状态下的话也可能会收到Scroll消息导致误发回执消息
				if (list_last && this->GetHWND() == GetForegroundWindow())
					SendReceiptIfNeeded();
			}
		}
	}
	else if (param->Type == ui::kEventNotify)
	{
		MsgBubbleItem* item = dynamic_cast<MsgBubbleItem*>(param->pSender);
		assert(item);
		nim::IMMessage md = item->GetMsg();

		if (param->wParam == BET_RESEND)
		{
			RemoveMsgItem(md.client_msg_id_);
			ReSendMsg(md);
		}
		else if (param->wParam == BET_RELOAD)
		{
			item->SetLoadStatus(RS_LOADING);
			nim::NOS::FetchMedia(md, nim::NOS::DownloadMediaCallback(), nim::NOS::ProgressCallback());
		}
		else if (param->wParam == BET_DELETE)
		{
			RemoveMsgItem(md.client_msg_id_);
			//if (session_type_ == nim::kNIMSessionTypeTeam)
			//	nim::Session::DeleteRecentSession(nim::kNIMSessionTypeTeam, session_id_, nim::Session::DeleteRecentSessionCallabck());
			nim::MsgLog::DeleteAsync(session_id_, session_type_, md.client_msg_id_, nim::MsgLog::DeleteCallback());
		}
		else if (param->wParam == BET_RETWEET)
		{
			OnRetweetMessage(md);
		}
		else if (param->wParam == BET_RECALL)
		{
			nim::Talk::RecallMsg(md, "test notify when recall", nbase::Bind(&nim_comp::TalkCallback::OnReceiveRecallMsgCallback, std::placeholders::_1, std::placeholders::_2));
		}
		else if (param->wParam == BET_TRANSFORM)
		{
			nim::IMMessage msg_data = ((MsgBubbleAudio*)param->pSender)->GetMsg();
			nim::IMAudio audio;
			nim::Talk::ParseAudioMessageAttach(msg_data, audio);
			nim::AudioInfo audio_info;
			audio_info.samplerate_ = "16000";
			audio_info.url_ = audio.url_;
			audio_info.duration_ = audio.duration_;
			audio_info.mime_type_ = audio.file_extension_;

			nim::Tool::GetAudioTextAsync(audio_info, ToWeakCallback([this](int rescode, const std::string& text) {
				if (rescode == nim::kNIMResSuccess) {
					ShowMsgBox(m_hWnd, nbase::UTF8ToUTF16(text), MsgboxCallback(), L"转文字", L"确定", L"");
				}
				else {
					ShowMsgBox(m_hWnd, L"语音转化失败", MsgboxCallback(), L"转文字", L"确定", L"");
					QLOG_ERR(L"audio convert to text failed errorcode={0}") << rescode;
				}
			}));
		}
		else if (param->wParam == BET_SHOWPROFILE)
		{
			if (session_type_ == nim::kNIMSessionTypeTeam)
			{
				nim::NIMTeamUserType type = team_member_info_list_[LoginManager::GetInstance()->GetAccount()].GetUserType();
				ProfileForm::ShowProfileForm(session_id_, md.sender_accid_, type);
			}
			else
			{
				ProfileForm::ShowProfileForm(md.sender_accid_);
			}
		}
	}
	else if (param->Type == ui::kEventTextChange)
	{
		if (name == L"input_edit")
		{
			//当用户正在输入框内输入文字时，发送给对方“对方正在输入”的消息
			if (session_type_ == nim::kNIMSessionTypeP2P && !IsFileTransPhone())
			{
				std::wstring wstr = GetRichText(input_edit_);
				if (wstr.empty())
				{

				}
				else
				{
					long long now = nbase::Time::Now().ToTimeT();
					if (writing_time_ == 0 || now - writing_time_ > kCellWritingTime)
					{
						writing_time_ = now;

						Json::Value json;
						Json::FastWriter writer;
						json["id"] = "1";

						nim::SysMessage msg;
						msg.receiver_accid_ = session_id_;
						msg.sender_accid_ = LoginManager::GetInstance()->GetAccount();
						msg.client_msg_id_ = QString::GetGUID();
						msg.timetag_ = 1000 * nbase::Time::Now().ToTimeT();
						msg.attach_ = writer.write(json);
						msg.type_ = nim::kNIMSysMsgTypeCustomP2PMsg;

						nim::SystemMsg::SendCustomNotificationMsg(msg.ToJsonString());
					}
				}
			}
		}
	}
	return true;
}

bool SessionForm::OnClicked(ui::EventArgs* param)
{
	std::wstring name = param->pSender->GetName();
	if (name == L"btn_max_restore")
	{
		DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
		if (style & WS_MAXIMIZE)
			::ShowWindow(m_hWnd, SW_RESTORE);
		else
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
	}
	else if (name == L"btn_send")
	{
		OnBtnSend();
	}
	else if (name == L"btn_image")
	{
		OnBtnImage(false);
	}
	else if (name == L"btn_snapchat")
	{
		OnBtnImage(true);
	}
	else if (name == L"btn_file")
	{
		OnBtnFile();
	}
	else if (name == L"btn_jsb")
	{
		OnBtnJsb();
	}
	else if (name == L"btn_audio")
	{
		OnBtnAudio();
	}
	else if (name == L"btn_video")
	{
		OnBtnVideo();
	}
	else if (name == L"btn_rts")
	{
		OnBtnRts();
	}
	else if (name == L"btn_tip")
	{
		SendTip(L"这是一条提醒消息");
	}
	else if (name == CELL_BTN_LOAD_MORE_MSG)
	{
		Control* ctrl = msg_list_->FindSubControl(CELL_LOAD_MORE_MSG);
		assert(ctrl);
		ctrl->SetVisible(false);

		InvokeShowMsgs(false);
	}
	else if (name == L"btn_clip")
	{
		if (CaptureManager::GetInstance()->IsCaptureTracking())
		{
			return false;
		}

		//::ShowWindow( m_hWnd, SW_SHOWMINIMIZED );
		StdClosure callback = nbase::Bind(&SessionForm::DoClip, this);
		nbase::ThreadManager::PostDelayedTask(kThreadUI, callback, nbase::TimeDelta::FromMilliseconds(500));
	}
	else if (name == L"btn_msg_record")
	{
		MsgRecordForm* f = WindowsManager::SingletonShow<MsgRecordForm>(MsgRecordForm::kClassName);
		f->RefreshRecord(session_id_, session_type_);
	}
	else if (name == L"btn_custom_msg")
	{
		ShowCustomMsgForm();
	}
	else if (name == L"btn_new_broad")
	{
		BroadForm* broad = WindowsManager::SingletonShow<BroadForm>(BroadForm::kClassName);
		broad->SetTid(session_id_);
	}
	else if (name == L"btn_refresh_member")
	{
		InvokeGetTeamMember();
	}
	else if (name == L"add_msglog")
	{
		std::wstring wstr = GetRichText(input_edit_);
		int num = 100;
		nbase::StringToInt(param->pSender->GetDataID(), &num);
		for (int i = 0; i < num; i++)
		{
			nim::IMMessage msg;
			PackageMsg(msg);
			//msg.status_ = nim::kNIMMsgLogStatusSent;
			msg.status_ = nim::kNIMMsgLogStatusRead;
			msg.sender_accid_ = session_id_;
			msg.type_ = nim::kNIMMessageTypeText;
			msg.content_ = nbase::StringPrintf("%d>>%s", i, nbase::UTF16ToUTF8(wstr).c_str());
			//发送消息不需要填写昵称
			//UserInfo user_info;
			//UserService::GetInstance()->GetUserInfo(msg.from_account, user_info);
			//msg.from_nick = user_info.name;
			nim::MsgLog::WriteMsglogToLocalAsync(session_id_, msg, false, nim::MsgLog::WriteMsglogCallback());
		}
	}
	return true;
}

bool SessionForm::OnSelChanged(ui::EventArgs* param)
{
	EventType et = param->Type;
	std::wstring name = param->pSender->GetName();
	if (name == L"btn_face")
	{
		if (et == kEventSelect)
			OnBtnEmoji();
	}
	return true;
}

bool SessionForm::OnEditEnter(ui::EventArgs* param)
{
	OnBtnSend();
	return true;
}

void SessionForm::DoClip()
{
	std::wstring send_info;
	CaptureManager::CaptureCallback callback = nbase::Bind(&SessionForm::OnClipCallback, this, std::placeholders::_1, std::placeholders::_2);
	std::string acc = LoginManager::GetInstance()->GetAccount();
	assert(!acc.empty());
	std::wstring app_data_audio_path = QPath::GetUserAppDataDir(acc);
	if (CaptureManager::GetInstance()->StartCapture(callback, app_data_audio_path, send_info) == false)
	{
		OnClipCallback(FALSE, L"");
	}
}

void SessionForm::OnClipCallback(bool ret, const std::wstring& file_path)
{
	if (ret)
	{
		SendImage(file_path);
	}
};

void SessionForm::OnBtnSend()
{
	if (!LoginManager::GetInstance()->IsLinkActive())
	{
		AddTip(STT_LINK);

		ShowLinkForm();
		return;
	}

	std::wstring wstr = GetRichText(input_edit_);
	StringHelper::Trim(wstr);
	if (wstr.empty())
	{
		input_edit_->SetText(L"");
		ShowMsgBox(m_hWnd, L"请输入内容后再发送哦", MsgboxCallback(),
			L"提示", L"确定", L"");
		return;
	}
	else
	{
		std::string text = nbase::UTF16ToUTF8(wstr);
		SendText(text);
	}
	input_edit_->SetText(L"");
	input_edit_->SetFocus();
}

void SessionForm::ScrollToControl(const Control *control)
{
	if (scroll_weak_flag_.HasUsed())
		return;

	if (msg_list_)
	{
		ui::CSize sz = msg_list_->GetScrollPos();

		StdClosure callback = nbase::Bind([sz, control, this]()
		{
			auto playCallback = nbase::Bind(&SessionForm::ScrollToControlCallback, this, control);
			TimerManager::GetInstance()->AddCancelableTimer(scroll_weak_flag_.GetWeakFlag(), playCallback, 50, TimerManager::REPEAT_FOREVER);

			int top_pos = control->GetPos().top - msg_list_->GetPos().top;
			int bottom_pos = control->GetPos().bottom - msg_list_->GetPos().top;

			if (top_pos < sz.cy)
			{
				// 如果目标控件在上面
				scroll_top_ = true;
				scroll_step_ = abs((sz.cy - top_pos) / 10);
			}
			else if (bottom_pos > sz.cy + msg_list_->GetFixedHeight())
			{
				// 如果目标控件在下面
				scroll_top_ = false;
				scroll_step_ = abs((bottom_pos - sz.cy) / 10);
			}

			if (scroll_step_ < 100)
				scroll_step_ = 100;
		}
		);

		nbase::ThreadManager::PostTask(ToWeakCallback(callback));
	}

}

void SessionForm::ScrollToControlCallback(const Control *control)
{
	assert(scroll_step_ >= 100);

	// 如果没有出现滚动条，那么目标控件就在当前的显示范围内，不需要滚动了
	if (!msg_list_->IsVScrollBarValid())
	{
		scroll_weak_flag_.Cancel();
		atme_view_->ShowLoadingTip(false);
		atme_view_->ShowNextMessage();
		return;
	}

	// VListBox中元素的位置减去VListBox本身的位置后的值是在VListBox中的滚动范围
	int top_pos = control->GetPos().top - msg_list_->GetPos().top;
	int bottom_pos = control->GetPos().bottom - msg_list_->GetPos().top;
	ui::CSize sz = msg_list_->GetScrollPos();

	if (scroll_top_ && top_pos < sz.cy)
	{
		sz.cy -= scroll_step_;
		if (sz.cy < top_pos)
			sz.cy = top_pos;

		msg_list_->SetScrollPosY(sz.cy);
	}
	else if (!scroll_top_ && bottom_pos > sz.cy + msg_list_->GetHeight())
	{
		sz.cy += scroll_step_;
		if (sz.cy > bottom_pos)
			sz.cy = bottom_pos;

		msg_list_->SetScrollPosY(sz.cy);
	}
	else
	{
		scroll_weak_flag_.Cancel();
		atme_view_->ShowLoadingTip(false);
		atme_view_->ShowNextMessage();
	}
}

std::string SessionForm::GetShowName(const std::string& uid)
{
	std::wstring alias = UserService::GetInstance()->GetFriendAlias(uid);
	if (!alias.empty())
		return nbase::UTF16ToUTF8(alias);

	std::string card_name;
	auto i = team_member_info_list_.find(uid);
	if (i != team_member_info_list_.end())
	{
		card_name = i->second.GetNick();
	}
	if (!card_name.empty())
		return card_name;

	std::wstring nick = UserService::GetInstance()->GetUserName(uid, false);
	return nbase::UTF16ToUTF8(nick);
}

bool SessionForm::OnBtnAtMe(ui::EventArgs* param)
{
	if (param->pSender->GetName() != L"btn_at_me")
		return true;

	if (scroll_weak_flag_.HasUsed())
		return true;

	if (atme_view_->GetAtMeCount() > 0)
	{
		MsgBubbleItem *cell = NULL;
		// 先查找当前窗体内是否有需要的消息
		std::wstring uuid = atme_view_->GetUuid();
		for (int i = msg_list_->GetCount() - 1; i >= 0; --i)
		{
			if (uuid == msg_list_->GetItemAt(i)->GetName())
			{
				cell = static_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
				break;
			}
		}

		if (cell && cell->GetMsgType() == nim::kNIMMessageTypeText)
		{
			ScrollToControl(cell);
		}
	}
	else
	{
		atme_view_->SetVisible(false);
	}

	return true;
}

bool SessionForm::OnCloseAtMeTip(ui::EventArgs* param)
{
	if (!atme_view_)
		return false;

	atme_view_->ShowNextMessage();
	atme_view_->SetVisible(false);

	return true;
}

//处理输入框的@消息
bool SessionForm::HandleAtMsg(WPARAM wParam, LPARAM lParam)
{
	if ((input_edit_ == NULL) || !input_edit_->IsFocused())
		return false;

	if (session_type_ != nim::kNIMSessionTypeTeam)
		return false;

	AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::UTF8ToUTF16(session_id_));
	ASSERT(at_list_form != NULL);

	if (wParam == '@')
	{
		RECT rc = input_edit_->GetCaretRect();
		POINT pt = { rc.left, rc.bottom };
		::ClientToScreen(m_hWnd, &pt);

		std::list<std::string> uid_last_five;
		GetLastFiveSender(uid_last_five);
		at_list_form->ShowAllItems(uid_last_five);
		at_list_form->SetShowPos(pt);

		return false;
	}
	else if (wParam == VK_BACK)
	{
		// 获取当前光标之前的所有文本
		long start = 0;
		long end = 0;
		input_edit_->GetSel(start, end);
		std::wstring sel_str = input_edit_->GetTextRange(0, end);
		size_t pos = sel_str.rfind(L"@");

		if (pos == std::wstring::npos || end == 0)
		{
			at_list_form->ShowWindow(false, false);
			return false;
		}

		std::wstring at_str = input_edit_->GetTextRange(pos + 1, end);
		if (!at_str.empty())
		{
			if (at_str.length() == 1)
			{
				// 如果只有一个字符，则删除后就剩下了@，此时显示所有@项
				ui::CPoint pt = input_edit_->PosFromChar(end - 1);
				pt.y = input_edit_->GetCaretRect().bottom;
				::ClientToScreen(m_hWnd, &pt);

				std::list<std::string> uid_last_five;
				GetLastFiveSender(uid_last_five);
				at_list_form->ShowAllItems(uid_last_five);
				at_list_form->SetShowPos(pt);
				return false;
			}

			wchar_t last_char = at_str.at(at_str.length() - 1);
			std::wstring at_name = at_str.substr(0, at_str.length() - 1);

			if (last_char == L' ')
			{
				//如果当前要删的是空格，而且空格前包含了完整的@某人的信息，就删除整条信息，并且不让消息继续传递			
				if (uid_at_someone_.find(nbase::UTF16ToUTF8(at_name)) != uid_at_someone_.end())
				{
					input_edit_->SetSel(pos, end);
					input_edit_->ReplaceSel(L"", true);
					at_list_form->ShowWindow(false, false);
					return true;
				}
			}
			else
			{
				wchar_t last_second_char = at_str.at(at_str.length() - 2);
				if (last_second_char == L' ')
				{
					std::wstring at_name = at_str.substr(0, at_str.length() - 2);
					//如果当前要删的是空格的后一位，而且空格前包含了完整的@某人的信息，就不继续匹配而继续隐藏@列表，直接返回		
					if (uid_at_someone_.find(nbase::UTF16ToUTF8(at_name)) != uid_at_someone_.end())
					{
						return false;
					}
				}

				ui::CPoint pt = input_edit_->PosFromChar(end - 1);
				pt.y = input_edit_->GetCaretRect().bottom;
				::ClientToScreen(m_hWnd, &pt);
				if (at_list_form->Match(at_name, false))
				{
					at_list_form->SetShowPos(pt);
				}
				else
				{
					at_list_form->ShowWindow(false, false);
				}

			}
		}
		else
		{
			at_list_form->ShowWindow(false, false);
		}

		return false;
	}
	else if (wParam == VK_RETURN)
	{
		return at_list_form->HandleKeyEnter();
	}
	else if (wParam == VK_UP)
	{
		return at_list_form->HandleKeyUp();
	}
	else if (wParam == VK_DOWN)
	{
		return at_list_form->HandleKeyDown();
	}
	else
	{
		// 获取当前光标之前的所有文本
		long start = 0;
		long end = 0;
		input_edit_->GetSel(start, end);
		std::wstring sel_str = input_edit_->GetTextRange(0, end);
		size_t pos = sel_str.rfind(L"@");

		if (pos == std::wstring::npos || end == 0)
		{
			at_list_form->ShowWindow(false, false);
			return false;
		}

		std::wstring at_str = input_edit_->GetTextRange(pos + 1, end);
		if (!at_str.empty())
		{
			RECT rc = input_edit_->GetCaretRect();
			POINT pt = { rc.left, rc.bottom };
			::ClientToScreen(m_hWnd, &pt);

			// 如果匹配的关键字只有一个，则是第一次匹配
			// 由于响应@时ShowAllItems函数隐藏了列表中最近发言的5个人，所以第一次匹配时，match_visible设为false不让Match函数忽略隐藏项
			bool match_visible = at_str.length() > 1;
			if (at_list_form->Match(at_str, match_visible))
			{
				at_list_form->SetShowPos(pt);
			}
			else
			{
				at_list_form->ShowWindow(false, false);
			}
		}
	}

	return false;
}

bool SessionForm::HandleAtMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (session_type_ != nim::kNIMSessionTypeTeam)
		return false;

	AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::UTF8ToUTF16(session_id_));
	if (at_list_form == NULL)
		return false;

	if (IsWindowVisible(at_list_form->GetHWND()) && at_list_form->GetHWND() != ::GetFocus())
	{
		int zDelta = (int)(short)HIWORD(wParam);
		return at_list_form->HandleMouseWheel(zDelta > 0);
	}

	return false;
}

void SessionForm::HideAtListForm()
{
	if (session_type_ != nim::kNIMSessionTypeTeam)
		return;

	AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::UTF8ToUTF16(session_id_));
	if (at_list_form == NULL)
		return;

	if (IsWindowVisible(at_list_form->GetHWND()) && at_list_form->GetHWND() != ::GetFocus())
	{
		at_list_form->ShowWindow(false, false);
	}
}

void SessionForm::OnSelectAtItemCallback(const std::string& uid)
{
	if (uid.empty())
		return;

	if (!LoginManager::GetInstance()->IsEqual(uid))
	{
		//找到当前光标之前的最近一个@的位置
		long start = 0;
		long end = 0;
		input_edit_->GetSel(start, end);
		std::wstring sel_str = input_edit_->GetTextRange(0, end);
		size_t pos = sel_str.rfind(L"@");

		if (pos == std::wstring::npos || end == 0)
			return;
		
		std::string show_name;
		auto i = team_member_info_list_.find(uid);
		if (i != team_member_info_list_.end())
		{
			show_name = i->second.GetNick();
		}
		if (show_name.empty())
		{
			show_name = nbase::UTF16ToUTF8(UserService::GetInstance()->GetUserName(uid, false));
		}

		uid_at_someone_[show_name] = uid;

		std::wstring show_text = nbase::UTF8ToUTF16(show_name);
		show_text.append(L" ");

		input_edit_->SetSel(pos + 1, end);
		input_edit_->ReplaceSel(show_text, false);
	}
}

void SessionForm::GetLastFiveSender(std::list<std::string> &uid_last_five)
{
	// 更新最近发消息的成员的链表
	if (session_type_ != nim::kNIMSessionTypeTeam)
		return;

	if (msg_list_)
	{
		for (int i = msg_list_->GetCount(); i >= 0; i--)
		{
			MsgBubbleItem* cell = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
			if (cell)
			{
				std::string sender_id = cell->GetSenderId();
				if (sender_id.empty() || LoginManager::GetInstance()->IsEqual(sender_id))
					continue;;

				auto it = std::find(uid_last_five.begin(), uid_last_five.end(), sender_id);
				if (it == uid_last_five.end())
				{
					uid_last_five.push_back(sender_id);
					if (uid_last_five.size() >= kMaxRecordLastPeople)
						break;
				}
			}
		}
	}
}

void SessionForm::FlashTaskbar()
{
	FLASHWINFO flash_info = { sizeof(FLASHWINFO), m_hWnd, FLASHW_TRAY, 3, 0 };
	::FlashWindowEx(&flash_info);
}

void SessionForm::AdjustSizeForAdvancedTeam()
{
	if (session_type_ == nim::kNIMSessionTypeTeam && team_info_.GetType() == nim::kNIMTeamTypeAdvanced)
	{
		int width = 740, height = 600;
		this->SetMinInfo(width, height);
		this->SetPos(ui::UiRect(0, 0, width, height), SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

void SessionForm::OnNewMsg(bool create, bool flash)
{
	if (flash || create)
	{
		bool need = false;

		if (create)
		{
			RECT rc;
			::GetWindowRect(m_hWnd, &rc);

			WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
			::GetWindowPlacement(m_hWnd, &wp);
			wp.showCmd = SW_SHOWMINNOACTIVE;
			wp.rcNormalPosition = rc;
			::SetWindowPlacement(m_hWnd, &wp);

			need = true;
		}
		else
		{
			if (::IsIconic(m_hWnd))
				need = true;
			else if (::GetForegroundWindow() != m_hWnd)
				need = true;
		}

		if (need)
			FlashTaskbar();
	}
}

void SessionForm::OnWndSizeMax(bool max)
{
	if (btn_max_restore_)
		btn_max_restore_->SetClass(max ? L"btn_wnd_restore" : L"btn_wnd_max");
}

void SessionForm::OnDropFile(HDROP hDrop)
{
	const int MAX_PATH_SIZE = 1024;

	wchar_t buff[MAX_PATH_SIZE] = { 0 };
	int iFile = DragQueryFileW(hDrop, 0xFFFFFFFF, buff, MAX_PATH_SIZE);
	bool large_file = false;
	for (int i = 0; i < iFile; i++)
	{
		int len = DragQueryFileW(hDrop, i, buff, MAX_PATH_SIZE);
		std::wstring path(buff, len);

		std::wstring mime_type = ZoomImage::GetMimeType(path);
		if (!mime_type.empty())
		{
			SendImage(path);
		}
		else if (CheckFileSize(path))
		{
			SendFile(path);
		}
		else
		{
			large_file = true;
			wprintf_s(L"Send File %s\n", path.c_str());
		}
	}
	if (large_file)
	{
		ShowMsgBox(m_hWnd, L"暂时只支持小于15MB的非空文件", MsgboxCallback(), L"提示", L"确定", L"");
	}
	//目前是通过RichEdit的拖曳来处理
	//DragFinish(hDrop);
}

void SessionForm::OnBtnImage(bool is_snapchat)
{
	std::wstring file_type = ui::MutiLanSupport::GetInstance()->GetStringViaID(L"STRING_PIC_FILE");
	LPCTSTR filter = L"*.jpg;*.jpeg;*.png;*.bmp";
	std::wstring text = nbase::StringPrintf(L"%s(%s)", file_type.c_str(), filter);
	std::map<LPCTSTR, LPCTSTR> filters;
	filters[text.c_str()] = filter;

	CFileDialogEx::FileDialogCallback2 cb = nbase::Bind(&SessionForm::OnImageSelected, this, is_snapchat, std::placeholders::_1, std::placeholders::_2);

	CFileDialogEx* file_dlg = new CFileDialogEx();
	file_dlg->SetFilter(filters);
	file_dlg->SetParentWnd(m_hWnd);
	file_dlg->AyncShowOpenFileDlg(cb);
}

void SessionForm::OnBtnFile()
{
	std::wstring file_type = L"文件格式(*.*)";
	LPCTSTR filter = L"*.*";
	std::map<LPCTSTR, LPCTSTR> filters;
	filters[file_type.c_str()] = filter;

	CFileDialogEx::FileDialogCallback2 cb = nbase::Bind(&SessionForm::OnFileSelected, this, std::placeholders::_1, std::placeholders::_2);

	CFileDialogEx* file_dlg = new CFileDialogEx();
	file_dlg->SetFilter(filters);
	file_dlg->SetParentWnd(m_hWnd);
	file_dlg->AyncShowOpenFileDlg(cb);
}

void SessionForm::OnImageSelected(bool is_snapchat, BOOL ret, std::wstring file_path)
{
	if (ret)
	{
		if (!is_snapchat)
		{
			SendImage(file_path);
		}
		else {
			SendSnapChat(file_path);
		}
	}
}

void SessionForm::OnFileSelected(BOOL ret, std::wstring file_path)
{
	if (ret)
	{
		if (CheckFileSize(file_path))
		{
			SendFile(file_path);
		}
		else
		{
			ShowMsgBox(m_hWnd, L"暂时只支持小于15MB的非空文件", MsgboxCallback(), L"提示", L"确定", L"");
		}
	}
}

void SessionForm::OnBtnJsb()
{
	int jsb = (rand() % 3 + rand() % 4 + rand() % 5) % 3 + 1;

	Json::Value json;
	Json::FastWriter writer;
	json["type"] = CustomMsgType_Jsb;
	json["data"]["value"] = jsb;

	SendJsb(writer.write(json));
}

void SessionForm::OnBtnEmoji()
{
	RECT rc = btn_face_->GetPos(true);
	POINT pt = { rc.left - 150, rc.top - 350 };
	::ClientToScreen(m_hWnd, &pt);

	OnSelectEmotion sel = nbase::Bind(&SessionForm::OnEmotionSelected, this, std::placeholders::_1);
	OnSelectEmotion2 sel2 = nbase::Bind(&SessionForm::OnEmotionSelectedSticker, this, std::placeholders::_1, std::placeholders::_2);
	OnEmotionClose  cls = nbase::Bind(&SessionForm::OnEmotionClosed, this);

	EmojiForm* emoji_form = new EmojiForm;
	emoji_form->ShowEmoj(pt, sel, sel2, cls);
}

void SessionForm::OnEmotionSelected(std::wstring emo)
{
	std::wstring file;
	if (emoji::GetEmojiFileByTag(emo, file))
	{
		emoji::InsertFaceToEdit(input_edit_, file, emo);
	}
}
void SessionForm::OnEmotionSelectedSticker(const std::wstring &catalog, const std::wstring &name)
{
	SendSticker(nbase::UTF16ToUTF8(catalog), nbase::UTF16ToUTF8(name));
}

void SessionForm::OnEmotionClosed()
{
	btn_face_->Selected(false, false);

	input_edit_->SetFocus();
}
void SessionForm::OnBtnAudio()
{
	if (session_type_ == nim::kNIMSessionTypeP2P)
	{
		VideoManager::GetInstance()->ShowVideoChatForm(session_id_, false);
	}
}
void SessionForm::OnBtnVideo()
{
	if (session_type_ == nim::kNIMSessionTypeP2P)
	{
		VideoManager::GetInstance()->ShowVideoChatForm(session_id_, true);
	}
}
void SessionForm::OnBtnRts()
{
	if (session_type_ == nim::kNIMSessionTypeP2P)
	{
		RtsManager::GetInstance()->StartRtsForm(nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, session_id_, "");
	}
}

void SessionForm::OnCloseInputMsgBoxCallback(MsgBoxRet ret)
{
	if (ret == MB_YES)
		this->Close();
}

void SessionForm::RemoveTip(SessionTipType type)
{
	if (type == STT_LINK)
	{
		Control* ctrl = msg_content_->FindSubControl(SESSION_TIP_LINK);
		if (ctrl)
			msg_content_->Remove(ctrl);
	}
	else if (type == STT_LEAVE)
	{
		Control* ctrl = msg_content_->FindSubControl(SESSION_TIP_LEAVE);
		if (ctrl)
			msg_content_->Remove(ctrl);
	}
	else if (type == STT_NOT_FRIEND)
	{
		Control* ctrl = msg_content_->FindSubControl(SESSION_TIP_NOT_FRIEND);
		if (ctrl)
			msg_content_->Remove(ctrl);
	}
	else if (type == STT_LOAD_MORE_MSG)
	{
		Control* ctrl = msg_list_->FindSubControl(CELL_LOAD_MORE_MSG);
		if (ctrl)
			msg_list_->Remove(ctrl);
	}
	else if (type == STT_WRITING)
	{
		Control* ctrl = msg_list_->FindSubControl(CELL_WRITING);
		if (ctrl)
			msg_list_->Remove(ctrl);
		has_writing_cell_ = false;
	}
}

void SessionForm::AddTip(SessionTipType type)
{
	if (type == STT_LINK)
	{
		RemoveTip(STT_LINK);

		HBox* hbox = new HBox;
		GlobalManager::FillBoxWithCache(hbox, L"session/tip_link.xml");
		msg_content_->Add(hbox);

		hbox->SetName(SESSION_TIP_LINK);
	}
	else if (type == STT_LEAVE)
	{
		RemoveTip(STT_LEAVE);

		HBox* hbox = new HBox;
		GlobalManager::FillBoxWithCache(hbox, L"session/tip_leave.xml");
		msg_content_->Add(hbox);

		hbox->SetName(SESSION_TIP_LEAVE);
	}
	else if (type == STT_NOT_FRIEND)
	{
		RemoveTip(STT_NOT_FRIEND);

		HBox* hbox = new HBox;
		GlobalManager::FillBoxWithCache(hbox, L"session/tip_not_friend.xml");
		msg_content_->Add(hbox);

		hbox->SetName(SESSION_TIP_NOT_FRIEND);
	}
	else if (type == STT_LOAD_MORE_MSG)
	{
		RemoveTip(STT_LOAD_MORE_MSG);

		ListContainerElement* cell = new ListContainerElement;
		GlobalManager::FillBoxWithCache(cell, L"session/cell_more_msg.xml");
		msg_list_->AddAt(cell, 0);

		cell->SetName(CELL_LOAD_MORE_MSG);
	}
}

void SessionForm::EnterTeamHandle()
{
	RemoveTip(STT_LEAVE);
	is_valid_ = true;

	btn_header_->SetEnabled(true);
	input_edit_->SetEnabled(true);
	btn_send_->SetEnabled(true);
	FindControl(L"btn_custom_msg")->SetEnabled(true);
	FindControl(L"btn_msg_record")->SetEnabled(true);
}

void SessionForm::LeaveTeamHandle()
{
	AddTip(STT_LEAVE);
	is_valid_ = false;

	btn_header_->SetEnabled(false);
	btn_invite_->SetEnabled(false);
	input_edit_->SetReadOnly(true);
	FindControl(L"bottom_panel")->SetEnabled(false);
	btn_new_broad_->SetEnabled(false);
	btn_refresh_member_->SetEnabled(false);
}

void SessionForm::DismissTeamHandle()
{
	AddTip(STT_DISMISS);
	is_valid_ = false;

	btn_header_->SetEnabled(false);
	btn_invite_->SetEnabled(false);
	input_edit_->SetReadOnly(true);
	FindControl(L"bottom_panel")->SetEnabled(false);
	btn_new_broad_->SetEnabled(false);
	btn_refresh_member_->SetEnabled(false);
}

BOOL SessionForm::CheckDropEnable(POINTL pt)
{
	RECT rect = input_edit_->GetPos();
	ClientToScreen(m_hWnd, (LPPOINT)&rect);
	ClientToScreen(m_hWnd, ((LPPOINT)&rect) + 1);
	if (pt.x >= rect.left && pt.x <= rect.right && pt.y >= rect.top && pt.y <= rect.bottom)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HRESULT SessionForm::QueryInterface(REFIID iid, void ** ppvObject)
{
	if (droptarget_ == NULL)
	{
		return -1;
	}
	return droptarget_->QueryInterface(iid, ppvObject);
}

ULONG SessionForm::AddRef(void)
{
	if (droptarget_ == NULL)
	{
		return 0;
	}
	return droptarget_->AddRef();
}

ULONG SessionForm::Release(void)
{
	if (droptarget_ == NULL)
	{
		return 0;
	}
	return droptarget_->Release();
}

HRESULT SessionForm::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	if (droptarget_ == NULL)
	{
		return -1;
	}
	if (!CheckDropEnable(pt))
	{
		*pdwEffect = DROPEFFECT_NONE;
	}
	return droptarget_->DragOver(grfKeyState, pt, pdwEffect);
}
HRESULT SessionForm::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	if (droptarget_ == NULL)
	{
		return -1;
	}
	return droptarget_->DragEnter(pDataObject, grfKeyState, pt, pdwEffect);
}
HRESULT SessionForm::DragLeave(void)
{
	if (droptarget_ == NULL)
	{
		return -1;
	}
	return droptarget_->DragLeave();
}

HRESULT SessionForm::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
	if (droptarget_ == NULL)
	{
		return -1;
	}
	if (pDataObj)
	{
		STGMEDIUM stgMedium;
		FORMATETC cFmt = { (CLIPFORMAT)CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		HRESULT hr = pDataObj->GetData(&cFmt, &stgMedium);
		if (hr == S_OK)
		{
			//拖曳文件
			HDROP file = NULL;
			file = (HDROP)GlobalLock(stgMedium.hGlobal);
			GlobalUnlock(stgMedium.hGlobal);
			OnDropFile(file);
			::ReleaseStgMedium(&stgMedium);
			droptarget_->DragLeave();
			return S_OK;
		}
	}
	return droptarget_->Drop(pDataObj, grfKeyState, pt, pdwEffect);
}

void SessionForm::UpdateSessionIcon(const std::wstring &icon)
{
	if (!nbase::FilePathIsExist(icon, false))
		return;

	if (icon_handle_)
	{
		::DestroyIcon(icon_handle_);
	}

	Gdiplus::Bitmap bm(icon.c_str());
	bm.GetHICON(&icon_handle_);
	if (icon_handle_)
	{
		::SendMessage(this->GetHWND(), WM_SETICON, (WPARAM)TRUE, (LPARAM)icon_handle_);
		::SendMessage(this->GetHWND(), WM_SETICON, (WPARAM)FALSE, (LPARAM)icon_handle_);
	}
}

void SessionForm::CheckHeader()
{
	std::wstring name, photo;
	bool is_team = (session_type_ == nim::kNIMSessionTypeTeam);

	if (is_team)
	{
		label_tid_->SetUTF8Text(session_id_);
		name = TeamService::GetInstance()->GetTeamName(session_id_);
		photo = PhotoService::GetInstance()->GetTeamPhoto(session_id_, true);

	}
	else
	{
		label_tid_->SetVisible(false);
		name = IsFileTransPhone() ? L"我的手机" : UserService::GetInstance()->GetUserName(session_id_);
		photo = PhotoService::GetInstance()->GetUserPhoto(session_id_);
	}

	label_title_->SetText(name);
	SetTaskbarTitle(name);
	btn_header_->SetBkImage(photo);
	UpdateSessionIcon(photo);
}

void SessionForm::OnRetweetMessage(const nim::IMMessage &msg)
{
	ContactSelectForm *contact_select_form = (ContactSelectForm *)WindowsManager::GetInstance()->GetWindow(ContactSelectForm::kClassName, nbase::UTF8ToUTF16(ContactSelectForm::kRetweetMessage));
	if (!contact_select_form)
	{
		contact_select_form = new ContactSelectForm(ContactSelectForm::kRetweetMessage, std::list<UTF8String>(), nbase::Bind(&SessionForm::RetweetMessage, this, msg, std::placeholders::_1, std::placeholders::_2), true);
		contact_select_form->Create(NULL, L"", UI_WNDSTYLE_FRAME& ~WS_MAXIMIZEBOX, 0L);
		contact_select_form->CenterWindow();
	}
	else
	{
		contact_select_form->ActiveWindow();
	}
}

void SessionForm::RetweetMessage(nim::IMMessage msg, const std::list<std::string>& friend_list, const std::list<std::string>& team_list)
{
	if (friend_list.empty() && team_list.empty())
		return;

	auto retweet_cb = [this](const std::string &msg, const std::string &receiver_accid)
	{
		nim::IMMessage sending_msg;
		nim::Talk::ParseIMMessage(msg, sending_msg);
		sending_msg.sender_accid_ = LoginManager::GetInstance()->GetAccount();
		if (receiver_accid == this->session_id_)
		{
			// 如果这个消息对应的资源文件被清理掉，就重新下载
			if (!IsResourceExist(sending_msg))
			{
				nim::NOS::FetchMedia(sending_msg, nbase::Bind(&SessionForm::OnRetweetResDownloadCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nim::NOS::ProgressCallback());
			}
			this->AddSendingMsg(sending_msg);
		}

		nim::Talk::SendMsg(msg);
	};

	for (auto &it : friend_list)
	{
		std::string send_msg = nim::Talk::CreateRetweetMessage(msg.ToJsonString(false), QString::GetGUID(), nim::kNIMSessionTypeP2P, it, msg.msg_setting_, 1000 * nbase::Time::Now().ToTimeT());
		retweet_cb(send_msg, it);
	}

	for (auto &it : team_list)
	{
		std::string send_msg = nim::Talk::CreateRetweetMessage(msg.ToJsonString(false), QString::GetGUID(), nim::kNIMSessionTypeTeam, it, msg.msg_setting_, 1000 * nbase::Time::Now().ToTimeT());
		retweet_cb(send_msg, it);
	}
}

bool SessionForm::OnBtnInvite(ui::EventArgs* param)
{
	std::wstring session_id = nbase::UTF8ToUTF16(session_id_);
	ContactSelectForm* contact_select_form = (ContactSelectForm*)WindowsManager::GetInstance()->GetWindow\
		(ContactSelectForm::kClassName, session_id);
	if (contact_select_form == NULL)
	{
		std::list<std::string> exnclud_ids;
		exnclud_ids.push_back(session_id_);
		contact_select_form = new ContactSelectForm(session_id_, exnclud_ids, nbase::Bind(&SessionForm::CreateGroup, this, std::placeholders::_1, std::placeholders::_2));
		contact_select_form->Create(NULL, L"", WS_OVERLAPPEDWINDOW& ~WS_MAXIMIZEBOX, 0L);
		contact_select_form->CenterWindow();
		contact_select_form->ShowWindow(true);
	}
	else
	{
		contact_select_form->ActiveWindow();
	}

	return true;
}

void SessionForm::CreateGroup(const std::list<std::string>& friend_list, const std::list<std::string>& team_list)
{
	std::list<std::string> id_list = friend_list;
	id_list.push_back(session_id_);
	
	UTF16String user_names;
	auto it = id_list.cbegin();
	for (int i = 0; it != id_list.cend() && i < 2; it++, i++)
		user_names += UserService::GetInstance()->GetUserName(*it, false) + L";";
	user_names += UserService::GetInstance()->GetUserName(it == id_list.end() ? LoginManager::GetInstance()->GetAccount() : *it, false);

	nim::TeamInfo tinfo;
	tinfo.SetName(nbase::UTF16ToUTF8(user_names));
	tinfo.SetType(nim::kNIMTeamTypeNormal);
	nim::Team::CreateTeamAsync(tinfo, id_list, "", nbase::Bind(&TeamCallback::OnTeamEventCallback, std::placeholders::_1));

	Close();
}

bool SessionForm::OnBtnHeaderClick(ui::EventArgs* param)
{
	if (session_type_ == nim::kNIMSessionTypeTeam) {
		std::wstring session_id = nbase::UTF8ToUTF16(session_id_);
		TeamInfoForm* team_info_form = (TeamInfoForm*)WindowsManager::GetInstance()->GetWindow\
			(TeamInfoForm::kClassName, session_id);
		if (team_info_form == NULL)
		{
			std::wstring title = team_info_.GetType() == nim::kNIMTeamTypeNormal ? L"讨论组资料" : L"群资料";
			team_info_form = new TeamInfoForm(false, team_info_.GetType(), session_id_, team_info_);
			team_info_form->Create(NULL, title.c_str(), WS_OVERLAPPEDWINDOW& ~WS_MAXIMIZEBOX, 0L);
			team_info_form->CenterWindow();
			team_info_form->ShowWindow(true);
		}
		else
		{
			team_info_form->ActiveWindow();
		}
	}
	else if (session_type_ == nim::kNIMSessionTypeP2P)
	{
		std::wstring session_id = nbase::UTF8ToUTF16(session_id_);
		ProfileForm::ShowProfileForm(nbase::UTF16ToUTF8(session_id));
	}
	return true;
}

bool SessionForm::IsFileTransPhone()
{
	if (session_type_ == nim::kNIMSessionTypeP2P && LoginManager::GetInstance()->IsEqual(session_id_))
	{
		return true;
	}
	return false;
}

void SessionForm::OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos)
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
				if (bubble_item != NULL && bubble_item->msg_.sender_accid_ == info.GetAccId())
					bubble_item->SetShowHeader();
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

void SessionForm::OnUserPhotoReady(PhotoType type, const std::string& accid, const std::wstring &photo_path)
{
	auto refresh_msglist_photo = [=]() //更新消息列表头像
	{
		int msg_count = msg_list_->GetCount();
		for (int i = 0; i < msg_count; i++)
		{
			MsgBubbleItem* bubble_item = dynamic_cast<MsgBubbleItem*>(msg_list_->GetItemAt(i));
			if (bubble_item != NULL && bubble_item->msg_.sender_accid_ == accid)
				bubble_item->msg_header_button_->SetBkImage(photo_path);
		}
	};

	if (accid == session_id_)
	{
		UpdateSessionIcon(photo_path);
		btn_header_->SetBkImage(photo_path);
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
}

bool CheckRichEditLink(WPARAM wParam, LPARAM lParam)
{
	if (wParam == EN_LINK)
	{
		std::wstring url = *(std::wstring*)lParam;
		if (!url.empty())
		{
			std::wstring ws = url;
			nbase::LowerString(ws);
			// 以"file:"开头 或者 包含".." 的超链接不允许打开
			std::size_t k = ws.find(L"file:", 0, 5);
			if (k == 0)
				return true;

			k = ws.find(L"..");
			if (k != std::wstring::npos)
				return true;

			Post2GlobalMisc(nbase::Bind(&shared::tools::SafeOpenUrl, url, SW_SHOW));
			return true;
		}
	}
	return false;
}
} // namespace nim_comp
