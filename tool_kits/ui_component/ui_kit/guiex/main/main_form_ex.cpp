#include "stdafx.h"
#include "main_form_ex.h"
#include "shared\ui\ui_menu.h"
#include "export\nim_ui_contacts_list_manager.h"
#include "export\nim_ui_session_list_manager.h"
#include "callback\login\login_callback.h"
#include "util\user.h"
#include "nim_service\module\service\session_service.h"
#include "module\plugins\main_plugins_manager.h"
#include "module\session\session_manager.h"
#include "gui\profile_form\profile_form.h"
#include "gui/plugins/session/session_plugin.h"
#include "nim_service\module\subscribe_event\subscribe_event_manager.h"
#include "main_form_menu.h"
namespace nim_comp
{
	const LPCTSTR MainFormEx::kClassName = L"MainFormEx";
	MainFormEx::MainFormEx(IMainFormMenuHandler* main_menu_handler) : 
		is_trayicon_left_double_clicked_(false), is_busy_(false),
		main_menu_handler_(main_menu_handler),
		btn_max_restore_(nullptr)
	{
		OnUserInfoChangeCallback cb1 = nbase::Bind(&MainFormEx::OnUserInfoChange, this, std::placeholders::_1);
		unregister_cb.Add(nim_comp::UserService::GetInstance()->RegUserInfoChange(cb1));

		OnPhotoReadyCallback cb2 = nbase::Bind(&MainFormEx::OnUserPhotoReady, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		unregister_cb.Add(nim_comp::PhotoService::GetInstance()->RegPhotoReady(cb2));
		main_menu_handler_->SetHostWindow(this);
	}

	MainFormEx::~MainFormEx()
	{

	}

	std::wstring MainFormEx::GetSkinFolder()
	{
		return L"mainex";
	}

	std::wstring MainFormEx::GetSkinFile()
	{
		return L"main.xml";
	}

	std::wstring MainFormEx::GetWindowClassName() const
	{
		return kClassName;
	}

	std::wstring MainFormEx::GetWindowId() const
	{
		return kClassName;
	}

	void MainFormEx::InitWindow()
	{
		main_bar_ = dynamic_cast<ui::VBox*>(FindControl(L"main_bar"));
		main_plugins_bar_ = dynamic_cast<ui::VBox*>(FindControl(L"main_plugin_bar"));
		simple_plugin_bar_ = dynamic_cast<ui::VBox*>(FindControl(L"simple_plugin_bar"));
		main_plugins_showpage_ = dynamic_cast<ui::TabBox*>(FindControl(L"main_plugins_showpage"));
		btn_max_restore_ = static_cast<ui::Button*>(FindControl(L"btn_max_restore"));
		btn_header_ = dynamic_cast<ui::Button*>(FindControl(L"btn_header"));		
		InitHeader();
		btn_online_state_ = dynamic_cast<ui::Button*>(FindControl(L"btn_online_state"));
		btn_online_state_->SetVisible(nim_comp::SubscribeEventManager::GetInstance()->IsEnabled());		
		btn_header_->AttachClick([this](ui::EventArgs* param){
			nim_comp::ProfileForm::ShowProfileForm(nim_comp::LoginManager::GetInstance()->GetAccount());
			return true;
		});
		btn_online_state_->AttachClick(nbase::Bind(&MainFormEx::OnlineStateMenuButtonClick, this, std::placeholders::_1));
		TrayIconManager::GetInstance()->AddTrayIconEventHandler(this);
		auto hicon = ::ExtractIcon(nbase::win32::GetCurrentModuleHandle(), nbase::win32::GetCurrentModuleName().c_str(), 0);
		TrayIconManager::GetInstance()->SetTrayIcon(hicon, ui::MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_MIANWINDOW_TITLE"));
		LoadPlugins();
		nim_ui::ContactsListManager::GetInstance()->InvokeGetAllUserInfo();
		nim_ui::SessionListManager::GetInstance()->QueryUnreadSysMsgCount();
		nim_comp::SessionService::GetInstance()->InvokeLoadSessionList();
		

		ui::ButtonBox* main_menu_button = (ui::ButtonBox*)FindControl(L"main_menu_button");
		main_menu_button->AttachClick(nbase::Bind(&MainFormEx::MainMenuButtonClick, this, std::placeholders::_1));

		InitSearchBar();
		m_pRoot->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&MainFormEx::Notify, this, std::placeholders::_1));
		m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&MainFormEx::OnClicked, this, std::placeholders::_1));

		//auto name_task = [](){
		//	static std::wstring xing = L"��Ǯ��������֣���������������������������ʩ�ſײ��ϻ���κ�ս���л������ˮ��������˸��ɷ�����³Τ������ﻨ������Ԭ��ۺ��ʷ��";
		//	static std::wstring ming = L"��������������������������������������������������������������øĸŸǸȸɸʸ̸͸θϸиҸոٸ޸߸�������������������������������������������������������������¹ùĹŹǹȹʹ˹̹ιϹѹҹֹ׹عٹڹ۹ܹݹ޹�������������";
		//	std::wstring ret;
		//	ret.append(1,xing[rand() % xing.length()]);
		//	auto ming_length = rand() % 2 + 1;
		//	for (auto i = 0; i < ming_length; i++)
		//		ret.append(1, ming[rand() % ming.length()]);
		//	return ret;
		//};
		//auto mobile_task = [](){
		//	static std::vector<std::wstring> head = { L"160", L"161", L"162", L"163", L"164", L"165", L"166", L"167" };
		//	static std::vector<std::wstring> body = { L"11111111", L"22222222", L"33333333", L"55555555", L"66666666", L"77777777", L"88888888" };
		//	std::wstring ret;
		//	ret.append(head[rand() % head.size()]);
		//	ret.append(body[rand() % body.size()]);
		//	return ret;
		//};
		//auto department_task = [](int index){
		//	std::wstring ret;
		//	static std::vector<std::wstring> dep_index = { L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"ʮ", L"ʮһ" };
		//	ret = nbase::StringPrintf(L"��%s��ҵ��", dep_index[index].c_str());
		//	return ret;
		//};
		//auto employee_sa_task = [](){
		//	static int employee_sa_begin = 200;
		//	return nbase::StringPrintf(L"%06d", employee_sa_begin++);
		//};
		//auto email_task = [](const std::wstring& employee_sa){
		//	std::wstring ret(L"addresbook_");
		//	if (employee_sa.empty())
		//		ret.append(L"No").append(L"@demo.com");
		//	else
		//		ret.append(employee_sa).append(L"@demo.com");
		//	return ret;
		//};
		//TiXmlDocument* document = new TiXmlDocument("d:/addresbook.xml");
		//TiXmlElement* root = new TiXmlElement("Item");	
		//root->SetAttribute("id", QString::GetGUID().c_str());
		//root->SetAttribute("type", 0);
		//root->SetAttribute("name", nbase::UTF16ToUTF8(L"nim_demo�Ƽ�"));
		//document->LinkEndChild(root);
		//TiXmlElement* elment = new TiXmlElement("Item");
		//root->LinkEndChild(elment);
		//elment->SetAttribute("id", QString::GetGUID().c_str());
		//elment->SetAttribute("type", 0);
		//elment->SetAttribute("name", nbase::UTF16ToUTF8(L"��ҵֱ����Ա"));
		//auto count = rand() % 100 + 1;
		//if (count < 30)
		//	count = 30;
		//for (auto i = 0; i < count; i++)
		//{
		//	auto sub_elment = new TiXmlElement("Item");
		//	elment->LinkEndChild(sub_elment);
		//	sub_elment->SetAttribute("id", QString::GetGUID().c_str());
		//	sub_elment->SetAttribute("type", 1);
		//	sub_elment->SetAttribute("name", nbase::UTF16ToUTF8(name_task()));
		//	sub_elment->SetAttribute("mobile", nbase::UTF16ToUTF8(mobile_task()));
		//	sub_elment->SetAttribute("mail", nbase::UTF16ToUTF8(email_task(L"")));
		//}

		//auto task_create_sub = [&](TiXmlElement* dep_elment){
		//	for (int j = 0; j < 10; j++)
		//	{
		//		auto sub_dep_elment = new TiXmlElement("Item");
		//		dep_elment->LinkEndChild(sub_dep_elment);
		//		sub_dep_elment->SetAttribute("id", QString::GetGUID().c_str());
		//		sub_dep_elment->SetAttribute("type", 0);
		//		sub_dep_elment->SetAttribute("name", nbase::UTF16ToUTF8(department_task(j)));
		//		int count = rand() % 80;
		//		if (count < 30)
		//			count = 30;
		//		if (count > 0)
		//		{
		//			for (int k = 0; k < count; k++)
		//			{
		//				auto  employee_elment = new TiXmlElement("Item");
		//				sub_dep_elment->LinkEndChild(employee_elment);
		//				employee_elment->SetAttribute("id", QString::GetGUID().c_str());
		//				employee_elment->SetAttribute("type", 1);
		//				auto sa = employee_sa_task();
		//				employee_elment->SetAttribute("employeesa", nbase::UTF16ToUTF8(sa));
		//				employee_elment->SetAttribute("name", nbase::UTF16ToUTF8(name_task()));
		//				employee_elment->SetAttribute("mobile", nbase::UTF16ToUTF8(mobile_task()));
		//				employee_elment->SetAttribute("mail", nbase::UTF16ToUTF8(email_task(sa)));
		//			}
		//		}

		//		{
		//			for (int j = 0; j < 10; j++)
		//			{
		//				auto sub_sub_dep_elment = new TiXmlElement("Item");
		//				sub_dep_elment->LinkEndChild(sub_sub_dep_elment);
		//				sub_sub_dep_elment->SetAttribute("id", QString::GetGUID().c_str());
		//				sub_sub_dep_elment->SetAttribute("type", 0);
		//				sub_sub_dep_elment->SetAttribute("name", nbase::UTF16ToUTF8(department_task(j)));
		//				int count = rand() % 60;
		//				if (count < 5)
		//					count = 5;
		//				if (count > 0)
		//				{
		//					for (int k = 0; k < count; k++)
		//					{
		//						auto  employee_elment = new TiXmlElement("Item");
		//						sub_sub_dep_elment->LinkEndChild(employee_elment);
		//						employee_elment->SetAttribute("id", QString::GetGUID().c_str());
		//						employee_elment->SetAttribute("type", 1);
		//						auto sa = employee_sa_task();
		//						employee_elment->SetAttribute("employeesa", nbase::UTF16ToUTF8(sa));
		//						employee_elment->SetAttribute("name", nbase::UTF16ToUTF8(name_task()));
		//						employee_elment->SetAttribute("mobile", nbase::UTF16ToUTF8(mobile_task()));
		//						employee_elment->SetAttribute("mail", nbase::UTF16ToUTF8(email_task(sa)));
		//					}
		//				}
		//			}
		//		}
		//	}
		//};

		//for (int i = 1; i < 10; i++)
		//{
		//	auto dep_elment = new TiXmlElement("Item");
		//	root->LinkEndChild(dep_elment);
		//	dep_elment->SetAttribute("id", QString::GetGUID().c_str());
		//	dep_elment->SetAttribute("type", 0);
		//	dep_elment->SetAttribute("name", nbase::UTF16ToUTF8(department_task(i)));
		//	task_create_sub(dep_elment);
		//}
		//document->SaveFile();
	}
	LRESULT MainFormEx::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT result = WindowEx::OnNcHitTest(uMsg, wParam, lParam, bHandled);
		if (result == HTCLIENT)
		{
			POINT point;
			GetCursorPos(&point);
			::ScreenToClient(m_hWnd, &point);
			if (main_bar_)
			{
				RECT rect = main_bar_->GetPos();
				if (::PtInRect(&rect, point))
				{
					ui::Control* pControl = FindControl(point);
					if (pControl)
					{
						if (dynamic_cast<ui::Button*>(pControl) || dynamic_cast<ui::ButtonBox*>(pControl) || dynamic_cast<ui::RichEdit*>(pControl))
							return HTCLIENT;						
						else
							return HTCAPTION;
					}
				}
			}
		}
		return result;
	}
	void MainFormEx::LoadPlugins()
	{
		MainPluginsManager::GetInstance()->LoadPlugins();
		auto&& plugins = MainPluginsManager::GetInstance()->GetPluginList();
		ui::VBox* plugin_bar = nullptr;
		for (auto plugin : plugins)
		{
			if (plugin->IsActivePage())
				plugin_bar = main_plugins_bar_;
			else
				plugin_bar = simple_plugin_bar_;
			auto icon = plugin->GetPluginIcon();
			auto page = plugin->GetPluginPage();
			if (icon != nullptr)	plugin_bar->Add(icon);
			if (page != nullptr)	main_plugins_showpage_->Add(page);
			plugin->DoInit();
			if (plugin->IsActivePage())
			{
				plugin->SetGroup(L"Main_Plugin_Group");
				plugin->AttachSelect(plugin->ToWeakCallback([this, plugin](ui::EventArgs* param){
					search_bar_->SetVisible(plugin->ShowSearchBar());
					search_edit_->SetText(L"");
					if (plugin->GetPluginPage() != nullptr)
						main_plugins_showpage_->SelectItem(plugin->GetPluginPage());

					return true;
				}));
			}			
		}
	}
	void MainFormEx::OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos)
	{
		for (auto iter = uinfos.cbegin(); iter != uinfos.cend(); iter++)
		{
			if (nim_comp::LoginManager::GetInstance()->IsEqual(iter->GetAccId()))
			{
				InitHeader();
				break;
			}
		}
	}

	void MainFormEx::OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path)
	{
		if (type == kUser && nim_comp::LoginManager::GetInstance()->GetAccount() == account)
			btn_header_->SetBkImage(photo_path);
	}
	void MainFormEx::InitHeader()
	{
		std::string my_id = nim_comp::LoginManager::GetInstance()->GetAccount();
		btn_header_->SetBkImage(nim_comp::PhotoService::GetInstance()->GetUserPhoto(my_id));
	}
	bool MainFormEx::OnLeftClick()
	{
		this->ActiveWindow();
		::ShowWindow(m_hWnd, SW_SHOW);
		if (is_trayicon_left_double_clicked_)
		{
			is_trayicon_left_double_clicked_ = false;
			return true;
		}
		::SetForegroundWindow(m_hWnd);
		::BringWindowToTop(m_hWnd);
		return true;
	}

	bool MainFormEx::OnLeftDoubleClick()
	{
		is_trayicon_left_double_clicked_ = true;
		return true;
	}

	bool MainFormEx::OnRightClick()
	{
		POINT point;
		::GetCursorPos(&point);
		PopupTrayMenu(point);
		return true;
	}

	void MainFormEx::PopupTrayMenu(POINT point)
	{
		//�����˵�����
		ui::CMenuWnd* pMenu = new ui::CMenuWnd(NULL);
		ui::STRINGorID xml(L"tray_menu.xml");
		pMenu->Init(xml, _T("xml"), point, ui::CMenuWnd::RIGHT_TOP);
		//ע��ص�
		ui::CMenuElementUI* display_session_list = (ui::CMenuElementUI*)pMenu->FindControl(L"display_session_list");
		display_session_list->AttachSelect(ToWeakCallback([this](ui::EventArgs* param){
			auto session_plugin = MainPluginsManager::GetInstance()->GetPlugin(SessionPlugin::kPLUGIN_NAME);
			session_plugin->Selected(true, true);			
			LeftClick();
			return true;
		}));

		ui::CMenuElementUI* logoff = (ui::CMenuElementUI*)pMenu->FindControl(L"logoff");
		logoff->AttachSelect(ToWeakCallback([this](ui::EventArgs* param){
			QCommand::Set(kCmdRestart, L"true");
			std::wstring wacc = nbase::UTF8ToUTF16(LoginManager::GetInstance()->GetAccount());
			QCommand::Set(kCmdAccount, wacc);
			auto task = [](){
				nim_comp::LoginCallback::DoLogout(false, nim::kNIMLogoutChangeAccout);			
			};
			nbase::ThreadManager::PostTask(task);
			return true;
		}));

		ui::CMenuElementUI* quit = (ui::CMenuElementUI*)pMenu->FindControl(L"quit");
		quit->AttachSelect(ToWeakCallback([this](ui::EventArgs* param){
			auto task = [](){
				nim_comp::LoginCallback::DoLogout(false);
			};
			nbase::ThreadManager::PostTask(task);
			return true;
		}));
		//��ʾ
		pMenu->Show();
	}
	bool MainFormEx::OnlineStateMenuButtonClick(ui::EventArgs* param)
	{
		RECT rect = param->pSender->GetPos();
		ui::CPoint point;
		point.x = rect.left;
		point.y = rect.bottom;
		ClientToScreen(m_hWnd, &point);

		//�����˵�����
		ui::CMenuWnd* pMenu = new ui::CMenuWnd(NULL);
		ui::STRINGorID xml(L"online_state_menu.xml");
		pMenu->Init(xml, _T("xml"), point);
		//ע��ص�
		ui::CMenuElementUI* look_log = (ui::CMenuElementUI*)pMenu->FindControl(L"online");
		look_log->AttachSelect(nbase::Bind(&MainFormEx::OnlineStateMenuItemClick, this, std::placeholders::_1));

		ui::CMenuElementUI* file_trans = (ui::CMenuElementUI*)pMenu->FindControl(L"busy");
		file_trans->AttachSelect(nbase::Bind(&MainFormEx::OnlineStateMenuItemClick, this, std::placeholders::_1));

		//��ʾ
		pMenu->Show();
		return true;
	}
	bool MainFormEx::OnlineStateMenuItemClick(ui::EventArgs* param)
	{
		std::wstring name = param->pSender->GetName();
		if (name == L"online")
		{
			if (!is_busy_)
				return true;

			is_busy_ = false;
		}
		else if (name == L"busy")
		{
			if (is_busy_)
				return true;

			is_busy_ = true;
		}

		SetOnlineState();
		return true;
	}
	void MainFormEx::SetOnlineState()
	{
		if (!nim_comp::SubscribeEventManager::GetInstance()->IsEnabled())
			return;

		nim::EventData event_data = nim_comp::OnlineStateEventHelper::CreateBusyEvent(is_busy_);
		nim::SubscribeEvent::Publish(event_data,
			this->ToWeakCallback([this](nim::NIMResCode res_code, int event_type, const nim::EventData& event_data){
			if (res_code == nim::kNIMResSuccess)
			{
				if (is_busy_)
					btn_online_state_->SetBkImage(L"..\\menu\\icon_busy.png");
				else
					btn_online_state_->SetBkImage(L"..\\menu\\icon_online.png");
			}
			else
			{
				QLOG_ERR(L"OnlineStateMenuItemClick publish busy event error, code:{0}, event_type:{1}") << res_code << event_type;
			}
		}));
	}
	bool MainFormEx::MainMenuButtonClick(ui::EventArgs* param)
	{
		RECT rect = param->pSender->GetPos();
		ui::CPoint point;
		point.x = rect.left ;
		point.y = rect.top - 2;
		ClientToScreen(m_hWnd, &point);
		main_menu_handler_->PopupMainMenu(point);		
		return true;
	}
	void MainFormEx::InitSearchBar()
	{
		search_bar_ = dynamic_cast<ui::ButtonBox*>(FindControl(L"search_bar"));
		search_edit_ = static_cast<ui::RichEdit*>(FindControl(_T("search_edit")));
		search_edit_->AttachTextChange(nbase::Bind(&MainFormEx::SearchEditChange, this, std::placeholders::_1));
		search_edit_->SetLimitText(30);
		search_edit_->AttachBubbledEvent(ui::kEventKeyDown, nbase::Bind(&MainFormEx::Notify, this, std::placeholders::_1));
		btn_clear_input_ = static_cast<ui::Button*>(FindControl(L"clear_input"));
		btn_clear_input_->AttachClick(nbase::Bind(&MainFormEx::OnClearInputBtnClicked, this, std::placeholders::_1));
		btn_clear_input_->SetNoFocus();

		search_result_list_ = static_cast<ui::ListBox*>(FindControl(_T("search_result_list")));
		search_result_list_->AttachBubbledEvent(ui::kEventReturn, nbase::Bind(&MainFormEx::OnReturnEventsClick, this, std::placeholders::_1));
		search_result_list_->AttachBubbledEvent(ui::kEventMouseDoubleClick, [this](ui::EventArgs* param){
			search_result_list_->SetVisible(false);
			return true;
		});
	}
	bool MainFormEx::SearchEditChange(ui::EventArgs* param)
	{
		UTF8String search_key = search_edit_->GetUTF8Text();
		bool has_serch_key = !search_key.empty();
		RunTimeDataManager::GetInstance()->SetSearchingFriendState(has_serch_key);
		btn_clear_input_->SetVisible(has_serch_key);
		search_result_list_->SetVisible(has_serch_key);
		//option_panel_->SetVisible(!has_serch_key);
		FindControl(L"no_search_result_tip")->SetVisible(has_serch_key);
		if (has_serch_key)
		{
			nim_ui::ContactsListManager::GetInstance()->FillSearchResultList(search_result_list_, search_key);
			FindControl(L"no_search_result_tip")->SetVisible(search_result_list_->GetCount() == 0);
			if (search_result_list_->GetCount() > 0)
				search_result_list_->SelectItem(0);
		}
		return true;
	}

	bool MainFormEx::OnClearInputBtnClicked(ui::EventArgs* param)
	{
		search_edit_->SetText(L"");
		return true;
	}
	bool MainFormEx::OnReturnEventsClick(ui::EventArgs* param)
	{
		if (param->Type == ui::kEventReturn)
		{
			nim_comp::FriendItem* item = dynamic_cast<nim_comp::FriendItem*>(param->pSender);
			assert(item);
			if (item)
			{
				search_edit_->SetText(L"");
				nim_comp::SessionManager::GetInstance()->OpenSessionBox(item->GetUTF8DataID(), item->GetFriendItemType() == kFriendItemTypeTeam ?  nim::kNIMSessionTypeTeam:nim::kNIMSessionTypeP2P );
			}
		}

		return true;
	}
	bool MainFormEx::OnClicked(ui::EventArgs* msg)
	{
		std::wstring name = msg->pSender->GetName();
		if (name == L"btn_wnd_close")
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
			::ShowWindow(m_hWnd, SW_HIDE);
		}	
		else if (name == L"btn_max_restore")
		{
			DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
			if (style & WS_MAXIMIZE)
				::ShowWindow(m_hWnd, SW_RESTORE);
			else
				::ShowWindow(m_hWnd, SW_MAXIMIZE);
		}
		return true;
	}
	bool MainFormEx::Notify(ui::EventArgs* msg)
	{
		std::wstring name = msg->pSender->GetName();
		if (msg->Type == ui::kEventReturn)
		{
			if (search_edit_->IsFocused() && !search_edit_->GetText().empty())
			{
				if (search_result_list_->GetCount() != 0)
				{
					nim_comp::FriendItem* item = dynamic_cast<nim_comp::FriendItem*>(search_result_list_->GetItemAt(search_result_list_->GetCurSel()));
					assert(item);
					if (item)
					{
						search_edit_->SetText(L"");
						nim_comp::SessionManager::GetInstance()->OpenSessionBox(item->GetUTF8DataID(), item->GetFriendItemType() == kFriendItemTypeTeam ?  nim::kNIMSessionTypeTeam:nim::kNIMSessionTypeP2P);
					}
				}
				return true;
			}
		}
		if (msg->Type == ui::kEventKeyDown && search_result_list_->IsVisible())
		{
			switch (msg->chKey)
			{
			case VK_UP:
			case VK_DOWN:
				ui::EventArgs msg_temp = *msg;
				msg_temp.pSender = search_result_list_;
				search_result_list_->HandleMessage(msg_temp);
				return true;
			}			
		}
		return true;
	}
	LRESULT MainFormEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_SIZE)
		{
			if (wParam == SIZE_RESTORED)
				OnWndSizeMax(false);
			else if (wParam == SIZE_MAXIMIZED)
				OnWndSizeMax(true);
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	void MainFormEx::OnWndSizeMax(bool max)
	{
		if (btn_max_restore_)
			btn_max_restore_->SetClass(max ? L"btn_wnd_restore" : L"btn_wnd_max");
	}
	void MainFormEx::OnEsc(BOOL &bHandled)
	{
		bHandled = TRUE;
	}
	LRESULT MainFormEx::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		::ShowWindow(m_hWnd, SW_HIDE);
		bHandled = true;
		return 0;
	}
	void MainFormEx::OnFinalMessage(HWND hWnd)
	{
		TrayIconManager::GetInstance()->Destroy();
		__super::OnFinalMessage(hWnd);
	}
}