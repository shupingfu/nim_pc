#pragma once
#include "shared/threads.h"
#include "shared/auto_unregister.h"

enum HttpResourceType
{
	kUndefined = 0,
	kImage = 1,
	kChatroomMemberIcon = 2,
};

typedef std::function<void(HttpResourceType type, const std::string& id, const std::wstring &res_path)> OnDownloadCompleteCallback;
typedef std::function<void(HttpResourceType type, const std::string& id, const std::wstring &local_path, const std::string& url)> OnUploadCompleteCallback;

namespace nim_comp
{
	/** @class HttpService
	  * @brief ��Դ���ط���
	  * @copyright (c) 2016, NetEase Inc. All rights reserved
	  * @date 2016/09/18
	  */
	class HttpService : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(HttpService);

	private:
		HttpService() {}
		~HttpService() {}

	public:
		/**
		* ����ͼƬ��Դ
		* @param[in] type ��Դ����
		* @param[in] id ��ԴID����Ϊʶ���ʶ
		* @param[in] url ���ص�ַ
		* @return wstring ͼƬ·��
		*/
		std::wstring GetCustomImage(HttpResourceType type, const std::string &id, const std::string &url);

		/**
		* ע����Դ������ɵĻص�
		* @param[in] callback ������ɵĻص�����
		* @return UnregisterCallback ��ע�����
		*/
		UnregisterCallback RegDownloadComplete(const OnDownloadCompleteCallback& callback);

	private:
		/**
		* ������Դ�ļ�
		* @param[in] type ��Դ����
		* @param[in] id ��ԴID����Ϊʶ���ʶ
		* @param[in] url ���ص�ַ
		* @return wstring ͼƬ·��
		* @return void	�޷���ֵ
		*/
		void DownloadResource(HttpResourceType type, const std::string &id, const std::string &url);

		/**
		* ���ͼƬ�Ƿ���
		* @param[in] image_path ͼƬ·��
		* @return bool true �ɹ���false ʧ��
		*/
		bool CheckImageValid(std::wstring image_path);

		/**
		* ��ȡhttp�ļ�����·��
		* @return wstring ����·��
		*/
		std::wstring GetHttpCachedFileDir();

	private:
		std::map<int, std::unique_ptr<OnDownloadCompleteCallback>> download_ready_cb_list_;
	};


}