#pragma once
#include "include/cef_browser.h"
#include "cef_control/cef_control.h"

class CefChatroomList : public ui::Box
{
public:
	CefChatroomList();
	~CefChatroomList();

	virtual void DoInit() override;
	static ui::Control* CreateControl(const std::wstring& pstrClass);

	void InvokChatroomList();

private:
	void ShowChatroomById(const std::string& params, nim_cef::ReportResultFunction callback);

private:
	/**
	 * �� browser �����ص��÷�������ʱ������ native ��ע���ṩ js ���õķ����ˡ�
	 */
	void OnAfterCreated(CefRefPtr<CefBrowser> browser);
	/**
	 * �Ҽ��˵�
	 */
	void OnBeforeContextMenu(CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model);
	/**
	 * ҳ��������
	 */
	void OnLoadEnd(int httpStatusCode);

private:
	ui::CefControl*	cef_control_;
};