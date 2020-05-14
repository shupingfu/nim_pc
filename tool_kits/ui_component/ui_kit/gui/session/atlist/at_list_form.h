#pragma once
#include "util/window_ex.h"
#include "at_list_item.h"
#include "shared/pin_yin_helper.h"
typedef std::function<void(const std::string& uid)> OnSelectAtItem;

namespace nim_comp
{
/** @class AtlistForm
  * @brief @�б���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/9/6
  */
class AtListItem;
class AtlistForm : public WindowEx, public ui::VirtualListInterface
{
	class ItemMatcher
	{
	public:
		void SetAliasName(const std::wstring& name)
		{
			alias_name_ = nbase::UTF16ToUTF8(name);
			alias_full_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToFullSpell(name));
			alias_simple_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToSimpleSpell(name));
		}

		void SetTeamCardName(const std::wstring& name)
		{
			team_card_name_ = nbase::UTF16ToUTF8(name);
			team_card_full_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToFullSpell(name));
			team_card_simple_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToSimpleSpell(name));
		}

		void SetNickName(const std::wstring& name)
		{
			nick_name_ = nbase::UTF16ToUTF8(name);
			nick_full_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToFullSpell(name));
			nick_simple_name_ = nbase::MakeLowerString(PinYinHelper::GetInstance()->ConvertToSimpleSpell(name));
		}

		bool Match(const std::string& search_key)
		{
			if (alias_name_.find(search_key) != std::string::npos
				|| alias_full_name_.find(search_key) != std::string::npos
				|| alias_simple_name_.find(search_key) != std::string::npos
				|| team_card_name_.find(search_key) != std::string::npos
				|| team_card_full_name_.find(search_key) != std::string::npos
				|| team_card_simple_name_.find(search_key) != std::string::npos
				|| nick_name_.find(search_key) != std::string::npos
				|| nick_full_name_.find(search_key) != std::string::npos
				|| nick_simple_name_.find(search_key) != std::string::npos
				|| uid_.find(search_key) != std::string::npos)
			{
				return true;
			}
			return false;
		}
	public:
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
public:
	/**
	* ���캯��
	* @param[in] session_id �ỰID
	* @param[in] session_type �Ự����
	* @param[in] cb ĳ�ѡ�еĻص�����
	* @return void	�޷���ֵ
	*/
	AtlistForm(std::string session_id, nim::NIMSessionType session_type, OnSelectAtItem cb);
	virtual ~AtlistForm();

	//�����麯��
    virtual std::wstring GetSkinFolder() override;
    virtual std::wstring GetSkinFile() override;
    virtual std::wstring GetWindowClassName() const override;
    virtual std::wstring GetWindowId() const override;
	virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true) override;
	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;
   
	/**
	* Ϊ@�б��ʼ��Ⱥ��Ա��Ϣ
	* @param[in] team_member_info_list Ⱥ��Ա��Ϣ��
	* @return void �޷���ֵ
	*/
	void InitTeamMembers(const std::map<std::string, std::shared_ptr<nim::TeamMemberProperty>>& team_member_info_list);

	/**
	* ���ݹؼ���ȥ��ʾ������@�б���
	* @param[in] search_key �ؼ���
	* @param[in] match_visible true ֻ�����б����Ѿ���ʾ����Ŀ����ƥ��ؼ�������ʱʹ�ã�,false ���±���������Ŀ����ƥ��Ĺؼ��ּ���ʱʹ�ã�
	* @return bool true ƥ��ɹ���false ƥ��ʧ��
	*/
	bool Match(const std::wstring& search_key, bool match_visible);

	/**
	* ����@�����ɾ����ֻʣ����һ��@��������ʾ���е��б���
	* @param[in] last_five ���5��������
	* @return void	�޷���ֵ
	*/
	void ShowMemberItems(std::list<std::string> &last_five);

	/**
	* ����@�б���ʾ��λ�ã�ͬʱ������ʾ��ˢ��@�б�Ĺ���
	* @param[in] pt ��ʾ����
	* @return void	�޷���ֵ
	*/
	void SetShowPos(POINT pt);

	/**
	* �Ự�����յ��˼��̺������Ϣ��ת��AtListForm�������Enter����Ϣ
	* @return bool true �Ѵ���false û�д���
	*/
	bool HandleKeyEnter();

	/**
	* �Ự�����յ��˼��̺������Ϣ��ת��AtListForm�������Down����Ϣ
	* @return bool true �Ѵ���false û�д���
	*/
	bool HandleKeyDown();

	/**
	* �Ự�����յ��˼��̺������Ϣ��ת��AtListForm�������Up����Ϣ
	* @return bool true �Ѵ���false û�д���
	*/
	bool HandleKeyUp();

	/**
	* �Ự�����յ��˼��̺������Ϣ��ת��AtListForm���������������Ϣ
	* @param[in] is_up �Ƿ����Ϲ���
	* @return bool true �Ѵ���false û�д���
	*/
	bool HandleMouseWheel(bool is_up);

	/**
	* ֱ�����ٴ���
	* @return void	�޷���ֵ
	*/
	void CloseForm();
	/**
	 * @brief ����һ������
	 * @return ���ش����������ָ��
	 */
	virtual ui::Control* CreateElement() override;

	/**
	 * @brief ���ָ������
	 * @param[in] control ����ؼ�ָ��
	 * @param[in] index ����
	 * @return ���ش����������ָ��
	 */
	virtual void FillElement(ui::Control* control, int index) override;

	/**
	 * @brief ��ȡ��������
	 * @return ������������
	 */
	virtual int GetElementtCount() override;
