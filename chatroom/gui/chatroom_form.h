#pragma once
#include "module/emoji/richedit_util.h"
#include "module/emoji/richedit_olecallback.h"
#include "cef/cef_module/cef_control/cef_control.h"
#include <time.h>

namespace nim_chatroom
{
enum SenderType
{
	kMember,
	kJsb,
	kRobot
};

class ChatroomForm : public nim_comp::WindowEx, public IDropTarget, public ui::VirtualListInterface
{
public:
	ChatroomForm(__int64 room_id);
	~ChatroomForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;

	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual UINT GetClassStyle() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;

	virtual void InitWindow() override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void Close(UINT nRet = IDOK) override;

	void OnWndSizeMax(bool max);

public:
	void RequestEnter(const __int64 room_id);
	void AnonymousLogin(const __int64 room_id);
	__int64 GetRoomId();

	void OnReceiveMsgCallback(const ChatRoomMessage& result);
	void OnReceiveMsgsCallback(const std::list<ChatRoomMessage>& result);
	void OnEnterCallback(int error_code, const ChatRoomInfo& info, const ChatRoomMemberInfo& my_info);
	void OnGetChatRoomInfoCallback(__int64 room_id, int error_code, const ChatRoomInfo& info);
	void OnNotificationCallback(const ChatRoomNotification& notification);
	void OnChatRoomRequestEnterCallback(int error_code, const std::string& result);
	void OnRegLinkConditionCallback(__int64 room_id, const NIMChatRoomLinkCondition condition);
	void OnSetMemberAttributeCallback(__int64 room_id, int error_code, const ChatRoomMemberInfo& info);
	void OnKickMemberCallback(__int64 room_id, int error_code);
	void OnTempMuteCallback(__int64 room_id, int error_code, const ChatRoomMemberInfo& info);

public:
	void SetAnonymity(bool anonymity);

private: // cef
	void OnLoadEnd(int httpStatusCode);
	void OnAfterCreated(CefRefPtr<CefBrowser> browser);
	void OnBeforeContextMenu(CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model);
	void OnBeforeClose(CefRefPtr<CefBrowser> browser);

private:
	void InitHeader();
	void GetOnlineCount();	// ˢ����������

	void GetMembers();
	void OnGetMembersCallback(__int64 room_id, int error_code, const std::list<ChatRoomMemberInfo>& infos);

	void GetHistorys();
	void GetMsgHistoryCallback(__int64 room_id, int error_code, const std::list<ChatRoomMessage>& msgs);

	void OnHttoDownloadReady(HttpResourceType type, const std::string& account, const std::wstring& photo_path);
	void OnRequestRoomError();
	void Logout();

private:
	void AddMsgItem(const ChatRoomMessage& result, bool is_history, bool first_msg_each_batch = false);
	void AddNotifyItem(const ChatRoomNotification& notification, bool is_history, bool first_msg_each_batch = false);
	void AddText(const std::wstring &text, const std::wstring &sender_name, const std::string &sender_id, SenderType sender_type, bool is_history, bool first_msg_each_batch = false);
	void AddNotify(const std::wstring &notify, bool is_history, bool first_msg_each_batch = false);
	void AddJsb(const int value, const std::wstring &sender_name, const std::string& sender_id, bool is_history, bool first_msg_each_batch = false);
	void OnBtnSend();
	void OnBtnLogin();
	void SendText(const std::string &text);
	void OnBtnJsb();
	void SendJsb(const std::string &attach);
	void SendImage(const std::wstring &src);

	LRESULT HandleDiscuzMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void GetMemberInfo(const std::string& params, nim_cef::ReportResultFunction callback);
	void SetMemberAdmin(const std::string &id, bool is_admin);	//���ó�Ա�б������ĳ����Ա������
	void SetMemberBlacklist(const std::string &id, bool is_black);
	void SetMemberMute(const std::string &id, bool is_mute);
	void SetMemberFixed(const std::string &id, bool is_fixed);
	void SetMemberTempMute(const std::string &id, bool temp_mute, __int64 duration);
	void SetRoomMemberMute(bool mute);
	void RemoveMember(const std::string &uid);

private:
	virtual bool OnClicked(ui::EventArgs* msg);
	bool OnSelectChanged(ui::EventArgs* param);
	bool OnEditEnter(ui::EventArgs* param);
	void OnBtnEmoji();
	void OnEmotionSelected(std::wstring emo);
	void OnEmotionSelectedSticker(const std::wstring &catalog, const std::wstring &name);
	void OnEmotionClosed();
	bool OnLinkClick(WPARAM wParam, LPARAM lParam);
	bool OnMemberMenu(ui::EventArgs* args);

	void ShowMemberMenu(std::wstring &name);
	bool KickMenuItemClick(ui::EventArgs* args);
	bool AddMuteMenuItemClick(ui::EventArgs* args);
	bool RemoveMuteMenuItemClick(ui::EventArgs* args);
	bool AddBlacklistMenuItemClick(ui::EventArgs* args);
	bool RemoveBlacklistMenuItemClick(ui::EventArgs* args);
	bool AddAdminMenuItemClick(ui::EventArgs* args);
	bool RemoveAdminMenuItemClick(ui::EventArgs* args);
	bool TempMuteMenuItemClick(ui::EventArgs* args);
	bool RemoveTempMuteMenuItemClick(ui::EventArgs* args);

