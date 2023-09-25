/**
 * @file nim_qchat_instance_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-15
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {

// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_reg_custom_token_cb)(const NIMQChatRegCustomTokenCbParam* param);
typedef void (*nim_qchat_reg_kicked_out_cb)(const NIMQChatRegKickedCbParam* param);
typedef void (*nim_qchat_reg_login_status_cb)(const NIMQChatRegLoginStatusCbParam* param);
typedef void (*nim_qchat_reg_multispot_login_cb)(const NIMQChatRegMultispotLoginCbParam* param);
typedef void (*nim_qchat_reg_sdk_log_cb)(const NIMQChatRegSDKLogCbParam* param);
typedef void (*nim_qchat_init)(const NIMQChatInitParam* param);
typedef void (*nim_qchat_cleanup)(const NIMQChatCleanupParam* param);
typedef void (*nim_qchat_login)(const NIMQChatLoginParam* param);
typedef void (*nim_qchat_logout)(const NIMQChatLogoutParam* param);
typedef void (*nim_qchat_kick_other_clients)(const NIMQChatKickParam* param);
#else
#include "nim_qchat.h"
#endif

void QChat::RegCustomTokenCb(const QChatRegCustomTokenCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_custom_token_cb)(&c_param);
}

void QChat::RegKickedOutCb(const QChatRegKickedCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_kicked_out_cb)(&c_param);
}

void QChat::RegLoginStatusCb(const QChatRegLoginStatusCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_login_status_cb)(&c_param);
}

void QChat::RegMultispotLoginCb(const QChatRegMultispotLoginCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_multispot_login_cb)(&c_param);
}

static SDKLogCallback g_sdk_log_cb = nullptr;
void QChat::RegSDKLogCb(const SDKLogCallback& callback) {
    g_sdk_log_cb = callback;
    NIMQChatRegSDKLogCbParam param;
    param.cb = [](const NIMQChatSDKLogResp* resp) {
        nim::CallbackProxy::DoSafeCallback<SDKLogCallback>(resp->user_data, [c_resp = *resp](const SDKLogCallback& cb) {
            nim::CallbackProxy::Invoke(cb, c_resp);
        });
    };
    param.user_data = &g_sdk_log_cb;
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_reg_sdk_log_cb)(&param);
}

bool QChat::Init(const QChatInitParam& param) {
#ifdef NIM_SDK_DLL_IMPORT

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // define something for Windows (32-bit and 64-bit, this part is common)
    static const char* kSdkNimDll = "nim_qchat.dll";
#elif __APPLE__
    static const char* kSdkNimDll = "libnim_qchat.dylib";
#else
    static const char* kSdkNimDll = "libnim_qchat.so";
#endif

    if (NULL == nim_qchat_sdk_instance) {
        nim_qchat_sdk_instance = new nim::SDKInstance();
    }
    if (!nim_qchat_sdk_instance->LoadSdkDll("", kSdkNimDll))
        return false;

#endif
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_init)(&c_param);
#ifdef NIM_SDK_DLL_IMPORT
    nim_qchat_sdk_instance->OnSDKInited();
#endif
    return true;
}

bool QChat::Cleanup(const QChatCleanupParam& param) {
#ifdef NIM_SDK_DLL_IMPORT
    if (nim_qchat_sdk_instance == nullptr) {
        return false;
    }
#endif
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_cleanup)(&c_param);
#ifdef NIM_SDK_DLL_IMPORT
    nim_qchat_sdk_instance->UnLoadSdkDll();
    delete nim_qchat_sdk_instance;
    nim_qchat_sdk_instance = nullptr;
#endif
    return true;
}

void QChat::Login(const QChatLoginParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_login)(&c_param);
}

void QChat::Logout(const QChatLogoutParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_logout)(&c_param);
}

void QChat::KickOtherClients(const QChatKickParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_kick_other_clients)(&c_param);
}

}  // namespace nim
