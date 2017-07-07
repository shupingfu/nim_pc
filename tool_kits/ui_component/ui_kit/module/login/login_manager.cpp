﻿#include "login_manager.h"
#include "shared/xml_util.h"

namespace nim_comp
{

namespace
{
	const wchar_t kMutexName[] = L"Netease.IM.PC.Instance.";
}

LoginManager::LoginManager()
{
	account_ = "";
	password_ = "";
	status_ = LoginStatus_NONE;
	active_ = true;
	ReadDemoLogLevel();
}

bool LoginManager::IsSelf( const std::string &account )
{
	return (account == account_);
}

void LoginManager::SetAccount( const std::string &account )
{
	account_ = account;
	nbase::LowerString(account_);;
}

std::string LoginManager::GetAccount()
{
	return account_;
}

bool LoginManager::IsEqual(const std::string& account)
{
	std::string new_account = account;
	nbase::LowerString(new_account);
	return account_ == new_account;
}

void LoginManager::SetPassword( const std::string &password )
{
	password_ = password;
}

std::string LoginManager::GetPassword()
{
	return password_;
}

void LoginManager::SetLoginStatus( LoginStatus status )
{
	status_ = status;
}

LoginStatus LoginManager::GetLoginStatus()
{
	return status_;
}

void LoginManager::SetLinkActive( bool active )
{
	active_ = active;
	
	Json::Value json;
	json["link"] = active;
	NotifyCenter::GetInstance()->InvokeNotify(NT_LINK, json);
}

bool LoginManager::IsLinkActive()
{
	return active_;
}

void LoginManager::CreateSingletonRunMutex()
{
	std::string config = GetConfigValue("check_singleton");
	if (!config.empty() && 0 == atoi(config.c_str()))
		return;

	std::wstring mutex_name = kMutexName + nbase::UTF8ToUTF16(account_);
	::CreateMutex(NULL, TRUE, mutex_name.c_str());
}

bool LoginManager::CheckSingletonRun(const std::wstring& username)
{
	std::string config = GetConfigValue("check_singleton");
	if (!config.empty() && 0 == atoi(config.c_str()))
		return true;

	// 同一PC不允许重复登录同一账号
	std::wstring mutex_name = kMutexName + username;
	HANDLE mutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutex_name.c_str());
	if (mutex != NULL)
	{
		::CloseHandle(mutex);
		return false;
	}
	return true;
}

void LoginManager::ReadDemoLogLevel()
{
	std::wstring server_conf_path = QPath::GetAppPath();
	server_conf_path.append(L"server_conf.txt");
	nim::SDKConfig config;
	TiXmlDocument document;
	if (shared::LoadXmlFromFile(document, server_conf_path))
	{
		TiXmlElement* root = document.RootElement();
		if (root)
		{
			if (auto pchar = root->Attribute("kNIMSDKLogLevel")){
				int log_level = 5;
				nbase::StringToInt((std::string)pchar, &log_level);
				SetDemoLogLevel(log_level);
			}
			if (auto pchar = root->Attribute("LimitFileSize")){
				int file_size = 15;
				nbase::StringToInt((std::string)pchar, &file_size);
				SetFileSizeLimit(file_size);
			}
		}
	}
}

}