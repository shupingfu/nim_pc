/** @file cef_control.h
 * @brief ��װCef���������Ϊduilib�ؼ�
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once
#include "include/cef_base.h"
#include "cef_control_event.h"
#include "js_bridge/cef_js_bridge.h"
#include "handler/browser_handler.h"
#include "util/memory_dc.h"

namespace ui 
{

class CefControl :public Control, public IUIMessageFilter, public nim_cef::BrowserHandler::HandlerDelegate
{	
public:
	CefControl(void);
	~CefControl(void);	

	virtual void Init() override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetVisible(bool bVisible = true) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint) override;

	// ��������Ϣ��ת����Cef���������
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;	
protected:
	// ת��������Ϣ��ת����Cef���������
	LRESULT SendButtonDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendButtonDoubleDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendButtonUpEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendMouseMoveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendMouseWheelEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendMouseLeaveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT SendCaptureLostEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	
	static bool IsKeyDown(WPARAM wparam);
	static int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);
	static int GetCefMouseModifiers(WPARAM wparam);
private:
	void ReCreateBrowser();
public:
	// �ؼ�����Ŀ��ƽӿ�
	void LoadURL(const CefString& url);
	void LoadString(const CefString& stringW, const CefString& url);
	void GoBack();
	void GoForward();
	bool CanGoBack();
	bool CanGoForward();
	void Refresh();
	void StopLoad();
	bool IsLoading();

	CefString GetURL();
	std::string GetUTF8URL();
	CefString GetMainURL(const CefString& url);

	bool RegisterCppFunc(const std::wstring& function_name, nim_cef::CppFunction function, bool global_function = false);
	void UnRegisterCppFunc(const std::wstring& function_name);
	bool CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_cef::CallJsFunctionCallback callback, const std::wstring& frame_name = L"");
	bool CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_cef::CallJsFunctionCallback callback, int frame_id);

	virtual bool AttachDevTools(CefControl* view);
	virtual void DettachDevTools();
	virtual bool IsAttachedDevTools() const { return devtool_attached_; };
	virtual void RepairBrowser();
public:
	// �¼�����ӿڻص��������������������Щ�¼������ûص�������֪ͨ
	void AttachBeforeContextMenu(const OnBeforeMenuEvent& callback){ cb_before_menu_ = callback; }
	void AttachMenuCommand(const OnMenuCommandEvent& callback){ cb_menu_command_ = callback; }
	void AttachTitleChange(const OnTitleChangeEvent& callback){ cb_title_change_ = callback; }
	void AttachUrlChange(const OnUrlChangeEvent& callback){ cb_url_change_ = callback; }
	void AttachMainURLChange(OnMainURLChengeEvent cb){ cb_main_url_change_ = cb; }
	void AttachBeforeNavigate(const OnBeforeResourceLoadEvent& callback){ cb_before_resource_load_ = callback; }
	void AttachLinkClick(const OnLinkClickEvent& callback){ cb_link_click_ = callback; }
	void AttachLoadingStateChange(const OnLoadingStateChangeEvent& callback){ cb_loadstate_change_ = callback; }
	void AttachLoadStart(const OnLoadStartEvent& callback){ cb_load_start_ = callback; }
	void AttachLoadEnd(const OnLoadEndEvent& callback){ cb_load_end_ = callback; }
	void AttachLoadError(const OnLoadErrorEvent& callback){ cb_load_error_ = callback; }
	void AttachDevToolAttachedStateChange(const OnDevToolAttachedStateChangeEvent& callback){ cb_devtool_visible_change_ = callback; };
	void AttachAfterCreated(const OnAfterCreatedEvent& callback){ cb_after_created_ = callback; }
	void AttachBeforeCLose(const OnBeforeCloseEvent& callback) { cb_before_close_ = callback; }
	void AttachBeforeBrowser(const OnBeforeBrowserEvent& callback) { cb_before_browser_ = callback; }
	void AttachProtocolExecution(const OnProtocolExecutionEvent& callback) { cb_protocol_execution_ = callback; }

private:
	// ����BrowserHandler��HandlerDelegateί�нӿ�

	// ���������Ⱦ���ݱ仯ʱ���ᴥ���˽ӿڣ���ʱ����Ⱦ���ݱ��浽�ڴ�dc
	// ����֪ͨ����ˢ�¿ؼ����ڿؼ���Paint��������ڴ�dc��λͼ����������
	// �ɴ�ʵ��������Ⱦ���ݻ���������
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::PaintElementType type,
		const CefRenderHandler::RectList& dirtyRects,
		const std::string* buffer,
		int width,
		int height) OVERRIDE;

	virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;

	virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE;

	virtual void UpdateWindowPos() OVERRIDE;

	virtual void UpdateUI() OVERRIDE;

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;

	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		CefContextMenuHandler::EventFlags event_flags) OVERRIDE;

	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;

	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;

	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;

	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame) OVERRIDE;

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;

	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefLoadHandler::ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

	// �ڷ�UI�߳��б�����
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;

	virtual bool OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// �ڷ�UI�߳��б�����
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) OVERRIDE;

	virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) OVERRIDE;

	// �ڷ�UI�߳��б�����
	virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status) OVERRIDE;

	virtual bool OnExecuteCppFunc(const CefString& function_name, const CefString& params, int js_callback_id, CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool OnExecuteCppCallbackFunc(int cpp_callback_id, const CefString& json_string) OVERRIDE;

private:

	CefRefPtr<nim_cef::BrowserHandler>		browser_handler_ = nullptr;
	std::shared_ptr<nim_cef::CefJSBridge>	js_bridge_;
	MemoryDC			dc_cef_;		// �ڴ�dc,��cef������Ⱦ�����ݱ��浽dc��
	MemoryDC			dc_cef_popup_;	// �ڴ�dc,��cef��popup���ڵ�������Ⱦ���ݱ��浽dc��
	CefRect				rect_popup_;	// ����ҳ����Ͽ�һ��Ŀؼ�����ʱ����¼������λ��
	CefString			url_;

	OnBeforeMenuEvent	cb_before_menu_ = nullptr;
	OnMenuCommandEvent	cb_menu_command_ = nullptr;
	OnTitleChangeEvent	cb_title_change_ = nullptr;
	OnBeforeResourceLoadEvent	cb_before_resource_load_ = nullptr;
	OnUrlChangeEvent	cb_url_change_ = nullptr;
	OnMainURLChengeEvent cb_main_url_change_ = nullptr;
	OnLinkClickEvent	cb_link_click_ = nullptr;
	OnLoadingStateChangeEvent	cb_loadstate_change_ = nullptr;
	OnLoadStartEvent	cb_load_start_ = nullptr;
	OnLoadEndEvent		cb_load_end_ = nullptr;
	OnLoadErrorEvent	cb_load_error_ = nullptr;
	OnAfterCreatedEvent	cb_after_created_ = nullptr;
	OnBeforeCloseEvent	cb_before_close_ = nullptr;
	OnBeforeBrowserEvent cb_before_browser_ = nullptr;
	OnProtocolExecutionEvent cb_protocol_execution_ = nullptr;
	OnDevToolAttachedStateChangeEvent cb_devtool_visible_change_ = nullptr;
	bool devtool_attached_;
	int					js_callback_thread_id_ = -1; // ������յ� JS ���� CPP �����Ĵ��������̣߳��Ժ󴥷� JS �ص�ʱ�ѻص�ת���Ǹ��߳�
};

}