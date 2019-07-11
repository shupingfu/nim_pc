#pragma once

/** @class LoginForm
  * @brief ��¼����
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @date 2016/10/12
  */
class LoginForm : public nim_comp::WindowEx
{
public:
	LoginForm();
	~LoginForm();
	
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
	* ���ز�����WM_CLOSE��Ϣ
	* @param[in] uMsg ��Ϣ����
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @param[in] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
	* @return LRESULT ������
	*/
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetAnonymousChatroomVisible(bool visible);
	void SetLoginFunctionFlag(){ login_function_ = true; }
	void SwitchToLoginPage();
private:
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
	* ע��UIKIT�ص���������UIKIT���Ƶ�¼�����һЩ��Ϊ
	* @return void	�޷���ֵ
	*/
	void RegLoginManagerCallback();

	/**
	* ��Ӧ��¼����Ļص���
	* @return void	�޷���ֵ
	*/
	void OnLoginError(int error);

	/**
	* ��Ӧȡ����¼�Ļص������ý���ؼ�Ч��
	* @return void	�޷���ֵ
	*/
	void OnCancelLogin();

private:
	/**
	* ����û��������ݣ��������Ϸ���ʼ��¼
	* @return void	�޷���ֵ
	*/
	void DoBeforeLogin();

	/**
	* ����û��������ݣ��������Ϸ���ע���ʺ�
	* @return void	�޷���ֵ
	*/
	void DoRegisterAccount();

	/**
	* ִ�е�¼����
	* @param[in] username �û�id
	* @param[in] password �û�����
	* @return void	�޷���ֵ
	*/
	void StartLogin(std::string username, std::string password);

	/**
	* �ڽ�������ʾһЩ��ʾ����
	* @return void	�޷���ֵ
	*/
	void ShowLoginTip(std::wstring tip_text);
	/**
	* ����uikit��ʼ��
	* @param[in] mode ��ʼ������(IM/����������)
	* @return void	�޷���ֵ
	*/
	void DoInitUiKit(nim_ui::InitManager::InitMode mode);
	
public:
	void OnLoginOK();

private:
	void InitLoginData();
	void OnLogin();
	void OnLogin_i();
	bool InitSDK(const std::string& pravate_settings_file_path = "");
	void CheckAutoLogin();
	bool OnSwitchToLoginPage();
public:
	static const LPCTSTR kClassName;
	bool login_function_;
	bool anonymous_chatroom_login_;
private:
	ui::Control*	usericon_;
	ui::Control*	passwordicon_;
	ui::RichEdit*	user_name_edit_;
	ui::RichEdit*	nick_name_edit_;
	ui::RichEdit*	password_edit_;
	ui::Control*	login_ing_tip_;
	ui::Label*		login_error_tip_;
	ui::Label*		register_ok_toast_;
	ui::Button*		btn_login_;
	ui::Button*		btn_register_;
	ui::Button*		btn_cancel_;
	ui::CheckBox*	remember_pwd_ckb_ = nullptr;
	ui::CheckBox*	remember_user_ckb_ = nullptr;
	ui::CheckBox* use_new_uistyle_;
	ui::CheckBox* use_private_settings_;
	ui::CheckBox* chkbox_private_use_proxy_enable_;
	ui::RichEdit* private_settings_url_;

	UnregisterCallback sdk_init_autounreg_cb_;

};