private:
	/**
	* ���ز������ڹر���Ϣ�������Զ����أ�����CloseForm����ֱ�����ٴ���
	* @param[in] bHandled �Ƿ�������Ϣ��Ϊtrue�򲻼�������
	* @return LRESULT ������
	*/
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;

	/**
	* ����һ���б���ؼ�
	* @param[in] uid �û�id
	* @param[in] is_last_five �Ƿ�Ϊ���������
	* @return AtListItem* �б���ؼ�
	*/
	AtListItem* CreateAtListItem(const std::string& uid, bool is_last_five);



	/**
	* �Ƴ�һ���б���ؼ�
	* @param[in] uid �û�id
	* @return void	�޷���ֵ
	*/
	void RemoveListItem(const std::string& uid);

	/**
	* ˢ��ĳ���б����ͷ����ʾ
	* @param[in] uid �û�id
	* @param[in] photo_path ͷ��·��
	* @return void	�޷���ֵ
	*/
	void CheckListItemHeadIcon(const std::string& uid, const std::wstring& photo_path);

	/**
	* ɾ�����������
	* @return void	�޷���ֵ
	*/
	void RemoveLastFiveSender();

	/**
	* �����5�������������
	* @param[in] uid �û�id
	* @return AtListItem* �б���ؼ�
	*/
	AtListItem* FindInLastFiveSender(const std::wstring& uid);

private:
	/**
	* ��ȡȺ�ǳ�
	* @param[in] uid �û�id
	* @return std::wstring	Ⱥ�ǳ�
	*/
	std::wstring GetTeamCardName(const std::string& uid);

	/**
	* ��ȡʵ����ʾ������
	* @param[in] uid �û�id
	* @return std::wstring	����
	*/
	std::wstring GetShowName(const std::string& uid);

	/**
	* �������пؼ���ѡ����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnSelectItem(ui::EventArgs *param);
private:
	/**
	* ��Ӧ�û�ͷ��ı�Ļص�����
	* @param[in] type ͷ������
	* @param[in] accid �û�id
	* @param[in] photo_path ͷ��·��
	* @return void	�޷���ֵ
	*/
	void OnUserPhotoChange(PhotoType type, const std::string& accid, const std::wstring &photo_path);

	/**
	* ��Ӧ�û���Ϣ�ı�Ļص�����
	* @param[in] uinfos �û���Ϣ�б�
	* @return void	�޷���ֵ
	*/
	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);

	/**
	* ��Ӧ���Ѹı�Ļص�����
	* @param[in] change_type ���Ѹı�����
	* @param[in] accid �û�id
	* @return void	�޷���ֵ
	*/
	void OnFriendInfoChange(FriendChangeType change_type, const std::string& accid);

	/**
	* ��ӦȺ��Ա�ǳƸı�Ļص�����
	* @param[in] tid_uid ����Ⱥ��id���û�id���ַ���
	* @param[in] team_card Ⱥ�ǳ�
	* @return void	�޷���ֵ
	*/
	void OnTeamCardChange(const std::string& tid_uid, const std::string& team_card);

	/**
	* ��ӦȺ��Ա���ӵĻص�����
	* @param[in] tid Ⱥ��id
	* @param[in] team_member_info Ⱥ��Ա��Ϣ
	* @return void	�޷���ֵ
	*/
	void OnTeamMemberAdd(const std::string& tid, const nim::TeamMemberProperty& team_member_info);

	/**
	* ��ӦȺ��Ա���ٵĻص�����
	* @param[in] tid Ⱥ��id
	* @param[in] uid �û�id
	* @return void	�޷���ֵ
	*/
	void OnTeamMemberRemove(const std::string& tid, const std::string& uid);

	void RefrashShowListData();
	void UpdateLastFive(const std::list<std::string>& last_five);
public:
    static const LPCTSTR kClassName;

private:
    std::string	session_id_;
	nim::NIMSessionType session_type_;

	std::map<std::string, std::shared_ptr<nim::TeamMemberProperty>> team_member_info_list_;
	//ui::ListBox				*team_members_container_ = nullptr;
	ui::VirtualListBox* team_members_container_;
	std::vector<std::shared_ptr<ItemMatcher>> team_member_sort_list_;
	std::vector<std::shared_ptr<ItemMatcher>> team_member_match_list_;
	std::list<std::string>	uid_last_five_;	//�������Ϣ��5���ˣ��������Լ���,���·��Ե����б����
	std::map<std::string, std::string> uid_last_five_for_fined_;

	OnSelectAtItem			callback_select_;//ѡ��ĳ���Ļص�����
	AutoUnregister	unregister_cb;
};
}