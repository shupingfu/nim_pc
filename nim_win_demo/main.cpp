#include "main.h"
#include "resource.h"
#include "app_dump.h"
#include "base/util/at_exit.h"
#include "base/util/string_number_conversions.h"
#include "gui/extern_ctrl/extern_ctrl_manager.h"
#include "shared/xml_util.h"
#include "gui/login/login_form.h"
#include "gui/main/main_form.h"
#include "callback/chatroom_callback.h"
#include "module/config/config_helper.h"
#include "cef/cef_module/cef_manager.h"
#include "duilib/Utils/MultiLangSupport.h"

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);
	PreMessageLoop();

	std::wstring theme_dir = QPath::GetAppPath();
	bool adapt_api = ConfigHelper::GetInstance()->IsAdaptDpi();
	std::wstring language = nbase::UTF8ToUTF16(ConfigHelper::GetInstance()->GetLanguage());
	//ui::GlobalManager::Startup(theme_dir + L"themes\\default", ui::CreateControlCallback(), adapt_api, language);
	ui::GlobalManager::Startup(theme_dir + L"themes\\default", ExternCtrlManager::CreateExternCtrl, adapt_api, language);
	nim_ui::UserConfig::GetInstance()->SetDefaultIcon(IDI_ICON);

	std::wstring app_crash = QCommand::Get(kCmdAppCrash);
	if( app_crash.empty() )
	{	
		nim_ui::WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
	}
	else
	{
		std::wstring content = nbase::StringPrintf(ui::MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_APP_DUMP_DUMP_TIP").c_str(), app_crash.c_str());
		MsgboxCallback cb = nbase::Bind(&MainThread::OnMsgBoxCallback, this, std::placeholders::_1);
		ShowMsgBox(NULL, cb, content, false, L"STRING_TIPS", true, L"STRID_APP_DUMP_OPEN", true, L"STRING_NO", true);
	}
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	PostMessageLoop();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();

	nim_chatroom::ChatRoom::Cleanup();
}

void MainThread::PreMessageLoop()
{
	misc_thread_.reset( new MiscThread(kThreadGlobalMisc, "Global Misc Thread") );
	misc_thread_->Start();

	screen_capture_thread_.reset(new MiscThread(kThreadScreenCapture, "screen capture"));
	screen_capture_thread_->Start();

	db_thread_.reset( new DBThread(kThreadDatabase, "Database Thread") );
	db_thread_->Start();

	app_sdk_thread_.reset(new MiscThread(kThreadApp, "App SDK Thread"));
	app_sdk_thread_->Start();
}

void MainThread::PostMessageLoop()
{
	app_sdk_thread_->Stop();
	app_sdk_thread_.reset(NULL);

	misc_thread_->Stop();
	misc_thread_.reset(NULL);

	screen_capture_thread_->Stop();
	screen_capture_thread_.reset(NULL);

	db_thread_->Stop();
	db_thread_.reset(NULL);

	
}

void MainThread::OnMsgBoxCallback( MsgBoxRet ret )
{
	if(ret == MB_YES)
	{
		std::wstring dir = QPath::GetUserAppDataDir("");
		QCommand::AppStartWidthCommand(dir, L"");
	}

	nim_ui::WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
}

