/**
 * @file nim_qchat_client_cpp_def.h
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright (c) 2022, NetEase Inc. All rights reserved
 *
 */
#ifndef __NIM_QCHAT_CLIENT_CPP_DEF_H__
#define __NIM_QCHAT_CLIENT_CPP_DEF_H__

#include "nim_qchat_public_cpp_def.h"

namespace nim {
/** @struct QChatCustomTokenResp 获取自定义token */
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatCustomTokenResp {
    /// appkey
    std::string appkey;
    /// accid
    std::string accid;
    /// 自定义token
    std::string token;
    QChatCustomTokenResp() = default;
    QChatCustomTokenResp(const NIMQChatCustomTokenResp& c_resp) {
        appkey = c_resp.appkey;
        accid = c_resp.accid;
        token = "";
    }
    NIMQChatCustomTokenResp ToCParam() const {
        NIMQChatCustomTokenResp c_resp;
        c_resp.appkey = const_cast<char*>(appkey.c_str());
        c_resp.accid = const_cast<char*>(accid.c_str());
        c_resp.token = const_cast<char*>(token.c_str());
        return c_resp;
    }
};

/** @struct QChatLoginClientInfo 多端登录信息 */
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLoginClientInfo {
    /// 终端类型
    NIMQChatClientType client_type{kQChatClientTypeUnknown};
    /// 设备 ID
    std::string device_id{""};
    /// 客户端 IP
    std::string client_ip{""};
    /// 客户端端口
    uint16_t client_port{0};
    /// 会话 ID
    std::string consid{""};
    /// 时间戳
    uint64_t timestamp{0};
    /// 自定义终端类型
    uint32_t custom_client_type{0};
    /// 自定义标签
    std::string custom_tag{""};
    /// 操作系统名称
    std::string os_name{""};
    QChatLoginClientInfo() = default;
    QChatLoginClientInfo(const NIMQChatLoginClientInfo& c_param) {
        client_type = c_param.client_type;
        device_id = c_param.device_id;
        client_ip = c_param.client_ip ? c_param.client_ip : "";
        client_port = c_param.client_port;
        consid = c_param.consid_ ? c_param.consid_ : "";
        timestamp = c_param.timestamp;
        custom_client_type = c_param.custom_client_type_;
        custom_tag = c_param.custom_tag_ ? c_param.custom_tag_ : "";
        os_name = c_param.os_name_ ? c_param.os_name_ : "";
    }
    NIMQChatLoginClientInfo ToCParam() const {
        NIMQChatLoginClientInfo c_param = {};
        c_param.client_type = client_type;
        c_param.device_id = const_cast<char*>(device_id.c_str());
        c_param.client_ip = const_cast<char*>(client_ip.c_str());
        c_param.client_port = client_port;
        c_param.consid_ = const_cast<char*>(consid.c_str());
        c_param.timestamp = timestamp;
        c_param.custom_client_type_ = custom_client_type;
        c_param.custom_tag_ = const_cast<char*>(custom_tag.c_str());
        c_param.os_name_ = const_cast<char*>(os_name.c_str());
        return c_param;
    }
};

/// @struct QChatKickedResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatKickedResp {
    /// 客户端类型
    uint32_t client_type{0};
    /// 被踢原因
    uint32_t kick_reason{0};
    /// 扩展字段
    std::string ext{""};
    /// 自定义客户端类型
    uint32_t custom_client_type{0};
    QChatKickedResp() = default;
    QChatKickedResp(const NIMQChatKickedResp& c_resp) {
        client_type = c_resp.client_type;
        kick_reason = c_resp.kick_reason;
        ext = c_resp.ext ? c_resp.ext : "";
        custom_client_type = c_resp.custom_client_type;
    }
};

/// @struct QChatLoginStatusResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLoginStatusResp {
    /// 登录状态
    NIMQChatLoginStatus login_status{kLoginStatusUnlogin};
    QChatLoginStatusResp() = default;
    QChatLoginStatusResp(const NIMQChatLoginStatusResp& c_resp) { login_status = c_resp.login_status; }
};

