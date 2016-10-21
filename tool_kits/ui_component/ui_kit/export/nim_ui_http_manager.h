#pragma once

#include "nim_ui_dll.h"
#include "module/service/http_service.h"
#include "tool_kits/base/memory/singleton.h"

namespace nim_ui
{

/** @class HttpManager
  * @brief �ṩHttp������Դ�йصĽӿ�
  * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
  * @author Oleg
  * @date 2016/5/12
  */
class NIM_UI_DLL_API HttpManager
{
public:
	SINGLETON_DEFINE(HttpManager);
	HttpManager() {}
	~HttpManager() {}

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
};

}