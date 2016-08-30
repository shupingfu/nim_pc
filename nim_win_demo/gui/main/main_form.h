#pragma once

#include "gui/tray_icon/tray_icon.h"
#include "shared/ui/ui_menu.h"
#include "base/framework/task.h"


/** @class MainForm
  * @brief demo���������洰����
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/1/1
  */
class MainForm : 
	public nim_comp::WindowEx,
	public TrayIconDelegate
{
public:
	MainForm();
	~MainForm();

	virtual void LeftClick() override;
	virtual void LeftDoubleClick() override;
	virtual void RightClick() override;

	/**
	* �麯����ָ���������xml�����ļ���ͼƬ�ز����ڵ�Ŀ¼�����·��
	* @return std::wstring ���ظ�Ŀ¼�����[��װĿ¼]/bin/themes/default��·��
	*/
	virtual std::wstring GetSkinFolder() override;

	/**
	* �麯����ָ���������xml�����ļ���
	* @return std::wstring ���ظ��ļ���
	*/
	virtual std::wstring GetSkinFile() override;

	/**
	* �麯����ָ��������ʹ�õ���Դ�ļ����ͣ�xml����zip����Ŀǰ��֧��xml����
	* @return ui::UILIB_RESOURCETYPE ����һ��ö��ֵ����ʾ��Դ�ļ�����
	*/
	virtual ui::UILIB_RESOURCETYPE GetResourceType() const;
	virtual std::wstring GetZIPFileName() const;
	
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual UINT GetClassStyle() const override;
	
	virtual void OnFinalMessage(HWND hWnd);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnEsc(BOOL &bHandled);

	/**
	* �麯��������ϵͳ�����һЩUI�¼����細�ڴ��������ڹرգ����ڴ�С�仯����걻��������̰��������£���
	* @param[in] uMsg	ϵͳ������¼�����
	* @param[in] wParam	uMsg�����޶��£�����ϸ���¼�����
	* @param[in] lParam	wParam�����޶��£�����ϸ���¼�����
	* @return LRESULT	����ϵͳ���������ֵ����ʾ������
	*/
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	/**
	* �麯��������WindowEx����ʱ���Զ�ִ�У���ɿؼ��ĳ�ʼ���Ϳؼ��¼��������İ󶨡�
	* @return void �޷���ֵ
	*/
	virtual void InitWindow() override;

	/**
	* Notify, OnCloseBtnClicked, OnClicked: �Զ���Ľ����¼���Ӧ������һ����InitWindow������ע�ᡣ
	* @param[in] msg	�¼�����
	* @return bool	���ش�����
	*/
	bool Notify(ui::EventArgs* msg);
	bool OnCloseBtnClicked(ui::EventArgs* msg);
	bool OnClicked(ui::EventArgs* msg);

private:
	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);
	void OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path);
	void OnUnreadCountChange(int unread_count);
	void InitHeader();

	bool SearchEditChange(ui::EventArgs* param);
	bool OnClearInputBtnClicked(ui::EventArgs* param);
	bool MainMenuButtonClick(ui::EventArgs* param);

	void PopupMainMenu(POINT point);
	void PopupMainTrayMenu(POINT point);
	bool LookLogMenuItemClick(ui::EventArgs* param);
	bool FileTransMenuItemClick(ui::EventArgs* param);
	bool AddressMenuItemClick(ui::EventArgs* param);
	bool ExportMsglogMenuItemClick(ui::EventArgs* param);
	bool ImportMsglogMenuItemClick(ui::EventArgs* param);
	bool ClearChatRecordMenuItemClick(bool del_session, ui::EventArgs* param);
	bool ClearChatRecordBySessionTypeMenuItemClick(bool del_session, nim::NIMSessionType type, ui::EventArgs* param);
	bool VChatSettingMenuItemClick(ui::EventArgs* param);
	bool RtsReplayMenuItemClick(ui::EventArgs* param);
	bool SessionListMenuItemClick(ui::EventArgs* param);
	bool AboutMenuItemClick(ui::EventArgs* param);
	bool LogoffMenuItemClick(ui::EventArgs* param);
	bool QuitMenuItemClick(ui::EventArgs* param);

public:
	static const LPCTSTR kClassName;

private:
	ui::Button*		btn_header_;
	ui::Label*		label_name_;

	ui::Box*		box_unread_;
	ui::Label*		label_unread_;

	ui::RichEdit*	search_edit_;
	ui::Button*		btn_clear_input_;
	ui::ListBox*	search_result_list_;
	bool			is_trayicon_left_double_clicked_;

	AutoUnregister	unregister_cb;
};

using namespace nbase;
DISABLE_RUNNABLE_METHOD_REFCOUNT(MainForm);