/**
* ȫ�ֺ�������ʼ�����š�������ȡӦ�÷�������ַ����������sdk����ʼ����װĿ¼���û�Ŀ¼��ע���յ�����ʱִ�еĻص�������
* @return void �޷���ֵ
*/
static void InitNim()
{
	std::wstring server_conf_path = QPath::GetAppPath();
	server_conf_path.append(L"global_conf.txt");
	nim::SDKConfig config;
	TiXmlDocument document;
	if (shared::LoadXmlFromFile(document, server_conf_path))
	{
		TiXmlElement* root = document.RootElement();
		if (root)
		{
			if (auto pchar = root->Attribute("kNIMSDKLogLevel")){
				int log_level = 5;
				nbase::StringToInt((std::string)pchar, &log_level);
				config.sdk_log_level_ = (nim::NIMSDKLogLevel)log_level;
			}
			if (auto pchar = root->Attribute("kNIMPreloadImageQuality")){
				int quality = -1;
				nbase::StringToInt((std::string)pchar, &quality);
				config.preload_image_quality_ = quality;
			}
			if (auto pchar = root->Attribute("kNIMPreloadImageResize")){
				config.preload_image_resize_ = (std::string)pchar;
			}
			if (auto pchar = root->Attribute("kNIMTeamNotificationUnreadCount")){
				int need = -1;
				nbase::StringToInt((std::string)pchar, &need);
				config.team_notification_unread_count_ = need > 0;
			}
			if (auto pchar = root->Attribute("kNIMAnimatedImageThumbnailEnabled")){
				int need = -1;
				nbase::StringToInt((std::string)pchar, &need);
				config.animated_image_thumbnail_enabled_ = need > 0;
			}
			if (auto pchar = root->Attribute("kNIMNosUseHttps")) {
				int need = 0;
				nbase::StringToInt((std::string)pchar, &need);
				config.use_https_ = (need != 0);
			}
			if (auto pchar = root->Attribute("kNIMServerConfFilePath")) {
				config.server_conf_file_path_ = (std::string)pchar;
			}
			if (auto pchar = root->Attribute("kNIMTeamMessageAckEnabled")) {
				int need = 0;
				nbase::StringToInt((std::string)pchar, &need);
				config.team_msg_ack_ = (need != 0);
			}
			if (auto pchar = root->Attribute("kNIMCachingMarkreadEnabled")) {
				int enable = 0;
				nbase::StringToInt((std::string)pchar, &enable);
				config.caching_markread_ = (enable != 0);
			}
			if (auto pchar = root->Attribute("kNIMCachingMarkreadTime")){
				int time = 1000;
				nbase::StringToInt((std::string)pchar, &time);
				config.caching_markread_time_ = time;
			}
			if (auto pchar = root->Attribute("kNIMCachingMarkreadCount")){
				int count = 10;
				nbase::StringToInt((std::string)pchar, &count);
				config.caching_markread_count_ = count;
			}
			if (auto pchar = root->Attribute("kNIMClientAntispam")){
				int enable = 10;
				nbase::StringToInt((std::string)pchar, &enable);
				config.client_antispam_ = (enable != 0);
			}
		}
	}
	config.database_encrypt_key_ = "Netease"; //string��db key���Ŀǰֻ֧�����32���ַ��ļ�����Կ������ʹ��32���ַ���

	std::string app_key = app_sdk::AppSDKInterface::GetAppKey();
	std::wstring app_install;// = QPath::GetAppPath() + L"\\x64_dlls\\";
	bool ret = nim::Client::Init(app_key, "Netease", nbase::UTF16ToUTF8(app_install), config); // ��������sdk����ʼ����װĿ¼���û�Ŀ¼
	assert(ret);
	//��ʼ��������
	{
		Json::Value extension;
		nim_chatroom::ChatRoomPlatformConfig chatroom_platform_config;
		chatroom_platform_config.AddNTServerAddress(config.ntserver_address_);
		chatroom_platform_config.EnableUploadStatisticsData(config.upload_statistics_data_);
		chatroom_platform_config.ToJsonObject(extension[nim_chatroom::ChatRoomPlatformConfig::kPlatformConfigToken]);
		ret = nim_chatroom::ChatRoom::Init(nbase::UTF16ToUTF8(app_install), nim::GetJsonStringWithNoStyled(extension));
		nim_chatroom::ChatRoom::SetMsgsBatchReport(true);
		assert(ret);
	}	
	// ��ʼ����������Ƶ
	ret = nim::VChat::Init("");
	assert(ret);
	// ��ʼ������http
	nim_http::Init();

	nim_chatroom::ChatroomCallback::InitChatroomCallback();
	// InitUiKit�ӿڲ��������Ƿ������¼�����ģ�飬Ĭ��Ϊfalse�����������demo app��Ϊtrue
	// ������App�������¼����Ĺ��ܣ���˲�����Ϊtrue
	//nim_ui::InitManager::GetInstance()->InitUiKit(app_sdk::AppSDKInterface::IsNimDemoAppKey(app_sdk::AppSDKInterface::GetAppKey())); 
}
int WINAPI NimMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	// ��cef dll�ļ���λ����ӵ������"path"����������,�������԰�dll�ļ��ŵ�bin�����Ŀ¼�����Ҳ���Ҫ�ֶ�Ƶ���л�dll�ļ������д������д��main�Ŀ�ͷ
	nim_cef::CefManager::GetInstance()->AddCefDllToPath();
#ifdef SUPPORTLOCALPLAYER
	TCHAR path_envirom[4096] = { 0 };
	GetEnvironmentVariable(L"path", path_envirom, 4096);
	std::wstring local_video_player_path = QPath::GetAppPath().append(L"live_player").append(L";");
	std::wstring new_envirom(local_video_player_path);
	new_envirom.append(path_envirom);
	SetEnvironmentVariable(L"path", new_envirom.c_str());
#endif

	nbase::AtExitManager at_manager;

	CComModule _Module;
	_Module.Init(NULL, hInst);

	_wsetlocale(LC_ALL, L"chs");

#ifdef _DEBUG
	AllocConsole();
	FILE* fp = NULL;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
	wprintf_s(L"Command:\n%s\n\n", lpszCmdLine);
#endif

	srand((unsigned int)time(NULL));

	::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//��ʼ��cef
	CefSettings settings;
	if (!nim_cef::CefManager::GetInstance()->Initialize(QPath::GetNimAppDataDir(L"Netease"), settings, atoi(GetConfigValue("cef_osr_enabled").c_str()) > 0))
		return 0;

	QCommand::ParseCommand(lpszCmdLine);

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	// ��ʼ�����ź�UI���
	InitNim();

	g_need_restart_after_dump = true;

	{
		MainThread thread; // �������߳�
		thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop); // ִ�����߳�ѭ��
	}
	QLOG_APP(L"exit ui loop");

	//����cef
	nim_cef::CefManager::GetInstance()->UnInitialize();

	// �������֮ǰ����������sdk��UI���
	nim_ui::InitManager::GetInstance()->CleanupUiKit();

	QLOG_APP(L"app exit");

	// �Ƿ��������г���
	std::wstring restart = QCommand::Get(kCmdRestart);
	if (!restart.empty())
	{
		std::wstring cmd;
		std::wstring acc = QCommand::Get(kCmdAccount);
		if (!acc.empty())
			cmd.append(nbase::StringPrintf(L" /%s %s ", kCmdAccount.c_str(), acc.c_str()));
		std::wstring exit_why = QCommand::Get(kCmdExitWhy);
		if (!exit_why.empty())
			cmd.append(nbase::StringPrintf(L" /%s %s ", kCmdExitWhy.c_str(), exit_why.c_str()));
		QCommand::RestartApp(cmd);
	}

	_Module.Term();
	::OleUninitialize();
	return 0;
}
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	int ret = NimMain(hInst, hPrevInst, lpszCmdLine, nCmdShow);	
	exit(ret);
	return ret;
}

