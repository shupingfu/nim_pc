/**
 * @file nim_qchat_public_def.h
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright (c) 2022, NetEase Inc. All rights reserved
 *
 */
#ifndef __NIM_QCHAT_PUBLIC_DEF_H__
#define __NIM_QCHAT_PUBLIC_DEF_H__

#include <stddef.h>
#include <cstdint>
#include "public_defines.h"

#ifdef __cplusplus
extern "C" {
#endif
/** @enum NIMQChatChannelViewMode 圈组频道查看模式 */
enum NIMQChatChannelViewMode {
    /// 公开模式
    kNIMQChatChannelViewModePublic = 0,
    /// 私密模式
    kNIMQChatChannelViewModePrivate = 1,
};

/** @enum NIMQChatChannelVisitorMode 圈组频道游客访问模式 */
enum NIMQChatChannelVisitorMode {
    /// 游客可见
    kNIMQChatChannelVisitorModeVisible = 0,
    /// 游客不可见
    kNIMQChatChannelVisitorModeInvisible = 1,
    /// 跟随模式, 频道公开游客可见, 频道私密游客不可见
    kNIMQChatChannelVisitorModeFollow = 2,
};

/// @enum NIMQChatChannelSyncMode
enum NIMQChatChannelSyncMode {
    /// 不与频道分组同步
    kNIMQChatChannelSyncModeNoSync = 0,
    /// 与频道分组同步
    kNIMQChatChannelSyncModeSync = 1,
};

/** @enum NIMQChatSubscribeOpeType 圈组订阅操作类型 */
enum NIMQChatSubscribeOpeType {
    /// 订阅
    kNIMQChatSubscribeOpeTypeSubscribe = 1,
    /// 取消订阅
    kNIMQChatSubscribeOpeTypeUnsubscribe = 2,
};

/** @enum NIMQChatSubscribeType 圈组订阅类型 */
enum NIMQChatSubscribeType {
    /// 未知
    kNIMQChatSubscribeTypeUnknow = 0,
    /// 订阅某个channel的【消息】/【通知】
    kNIMQChatSubscribeTypeMsg = 1,
    /// 订阅某个channel的【消息未读数】/【通知】
    kNIMQChatSubscribeTypeUnreadCount = 2,
    /// 订阅某个channel的【消息未读状态】/【通知】
    kNIMQChatSubscribeTypeUnreadStatus = 3,
    /// 订阅某个server的【消息】/【通知】
    kNIMQChatSubscribeTypeServerNotification = 4,
    /// 订阅某个channel的【正在输入事件】
    kNIMQChatSubscribeTypeTypingEvent = 5,
    /// 以游客身份订阅某个channel的【消息】/【通知】
    kNIMQChatSubscribeTypeVisitorMsg = 6,
    /// 以游客身份订阅某个server的【消息】/【通知】
    kNIMQChatSubscribeTypeVisitorServerNotification = 7,
};

static inline bool NIMQChatSubscribeTypeIsServer(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeServerNotification;
};

static inline bool NIMQChatSubscribeTypeIsChannel(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeMsg || type == kNIMQChatSubscribeTypeUnreadCount || type == kNIMQChatSubscribeTypeUnreadStatus ||
           type == kNIMQChatSubscribeTypeTypingEvent;
};

static inline bool NIMQChatSubscribeTypeIsVisitor(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeVisitorMsg || type == kNIMQChatSubscribeTypeVisitorServerNotification;
};

static inline bool NIMQChatSubscribeTypeIsServerVisitor(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeVisitorServerNotification;
};

static inline bool NIMQChatSubscribeTypeIsChannelVisitor(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeVisitorMsg;
};

static inline bool NIMQChatSubscribeTypeIsMsg(NIMQChatSubscribeType type) {
    return type == kNIMQChatSubscribeTypeMsg || type == kNIMQChatSubscribeTypeUnreadCount || type == kNIMQChatSubscribeTypeUnreadStatus;
};

/// @enum NIMQChatBanMemberOpeType 圈组封禁操作类型
enum NIMQChatBanMemberOpeType {
    /// 封禁
    kNIMQChatBanMemberOpeTypeBan = 1,
    /// 解除封禁
    kNIMQChatBanMemberOpeTypeUnban = 2,
};

/// @enum NIMQChatSearchOrder 搜索排序顺序
enum NIMQChatSearchOrder {
    /// 按时间正序
    kNIMQChatSearchOrderAsc = 1,
    /// 按时间倒序
    kNIMQChatSearchOrderDesc = 2,
};

/// @enum NIMQChatServerSearchSort 服务器搜索排序规则
enum NIMQChatServerSearchSort {
    /// 自定义权重排序(详见服务器端文档)
    kNIMQChatServerSearchSortCustom = 0,
    /// 创建时间排序
    kNIMQChatServerSearchSortCreateTime = 1,
    /// 服务器总人数排序
    kNIMQChatServerSearchSortMemberCOunt = 2,
};

/// @enum NIMQChatChannelSearchSort 频道搜索排序规则
enum NIMQChatChannelSearchSort {
    /// 自定义权重排序(详见服务器端文档)
    kNIMQChatChannelSearchSortCustom = 0,
    /// 创建时间排序
    kNIMQChatChannelSearchSortCreateTime = 1,
};

/// @enum NIMQChatMessageSearchSort 消息搜索排序规则
enum NIMQChatMessageSearchSort {
    /// 创建时间排序
    kNIMQChatMessageSearchSortCreateTime = 1,
};

/// @struct NIMQChatBaseResp
struct NIMQChatBaseResp {
    /// 操作结果, 参考NIMResCode
    uint32_t res_code;
    /// 自定义用户数据
    void* user_data;
};

/// @struct NIMQChatBusinessAntiSpamInfo
struct NIMQChatBusinessAntiSpamInfo {
    /// 反垃圾文本业务id
    char* text_bid;
    /// 反垃圾图片业务id
    char* pic_bid;
};

/** @struct NIMQChatChannelIdInfo 圈组频道ID信息 */
struct NIMQChatChannelIDInfo {
    /// server id
    uint64_t server_id;
    /// channel id
    uint64_t channel_id;
};

/// @struct NIMQChatMessageUpdateInfo
struct NIMQChatMessageUpdateInfo {
    /// 操作附言
    char* postscript;
    /// 操作扩展字段
    char* extension;
    /// 推送文案
    char* push_content;
    /// 推送payload
    char* push_payload;
    /// 环境变量，用于指向不同的抄送、第三方回调等配置
    char* env;
    /// 是否需要抄送
    bool route_enable;
    // 以下字段仅在收到消息时有效
    /// 操作者账号
    char* operator_id;
    /// 操作者客户端类型
    uint32_t operator_client_type;
};

/// @struct NIMQChatMessageAntiSpamInfo
struct NIMQChatMessageAntiSpamInfo {
    /// 是否需要使用自定义反垃圾字段(即antiSpamContent), false:不需要, true:需要, 默认0
    bool use_custom_content;
    /// 单条消息是否使用易盾反垃圾, false:(在开通易盾的情况下)不过易盾反垃圾, 其他都是按照原来的规则
    bool anti_spam_using_yidun;
    /// 反垃圾内容, 开发者自定义的反垃圾字段, 如果提供了这个字段, 在过反垃圾时不会再使用body或者attach
    char* anti_spam_content;
    /// 反垃圾业务ID, 对某些单条消息另外的反垃圾的业务ID
    char* anti_spam_bussiness_id;
    /// 易盾check的回调URL
    char* yidun_callback_url;
    /// 易盾反垃圾增强反作弊专属字段, 限制json, 长度限制1024
    char* yidun_anti_cheating;
    /// 易盾反垃圾扩展字段, 限制json, 长度限制1024
    char* yidun_anti_spam_ext;
};

/// @struct NIMQChatPageInfo 圈组分页查询信息
struct NIMQChatPageInfo {
    /// 是否还有更多
    bool has_more;
    /// 下一次分页起始时间
    uint64_t next_timestamp;
    /// 下一次分页起始游标
    char* cursor;
};

/// @struct NIMQChatUnreadInfo 圈组频道未读信息
struct NIMQChatUnreadInfo {
    /// 服务器ID
    uint64_t server_id;
    /// 频道ID
    uint64_t channel_id;
    /// 已读时间戳
    uint64_t ack_timestamp;
    /// 未读数
    uint64_t unread_count;
    /// 被@未读数
    uint64_t mentioned_unread_count;
    /// 订阅未读数时的最大未读数
    uint64_t max_unread_count;
    /// 最后一条消息时间戳
    uint64_t last_msg_timestamp;
};

/// @struct NIMQChatServerUnreadInfo 圈组服务器未读信息
struct NIMQChatServerUnreadInfo {
    /// 服务器ID
    uint64_t server_id;
    /// 未读数
    uint64_t unread_count;
    /// 被@未读数
    uint64_t mentioned_unread_count;
    /// 订阅未读数时的最大未读数
    uint64_t max_unread_count;
};

#ifdef __cplusplus
}
#endif

#endif  // __NIM_QCHAT_PUBLIC_DEF_H__
