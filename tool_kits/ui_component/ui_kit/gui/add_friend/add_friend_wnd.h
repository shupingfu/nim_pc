// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// Author: pqk
// Date: 2013/8/14
#pragma once

#include "util/window_ex.h"
#include "module/service/photo_service.h"

namespace nim_comp
{
const LPCTSTR g_ADDFRIEND_SEARCH_PAGE	  = L"search_page";
const LPCTSTR g_ADDFRIEND_USERINFO_PAGE	  = L"userinfo_page";
const LPCTSTR g_ADDFRIEND_NOTFOUND_PAGE	  = L"notfound_page";
const LPCTSTR g_ADDFRIEND_SENDFRIENDAPPLY_PAGE	  = L"send_friend_apply_page";
const LPCTSTR g_ADDFRIEND_SENDCOMPLETE_PAGE	  = L"send_complete_page";
const LPCTSTR g_ADDFRIEND_SENDTOOFREQUENT_PAGE	  = L"send_too_frequent_page";
const LPCTSTR g_ADDFRIEND_ADDSUCCESS_PAGE	  = L"add_success_page";
const LPCTSTR g_ADDFRIEND_NET_ABNORMAL_PAGE = L"net_abnormal";

/** @class AddFriendWindow
  * @brief ��Ӻ��Ѵ���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author pqk
  * @date 2016/09/19
  */
class AddFriendWindow : public WindowEx
{
public:
	AddFriendWindow();
	virtual ~AddFriendWindow();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override { return kClassName; };
	virtual std::wstring GetWindowId() const override { return kClassName; };
	virtual UINT GetClassStyle() const override { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

	/**
	* ���ز�����ײ㴰����Ϣ
	* @param[in] uMsg ��Ϣ����
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @return LRESULT ������
	*/
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	/**
	* ����������ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool Search(ui::EventArgs* param);

	/**
	* ����ȡ����ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool Cancel(ui::EventArgs* param);

	/**
	* �����û���Ƭ���ò��л��û���Ϣչʾҳ��
	* @param[in] user_info �û���Ƭ
	* @return void	�޷���ֵ
	*/
	void InitUserProfile(const nim::UserNameCard& user_info);

	/**
	* ���������е��ı�
	* @return void	�޷���ֵ
	*/
	void InitEdit();

	enum INIT_TYPE
	{
		NONE,
		SearchAndSendApplyEdit,
		SendApplyEdit,
	};

	/**
	* ������һ��������һ����ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @param[in] page_name Ҫ��ת��Ŀ��ҳ��
	* @param[in] init_edit Ҫ���������������
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool PreOrNextClick(ui::EventArgs* param, const std::wstring& page_name, INIT_TYPE init_edit);

public:
	static const LPCTSTR kClassName;

private:
	/**
	* ������Ӻ��Ѱ�ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool AddFriendClick(ui::EventArgs* param);

	/**
	* ����ʼ���찴ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool ChatClick(ui::EventArgs* param);

	/**
	* ����Ӻ������Ƴ���ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool RemoveFromBlack(ui::EventArgs* args);

	/**
	* ��������Ӻ�������ť�ĵ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool SendFriendApplyClick(ui::EventArgs* param);

	/**
	* ����������ȡ�������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnSearchKeyEditSetFocus(void* param);

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

private:
	ui::Button* headimage_;
	ui::Label* nick_name_;
	ui::RichEdit* search_key_;
	ui::RichEdit* apply_words_;
	ui::TabBox*	tablayout_;
	ui::TabBox* addfriend_or_chat_;
	ui::Button* userinfo_page_add_friend_;

private:
	UTF8String id_;
	bool da_key_;
	AutoUnregister unregister_cb;
};
}
