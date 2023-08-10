/** @file nim_cpp_plugin_in.cpp
 * @brief NIM SDK 提供的plugin接入接口
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @date 2015/12/29
 */

#include "nim_cpp_wrapper/api/nim_cpp_plugin_in.h"
#include "nim_cpp_wrapper/helper/nim_sdk_loader_helper.h"
#include "nim_wrapper_util/nim_json_util.h"

namespace nim {

#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_plugin_chatroom_request_enter_async)(const int64_t room_id,
    const char* json_extension,
    nim_plugin_chatroom_request_enter_cb_func cb,
    const void* user_data);
typedef void (*nim_plugin_qchat_request_link_address_async)(uint32_t ip_version,
    const char* json_extension,
    nim_plugin_qchat_request_link_address_cb_func cb,
    const void* user_data);
#else
#include "nim_plugin_in.h"
#endif

static void CallbackRequestChatRoomEnter(int error_code, const char* result, const char* json_extension, const void* user_data) {
    CallbackProxy::DoSafeCallback<PluginIn::ChatRoomRequestEnterCallback>(user_data, [=](const PluginIn::ChatRoomRequestEnterCallback& cb) {
        CallbackProxy::Invoke(cb, error_code, PCharToString(result));
    });
}

static void CallbackRequestQChatLinkAddress(int error_code, const char* link_address_json_array, const void* user_data) {
    std::list<std::string> link_address_list;
    if (link_address_json_array) {
        nim::JsonArrayStringToList(link_address_json_array, link_address_list);
    }
    CallbackProxy::DoSafeCallback<PluginIn::QChatRequestLinkAddressCallback>(user_data, [=](const PluginIn::QChatRequestLinkAddressCallback& cb) {
        CallbackProxy::Invoke(cb, error_code, link_address_list);
    });
}

void PluginIn::ChatRoomRequestEnterAsync(const int64_t room_id,
    const ChatRoomRequestEnterCallback& callback,
    const std::string& json_extension /* = ""*/) {
    ChatRoomRequestEnterCallback* cb_pointer = nullptr;
    if (callback)
        cb_pointer = new ChatRoomRequestEnterCallback(callback);

    NIM_SDK_GET_FUNC(nim_plugin_chatroom_request_enter_async)(room_id, json_extension.c_str(), &CallbackRequestChatRoomEnter, cb_pointer);
}

void PluginIn::QChatRequestLinkAddress(uint32_t ip_version,
    const QChatRequestLinkAddressCallback& callback,
    const std::string& json_extension /* = ""*/) {
    QChatRequestLinkAddressCallback* cb_pointer = nullptr;
    if (callback)
        cb_pointer = new QChatRequestLinkAddressCallback(callback);

    NIM_SDK_GET_FUNC(nim_plugin_qchat_request_link_address_async)(ip_version, json_extension.c_str(), &CallbackRequestQChatLinkAddress, cb_pointer);
}

}  // namespace nim
