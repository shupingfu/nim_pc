/** @file nim_tools_http_cpp.cpp
 * @brief NIM HTTP提供的传输工具接口(CPP接口)
 * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
 * @author towik, Oleg
 * @date 2015/4/30
 */

#include "nim_tools_cpp_wrapper/nim_tools_http_cpp.h"
#include <assert.h>
#include <exception>
#include "nim_wrapper_util/nim_sdk_util.h"
#if defined(WIN32)
#include <windows.h>
#endif

namespace nim_http {

nim::SDKInstance* sdk_instance_ = nullptr;

typedef void (*typeof_nim_http_init)();
typedef void (*typeof_nim_http_uninit)();
typedef void (*typeof_nim_http_init_log)(const char* log_file_path);
typedef bool (*typeof_nim_http_is_init_log)();
typedef HttpRequestHandle (*typeof_nim_http_create_download_file_request)(const char* url,
    const char* download_file_path,
    nim_http_request_completed_cb complete_cb,
    const void* user_data);
typedef HttpRequestHandle (*typeof_nim_http_create_download_file_range_request)(const char* url,
    const char* download_file_path,
    int64_t range_start,
    nim_http_request_completed_cb complete_cb,
    const void* user_data);
typedef HttpRequestHandle (*typeof_nim_http_create_request)(const char* url,
    const char* post_body,
    size_t post_body_size,
    nim_http_request_response_cb response_cb,
    const void* user_data);
typedef HttpRequestHandle (*typeof_nim_http_create_request_ex)(const char* url,
    const char* post_body,
    size_t post_body_size,
    nim_http_request_response_ex_cb response_cb,
    const void* user_data);
typedef void (*typeof_nim_http_add_request_header)(HttpRequestHandle request_handle, const char* key, const char* value);
typedef void (*typeof_nim_http_set_request_progress_cb)(HttpRequestHandle request_handle,
    nim_http_request_progress_cb progress_callback,
    const void* user_data);
typedef void (
    *typeof_nim_http_set_request_speed_cb)(HttpRequestHandle request_handle, nim_http_request_speed_cb speed_callback, const void* user_data);
typedef void (*typeof_nim_http_set_request_transfer_cb)(HttpRequestHandle request_handle,
    nim_http_request_transfer_cb transfer_callback,
    const void* user_data);
typedef void (*typeof_nim_http_set_request_method_as_post)(HttpRequestHandle request_handle);
typedef void (*typeof_nim_http_set_timeout)(HttpRequestHandle request_handle, int timeout_ms);
typedef void (*typeof_nim_http_set_low_speed)(HttpRequestHandle request_handle, int low_speed_limit, int low_speed_time);
typedef void (
    *typeof_nim_http_set_proxy)(HttpRequestHandle request_handle, int type, const char* host, short port, const char* user, const char* pass);
typedef HttpRequestID (*typeof_nim_http_post_request)(HttpRequestHandle);
typedef void (*typeof_nim_http_remove_request)(HttpRequestID http_request_id);
typedef const char* const (*typeof_nim_http_get_response_head)(HttpRequestID http_request_id);

typeof_nim_http_init g_nim_http_init = nullptr;
typeof_nim_http_uninit g_nim_http_uninit = nullptr;
typeof_nim_http_init_log g_nim_http_init_log = nullptr;
typeof_nim_http_is_init_log g_nim_http_is_init_log = nullptr;
typeof_nim_http_create_download_file_request g_nim_http_create_download_file_request = nullptr;
typeof_nim_http_create_download_file_range_request g_nim_http_create_download_file_range_request = nullptr;
typeof_nim_http_create_request g_nim_http_create_request = nullptr;
typeof_nim_http_create_request_ex g_nim_http_create_request_ex = nullptr;
typeof_nim_http_add_request_header g_nim_http_add_request_header = nullptr;
typeof_nim_http_set_request_progress_cb g_nim_http_set_request_progress_cb = nullptr;
typeof_nim_http_set_request_speed_cb g_nim_http_set_request_speed_cb = nullptr;
typeof_nim_http_set_request_transfer_cb g_nim_http_set_request_transfer_cb = nullptr;
typeof_nim_http_set_request_method_as_post g_nim_http_set_request_method_as_post = nullptr;
typeof_nim_http_set_timeout g_nim_http_set_timeout = nullptr;
typeof_nim_http_set_low_speed g_nim_http_set_low_speed = nullptr;
typeof_nim_http_set_proxy g_nim_http_set_proxy = nullptr;
typeof_nim_http_post_request g_nim_http_post_request = nullptr;
typeof_nim_http_remove_request g_nim_http_remove_request = nullptr;
typeof_nim_http_get_response_head g_nim_http_get_response_head = nullptr;
// HMODULE g_hmod = NULL;
struct CompletedCallbackUserData {
    CompletedCallbackUserData()
        : completed_cb()
        , progress_cb_pointer(nullptr)
        , speed_cb_pointer(nullptr)
        , transfer_cb_pointer(nullptr) {}
    CompletedCallback completed_cb;
    ProgressCallback* progress_cb_pointer;
    SpeedCallback* speed_cb_pointer;
    TransferCallback* transfer_cb_pointer;
};

/// @struct ResponseCallbackUserData
struct ResponseCallbackUserData {
    ResponseCallbackUserData()
        : response_cb()
        , progress_cb_pointer(nullptr)
        , speed_cb_pointer(nullptr)
        , transfer_cb_pointer(nullptr) {}
    ResponseCallbackEx response_cb;
    ProgressCallback* progress_cb_pointer;
    SpeedCallback* speed_cb_pointer;
    TransferCallback* transfer_cb_pointer;
};

bool Init(const std::string& library_path /* = L""*/) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // define something for Windows (32-bit and 64-bit, this part is common)
    static const char* kSdkNimDll = "nim_tools_http.dll";
#ifdef _WIN64
    // define something for Windows (64-bit only)
#else
    // define something for Windows (32-bit only)
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    static const char* kSdkNimDll = "libnim_tools_http.dylib";
#elif TARGET_OS_IPHONE
    // iOS device
    static const char* kSdkNimDll = "libnim_tools_http.dylib";
#elif TARGET_OS_MAC
    // Other kinds of Mac OS
    static const char* kSdkNimDll = "libnim_tools_http.dylib";
#else
#error "Unknown Apple platform"
    static const char* kSdkNimDll = nullptr;
#endif
#elif __ANDROID__
    static const char* kSdkNimDll = "libnim_tools_http.so";
#elif __linux__
    // linux
    static const char* kSdkNimDll = "libnim_tools_http.so";
#elif __unix__  // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#error "Unknown compiler"
    static const char* kSdkNimDll = nullptr;
#endif
    sdk_instance_ = new nim::SDKInstance();
    if (!sdk_instance_->LoadSdkDll(library_path.c_str(), kSdkNimDll))
        return false;

