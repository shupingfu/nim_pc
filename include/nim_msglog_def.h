/** @file nim_msglog_def.h
 * @brief 消息历史 接口相关的常量函数等定义头文件
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @author Oleg
 * @date 2015/02/12
 */
#ifndef NIM_SDK_DLL_EXPORT_HEADERS_MSGLOG_DEF_H_
#define NIM_SDK_DLL_EXPORT_HEADERS_MSGLOG_DEF_H_

#include "nim_session_def.h"
#include "public_defines.h"

#ifdef __cplusplus
extern "C" {
#endif
/** @brief 删除与某一会话的历史记录推送数据定义 */
typedef struct {
    /// enum 会话类型，双人0，群组1,超大群5 (nim_msglog_def.h)
    enum NIMSessionType to_type;
    /// string 会话的ID
    char id[128];
    /// int64_t 删除操作的时间戳
    int64_t time;
    /// 扩展字段
    char ext[1024];
} NIMDeleteSessionHistoryMessagesNotifyInfo;

/** @typedef void(*nim_msglog_delete_history_online_res_cb_func)(int res_code, const char *accid,const void *user_data)
 * 删除与某一账号云端的历史记录回调函数定义(p2p类型)
 * @param[out] res_code        操作结果，成功200
 * @param[out] accid            进行删除操作时传入的对方accid
 * @param[out] user_data    APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_delete_history_online_res_cb_func)(int res_code, const char* accid, const void* user_data);

/** @typedef void(*nim_msglog_delete_history_online_res_cb_func_ex)(int res_code,
 *                                                                  const char *accid,
 *                                                                  enum NIMSessionType type,
 *                                                                  uint64_t time,const char *ext,
 *                                                                  const void *user_data)
 * 删除某一会话的云端的历史记录回调函数定义[v8.0.0]
 * @param[out] res_code        操作结果，成功200
 * @param[out] accid            进行删除操作时传入的会话id p2p:accid team:tid
 * @param[out] type                会话的类型
 * @param[out] time                删除时间
 * @param[out] ext                删除某一会话的云端的历史记录操作时传入的ext字段
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_delete_history_online_res_cb_func_ex)(int res_code,
    const char* accid,
    enum NIMSessionType type,
    uint64_t time,
    const char* ext,
    const void* user_data);

/** @typedef void(*nim_msglog_delete_history_online_notify_cb_func)(NIMDeleteSessionHistoryMessagesNotifyInfo *info_list_head_node,
 *                                                                  int node_count,const void *user_data)
 * 删除某一会话的云端的历史记录推送回调函数定义[v8.0.0]
 * @param[out] info_list_head_node 被删除的会话列表的头指针
 * @param[out] node_count     会话列表的长度
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_delete_history_online_notify_cb_func)(NIMDeleteSessionHistoryMessagesNotifyInfo* info_list_head_node,
    int node_count,
    const void* user_data);

/** @typedef void(*nim_msglog_delete_message_self_res_cb_func)(int res_code, const char *result,const void *user_data)
 * 单向删除消息记录回调函数定义
 * @param[out] res_code        操作结果，成功200
 * @param[out] user_data    APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_delete_message_self_res_cb_func)(int res_code, const void* user_data);

/** @typedef void(*nim_msglog_delete_message_self_notify_cb_func)(const char *infos,const void *user_data)
 * 单向删除消息记录推送回调函数定义
 * @param[out] infos    被删除的消息摘要 数组包括session_id ,client_id,ext字段
 * @param[out] user_data    APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_delete_message_self_notify_cb_func)(const char* infos, const void* user_data);

/** @typedef void (*nim_msglog_query_single_cb_func)(int res_code, const char *msg_id, const char *result,
 *                                                   const char *json_extension, const void *user_data)
 * 查询单条消息历史回调函数定义
 * @param[out] res_code        操作结果，成功200
 * @param[out] id                查询时传入的客户端消息ID
 * @param[out] result            查询结果 Json String (Keys SEE MORE in `nim_talk_def.h` 『消息结构 Json Keys』)
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (
    *nim_msglog_query_single_cb_func)(int res_code, const char* msg_id, const char* result, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_query_cb_func)(int res_code,
 *                                            const char *id,
                                              enum NIMSessionType type,
                                              const char *result,
                                              const char *json_extension,
                                              const void *user_data)
 * 本地或在线查询消息的回调函数定义
 * @param[out] res_code            操作结果，成功200
 *                                1. 在线查询双人会话消息需要处理的错误号：
 *                                414:参数错误
 *                                2. 在线查询群组消息需要处理的错误号：
 *                                414:参数错误
 *                                403:禁止访问(不在该群)
 * @param[out] id                查询时传入的对方account id或者群组tid
 * @param[out] type             查询时传入的会话类型
 * @param[out] result            查询结果json object array string (SEE MORE 『查询消息历史结果Json Keys』 as
 *follows)（查询结果的实际数量需要解析该结果获取）
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_query_cb_func)(int res_code,
    const char* id,
    enum NIMSessionType type,
    const char* result,
    const char* json_extension,
    const void* user_data);

/** @typedef void (*nim_msglog_full_text_search_online_cb_func)(int res_code, uint32_t count, const char *result, const char *json_extension, const
 * void *user_data)
 * @param[out] res_code         操作结果，成功200
 * @param[out] count            返回的消息总数
 * @param[out] result           全文检索返回的消息体列表
 * @param[out] json_extension    json 扩展数据（备用）
 * @param[out] user_data        APP 的自定义用户数据，SDK 只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (
    *nim_msglog_full_text_search_online_cb_func)(int res_code, uint32_t count, const char* result, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_res_cb_func)(int res_code, const char *msg_id, const char *json_extension, const void *user_data)
 * 消息历史操作结果的回调函数定义(按消息历史id操作)
 * @param[out] res_code        操作结果，成功200
 * @param[out] msg_id            消息历史id
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_res_cb_func)(int res_code, const char* msg_id, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_res_ex_cb_func)(int res_code, const char *uid, enum NIMSessionType type,
 *                                             const char *json_extension, const void *user_data)
 * 消息历史操作结果的回调函数定义（按消息对象id操作）
 * @param[out] res_code    操作结果，成功200
 * @param[out] uid            对象id(account_id , team_id etc.)
 * @param[out] to_type        对象类型(好友， 群组 etc.)
 * @param[out] json_extension json扩展数据（备用）
 * @param[out] user_data    APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_res_ex_cb_func)(int res_code, const char* uid, enum NIMSessionType type, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_modify_res_cb_func)(int res_code, const char *json_extension, const void *user_data)
 * 消息历史操作结果的回调函数定义(只关心rescode)
 * @param[out] res_code        操作结果，成功200
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_modify_res_cb_func)(int res_code, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_import_prg_cb_func)(int64_t imported_count, int64_t total_count, const char *json_extension, const void *user_data)
 * 消息历史数据库导入过程的回调函数定义
 * @param[out] imported_count         已导入的消息历史记录数目
 * @param[out] total_count            消息历史记录总数目
 * @param[out] json_extension        json扩展数据（备用）
 * @param[out] user_data            APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_import_prg_cb_func)(int64_t imported_count, int64_t total_count, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_status_changed_cb_func)(int res_code, const char *result, const char *json_extension, const void *user_data)
 * 消息状态变更通知的回调函数定义
 * @param[out] res_code         操作结果，成功200
 * @param[out] result            结果(key定义见下面的 消息状态变更通知结果 Json Keys)
 * @param[out] json_extension    json扩展数据（备用）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_status_changed_cb_func)(int res_code, const char* result, const char* json_extension, const void* user_data);

/** @typedef void (*nim_msglog_query_message_is_thread_root_async_cb_func)(int res_code,
                                                                           const char *client_id,
                                                                           bool is_root,
                                                                           int reply_count,
                                                                           const void *user_data)
 * 查询某条消息是否为thread聊天的根消息结果回调
 * @param[out] res_code         操作结果，成功200
 * @param[out] client_id         被查询的消息的客户端id
 * @param[out] is_root            是否为根消息 true/false
 * @param[out] reply_count        当为根消息时会同时返回被回复数量(回复数量是基于本地缓存数据统计,可以通过查询云端thread talk历史记录拿到精确值)
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_query_message_is_thread_root_async_cb_func)(int res_code,
    const char* client_id,
    bool is_root,
    int reply_count,
    const void* user_data);

/** @typedef void (*nim_msglog_query_thread_history_msg_cb_func)(int res_code,
                                                                 const char *root_msg,
                                                                 int total,
                                                                 uint64_t timestamp,
                                                                 const char *msgs,
                                                                 const void *user_data)
 * 查询thread 聊天的记录结果回调
 * @param[out] res_code         操作结果，成功200
 * @param[out] root_msg         thread 聊天根消息的内容
 * @param[out] total            总的消息数量
 * @param[out] timestamp    最的一条回复消息的时间戳
 * @param[out] msgs    回复消息的列表 json object array string (SEE MORE 『查询消息历史结果Json Keys』 as
 * follows)（查询结果的实际数量需要解析该结果获取）
 * @param[out] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
 * @return void 无返回值
 */
typedef void (*nim_msglog_query_thread_history_msg_cb_func)(int res_code,
    const char* root_msg,
    int total,
    uint64_t timestamp,
    const char* msgs,
    const void* user_data);

/** @name 消息状态变更通知结果 Json Keys
 * @{
 */
/// 会话ID
static const char* kNIMMsglogStatusChangedKeyTalkID = "talk_id";
/// 临界的消息的时间戳
static const char* kNIMMsglogStatusChangedKeyMsgTimetag = "msg_timetag";
/// 变更后的状态
static const char* kNIMMsglogStatusChangedKeyStatus = "status";
/** @}*/  // 消息状态变更通知结果 Json Keys

/** @name 查询消息历史结果Json Keys
  * for example:
  * {"count": 20,
         "source" : 0,
  *  "content":[obj, obj, obj, ....]
  * }
  * @{
  */
/// int, 查询得到的数量
static const char* kNIMMsglogQueryKeyCount = "count";
/// NIMMsglogQuerySource, 查询结果来源
static const char* kNIMMsglogQueryKeySource = "source";
static const char* kNIMMsglogQueryKeyContent =
    /// json object array (Keys SEE MORE in `nim_talk_def.h` 『消息结构 Json Keys』)，查询得到的msglog内容
    "content";
static const char* kNIMMsglogReliable = "reliable";
/** @}*/  // 查询消息历史结果Json Keys

/** @enum NIMMsglogQuerySource 消息历史查询来源 */
enum NIMMsglogQuerySource {
    /// 本地查询
    kNIMMsglogQuerySourceLocal = 0,
    /// 云端查询
    kNIMMsglogQuerySourceServer = 1,
};

/** @enum NIMMessageType Message Type */
enum NIMMessageType {
    /// 文本类型消息
    kNIMMessageTypeText = 0,
    /// 图片类型消息
    kNIMMessageTypeImage = 1,
    /// 声音类型消息
    kNIMMessageTypeAudio = 2,
    /// 视频类型消息
    kNIMMessageTypeVideo = 3,
    /// 位置类型消息
    kNIMMessageTypeLocation = 4,
    /// 系统类型通知（包括入群出群通知等） NIMNotificationId
    kNIMMessageTypeNotification = 5,
    /// 文件类型消息
    kNIMMessageTypeFile = 6,
    /// 提醒类型消息,Tip内容根据格式要求填入消息结构中的kNIMMsgKeyServerExt字段
    kNIMMessageTypeTips = 10,
    /// 波特机器人消息
    kNIMMessageTypeRobot = 11,
    /// G2话单消息
    kNIMMessageTypeG2NetCall = 12,
    /// 自定义消息
    kNIMMessageTypeCustom = 100,

    /// 未知类型消息，本地使用，发送时勿使用，作为默认值
    kNIMMessageTypeUnknown = 1000,
};

/** @enum NIMMessageFeature 消息种类 */
enum NIMMessageFeature {
    /// 默认
    kNIMMessageFeatureDefault = 0,
    /// 离线消息
    kNIMMessageFeatureLeaveMsg = 1,
    /// 漫游消息
    kNIMMessageFeatureRoamMsg = 2,
    /// 同步消息
    kNIMMessageFeatureSyncMsg = 3,
    /// 透传消息
    kNIMMessageFeatureCustomizedMsg = 4,
};

/** @enum NIMMsgLogStatus 消息状态 */
enum NIMMsgLogStatus {
    /// 默认,不能当查询条件,意义太多
    kNIMMsgLogStatusNone = 0,
    /// 收到消息,未读
    kNIMMsgLogStatusUnread = 1,
    /// 收到消息,已读
    kNIMMsgLogStatusRead = 2,
    /// 已删
    kNIMMsgLogStatusDeleted = 3,
    /// 发送中
    kNIMMsgLogStatusSending = 4,
    /// 发送失败
    kNIMMsgLogStatusSendFailed = 5,
    /// 已发送
    kNIMMsgLogStatusSent = 6,
    /// 对方已读发送的内容
    kNIMMsgLogStatusReceipt = 7,
    /// 草稿
    kNIMMsgLogStatusDraft = 8,
    /// 发送取消
    kNIMMsgLogStatusSendCancel = 9,
    /// 被对方拒绝,比如被对方加入黑名单等等
    kNIMMsgLogStatusRefused = 10,
};

/** @enum NIMNotificationId 通知类型 */
enum NIMNotificationId {
    /// 普通群拉人，{"ids":["a1", "a2"],"user_namecards":["namecard1", "namecard2"], "attach" : ""}
    kNIMNotificationIdTeamInvite = 0,
    /// 普通群踢人，{"ids":["a1", "a2"],"user_namecards":["namecard1", "namecard2"], "attach" : ""}
    kNIMNotificationIdTeamKick = 1,
    /// 退出群，{"id" : "a1","user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamLeave = 2,
    /// 群信息更新，{"team_info":team_info,"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamUpdate = 3,
    /// 群解散，{"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamDismiss = 4,
    /// 高级群申请加入成功，{"id":"a1","user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamApplyPass = 5,
    /// 高级群移交群主，{"id":"a1", "leave" : bool,"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamOwnerTransfer = 6,
    /// 增加管理员，{"ids":["a1","a2"],"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamAddManager = 7,
    /// 删除管理员，{"ids":["a1","a2"],"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamRemoveManager = 8,
    /// 高级群接受邀请进群，{"id":"a1","user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdTeamInviteAccept = 9,
    /// 禁言/解禁群成员，{"user_namecards":["namecard1", "namecard2"],"team_info":team_info,"id":"a1","mute":1-禁言,0-解禁}
    kNIMNotificationIdTeamMuteMember = 10,
    /// 未接电话,{"calltype":1,"channel":6146078138783760761,"from":"id1","ids":["id1","id2"],"time":1430995380471}
    kNIMNotificationIdNetcallMiss = 101,
    /// 话单,{"calltype":2,"channel":6146077129466446197,"duration":8,"ids":["id1","id2"],"time":1430995117398}
    kNIMNotificationIdNetcallBill = 102,
    /// 拒绝电话,{"calltype":1,"channel":6144978055925334000,"from":"id1","ids":["id1"],"time":139323423424}
    kNIMNotificationIdNetcallReject = 103,

    // 服务器在线同步协议返回的结果
    /// 创建群 {"team_info" : team_info} // 群组信息(Keys SEE MORE `nim_team_def.h` 『群组信息 Json Keys』)
    kNIMNotificationIdTeamSyncCreate = 1000,
    /// 群成员变更{"team_member" : team_member_info}
    kNIMNotificationIdTeamMemberChanged = 1001,
    /// 同步通知：修改群成员属性（可能是自己的或别人的)
    kNIMNotificationIdTeamSyncUpdateMemberProperty = 1002,

    // 本地发起的操作通知APP上层
    /// 本地操作创建群 {"ids" : ["a1", "a2"]}
    kNIMNotificationIdLocalCreateTeam = 2000,
    /// 本地操作申请加入群 {}
    kNIMNotificationIdLocalApplyTeam = 2001,
    /// 本地操作拒绝申请 {"id":"a1"}
    kNIMNotificationIdLocalRejectApply = 2002,
    /// 本地操作拒绝邀请 {"id":"a1"}
    kNIMNotificationIdLocalRejectInvite = 2003,
    /// 本地操作更新群成员属性  {"team_member" : team_member_info}
    kNIMNotificationIdLocalUpdateMemberProperty = 2004,
    /// 本地操作更新他人nickname {}
    kNIMNotificationIdLocalUpdateOtherNick = 2005,
    /// 本地操作获取群信息 {"team_info":team_info} // 群组信息(Keys SEE MORE `nim_team_def.h` 『群组信息 Json Keys』)
    kNIMNotificationIdLocalGetTeamInfo = 2006,
    /// 本地操作获取群成员信息结束
    kNIMNotificationIdLocalGetTeamList = 2007,
    /// 本地操作对群成员禁言 {"id":"a1", "mute":1-禁言,0-解禁}
    kNIMNotificationIdLocalMuteMember = 2008,
    /// 本地操作对群禁言 {}
    kNIMNotificationIdLocalMute = 2009,
    /// 获取群消息未读数 {[{"client_msg_id":"", "count":int, "read_accid":"当前已读成员的accid"},...]}
    kNIMNotificationIdLocalGetTeamMsgUnreadCount = 2010,
    /// 获取群消息未读列表 {"client_msg_id":"", "read":["id1",...], "unread":["id2",...]}
    kNIMNotificationIdLocalGetTeamMsgUnreadList = 2011,

    // Netcall本地操作通知
    /// 拒绝电话,{"calltype":1,"channel":6146078138783760761,"from":"id1","ids":["id1","id2"],"time":1430995380471}
    kNIMNotificationIdLocalNetcallReject = 3103,
    /// 无应答，未接通电话,{"calltype":1,"channel":6146078138783760761,"from":"id1","ids":["id1","id2"],"time":1430995380471}
    kNIMNotificationIdLocalNetcallNoResponse = 3104,
    /// 未接通前主叫方挂断，{"calltype":1,"channel":6146078138783760761,"from":"id1","ids":["id1","id2"],"time":1430995380471}
    kNIMNotificationIdLocalNetcallCanceled = 3105,
    /// 超大群拉人，{"ids":["a1", "a2"],"user_namecards":["namecard1", "namecard2"], "attach" : ""}
    kNIMNotificationIdSuperTeamInvite = 401,
    /// 超大群踢人，{"ids":["a1", "a2"],"user_namecards":["namecard1", "namecard2"], "attach" : ""}
    kNIMNotificationIdSuperTeamKick = 402,
    /// 退出超大群，{"id" : "a1","user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdSuperTeamLeave = 403,
    /// 超大群群信息更新，{"team_info":team_info,"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdSuperTeamUpdate = 404,
    /// 超大群解散，{"user_namecards":["namecard1", "namecard2"]}
    kNIMNotificationIdSuperTeamDismiss = 405,
    /// 超大群移交群主，{"id":"a1","uinfos":["uinfo1", "uinfo2"]}
    kNIMNotificationIdSuperTeamOwnerTransfer = 406,
    /// 超大群增加管理员，{"ids":["a1","a2"],"uinfos":["uinfo1", "uinfo2"]}
    kNIMNotificationIdSuperTeamAddManager = 407,
    /// 超大群删除管理员，{"ids":["a1","a2"],"uinfos":["uinfo1", "uinfo2"]}
    kNIMNotificationIdSuperTeamRemoveManager = 408,
    /// 超大群禁言/解禁群成员，{"uinfos":["uinfo1", "uinfo2"],“tinfo”:tinfo,"id":"a1","mute":1-禁言,0-解禁}
    kNIMNotificationIdSuperTeamMuteMember = 409,
    /// 超大群申请加入成功，{"tinfo":tinfo,"id":"a1","uinfos":["uinfo1", "uinfo2"]}
    kNIMNotificationIdSuperTeamApplyPass = 410,
    /// 超大群接受邀请进群，{"tinfo":tinfo,"id":"a1"}
    kNIMNotificationIdSuperTeamInviteAccept = 411
};

/** @enum NIMMsgLogSubStatus 消息子状态 */
enum NIMMsgLogSubStatus {
    /// 默认状态
    kNIMMsgLogSubStatusNone = 0,
    // 这二个标志适用于所有
    /// 未播放
    kNIMMsgLogSubStatusNotPlaying = 20,
    /// 已播放
    kNIMMsgLogSubStatusPlayed = 21,
};

/** @导出/导入云端备份操作类型 */
enum LogsBackupRemoteOperate {
    /// 导出
    LogsBackupRemoteOperate_Export = 0,
    /// 导入
    LogsBackupRemoteOperate_Import = 1
};
/** @导出/导入云端备份操作结果 */
enum LogsBackupRemoteState {
    /// 定义开始
    LogsBackupRemoteState_Begin = -2,
    /// 未定义
    LogsBackupRemoteState_UnDef,
    /// 已取消
    LogsBackupRemoteState_UserCanceled = 5,
    /// SDK 已出错
    LogsBackupRemoteState_SDKError,
    // 导入状态
    /// 没有备份文件
    LogsBackupRemoteState_IMP_NoBackup,
    /// 查询备份失败一般是网络错误
    LogsBackupRemoteState_IMP_SyncFromSrvError,
    /// 下载备份文件出错
    LogsBackupRemoteState_IMP_DownloadBackupFailed,
    /// 解密/解压出来的源文件格式错误
    LogsBackupRemoteState_IMP_RAWError,
    /// 解析源文件格式错误
    LogsBackupRemoteState_IMP_ParseRAWError,
    /// 导入本地DB出错
    LogsBackupRemoteState_IMP_LocalDBFailed,
    // 导出状态
    /// 打开本地DB失败
    LogsBackupRemoteState_EXP_LocalDBFailed,
    /// 导出到源文件失败
    LogsBackupRemoteState_EXP_RAWError,
    /// 上传备份文件出错
    LogsBackupRemoteState_EXP_UploadBackupFailed,
    /// 同步到服务器出错一般是网络错误
    LogsBackupRemoteState_EXP_SyncToSrvError,

    /// 完成
    LogsBackupRemoteState_Done,
    /// 完成，但未导出任何记录
    LogsBackupRemoteState_Done_NoLogs,
    /// 定义结束
    LogsBackupRemoteState_End,
    /// 是否已是最终状态的一个标识，可以判断state是否为终态(state >= LogsBackupRemoteState_FinalState_Begin)
    LogsBackupRemoteState_FinalState_Begin = LogsBackupRemoteState_UserCanceled,
};

/** @brief 全文检索排序规则 */
enum NIMFullTextSearchRule { kNIMFullTextSearchOrderByAsc = 1, kNIMFullTextSearchOrderByDesc = 2, kNIMFullTextSearchNoGroupBySession = 4 };

/** @brief 导出消息记录到云端参数定义 */
typedef struct {
    /** 自定义的打包方式回调，SDK生成原始数据文后会调用开发者自定义的打包（压缩）方法
     * @param[in] file_path        原始数据文件路径
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return char* 生成的打包（压缩）文件的路径
     */
    char* (*ExportBackupToRemotePackageCallback)(const char* const file_path, void* user_data);
    /** 自定义的加密方式回调，SDK生成原始数据经过打包（压缩）后会调用开发者自定义的加密方法
     * @param[in] file_path        打包（压缩）后的文件路径
     * @param[in] encrypt_key    加密秘钥 与 encrypt_key_为同一个值
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return char* 加密后的文件路径
     */
    char* (*ExportBackupToRemoteEncryptCallback)(const char* const file_path, const char* const encrypt_key, void* user_data);
    /// 加密秘钥
    char* encrypt_key_;
    /**导出过程的进度回调
     * @param[in] op        操作类型 导入/导出
     * @param[in] progress    进度 0-1
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return void
     */
    void (*LogsBackupProgressCallback)(enum LogsBackupRemoteOperate op, float progress, void* user_data);
    /** 导出完成后的结果回调
     * @param[in] op        操作类型 导入/导出
     * @param[in] state    完成状态参考 LogsBackupRemoteState
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return void
     */
    void (*LogsBackupCompleteCallback)(enum LogsBackupRemoteOperate op, enum LogsBackupRemoteState state, void* user_data);
    /**开发者自定义的导出消息的过滤器
     * @param[in] msg        消息的详细数据 json格式
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return bool true:导出这条消息;false:不导出这条消息
     */
    bool (*ExportBackupToRemoteLogFiter)(const char* const msg, void* user_data);
    /**在导出过程中生成的新路径（字符串）释放接口
     * @param[in] data_ptr    指向路径（字符串）的引用
     * @return void
     */
    void (*NewPathRelease)(char** data_ptr);
    /// APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
    void* user_data_;
} NIMLogsBackupExportInfo;
/** @brief从云端导入消息记录参数定义 */
typedef struct {
    /**自定义的解包方式回调，从云端步的备份文件经解密后会回调开发者自定义的解包（解压）方法
     * @param[in] file_path        经解密后文件路径
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return char* 解包后的文件路径
     */
    char* (*ImportBackupFromRemoteUnPackageCallback)(const char* const file_path, void* user_data);
    /**自定义的解密方式回调，SDK从云端同步完备份文件后会调用开发者自定义的解密方法
     * @param[in] file_path        从云端同步到的文件路径
     * @param[in] encrypt_key    解密秘钥 与导出时加密密钥相同
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return char* 解密后的文件路径
     */
    char* (*ImportBackupFromRemoteDecryptCallback)(const char* const file_path, const char* const encrypt_key, void* user_data);
    /** 导入过程的进度回调
     * @param[in] op        操作类型 导入/导出
     * @param[in] progress    进度 0-1
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return void
     */
    void (*LogsBackupProgressCallback)(enum LogsBackupRemoteOperate op, float progress, void* user_data);
    /**导入完成后的结果回调
     * @param[in] op        操作类型 导入/导出
     * @param[in] state     完成状态参考 LogsBackupRemoteState
     * @param[in] user_data        APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
     * @return void
     */
    void (*LogsBackupCompleteCallback)(enum LogsBackupRemoteOperate op, enum LogsBackupRemoteState state, void* user_data);
    /**在导入过程中生成的新路径（字符串）释放接口
     * @param[in] data_ptr    指向路径（字符串）的引用
     * @return bool true:导出这条消息;false:不导出这条消息
     */
    void (*NewPathRelease)(char**);
    /// APP的自定义用户数据，SDK只负责传回给回调函数，不做任何处理！
    void* user_data_;
} NIMLogsBackupImportInfo;

/** @name 接口nim_msglog_query_msg_async扩展参数json object key定义
 * @{
 */
/// NIMMsglogSearchDirection，默认为kForward
static const char* kNIMMsglogQueryJsonExtensionKeyDirection = "direction";
/// bool，返回的消息历史排序正序(false)/逆序(true),默认为false
static const char* kNIMMsglogQueryJsonExtensionKeyReverse = "reverse";
/// int64_t，查询消息的截止时间，如果direction为kForward，则截止时间应小于anchor_msg_time，否则大于anchor_msg_time,默认为0代表不限制截止时间
static const char* kNIMMsglogQueryJsonExtensionKeyEndTime = "endtime";
/** @}*/  // 接口nim_msglog_query_msg_async扩展参数json key定义

/** @name 接口nim_msglog_query_msg_online_async扩展参数json_extension key定义
 * @{
 */
/// bool 查询结果回来后，是否需要sdk自动下载消息附件。true：需要，false：不需要
static const char* kNIMMsglogJsonExtKeyNeedAutoDownloadAttachment = "need_auto_download_attachment";
/// array 要查询的消息类型
static const char* kNIMMsglogJsonExtKeyQueryMsgTypeList = "query_msg_type_list";
/** @}*/  // 接口nim_msglog_query_msg_online_async扩展参数json_extension key定义

/** @name 接口 nim_msglog_insert_msglog_async 扩展参数 json_extension key 定义
 * @{
 */
/// bool 插入新消息若需要更新会话数据，指定是否需要绑定历史消息中的最后一条消息。true：需要，false：不需要
static const char* kNIMMsglogJsonExtKeyComposeLastMsg = "compose_last_msg";
/// array 绑定的消息要排除的类型，始终排除掉未知类型（1000）消息
static const char* kNIMMsglogJsonExtKeyExcludeMsgType = "exclude_msg_type";
/** @}*/  // 接口 nim_msglog_insert_msglog_async 扩展参数 json_extension key 定义

/** @enum NIMMsglogSearchDirection 消息历史查询方向 */
enum NIMMsglogSearchDirection {
    /// 以结束时间点为准向前搜索
    kForward = 0,
    /// 以起始时间点为准向后搜索
    kBackward = 1,
};

/** @enum NIMMsgLogQueryRange 消息历史的检索范围 */
enum NIMMsgLogQueryRange {
    /// 指定的个人（点对点会话）（注意：暂不支持指定多个人的检索！）
    kNIMMsgLogQueryRangeP2P = kNIMSessionTypeP2P,
    /// 指定的群组（注意：暂不支持指定多个群组的检索！）
    kNIMMsgLogQueryRangeTeam = kNIMSessionTypeTeam,
    /// 指定的超大群组（注意：暂不支持指定多个群组的检索！）
    kNIMMsgLogQueryRangeSuperTeam = kNIMSessionTypeSuperTeam,
    /// 全部
    kNIMMsgLogQueryRangeAll = 100,
    /// 所有个人会话
    kNIMMsgLogQueryRangeAllP2P = 101,
    /// 所有群组
    kNIMMsgLogQueryRangeAllTeam = 102,
    /// 所有超大群组
    kNIMMsgLogQueryRangeAllSuperTeam = 103,
    /// 未知（如指定个人和群组的集合）（注意：暂不支持指定个人和群组的混合检索！）
    kNIMMsgLogQueryRangeUnknown = 200,
};

/** @brief从云端导入消息记录参数定义 */
typedef struct {
    /// enum 会话类型，双人0，群组1 (nim_msglog_def.h)
    enum NIMSessionType to_type_;
    /// int 本次查询的消息条数上限(最多100条)
    int limit_count_;
    /// int64_t 起始时间点，单位：毫秒
    int64_t from_time_;
    /// int64_t 结束时间点，单位：毫秒
    int64_t end_time_;
    /// int64_t 结束查询的最后一条消息的server_msg_id(不包含在查询结果中)
    int64_t end_msg_id_;
    /// bool true：反向查询(按时间正序起查，正序排列)，false：按时间逆序起查，逆序排列（建议默认为false）
    bool reverse_;
    /// bool 将在线查询结果保存到本地，false: 不保存
    bool need_save_to_local_;
    /// bool 查询结果回来后，是否需要sdk自动下载消息附件。true：需要，false：不需要
    bool auto_download_attachment_;
    /// list 要获取或排除掉的消息类型 由 is_exclusion_type_ 参数决定
    enum NIMMessageType* msg_type_list_;
    /// int type list 长度
    int type_list_length_;
    /// bool true : 获取除msg_type_list_中指定的所有类型消息 ,false :只获取 msg_type_list_ 中指定的类型的消息
    bool is_exclusion_type_;
} NIMQueryMsgOnlineAsyncParam;

/** @brief根据关键字村云端查询消息记录的参数定义 */
typedef struct {
    /// enum 会话类型，双人0，群组1 (nim_msglog_def.h)
    enum NIMSessionType to_type_;
    /// int 本次查询的消息条数上限(最多100条)
    int limit;
    /// int64_t 起始时间点，单位：毫秒
    int64_t from_time_;
    /// int64_t 结束时间点，单位：毫秒
    int64_t end_time_;
    /// bool true：反向查询(按时间正序起查，正序排列)，false：按时间逆序起查，逆序排列（建议默认为false）
    bool reverse_;
} NIMQueryMsgByKeywordOnlineAsyncParam;

/** @name 群组通知 Json Keys
 * for example:
 * {"id": 1,
 *  "data": ***
 * }
 * @{
 */
/// int, 见NIMNotificationId
static const char* kNIMNotificationKeyId = "id";
/// json object 包含以下5种可能的数据结构
static const char* kNIMNotificationKeyData = "data";
/// string array
static const char* kNIMNotificationKeyDataIds = "ids";
/// string array
static const char* kNIMNotificationKeyDataInvalidIds = "invalid_ids";
/// string
static const char* kNIMNotificationKeyDataId = "id";
/// bool
static const char* kNIMNotificationKeyDataLeave = "leave";
/// int
static const char* kNIMNotificationKeyDataMute = "mute";
/// string, team_info 群组信息 Json Keys
static const char* kNIMNotificationKeyTeamInfo = "team_info";
/// string, team_member_property 群组成员信息 Json Keys
static const char* kNIMNotificationKeyTeamMember = "team_member";
/// json string array, 操作者和被操作者双方的 用户名片 Json Keys
static const char* kNIMNotificationKeyUserNameCards = "name_cards";
/// string array, 应用自定义扩展字段
static const char* kNIMNotificationKeyAttach = "attach";
/** @}*/  // 群组通知 Json Keys

/** @name
 * kNIMNotificationIdNetcallBill/kNIMNotificationIdNetcallMiss/kNIMNotificationIdNetcallReject/kNIMNotificationIdLocalNetcallReject/kNIMNotificationIdLocalNetcallNoResponse
 * Data Keys
 * @{
 */
/// int 通话类型对应NIMVideoChatMode
static const char* kNIMNotificationIdNetCallTypeKey = "calltype";
/// int64 时间 单位毫秒
static const char* kNIMNotificationIdNetCallTimeKey = "time";
/// StrArray 帐号
static const char* kNIMNotificationIdNetCallIdsKey = "ids";
/// int64 通道id
static const char* kNIMNotificationIdNetCallChannelKey = "channel";
/// int64 通话时长 单位秒
static const char* kNIMNotificationIdNetCallDurationKey = "duration";
/// string 发起者帐号
static const char* kNIMNotificationIdNetCallFromKey = "from";
/// string 会发起时传入的 custominfo参数
static const char* kNIMNotificationIdNetCallEXTKey = "ext";
/** @}*/  // kNIMNotificationIdNetcallBill
          // kNIMNotificationIdNetcallMiss
          // kNIMNotificationIdNetcallReject
          // kNIMNotificationIdLocalNetcallReject
          // kNIMNotificationIdLocalNetcallNoResponse
          // Data Keys

/** @name 单向删除某条消息服务端推送 Json Keys
 * @{
 */
/// string, 会话 ID
static const char* kNIMDELMSGSelfNotifyKeySessionID = "session_id";
/// string 消息ID
static const char* kNIMDELMSGSelfNotifyKeyMsgClientID = "client_id";
/// string 用户自定义数据
static const char* kNIMDELMSGSelfNotifyKeyEXT = "ext";
/** @}*/  // 单向删除某条消息服务端推送 Json Keys

/** @name 群组消息标记已读回调 json key
 * @{
 */
/// string json array 群组消息标记已读成功的msg id list key
static const char* kNIMTEAMMSGACKREADSUCCESSIDSKEY = "success_ids";
/// string json array 群组消息标记已读失败的msg id list key
static const char* kNIMTEAMMSGACKREADFAILUREIDSKEY = "failure_ids";
/// string json array 群组消息标记已读忽略的msg id list key(消息不需要标记，或者已经标记过)
static const char* kNIMTEAMMSGACKREADSIGNOREDIDSKEY = "ignored_ids";
/** @}*/  // 单向删除某条消息参数 Json Keys

/** @brief 单向删除某条消息参数 */
typedef struct {
    /// enum 会话类型，双人0，群组1,超大群5 (nim_msglog_def.h)
    enum NIMSessionType to_type_;
    /// string 消息的发送方
    char from_account[128];
    /// string 消息的接收方
    char to_account[128];
    /// int64_t 消息的服务端id
    int64_t server_id;
    /// string 消息的客户端id
    char client_id[128];
    /// int64_t 消息时间戳
    int64_t time;
    /// string 扩展字段
    char ext[1024];
} NIMDeleteMessagesSelfParam;

/** @brief 云端查询某条消息参数一般用在thread聊天场景中 */
typedef struct {
    /// enum 会话类型，双人0，群组1,超大群5 (nim_msglog_def.h)
    enum NIMSessionType to_type_;
    /// string 消息的发送方
    char from_account[128];
    /// string 消息的接收方
    char to_account[128];
    /// int64_t 消息的服务端id
    int64_t server_id;
    /// string 消息的客户端id
    char client_id[128];
    /// int64_t 消息时间戳
    int64_t time;
} NIMQueryMsgAsyncParam;

/** @brief 云端查询thread聊天历史参数 */
typedef struct {
    /// int64_t 起始时间 缺省0
    int64_t from_time;
    /// int64_t 结束时间 缺省0
    int64_t to_time;
    /// int64_t 截至消息的服务端id，不包含在查询结果中 缺省0
    int64_t exclude_msg_id;
    /// int32_t 查询条数限制 缺省100
    int32_t limit;
    /// int64_t 排序 缺省0 false
    int32_t reverse;
} NIMQueryThreadHistoryMsgAsyncParam;

#ifdef __cplusplus
};
#endif  //__cplusplus
#endif  // NIM_SDK_DLL_EXPORT_HEADERS_MSGLOG_DEF_H_
