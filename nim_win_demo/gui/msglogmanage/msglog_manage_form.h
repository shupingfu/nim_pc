#pragma once

/** @class MsglogManageForm
  * @brief ��Ϣ��¼����\��������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/10/12
  */

enum MigrateMsglogOption	// ���뻹�ǵ���
{
	kExport,
	kImport
};

enum MigrateMsglogTarget	// ���ػ����ƴ���
{
	kLocal,
	kRemote
};

class MsglogManageForm : public nim_comp::WindowEx
{
public:
	MsglogManageForm();
	~MsglogManageForm();
	
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
	* ������������Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void OnFinalMessage(HWND hWnd);

	/**
	* ���ز�����ײ㴰����Ϣ
	* @param[in] uMsg ��Ϣ����
	* @param[in] wParam ���Ӳ���
	* @param[in] lParam ���Ӳ���
	* @return LRESULT ������
	*/
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	/**
	* ���ô��ڵ���\��������
	* @param[in] export_or_import true:������false:����
	* @return bool true �ɹ���false ʧ��
	*/
	bool SetType(nim::LogsBackupRemoteOperate option, MigrateMsglogTarget target);

	/**
	* ���ݵ��뵼�����ȵĻص�
	* @param[in] operate ���뵼������
	* @param[in] progress ����
	* @return void	�޷���ֵ
	*/
	void OnMigrateMsglogProgressCallbackUI(nim::LogsBackupRemoteOperate operate, float progress);

	/**
	* ���ݵ��뵼����ɵĻص�
	* @param[in] operate ���뵼������
	* @param[in] state ����״̬
	* @return void	�޷���ֵ
	*/
	void OnMigrateMsglogCompletedCallbackUI(nim::LogsBackupRemoteOperate operate, nim::LogsBackupRemoteState state);

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
	* ѡ�񵼳�\����·��
	* @return void	�޷���ֵ
	*/
	void SelectPath();

	/**
	* ��Ӧ·����ѡ��Ļص�����
	* @param[in] ret �Ƿ�ѡ����·��
	* @param[in] file_path ·��ֵ
	* @return void	�޷���ֵ
	*/
	void OnSelectPathCallback(BOOL ret, std::wstring file_path);

	/**
	* ִ�е�������
	* @param[in] path ����·��
	* @return void	�޷���ֵ
	*/
	void Export(const std::string& path);

	/**
	* �������ƶ�
	* @return bool	�Ƿ���Խ��е�������
	*/
	bool ExportToRemote();

	/**
	* ��Ӧ������ɵĻص�����
	* @param[in] res_code �������������
	* @return void	�޷���ֵ
	*/
	void OnExportCompeleteCallback(nim::NIMResCode res_code);

	/**
	* ִ�е������
	* @param[in] path ����·��
	* @return void	�޷���ֵ
	*/
	void Import(const std::string& path);

	/**
	* ���ƶ˵���
	* @return bool �Ƿ���Խ��е������
	*/
	bool ImportFromRemote();

	/**
	* ��Ӧ������̵Ļص�����
	* @param[in] imported_count �ѵ�������
	* @param[in] total_count ����
	* @return void	�޷���ֵ
	*/
	void OnImportProgressCallback(int64_t imported_count, int64_t total_count);

	/**
	* ��Ӧ������ɵĻص�����
	* @param[in] res_code ������������
	* @return void	�޷���ֵ
	*/
	void OnImportCompeleteCallback(nim::NIMResCode res_code);

	/**
	* �ӳ�һ��ʱ���رմ���
	* @return void	�޷���ֵ
	*/
	void DelayClose();

	/**
	* �����Ƿ�����ִ�е���\��������
	* @param[in] running true:����ִ�У�falseû��ִ��
	* @return void	�޷���ֵ
	*/
	void SetDbStatus(bool running);

public:
	static const LPCTSTR kClassName;

private:
	nbase::WeakCallbackFlag close_timer_;

	nim::LogsBackupRemoteOperate export_or_import_;
	bool db_running_;
	bool open_file_;

	ui::Label*		tip_text_;
	ui::Button*		btn_sel_;
	ui::Button*		btn_run_;
	ui::Button*		btn_cancel_;
	ui::RichEdit*	path_edit_;

	ui::Progress*	progress_;
	ui::Label*		result_text_;
	ui::Label*		progress_text_;
	ui::Box*		path_box_;
	ui::Box*		prg_box_;
	ui::Button*		btn_close_;
};