/// @struct QChatMultispotLoginResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatMultispotLoginResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code{0};
    /// 通知类型 1:登录 2:登出
    uint32_t notify_type{0};
    /// 客户端信息
    QChatLoginClientInfo client_info{};
    QChatMultispotLoginResp() = default;
    QChatMultispotLoginResp(const NIMQChatMultispotLoginResp& c_resp) {
        notify_type = c_resp.notify_type;
        client_info = c_resp.client_info;
    }
};

/// @struct QChatLoginResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLoginResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code{0};
    /// 客户端信息
    QChatLoginClientInfo client_info{};
    /// 其他客户端信息
    std::vector<QChatLoginClientInfo> other_clients{};
    QChatLoginResp() = default;
    QChatLoginResp(const NIMQChatLoginResp& c_resp) {
        res_code = c_resp.res_code;
        if (c_resp.res_code != kNIMResSuccess)
            return;
        client_info = c_resp.client_info;
        for (size_t i = 0; i < c_resp.other_clients_count; i++) {
            other_clients.emplace_back(c_resp.other_clients[i]);
        }
    }
    NIMQChatLoginResp ToCParam() const {
        NIMQChatLoginResp c_resp = {};
        c_resp.res_code = res_code;
        c_resp.client_info = client_info.ToCParam();
        c_resp.other_clients_count = other_clients.size();
        c_resp.other_clients = new NIMQChatLoginClientInfo[c_resp.other_clients_count];
        size_t i = 0;
        for (auto& it : other_clients) {
            c_resp.other_clients[i++] = it.ToCParam();
        }
        return c_resp;
    }
};

/// @struct QChatLogoutResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLogoutResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code{0};
    QChatLogoutResp() = default;
    QChatLogoutResp(const NIMQChatLogoutResp& c_resp) { res_code = c_resp.res_code; }
};

/// @struct QChatKickResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatKickResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code{0};
    /// 被踢的设备id列表
    std::vector<std::string> kicked_device_ids{};
    QChatKickResp() = default;
    QChatKickResp(const NIMQChatKickResp& c_resp) {
        res_code = c_resp.res_code;
        if (c_resp.res_code != kNIMResSuccess)
            return;
        for (size_t i = 0; i < c_resp.kicked_device_ids_count; i++)
            kicked_device_ids.push_back(c_resp.kicked_device_ids[i]);
    }
    NIMQChatKickResp ToCParam() const {
        NIMQChatKickResp c_param = {};
        c_param.res_code = res_code;
        vec_kicked_device_ids.clear();
        for (auto&& device_id : kicked_device_ids)
            vec_kicked_device_ids.push_back(const_cast<char*>(device_id.c_str()));
        c_param.kicked_device_ids_count = vec_kicked_device_ids.size();
        c_param.kicked_device_ids = vec_kicked_device_ids.data();
        return c_param;
    }

protected:
    // 以下字段为内部使用
    mutable std::vector<char*> vec_kicked_device_ids;
};

/// @struct QChatSDKLogResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatSDKLogResp {
    /// 日志内容
    std::string log_content{""};
    QChatSDKLogResp() = default;
    QChatSDKLogResp(const NIMQChatSDKLogResp& c_resp) { log_content = c_resp.log_content ? c_resp.log_content : ""; }
};

/// @struct QChatGetRTCTokenResp
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatGetRTCTokenResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code{0};
    /// token
    std::string token;
    /// ttl, 单位秒
    uint32_t ttl{0};
    QChatGetRTCTokenResp() = default;
    QChatGetRTCTokenResp(const NIMQChatGetRTCTokenResp& c_resp) {
        res_code = c_resp.res_code;
        token = c_resp.token ? c_resp.token : "";
        ttl = c_resp.ttl;
    }
    NIMQChatGetRTCTokenResp ToCParam() const {
        NIMQChatGetRTCTokenResp c_response{};
        c_response.res_code = res_code;
        c_response.token = const_cast<char*>(token.c_str());
        c_response.ttl = ttl;
        return c_response;
    }
};

