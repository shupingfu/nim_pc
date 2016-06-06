﻿#include "nim_ui_team_manager.h"


namespace nim_ui
{

TeamManager::TeamManager()
{

}

TeamManager::~TeamManager()
{

}


UnregisterCallback TeamManager::RegAddTeam(OnTeamAdd add)
{
	return nim_comp::TeamService::GetInstance()->RegAddTeam(add);
}

UnregisterCallback TeamManager::RegRemoveTeam(OnTeamRemove remove)
{
	return nim_comp::TeamService::GetInstance()->RegRemoveTeam(remove);
}

UnregisterCallback TeamManager::RegChangeTeamName(OnTeamNameChange change)
{
	return nim_comp::TeamService::GetInstance()->RegChangeTeamName(change);
}

UnregisterCallback TeamManager::RegAddTeamMember(OnTeamMemberAdd add)
{
	return nim_comp::TeamService::GetInstance()->RegAddTeamMember(add);
}

UnregisterCallback TeamManager::RegRemoveTeamMember(OnTeamMemberRemove remove)
{
	return nim_comp::TeamService::GetInstance()->RegRemoveTeamMember(remove);
}

UnregisterCallback TeamManager::RegChangeTeamMember(OnTeamMemberChange change)
{
	return nim_comp::TeamService::GetInstance()->RegChangeTeamMember(change);
}

UnregisterCallback TeamManager::RegSetTeamAdmin(OnTeamAdminSet admin)
{
	return nim_comp::TeamService::GetInstance()->RegSetTeamAdmin(admin);
}

UnregisterCallback TeamManager::RegChangeTeamOwner(OnTeamOwnerChange set_team_owner)
{
	return nim_comp::TeamService::GetInstance()->RegChangeTeamOwner(set_team_owner);
}

std::wstring TeamManager::GetTeamName(const std::string& tid)
{
	return nim_comp::TeamService::GetInstance()->GetTeamName(tid);
}

void TeamManager::QueryAllTeamInfo()
{
	nim_comp::TeamService::GetInstance()->QueryAllTeamInfo();
}

void TeamManager::InvokeChangeTeamMember(const std::string& tid, const std::string& uid, const std::string& team_card)
{
	nim_comp::TeamService::GetInstance()->InvokeChangeTeamMember(tid, uid, team_card);
}

UnregisterCallback TeamManager::RegMuteMember(OnMuteMember mute)
{
	return nim_comp::TeamService::GetInstance()->RegMuteMember(mute);
}

void TeamManager::InvokeTeamDataSyncCallback(nim::NIMDataSyncType sync_type, nim::NIMDataSyncStatus status, const std::string &data_sync_info)
{
	nim_comp::TeamService::GetInstance()->InvokeTeamDataSyncCallback(sync_type, status, data_sync_info);
}

}

