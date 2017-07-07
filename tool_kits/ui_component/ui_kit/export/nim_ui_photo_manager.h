#pragma once

#include "nim_ui_dll.h"
#include "module/service/photo_service.h"
#include "tool_kits/base/memory/singleton.h"

namespace nim_ui
{

/** @class PhotoManager
  * @brief �ṩͷ����Դ�йصĽӿ�
  * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
  * @author Oleg
  * @date 2016/5/21
  */
class NIM_UI_DLL_API PhotoManager
{
public:
	SINGLETON_DEFINE(PhotoManager);

public:

	/**
	* ��ȡ�û�ͷ��
	* @param[in] accid �û�id
	* @return wstring ͷ���·��
	*/
	std::wstring GetUserPhoto(const std::string &accid);

	/**
	* ��ȡȺ��ͷ��
	* @param[in] tid Ⱥid
	* @param[in] full_path �Ƿ���Ҫ����·��
	* @return wstring ͷ���·��
	*/
	std::wstring GetTeamPhoto(const std::string &tid, bool full_path = true);

	/**
	* ע��ͷ��������Ϻ�Ļص�����
	* @param[in] callback �ص�����
	* @return UnregisterCallback ��ע�����
	*/
	UnregisterCallback RegPhotoReady(const OnPhotoReadyCallback& callback);

private:
	PhotoManager() {}
	~PhotoManager() {}
};

}