#pragma once

namespace nim_comp
{
typedef std::function<void(int rescode, const std::string& sid, const std::string& file_path, const std::string& file_ext, long file_size, int audio_duration)> OnCaptureCompeleteCallback;

/** @class AudioCaptureView
  * @brief �ڻỰ��������ʾ����¼�ƽ���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/9/12
  */
class AudioCaptureView : public ui::ButtonBox
{
public:
	AudioCaptureView();

	/**
	* ��ʼ������¼�ƹ��ܽ���
	* @param[in] session_id ����¼�ƽ��������Ựid
	* @param[in] callback ����¼����ϵĻص�֪ͨ����
	* @return void �޷���ֵ
	*/
	void InitControl(const std::string& session_id, OnCaptureCompeleteCallback callback);

	/**
	* ��ʾ(����)����¼�ƽ���
	* @param[in] show ��ʾ(����)����¼�ƽ���
	* @return void �޷���ֵ
	*/
	void ShowControl(bool show);
private:
	/**
	* ��Ӧ��ť������Ϣ
	* @param[in] param �������İ�ť�������Ϣ
	* @return bool ����ֵtrue: �������ݿؼ���Ϣ�� false: ֹͣ���ݿؼ���Ϣ
	*/
	bool OnClicked(ui::EventArgs* param);
	
	/**
	* ��ʼ¼�������Ļص�
	* @param[in] session_id ������ʼ¼�����ܵĻỰid
	* @param[in] code ������
	* @return void �޷���ֵ
	*/
	void OnStartCaptureCallback(const std::string& session_id, int code);

	/**
	* ¼��������ɵĻص�
	* @param[in] rescode ���������һ������200
	* @param[in] sid �Ựid
	* @param[in] cid ��Ϣid
	* @param[in] file_path �����ļ�����·��
	* @param[in] file_ext �ļ���չ��
	* @param[in] file_size �ļ���С
	* @param[in] audio_duration ����ʱ��
	* @return void �޷���ֵ
	*/
	void OnStopCaptureCallback(int rescode, const std::string& sid, const std::string& cid, const std::string& file_path, const std::string& file_ext, long file_size, int audio_duration);

	/**
	* ȡ��¼�������Ļص�
	* @param[in] session_id ����ȡ��¼�����ܵĻỰid
	* @param[in] code ������
	* @return void �޷���ֵ
	*/
	void OnCancelCaptureCallback(const std::string& session_id, int code);

	/**
	* ö�ٱ���¼���ɼ��豸�Ļص�
	* @param[in] rescode ���������һ������200
	* @param[in] device_list �豸�б�
	*/
	void OnEnumCaptureDeviceCallback(int rescode, const std::wstring& device_list);

	/**
	* ¼�Ʋ���ʱ������¼�ƽ��ȵĻص�����
	* @return void �޷���ֵ
	*/
	void OnTick();

private:
	std::string		session_id_;
	OnCaptureCompeleteCallback compelete_callback_;

	bool			need_cancel_;
	std::string		msg_uuid_;

	ui::Label		*time_text_;
	ui::Label		*tip_text;
	ui::Button		*button_start_;
	ui::Button		*button_stop_;
	ui::Button		*button_cancel_;

	nbase::WeakCallbackFlag timer_;
	int				tick_;
};
}