#include "js_handler.h"
#include "ipc_string_define.h"
#include "util.h"

namespace nim_cef
{

bool CefJSHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	// ��Web�е�����"NimCefWebFunction"�����󣬻ᴥ�������Ȼ��Ѳ������棬ת����Broswer����
	// Broswer���̵�BrowserHandler����OnProcessMessageReceived�ӿ��д���kJsCallbackMessage��Ϣ���Ϳ����յ������Ϣ
	if (name == "NimCefWebFunction" && arguments.size() == 1)
	{
		for (auto &it : arguments)
		{
			if (it->IsString())
			{
				CefString param = it->GetStringValue();

				CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
				CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(kJsCallbackMessage);

				message->GetArgumentList()->SetString(0, name);
				message->GetArgumentList()->SetString(1, param);
				browser->SendProcessMessage(PID_BROWSER, message);

				retval = CefV8Value::CreateBool(true);
			}
		}
		return true;
	}

	// Function does not exist.
	return false;
}

}
