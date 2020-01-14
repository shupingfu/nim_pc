#include "js_handler.h"
#include "../util.h"
#include "base/util/string_util.h"
#include "cef/cef_module/js_bridge/ipc_string_define.h"
#include "cef/cef_module/js_bridge/cef_js_bridge.h"

namespace nim_cef
{

bool CefJSHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	// ��Web�е�����"NimCefWebFunction"�����󣬻ᴥ�������Ȼ��Ѳ������棬ת����Broswer����
	// Broswer���̵�BrowserHandler����OnProcessMessageReceived�ӿ��д���kJsCallbackMessage��Ϣ���Ϳ����յ������Ϣ

	if (arguments.size() < 2)
	{
		exception = "Invalid arguments.";
		return false;
	}

	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	CefRefPtr<CefFrame> frame = context->GetFrame();
	CefRefPtr<CefBrowser> browser = context->GetBrowser();

	if (name == "call")
	{
		// ����û�в����б�ĵ��ã��ڶ�������Ϊ�ص�
		// ��������˲����б���ô�ص��ǵ���������
		CefString function_name = arguments[0]->GetStringValue();
		CefString params = "{}";
		CefRefPtr<CefV8Value> callback;
		if (arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			callback = arguments[1];
		}
		else if (arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsFunction())
		{
			params = arguments[1]->GetStringValue();
			callback = arguments[2];
		}
		else
		{
#if defined(CEF_STRING_TYPE_UTF16)
			exception = L"Invalid arguments.";
#else
			exception = "Invalid arguments.";
#endif
			return false;
		}

		// ִ�� C++ ����
		if (!js_bridge_->CallCppFunction(function_name, params, callback))
		{
#if defined(CEF_STRING_TYPE_UTF16)
			exception = nbase::UTF8ToUTF16(nbase::StringPrintf("Failed to call function %s.", function_name.c_str())).c_str();
#else
			exception = nbase::StringPrintf("Failed to call function %s.", function_name).c_str();
#endif
			return false;
		}

		return true;
	}
	else if (name == "register")
	{
		if (arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			std::string function_name = arguments[0]->GetStringValue();
			CefRefPtr<CefV8Value> callback = arguments[1];
			if (!js_bridge_->RegisterJSFunc(function_name, callback))
			{
				exception = "Failed to register function.";
				return false;
			}
			return true;
		}
		else
		{
			exception = "Invalid arguments.";
			return false;
		}
	}

	return false;
}

}
