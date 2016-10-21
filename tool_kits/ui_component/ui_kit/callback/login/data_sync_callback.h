#pragma once
#include "nim_client_helper.h"

namespace nim_comp
{
/** @class DataSyncCallback
  * @brief ����ͬ���¼��ص���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/09/13
  */
class DataSyncCallback
{
public:

	/**
	* ע������ͬ����ɵĻص�����
	* @param[in] sync_type ����ͬ������
	* @param[in] status ����ͬ��״̬
	* @param[in] data_sync_info ��������
	* @return void �޷���ֵ
	*/
	static void SyncCallback(nim::NIMDataSyncType sync_type, nim::NIMDataSyncStatus status, const std::string &data_sync_info);
};
}