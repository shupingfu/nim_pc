#pragma once

#include "util/window_ex.h"
#include "module/service/photo_service.h"

namespace nim_comp
{
/** @class BlackListWindow
  * @brief ����������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/09/19
  */
class BlackListWindow : public WindowEx
{
public:
	BlackListWindow();
	virtual ~BlackListWindow();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override { return kClassName; };
	virtual std::wstring GetWindowId() const override { return kClassName; };

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

	/**
	* ˢ�º���������
	* @return void	�޷���ֵ
	*/
	void RefreshBlackList();

private:
	/**
	* ��������ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnRemoveBtnClicked(ui::EventArgs *args);
	
	/**
	* �����û���Ƭ�����б��ж�Ӧ�ĺ�������
	* @param[in] info �û���Ƭ
	* @return void	�޷���ֵ
	*/
	void ResetUserInfo(const nim::UserNameCard &info);

	/**
	* �����û���Ƭ���б�����Ӻ�������
	* @param[in] info �û���Ƭ
	* @return void	�޷���ֵ
	*/
	void AddBlackListMember(const nim::UserNameCard &info);

	/**
	* ��Ӧ�û���Ϣ�ı�Ļص�����
	* @param[in] uinfos �û���Ϣ�б�
	* @return void �޷���ֵ
	*/
	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);

	/**
	* ��Ӧ�û�ͷ��ı�Ļص�����
	* @param[in] type ͷ������
	* @param[in] account �û�id
	* @param[in] photo_path ͷ��·��
	* @return void �޷���ֵ
	*/
	void OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path);

	/**
	* ��Ӧ�������ı�Ļص�����
	* @param[in] id �û�id
	* @param[in] black �Ƿ���������
	* @return void �޷���ֵ
	*/
	void OnSetBlackCallback(const std::string& id, bool black);
public:
	static const LPCTSTR kClassName;

private:
	ui::ListBox*	m_black_list = NULL;
	ui::Button*		m_add_black = NULL;
	ui::TreeView*	m_friend_list = NULL;
	ui::TabBox*		m_page_switch = NULL;
	AutoUnregister	unregister_cb;
};
}