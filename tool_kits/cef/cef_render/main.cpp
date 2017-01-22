#include "include/cef_client.h"
#include "client_app.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	_wsetlocale(LC_ALL, L"chs");

#ifdef _DEBUG
	AllocConsole();
	FILE* fp = NULL;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
	wprintf_s(L"Command:\n%s\n\n", lpszCmdLine);
#endif

	CefMainArgs main_args(GetModuleHandle(NULL));

	CefRefPtr<nim_cef::ClientApp> app(new nim_cef::ClientApp());

	// ִ���ӽ����߼�����ʱ�����ֱ���ӽ����˳���
	return CefExecuteProcess(main_args, app.get(), NULL);
}
