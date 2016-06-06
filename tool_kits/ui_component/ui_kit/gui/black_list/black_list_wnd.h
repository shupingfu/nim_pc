#pragma once

#include "util/window_ex.h"
#include "module/service/photo_service.h"

namespace nim_comp
{
class BlackListWindow : public WindowEx
{
public:
	BlackListWindow();
	virtual ~BlackListWindow();

	//�ӿ�ʵ��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;

	//�����麯��
	virtual std::wstring GetWindowClassName() const override { return kClassName; };
	virtual std::wstring GetWindowId() const override { return kClassName; };
	virtual void InitWindow() override;

	void RefreshBlackList(); // �򿪺���������ʱ����ȡ��������

private:
	bool OnRemoveBtnClicked(ui::EventArgs *args); // ������������ť
	void OnSetBlackCallback(const std::string& id, bool black);
	void RestoreUserInfo(const nim::UserNameCard &info);
	void AddBlackListMember(const nim::UserNameCard &info);
	void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);
	void OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path);

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