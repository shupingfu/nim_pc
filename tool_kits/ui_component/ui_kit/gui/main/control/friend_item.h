#pragma once

#include "shared/pin_yin_helper.h"

namespace nim_comp
{
/** @class FriendItem
  * @brief �����б����UI��
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/1/1
  */
class FriendItem : public ui::TreeNode
{
public:
	FriendItem();
	virtual ~FriendItem();

	/**
	* ��ʼ���ؼ�
	* @param[in] is_team �Ƿ�ΪȺ����
	* @param[in] accid �û�id����Ⱥ��id
	* @return void	�޷���ֵ
	*/
	virtual void Init(bool is_team, const std::string &accid);

	/**
	* �Ƿ�ΪȺ����
	* @return bool true �ǣ�false ��
	*/
	bool GetIsTeam() const { return is_team_; }

	/**
	* ��ȡ�û�id����Ⱥ��id
	* @return string �û�id����Ⱥ��id
	*/
	std::string GetId()	{ return id_; }

	/**
	* ���ǳơ��û�������Ϣƥ�������ؼ���
	* @param[in] search_key �ؼ���
	* @return bool true ƥ��ɹ���false ƥ��ʧ��
	*/
	bool Match(const UTF8String& search_key)
	{
		std::wstring ws_search_key = nbase::UTF8ToUTF16(search_key);
		ws_search_key = nbase::MakeLowerString(ws_search_key);
		if (nick_name_.find(ws_search_key) != std::wstring::npos
			|| nick_name_full_spell_.find(search_key) != UTF8String::npos
			|| nick_name_simple_spell_.find(search_key) != UTF8String::npos)
		{
			return true;
		}
		return false;
	}

	/**
	* ����һ��������ؼ����бȽϣ������ǳ��жϴ�С
	* @param[in] elment2 ������ؼ�
	* @return bool true �ǳ�����һ���ؼ�֮ǰ��false �ǳ�����һ���ؼ�֮��
	*/
	bool operator < (const FriendItem& elment2) const
	{
		static DWORD lcid = MAKELCID(MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRCP);
		return ::CompareString(lcid, 0, this->nick_name_.c_str(), this->nick_name_.length(), elment2.nick_name_.c_str(),elment2.nick_name_.length()) == 1;
	}

	/**
	* �ж�����һ��������ؼ�id�Ƿ�һ��
	* @param[in] elment2 ������ؼ�
	* @return bool true һ����false ��һ��
	*/
	bool operator == (const FriendItem& elment2) const 
	{
		return this->id_ == elment2.id_;
	}

	/**
	* ������ѿؼ�˫����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnDbClicked(ui::EventArgs* arg);
private:
	/**
	* ��Ӧ�û�ͷ��ı�Ļص�����
	* @param[in] type ͷ������
	* @param[in] accid �û�id����Ⱥ��id
	* @param[in] photo_path ͷ��·��
	* @return void �޷���ֵ
	*/
	void OnUserPhotoReady(PhotoType type, const std::string& accid, const std::wstring& photo_path);

	/**
	* ��ӦȺ���Ƹı�Ļص�����
	* @param[in] team_info Ⱥ��Ϣ
	* @return void �޷���ֵ
	*/
	void OnTeamNameChange(const nim::TeamInfo& team_info);
private:
	ui::Label*		contact_;

	bool			is_team_;
	std::string		id_;

	std::wstring	nick_name_;
	UTF8String		nick_name_full_spell_;
	UTF8String		nick_name_simple_spell_;

	AutoUnregister unregister_cb;
};
}