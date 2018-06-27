#pragma once
#include "guiex/main/main_form_menu.h"
class MainFormMenu : public nim_comp::IMainFormMenuHandler, public nbase::SupportWeakCallback
{
protected:
	virtual void OnPopupMainMenu(POINT point) override;
private:
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

};