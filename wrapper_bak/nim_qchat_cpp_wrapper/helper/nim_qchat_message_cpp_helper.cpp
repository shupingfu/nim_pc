#include "nim_qchat_message_cpp_def.h"

namespace nim {

// clang-format off
std::map<NIMQChatMsgType, QChatMessageSend::AttachRawCreateFunc> QChatMessageSend::attach_raw_create_function_ = {
        { kNIMQChatMsgTypeText, [](const std::string& r) { return std::make_shared<QChatDefaultAttach>(r); } },
        { kNIMQChatMsgTypeImage, [](const std::string& r) { return std::make_shared<QChatImageAttach>(r); } },
        { kNIMQChatMsgTypeAudio, [](const std::string& r) { return std::make_shared<QChatAudioAttach>(r); } },
        { kNIMQChatMsgTypeVideo, [](const std::string& r) { return std::make_shared<QChatVideoAttach>(r); } },
        { kNIMQChatMsgTypeFile, [](const std::string& r) { return std::make_shared<QChatFileAttach>(r); } },
        { kNIMQChatMsgTypeLocation, [](const std::string& r) { return std::make_shared<QChatLocationAttach>(r); } },
        { kNIMQChatMsgTypeNotification, [](const std::string& r) { return std::make_shared<QChatNotificationAttach>(r); } },
        { kNIMQChatMsgTypeTips, [](const std::string& r) { return std::make_shared<QChatTipsAttach>(r); } }
    };
// clang-format on

}  // namespace nim
