/** @file nim_chatroom_cpp.h
 * @brief 聊天室SDK
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @date 2015/12/29
 */

#ifndef _NIM_CHATROOM_SDK_CPP_H_
#define _NIM_CHATROOM_SDK_CPP_H_

#include <functional>
#include <list>
#include <string>
#include "nim_chatroom_cpp_wrapper/helper/nim_chatroom_helper.h"
#include "nim_chatroom_cpp_wrapper/nim_chatroom_sdk_cpp_wrapper.h"

/**
 * @namespace nim_chatroom
 * @brief 聊天室
 */
namespace nim_chatroom {
/** @class ChatRoom
 * @brief 聊天室
 */
class NIM_CHATROOM_SDK_CPPWRAPPER_DLL_API ChatRoom {
public:
    typedef std::function<void(const std::function<void()>&)> SDKClosure;
    /// 登录回调, 如果错误码为kResRoomLocalNeedRequestAgain，聊天室重连机制结束，则需要向IM服务器重新请求进入该聊天室权限
    typedef std::function<
        void(int64_t room_id, const NIMChatRoomEnterStep step, int error_code, const ChatRoomInfo& info, const ChatRoomMemberInfo& my_info)>
        EnterCallback;
    /// 登出、被踢回调
    typedef std::function<void(int64_t room_id, int error_code, NIMChatRoomExitReason exit_reason)> ExitCallback;
    /// 登出、被踢回调
    typedef std::function<void(int64_t room_id, int error_code, const NIMChatRoomExitReasonInfo& exit_info)> ExitCallback_2;
    /// 发送消息回执
    typedef std::function<void(int64_t room_id, int error_code, const ChatRoomMessage& result)> SendMsgAckCallback;
    /// 接收消息回调
    typedef std::function<void(int64_t room_id, const ChatRoomMessage& result)> ReceiveMsgCallback;
    /// 接收批量消息回调
    typedef std::function<void(int64_t room_id, const std::list<ChatRoomMessage>& result)> ReceiveMsgsCallback;
    /// 通知回调
    typedef std::function<void(int64_t room_id, const ChatRoomNotification& notification)> NotificationCallback;
    /// 在线查询成员列表回调
    typedef std::function<void(int64_t room_id, int error_code, const std::list<ChatRoomMemberInfo>& infos)> GetMembersCallback;
    /// 在线查询成员列表回调
    typedef std::function<void(int64_t room_id, int error_code, uint64_t)> GetMembersCountByTagCallback;
    /// 在线查询消息历史回调
    typedef std::function<void(int64_t room_id, int error_code, const std::list<ChatRoomMessage>& msgs)> GetMsgHistoryCallback;
    /// 设置成员身份标识回调
    typedef std::function<void(int64_t room_id, int error_code, const ChatRoomMemberInfo& info)> SetMemberAttributeCallback;
    /// 获取当前聊天室信息回调
    typedef std::function<void(int64_t room_id, int error_code, const ChatRoomInfo& info)> GetChatRoomInfoCallback;
    /// 踢掉指定成员回调
    typedef std::function<void(int64_t room_id, int error_code)> KickMemberCallback;
    /// 服务连接情况回调
    typedef std::function<void(int64_t room_id, const NIMChatRoomLinkCondition condition)> LinkConditionCallback;
    /// 自己的聊天室 tags 信息变更回调
    typedef std::function<void(int64_t room_id, const std::string& new_tag)> TagsChangedCallback;
    /// 获取自定义token回调
    typedef std::function<void(std::string& token)> CustomTokenCallback;
    /// 临时禁言/解禁回调
    typedef SetMemberAttributeCallback TempMuteMemberCallback;
    /// 更新聊天室信息回调
    typedef KickMemberCallback UpdateRoomInfoCallback;
    /// 更新我的信息回调
    typedef KickMemberCallback UpdateMyRoomRoleCallback;
    /// 更新聊天室中用户的位置信息回调
    typedef KickMemberCallback UpdateLocationCallback;
    /// 更新聊天室标签回调
    typedef KickMemberCallback UpdateTagsCallback;
    /// 新加(更新)麦序队列元素回调
    typedef KickMemberCallback QueueOfferCallback;
    /// 取出麦序队列元素回调
    typedef std::function<void(int64_t room_id, int error_code, const ChatRoomQueueElement& element)> QueuePollCallback;
    /// 排序列出麦序队列所有元素回调
    typedef std::function<void(int64_t room_id, int error_code, const ChatRoomQueue& queue)> QueueListCallback;
    /// 删除麦序队列元素回调
    typedef KickMemberCallback QueueDropCallback;
    /// 查看麦序队列头元素回调
    typedef QueuePollCallback QueueHeaderCallback;
    /// 获取机器人信息事件通知回调模板
    typedef std::function<void(int rescode, const RobotInfos& infos)> RobotQueryCallback;
    /// 队列批量更新回调
    typedef std::function<void(int64_t room_id, int error_code, const std::list<std::string>& not_in_queue)> QueueBatchUpdateCallback;
    /// SDK系统日志回调模板
    typedef std::function<void(const std::string& text)> SDKLogCallback;

public:
    /** @fn void RegEnterCb(const EnterCallback& cb, const std::string& json_extension = "")
     * 注册全局登录回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegEnterCb(const EnterCallback& cb, const std::string& json_extension = "");

    /** @fn void RegExitCb(const ExitCallback& cb, const std::string& json_extension = "")
     * 注册全局登出、被踢回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegExitCb(const ExitCallback& cb, const std::string& json_extension = "");

    /** @fn void RegExitCb_2(const ExitCallback_2& cb, const std::string& json_extension = "")
     * 注册全局登出、被踢回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegExitCb_2(const ExitCallback_2& cb, const std::string& json_extension = "");

    /** @fn void RegSendMsgAckCb(const SendMsgAckCallback& cb, const std::string& json_extension = "")
     * 注册全局发送消息回执回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegSendMsgAckCb(const SendMsgAckCallback& cb, const std::string& json_extension = "");

    /** @fn void RegReceiveMsgCb(const ReceiveMsgCallback& cb, const std::string& json_extension = "")
     * 注册全局接收消息回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegReceiveMsgCb(const ReceiveMsgCallback& cb, const std::string& json_extension = "");

    /** @fn void RegReceiveMsgsCb(const ReceiveMsgsCallback& cb, const std::string& json_extension = "")
     * 注册全局接收批量消息回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegReceiveMsgsCb(const ReceiveMsgsCallback& cb, const std::string& json_extension = "");

    /** @fn void RegNotificationCb(const NotificationCallback& cb, const std::string& json_extension = "")
     * 注册全局接收通知回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegNotificationCb(const NotificationCallback& cb, const std::string& json_extension = "");

    /** @fn void RegLinkConditionCb(const LinkConditionCallback& cb, const std::string& json_extension = "")
     * 注册全局聊天室链接情况回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegLinkConditionCb(const LinkConditionCallback& cb, const std::string& json_extension = "");

    static void RegTagsChangedCb(const TagsChangedCallback& cb, const std::string& json_extension = "");

    /** @fn void RegCustomTokenCb(const CustomTokenCallback& cb, const std::string& json_extension = "");
     * 注册全局聊天室自定义token获取回调
     * @param[in] cb              回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void RegCustomTokenCb(const CustomTokenCallback& cb, const std::string& json_extension = "");

    /** @fn static void RegSDKLogCallback(const SDKLogCallback& callback)
     * SDK系统日志回调，可用于动态获取日志，不建议用于生产环境
     * @param[in] callback 回调函数
     * @return
     */
    static void RegSDKLogCallback(const SDKLogCallback& callback);