/// 获取自定义token回调
typedef std::function<QChatCustomTokenResp(const QChatCustomTokenResp&)> CustomTokenCallback;
/// 被踢回调
typedef std::function<void(const QChatKickedResp&)> KickedCallback;
/// 被踢回调
typedef std::function<void(const QChatLoginStatusResp&)> LoginStatusCallback;
/// 多点登录回调
typedef std::function<void(const QChatMultispotLoginResp&)> MultispotLoginCallback;
/// SDK系统日志回调模板
typedef std::function<void(const QChatSDKLogResp&)> SDKLogCallback;
/// 登录回调
typedef std::function<void(const QChatLoginResp&)> LoginCallback;
/// 登出回调
typedef std::function<void(const QChatLogoutResp&)> LogoutCallback;
/// 踢掉自己其他端回调
typedef std::function<void(const QChatKickResp&)> KickCallback;
/// 获取RTC频道token回调
using GetRTCTokenCallback = std::function<void(const QChatGetRTCTokenResp&)>;

/// @struct QChatRegCustomTokenCbParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatRegCustomTokenCbParam {
    /// 回调函数
    CustomTokenCallback cb{nullptr};
    NIMQChatRegCustomTokenCbParam ToCParam() const {
        static CustomTokenCallback cb_holder = nullptr;
        static QChatCustomTokenResp resp_holder{};
        cb_holder = cb;
        NIMQChatRegCustomTokenCbParam param = {};
        param.cb = [](const NIMQChatCustomTokenResp* resp) {
            if (cb_holder)
                resp_holder = cb_holder(*resp);
            return resp_holder.ToCParam();
        };
        return param;
    }
};

/** @struct NIMQChatEncryptionConfiguration
 * @brief 圈组协议加密配置
 */
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatEncryptionConfiguration {
    /// "交换密钥"协议加密算法 {1(RSA),2(SM2)}, def:1(RSA) 非对称加密
    NIMQChatExchangeKeyEncryptionAlgorithm nego_key_neca = kNIMQChatEncryptionAlgorithmRSA;
    /// 通信加密算法 {1(RC4),2(AES128),4(SM4)} def:1(RC4) 对称加密
    NIMQChatCommEncryptionAlgorithm comm_neca = kNIMQChatCommEncryptionAlgorithmRC4;
    /// 握手协议类型 0:支持配置多种对称与非对称加密算法,1:只支持RAS + RC4
    NIMQChatHandShakeType hand_shake_type = kNIMQChatHandShakeTypeMulti;
    /// "交换密钥"协议加密算法密钥, 为空使用默认值
    std::string nego_key_neca_key_parta;
    /// 非对称加密算法 key
    std::string nego_key_neca_key_partb;
    /// 非对称加密算法的 key version
    uint32_t nego_key_neca_key_version{0};
    QChatEncryptionConfiguration() = default;
    QChatEncryptionConfiguration(const NIMQChatEncryptionConfiguration& c_param) {
        nego_key_neca = c_param.nego_key_neca;
        comm_neca = c_param.comm_neca;
        hand_shake_type = c_param.hand_shake_type;
        nego_key_neca_key_parta = c_param.nego_key_neca_key_parta ? c_param.nego_key_neca_key_parta : "";
        nego_key_neca_key_partb = c_param.nego_key_neca_key_partb ? c_param.nego_key_neca_key_partb : "";
        nego_key_neca_key_version = c_param.nego_key_neca_key_version;
    }
    NIMQChatEncryptionConfiguration ToCParam() const {
        NIMQChatEncryptionConfiguration c_param{};
        c_param.nego_key_neca = nego_key_neca;
        c_param.comm_neca = comm_neca;
        c_param.hand_shake_type = hand_shake_type;
        c_param.nego_key_neca_key_parta = const_cast<char*>(nego_key_neca_key_parta.c_str());
        c_param.nego_key_neca_key_partb = const_cast<char*>(nego_key_neca_key_partb.c_str());
        c_param.nego_key_neca_key_version = nego_key_neca_key_version;
        return c_param;
    }
};

