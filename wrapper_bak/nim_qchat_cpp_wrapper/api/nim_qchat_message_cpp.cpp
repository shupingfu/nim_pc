/**
 * @file nim_qchat_Message_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_message_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_reg_recv_msg_cb)(const NIMQChatRegRecvMsgCbParam* param);
typedef void (*nim_qchat_reg_msg_updated_cb)(const NIMQChatRegMsgUpdatedCbParam* param);
typedef void (*nim_qchat_send_msg)(const NIMQChatSendMessageParam* param);
typedef void (*nim_qchat_update_msg)(const NIMQChatUpdateMessageParam* param);
typedef void (*nim_qchat_revoke_msg)(const NIMQChatRevokeMessageParam* param);
typedef void (*nim_qchat_delete_msg)(const NIMQChatDeleteMessageParam* param);
typedef void (*nim_qchat_mark_message_read)(const NIMQChatMarkMessageReadParam* param);
typedef void (*nim_qchat_query_history_msg)(const NIMQChatGetMessagesParam* param);
typedef void (*nim_qchat_get_last_messages)(const NIMQChatGetLastMessagesParam* param);
typedef void (*nim_qchat_query_history_msg_cache)(const NIMQChatGetMessagesCacheParam* param);
typedef void (*nim_qchat_reply_message)(const NIMQChatReplyMessageParam* param);
typedef void (*nim_qchat_get_message_history_by_ids)(const NIMQChatGetMessageHistoryByIdsParam* param);
typedef void (*nim_qchat_get_refer_messages)(const NIMQChatGetReferMessagesParam* param);
typedef void (*nim_qchat_get_mentioned_me_messages)(const NIMQChatGetMentionedMeMessagesParam* param);
typedef void (*nim_qchat_are_mentioned_me_messages)(const NIMQChatAreMentionedMeMessagesParam* param);
typedef void (*nim_qchat_get_thread_messages)(const NIMQChatGetThreadMessagesParam* param);
typedef void (*nim_qchat_get_thread_root_messages_meta)(const NIMQChatGetThreadRootMessagesMetaParam* param);
typedef void (*nim_qchat_add_quick_comment)(const NIMQChatAddQuickCommentParam* param);
typedef void (*nim_qchat_remove_quick_comment)(const NIMQChatRemoveQuickCommentParam* param);
typedef void (*nim_qchat_get_quick_comments)(const NIMQChatGetQuickCommentsParam* param);
typedef void (*nim_qchat_search_msg_by_page)(const NIMQChatMessageSearchPageParam* param);
#else
#include "nim_qchat_message.h"
#endif

void Message::RegRecvCb(const QChatRegRecvMsgCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_recv_msg_cb)(&c_param);
}

void Message::RegUpdatedCb(const QChatRegMsgUpdatedCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_msg_updated_cb)(&c_param);
}

void Message::Send(const QChatSendMessageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_send_msg)(&c_param);
}

void Message::Update(const QChatUpdateMessageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_update_msg)(&c_param);
}

void Message::Revoke(const QChatRevokeMessageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_revoke_msg)(&c_param);
}

void Message::Delete(const QChatDeleteMessageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_delete_msg)(&c_param);
}

void Message::MarkRead(const QChatMarkMessageReadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_mark_message_read)(&c_param);
}

void Message::GetMessages(const QChatGetMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_query_history_msg)(&c_param);
}

void Message::GetMessagesCache(const QChatGetMessagesCacheParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_query_history_msg_cache)(&c_param);
}

void Message::GetLastMessages(const QChatGetLastMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_last_messages)(&c_param);
}

void Message::Reply(const QChatReplyMessageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reply_message)(&c_param);
}

void Message::GetMessageHistoryByIds(const QChatGetMessageHistoryByIdsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_message_history_by_ids)(&c_param);
}

void Message::GetReferMessages(const QChatGetReferMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_refer_messages)(&c_param);
}

void Message::GetMentionedMeMessages(const QChatGetMentionedMeMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_mentioned_me_messages)(&c_param);
}

void Message::AreMentionedMeMessages(const QChatAreMentionedMeMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_are_mentioned_me_messages)(&c_param);
}

void Message::GetThreadMessages(const QChatGetThreadMessagesParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_thread_messages)(&c_param);
}

void Message::GetThreadRootMessagesMeta(const QChatGetThreadRootMessagesMetaParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_thread_root_messages_meta)(&c_param);
}

void Message::AddQuickComment(const QChatAddQuickCommentParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_add_quick_comment)(&c_param);
}

void Message::RemoveQuickComment(const QChatRemoveQuickCommentParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_remove_quick_comment)(&c_param);
}

void Message::GetQuickComments(const QChatGetQuickCommentsParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_get_quick_comments)(&c_param);
}

void Message::SearchMsgByPage(const QChatMessageSearchPageParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_search_msg_by_page)(&c_param);
}

}  // namespace nim