    /** @fn void Init(const std::string& app_install_dir, const std::string& json_extension = "")
     * 聊天室模块初始化(SDK初始化时调用一次)
     * @param[in] app_install_dir Deprecated
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return bool 模块加载结果
     */
    static bool Init(const std::string& app_install_dir, const std::string& json_extension = "");

#ifdef CPPWRAPPER_DLL
    /** @fn void SetCallbackFunction(const ChatRoom::SDKClosure& callback)
     * 当以动态库使用SDK时 设置SDK回调方法，为了不阻塞SDK线程，在回调中应该把任务抛到应用层的线程中
     * @param[in] callback      回调方法
     * @return void 无返回值
     */
    static void SetCallbackFunction(const ChatRoom::SDKClosure& callback);
#endif

    /** @fn void Cleanup(const std::string& json_extension = "")
     * 聊天室模块清理(SDK卸载前调用一次)
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void Cleanup(const std::string& json_extension = "");

    /** @fn bool IndependentEnter(const int64_t room_id,const ChatRoomIndependentEnterInfo& info)
     * 聊天室独立进入
     * @param[in] room_id              聊天室ID
     * @param[in] info      聊天室进入信息
     * @return bool 进入信息是否正确,返回失败则不会触发进入回调
     */
    static bool IndependentEnter(const int64_t room_id, const ChatRoomIndependentEnterInfo& info);