/** @struct NIMQChatFCSConfiguration
 * @brief 圈组云端数据存储配置信息
 */
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatFCSConfiguration {
    /// 缩略图宽度
    uint32_t thumbnail_width = 150;
    /// 缩略图高度
    uint32_t thumbnail_height = 100;
    /// 是否自动下载历史消息附件
    bool auto_download_history_msg_attach_ = false;
    /// 是否自动下载图片缩略图
    bool auto_download_image_thumb = true;
    /// 是否自动下载视频缩略图
    bool auto_download_video_thumb = true;
    /// 是否自动下载图片
    bool auto_download_image = false;
    /// 是否自动下载音频
    bool auto_download_audio = false;
    /// 是否自动下载视频
    bool auto_download_video = false;
    /// 是否自动下载文件
    bool auto_download_file = false;
    /// 下载附件类数据的鉴权方式
    NIMQChatFCSAuthenticationType fcs_auth_type = kNIMQChatFCSAuthenticationTypeToken;
    /// mock user-agent
    std::string mock_ua;
    /// mock refer
    std::string mock_refer;
    QChatFCSConfiguration() = default;
    QChatFCSConfiguration(const NIMQChatFCSConfiguration& c_param) {
        thumbnail_width = c_param.thumbnail_width;
        thumbnail_height = c_param.thumbnail_height;
        auto_download_history_msg_attach_ = c_param.auto_download_history_msg_attach_;
        auto_download_image_thumb = c_param.auto_download_image_thumb;
        auto_download_video_thumb = c_param.auto_download_video_thumb;
        auto_download_image = c_param.auto_download_image;
        auto_download_audio = c_param.auto_download_audio;
        auto_download_video = c_param.auto_download_video;
        auto_download_file = c_param.auto_download_file;
        fcs_auth_type = c_param.fcs_auth_type;
        mock_ua = c_param.mock_ua ? c_param.mock_ua : "";
        mock_refer = c_param.mock_refer ? c_param.mock_refer : "";
    }
    NIMQChatFCSConfiguration ToCParam() const {
        NIMQChatFCSConfiguration c_param{};
        c_param.thumbnail_width = thumbnail_width;
        c_param.thumbnail_height = thumbnail_height;
        c_param.auto_download_history_msg_attach_ = auto_download_history_msg_attach_;
        c_param.auto_download_image_thumb = auto_download_image_thumb;
        c_param.auto_download_video_thumb = auto_download_video_thumb;
        c_param.auto_download_image = auto_download_image;
        c_param.auto_download_audio = auto_download_audio;
        c_param.auto_download_video = auto_download_video;
        c_param.auto_download_file = auto_download_file;
        c_param.fcs_auth_type = fcs_auth_type;
        c_param.mock_ua = const_cast<char*>(mock_ua.c_str());
        c_param.mock_refer = const_cast<char*>(mock_refer.c_str());
        return c_param;
    }
};

/// @struct QChatMessageCacheConfiguration
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatMessageCacheConfiguration {
    /// 是否开启缓存消息，默认不开启
    bool enable_message_cache = false;
    /// 最大缓存消息得频道数量，默认缓存 100 个频道
    size_t max_cache_channel_count;
    /// 每个频道中最大缓存消息数量，默认缓存 20 条消息
    size_t max_cache_message_count;
    QChatMessageCacheConfiguration() = default;
    QChatMessageCacheConfiguration(const NIMQChatMessageCacheConfiguration& c_param) {
        enable_message_cache = c_param.enable_message_cache;
        max_cache_channel_count = c_param.max_cache_channel_count;
        max_cache_message_count = c_param.max_cache_message_count;
    }
    NIMQChatMessageCacheConfiguration ToCParam() const {
        NIMQChatMessageCacheConfiguration c_param{};
        c_param.enable_message_cache = enable_message_cache;
        c_param.max_cache_channel_count = max_cache_channel_count;
        c_param.max_cache_message_count = max_cache_message_count;
        return c_param;
    }
};

