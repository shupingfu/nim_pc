/**
 * @file nim_qchat_notification_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_system_notification_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_reg_recv_system_notification_cb)(const NIMQChatRegRecvSystemNotificationCbParam* param);
typedef void (*nim_qchat_reg_recv_typing_event_cb)(const NIMQChatRegRecvTypingEventCbParam* param);
typedef void (*nim_qchat_reg_system_notification_updated_cb)(const NIMQChatRegSystemNotificationUpdatedCbParam* param);
typedef void (*nim_qchat_send_system_notification)(const NIMQChatSendSystemNotificationParam* param);
typedef void (*nim_qchat_update_system_notification)(const NIMQChatUpdateSystemNotificationParam* param);
typedef void (*nim_qchat_mark_system_notifications_read)(const NIMQChatMarkSystemNotificationsReadParam* param);
typedef void (*nim_qchat_send_typing_event)(const NIMQChatSendTypingEventParam* param);
#else
#include "nim_qchat_system_notification.h"
#endif

void SystemNotification::RegRecvCb(const QChatRegRecvSystemNotificationCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_recv_system_notification_cb)(&c_param);
}

void SystemNotification::RegRecvTypingEvent(const QChatRegRecvTypingEventCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_recv_typing_event_cb)(&c_param);
}

void SystemNotification::RegUpdatedCb(const QChatRegSystemNotificationUpdatedCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_system_notification_updated_cb)(&c_param);
}

void SystemNotification::Send(const QChatSendSystemNotificationParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_send_system_notification)(&c_param);
}

void SystemNotification::Update(const QChatUpdateSystemNotificationParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_update_system_notification)(&c_param);
}

void SystemNotification::MarkSystemNotificationsRead(const QChatMarkSystemNotificationsReadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_mark_system_notifications_read)(&c_param);
}

void SystemNotification::SendTypingEvent(const QChatSendTypingEventParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_send_typing_event)(&c_param);
}

}  // namespace nim
