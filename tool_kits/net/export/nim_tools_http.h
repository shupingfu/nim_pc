/** @file nim_http.h
  * @brief NIM HTTP�ṩ��http������ؽӿ�
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/4/30
  */

#ifndef NET_NIM_HTTP_H_
#define NET_NIM_HTTP_H_

#include "net/base/net_export.h"

extern "C"
{
/** @enum NIMProxyType �������� */
enum NIMProxyType
{
	kNIMProxyNone		= 0,	/**< ��ʹ�ô���*/
	kNIMProxyHttp11		= 1,	/**< HTTP 1.1 Proxy*/
	kNIMProxySocks4		= 4,	/**< Socks4 Proxy*/
	kNIMProxySocks4a	= 5,	/**< Socks4a Proxy*/
	kNIMProxySocks5		= 6,	/**< Socks5 Proxy*/
};

/** @typedef void* HttpRequestHandle
  * http������
  */
typedef void* HttpRequestHandle;

/** @typedef void (*nim_http_request_completed_cb)(const void* user_data, bool, int)
  * nim callback function that has been registered in nim_http_create_*** API
  * @param[out] user_data		�ش����Զ�������
  * @param[out] result			��������true������ɹ���false������ʧ��
  * @param[out] response_code	http��Ӧ��
  * @return void				�޷���ֵ
  */
typedef void (*nim_http_request_completed_cb)(const void* user_data, bool result, int response_code);

/** @typedef void (*nim_http_request_response_cb)(const void* user_data, bool, int)
  * nim callback function that has been registered in nim_http_create_*** API
  * @param[out] user_data			�ش����Զ�������
  * @param[out] result				��������true������ɹ���false������ʧ��
  * @param[out] response_code		http��Ӧ��
  * @param[out] response_content	http��Ӧʵ������
  * @return void					�޷���ֵ
  */
typedef void (*nim_http_request_response_cb)(const void* user_data, bool result, int response_code, const char* response_content);

/** @typedef void (*nim_http_request_progress_cb)(const void* user_data, double uploaded_size, double total_upload_size, double downloaded_size, double total_download_size)
  * nim callback function that has been registered in nim_http_set_request_progress_cb API
  * @param[out] user_data				�ش����Զ�������
  * @param[out] uploaded_size			�Ѿ��ϴ����ֽ���
  * @param[out] total_upload_size		�ܵĴ��ϴ����ֽ���
  * @param[out] downloaded_size			�Ѿ����ص��ֽ���
  * @param[out] total_download_size		�ܵĴ����ص��ֽ���
  * @return void						�޷���ֵ
  */
typedef void (*nim_http_request_progress_cb)(const void* user_data, double uploaded_size, double total_upload_size, double downloaded_size, double total_download_size);

/** @fn void nim_http_init()
  * NIM HTTP ��ʼ��
  * @return void �޷���ֵ
  */
NET_EXPORT void nim_http_init();

/** @fn void nim_http_uninit()
  * NIM HTTP ����ʼ��
  * @return void �޷���ֵ
  */
NET_EXPORT void nim_http_uninit();

/** @fn int nim_http_post_request(HttpRequestHandle)
* NIM HTTP ��������
* @param[in] request_handle	http������
* @return int					����id
*/
NET_EXPORT int nim_http_post_request(HttpRequestHandle);

/** @fn void nim_http_remove_request(int http_request_id)
* NIM HTTP ȡ������
* @param[in] http_request_id	����id
* @return void				�޷���ֵ
*/
NET_EXPORT void nim_http_remove_request(int http_request_id);

/** @fn HttpRequestHandle nim_http_create_download_file_request(const char* url, const char *download_file_path, nim_http_request_completed_cb complete_cb, const void* user_data)
  * NIM HTTP ���������ļ�����
  * @param[in] url					��Դ��ַ
  * @param[in] download_file_path	�����ļ�����ı���·��
  * @param[in] complete_cb			�����ص�
  * @param[in] user_data			�Զ�������
  * @return HttpRequestHandle		http������
  */
NET_EXPORT HttpRequestHandle nim_http_create_download_file_request(const char* url, const char *download_file_path,
															nim_http_request_completed_cb complete_cb, const void* user_data);

/** @fn HttpRequestHandle nim_http_create_download_file_range_request(const char* url, const char *download_file_path, __int64 range_start, nim_http_request_completed_cb complete_cb, const void* user_data)
  * NIM HTTP ���������ļ�����֧�ֶϵ�����
  * @param[in] url					��Դ��ַ
  * @param[in] download_file_path	�����ļ�����ı���·��
  * @param[in] range_start			�����ļ�����ʼ��
  * @param[in] complete_cb			�����ص�
  * @param[in] user_data			�Զ�������
  * @return HttpRequestHandle		http������
  */
NET_EXPORT HttpRequestHandle nim_http_create_download_file_range_request(const char* url, const char *download_file_path,
															__int64 range_start, nim_http_request_completed_cb complete_cb, const void* user_data);

/** @fn HttpRequestHandle nim_http_create_request(const char* url, const char* post_body, size_t post_body_size, nim_http_request_response_cb response_cb, const void* user_data)
  * NIM HTTP ��������
  * @param[in] url				��Դ��ַ
  * @param[in] post_body		�ϴ�����
  * @param[in] post_body_size	�ϴ����ݴ�С
  * @param[in] response_cb		�����ص�����Ӧʵ������
  * @param[in] user_data		�Զ�������
  * @return HttpRequestHandle	http������
  */
NET_EXPORT HttpRequestHandle nim_http_create_request(const char* url, const char* post_body, size_t post_body_size, 
															nim_http_request_response_cb response_cb, const void* user_data);

/** @fn void nim_http_add_request_header(HttpRequestHandle request_handle, const char* key, const char* value)
  * NIM HTTP ��������
  * @param[in] request_handle	http������
  * @param[in] key				ͷ��key
  * @param[in] value			ͷ��value
  * @return void				�޷���ֵ
  */
NET_EXPORT void nim_http_add_request_header(HttpRequestHandle request_handle, const char* key, const char* value);

/** @fn void nim_http_set_request_progress_cb(HttpRequestHandle request_handle, nim_http_request_progress_cb progress_callback, const void* user_data)
  * NIM HTTP ���ý��Ȼص�
  * @param[in] request_handle		http������
  * @param[in] progress_callback	���Ȼص�����
  * @param[in] user_data			�Զ�������
  * @return void					�޷���ֵ
  */
NET_EXPORT void nim_http_set_request_progress_cb(HttpRequestHandle request_handle, nim_http_request_progress_cb progress_callback, const void* user_data);

/** @fn void nim_http_set_request_method_as_post(HttpRequestHandle request_handle)
  * NIM HTTP ǿ������http���󷽷�Ϊpost
  * @param[in] request_handle	http������
  * @return void				�޷���ֵ
  */
NET_EXPORT void nim_http_set_request_method_as_post(HttpRequestHandle request_handle);

/** @fn void nim_http_set_timeout(HttpRequestHandle request_handle, int timeout_ms)
  * NIM HTTP ���ó�ʱ
  * @param[in] request_handle	http������
  * @param[in] timeout_ms		��ʱʱ�䣬��λ�Ǻ���
  * @return void				�޷���ֵ
  */
NET_EXPORT void nim_http_set_timeout(HttpRequestHandle request_handle, int timeout_ms);

/** @fn void nim_http_set_proxy(HttpRequestHandle request_handle, int type, const char* host, short port, const char* user, const char* pass)
  * NIM HTTP ���ô���
  * @param[in] request_handle	http������
  * @param[in] type				��������NIMProxyType
  * @param[in] host				�����ַ
  * @param[in] port				����˿�
  * @param[in] user				�����˺�
  * @param[in] pass				��������
  * @return void				�޷���ֵ
  */
NET_EXPORT void nim_http_set_proxy(HttpRequestHandle request_handle, int type, const char* host, short port, const char* user, const char* pass);

}

#endif // NET_NIM_HTTP_H_