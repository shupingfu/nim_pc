/**
 * @file nim_qchat_server_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_server_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_server_reg_unread_cb)(const NIMQChatServerRegUnreadCbParam* param);
typedef void (*nim_qchat_server_create)(const NIMQChatServerCreateParam* param);
typedef void (*nim_qchat_server_remove)(const NIMQChatServerDeleteParam* param);
typedef void (*nim_qchat_server_update)(const NIMQChatServerUpdateParam* param);
typedef void (*nim_qchat_server_subscribe)(const NIMQChatServerSubscribeParam* param);
typedef void (*nim_qchat_server_enter_as_visitor)(const NIMQChatServerEnterAsVisitorParam* param);
typedef void (*nim_qchat_server_leave_as_visitor)(const NIMQChatServerLeaveAsVisitorParam* param);
typedef void (*nim_qchat_server_subscribe)(const NIMQChatServerSubscribeParam* param);
typedef void (*nim_qchat_server_subscribe_as_visitor)(const NIMQChatServerSubscribeAsVisitorParam* param);
typedef void (*nim_qchat_server_get_servers)(const NIMQChatServerGetServersParam* param);
typedef void (*nim_qchat_server_get_servers_page)(const NIMQChatServerGetServersPageParam* param);
typedef void (*nim_qchat_server_invite)(const NIMQChatServerInviteParam* param);
typedef void (*nim_qchat_server_accept_invite)(const NIMQChatServerAcceptInviteParam* param);
typedef void (*nim_qchat_server_reject_invite)(const NIMQChatServerRejectInviteParam* param);
typedef void (*nim_qchat_server_apply)(const NIMQChatServerApplyParam* param);
typedef void (*nim_qchat_server_accept_apply)(const NIMQChatServerAcceptApplyParam* param);
typedef void (*nim_qchat_server_reject_apply)(const NIMQChatServerRejectApplyParam* param);
typedef void (*nim_qchat_server_kick)(const NIMQChatServerKickParam* param);
typedef void (*nim_qchat_server_leave)(const NIMQChatServerLeaveParam* param);
typedef void (*nim_qchat_server_update_member_info)(const NIMQChatServerUpdateMemberInfoParam* param);
typedef void (*nim_qchat_server_get_members)(const NIMQChatServerGetMembersParam* param);
typedef void (*nim_qchat_server_get_members_page)(const NIMQChatServerGetMembersPageParam* param);
typedef void (*nim_qchat_server_ban_member)(const NIMQChatServerBanMemberParam* param);
typedef void (*nim_qchat_server_unban_member)(const NIMQChatServerUnbanMemberParam* param);
typedef void (*nim_qchat_server_get_banned_members_page)(const NIMQChatServerGetBannedMembersPageParam* param);
typedef void (*nim_qchat_server_search_page)(const NIMQChatServerSearchPageParam* param);
typedef void (*nim_qchat_server_member_search)(const NIMQChatServerMemberSearchParam* param);
typedef void (*nim_qchat_server_generate_invite_code)(const NIMQChatServerGenerateInviteCodeParam* param);
typedef void (*nim_qchat_server_join_by_invite_code)(const NIMQChatServerJoinByInviteCodeParam* param);
typedef void (*nim_qchat_server_get_invite_apply_record_of_server)(const NIMQChatServerGetInviteApplyRecordOfServerParam* param);
typedef void (*nim_qchat_server_get_invite_apply_record_by_self)(const NIMQChatServerGetInviteApplyRecordOfSelfParam* param);
typedef void (*nim_qchat_server_subscribe_all_channel)(const NIMQChatServerSubscribeAllChannelParam* param);
typedef void (*nim_qchat_server_mark_read)(const NIMQChatServerMarkReadParam* param);
#else
#include "nim_qchat_server.h"
#endif

void Server::RegUnreadCb(const QChatServerRegUnreadCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_reg_unread_cb)(&c_param);
}

void Server::CreateServer(const QChatServerCreateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_create)(&c_param);
}

void Server::DeleteServer(const QChatServerDeleteParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_remove)(&c_param);
}

void Server::UpdateServer(const QChatServerUpdateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_update)(&c_param);
}

void Server::EnterAsVisitor(const QChatServerEnterAsVisitorParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_enter_as_visitor)(&c_param);
}

void Server::LeaveAsVisitor(const QChatServerLeaveAsVisitorParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_leave_as_visitor)(&c_param);
}

void Server::Subscribe(const QChatServerSubscribeParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_subscribe)(&c_param);
}

void Server::SubscribeAllChannel(const QChatServerSubscribeAllChannelParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_subscribe_all_channel)(&c_param);
}

void Server::SubscribeAsVisitor(const QChatServerSubscribeAsVisitorParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_subscribe_as_visitor)(&c_param);
}

void Server::MarkRead(const QChatServerMarkReadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_mark_read)(&c_param);
}

void Server::GetServers(const QChatServerGetServersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_servers)(&c_param);
}

void Server::GetServersByPage(const QChatServerGetServersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_servers_page)(&c_param);
}

void Server::Invite(const QChatServerInviteParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_invite)(&c_param);
}

void Server::AcceptInvite(const QChatServerAcceptInviteParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_accept_invite)(&c_param);
}

void Server::RejectInvite(const QChatServerRejectInviteParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_reject_invite)(&c_param);
}

void Server::Apply(const QChatServerApplyParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_apply)(&c_param);
}

void Server::AcceptApply(const QChatServerAcceptApplyParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_accept_apply)(&c_param);
}

void Server::RejectApply(const QChatServerRejectApplyParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_reject_apply)(&c_param);
}

void Server::Kick(const QChatServerKickParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_kick)(&c_param);
}

void Server::Leave(const QChatServerLeaveParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_leave)(&c_param);
}

void Server::UpdateMemberInfo(const QChatServerUpdateMemberInfoParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_update_member_info)(&c_param);
}

void Server::GetServerMembers(const QChatServerGetMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_members)(&c_param);
}

void Server::GetServerMembersByPage(const QChatServerGetMembersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_members_page)(&c_param);
}

void Server::BanMember(const QChatServerBanMemberParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_ban_member)(&c_param);
}

void Server::UnbanMember(const QChatServerUnbanMemberParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_unban_member)(&c_param);
}

void Server::GetBannedMembersByPage(const QChatServerGetBannedMembersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_banned_members_page)(&c_param);
}

void Server::ServerSearchByPage(const QChatServerSearchPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_search_page)(&c_param);
}

void Server::ServerMemberSearch(const QChatServerMemberSearchParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_member_search)(&c_param);
}

void Server::GenerateInviteCode(const QChatServerGenerateInviteCodeParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_generate_invite_code)(&c_param);
}

void Server::JoinByInviteCode(const QChatServerJoinByInviteCodeParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_join_by_invite_code)(&c_param);
}

void Server::GetInviteApplyRecordOfServer(const QChatServerGetInviteApplyRecordOfServerParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_invite_apply_record_of_server)(&c_param);
}

void Server::GetInviteApplyRecordOfSelf(const QChatServerGetInviteApplyRecordOfSelfParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_server_get_invite_apply_record_by_self)(&c_param);
}

}  // namespace nim
