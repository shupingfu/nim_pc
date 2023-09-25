/**
 * @file nim_qchat_channel_category_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_channel_category_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_channel_category_create)(const NIMQChatChannelCategoryCreateParam* param);
typedef void (*nim_qchat_channel_category_remove)(const NIMQChatChannelCategoryRemoveParam* param);
typedef void (*nim_qchat_channel_category_update)(const NIMQChatChannelCategoryUpdateParam* param);
typedef void (*nim_qchat_channel_get_categories_by_id)(const NIMQChatChannelGetCategoriesByIDParam* param);
typedef void (*nim_qchat_channel_get_categories_page)(const NIMQChatChannelGetCategoriesPageParam* param);
typedef void (*nim_qchat_channel_get_category_channels_page)(const NIMQChatChannelGetCategoryChannelsPageParam* param);
typedef void (*nim_qchat_channel_category_update_white_black_role)(const NIMQChatChannelCategoryUpdateWhiteBlackRoleParam* param);
typedef void (*nim_qchat_channel_category_get_white_black_roles_by_id)(const NIMQChatChannelCategoryGetExistingWhiteBlackRolesParam* param);
typedef void (*nim_qchat_channel_category_get_white_black_roles_page)(const NIMQChatChannelCategoryGetWhiteBlackRolesPageParam* param);
typedef void (*nim_qchat_channel_category_update_white_black_members)(const NIMQChatChannelCategoryUpdateWhiteBlackMembersParam* param);
typedef void (*nim_qchat_channel_category_get_white_black_members_by_id)(const NIMQChatChannelCategoryGetExistingWhiteBlackMembersParam* param);
typedef void (*nim_qchat_channel_category_get_white_black_members_page)(const NIMQChatChannelCategoryGetWhiteBlackMembersPageParam* param);
#else
#include "nim_qchat_channel_category.h"
#endif

void ChannelCategory::CreateChannelCategory(const QChatChannelCategoryCreateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_create)(&c_param);
}

void ChannelCategory::UpdateChannelCategory(const QChatChannelCategoryUpdateParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_update)(&c_param);
}

void ChannelCategory::RemoveChannelCategory(const QChatChannelCategoryRemoveParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_remove)(&c_param);
}

void ChannelCategory::GetChannelCategoriesByID(const QChatChannelGetCategoriesByIDParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_categories_by_id)(&c_param);
}

void ChannelCategory::GetChannelCategoriesPage(const QChatChannelGetCategoriesPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_categories_page)(&c_param);
}

void ChannelCategory::GetChannelCategoryChannelsPage(const QChatChannelGetCategoryChannelsPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_get_category_channels_page)(&c_param);
}

void ChannelCategory::UpdateChannelCategoryWhiteBlackRole(const QChatChannelCategoryUpdateWhiteBlackRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_update_white_black_role)(&c_param);
}

void ChannelCategory::GetExistingChannelCategoryWhiteBlackRoles(const QChatChannelCategoryGetExistingWhiteBlackRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_get_white_black_roles_by_id)(&c_param);
}

void ChannelCategory::GetChannelCategoryWhiteBlackRolesPage(const QChatChannelCategoryGetWhiteBlackRolesPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_get_white_black_roles_page)(&c_param);
}

void ChannelCategory::UpdateChannelCategoryWhiteBlackMembers(const QChatChannelCategoryUpdateWhiteBlackMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_update_white_black_members)(&c_param);
}

void ChannelCategory::GetExistingChannelCategoryWhiteBlackMembers(const QChatChannelCategoryGetExistingWhiteBlackMembersParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_get_white_black_members_by_id)(&c_param);
}

void ChannelCategory::GetChannelCategoryWhiteBlackMembersPage(const QChatChannelCategoryGetWhiteBlackMembersPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_channel_category_get_white_black_members_page)(&c_param);
}

}  // namespace nim
