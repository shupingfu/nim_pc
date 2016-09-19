#pragma once

namespace nim_comp
{
/** @class AtListItem
* @brief @�б����е��б���
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/9/6
*/
class AtListItem :public ui::ListContainerElement
{
public:
	AtListItem(bool is_last_five);
	~AtListItem();
	
	void InitControls();
	virtual void Activate() override;

	bool IsLastFive() const	{ return is_last_five_; }

	void SetUserID(const std::string& uid);

	std::string GetUserID() const { return uid_;	}

	void SetTeamID(const std::string& tid) { team_id_ = tid;	}

	std::string GetTeamID() const { return team_id_;	}

	void SetAliasName(const std::wstring& name);

	std::string GetAliasName() const	{ return alias_name_; }

	void SetTeamCardName(const std::wstring& name);

	std::string GetTeamCardName() const { return team_card_name_; }

	void SetNickName(const std::wstring& name);

	std::string GetNickName() const { return nick_name_; }

	void SetHeadImage(std::wstring head);

	// �����б�����ʾ�����֣�������ʾ��ע���������ʾȺ��Ƭ�������ʾ�ǳ�
	void SetShowName(const std::wstring& name);
	std::wstring GetShowName() const { return name_label_->GetText(); }

	// �ֱ�ƥ�䱸ע����Ⱥ��Ƭ���ǳƣ�ui;����ɹ��򷵻�true
	bool Match(const std::string& search_key);

public:
	static const char *kLastFiveItemName;
private:
	bool		is_last_five_;	//�Ƿ�Ϊ��ʾ���б���ǰ���������Ե�5��֮һ

	std::string	team_id_;
	ui::Control	*head_icon_;
	ui::Label	*name_label_;

	std::string	uid_;

	std::string	alias_name_;	//�û���ע��,������ʾ
	std::string	alias_full_name_;
	std::string	alias_simple_name_;
	std::string	team_card_name_;//�û�Ⱥ��Ƭ,�����ʾ
	std::string	team_card_full_name_;
	std::string	team_card_simple_name_;
	std::string	nick_name_;		//�û��ǳ�,�����ʾ
	std::string	nick_full_name_;
	std::string	nick_simple_name_;

};
}