    g_nim_http_init = reinterpret_cast<typeof_nim_http_init>(sdk_instance_->GetFunction("nim_http_init"));
    g_nim_http_uninit = reinterpret_cast<typeof_nim_http_uninit>(sdk_instance_->GetFunction("nim_http_uninit"));
    g_nim_http_init_log = reinterpret_cast<typeof_nim_http_init_log>(sdk_instance_->GetFunction("nim_http_init_log"));
    g_nim_http_is_init_log = reinterpret_cast<typeof_nim_http_is_init_log>(sdk_instance_->GetFunction("nim_http_is_init_log"));
    g_nim_http_create_download_file_request =
        reinterpret_cast<typeof_nim_http_create_download_file_request>(sdk_instance_->GetFunction("nim_http_create_download_file_request"));
    g_nim_http_create_download_file_range_request = reinterpret_cast<typeof_nim_http_create_download_file_range_request>(
        sdk_instance_->GetFunction("nim_http_create_download_file_range_request"));
    g_nim_http_create_request = reinterpret_cast<typeof_nim_http_create_request>(sdk_instance_->GetFunction("nim_http_create_request"));
    g_nim_http_create_request_ex = reinterpret_cast<typeof_nim_http_create_request_ex>(sdk_instance_->GetFunction("nim_http_create_request_ex"));
    g_nim_http_add_request_header = reinterpret_cast<typeof_nim_http_add_request_header>(sdk_instance_->GetFunction("nim_http_add_request_header"));
    g_nim_http_set_request_progress_cb =
        reinterpret_cast<typeof_nim_http_set_request_progress_cb>(sdk_instance_->GetFunction("nim_http_set_request_progress_cb"));
    g_nim_http_set_request_speed_cb =
        reinterpret_cast<typeof_nim_http_set_request_speed_cb>(sdk_instance_->GetFunction("nim_http_set_request_speed_cb"));
    g_nim_http_set_request_transfer_cb =
        reinterpret_cast<typeof_nim_http_set_request_transfer_cb>(sdk_instance_->GetFunction("nim_http_set_request_transfer_cb"));
    g_nim_http_set_request_method_as_post =
        reinterpret_cast<typeof_nim_http_set_request_method_as_post>(sdk_instance_->GetFunction("nim_http_set_request_method_as_post"));
    g_nim_http_set_timeout = reinterpret_cast<typeof_nim_http_set_timeout>(sdk_instance_->GetFunction("nim_http_set_timeout"));
    g_nim_http_set_low_speed = reinterpret_cast<typeof_nim_http_set_low_speed>(sdk_instance_->GetFunction("nim_http_set_low_speed"));
    g_nim_http_set_proxy = reinterpret_cast<typeof_nim_http_set_proxy>(sdk_instance_->GetFunction("nim_http_set_proxy"));
    g_nim_http_post_request = reinterpret_cast<typeof_nim_http_post_request>(sdk_instance_->GetFunction("nim_http_post_request"));
    g_nim_http_remove_request = reinterpret_cast<typeof_nim_http_remove_request>(sdk_instance_->GetFunction("nim_http_remove_request"));
    g_nim_http_get_response_head = reinterpret_cast<typeof_nim_http_get_response_head>(sdk_instance_->GetFunction("nim_http_get_response_head"));
    if (g_nim_http_init != nullptr)
        g_nim_http_init();
    else
        return false;
    return true;
}