/// @struct QChatInitParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatInitParam {
    /// 云信appkey
    std::string appkey;
    /// APP数据存储路径，如果为空，则使用系统默认路径
    std::string app_data_path;
    /// 自定义通信超时时间，单位秒, 为0使用默认值
    uint32_t custom_timeout{0};
    /// 自定义鉴权超时时间，单位秒, 为0使用默认值
    uint32_t auth_timeout{0};
    /// 数据库加密密钥，如果为空，则使用默认值
    std::string database_encrypt_key;
    /// 加密配置
    QChatEncryptionConfiguration encryption_configuration;
    /// FCS配置
    QChatFCSConfiguration fcs_configuration;
    /// 消息缓存配置
    QChatMessageCacheConfiguration message_cache_configuration;
    QChatInitParam() = default;
    QChatInitParam(const NIMQChatInitParam& param) {
        appkey = param.appkey ? param.appkey : "";
        app_data_path = param.app_data_path ? param.app_data_path : "";
        custom_timeout = param.custom_timeout;
        auth_timeout = param.auth_timeout;
        database_encrypt_key = param.database_encrypt_key ? param.database_encrypt_key : "";
        encryption_configuration = param.encryption_configuration;
        fcs_configuration = param.fcs_configuration;
        message_cache_configuration = param.message_cache_configuration;
    }
    NIMQChatInitParam ToCParam() const {
        NIMQChatInitParam param = {};
        param.appkey = const_cast<char*>(appkey.c_str());
        param.app_data_path = const_cast<char*>(app_data_path.c_str());
        param.custom_timeout = custom_timeout;
        param.auth_timeout = auth_timeout;
        param.database_encrypt_key = const_cast<char*>(database_encrypt_key.c_str());
        param.encryption_configuration = encryption_configuration.ToCParam();
        param.fcs_configuration = fcs_configuration.ToCParam();
        param.message_cache_configuration = message_cache_configuration.ToCParam();
        return param;
    }
};

/// @struct QChatRegKickedCbParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatRegKickedCbParam {
    /// 回调函数
    KickedCallback cb{nullptr};
    NIMQChatRegKickedCbParam ToCParam() const {
        static KickedCallback cb_holder = nullptr;
        cb_holder = cb;
        NIMQChatRegKickedCbParam param = {};
        param.cb = [](const NIMQChatKickedResp* resp) {
            if (cb_holder)
                cb_holder(*resp);
        };
        return param;
    }
};

/// @struct QChatRegLoginStatusCbParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatRegLoginStatusCbParam {
    /// 回调函数
    LoginStatusCallback cb{nullptr};
    NIMQChatRegLoginStatusCbParam ToCParam() const {
        static LoginStatusCallback cb_holder = nullptr;
        cb_holder = cb;
        NIMQChatRegLoginStatusCbParam param = {};
        param.cb = [](const NIMQChatLoginStatusResp* resp) {
            if (cb_holder)
                cb_holder(*resp);
        };
        return param;
    }
};

/// @struct QChatRegMultispotLoginCbParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatRegMultispotLoginCbParam {
    /// 回调函数
    MultispotLoginCallback cb{nullptr};
    NIMQChatRegMultispotLoginCbParam ToCParam() const {
        static MultispotLoginCallback cb_holder = nullptr;
        cb_holder = cb;
        NIMQChatRegMultispotLoginCbParam param = {};
        param.cb = [](const NIMQChatMultispotLoginResp* resp) {
            if (cb_holder)
                cb_holder(*resp);
        };
        return param;
    }
};

/// @struct QChatCleanupParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatCleanupParam {
    QChatCleanupParam() = default;
    QChatCleanupParam(const NIMQChatCleanupParam& param) {}
    NIMQChatCleanupParam ToCParam() const {
        NIMQChatCleanupParam param = {};
        return param;
    }
};

/// @struct QChatLoginParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLoginParam {
    /// 回调函数
    LoginCallback cb{nullptr};
    /// app唯一标识符
    std::string appkey;
    /// 用户帐号
    std::string accid;
    /// 鉴权方式
    NIMQChatLoginAuthType auth_type{kNIMQChatLoginAuthTypeDefault};
    /// 自定义终端类型
    uint32_t custom_client_type{0};
    /// 登陆的token
    std::string login_token;
    /// 登录自定义字段
    std::string login_ext;
    /// 链接地址列表
    std::vector<std::string> link_address{};
    QChatLoginParam() = default;
    QChatLoginParam(const NIMQChatLoginParam& param) {
        cb = [c_callback = param.cb, user_data = param.user_data](const QChatLoginResp& response) {
            auto c_response = response.ToCParam();
            c_response.user_data = user_data;
            c_callback(&c_response);
        };
        appkey = param.appkey;
        accid = param.accid;
        auth_type = param.auth_type;
        custom_client_type = param.custom_client_type;
        login_token = param.login_token;
        login_ext = param.login_ext;
        nim::JsonArrayStringToList(param.link_address, link_address);
    }
    NIMQChatLoginParam ToCParam() const {
        static LoginCallback cb_holder = nullptr;
        cb_holder = cb;
        link_address_json_array.clear();
        nim::StrListToJsonString(link_address, link_address_json_array);
        NIMQChatLoginParam param = {};
        param.cb = [](const NIMQChatLoginResp* resp) {
            if (cb_holder)
                cb_holder(*resp);
        };
        param.appkey = const_cast<char*>(appkey.c_str());
        param.accid = const_cast<char*>(accid.c_str());
        param.auth_type = auth_type;
        param.custom_client_type = custom_client_type;
        param.login_token = const_cast<char*>(login_token.c_str());
        param.login_ext = const_cast<char*>(login_ext.c_str());
        param.link_address = const_cast<char*>(link_address_json_array.c_str());
        return param;
    }

