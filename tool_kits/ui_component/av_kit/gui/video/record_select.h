#pragma once

namespace nim_comp
{
	typedef std::function<void(bool ret, std::wstring, std::wstring, std::wstring)> SelectRecordFileCallback;

/** @class RecordSelectForm
  * @brief �ṩѡ������Ƶͨ��ʱ¼��ѡ��Ĺ���
  * @copyright (c) 2017, NetEase Inc. All rights reserved
  * @author gq
  * @date 2017/02/15
  */
class RecordSelectForm : public ui::WindowImplBase
{
public:
	RecordSelectForm();
	~RecordSelectForm();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual UINT GetClassStyle() const override;

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

	/**
	* ���ز�����ײ㴰����Ϣ
	* @param[in] uMsg ��Ϣ����
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @return LRESULT ������
	*/
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	/**
	* �������пؼ���������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool Notify(ui::EventArgs* msg);

	/**
	* �������пؼ�������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnClicked(ui::EventArgs* msg);

	/**
	* �������пؼ���ѡ����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	virtual bool OnSelected(ui::EventArgs* msg);

	/**
	* ����ѡ�񴰿���ϵĻص�����
	* @param[in] video_mode �Ƿ�����Ƶģʽ
	* @param[in] cb �ص�����
	* @return void �޷���ֵ
	*/
	void SetSelFileCb(bool video_mode, SelectRecordFileCallback cb);

private:

public:
	static const LPTSTR kClassName;

private:
	SelectRecordFileCallback select_file_cb_; 
	ui::RichEdit* path_edit_;
	ui::CheckBox* mp4_sel_;
	ui::CheckBox* mp4_sel2_;
	ui::CheckBox* audio_sel_;
};
}