void Uninit() {
    if (g_nim_http_uninit != nullptr)
        g_nim_http_uninit();
    if (sdk_instance_)
        sdk_instance_->UnLoadSdkDll();
}

void InitLog(const std::string& log_file_path) {
    g_nim_http_init_log(log_file_path.c_str());
}

bool IsInitLog() {
    return g_nim_http_is_init_log();
}

// 设置cpp封装层的全局代理
nim::NIMProxyType proxy_type_ = nim::kNIMProxyNone;
std::string proxy_host_;
short proxy_port_ = 0;
std::string proxy_user_;
std::string proxy_pass_;
void SetGlobalProxy(nim::NIMProxyType type, const std::string& host, short port, const std::string& user, const std::string& pass) {
    proxy_type_ = type;
    proxy_host_ = host;
    proxy_port_ = port;
    proxy_user_ = user;
    proxy_pass_ = pass;
}

HttpRequestID PostRequest(const HttpRequest& http_request) {
    return g_nim_http_post_request(http_request.http_reuqest_handle_);
}

void RemoveRequest(HttpRequestID http_request_id) {
    g_nim_http_remove_request(http_request_id);
}
std::string GetResponseHead(HttpRequestID http_request_id) {
    auto head_info = g_nim_http_get_response_head(http_request_id);
    if (head_info != nullptr)
        return head_info;
    return "";
}
HttpRequest::HttpRequest(const std::string& url,
    const std::string& download_file_path,
    const CompletedCallback& complete_cb,
    const ProgressCallback& progress_cb,
    const SpeedCallback& speed_cb,
    const TransferCallback& transfer_cb) {
    ProgressCallback* progress_cb_pointer = nullptr;
    if (progress_cb)
        progress_cb_pointer = new ProgressCallback(progress_cb);

    SpeedCallback* speed_cb_pointer = nullptr;
    if (speed_cb)
        speed_cb_pointer = new SpeedCallback(speed_cb);

    TransferCallback* transfer_cb_pointer = nullptr;
    if (transfer_cb)
        transfer_cb_pointer = new TransferCallback(transfer_cb);

    CompletedCallbackUserData* complete_cb_userdata = nullptr;
    if (complete_cb) {
        complete_cb_userdata = new CompletedCallbackUserData();
        complete_cb_userdata->completed_cb = complete_cb;
        complete_cb_userdata->progress_cb_pointer = progress_cb_pointer;
        complete_cb_userdata->speed_cb_pointer = speed_cb_pointer;
        complete_cb_userdata->transfer_cb_pointer = transfer_cb_pointer;
    }
    http_reuqest_handle_ =
        g_nim_http_create_download_file_request(url.c_str(), download_file_path.c_str(), &CompletedCallbackWrapper, complete_cb_userdata);

    if (proxy_type_ != nim::kNIMProxyNone)
        SetProxy(proxy_type_, proxy_host_, proxy_port_, proxy_user_, proxy_pass_);

    if (progress_cb)
        g_nim_http_set_request_progress_cb(http_reuqest_handle_, &ProgressCallbackWrapper, progress_cb_pointer);

    if (speed_cb)
        g_nim_http_set_request_speed_cb(http_reuqest_handle_, &SpeedCallbackWrapper, speed_cb_pointer);

    if (transfer_cb)
        g_nim_http_set_request_transfer_cb(http_reuqest_handle_, &TransferCallbackWrapper, transfer_cb_pointer);
}

