/** @file nim_msg_helper.cpp
 * @brief Message 通用辅助方法和数据结构定义
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @date 2015/10/20
 */
#if !defined(WIN32)
#include <pthread.h>
#endif
#include "nim_cpp_wrapper/helper/nim_msg_helper.h"
#include "nim_msglog_def.h"
#include "nim_talk_def.h"
#include "nim_wrapper_util/nim_json_util.h"
namespace nim {
bool ParseRecallMsgNotify(const nim_cpp_wrapper_util::Json::Value& json_value, RecallMsgNotify& notify) {
    notify.from_id_ = json_value[kNIMRecallMsgKeyFromAccID].asString();
    notify.to_id_ = json_value[kNIMRecallMsgKeyToAccID].asString();
    notify.msg_id_ = json_value[kNIMRecallMsgKeyMsgId].asString();
    notify.operator_id_ = json_value[kNIMRecallMsgKeyOpeAccID].asString();
    notify.notify_ = json_value[kNIMRecallMsgKeyNotify].asString();
    notify.session_type_ = (NIMSessionType)json_value[kNIMRecallMsgKeyToType].asInt();
    notify.notify_timetag_ = json_value[kNIMRecallMsgKeyTime].asUInt64();
    notify.notify_feature_ = (NIMMessageFeature)json_value[kNIMRecallMsgKeyNotifyFeature].asInt();
    notify.msglog_exist_ = json_value[kNIMRecallMsgKeyMsgExist].asBool();
    notify.msglog_timetag_ = json_value[kNIMRecallMsgKeyMsgTime].asUInt64();
    notify.from_nick_ = json_value[kNIMRecallMsgKeyMsgFromNick].asString();
    notify.callback_ext_ = json_value[kNIMRecallMsgTagCallbackExt].asString();  // v8.2.0
    notify.attach_ = json_value[kNIMRecallMsgTagAttach].asString();             // v8.2.0
    return true;
}
bool ParseRecallMsgNotify(const std::string json_value, RecallMsgNotify& notify) {
    nim_cpp_wrapper_util::Json::Value value;
    if (nim::ParseJsonValue(json_value, value)) {
        return ParseRecallMsgNotify(value, notify);
    }
    return false;
}
bool ParseRecallMsgNotify(const std::string& notify_json, std::list<RecallMsgNotify>& notifys) {
    nim_cpp_wrapper_util::Json::Value values;
    if (nim::ParseJsonValue(notify_json, values) && values.isArray()) {
        for (auto it : values) {
            RecallMsgNotify notify;
            if (ParseRecallMsgNotify(it, notify))
                notifys.emplace_back(notify);
        }
        return true;
    }
    return false;
}

bool ParseSendMessageAck(const std::string& arc_json, SendMessageArc& arc) {
    nim_cpp_wrapper_util::Json::Value values;
    if (nim::ParseJsonValue(arc_json, values) && values.isObject()) {
        arc.rescode_ = (NIMResCode)values[kNIMSendAckKeyRescode].asUInt();
        arc.msg_id_ = values[kNIMSendAckKeyMsgId].asString();
        arc.msg_id_server_ = values[kNIMSendAckKeyMsgIdServer].asInt64();
        arc.talk_id_ = values[kNIMSendAckKeyTalkId].asString();
        arc.msg_timetag_ = values[kNIMSendAckKeyTimetag].asInt64();
        if (values.isMember(kNIMSendAckKeyCallbackExt))
            arc.third_party_callback_ext_ = values[kNIMSendAckKeyCallbackExt].asString();
        if (values.isMember(kNIMMsgKeyAntiSpamRes))
            arc.anti_spam_res_ = values[kNIMMsgKeyAntiSpamRes].asString();
        return true;
    }
    return false;
}

}  // namespace nim