    /** @fn bool IndependentEnter2(const int64_t room_id,const ChatRoomIndependentEnterInfo& info)
     * 聊天室独立进入
     * @param[in] room_id              聊天室ID
     * @param[in] info      聊天室进入信息
     * @param[in] config_getter      聊天室配置对象
     * @return bool 进入信息是否正确,返回失败则不会触发进入回调
     */
    static bool IndependentEnter2(const int64_t room_id, const ChatRoomIndependentEnterInfo& info, const NIMChatRoomConfigGetter config_getter);

    /** @fn bool AnonymousEnter(const int64_t room_id, const ChatRoomAnoymityEnterInfo& anonymity_info, const ChatRoomEnterInfo& info, const
     * std::string& json_extension = "")
     * 聊天室匿名进入
     * @param[in] room_id              聊天室ID
     * @param[in] anonymity_info      匿名登录相关信息
     * @param[in] info          聊天室进入信息
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return bool 进入信息是否正确,返回失败则不会触发进入回调
     */
    static bool AnonymousEnter(const int64_t room_id,
        const ChatRoomAnoymityEnterInfo& anonymity_info,
        const ChatRoomEnterInfo& info,
        const std::string& json_extension = "");

    /** @fn bool AnonymousEnter2(const int64_t room_id, const ChatRoomAnoymityEnterInfo& anonymity_info, const ChatRoomEnterInfo& info, const
     NIMChatRoomConfigGetter config_getter)
     * 聊天室匿名进入
     * @param[in] room_id              聊天室ID
     * @param[in] anonymity_info      匿名登录相关信息
     * @param[in] info          聊天室进入信息
     * @param[in] config_getter      聊天室配置对象
     * @return bool 进入信息是否正确,返回失败则不会触发进入回调
     */
    static bool AnonymousEnter2(const int64_t room_id,
        const ChatRoomAnoymityEnterInfo& anonymity_info,
        const ChatRoomEnterInfo& info,
        const NIMChatRoomConfigGetter config_getter);

    /** @fn bool Enter(const int64_t room_id, const std::string& request_login_data, const ChatRoomEnterInfo& info = ChatRoomEnterInfo(), const
     std::string& json_extension = "")
     * 聊天室登录
     * @param[in] room_id              聊天室ID
     * @param[in] request_login_data 聊天室登录信息(NIM SDK请求聊天室返回的数据)
     * @param[in] info                  聊天室可选信息
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return bool 登录信息是否正确,返回失败则不会促发登录回调
     */
    static bool Enter(const int64_t room_id,
        const std::string& request_login_data,
        const ChatRoomEnterInfo& info = ChatRoomEnterInfo(),
        const std::string& json_extension = "");