HttpRequest::HttpRequest(const std::string& url,
    const std::string& download_file_path,
    int64_t range_start,
    const CompletedCallback& complete_cb,
    const ProgressCallback& progress_cb,
    const SpeedCallback& speed_cb,
    const TransferCallback& transfer_cb) {
    ProgressCallback* progress_cb_pointer = nullptr;
    if (progress_cb)
        progress_cb_pointer = new ProgressCallback(progress_cb);

    SpeedCallback* speed_cb_pointer = nullptr;
    if (speed_cb)
        speed_cb_pointer = new SpeedCallback(speed_cb);

    TransferCallback* transfer_cb_pointer = nullptr;
    if (transfer_cb)
        transfer_cb_pointer = new TransferCallback(transfer_cb);

    CompletedCallbackUserData* complete_cb_userdata = nullptr;
    if (complete_cb) {
        complete_cb_userdata = new CompletedCallbackUserData();
        complete_cb_userdata->completed_cb = complete_cb;
        complete_cb_userdata->progress_cb_pointer = progress_cb_pointer;
        complete_cb_userdata->speed_cb_pointer = speed_cb_pointer;
        complete_cb_userdata->transfer_cb_pointer = transfer_cb_pointer;
    }
    http_reuqest_handle_ = g_nim_http_create_download_file_range_request(
        url.c_str(), download_file_path.c_str(), range_start, &CompletedCallbackWrapper, complete_cb_userdata);

    if (proxy_type_ != nim::kNIMProxyNone)
        SetProxy(proxy_type_, proxy_host_, proxy_port_, proxy_user_, proxy_pass_);

    if (progress_cb)
        g_nim_http_set_request_progress_cb(http_reuqest_handle_, &ProgressCallbackWrapper, progress_cb_pointer);

    if (speed_cb)
        g_nim_http_set_request_speed_cb(http_reuqest_handle_, &SpeedCallbackWrapper, speed_cb_pointer);

    if (transfer_cb)
        g_nim_http_set_request_transfer_cb(http_reuqest_handle_, &TransferCallbackWrapper, transfer_cb_pointer);
}