protected:
    // 以下字段为内部使用
    mutable std::string link_address_json_array;
};

/// @struct QChatLogoutParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatLogoutParam {
    /// 回调函数
    LogoutCallback cb{nullptr};
    NIMQChatLogoutParam ToCParam() const {
        static LogoutCallback cb_holder = nullptr;
        cb_holder = cb;
        NIMQChatLogoutParam param = {};
        param.cb = [](const NIMQChatLogoutResp* resp) {
            if (cb_holder)
                cb_holder(*resp);
        };
        return param;
    }
};

/// @struct QChatKickParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatKickParam {
    /// 回调函数
    KickCallback cb{nullptr};
    /// 要踢下线的设备id列表
    std::vector<std::string> device_ids{};
    QChatKickParam() = default;
    QChatKickParam(const NIMQChatKickParam& c_param) {
        for (size_t i = 0; i < c_param.device_ids_count; i++)
            device_ids.push_back(c_param.device_ids[i]);
        cb = [c_callback = c_param.cb, user_data = c_param.user_data](const QChatKickResp& response) {
            auto c_response = response.ToCParam();
            c_response.user_data = user_data;
            c_callback(&c_response);
        };
    }
    NIMQChatKickParam ToCParam() const {
        auto cb_holder = new KickCallback(cb);
        NIMQChatKickParam param = {};
        vec_device_ids.clear();
        for (auto&& accid : device_ids)
            vec_device_ids.push_back(const_cast<char*>(accid.c_str()));
        param.device_ids_count = vec_device_ids.size();
        param.device_ids = vec_device_ids.data();
        param.user_data = cb_holder;
        param.cb = [](const NIMQChatKickResp* resp) {
            InvokeCallback<KickCallback, NIMQChatKickResp>(resp);
        };
        return param;
    }

protected:
    // 以下字段为内部使用
    mutable std::vector<char*> vec_device_ids;
};

/// @struct QChatGetRTCTokenParam
struct NIM_QCHAT_SDK_CPPWRAPPER_DLL_API QChatGetRTCTokenParam {
    /// 获取RTC频道token回调
    GetRTCTokenCallback cb{nullptr};
    /// device_id
    std::string device_id{""};
    QChatGetRTCTokenParam() = default;
    QChatGetRTCTokenParam(const NIMQChatGetRTCTokenParam& c_param) {
        cb = [c_callback = c_param.cb, user_data = c_param.user_data](const QChatGetRTCTokenResp& response) {
            auto c_response = response.ToCParam();
            c_response.user_data = user_data;
            c_callback(&c_response);
        };
        device_id = c_param.device_id ? c_param.device_id : "";
    }
    NIMQChatGetRTCTokenParam ToCParam() const {
        auto cb_holder = new GetRTCTokenCallback(cb);
        NIMQChatGetRTCTokenParam param = {};
        param.user_data = cb_holder;
        param.cb = [](const NIMQChatGetRTCTokenResp* resp) {
            InvokeCallback<GetRTCTokenCallback, NIMQChatGetRTCTokenResp>(resp);
        };
        param.device_id = const_cast<char*>(device_id.c_str());
        return param;
    }
};

}  // namespace nim

#endif  // __NIM_QCHAT_CLIENT_CPP_DEF_H__
