#pragma once
/** @file http_protocol_interface_define.h
* @brief app_sdk ����http�����Э������������Ͷ���
* @copyright (c) 2015-2018, NetEase Inc. All rights reserved
* @date 2018/4/26
*/
namespace app_sdk
{	
	template<typename THttpRequest>
	using TSharedHttpRequest = std::shared_ptr<typename THttpRequest>;

	template<typename THttpResponse>
	using TSharedHttpResponse = std::shared_ptr<typename THttpResponse>;

	template<typename TSharedHttpRequest, typename TSharedHttpResponse>
	using THttpCallback = std::function<void(const typename TSharedHttpRequest&, const typename TSharedHttpResponse&)>;
	
	class SDKManager;
	/** @class IHttpRequest
	* @brief app_sdk ����ӿڶ���
	* @copyright (c) 2018, NetEase Inc. All rights reserved
	* @date 2018/4/26
	*/
	class IHttpRequest
	{
	public:
		IHttpRequest() = default;
		virtual ~IHttpRequest() = default;
		//�Ƿ�ʹ��post����
		virtual bool UsePostMethod() const;
		//http�����ַ
		virtual std::string GetAPIURL() const = 0;	
		//����ĳ�ʱʱ��
		virtual int GetRequestTimeout() const;
		//��ȡ��������
		virtual void GetRequestContent(std::string& content) const = 0;
		//��ȡ����ʱhttpͷҪ�������Ϣ
		virtual void GetRequestHead(std::map<std::string, std::string>& heads) = 0;
	private:
		static const uint32_t request_time_out_;
	};
	/** @class IHttpRequest
	* @brief app_sdk Ӧ��ӿڶ���
	* @copyright (c) 2018, NetEase Inc. All rights reserved
	* @date 2018/4/26
	*/
	class IHttpResponse
	{
		friend class SDKManager;
	public:	
		IHttpResponse();
		virtual ~IHttpResponse();
		virtual void Parse(const std::string& response) = 0;	//����Ӧ��		
		//��ȡhttp����Ӧ����
		int GetResponseCode() const;
	private:
		void SetResponseCode(int code);
	protected:
		int response_code_;//http�����Ӧ����
	};	

	//����һ���̳���IHttpRequest��http����
	template<typename TSharedHttpRequest, typename... TParams>
	static auto CreateHttpRequest(const TParams&... params)->
		std::shared_ptr<typename TSharedHttpRequest::element_type>
	{
		using TRequest = typename TSharedHttpRequest::element_type;
		static_assert(std::is_base_of<IHttpRequest, TRequest>::value, "app_sdk::IHttpRequest should be base of TRequest");
		return std::make_shared< TRequest>(params...);
	}
}