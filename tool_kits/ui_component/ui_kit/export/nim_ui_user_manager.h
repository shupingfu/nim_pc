﻿#pragma once

#include "nim_ui_dll.h"
#include "module/service/user_service.h"
#include "tool_kits/base/memory/singleton.h"

namespace nim_ui
{

/** @class UserManager
  * @brief 提供用户数据获取接口
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2015/9/16
  */
class NIM_UI_DLL_API UserManager
{
public:
	SINGLETON_DEFINE(UserManager);
public:
	UserManager();
	~UserManager();
public:
	void InvokeRegisterAccount(const std::string &username, const std::string &password, const std::string &nickname, const OnRegisterAccountCallback& cb);
	void InvokeGetAllUserInfo(const OnGetUserInfoCallback& cb);
	void InvokeUpdateUserInfo(const nim::UserNameCard &new_info, const OnUpdateUserInfoCallback& cb);
	void InvokeChangeUserPhoto(const std::string &url, const OnUpdateUserInfoCallback& cb);
	const std::map<std::string, nim::UserNameCard>& GetAllUserInfos();
	bool GetUserInfo(const std::string &account, nim::UserNameCard &info);
	void GetUserInfos(const std::list<std::string>& ids, std::list<nim::UserNameCard>& uinfos);
	nim::NIMFriendFlag GetUserType(const std::string &id);
	std::wstring GetUserName(const std::string &id, bool alias_prior = true);
	std::wstring GetFriendAlias(const std::string &id);
	UnregisterCallback RegFriendListChange(const OnFriendListChangeCallback& callback);
	UnregisterCallback RegUserInfoChange(const OnUserInfoChangeCallback& callback);
	UnregisterCallback RegMiscUInfoChange(const OnUserInfoChangeCallback& callback);
};

}