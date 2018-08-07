/** @file nim_tools_http_def.h
* @brief NIM HTTP�ṩ��http������ؽӿڵĶ���
* @copyright (c) 2015-2016, NetEase Inc. All rights reserved
* @author towik, Oleg
* @date 2015/4/30
*/
#ifndef NIM_TOOLS_HTTP_DEF_H
#define NIM_TOOLS_HTTP_DEF_H
#ifdef __cplusplus
extern"C"
{
#endif

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
#define INVALID_HTTPREQUESTHANDLE (0)
/** @typedef int HttpRequestID
* http����ID���������ͺ�����ɴ�ID��ȡ��������ȡӦ��ͷ��ʹ�ø�ID
*/
typedef int HttpRequestID;
#define INVALID_HTTPREQUESTID (0xFFFFFFFF)
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

/** @typedef void (*nim_http_request_progress_cb)(const void* user_data, double total_upload_size, double uploaded_size, double total_download_size, double downloaded_size)
  * nim callback function that has been registered in nim_http_set_request_progress_cb API
  * @param[out] user_data				�ش����Զ�������
  * @param[out] total_upload_size		�ܵĴ��ϴ����ֽ���
  * @param[out] uploaded_size			�Ѿ��ϴ����ֽ���
  * @param[out] total_download_size		�ܵĴ����ص��ֽ���
  * @param[out] downloaded_size			�Ѿ����ص��ֽ���
  * @return void						�޷���ֵ
  */
typedef void(*nim_http_request_progress_cb)(const void* user_data, double total_upload_size, double uploaded_size, double total_download_size, double downloaded_size);

/** @typedef void (*nim_http_request_transfer_cb)(const void* user_data, double total_upload_size, double uploaded_size, double total_download_size, double downloaded_size)
  * nim callback function that has been registered in nim_http_set_request_transfer_cb API
  * @param[out] user_data				�ش����Զ�������
  * @param[out] actual_upload_size		ʵ���ϴ����ֽ���
  * @param[out] upload_speed			ƽ���ϴ��ٶȣ��ֽ�ÿ�룩
  * @param[out] actual_download_size	ʵ�����ص��ֽ���
  * @param[out] download_speed			ƽ�������ٶȣ��ֽ�ÿ�룩
  * @return void						�޷���ֵ
  */
typedef void(*nim_http_request_transfer_cb)(const void* user_data, double actual_upload_size, double upload_speed, double actual_download_size, double download_speed);

/** @typedef void (*nim_http_request_speed_cb)(const void* user_data, double, double)
  * nim callback function that has been registered in nim_http_set_request_speed_cb API
  * @param[out] user_data		�ش����Զ�������
  * @param[out] upload_speed	ʵʱ�ϴ��ٶȣ��ֽ�ÿ�룩
  * @param[out] download_speed	ʵʱ�����ٶȣ��ֽ�ÿ�룩
  * @return void				�޷���ֵ
  */
typedef void(*nim_http_request_speed_cb)(const void* user_data, double upload_speed, double download_speed);

#ifdef __cplusplus
};
#endif //__cplusplus
#endif//NIM_TOOLS_HTTP_DEF_H
