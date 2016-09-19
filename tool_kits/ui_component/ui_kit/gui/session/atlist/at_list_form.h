#pragma once
#include "util/window_ex.h"
#include "at_list_item.h"

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
class AtlistForm : public WindowEx
{
public:
	AtlistForm(std::string tid, OnSelectAtItem cb);
	virtual ~AtlistForm();

    //�ӿ�ʵ��
    virtual std::wstring GetSkinFolder() override;
    virtual std::wstring GetSkinFile() override;
    virtual ui::UILIB_RESOURCETYPE GetResourceType() const;
    virtual std::wstring GetZIPFileName() const;
    //�����麯��
    virtual std::wstring GetWindowClassName() const override;
    virtual std::wstring GetWindowId() const override;

	virtual void InitWindow() override;
   
	// @param match_visible:Ϊtrueʱֻ�����б����Ѿ���ʾ����Ŀ����ƥ��ؼ�������ʱʹ�ã�
	//						Ϊfalseʱ���±���������Ŀ����ƥ��Ĺؼ��ּ���ʱʹ�ã�
	// ����ֵ��������ƥ����ʱ����true
	bool Match(const std::wstring& search_key, bool match_visible);

	// ����@�����ɾ����ֻʣ����һ��@����������ʾ���е��б���
	void ShowAllItems(std::list<std::string> &last_five);

	// ����@�б���ʾ��λ�ã�ͬʱ������ʾ��ˢ��@�б�Ĺ���
	void SetShowPos(POINT pt);

	// �Ự�����յ��˼��̺������Ϣ��ת��AtListForm������ĵļ�����Ϣ
	bool HandleKeyEnter();
	bool HandleKeyDown();
	bool HandleKeyUp();
	bool HandleMouseWheel(bool is_up);

	// ֱ�����ٴ���
	void CloseForm();

private:
	// ��д��WM_CLOSE��Ϣ��Ӧ�������Զ����أ�����CloseForm����ֱ�����ٴ���
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	
	/**
	* ��ȡ��Ⱥ��Ա��Ϣ���Զ����õĻص�����
	* @param[in] tid	ȺID
	* @param[in] count	Ⱥ��Ա��
	* @param[in] team_member_info_list Ⱥ��Ա��Ϣ��
	* @return void �޷���ֵ
	*/
	void OnGetTeamMemberCb(const std::string& tid, int count, const std::list<nim::TeamMemberProperty>& team_member_info_list);

	// @param is_last_five:�Ƿ񴴽��������5�������˵�item
	AtListItem* CreateAtListItem(const std::string& uid, bool is_last_five = false);
	// @param is_last_five : �Ƿ񴴽��������5�������˵�item
	// @param index : -1������뵽ĩβ
	AtListItem* AddListItem(const std::string& uid, int index = -1, bool is_last_five = false);
	void RemoveListItem(const std::string& uid);

	// ˢ��ĳ���б����ͷ����ʾ
	void CheckListItemHeadIcon(const std::string& uid, const std::wstring& photo_path);

	// ɾ�����������
	void RemoveLastFiveSender();
	// �����5�������������
	AtListItem* FindInLastFiveSender(const std::wstring& uid);

private:
	std::wstring GetTeamCardName(const std::string& uid);
	std::wstring GetShowName(const std::string& uid);
	bool OnSelectItem(ui::EventArgs *param);
private:
	// ����Ⱥ��Ա�ͺ�����Ϣ�ĸı�
	void OnUserPhotoChange(PhotoType type, const std::string& accid, const std::wstring &photo_path);

	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);
	void OnFriendInfoChange(FriendChangeType change_type, const std::string& accid);
	void OnTeamCardChange(const std::string& tid_uid, const std::string& team_card);

	void OnTeamMemberAdd(const std::string& tid, const nim::TeamMemberProperty& team_member_info);
	void OnTeamMemberRemove(const std::string& tid, const std::string& uid);

public:
    static const LPCTSTR kClassName;

private:
    std::string	team_id_;

	std::map<std::string, nim::TeamMemberProperty> team_member_info_list_;
	ui::ListBox				*team_members_container_;

	std::list<std::string>	uid_last_five_;	//�������Ϣ��5���ˣ��������Լ���,���·��Ե����б����

	OnSelectAtItem			callback_select_;//ѡ��ĳ���Ļص�����
	AutoUnregister	unregister_cb;
};
}