/** @file browser_handler.h
  * @brief ʵ��CefClient�ӿڣ�����Cef��������󷢳��ĸ����¼�����Ϣ�������ϲ�ؼ��������ݽ���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/7/19
*/
#pragma once
#include "include/cef_client.h"
#include "include/cef_browser.h"

namespace nim_cef
{
// BrowserHandler implements CefClient and a number of other interfaces.
class BrowserHandler : 
	public nbase::SupportWeakCallback,
	public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	public CefDragHandler,
	public CefGeolocationHandler,
	public CefJSDialogHandler,
	public CefKeyboardHandler,	
	public CefLoadHandler,
	public CefRequestHandler
{
public:
	BrowserHandler();

	/** @class HandlerDelegate
	 * @brief BrowserHandler����Ϣί����ӿڣ�BrowserHandler��ᴦ�����Cef�����������¼�
	 *		 ����һЩ��Ҫ���ϲ㽻�����¼��ӿڷ�װ�������У�BrowserHandler����Щ�¼����ݵ�ί�нӿ���
	 *		 ���Ը�����������չ�˽ӿ�
	 */
	class HandlerDelegate : public virtual nbase::SupportWeakCallback
	{
	public:

		// �ڷ�UI�߳��б�����
		virtual void OnPaint(CefRefPtr<CefBrowser> browser,
			CefRenderHandler::PaintElementType type,
			const CefRenderHandler::RectList& dirtyRects,
			const void* buffer,
			int width,
			int height) = 0;

		virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) = 0;

		virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) = 0;

		virtual void UpdateWindowPos() = 0;

		virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame, const CefString& url) = 0;

		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) = 0;

		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) = 0;

		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame) = 0;

		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) = 0;

		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) = 0;

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
			bool* no_javascript_access) = 0;

		// �ڷ�UI�߳��б�����
		virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) = 0;

		// �ڷ�UI�߳��б�����
		virtual ReturnValue OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) = 0;

		virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) = 0;

		// �ڷ�UI�߳��б�����
		virtual void OnJsCallback(const CefString& fun_name, const CefString& param) = 0;
	};

public:

	// ����Cef��������������Ĵ���ľ��
	void SetHostWindow(HWND hwnd){ hwnd_ = hwnd; }

	// ����ί����ָ�룬����������һЩ�¼��ύ����ָ�����������
	// ��ָ����ָ�Ķ�����Ҫ�����¼�ʱ��Ӧ�ø���������NULL
	void SetHandlerDelegate(HandlerDelegate *handler){ handle_delegate_ = handler; }

	// ����Cef��Ⱦ���ݵĴ�С
	void SetViewRect(RECT rc);

	// ��ǰWebҳ���л�ȡ�����Ԫ���Ƿ�ɱ༭
	bool IsCurFieldEditable(){ return is_focus_oneditable_field_; }

	CefRefPtr<CefBrowser> GetBrowser(){ return browser_; }

	CefRefPtr<CefBrowserHost> GetBrowserHost();

	// ���һ�����񵽶����У���Browser���󴴽��ɹ��󣬻����δ�������
	// ���紴��Browser�����LoadUrl������ҳ��������ʱBrowser�ܿ��ܻ�û�д����ɹ����Ͱ�LoadUrl������ӵ�����
	void AddAfterCreateTask(const StdClosure& cb);

public:
	// CefClient methods. Important to return |this| for the handler callbacks.
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {	return this; }
	virtual CefRefPtr<CefRenderHandler>  GetRenderHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() { return this; }
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{ return this; }
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)	OVERRIDE;

	// CefLifeSpanHandler methods
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

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefRenderHandler methods
	virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;

	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;

	virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) OVERRIDE;

	virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;

	virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE;

	virtual void OnPaint(CefRefPtr<CefBrowser> browser,	PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;

	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type,	const CefCursorInfo& custom_cursor_info) OVERRIDE;

	// CefContextMenuHandler methods
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;

	// CefDisplayHandler methods
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;

	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;

	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) OVERRIDE;

	// CefLoadHandler methods
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;

	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame) OVERRIDE;

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;

	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

	// CefJSDialogHandler methods
	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		const CefString& accept_lang,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message) OVERRIDE;

	// CefRequestHandler methods
	bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool is_redirect) OVERRIDE;

	cef_return_value_t OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		int64 new_size,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
		TerminationStatus status) OVERRIDE;

protected:
	CefRefPtr<CefBrowser>	browser_;
	HWND					hwnd_;
	HandlerDelegate			*handle_delegate_;
	RECT					rect_cef_control_;
	std::string				paint_buffer_;
	bool					is_focus_oneditable_field_;

	std::vector<StdClosure> task_list_after_created_;
	IMPLEMENT_REFCOUNTING(BrowserHandler);
};
}