    /** @fn void Exit(const int64_t room_id, const std::string& json_extension = "")
     * 聊天室登出
     * @param[in] room_id              聊天室ID
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void Exit(const int64_t room_id, const std::string& json_extension = "");

    /** @fn NIMChatRoomLoginState GetLoginState(const int64_t room_id, const std::string& json_extension = "")
     * 获取聊天室登录状态
     * @param[in] room_id              聊天室ID
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static NIMChatRoomLoginState GetLoginState(const int64_t room_id, const std::string& json_extension = "");

    /** @fn void SetMsgsBatchReport(bool set_batch, const std::string& json_extension = "")
     * 设置消息接收批量上报开关
     * @param[in] set_batch 批量开关
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void SetMsgsBatchReport(bool set_batch, const std::string& json_extension = "");

    /** @fn void SendMsg(const int64_t room_id, const std::string& json_msg, const std::string& json_extension = "")
     * 发送消息
     * @param[in] room_id              聊天室ID
     * @param[in] json_msg              消息json string(可以通过 CreateRoomMessage 生成)
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void SendMsg(const int64_t room_id, const std::string& json_msg, const std::string& json_extension = "");

    /** @fn void SendMsg(const int64_t room_id, const ChatRoomMessage& message, const std::string& json_extension = "")
     * 发送消息
     * @param[in] room_id              聊天室ID
     * @param[in] message              消息
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void SendMsg(const int64_t room_id, const ChatRoomMessage& message, const std::string& json_extension = "");

    /** @fn std::string CreateRoomMessage(const NIMChatRoomMsgType msg_type, const std::string& client_msg_id, const std::string& attach, const
     ChatRoomMessageSetting& msg_setting, int64_t timetag = 0)
     * 生成消息内容（所有支持的消息类型的内容）
     * @param[in] msg_type          消息类型
     * @param[in] client_msg_id     消息ID
     * @param[in] attach            消息内容（包含多媒体的消息类型，此处传入的是约定的可以解析为json的非格式化的字符串，如图片、文件消息等）
     * @param[in] msg_body          文本消息内容
     * @param[in] msg_setting       消息属性
     * @param[in] timetag           消息时间
     * @param[in] sub_type          消息子类型( > 0 ),由开发者自定义,服务端只负责透传 v7.8
     * @param[in] target_tags
     * 表示本条消息的目标标签（是一个标签表达式），若缺失，则使用发送者登录时设置的notifyTargetTags，若依然缺失，则使用发送者登录时设置的tags
     * @return std::string 消息json string
     */
    static std::string CreateRoomMessage(const NIMChatRoomMsgType msg_type,
        const std::string& client_msg_id,
        const std::string& attach,
        const std::string& msg_body,
        const ChatRoomMessageSetting& msg_setting,
        int64_t timetag = 0,
        int32_t msg_sub_type = 0,
        const std::string& target_tags = "");

    /** @fn void GetMembersOnlineAsync(const int64_t room_id, const ChatRoomGetMembersParameters &parameters, const GetMembersCallback &callback,
     const std::string& json_extension = "")
     * 异步查询成员列表
     * @param[in] room_id              聊天室ID
     * @param[in] parameters          查询参数
     * @param[in] callback              回调函数
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void GetMembersOnlineAsync(const int64_t room_id,
        const ChatRoomGetMembersParameters& parameters,
        const GetMembersCallback& callback,
        const std::string& json_extension = "");

    /** @fn void GetMembersByTagOnlineAsync(const int64_t room_id, const ChatRoomGetMembersByTagParameters& parameters, const GetMembersCallback&
     callback, const std::string& json_extension = "")
     * @brief 根据 tag 异步查询聊天室成员列表
     * @param room_id               聊天室ID
     * @param parameters            查询参数
     * @param callback              回调函数
     * @param json_extension        json扩展参数（备用，目前不需要）
     */
    static void GetMembersByTagOnlineAsync(const int64_t room_id,
        const ChatRoomGetMembersByTagParameters& parameters,
        const GetMembersCallback& callback,
        const std::string& json_extension = "");

    /** @fn void GetMembersCountByTagOnlineAsync(const int64_t room_id, const std::string& target_tag, const GetMembersCountByTagCallback& callback,
     const std::string& json_extension)
     * @brief 获取聊天室内指定 tag 的成员数量
     * @param room_id           聊天室ID
     * @param target_tag        要查询的成员的 tag
     * @param callback          回调函数
     * @param json_extension    json扩展参数（备用，目前不需要）
     */
    static void GetMembersCountByTagOnlineAsync(const int64_t room_id,
        const std::string& target_tag,
        const GetMembersCountByTagCallback& callback,
        const std::string& json_extension);