HttpRequest::HttpRequest(const std::string& url,
    const char* post_body,
    size_t post_body_size,
    const ResponseCallback& response_cb,
    const ProgressCallback& progress_cb,
    const SpeedCallback& speed_cb,
    const TransferCallback& transfer_cb) {
    ProgressCallback* progress_cb_pointer = nullptr;
    if (progress_cb)
        progress_cb_pointer = new ProgressCallback(progress_cb);

    SpeedCallback* speed_cb_pointer = nullptr;
    if (speed_cb)
        speed_cb_pointer = new SpeedCallback(speed_cb);

    TransferCallback* transfer_cb_pointer = nullptr;
    if (transfer_cb)
        transfer_cb_pointer = new TransferCallback(transfer_cb);

    ResponseCallbackUserData* response_cb_userdata = nullptr;
    if (response_cb) {
        response_cb_userdata = new ResponseCallbackUserData();
        response_cb_userdata->response_cb = [response_cb](bool ret, int code, const std::string& body, const std::string& head) {
            response_cb(ret, code, body);
        };
        response_cb_userdata->progress_cb_pointer = progress_cb_pointer;
        response_cb_userdata->speed_cb_pointer = speed_cb_pointer;
        response_cb_userdata->transfer_cb_pointer = transfer_cb_pointer;
    }
    http_reuqest_handle_ = g_nim_http_create_request_ex(url.c_str(), post_body, post_body_size, &ResponseCallbackWrapper, response_cb_userdata);

    if (proxy_type_ != nim::kNIMProxyNone)
        SetProxy(proxy_type_, proxy_host_, proxy_port_, proxy_user_, proxy_pass_);

    if (progress_cb)
        g_nim_http_set_request_progress_cb(http_reuqest_handle_, &ProgressCallbackWrapper, progress_cb_pointer);

    if (speed_cb)
        g_nim_http_set_request_speed_cb(http_reuqest_handle_, &SpeedCallbackWrapper, speed_cb_pointer);

    if (transfer_cb)
        g_nim_http_set_request_transfer_cb(http_reuqest_handle_, &TransferCallbackWrapper, transfer_cb_pointer);
}

HttpRequest::HttpRequest(const std::string& url,
    const char* post_body,
    size_t post_body_size,
    const ResponseCallbackEx& response_cb,
    const ProgressCallback& progress_cb,
    const SpeedCallback& speed_cb,
    const TransferCallback& transfer_cb) {
    ProgressCallback* progress_cb_pointer = nullptr;
    if (progress_cb)
        progress_cb_pointer = new ProgressCallback(progress_cb);

    SpeedCallback* speed_cb_pointer = nullptr;
    if (speed_cb)
        speed_cb_pointer = new SpeedCallback(speed_cb);

    TransferCallback* transfer_cb_pointer = nullptr;
    if (transfer_cb)
        transfer_cb_pointer = new TransferCallback(transfer_cb);

    ResponseCallbackUserData* response_cb_userdata = nullptr;
    if (response_cb) {
        response_cb_userdata = new ResponseCallbackUserData();
        response_cb_userdata->response_cb = response_cb;
        response_cb_userdata->progress_cb_pointer = progress_cb_pointer;
        response_cb_userdata->speed_cb_pointer = speed_cb_pointer;
        response_cb_userdata->transfer_cb_pointer = transfer_cb_pointer;
    }
    http_reuqest_handle_ = g_nim_http_create_request_ex(url.c_str(), post_body, post_body_size, &ResponseCallbackWrapper, response_cb_userdata);

    if (proxy_type_ != nim::kNIMProxyNone)
        SetProxy(proxy_type_, proxy_host_, proxy_port_, proxy_user_, proxy_pass_);

    if (progress_cb)
        g_nim_http_set_request_progress_cb(http_reuqest_handle_, &ProgressCallbackWrapper, progress_cb_pointer);

    if (speed_cb)
        g_nim_http_set_request_speed_cb(http_reuqest_handle_, &SpeedCallbackWrapper, speed_cb_pointer);

    if (transfer_cb)
        g_nim_http_set_request_transfer_cb(http_reuqest_handle_, &TransferCallbackWrapper, transfer_cb_pointer);
}

