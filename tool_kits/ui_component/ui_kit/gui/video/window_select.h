#pragma once

namespace nim_comp
{
typedef std::function<void(HWND, bool cut)> SelectWndCallback;

/** @class WindowSelectForm
  * @brief �ṩö�ٲ�ѡ�񴰿ڵĹ���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author gaoqi
  * @date 2016/09/21
  */
class WindowSelectForm : public nim_comp::WindowEx
{
public:
	WindowSelectForm();
	~WindowSelectForm();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
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
	* @param[in] cb �ص�����
	* @return void �޷���ֵ
	*/
	void SetSelWndCb(SelectWndCallback cb){ select_wnd_cb_ = cb; }

private:
	/**
	* ˢ�´����б�
	* @return void	�޷���ֵ
	*/
	void RefreshWndlist();

public:
	static const LPTSTR kClassName;

private:
	SelectWndCallback select_wnd_cb_;
	ui::ListBox* wnd_list_;
};
}