#pragma once
#include "base/http/http_protocol_interface_define.h"
namespace app_sdk
{	
	/** @class SDK_PRO
	* @brief app_sdk Э�鶨��
	* @copyright (c) 2018, NetEase Inc. All rights reserved
	* @date 2018/4/26
	*/
	class SDK_PRO
	{
	private:	
		/** @class ResponseBase
		* @brief app_sdk Ӧ�����
		* @copyright (c) 2018, NetEase Inc. All rights reserved
		* @date 2018/4/26
		*/
		class ResponseBase : public IHttpResponse
		{
		public:			
			ResponseBase() = default;
			virtual ~ResponseBase() = default;
			//����Ӧ������
			virtual void Parse(const std::string& response) override;
			//��ȡԭʼӦ������
			std::string GetReplyContent() const;
			//��ȡЭ�鶨���ҵ�񷵻���
			virtual int32_t GetProtocolReplyCode() const;
		protected:
			virtual void OnParse(const std::string& response);
			void SetProtocolReplyCode(int code);
		private:
			std::string reply_content_;//Ӧ��ԭʼ����
			int32_t pro_reply_code_;//ҵ�񷵻���
		};
		/** @class RequestBase
		* @brief app_sdk �������
		* @copyright (c) 2018, NetEase Inc. All rights reserved
		* @date 2018/4/26
		*/
		class RequestBase : public IHttpRequest
		{
		public:
			RequestBase() = default;
			virtual ~RequestBase() = default;
			virtual std::string GetAPIURL() const override;
			virtual bool UsePostMethod() const override;
			virtual void GetRequestContent(std::string& content) const override;
			virtual void GetRequestHead(std::map<std::string, std::string>& heads) override;
		protected:
			virtual bool IsUsePostMethod() const;
			virtual void OnGetRequestContent(std::string& content) const;
			//��ȡ��������ӵ�ַ������ֱ�ӷ��أ�������OnGetHost��OnGetApiƴ���������Բο�ʾ������
			virtual std::string OnGetAPIURL() const;
			virtual std::string OnGetHost() const;
			virtual std::string OnGetAPI() const;
			virtual void OnGetRequestHead(std::map<std::string, std::string>& heads) const;
		private:
			mutable std::string app_url_;
		};
		//ע���˺�����/Ӧ��
		class RegisterAccountRequest : public RequestBase
		{		
		public:
			RegisterAccountRequest(std::string username, std::string password, std::string nickname);
		protected:
			virtual std::string OnGetHost() const override;
			virtual std::string OnGetAPI() const override;
			virtual void OnGetRequestHead(std::map<std::string, std::string>& heads) const override;
			virtual void OnGetRequestContent(std::string& content) const override;
		public:
			std::string username_;
			std::string password_;
			std::string nickname_;
		};
		class RegisterAccountResponse : public ResponseBase
		{
		protected:
			virtual void OnParse(const std::string& response) override;
		public:
			std::string err_msg_;
		};
		//��ȡ�������б�����/Ӧ��
		class GetChatroomListRequest : public RequestBase
		{
		protected:
			virtual std::string OnGetAPIURL() const;
			virtual std::string OnGetAPI() const override;		
			virtual bool IsUsePostMethod() const override;
		};
		class GetChatroomListResponse : public ResponseBase
		{
		protected:
			virtual void OnParse(const std::string& response) override;
		public:
			std::vector<nim_chatroom::ChatRoomInfo> chatroom_list_;
		};
		//��ȡ���������ӵ�ַ����/Ӧ��
		class GetChatroomAddressRequest : public RequestBase
		{
		public:
			GetChatroomAddressRequest();
		protected:
			virtual std::string OnGetAPIURL() const;
			virtual std::string OnGetAPI() const override;
			virtual void OnGetRequestContent(std::string& content) const override;
		public:
			__int64 room_id_;
			std::string uid_;
			int type_;
		};
		class GetChatroomAddressResponse : public ResponseBase
		{
		protected:
			virtual void OnParse(const std::string& response) override;
		public:
			std::list<std::string> address_;	/**< �����ҵ�ַ����ַͨ��Ӧ�÷������ӿڻ�ȡ */
		};
	public:
		/****************************���Ⱪ¶����*****************************/

		//ע���˺�����/Ӧ��
		using RegisterAccountReq = TSharedHttpRequest<RegisterAccountRequest>;
		using RegisterAccountRsp = TSharedHttpResponse<RegisterAccountResponse>;
		//��ȡ�������б�����/Ӧ��
		using GetChatroomListReq = TSharedHttpRequest<GetChatroomListRequest>;
		using GetChatroomListRsp = TSharedHttpResponse<GetChatroomListResponse>;
		//��ȡ�������б�����/Ӧ��
		using GetChatroomAddressReq = TSharedHttpRequest<GetChatroomAddressRequest>;
		using GetChatroomAddressRsp = TSharedHttpResponse<GetChatroomAddressResponse>;
	};	
}
using app_sdk_pro = app_sdk::SDK_PRO;