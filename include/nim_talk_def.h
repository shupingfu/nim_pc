/** @file nim_talk_def.h
 * @brief P2P和群组聊天 接口相关的常量函数等定义头文件
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @author Oleg
 * @date 2015/02/02
 */
#ifndef NIM_SDK_DLL_EXPORT_HEADERS_TALK_DEF_H_
#define NIM_SDK_DLL_EXPORT_HEADERS_TALK_DEF_H_

#include "public_defines.h"

#ifdef __cplusplus
extern "C" {
#endif
/** @typedef void (*nim_talk_ack_cb_func)(const char *result, const void *user_data)
 * 发送消息回执的回调函数定义
 * @param[out] result        json string (Keys SEE MORE 『发送消息回执Json Keys』 as follows)
 * @param[out] user_data    APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_talk_ack_cb_func)(const char* result, const void* user_data);

/** @typedef void (*nim_talk_receive_cb_func)(const char *content, const char *json_extension, const void *user_data)
 * 接收消息的回调函数定义(如果是初始化时设置了自动下载图片和语音附件，开发者需要通过监听nim_nos_reg_download_cb返回的事件来保证附件已经成功缓存到本地)
 * @param[out] content            json string (Keys SEE MORE 『接收消息Json Keys』),批量接口回调时，内容为json string array
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_talk_receive_cb_func)(const char* content, const char* json_extension, const void* user_data);

/** @typedef bool (*nim_talk_team_notification_filter_func)(const char *content, const char *json_extension, const void *user_data)
 * 接收群通知是否需要过滤的函数定义（堵塞线程，谨慎使用，避免耗时行为）
 * @param[out] content            json string (Keys SEE MORE 『接收消息Json Keys』),批量接口回调时，内容为json string array
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef bool (*nim_talk_team_notification_filter_func)(const char* content, const char* json_extension, const void* user_data);

/** @typedef bool(*nim_talk_message_filter_func)(const char *content, const char *json_extension, const void *user_data)
 * 接收消息是否需要过滤的函数定义（堵塞线程，谨慎使用，避免耗时行为）
 * @param[out] content            json string (Keys SEE MORE 『接收消息Json Keys』),批量接口回调时，内容为json string array
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return bool true:APP已处理，SDK对这条消息不进行处理;false:APP未处理，SDK按正常逻辑处理
 */
typedef bool (*nim_talk_message_filter_func)(const char* content, const char* json_extension, const void* user_data);

/** @typedef void (*nim_talk_recall_msg_func)(int rescode, const char *content, const char *json_extension, const void *user_data)
 * 消息撤回通知
 * @param[out] rescode            错误码
 * @param[out] content            json string array string(Keys 『消息撤回通知Json Keys』)
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_talk_recall_msg_func)(int rescode, const char* content, const char* json_extension, const void* user_data);

/** @typedef void (*nim_talk_receive_broadcast_cb_func)(const char *content, const char *json_extension, const void *user_data)
 * 接收广播消息的回调函数定义
 * @param[out] content            json string (Keys SEE MORE 『接收广播消息Json Keys』),批量接口回调时，内容为json string array
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_talk_receive_broadcast_cb_func)(const char* content, const char* json_extension, const void* user_data);

/**
 *用于给撤回消息接口提供额外的参数，方便扩展
 */
typedef struct _nim_talk_recall_extra_params_ {
    const char* apnstext;
    const char* pushpayload;
    const char* json_extension;
    const char* env_config;
    const char* attach; /* v8.2.0 第三方回调回来的自定义字段 */
} nim_talk_recall_extra_params;

/** @name 消息撤回通知Json Keys
 * @{
 */
/// int,会话类型(NIMSessionType)
static const char* kNIMRecallMsgKeyToType = "to_type";
/// string,消息发送方ID
static const char* kNIMRecallMsgKeyFromAccID = "from_id";
/// string,消息接收方ID
static const char* kNIMRecallMsgKeyToAccID = "to_id";
/// string,客户端消息ID
static const char* kNIMRecallMsgKeyMsgId = "msg_id";
/// string,自定义通知文案,按需填
static const char* kNIMRecallMsgKeyNotify = "notify";
/// long,撤回操作的消息时间戳(毫秒)
static const char* kNIMRecallMsgKeyTime = "time";
/// int,撤回通知种类（NIMMessageFeature）
static const char* kNIMRecallMsgKeyNotifyFeature = "feature";
/// bool,撤回的消息本地是否存在,比如对方离线时发一条消息又撤回,对方上线收到离线撤回通知该tag为false
static const char* kNIMRecallMsgKeyMsgExist = "msg_exist";
/// long,要撤回消息的创建时间戳(毫秒)
static const char* kNIMRecallMsgKeyMsgTime = "msg_time";
/// string,要撤回消息的发送者昵称
static const char* kNIMRecallMsgKeyMsgFromNick = "from_nick";
/// string,操作者ID
static const char* kNIMRecallMsgKeyOpeAccID = "operator_id";
/// string,v8.2.0 第三方回调回来的自定义字段
static const char* kNIMRecallMsgTagCallbackExt = "callback_ext";
/// string,v8.2.0
static const char* kNIMRecallMsgTagAttach = "attach";
/** @}*/  // 消息撤回通知Json Keys

/** @name 接收消息Json Keys
 * 本地定义的error code如下：
 * kNIMResSuccess(200) : no error
 * kNIMLocalResParameterError(414) : url invalid
 * kNIMLocalResExist(417) : local file has exists
 * for example:
 * {"rescode": 200,
 *  "feature": 1,
 *  "content": "***"
 * }
 * @{
 */
/// int,消息错误码(NIMResCode)
static const char* kNIMMsgKeyLocalRescode = "rescode";
/// int,消息种类(NIMMessageFeature),SDK本地维护
static const char* kNIMMsgKeyLocalMsgFeature = "feature";
/// json string,消息内容，协议定义的消息结构
static const char* kNIMMsgKeyLocalReceiveMsgContent = "content";
/** @}*/  // 接收消息Json Keys

/** @name 消息结构 Json Keys
 * 例子 {"client_msg_id" : "c0d1167459fc754098a531482b51a1da","local_res_path" : "","log_status" : 4,"log_sub_status" : 0,"msg_attach" : "","msg_body"
 * : "发送文本消息正文", "msg_type" : 0, "resend_flag" : 0, "time" : 1437014897000, "to_accid" : "4045", "to_type" : 1}
 * @{
 */
// 协议定义
/// int,会话类型(NIMSessionType)
static const char* kNIMMsgKeyToType = "to_type";
/// string,消息接收方id,给自己发送消息时填写自己id
static const char* kNIMMsgKeyToAccount = "to_accid";
/// string,消息发送方id,服务器填写,发送方不需要填写
static const char* kNIMMsgKeyFromAccount = "from_id";
/// int,消息发送方客户端类型（NIMClientType）,服务器填写,发送方不需要填写
static const char* kNIMMsgKeyFromClientType = "from_client_type";
/// string,消息发送方设备id,服务器填写,发送方不需要填写
static const char* kNIMMsgKeyFromDeviceId = "from_device_id";
/// string,消息发送方昵称,服务器填写,发送方不需要填写
static const char* kNIMMsgKeyFromNick = "from_nick";
/// long,消息时间戳(毫秒)
static const char* kNIMMsgKeyTime = "time";
/// int,消息类型(NIMMessageType)
static const char* kNIMMsgKeyType = "msg_type";
/// string,消息正文,长度限制：5000字符
static const char* kNIMMsgKeyBody = "msg_body";
/// string,消息多媒体资源参数,不同类型多媒体参数不同,发送非多媒体消息时不需要填写,如需使用json string,必须为可以解析为json的非格式化的字符串
static const char* kNIMMsgKeyAttach = "msg_attach";
/// string,客户端消息id
static const char* kNIMMsgKeyClientMsgid = "client_msg_id";
/// long,服务器端消息id
static const char* kNIMMsgKeyServerMsgid = "server_msg_id";
/// int,消息重发标记位,第一次发送0,重发1
static const char* kNIMMsgKeyResendFlag = "resend_flag";
/// int,(可选)该消息是否存储云端历史,可选，0:不支持,1:支持, 默认1
static const char* kNIMMsgKeyHistorySave = "cloud_history";
/// int,(可选)该消息是否支持漫游,可选,0:不支持,1:支持, 默认1
static const char* kNIMMsgKeyMsgRoaming = "roam_msg";
/// int,(可选)该消息是否支持发送者多端同步,可选,0:不支持,1:支持, 默认1
static const char* kNIMMsgKeyMsgSync = "sync_msg";
/// int,(可选)推送是否要做消息计数(角标)，0:不需要，1:需要，默认1
static const char* kNIMMsgKeyPushNeedBadge = "push_need_badge";
/// string,(可选)自定义扩展字段,必须为可以解析为json的非格式化的字符串,长度限制1024
static const char* kNIMMsgKeyServerExt = "server_ext";
/// string,(可选)第三方自定义的推送属性，必须为可以解析为json的非格式化的字符串，长度2048
static const char* kNIMMsgKeyPushPayload = "push_payload";
/// string,(可选)自定义推送文案，长度限制200字节
static const char* kNIMMsgKeyPushContent = "push_content";
/// int,(可选)是否需要推送, 0:不需要,1:需要,默认1,aos在收到0是不要模拟本地推送
static const char* kNIMMsgKeyPushEnable = "push_enable";
/// int,(可选)推送是否需要前缀，0:不需要，1:需要，默认1
static const char* kNIMMsgKeyPushNeedPrefix = "push_prefix";
/// int,(可选)该消息是否抄送,0:不支持,1:支持,默认按照app的路由开关
static const char* kNIMMsgKeyMsgRoutable = "routable_msg";
/// int,(可选)消息是否要存离线,0:不需要, 1:需要，默认1
static const char* kNIMMsgKeySetMsgOffline = "offline_msg";
/// string,(可选)群组消息强推列表,推送指定账号id string array json, 如果推送全员不填
static const char* kNIMMsgKeyForcePushList = "force_push_list";
/// string,(可选)群组消息强推文本
static const char* kNIMMsgKeyForcePushContent = "force_push_content";
/// int,(可选)群组消息是否强推,0:不强推, 1:强推，属性只针对群组消息强推列表
static const char* kNIMMsgKeyIsForcePush = "is_force_push";
/// int, 是否需要过易盾反垃圾, 0:不需要,1:需要, 默认0
static const char* kNIMMsgKeyAntiSpamEnable = "anti_spam_enable";
/// string, (可选)开发者自定义的反垃圾字段,长度限制：5000字符, 格式为json string,{"type" : 1:文本，2：图片，3视频, "data" :
/// "文本内容or图片地址or视频地址"}
static const char* kNIMMsgKeyAntiSpamContent = "anti_spam_content";
/// int, (可选) 是否命中客户端反垃圾,命中:1 未命中：0 或者不填写
static const char* kNIMMsgKeyClientAntiSpam = "client_anti_spam_hitting";
/// string,  (可选)用户配置的对某些单条消息另外的反垃圾的业务ID
static const char* kNIMMsgKeyAntiSpamBizId = "anti_spam_business_id";
/// int,  (可选) 单条消息是否使用易盾反垃圾 0:(在开通易盾的情况下)不过易盾反垃圾
static const char* kNIMMsgKeyAntiSpamUsingYiDun = "anti_spam_using_yidun";
/// (可选)int, 群消息是否需要已读业务，0：不需要，1：需要
static const char* kNIMMsgKeyTeamMsgAck = "team_msg_ack";
/// (可选)bool, 消息是否需要刷新到session服务，false:否，true:是；只有消息存离线的情况下，才会判断该参数，缺省：true
static const char* kNIMMsgKeyIsUpdateSession = "is_update_session";
/// (可选)String, 易盾反垃圾增强反作弊专属字段, 限制json，长度限制1024
static const char* kNIMMsgKeyAntiCheatingYiDun = "yidun_anti_cheating";
/// (可选)String, 易盾反垃圾扩展字段，限制json，长度限制1024
static const char* kNIMMsgKeyAntiSpamExt = "anti_spam_ext";
/// String, 易盾反垃圾返回的结果字段
static const char* kNIMMsgKeyAntiSpamRes = "anti_spam_res";
static const char* kNIMMsgKeyRetweetedMsgId = "retweeted_msg_id"; /**< (可选)String, 转发的消息id*/

// thread 消息信息
/// (可选) json object，被回复消息的消息发送者
static const char* kNIMMsgKeyThreadInfo = "thread_info";
/// (可选)string，从属thread_info节点，被回复消息的消息发送者
static const char* kNIMMsgKeyReplyMsgFromAccount = "reply_msg_from_account";
/// (可选)string，从属thread_info节点，被回复消息的消息接受者，群的话是tid
static const char* kNIMMsgKeyReplyMsgToAccount = "replymsg_to_account";
/// (可选)long，从属thread_info节点，被回复消息的消息发送时间
static const char* kNIMMsgKeyReplyMsgTime = "reply_msg_time";
/// (可选)long，从属thread_info节点，被回复消息的消息ID)serverId)
static const char* kNIMMsgKeyReplyMsgIdServer = "reply_msg_id_server";
/// (可选)string，从属thread_info节点，被回复消息的消息ID)clientId)
static const char* kNIMMsgKeyReplyMsgIdClient = "reply_msg_id_client";
/// (可选)string，从属thread_info节点，thread消息的消息发送者
static const char* kNIMMsgKeyThreadMsgFromAccount = "thread_msg_from_account";
/// (可选)string，t从属thread_info节点，hread消息的消息接受者，群的话是tid
static const char* kNIMMsgKeyThreadMsgToAccount = "thread_msg_to_account";
/// (可选)long，从属thread_info节点，thread消息的消息发送时间
static const char* kNIMMsgKeyThreadMsgTime = "thread_msg_time";
/// (可选)long，从属thread_info节点，thread消息的消息ID)serverId)
static const char* kNIMMsgKeyThreadMsgIdServer = "thread_msg_id_server";
/// (可选)string，从属thread_info节点，thread消息的消息ID)clientId)
static const char* kNIMMsgKeyThreadMsgIdClient = "thread_msg_id_client";
/// int
/// 从属thread_info节点，消息是否已经被删除（可能是撤回，也可能是单向删除），查询thread消息历史时可能会有这个字段，大于0表示已经删除（目前撤回和单向删除都是1，未来可能区分）
static const char* kNIMMsgKeyDeleted = "deleted";
// v7.8 新增
/// (可选)string第三方回调回来的自定义扩展字段
static const char* kNIMMsgKeyThirdPartyCBEXT = "third_party_callback_ext";
/// (可选)int，消息的子类型，客户端定义，服务器透传
static const char* kNIMMsgKeySubType = "msg_sub_type";

// v8.0.0新增
static const char* kNIMMsgKeyEnv = "env_config"; /**(可选)String, 环境变量，用于指向不同的抄送、第三方回调等配置 */

// 本地定义
/// bool 是否已经发送群消息已读回执
static const char* kNIMMsgKeyLocalKeyTeamMsgAckSent = "team_msg_ack_sent";
/// int, 群消息未读数
static const char* kNIMMsgKeyLocalKeyTeamMsgUnreadCount = "team_msg_unread_count";
/// string,多媒体消息资源本地绝对路径,SDK本地维护,发送多媒体消息时必填
static const char* kNIMMsgKeyLocalFilePath = "local_res_path";
/// string,会话id,发送方选填,接收方收到的是消息发送方id
static const char* kNIMMsgKeyLocalTalkId = "talk_id";
/// string,多媒体资源id,发送方选填,接收方收到的是客户端消息id
static const char* kNIMMsgKeyLocalResId = "res_id";
/// int,消息状态(NIMMsgLogStatus)
static const char* kNIMMsgKeyLocalLogStatus = "log_status";
/// int,消息二级状态(NIMMsgLogSubStatus)
static const char* kNIMMsgKeyLocalLogSubStatus = "log_sub_status";
/// string,只维护在本地的扩展字段,必须为可以解析为json的非格式化的字符串
static const char* kNIMMsgKeyLocalExt = "local_ext";
/** @}*/  // 消息结构 Json Keys

/** @name 发送消息回执Json Keys
 * @{
 */
/// string,客户端消息id
static const char* kNIMSendAckKeyMsgId = "msg_id";
/// long,服务器消息id
static const char* kNIMSendAckKeyMsgIdServer = "msg_id_server";
/// string,会话id
static const char* kNIMSendAckKeyTalkId = "talk_id";
/// int,消息错误码(NIMResCode)
static const char* kNIMSendAckKeyRescode = "rescode";
/// long,消息时间戳(毫秒),收到ack包时更新上层缓存的消息时间戳
static const char* kNIMSendAckKeyTimetag = "msg_timetag";
/// string,第三方回调回来的自定义扩展字段 v7.8
static const char* kNIMSendAckKeyCallbackExt = "third_party_callback_ext";
/// string,易盾反垃圾返回的结果字段
static const char* kNIMSendAckKeyAntiSpamRes = "anti_spam_res";

/** @}*/  // 发送消息回执Json Keys

/** @name attach多媒体资源参数通用键名定义，可替代不同类型多媒体所使用的相同的参数的键名.
 * @{
 */
/// string,资源MD5,发送方选填
static const char* kNIMMsgAttachKeyMd5 = "md5";
/// long,资源大小(KB),发送方选填
static const char* kNIMMsgAttachKeySize = "size";
/// long,资源URL, 发送方不需要填写
static const char* kNIMMsgAttachKeyUrl = "url";
/// string,资源扩展名,不包括符号'.',发送方若选填需要保证资源本地路径包含扩展名
static const char* kNIMMsgAttachKeyExt = "ext";
/// string,用于显示的文件名,发送方选填,默认文件名
static const char* kNIMMsgAttachKeyDisplayName = "name";
/// string, (可选)发送含有附件的消息时使用的场景标签(可参见nos删除策略) ,Audio Image Video File 或者可以被SDK解析到本地文件路径的自定义消息
static const char* kNIMMsgAttachKeyTag = "upload_tag";
/** @}*/  // attach多媒体资源参数通用键名定义，可替代不同类型多媒体所使用的相同的参数的键名.

/** @name attach图片类型多媒体资源参数键名定义
 * @{
 */
/// string,资源MD5,发送方选填
static const char* kNIMImgMsgKeyMd5 = "md5";
/// long,资源大小(KB),发送方选填
static const char* kNIMImgMsgKeySize = "size";
/// int,图片宽度,发送方必填
static const char* kNIMImgMsgKeyWidth = "w";
/// int,图片高度,发送方必填
static const char* kNIMImgMsgKeyHeight = "h";
/// string,资源URL, 发送方不需要填写
static const char* kNIMImgMsgKeyUrl = "url";
/// string,资源扩展名,不包括符号'.',发送方若选填需要保证资源本地路径包含扩展名
static const char* kNIMImgMsgKeyExt = "ext";
/// string,用于显示的文件名,发送方选填,默认文件名
static const char* kNIMImgMsgKeyDisplayName = "name";
/** @}*/  // attach图片类型多媒体资源参数键名定义

/** @name attach语音类型多媒体资源参数键名定义
 * @{
 */
/// string,资源MD5,发送方选填
static const char* kNIMAudioMsgKeyMd5 = "md5";
/// long,资源大小(KB),发送方选填
static const char* kNIMAudioMsgKeySize = "size";
/// int,语音资源时间长度(毫秒),发送方必填
static const char* kNIMAudioMsgKeyDuration = "dur";
/// string,资源URL, 发送方不需要填写
static const char* kNIMAudioMsgKeyUrl = "url";
/// string,资源扩展名,不包括符号'.',发送方若选填需要保证资源本地路径包含扩展名
static const char* kNIMAudioMsgKeyExt = "ext";
/// string,用于显示的文件名,发送方选填,默认文件名
static const char* kNIMAudioMsgKeyDisplayName = "name";
/** @}*/  // attach语音类型多媒体资源参数键名定义

/** @name attach短视频类型多媒体资源参数键名定义
 * @{
 */
/// string,资源MD5,发送方选填
static const char* kNIMVideoMsgKeyMd5 = "md5";
/// long,资源大小(KB),发送方选填
static const char* kNIMVideoMsgKeySize = "size";
/// string,用于显示的文件名,发送方选填,默认文件名
static const char* kNIMVideoMsgKeyDisplayName = "name";
/// int,短视频资源时间长度(毫秒),发送方必填
static const char* kNIMVideoMsgKeyDuration = "dur";
/// int,短视频画面宽度,发送方必填
static const char* kNIMVideoMsgKeyWidth = "w";
/// int,短视频画面高度,发送方必填
static const char* kNIMVideoMsgKeyHeight = "h";
/// string,资源URL, 发送方不需要填写
static const char* kNIMVideoMsgKeyUrl = "url";
/// string,资源扩展名,不包括符号'.',发送方若选填需要保证资源本地路径包含扩展名
static const char* kNIMVideoMsgKeyExt = "ext";
/** @}*/  // attach短视频类型多媒体资源参数键名定义

/** @name attach文件类型多媒体资源参数键名定义
 * @{
 */
/// string,资源MD5,发送方选填
static const char* kNIMFileMsgKeyMd5 = "md5";
/// long,资源大小(KB),发送方选填
static const char* kNIMFileMsgKeySize = "size";
/// string,用于显示的文件名,发送方选填,默认文件名
static const char* kNIMFileMsgKeyDisplayName = "name";
/// string,资源URL, 发送方不需要填写
static const char* kNIMFileMsgKeyUrl = "url";
/// string,资源扩展名,不包括符号'.',发送方若选填需要保证资源本地路径包含扩展名
static const char* kNIMFileMsgKeyExt = "ext";
/** @}*/  // attach文件类型多媒体资源参数键名定义

/** @name attach位置类型多媒体资源参数键名定义
 * @{
 */
/// double,纬度
static const char* kNIMLocationMsgKeyLatitude = "lat";
/// double,经度
static const char* kNIMLocationMsgKeyLongitude = "lng";
/// string,位置信息
static const char* kNIMLocationMsgKeyTitle = "title";
/** @}*/  // attach位置类型多媒体资源参数键名定义

/** @name attach波特机器人类型多媒体资源参数键名定义
 * @{
 */
/// string，机器人云信ID
static const char* kNIMBotRobotMsgKeyRobotID = "robotAccid";
/// json string，给机器人发送时需要填的字段，字段内容定义可见Demo源码或参考波特开发文档
static const char* kNIMBotRobotSentMsgKeyParam = "param";
/// string，机器人回复时带的字段，表示该条回复相关联的消息ID
static const char* kNIMBotRobotReceivedMsgKeyClientMsgID = "clientMsgId";
/// bool，是否为机器人回复，默认为true
static const char* kNIMBotRobotReceivedMsgKeyMsgOut = "msgOut";
/// json string，机器人回复时带的字段，波特机器人消息
static const char* kNIMBotRobotReceivedMsgKeyRobotMsg = "robotMsg";
/// string，机器人回复时带的字段，表示该条回复类型，类型定义可见Demo源码或参考波特开发文档，bot或faq
static const char* kNIMBotRobotReceivedMsgKeyRobotMsgFlag = "flag";
/// json object，机器人回复时带的字段，表示该条回复内容，字段内容定义可见Demo源码或参考波特开发文档
static const char* kNIMBotRobotReceivedMsgKeyRobotMsgMessage = "message";
/** @}*/  // attach波特机器人类型多媒体资源参数键名定义

/** @name 广播消息 Json Keys
 * @{
 */
/// int64，消息ID
static const char* kNIMBroadcastMsgKeyID = "id";
/// jstring，发送者accid，可能不存在
static const char* kNIMBroadcastMsgKeyFromAccid = "from_accid";
/// int64， 时间戳
static const char* kNIMBroadcastMsgKeyTime = "time";
/// string，内容
static const char* kNIMBroadcastMsgKeyBody = "body";
/** @}*/  // 广播消息 Json Keys

/** @name 根据指定条件查询本地消息参数 Json Keys
 * @{
 */
/// enum NIMMsgLogQueryRange，消息历史的检索范围（目前暂不支持某些范围的组合检索，详见NIMMsgLogQueryRange说明）
static const char* kNIMQueryMsgByOptionsAsyncKeyQueryRange = "query_range";
/// jsonstring 会话id（对方的account id或者群组tid）的集合，目前暂不支持多个的组合检索，详见NIMMsgLogQueryRange说明
static const char* kNIMQueryMsgByOptionsAsyncKeyIDS = "ids";
/// int32， 本次查询的消息条数上限(最多100条)
static const char* kNIMQueryMsgByOptionsAsyncKeyLimit = "limit_count";
/// int64，起始时间点，单位：毫秒
static const char* kNIMQueryMsgByOptionsAsyncKeyFromTime = "from_time";
/// int64，结束时间点，单位：毫秒
static const char* kNIMQueryMsgByOptionsAsyncKeyEndTime = "end_time";
/// string，结束查询的最后一条消息的end_client_msg_id(不包含在查询结果中)
static const char* kNIMQueryMsgByOptionsAsyncKeyEndClientMSGID = "end_client_msg_id";
/// bool，true：反向查询(按时间正序起查，正序排列)，false：按时间逆序起查，逆序排列（建议默认为false）
static const char* kNIMQueryMsgByOptionsAsyncKeyReverse = "reverse";
/// enum NIMMessageType  检索的消息类型（目前只支持kNIMMessageTypeText、kNIMMessageTypeImage和kNIMMessageTypeFile这三种类型消息）
static const char* kNIMQueryMsgByOptionsAsyncKeyMsgType = "msg_type";
/// int32，消息的子类型
static const char* kNIMQueryMsgByOptionsAsyncKeyMsgSubType = "msg_sub_type";
/// string，检索文本（目前只支持kNIMMessageTypeText和kNIMMessageTypeFile这两种类型消息的文本关键字检索，即支持文字消息和文件名的检索
static const char* kNIMQueryMsgByOptionsAsyncKeySearchContent = "search_content";
/** @}*/  // 根据指定条件查询本地消息参数 Json Keys

#ifdef __cplusplus
};
#endif  //__cplusplus
#endif  // NIM_SDK_DLL_EXPORT_HEADERS_TALK_DEF_H_
