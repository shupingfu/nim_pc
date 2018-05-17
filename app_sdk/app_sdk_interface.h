#pragma once
#include "app_sdk_define.h"
namespace app_sdk
{
	class AppSDKInterface : public nbase::SupportWeakCallback
	{
	public:
		AppSDKInterface() = default;
		~AppSDKInterface() = default;
		SINGLETON_DEFINE(AppSDKInterface);
		/**
		* ��ȡ���ӷ�������ĳһ��������Ϣ
		* @param[in] key ��Ҫ��ȡ����Ϣ�ؼ���
		* @return string ������Ϣ
		*/
		static std::string GetConfigValue(const std::string& key);
		/**
		* ��ȡӦ�õ�app key
		* @return string ������Ϣapp key
		*/
		static std::string GetAppKey();
		/**
		* Ӧ�õ�app key�Ƿ�Ϊ��������Demo�����app key
		* @return bool true:��,false:��
		*/
		static bool IsNimDemoAppKey(const std::string& appkey);
	public:		
		/**
		* ��ȡӦ�÷���http�����host,host��apiƴ�ӳ�һ��������http�����ַ
		*�����߿����޸�Ϊ����Ӧ�õ�host
		* @return string http�����host
		*/
		std::string GetAppHost();
		/**
		* ע��һ�����˺�
		* @param[in] username Ҫ�û���
		* @param[in] password ����
		* @param[in] cb ��ʽ����ϵĻص�֪ͨ����
		* @return void	�޷���ֵ
		*/
		void InvokeFormatAccountAndPassword(const std::string &username, const std::string &password, const OnFormatAccountAndPasswordCallback& cb);
		/**
		* ע��һ�����˺�
		* @param[in] username Ҫע����û���
		* @param[in] password �û�����
		* @param[in] nickname �û��ǳ�
		* @param[in] cb ע����ϵĻص�֪ͨ����
		* @return void	�޷���ֵ
		*/
		void InvokeRegisterAccount(const std::string &username, const std::string &password, const std::string &nickname, const OnRegisterAccountCallback& cb);
		/**
		* ��ȡ�������б�
		* @param[in] cb �ص�֪ͨ����
		* @return void	�޷���ֵ
		*/
		void InvokeGetChatroomList(const OnGetChatroomListCallback& cb);
		/**
		* ��ȡ���������ӵ�ַ,������������¼ʱ
		* @param[in] room_id ������ID
		* @param[in] uid �û�UID
		* @param[in] type ���������ҵ�ģʽ Logined = 1,Anonymous = 2,		
		* @param[in] cb �ص�֪ͨ����
		* @return void	�޷���ֵ
		*/
		void InvokeGetChatroomAddress(__int64 room_id,const std::string& uid,int type, const OnGetChatroomAddressCallback& cb);

		
	private:
		static const std::string kAppKey;		
		static const std::string kAppHost;
	};

}