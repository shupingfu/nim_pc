#pragma once

#include "gui/tray_icon/tray_icon.h"
#include "shared/ui/ui_menu.h"
#include "shared/ui/msgbox.h"
#include "base/framework/task.h"
#include "gui/main/control/custom_button_box.h"


/** @class MainForm
  * @brief demo���������洰����
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/1/1
  */
namespace nim_comp
{
	class IMainFormMenuHandler;
}
class MainForm : 
	public nim_comp::WindowEx,
	public ITrayIconDelegate
{
public:
	MainForm();
	~MainForm();

	/**
	* �麯����ָ���������xml�����ļ���ͼƬ�ز����ڵ�Ŀ¼�����·��
	* @return std::wstring ���ظ�Ŀ¼�����[��װĿ¼]/bin/resources/themes/default��·��
	*/
	virtual std::wstring GetSkinFolder() override;

	//�����麯��
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual UINT GetClassStyle() const override;
	
	/**
	* ������������Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void OnFinalMessage(HWND hWnd);

	/**
	* ���ز�����WM_CLOSE��Ϣ
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
	* @return void	�޷���ֵ
	*/
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	* ����ESC������Ϣ����
	* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void OnEsc(BOOL &bHandled);

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

private:
	/**
	* �������пؼ���������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool Notify(ui::EventArgs* msg);

	/**
	* �������пؼ�������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnClicked(ui::EventArgs* msg);

	/**
	* ���������ڵ����˵���ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool MainMenuButtonClick(ui::EventArgs* param);

	/**
	* �������˵�
	* @param[in] point ��������
	* @return void	�޷���ֵ
	*/
	void PopupMainMenu(POINT point);

	/**
	* ����鿴��־Ŀ¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool LookLogMenuItemClick(ui::EventArgs* param);

	/**
	* �����ҵ��ֻ��˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool FileTransMenuItemClick(ui::EventArgs* param);

	/**
	* ����ˢ��ͨѶ¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool AddressMenuItemClick(ui::EventArgs* param);

	/**
	* �����������¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ExportMsglogMenuItemClick(ui::EventArgs* param);

	/**
	* �����������¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ImportMsglogMenuItemClick(ui::EventArgs* param);

	/**
	* ����������������¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ClearChatRecordMenuItemClick(bool del_session, ui::EventArgs* param);

	/**
	* ������ռ�¼�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ClearChatRecordBySessionTypeMenuItemClick(bool del_session, nim::NIMSessionType type, ui::EventArgs* param);

	/**
	* ��������Ƶ���ò˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool VChatSettingMenuItemClick(ui::EventArgs* param);

	/**
	* ����װ�طŲ˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool RtsReplayMenuItemClick(ui::EventArgs* param);

	/**
	* ��������DPI�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool AdaptDpiMenuItemClick(ui::EventArgs* param);

	/**
	* ��ʾ�����б�
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ShowLanguageList(ui::EventArgs* param);

	/**
	* �ر������б�
	* @param[in] msg ��Ϣ�������Ϣ
	* @param[in] check_mouse true: ��鵽����������б��ϾͲ��رգ�false����������λ�ã�ֱ�ӹر�
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool CloseLanguageList(ui::EventArgs* param, bool check_mouse);

	/**
	* ѡ������
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnSelectLanguage(ui::EventArgs* param);

	/**
	* ѡ�����ԵĻص�
	* @param[in] ret ѡ��İ�ť
	* @return void �޷���ֵ
	*/
	void OnSelectLanguageCallback(MsgBoxRet ret, const std::string& language);

	/**
	* ������ʾ�Ự�б�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool SessionListMenuItemClick(ui::EventArgs* param);

	/**
	* ������ڲ˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool AboutMenuItemClick(ui::EventArgs* param);

	/**
	* ����ע���˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool LogoffMenuItemClick(ui::EventArgs* param);

	/**
	* �����˳��˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool QuitMenuItemClick(ui::EventArgs* param);

	/**
	* ����״̬�˵���ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnlineStateMenuButtonClick(ui::EventArgs* param);

	/**
	* ����״̬�˵���ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnlineStateMenuItemClick(ui::EventArgs* param);

	/**
	* ������������������·�������״̬
	* @param[in] json ����������Ϣ
	* @return void	�޷���ֵ
	*/
	void CheckOnlineState(const Json::Value& json);

	/**
	* ��������״̬
	* @return void	�޷���ֵ
	*/
	void SetOnlineState();

	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

public:
	/**
	* ʵ��ITrayIconDelegate�ӿں���
	* @return void	�޷���ֵ
	*/
	virtual void LeftClick() override;
	virtual void LeftDoubleClick() override;
	virtual void RightClick() override;

private:
	/**
	* �������������ݸı����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool SearchEditChange(ui::EventArgs* param);

	/**
	* ����������������ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnClearInputBtnClicked(ui::EventArgs* param);

	/**
	* �������̲˵�
	* @param[in] point ��������
	* @return void	�޷���ֵ
	*/
	void PopupTrayMenu(POINT point);

private:
	/**
	* ��Ӧ�������ϸı�Ļص�����
	* @param[in] uinfos �µĸ��������б�
	* @return void �޷���ֵ
	*/
	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);

	/**
	* ��Ӧͷ��������ɵĻص�����
	* @param[in] type ͷ������
	* @param[in] account ͷ��������ɵ��û�id
	* @param[in] photo_path ͷ�񱾵�·��
	* @return void �޷���ֵ
	*/
	void OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path);

	/**
	* ��Ӧͷ����Ự�б�δ����Ϣ�����ı�Ļص�����
	* @param[in] unread_count δ������
	* @return void �޷���ֵ
	*/
	void OnUnreadCountChange(int unread_count);

	/**
	* ��ʼ���û�ͷ��
	* @return void	�޷���ֵ
	*/
	void InitHeader();
	bool OnReturnEventsClick(ui::EventArgs* param);

public:
	static const LPCTSTR kClassName;

private:
	ui::Button*		btn_header_;
	ui::Label*		label_name_;
	ui::Button*		btn_online_state_;
	ui::HBox* option_panel_ = nullptr;
	bool			is_busy_;

	nim_comp::CustomButtonBox*	box_unread_;
	ui::Label*		label_unread_;

	ui::RichEdit*	search_edit_;
	ui::Button*		btn_clear_input_;
	ui::ListBox*	search_result_list_;
	bool			is_trayicon_left_double_clicked_;

	AutoUnregister	unregister_cb;
	nim_comp::IMainFormMenuHandler* main_menu_handler_;
};

using namespace nbase;
DISABLE_RUNNABLE_METHOD_REFCOUNT(MainForm);
