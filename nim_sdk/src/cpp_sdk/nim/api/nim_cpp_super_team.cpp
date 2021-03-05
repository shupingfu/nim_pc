﻿/** @file nim_cpp_super_team.cpp
  * @brief 超大群组功能；主要包括查询群信息、查询群成员信息、加人、踢人等功能
  * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
  * @date 2015/2/1
  */

#include "src/cpp_sdk/nim/api//nim_cpp_super_team.h"
#include "src/cpp_sdk/nim/helper/nim_sdk_loader_helper.h"
#include "src/cpp_sdk/nim/api//nim_cpp_global.h"
namespace nim
{
#ifdef NIM_SDK_DLL_IMPORT
typedef void(*nim_super_team_reg_team_event_cb)(const char *json_extension, nim_super_team_event_cb_func cb, const void *user_data);
typedef void(*nim_super_team_query_team_info_online_async)(const char *tid, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_invite_async)(const char *tid, const char *jsonlist_uids, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_invite_async2)(const char *tid, const char *jsonlist_uids, const char *invitation_postscript, const char *invitation_attachment, const char *json_extension, nim_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_kick_async)(const char *tid, const char *jsonlist_uids, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_leave_async)(const char *tid, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_update_team_info_async)(const char *tid, const char *json_info, const char *json_extension, nim_super_team_event_cb_func cb_func, const void* user_data);
typedef void(*nim_super_team_update_my_property_async)(const char *info, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_apply_join_async)(const char *tid, const char *reason, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_pass_join_apply_async)(const char *tid, const char *applicant_id, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_reject_join_apply_async)(const char *tid, const char *applicant_id, const char *reason, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_add_managers_async)(const char *tid, const char *jsonlist_admin_ids, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_remove_managers_async)(const char *tid, const char *jsonlist_admin_ids, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_transfer_team_async)(const char *tid, const char *new_owner, bool is_leave, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_update_other_nick_async)(const char *info, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_accept_invitation_async)(const char *tid, const char *invitor, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_reject_invitation_async)(const char *tid, const char *invitor, const char *reason, const char *json_extension, nim_super_team_event_cb_func cb, const void* user_data);
typedef void(*nim_super_team_mute_member_async)(const char *tid, const char *member_id, bool set_mute, const char *json_extension, nim_team_opt_cb_func cb, const void *user_data);
typedef void(*nim_super_team_mute_async)(const char *tid, bool set_mute, const char *json_extension, nim_team_opt_cb_func cb, const void *user_data);
typedef void(*nim_super_team_query_mute_list_async)(const char* tid, const char* json_extension, nim_super_team_query_mute_list_cb_func cb, const void* user_data);
typedef void(*nim_super_team_msg_ack_read)(const char *tid, const char *json_msgs, const char *json_extension, nim_team_opt_cb_func cb, const void *user_data);

typedef void(*nim_super_team_query_all_my_teams_async)(const char *json_extension, nim_super_team_query_all_my_teams_cb_func cb, const void* user_data);
typedef void(*nim_super_team_query_all_my_teams_info_async)(const char *json_extension, nim_super_team_query_all_my_teams_info_cb_func cb, const void* user_data);
typedef void(*nim_super_team_query_teams_info_by_keyword_async)(const char* keyword, const char *json_extension, nim_super_team_query_all_my_teams_info_cb_func cb, const void *user_data);
typedef void(*nim_super_team_query_my_all_member_infos_async)(const char *json_extension,	nim_super_team_query_my_all_member_infos_cb_func cb, const void *user_data);
typedef void(*nim_super_team_query_team_members_async)(const char *tid, bool include_user_info, const char *json_extension, nim_super_team_query_team_members_cb_func cb, const void* user_data);
typedef void(*nim_super_team_query_team_member_async)(const char *tid, const char *user_id, const char *json_extension, nim_super_team_query_team_member_cb_func cb, const void *user_data);
typedef void(*nim_super_team_query_team_info_async)(const char *tid, const char *json_extension, nim_super_team_query_team_info_cb_func cb, const void* user_data);
typedef char*(*nim_super_team_query_team_member_block)(const char *tid, const char *user_id);
typedef char*(*nim_super_team_query_team_info_block)(const char *tid);
#else
#include "nim_super_team.h"
#endif

static void CallbackSuperTeamEvent(int res_code, int notification_id, const char *tid, const char *result, const char *json_extension, const void *user_data)
{

	CallbackProxy::DoSafeCallback<SuperTeam::SuperTeamEventCallback>(user_data, [=](const SuperTeam::SuperTeamEventCallback& cb){
		SuperTeamEvent team_event;
		ParseSuperTeamEvent(res_code, PCharToString(tid), (nim::NIMNotificationId)notification_id, PCharToString(result), team_event);
		CallbackProxy::Invoke(cb, team_event);
	});
}

static void CallbackSuperTeamChange(int res_code, int notification_id, const char *tid, const char *result, const char *json_extension, const void *user_data)
{
	CallbackProxy::DoSafeCallback<SuperTeam::SuperTeamEventCallback>(user_data, [=](const SuperTeam::SuperTeamEventCallback& cb){
		SuperTeamEvent team_event;
		ParseSuperTeamEvent(res_code, PCharToString(tid), (nim::NIMNotificationId)notification_id, PCharToString(result), team_event);
		CallbackProxy::Invoke(cb, team_event);
	},true);

}

static void CallbackQueryMySuperTeams(int team_count, const char *result, const char *json_extension, const void *user_data)
{
	CallbackProxy::DoSafeCallback<SuperTeam::QueryAllMySuperTeamsCallback>(user_data, [=](const SuperTeam::QueryAllMySuperTeamsCallback& cb){
		std::list<std::string> team_id_list;
		JsonStrArrayToList(GetJsonValueFromJsonString(PCharToString(result)), team_id_list);
		CallbackProxy::Invoke(cb, team_count, team_id_list);
	}, true);

}

static void CallbackQuerySuperTeamMembers(int error_code, const char *tid, int member_count, bool include_user_info, const char *result, const char *json_extension, const void *user_data)
{
	CallbackProxy::DoSafeCallback<SuperTeam::QuerySuperTeamMembersCallback>(user_data, [=](const SuperTeam::QuerySuperTeamMembersCallback& cb){
		std::list<nim::SuperTeamMemberProperty> team_member_info_list;
		ParseSuperTeamMemberPropertysJson(PCharToString(result), team_member_info_list);
		CallbackProxy::Invoke(cb, (NIMResCode)error_code,PCharToString(tid), member_count, team_member_info_list);
	}, true);

}

static void CallbackQuerySuperTeamMember(const char *tid, const char *id, const char *result, const char *json_extension, const void *user_data)
{

	CallbackProxy::DoSafeCallback<SuperTeam::QuerySuperTeamMemberCallback>(user_data, [=](const SuperTeam::QuerySuperTeamMemberCallback& cb){
		nim::SuperTeamMemberProperty prop;
		ParseSuperTeamMemberPropertyJson(PCharToString(result), prop);
		CallbackProxy::Invoke(cb, prop);
	}, true);
}

static void CallbackQueryAllMySuperTeamsInfo(int team_count, const char *result, const char *json_extension, const void *user_data)
{
	CallbackProxy::DoSafeCallback<SuperTeam::QueryAllMySuperTeamsInfoCallback>(user_data, [=](const SuperTeam::QueryAllMySuperTeamsInfoCallback& cb){
		std::list<nim::SuperTeamInfo> team_info_list;
		ParseSuperTeamInfosJson(PCharToString(result), team_info_list);
		CallbackProxy::Invoke(cb, team_count, team_info_list);
	}, true);
}

static void CallbackQueryMyAllMemberInfos(int count, const char *result, const char *json_extension, const void *user_data)
{


	CallbackProxy::DoSafeCallback<SuperTeam::QueryMyAllMemberInfosCallback>(user_data, [=](const SuperTeam::QueryMyAllMemberInfosCallback& cb){
		std::list<nim::SuperTeamMemberProperty> my_infos_list;
		ParseSuperTeamMemberPropertysJson(PCharToString(result), my_infos_list);
		CallbackProxy::Invoke(cb, count, my_infos_list);
	}, true);

}

static void CallbackQuerySuperTeamInfo(const char *tid, const char *result, const char *json_extension, const void *callback)
{
	CallbackProxy::DoSafeCallback<SuperTeam::QuerySuperTeamInfoCallback>(callback, [=](const SuperTeam::QuerySuperTeamInfoCallback& cb){
		nim::SuperTeamInfo team_info;
		ParseSuperTeamInfoJson(PCharToString(result), team_info);
		CallbackProxy::Invoke(cb, PCharToString(tid), team_info);
	}, true);

}

static void CallbackQueryMutedMemberList(int res_code, int count, const char* tid, const char* result, const char* json_extension, const void* user_data) {
    CallbackProxy::DoSafeCallback<SuperTeam::QuerySuperTeamMembersCallback>(user_data,
        [=](const SuperTeam::QuerySuperTeamMembersCallback& cb) {
        std::list<nim::SuperTeamMemberProperty> team_member_info_list;
        ParseSuperTeamMemberPropertysJson(PCharToString(result), team_member_info_list);
        CallbackProxy::Invoke(cb, (NIMResCode)res_code, PCharToString(tid), count, team_member_info_list);
    },
        true);
}

static SuperTeam::SuperTeamEventCallback g_cb_super_team_event_ = nullptr;
void SuperTeam::RegSuperTeamEventCb(const SuperTeamEventCallback& cb, const std::string& json_extension)
{
	g_cb_super_team_event_ = cb;
	NIM_SDK_GET_FUNC(nim_super_team_reg_team_event_cb)(json_extension.c_str(), &CallbackSuperTeamEvent, &g_cb_super_team_event_);
}

bool SuperTeam::InviteAsync(const std::string& tid
	, const std::list<std::string>& ids
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || ids.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	std::string uids;
	StrListToJsonString(ids, uids);
	NIM_SDK_GET_FUNC(nim_super_team_invite_async)(tid.c_str()
		, uids.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::InviteAsync2(const std::string& tid
	, const std::list<std::string>& ids
	, const std::string& invitation_postscript
	, const std::string& invitation_attachment
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || ids.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	std::string uids;
	StrListToJsonString(ids, uids);
	NIM_SDK_GET_FUNC(nim_super_team_invite_async2)(tid.c_str()
		, uids.c_str()
		, invitation_postscript.c_str()
		, invitation_attachment.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::KickAsync(const std::string& tid
	, const std::list<std::string>& ids
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || ids.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	std::string uids;
	StrListToJsonString(ids, uids);
	NIM_SDK_GET_FUNC(nim_super_team_kick_async)(tid.c_str()
		, uids.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::LeaveAsync(const std::string& tid, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_leave_async)(tid.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::UpdateSuperTeamInfoAsync(const std::string& tid
	, const SuperTeamInfo& team_info
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || team_info.GetSuperTeamID().empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_update_team_info_async)(tid.c_str()
		, team_info.ToJsonString().c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::UpdateMyPropertyAsync(const SuperTeamMemberProperty& prop, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (prop.GetSuperTeamID().empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_update_my_property_async)(prop.ToJsonString().c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::ApplyJoinAsync(const std::string& tid
	, const std::string& reason
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_apply_join_async)(tid.c_str()
		, reason.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::PassJoinApplyAsync(const std::string& tid
	, const std::string& applicant_id
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || applicant_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_pass_join_apply_async)(tid.c_str()
		, applicant_id.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::RejectJoinApplyAsync(const std::string& tid
	, const std::string& applicant_id
	, const std::string& reason
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || applicant_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_reject_join_apply_async)(tid.c_str()
		, applicant_id.c_str()
		, reason.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::AddManagersAsync(const std::string& tid
	, const std::list<std::string>& ids
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || ids.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	std::string uids;
	StrListToJsonString(ids, uids);
	NIM_SDK_GET_FUNC(nim_super_team_add_managers_async)(tid.c_str()
		, uids.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::RemoveManagersAsync(const std::string& tid
	, const std::list<std::string>& ids
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || ids.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	std::string uids;
	StrListToJsonString(ids, uids);
	NIM_SDK_GET_FUNC(nim_super_team_remove_managers_async)(tid.c_str()
		, uids.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::TransferTeamAsync(const std::string& tid
	, const std::string& new_owner_id
	, bool is_leave
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || new_owner_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_transfer_team_async)(tid.c_str()
		, new_owner_id.c_str()
		, is_leave
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::UpdateOtherNickAsync(const SuperTeamMemberProperty& prop, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (prop.GetSuperTeamID().empty() || prop.GetAccountID().empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_update_other_nick_async)(prop.ToJsonString().c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::AcceptInvitationAsync(const std::string& tid
	, const std::string& invitor_id
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || invitor_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_accept_invitation_async)(tid.c_str()
		, invitor_id.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::RejectInvitationAsync(const std::string& tid
	, const std::string& invitor_id
	, const std::string& reason
	, const SuperTeamEventCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || invitor_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_reject_invitation_async)(tid.c_str()
		, invitor_id.c_str()
		, reason.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::MuteMemberAsync(const std::string& tid, const std::string& member_id, bool set_mute, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || member_id.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_mute_member_async)(tid.c_str()
		, member_id.c_str()
		, set_mute
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::QueryMuteListAsync(const std::string& tid, const QuerySuperTeamMembersCallback& cb, const std::string& json_extension /*= ""*/) {
    if (tid.empty())
        return false;

    QuerySuperTeamMembersCallback* cb_pointer = nullptr;
    if (cb) {
        cb_pointer = new QuerySuperTeamMembersCallback(cb);
    }
    NIM_SDK_GET_FUNC(nim_super_team_query_mute_list_async)(tid.c_str(), json_extension.c_str(), &CallbackQueryMutedMemberList, cb_pointer);

    return true;
}

bool SuperTeam::MuteAsync(const std::string& tid, bool set_mute, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_mute_async)(tid.c_str()
		, set_mute
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

void SuperTeam::QueryAllMySuperTeamsAsync(const QueryAllMySuperTeamsCallback& cb, const std::string& json_extension/* = ""*/)
{
	QueryAllMySuperTeamsCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QueryAllMySuperTeamsCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_all_my_teams_async)(json_extension.c_str(), &CallbackQueryMySuperTeams, cb_pointer);
}

void SuperTeam::QueryAllMySuperTeamsInfoAsync(const QueryAllMySuperTeamsInfoCallback& cb, const std::string& json_extension/* = ""*/)
{
	QueryAllMySuperTeamsInfoCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QueryAllMySuperTeamsInfoCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_all_my_teams_info_async)(json_extension.c_str(), &CallbackQueryAllMySuperTeamsInfo, cb_pointer);
}

bool SuperTeam::QuerySuperTeamsInfoByKeywordAsync(const std::string& keyword, const QueryAllMySuperTeamsInfoCallback& cb, const std::string& json_extension /*= ""*/)
{
	if (keyword.empty())
		return false;

	QueryAllMySuperTeamsInfoCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QueryAllMySuperTeamsInfoCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_teams_info_by_keyword_async)(keyword.c_str(), json_extension.c_str(), &CallbackQueryAllMySuperTeamsInfo, cb_pointer);

	return true;
}

void SuperTeam::QueryMyAllMemberInfosAsync( const QueryMyAllMemberInfosCallback& cb, const std::string& json_extension /*= ""*/ )
{
	QueryMyAllMemberInfosCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QueryMyAllMemberInfosCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_my_all_member_infos_async)(json_extension.c_str(), &CallbackQueryMyAllMemberInfos, cb_pointer);
}

bool SuperTeam::QuerySuperTeamMembersAsync(const std::string& tid, const QuerySuperTeamMembersCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	QuerySuperTeamMembersCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QuerySuperTeamMembersCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_team_members_async)(tid.c_str(), true, json_extension.c_str(), &CallbackQuerySuperTeamMembers, cb_pointer);

	return true;
}

bool SuperTeam::QuerySuperTeamMemberAsync(const std::string& tid
	, const std::string& id
	, const QuerySuperTeamMemberCallback& cb
	, const std::string& json_extension/* = ""*/)
{
	if (tid.empty() || id.empty())
		return false;

	QuerySuperTeamMemberCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QuerySuperTeamMemberCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_team_member_async)(tid.c_str()
		, id.c_str()
		, json_extension.c_str()
		, &CallbackQuerySuperTeamMember
		, cb_pointer);

	return true;
}

SuperTeamMemberProperty SuperTeam::QuerySuperTeamMemberBlock(const std::string& tid, const std::string& id)
{
	const char *team_member_info = NIM_SDK_GET_FUNC(nim_super_team_query_team_member_block)(tid.c_str(), id.c_str());
	nim::SuperTeamMemberProperty prop;
	ParseSuperTeamMemberPropertyJson(PCharToString(team_member_info), prop);
	Global::FreeBuf((void *)team_member_info);
	return prop;
}

bool SuperTeam::QuerySuperTeamInfoAsync(const std::string& tid, const QuerySuperTeamInfoCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	QuerySuperTeamInfoCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new QuerySuperTeamInfoCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_team_info_async)(tid.c_str()
		, json_extension.c_str()
		, &CallbackQuerySuperTeamInfo
		, cb_pointer);

	return true;
}

SuperTeamInfo SuperTeam::QuerySuperTeamInfoBlock(const std::string& tid)
{
	const char *team_info = NIM_SDK_GET_FUNC(nim_super_team_query_team_info_block)(tid.c_str());
	nim::SuperTeamInfo tinfo;
	ParseSuperTeamInfoJson(PCharToString(team_info), tinfo);
	Global::FreeBuf((void *)team_info);
	return tinfo;
}

bool SuperTeam::QuerySuperTeamInfoOnlineAsync(const std::string& tid, const SuperTeamEventCallback& cb, const std::string& json_extension/* = ""*/)
{
	if (tid.empty())
		return false;

	SuperTeamEventCallback* cb_pointer = nullptr;
	if (cb)
	{
		cb_pointer = new SuperTeamEventCallback(cb);
	}
	NIM_SDK_GET_FUNC(nim_super_team_query_team_info_online_async)(tid.c_str()
		, json_extension.c_str()
		, &CallbackSuperTeamChange
		, cb_pointer);

	return true;
}

bool SuperTeam::ParseSuperTeamInfo(const std::string& json_team_info, SuperTeamInfo& team_info)
{
	return ParseSuperTeamInfoJson(json_team_info, team_info);
}

void SuperTeam::UnregSuperTeamCb()
{
	g_cb_super_team_event_ = nullptr;
}
}