    /** @fn void GetMessageHistoryOnlineAsync(const int64_t room_id, const ChatRoomGetMembersParameters &parameters, const GetMembersCallback
     &callback, const std::string& json_extension = "")
     * 异步查询消息历史
     * @param[in] room_id              聊天室ID
     * @param[in] parameters          查询参数
     * @param[in] callback              回调函数
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void GetMessageHistoryOnlineAsync(const int64_t room_id,
        const ChatRoomGetMsgHistoryParameters& parameters,
        const GetMsgHistoryCallback& callback,
        const std::string& json_extension = "");

    /**
     * @brief 根据消息关联的标签查询历史消息
     *
     * @param[in] room_id           聊天室 ID
     * @param[in] parameters        查询参数
     * @param[in] callback          回调函数
     * @param[in] json_extension    json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void GetMessageHistoryByTagsOnlineAsync(const int64_t room_id,
        const ChatRoomGetMsgHistoryByTagsParameters& parameters,
        const GetMsgHistoryCallback& callback,
        const std::string& json_extension = "");

    /** @fn void SetMemberAttributeOnlineAsync(const int64_t room_id, const ChatRoomSetMemberAttributeParameters &parameters, const GetMembersCallback
     &callback, const std::string& json_extension = "")
     * 异步设置成员身份标识
     * @param[in] room_id              聊天室ID
     * @param[in] parameters          查询参数
     * @param[in] callback              回调函数
     * @param[in] json_extension      json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void SetMemberAttributeOnlineAsync(const int64_t room_id,
        const ChatRoomSetMemberAttributeParameters& parameters,
        const SetMemberAttributeCallback& callback,
        const std::string& json_extension = "");

    /** @fn void GetInfoAsync(const int64_t room_id, const GetChatRoomInfoCallback& callback ,const std::string &json_extension)
     * 异步获取当前聊天室信息
     * @param[in] room_id                聊天室ID
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void GetInfoAsync(const int64_t room_id, const GetChatRoomInfoCallback& callback, const std::string& json_extension = "");

    /** @fn void GetMemberInfoByIDsAsync(const int64_t room_id, const std::list<std::string>& ids, const GetMembersCallback& callback ,const
     std::string &json_extension)
     * 异步获取指定成员信息
     * @param[in] room_id                聊天室ID
     * @param[in] ids
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void GetMemberInfoByIDsAsync(const int64_t room_id,
        const std::list<std::string>& ids,
        const GetMembersCallback& callback,
        const std::string& json_extension = "");

    /** @fn void KickMemberAsync(const int64_t room_id, const std::string& id, const std::string& notify_ext, const KickMemberCallback &callback,
     const std::string &json_extension)
     * 异步踢掉指定成员
     * @param[in] room_id                聊天室ID
     * @param[in] id                    成员id
     * @param[in] notify_ext            放到事件通知中的扩展字段
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void KickMemberAsync(const int64_t room_id,
        const std::string& id,
        const std::string& notify_ext,
        const KickMemberCallback& callback,
        const std::string& json_extension = "");

    /** @fn void SetProxy(NIMChatRoomProxyType type, const std::string& host, int port, const std::string& user, const std::string& password)
     * 设置SDK统一的网络代理。不需要代理时，type设置为kNIMProxyNone，其余参数都传空字符串（端口设为0）。有些代理不需要用户名和密码，相应参数也传空字符串。
     * @param[in] type 代理类型，见NIMChatRoomProxyType定义
     * @param[in] host 代理地址
     * @param[in] port 代理端口
     * @param[in] user 代理用户名
     * @param[in] password 代理密码
     * @return void 无返回值
     */
    static void SetProxy(NIMChatRoomProxyType type, const std::string& host, int port, const std::string& user, const std::string& password);

    /** @fn static void TempMuteMemberAsync(const int64_t room_id, const std::string& accid, const int64_t duration, bool need_notify, std::string&
     notify_ext, const TempMuteMemberCallback& callback, const std::string &json_extension = "")
     * 异步临时禁言/解禁成员
     * @param[in] room_id                聊天室ID
     * @param[in] accid                    成员ID
     * @param[in] duration                临时禁言时长（秒），解禁填0
     * @param[in] need_notify            是否聊天室内广播通知
     * @param[in] notify_ext            通知中的自定义字段，长度限制2048
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void TempMuteMemberAsync(const int64_t room_id,
        const std::string& accid,
        const int64_t duration,
        bool need_notify,
        const std::string& notify_ext,
        const TempMuteMemberCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void TempMuteMemberByTagAsync(const int64_t room_id, const std::string& target_tag, const int64_t duration, bool need_notify, const
     std::string& notify_ext, const TempMuteMemberCallback& callback, const std::string& notify_tags = "", const std::string& json_extension = "")
     * 异步根据 tag 临时禁言/解禁成员
     * @param room_id           聊天室ID
     * @param target_tag        要禁言的指定 tag
     * @param duration          临时禁言时长（秒），解禁填0
     * @param need_notify       是否聊天室内广播通知
     * @param notify_ext        通知中的自定义字段，长度限制2048
     * @param callback          回调函数
     * @param notify_tags       要通知的目标 tags，是一个标签表达式
     * @param json_extension    json扩展参数（备用，目前不需要）
     */
    static void TempMuteMemberByTagAsync(const int64_t room_id,
        const std::string& target_tag,
        const int64_t duration,
        bool need_notify,
        const std::string& notify_ext,
        const TempMuteMemberCallback& callback,
        const std::string& notify_tags = "",
        const std::string& json_extension = "");

