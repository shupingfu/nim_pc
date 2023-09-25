#include "nim_qchat_role_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_role_server_create)(const NIMQChatCreateServerRoleParam* param);
typedef void (*nim_qchat_role_server_update)(const NIMQChatUpdateServerRoleParam* param);
typedef void (*nim_qchat_role_server_batch_update_priorities)(const NIMQChatBatchUpdateServerRolePrioritiesParam* param);
typedef void (*nim_qchat_role_server_delete)(const NIMQChatDeleteServerRoleParam* param);
typedef void (*nim_qchat_role_server_get_list)(const NIMQChatGetServerRolesParam* param);
typedef void (*nim_qchat_role_server_add_members)(const NIMQChatAddMembersToServerRoleParam* param);
typedef void (*nim_qchat_role_server_remove_members)(const NIMQChatRemoveMembersFromServerRoleParam* param);
typedef void (*nim_qchat_role_server_get_members)(const NIMQChatGetMembersFromServerRoleParam* param);
typedef void (*nim_qchat_role_channel_add)(const NIMQChatAddChannelRoleParam* param);
typedef void (*nim_qchat_role_channel_update)(const NIMQChatUpdateChannelRoleParam* param);
typedef void (*nim_qchat_role_channel_remove)(const NIMQChatRemoveChannelRoleParam* param);
typedef void (*nim_qchat_role_channel_get_list)(const NIMQChatGetChannelRolesParam* param);
typedef void (*nim_qchat_role_member_add)(const NIMQChatAddMemberRoleParam* param);
typedef void (*nim_qchat_role_member_update)(const NIMQChatUpdateMemberRoleParam* param);
typedef void (*nim_qchat_role_member_remove)(const NIMQChatRemoveMemberRoleParam* param);
typedef void (*nim_qchat_role_member_get_list)(const NIMQChatGetMemberRolesParam* param);
typedef void (*nim_qchat_role_member_get_server_roles)(const NIMQChatGetServerRolesByMembersIdParam* param);
typedef void (*nim_qchat_role_member_get_roles_by_member_ids)(const NIMQChatGetServerRolesByMemberIdsParam* param);
typedef void (*nim_qchat_role_channel_get_existing_roles)(const NIMQChatGetExistingRolesInChannelParam* param);
typedef void (*nim_qchat_role_member_get_existing_roles)(const NIMQChatGetExistingAccidsOfMemberRolesParam* param);
typedef void (*nim_qchat_role_server_get_existing_members)(const NIMQChatGetExistingAccidsInServerRoleParam* param);
typedef void (*nim_qchat_role_check_permission)(const NIMQChatCheckPermissionParam* param);
typedef void (*nim_qchat_role_check_permissions)(const NIMQChatCheckPermissionsParam* param);
typedef void (*nim_qchat_role_add_channel_category_role)(const NIMQChatAddChannelCategoryRoleParam* param);
typedef void (*nim_qchat_role_remove_channel_category_role)(const NIMQChatRemoveChannelCategoryRoleParam* param);
typedef void (*nim_qchat_role_update_channel_category_role)(const NIMQChatUpdateChannelCategoryRoleParam* param);
typedef void (*nim_qchat_role_get_channel_category_roles_page)(const NIMQChatGetChannelCategoryRolesPageParam* param);
typedef void (*nim_qchat_role_add_channel_category_member_role)(const NIMQChatAddChannelCategoryMemberRoleParam* param);
typedef void (*nim_qchat_role_remove_channel_category_member_role)(const NIMQChatRemoveChannelCategoryMemberRoleParam* param);
typedef void (*nim_qchat_role_update_channel_category_member_role)(const NIMQChatUpdateChannelCategoryMemberRoleParam* param);
typedef void (*nim_qchat_role_get_channel_category_member_roles_page)(const NIMQChatGetChannelCategoryMemberRolesPageParam* param);
#else
#include "nim_qchat_role.h"
#endif

void Role::CreateServerRole(const QChatCreateServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_create)(&c_param);
}

void Role::UpdateServerRole(const QChatUpdateServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_update)(&c_param);
}

void Role::UpdateServerRolePriorities(const QChatUpdateServerRolePrioritiesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_batch_update_priorities)(&c_param);
}

void Role::DeleteServerRole(const QChatDeleteServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_delete)(&c_param);
}

void Role::GetServerRoles(const QChatGetServerRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_get_list)(&c_param);
}

void Role::AddMembersToServerRole(const QChatAddMembersToServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_add_members)(&c_param);
}

void Role::RemoveMembersFromServerRole(const QChatRemoveMembersFromServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_remove_members)(&c_param);
}

void Role::GetMembersFromServerRole(const QChatGetMembersFromServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_get_members)(&c_param);
}

void Role::AddChannelRole(const QChatAddChannelRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_channel_add)(&c_param);
}

void Role::UpdateChannelRole(const QChatUpdateChannelRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_channel_update)(&c_param);
}

void Role::RemoveChannelRole(const QChatRemoveChannelRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_channel_remove)(&c_param);
}

void Role::GetChannelRoles(const QChatGetChannelRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_channel_get_list)(&c_param);
}

void Role::AddMemberRole(const QChatAddMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_add)(&c_param);
}

void Role::UpdateMemberRole(const QChatUpdateMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_update)(&c_param);
}

void Role::RemoveMemberRole(const QChatRemoveMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_remove)(&c_param);
}

void Role::GetMemberRoles(const QChatGetMemberRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_get_list)(&c_param);
}

void Role::GetRolesByAccid(const QChatGetRolesByAccidParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_get_server_roles)(&c_param);
}

void Role::GetExistingServerRolesByAccids(const QChatGetExistingServerRolesByAccidsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_get_roles_by_member_ids)(&c_param);
}

void Role::GetExistingChannelRolesByServerRoleIds(const QChatGetExistingChannelRolesByServerRoleIdsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_channel_get_existing_roles)(&c_param);
}

void Role::GetExistingAccidsOfMemberRoles(const QChatGetExistingAccidsOfMemberRolesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_member_get_existing_roles)(&c_param);
}

void Role::GetExistingAccidsInServerRole(const QChatGetExistingAccidsInServerRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_server_get_existing_members)(&c_param);
}

void Role::CheckPermission(const QChatCheckPermissionParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_check_permission)(&c_param);
}

void Role::CheckPermissions(const QChatCheckPermissionsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_check_permissions)(&c_param);
}

void Role::AddChannelCategoryRole(const QChatAddChannelCategoryRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_add_channel_category_role)(&c_param);
}

void Role::RemoveChannelCategoryRole(const QChatRemoveChannelCategoryRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_remove_channel_category_role)(&c_param);
}

void Role::UpdateChannelCategoryRole(const QChatUpdateChannelCategoryRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_update_channel_category_role)(&c_param);
}

void Role::GetChannelCategoryRolesPage(const QChatGetChannelCategoryRolesPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_get_channel_category_roles_page)(&c_param);
}

void Role::AddChannelCategoryMemberRole(const QChatAddChannelCategoryMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_add_channel_category_member_role)(&c_param);
}

void Role::RemoveChannelCategoryMemberRole(const QChatRemoveChannelCategoryMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_remove_channel_category_member_role)(&c_param);
}

void Role::UpdateChannelCategoryMemberRole(const QChatUpdateChannelCategoryMemberRoleParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_update_channel_category_member_role)(&c_param);
}

void Role::GetChannelCategoryMemberRolesPage(const QChatGetChannelCategoryMemberRolesPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_role_get_channel_category_member_roles_page)(&c_param);
}

}  // namespace nim
