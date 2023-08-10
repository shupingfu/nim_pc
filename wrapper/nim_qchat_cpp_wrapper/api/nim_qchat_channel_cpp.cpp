/**
 * @file nim_qchat_channel_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_channel_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_channel_reg_unread_cb)(const NIMQChatChannelRegUnreadCbParam* param);
typedef void (*nim_qchat_channel_create)(const NIMQChatChannelCreateParam* param);
typedef void (*nim_qchat_channel_remove)(const NIMQChatChannelDeleteParam* param);
typedef void (*nim_qchat_channel_update)(const NIMQChatChannelUpdateParam* param);
typedef void (*nim_qchat_channel_update_category_info)(const NIMQChatChannelUpdateCategoryInfoParam* param);
typedef void (*nim_qchat_channel_get_channels)(const NIMQChatChannelGetChannelsParam* param);
typedef void (*nim_qchat_channel_get_channels_page)(const NIMQChatChannelGetChannelsPageParam* param);
typedef void (*nim_qchat_channel_get_members_page)(const NIMQChatChannelGetMembersPageParam* param);
typedef void (*nim_qchat_channel_subscribe)(const NIMQChatChannelSubscribeParam* param);
typedef void (*nim_qchat_channel_subscribe_as_visitor)(const NIMQChatChannelSubscribeAsVisitorParam* param);
typedef void (*nim_qchat_channel_query_unread_info)(const NIMQChatChannelQueryUnreadInfoParam* param);
typedef void (*nim_qchat_channel_update_white_black_role)(const NIMQChatChannelUpdateWhiteBlackRoleParam* param);
typedef void (*nim_qchat_channel_update_white_black_members)(const NIMQChatChannelUpdateWhiteBlackMembersParam* param);
typedef void (*nim_qchat_channel_get_white_black_roles_page)(const NIMQChatChannelGetWhiteBlackRolesPageParam* param);
typedef void (*nim_qchat_channel_get_white_black_members_page)(const NIMQChatChannelGetWhiteBlackMembersPageParam* param);
typedef void (*nim_qchat_channel_get_existing_white_black_roles)(const NIMQChatChannelGetExistingWhiteBlackRolesParam* param);
typedef void (*nim_qchat_channel_get_existing_white_black_members)(const NIMQChatChannelGetExistingWhiteBlackMembersParam* param);
typedef void (*nim_qchat_channel_search_page)(const NIMQChatChannelSearchPageParam* param);
typedef void (*nim_qchat_channel_member_search_page)(const NIMQChatChannelMemberSearchParam* param);
typedef void (*nim_qchat_channel_update_rtc_info)(const NIMQChatChannelUpdateRTCInfoParam* param);
typedef void (*nim_qchat_channel_get_rtc_info)(const NIMQChatChannelGetRTCInfoParam* param);
typedef void (*nim_qchat_channel_get_rtc_online_members)(const NIMQChatChannelGetRTCOnlineMembersParam* param);
#else
#include "nim_qchat_channel.h"
#endif

void Channel::RegUnreadCb(const QChatChannelRegUnreadCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_reg_unread_cb)(&c_param);
}

void Channel::CreateChannel(const QChatChannelCreateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_create)(&c_param);
}

void Channel::DeleteChannel(const QChatChannelDeleteParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_remove)(&c_param);
}

void Channel::UpdateChannel(const QChatChannelUpdateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_update)(&c_param);
}

void Channel::UpdateCategoryInfo(const QChatChannelUpdateCategoryInfoParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_update_category_info)(&c_param);
}

void Channel::Subscribe(const QChatChannelSubscribeParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_subscribe)(&c_param);
}

void Channel::SubscribeAsVisitor(const QChatChannelSubscribeAsVisitorParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_subscribe_as_visitor)(&c_param);
}

void Channel::QueryUnreadInfo(const QChatChannelQueryUnreadInfoParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_query_unread_info)(&c_param);
}

void Channel::GetChannels(const QChatChannelGetChannelsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_channels)(&c_param);
}

void Channel::GetChannelsByPage(const QChatChannelGetChannelsPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_channels_page)(&c_param);
}

void Channel::GetMembersByPage(const QChatChannelGetMembersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_members_page)(&c_param);
}

void Channel::UpdateWhiteBlackRole(const QChatChannelUpdateWhiteBlackRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_update_white_black_role)(&c_param);
}

void Channel::UpdateWhiteBlackMembers(const QChatChannelUpdateWhiteBlackMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_update_white_black_members)(&c_param);
}

void Channel::GetWhiteBlackRolesPage(const QChatChannelGetWhiteBlackRolesPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_white_black_roles_page)(&c_param);
}

void Channel::GetWhiteBlackMembersPage(const QChatChannelGetWhiteBlackMembersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_white_black_members_page)(&c_param);
}

void Channel::GetExistingWhiteBlackRoles(const QChatChannelGetExistingWhiteBlackRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_existing_white_black_roles)(&c_param);
}

void Channel::GetExistingWhiteBlackMembers(const QChatChannelGetExistingWhiteBlackMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_existing_white_black_members)(&c_param);
}

void Channel::ChannelSearchByPage(const QChatChannelSearchPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_search_page)(&c_param);
}

void Channel::ChannelMemberSearch(const QChatChannelMemberSearchParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_member_search_page)(&c_param);
}

void Channel::UpdateRTCInfo(const QChatChannelUpdateRTCInfoParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_update_rtc_info)(&c_param);
}

void Channel::GetRTCInfo(const QChatChannelGetRTCInfoParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_rtc_info)(&c_param);
}

void Channel::GetRTCOnlineMembers(const QChatChannelGetRTCOnlineMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_rtc_online_members)(&c_param);
}

}  // namespace nim
