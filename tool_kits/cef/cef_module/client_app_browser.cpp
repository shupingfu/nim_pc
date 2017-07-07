#include "client_app.h" 

#include <string>

#include "include/cef_cookie.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#include "util.h"

namespace nim_cef
{
//////////////////////////////////////////////////////////////////////////////////////////
// CefBrowserProcessHandler methods.
void ClientApp::OnContextInitialized() 
{
	// Register cookieable schemes with the global cookie manager.
	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
	ASSERT(manager.get());
	manager->SetSupportedSchemes(cookieable_schemes_, NULL);

	// �������ɾ���˱����Cooies��Ϣ
	// manager->DeleteCookies(L"", L"", nullptr);
}

void ClientApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
#if defined(SUPPORT_CEF_FLASH)	
	if (command_line->HasSwitch("type") && command_line->GetSwitchValue("type") == "ppapi" && command_line->HasSwitch("ppapi-flash-args"))
	{
		// �����flash���̴���
	}
	else
	{
		// ����SUPPORT_CEF_FLASH��󣬻�ʹ��sandboxģ�飬ĳЩ����µ�����Ⱦ�ӽ����޷��������������Զ��ڷ�flash���̹ص�sandbox����
		command_line->AppendSwitch("no-sandbox");
	}
#endif
}

void ClientApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) 
{
}

}