	//@������صĲ���
#pragma region At
private:
	/**
	* ����������@��صİ�����Ϣ
	* @param[in] wParam ������Ϣ
	* @param[in] lParam ������Ϣ
	* @return bool true ������������Ϣ��false ����������Ϣ
	*/
	bool HandleAtMsg(WPARAM wParam, LPARAM lParam);

	/**
	* ����������@��ص���������Ϣ
	* @param[in] wParam ������Ϣ
	* @param[in] lParam ������Ϣ
	* @return bool true ������������Ϣ��false ����������Ϣ
	*/
	bool HandleAtMouseWheel(WPARAM wParam, LPARAM lParam);

	/**
	* ����@�б���
	* @return void	�޷���ֵ
	*/
	void HideAtListForm();

	/**
	* ��ʼ���������б�
	* @return void	�޷���ֵ
	*/
	void InitRobots();
#pragma endregion At
	void DownloadImage(const std::string &url, const std::wstring &photo_path, bool is_complex_element, bool is_history);
	void OnDownloadCallback(bool success, const std::string& file_path, bool is_complex_element, bool is_history);

	/**
	* �������пؼ���������Ϣ
	* @param[in] param ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool Notify(ui::EventArgs* param);

private:
//	ui::Control* AddMemberItem(const ChatRoomMemberInfo& info);
	virtual ui::Control* CreateElement() override;
	virtual void FillElement(ui::Control *control, int index) override;
	virtual int GetElementtCount() override;
	bool OnNameMenu(const std::string &id, const std::string &name, ui::EventArgs* arg);

private:
	void OnDropFile(HDROP hDrop);
	BOOL CheckDropEnable(POINTL pt);
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT STDMETHODCALLTYPE DragLeave(void);
	HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);

	std::wstring GetCurrentTimeString();
	void TimeHandle();

public:
	static const LPTSTR kClassName;
	static const int kAllowClose = 10;
	static const int kForceClose = 11;

private:
	__int64			room_id_;
	bool			is_anonymity_ = false;
	std::string		room_enter_token_;

	bool			has_enter_;
	std::string		creater_id_;		//��������Ϣ

private:	
	ui::Control*	my_icon_ = NULL;	//������ͷ��
	ui::Label*		my_name_ = NULL;	//����������

	ui::Control*	host_icon_ = NULL;	//����ͷ��
	ui::Label*		host_name_ = NULL;	//��������

	ui::Label*		room_name_ = NULL;
	ui::Label*		online_num_ = NULL;
	ui::RichEdit*	bulletin_ = NULL;	//����

	ui::TabBox*		list_tab_ = NULL;
	ui::CefControl*	msg_list_ = NULL;
	ui::RichEdit*	input_edit_ = NULL;
	ui::CheckBox*	btn_face_ = NULL;

	ui::Option*		option_online_members_ = NULL;
//	ui::ListBox*	online_members_list_ = NULL;
	ui::VirtualListBox*	online_members_virtual_list_ = NULL;
	ui::Control*	empty_members_list_ = NULL;

private:
	bool has_add_creater_ = false;			// �Ƿ��ڳ�Ա�б�������˴�����
	std::map<std::string, ChatRoomMemberInfo> members_map_;	//��Ա��Ϣ <id,info>
	std::deque<std::string> members_list_;	//��ͨ��Ա�б�
	std::vector<std::string> managers_list_;//����Ա�б�
	std::map<std::string, std::string> nick_account_map_;	//��¼���ܵ�����Ϣ���ǳƶ�Ӧ���ʺţ�����ͨ���ǳƲ鵽�ʺ�

	std::string		clicked_user_account_;	//�������˵���Ϣ�б��е��ʺ�
	std::string		kicked_user_account_;	//���߳����ʺ�
	__int64			time_start_history_;		//��ȡ���������ʷ��Ϣʱ��
	time_t			time_refresh_ = 0;			//��һ��ˢ�����߳�Ա�б��ʱ��
	bool			is_loading_history_ = false;

	IDropTarget		*droptarget_;
	nim_comp::IRichEditOleCallbackEx *richeditolecallback_;
	AutoUnregister	unregister_cb;

	std::map<std::string, nbase::WeakCallbackFlag> temp_unmute_id_task_map_;
	bool room_mute_;

	struct AtSomeone
	{
		std::string uid_;
		bool is_robot_ = false;
	};
	std::map<std::string, AtSomeone>	uid_at_someone_;//��ǰ�����@���˵��ǳƺ�uid
	bool has_robots_ = false;
	Json::Value		complex_json_;

	std::map<std::wstring, Json::Value> msg_list_sender_name_link_;
	std::map<std::string, Json::Value> descripts_info_;

	enum SWITCH_STATUS
	{
		kNone = 0,
		kToLogined = 1,
		kToAnonymous = 2,
	};
	SWITCH_STATUS switch_to_login_status_ = kNone;

	ChatRoomMemberInfo my_info_;
};

}