void HttpRequest::AddHeader(const std::string& key, const std::string& value) {
    g_nim_http_add_request_header(http_reuqest_handle_, key.c_str(), value.c_str());
}

void HttpRequest::AddHeader(const std::map<std::string, std::string>& headers_map) {
    for (auto it = headers_map.begin(); it != headers_map.end(); it++) {
        g_nim_http_add_request_header(http_reuqest_handle_, it->first.c_str(), it->second.c_str());
    }
}

void HttpRequest::SetMethodAsPost() {
    g_nim_http_set_request_method_as_post(http_reuqest_handle_);
}

void HttpRequest::SetTimeout(int timeout_ms) {
    g_nim_http_set_timeout(http_reuqest_handle_, timeout_ms);
}

void HttpRequest::SetLowSpeed(int low_speed_limit, int low_speed_time) {
    g_nim_http_set_low_speed(http_reuqest_handle_, low_speed_limit, low_speed_time);
}

void HttpRequest::SetProxy(nim::NIMProxyType type, const std::string& host, short port, const std::string& user, const std::string& pass) {
    g_nim_http_set_proxy(http_reuqest_handle_, type, host.c_str(), port, user.c_str(), pass.c_str());
}

void HttpRequest::CompletedCallbackWrapper(const void* user_data, bool is_ok, int response_code) {
    if (user_data) {
        auto completed_cb_userdata = (CompletedCallbackUserData*)user_data;
        if (completed_cb_userdata->completed_cb) {
            completed_cb_userdata->completed_cb(is_ok, response_code);
        }
        ProgressCallback* progress_cb = completed_cb_userdata->progress_cb_pointer;
        SpeedCallback* speed_cb = completed_cb_userdata->speed_cb_pointer;
        TransferCallback* transfer_cb = completed_cb_userdata->transfer_cb_pointer;
        delete progress_cb;
        delete speed_cb;
        delete transfer_cb;
        delete completed_cb_userdata;
    }
}

void HttpRequest::ResponseCallbackWrapper(const void* user_data, bool is_ok, int response_code, const char* content, const char* head) {
    if (user_data) {
        auto response_cb_userdata = (ResponseCallbackUserData*)user_data;
        if (response_cb_userdata->response_cb) {
            response_cb_userdata->response_cb(is_ok, response_code, std::string(content), std::string(head));
        }
        ProgressCallback* progress_cb = response_cb_userdata->progress_cb_pointer;
        SpeedCallback* speed_cb = response_cb_userdata->speed_cb_pointer;
        TransferCallback* transfer_cb = response_cb_userdata->transfer_cb_pointer;
        delete progress_cb;
        delete speed_cb;
        delete transfer_cb;
        delete response_cb_userdata;
    }
}

void HttpRequest::ProgressCallbackWrapper(const void* user_data,
    double upload_size,
    double uploaded_size,
    double download_size,
    double downloaded_size) {
    if (user_data) {
        auto cb = (ProgressCallback*)user_data;
        if (*cb) {
            (*cb)(upload_size, uploaded_size, download_size, downloaded_size);
        }
    }
}

void HttpRequest::SpeedCallbackWrapper(const void* user_data, double upload_speed, double download_speed) {
    if (user_data) {
        auto cb = (SpeedCallback*)user_data;
        if (*cb) {
            (*cb)(upload_speed, download_speed);
        }
    }
}

void HttpRequest::TransferCallbackWrapper(const void* user_data,
    double actual_upload_size,
    double upload_speed,
    double actual_download_size,
    double download_speed) {
    if (user_data) {
        auto cb = (TransferCallback*)user_data;
        if (*cb) {
            (*cb)(actual_upload_size, upload_speed, actual_download_size, download_speed);
        }
    }
}

}  // namespace nim_http
