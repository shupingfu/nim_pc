#pragma once

#include "shared/list_item_util.h"
#include "gui/main/friend_list_item_manager.h"

/** @class FriendList
  * @brief ��װFriendListItemManager����������б������ṩ��������MainForm
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/1/1
  */
class FriendList : public nbase::SupportWeakCallback
{
public:
	FriendList(ui::TreeView* friend_list);
	virtual ~FriendList();

	void ScrollToLetter(char letter);
	
	ui::TreeView* GetFriendList()
	{
		return friend_list_;
	}

	void OnGetFriendList(const std::list<UserInfo>& uinfos)
	{
		nim_list_base_.OnGetFriendList(uinfos);
	}

private:
	bool OnScrollChange(ui::EventArgs* param);

private:
	ui::Label* tip_letter_;
	ui::TreeNode* pos_tip_;
	ui::TreeView* friend_list_;
	FriendListItemManager nim_list_base_;
};