    /** @fn static void UpdateRoomInfoAsync(const int64_t room_id, const ChatRoomInfo& info, bool need_notify, std::string& notify_ext, const
     UpdateRoomInfoCallback& callback, const std::string &json_extension = "")
     * 更新聊天室信息
     * @param[in] room_id                聊天室ID
     * @param[in] info                    聊天室信息
     * @param[in] need_notify            是否聊天室内广播通知
     * @param[in] notify_ext            通知中的自定义字段，长度限制2048
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数
     目前可用于配置反垃圾相关参数，格式{"anti_spam_business_id":"{\"textbid\":\"xxxx\",\"picbid\":\"xxxx\"}"}
     * @return void 无返回值
     */
    static void UpdateRoomInfoAsync(const int64_t room_id,
        const ChatRoomInfo& info,
        bool need_notify,
        const std::string& notify_ext,
        const UpdateRoomInfoCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void UpdateMyRoomRoleAsync(const int64_t room_id, const ChatRoomMemberInfo& info, bool need_notify, std::string& notify_ext, const
     UpdateMyRoomRoleCallback& callback, const std::string &json_extension = "")
     * 更新我的信息
     * @param[in] room_id                聊天室ID
     * @param[in] info                    我的信息
     * @param[in] need_notify            是否聊天室内广播通知
     * @param[in] notify_ext            通知中的自定义字段，长度限制2048
     * @param[in] callback                回调函数
     * @param[in] json_extension
     json扩展参数，针对固定成员，可配置更新的信息是否需要持久化，默认不持久化以及配置反垃圾相关参数{"need_save":false,"anti_spam_business_id":"{\"textbid\":\"xxxx\",\"picbid\":\"xxxx\"}"}
     * @return void 无返回值
     */
    static void UpdateMyRoomRoleAsync(const int64_t room_id,
        const ChatRoomMemberInfo& info,
        bool need_notify,
        const std::string& notify_ext,
        const UpdateMyRoomRoleCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void QueueOfferAsync(const int64_t room_id, const ChatRoomQueueElement& element, const QueueOfferCallback& callback, const
     std::string &json_extension = "");
     * 新加(更新)麦序队列元素，如果element.key_对应的元素已经在队列中存在了，那就是更新操作，如果不存在，就放到队列尾部
     * @param[in] room_id                聊天室ID
     * @param[in] element                麦序队列元素
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数
     * @return void 无返回值
     */
    static void QueueOfferAsync(const int64_t room_id,
        const ChatRoomQueueElement& element,
        const QueueOfferCallback& callback,
        const std::string& json_extension = "{\"transient\":true}");

    /** @fn static void QueueOfferAsyncEx(const int64_t room_id,
                                    const ChatRoomQueueElement& element,
                                    const ChatRoomQueueOfferOption& option,
                                    const QueueOfferCallback& callback)
        * 新加(更新)麦序队列元素，如果element.key_对应的元素已经在队列中存在了，那就是更新操作，如果不存在，就放到队列尾部
        * @param[in] room_id                聊天室ID
        * @param[in] element                麦序队列元素
        * @param[in] option                    更新排序选项，详见 ChatRoomQueueOfferOption
        * @param[in] callback                回调函数
        */
    static void QueueOfferAsyncEx(const int64_t room_id,
        const ChatRoomQueueElement& element,
        const ChatRoomQueueOfferOption& option,
        const QueueOfferCallback& callback);

    /** @fn static void QueuePollAsync(const int64_t room_id, const std::string& element_key, const QueuePollCallback& callback, const std::string
     &json_extension = "")
     * 取出麦序元素
     * @param[in] room_id                聊天室ID
     * @param[in] element_key            需要取出的元素的UniqKey, 传空传表示取出第一个元素
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void QueuePollAsync(const int64_t room_id,
        const std::string& element_key,
        const QueuePollCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void QueueListAsync(const int64_t room_id, const QueueListCallback& callback, const std::string &json_extension = "");
     * 排序列出所有元素
     * @param[in] room_id                聊天室ID
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void QueueListAsync(const int64_t room_id, const QueueListCallback& callback, const std::string& json_extension = "");

    /** @fn static void QueueHeaderAsync(const int64_t room_id, const QueueHeaderCallback& callback, const std::string &json_extension = "");
     * 查看麦序头元素
     * @param[in] room_id                聊天室ID
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void QueueHeaderAsync(const int64_t room_id, const QueueHeaderCallback& callback, const std::string& json_extension = "");

    /** @fn static void QueueDropAsync(const int64_t room_id, const QueueDropCallback& callback, const std::string &json_extension = "");
     * (管理员权限)删除麦序队列
     * @param[in] room_id                聊天室ID
     * @param[in] callback                回调函数
     * @param[in] json_extension        json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void QueueDropAsync(const int64_t room_id, const QueueDropCallback& callback, const std::string& json_extension = "");

    /** @fn void UnregChatroomCb()
     * 反注册Chatroom提供的所有回调
     * @return void 无返回值
     */
    static void UnregChatroomCb();

    /** @fn char *QueryAllRobotInfosBlock(const int64_t room_id, const std::string &json_extension = "")
     * 获取全部机器人信息(同步接口，堵塞NIM内部线程)
     * @param[in] room_id                聊天室ID
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return char 机器人信息 json string array
     */
    static RobotInfos QueryAllRobotInfosBlock(const int64_t room_id, const std::string& json_extension = "");

    /** @fn char *QueryRobotInfoByAccidBlock(const int64_t room_id, const std::string &accid, const std::string &json_extension = "")
     * 获取指定机器人信息(同步接口，堵塞NIM内部线程)
     * @param[in] room_id                聊天室ID
     * @param[in] accid 机器人accid
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return char 机器人信息 json string
     */
    static RobotInfo QueryRobotInfoByAccidBlock(const int64_t room_id, const std::string& accid, const std::string& json_extension = "");

    /** @fn void GetRobotInfoAsync(const int64_t room_id, const int64_t timetag, const RobotQueryCallback &callback, const std::string &json_extension
     = "")
     * 获取机器人信息
     * @param[in] room_id                聊天室ID
     * @param[in] timetag 时间戳
     * @param[in] callback        回调函数
     * @param[in] json_extension json扩展参数（备用，目前不需要）
     * @return  void
     */
    static void GetRobotInfoAsync(const int64_t room_id,
        const int64_t timetag,
        const RobotQueryCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void QueueBatchUpdateAsync(const int64_t room_id, const std::string& elements, const QueueListCallback& callback, const std::string
     &json_extension = "")
     * 批量更新聊天室队列
     * @param[in] room_id                    聊天室ID
     * @param[in] batch_elements                所有需要更新的列表
     * @param[in] callback                    回调函数
     * @param[in] json_extension                json扩展参数（备用，目前不需要）
     * @return void 无返回值
     */
    static void QueueBatchUpdateAsync(const int64_t room_id,
        const ChatRoomBatchMembers& batch_elements,
        bool need_notify,
        const std::string& notify_ext,
        const QueueBatchUpdateCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void UpdateLocation(const int64_t room_id, NIMChatRoomLocation location, const UpdateLocationCallback& callback, const std::string
     &json_extension = "")
     * 更新聊天室位置信息
     * @param[in] room_id           聊天室ID
     * @param[in] location          位置信息
     * @param[in] callback          回调函数
     * @param[in] json_extension    json扩展参数（备用，目前不需要）
     * @return bool true 为接口调用成功，false 为位置信息尚未初始化
     */
    static bool UpdateLocation(const int64_t room_id,
        NIMChatRoomLocation location,
        const UpdateLocationCallback& callback,
        const std::string& json_extension = "");

    /** @fn static void UpdateTags(const int64_t room_id, ChatRoomUpdateTagsInfo tags_info, const UpdateTagsCallback& callback, const std::string
    &json_extension = "")
    * 更新聊天室标签
    * @param[in] room_id           聊天室ID
    * @param[in] tags_info         标签信息
    * @param[in] callback          回调函数
    * @param[in] json_extension    json扩展参数（备用，目前不需要）
    * @return bool true 为接口调用成功，false 为位置信息尚未初始化
    */
    static bool UpdateTags(const int64_t room_id,
        ChatRoomUpdateTagsInfo tags_info,
        const UpdateTagsCallback& callback,
        const std::string& json_extension = "");
};

}  // namespace nim_chatroom

#endif  //_NIM_CHATROOM_SDK_CPP_H_
