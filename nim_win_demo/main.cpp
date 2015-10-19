#include "main.h"
#include "gui/login/login_form.h"
#include "gui/main/main_form.h"
#include "app_dump.h"
#include "base/util/at_exit.h"
#include "shared/xml_util.h"
#include "nim_cpp_wrapper/nim_sdk_function.h"

#include "shared/modal_wnd/async_do_modal.h"

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);
	PreMessageLoop();

	std::wstring theme_dir = QPath::GetAppPath();
	ui::GlobalManager::Startup(theme_dir + L"themes\\default", ui::CreateControlCallback());

	std::wstring app_crash = QCommand::Get(kCmdAppCrash);
	if( app_crash.empty() )
	{	
		WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
	}
	else
	{
		std::wstring content(L"��������ˣ�������־��");
		content.append(app_crash);

		MsgboxCallback cb = nbase::Bind(&MainThread::OnMsgBoxCallback, this, std::placeholders::_1);
		ShowMsgBox(NULL, content, cb, L"��ʾ", L"��", L"ȡ��");
	}
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	PostMessageLoop();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}

void MainThread::PreMessageLoop()
{
	misc_thread_.reset( new MiscThread(kThreadGlobalMisc, "Global Misc Thread") );
	misc_thread_->Start();

	db_thread_.reset( new DBThread(kThreadDatabase, "Database Thread") );
	db_thread_->Start();

	nim_ui::UserConfig::GetInstance()->LoadEmoji();
}

void MainThread::PostMessageLoop()
{
	CancelAllAsyncModalDialogs();

	misc_thread_->Stop();
	misc_thread_.reset(NULL);

	db_thread_->Stop();
	db_thread_.reset(NULL);
}

void MainThread::OnMsgBoxCallback( MsgBoxRet ret )
{
	if(ret == MB_YES)
	{
		std::wstring dir = QPath::GetNimAppDataDir();
		QCommand::AppStartWidthCommand(dir, L"");
	}

	WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
}

/**
* ȫ�ֺ�������ʼ�����š�������ȡӦ�÷�������ַ����������sdk����ʼ����װĿ¼���û�Ŀ¼��ע���յ�����ʱִ�еĻص�������
* @return void �޷���ֵ
*/
static void InitNim()
{
 	Json::Value config_root;

	std::wstring server_conf_path = QPath::GetAppPath();
	server_conf_path.append(L"server_conf.txt");
	TiXmlDocument document;
	if (shared::LoadXmlFromFile(document, server_conf_path))
	{
		TiXmlElement* root = document.RootElement();
		if (root)
		{
			bool use_test = false;
			Json::Value srv_config;
			if (auto pchar = root->Attribute("kNIMLbsAddress")) {
				srv_config[nim::kNIMLbsAddress] = pchar;
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMNosLbsAddress")) {
				srv_config[nim::kNIMNosLbsAddress] = pchar;
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMDefaultLinkAddress")) {
				srv_config[nim::kNIMDefaultLinkAddress].append(pchar);
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMDefaultNosUploadAddress")) {
				srv_config[nim::kNIMDefaultNosUploadAddress].append(pchar);
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMDefaultNosDownloadAddress")) {
				srv_config[nim::kNIMDefaultNosDownloadAddress].append(pchar);
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMDefaultNosAccessAddress")) {
				srv_config[nim::kNIMDefaultNosAccessAddress].append(pchar);
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMRsaPublicKeyModule")) {
				srv_config[nim::kNIMRsaPublicKeyModule] = pchar;
				use_test = true;
			}
			if (auto pchar = root->Attribute("kNIMRsaVersion")) {
				srv_config[nim::kNIMRsaVersion] = atoi(pchar);
				use_test = true;
			}
			if (use_test)
			{
				config_root[nim::kNIMPrivateServerSetting] = srv_config;
			}
		}
	}

	//sdk�������������
	Json::Value config_values;
	config_values[nim::kNIMDataBaseEncryptKey] = "Netease"; //string��db key���Ŀǰֻ֧�����32���ַ��ļ�����Կ������ʹ��32���ַ���
	config_values[nim::kNIMPreloadAttach] = true;        //bool ��ѡ��Ƿ���ҪԤ���ظ�������ͼ�� sdkĬ��Ԥ���أ�
	config_root[nim::kNIMGlobalConfig] = config_values;

	nim_ui::InitManager::GetInstance()->InitSdk("Netease", "", config_root.toStyledString());
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
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

	srand( (unsigned int) time(NULL) );

	::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	QCommand::ParseCommand(lpszCmdLine);

	HRESULT hr = ::OleInitialize(NULL);
	if( FAILED(hr) )
		return 0;

	InitNim(); // ��ʼ������

	{
		MainThread thread; // �������߳�
		thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop); // ִ�����߳�ѭ��
	}
	QLOG_APP(L"exit ui loop");

	// �������֮ǰ����������sdk
	nim_ui::InitManager::GetInstance()->CleanupSdk();
	QLOG_APP(L"app exit");

	// �Ƿ��������г���
	std::wstring restart = QCommand::Get(kCmdRestart);
	if( !restart.empty() )
	{
		std::wstring cmd;
		std::wstring acc = QCommand::Get(kCmdAccount);
		if( !acc.empty() )
			cmd.append( nbase::StringPrintf(L" /%s %s ", kCmdAccount.c_str(), acc.c_str()) );
		std::wstring exit_why = QCommand::Get(kCmdExitWhy);
		if( !exit_why.empty() )
			cmd.append( nbase::StringPrintf(L" /%s %s ", kCmdExitWhy.c_str(), exit_why.c_str()) );
		QCommand::RestartApp(cmd);
	}

	_Module.Term();
	::OleUninitialize();

	return 0;
}

