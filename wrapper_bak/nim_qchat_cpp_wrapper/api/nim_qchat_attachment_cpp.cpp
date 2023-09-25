/**
 * @file nim_qchat_attachment_cpp.cpp
 * @author NetEase Yunxin
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright (c) 2021, NetEase Inc. All rights reserved
 *
 */
#include "nim_qchat_cpp_wrapper/api/nim_qchat_attachment_cpp.h"
#include "nim_wrapper_util/callback_proxy.h"

namespace nim {
// C interfaces typedef
#ifdef NIM_SDK_DLL_IMPORT
typedef void (*nim_qchat_attachment_reg_custom_token_cb)(const NIMQChatAttachmentRegCustomTokenCbParam* param);
typedef void (*nim_qchat_attachment_reg_upload_cb)(const NIMQChatAttachmentRegUploadCbParam* param);
typedef void (*nim_qchat_attachment_reg_download_cb)(const NIMQChatAttachmentRegDownloadCbParam* param);
typedef void (*nim_qchat_attachment_reg_progress_cb)(const NIMQChatAttachmentRegProgressCbParam* param);
typedef void (*nim_qchat_attachment_upload)(const NIMQChatAttachmentUploadParam* param);
typedef void (*nim_qchat_attachment_stop_upload)(const NIMQChatAttachmentStopUploadParam* param);
typedef void (*nim_qchat_attachment_download)(const NIMQChatAttachmentDownloadParam* param);
typedef void (*nim_qchat_attachment_stop_download)(const NIMQChatAttachmentStopDownloadParam* param);
#else
#include "nim_qchat_attachment.h"
#endif
void Attachment::RegCustomTokenCb(const QChatAttachmentRegCustomTokenCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_reg_custom_token_cb)(&c_param);
}

void Attachment::RegUploadCb(const QChatAttachmentRegUploadCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_reg_upload_cb)(&c_param);
}

void Attachment::RegDownloadCb(const QChatAttachmentRegDownloadCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_reg_download_cb)(&c_param);
}

void Attachment::RegProgressCb(const QChatAttachmentRegProgressCbParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_reg_progress_cb)(&c_param);
}

void Attachment::Upload(const QChatAttachmentUploadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_upload)(&c_param);
}

void Attachment::StopUpload(const QChatAttachmentStopUploadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_stop_upload)(&c_param);
}

void Attachment::Download(const QChatAttachmentDownloadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_download)(&c_param);
}

void Attachment::StopDownload(const QChatAttachmentStopDownloadParam& param) {
    auto c_param = param.ToCParam();
    NIM_QCHAT_SDK_GET_FUNC(nim_qchat_attachment_stop_download)(&c_param);
}

}  // namespace nim
