
#pragma once

#include "base/synchronization/lock.h"
#include "base/base_types.h"
#include <string>

namespace nim_comp
{
/** @class AudioFrameMng
  * @brief ��Ƶ֡������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/09/19
  */
class AudioFrameMng
{
public:
	AudioFrameMng();
	~AudioFrameMng();

	/**
	* ���������Ƶ֡����
	* @return void	�޷���ֵ
	*/
	void Clear();

	/**
	* ���һ����Ƶ֡����
	* @param[in] capture �Ƿ�Ϊ¼����Ƶ��֡
	* @param[in] time ʱ���
	* @param[in] data ֡����
	* @param[in] size ֡���ݴ�С
	* @param[in] json ��������Ƶ�ʵ�json��
	* @return void	�޷���ֵ
	*/
	void AddAudioFrame(bool capture, int64_t time, const char* data, int size, const std::string& json);

	/**
	* ��ȡһ����Ƶ֡����
	* @param[in] capture �Ƿ�Ϊ¼����Ƶ��֡
	* @param[out] out_data ֡����
	* @param[out] rate ����Ƶ��
	* @return void	�޷���ֵ
	*/
	void GetAudioFrame(bool capture, std::string& out_data, int& rate);
private:
	nbase::NLock  lock_;
	std::string capture_audio_data_;
	int64_t capture_time_ = 0;
	std::string recv_audio_data_;
	int64_t recv_time_ = 0;
	int32_t rate_rec_;
	int32_t rate_capture_;
};
}