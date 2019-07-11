#pragma once
#include "module/thread/misc_thread.h"
#include "module/thread/db_thread.h"
#include "shared/ui/msgbox.h"

/** @class MainThread
  * @brief ���̣߳�UI�̣߳��࣬�̳�nbase::FrameworkThread
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author towik
  * @date 2015/1/1
  */
class MainThread :
	public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}
private:
	/**
	* �������ʱ�Ĵ�����
	* @param[in] ret	�û��Ĵ���ʽ���رջ��������г���
	* @return void	�޷���ֵ
	*/
	void OnMsgBoxCallback(MsgBoxRet ret, const std::string& dmp_path);

	/**
	* �麯������ʼ�����߳�
	* @return void	�޷���ֵ
	*/
	virtual void Init() override;

	/**
	* �麯�������߳��˳�ʱ����һЩ������
	* @return void	�޷���ֵ
	*/
	virtual void Cleanup() override;

	/**
	* ���߳̿�ʼѭ��ǰ��misc�̺߳�db�߳��ȿ�ʼѭ��
	* @return void	�޷���ֵ
	*/
	void PreMessageLoop();

	/**
	* ���߳̽���ѭ��ǰ��misc�̺߳�db�߳��Ƚ���ѭ��
	* @return void	�޷���ֵ
	*/
	void PostMessageLoop();
private:
	std::unique_ptr<MiscThread>	misc_thread_;
	std::unique_ptr<MiscThread>	screen_capture_thread_;
	std::unique_ptr<DBThread>	db_thread_;
	std::unique_ptr<MiscThread>	app_sdk_thread_;
};
class NimAPP : public nbase::Singleton< NimAPP,false>
{
	SingletonHideConstructor(NimAPP)
private:
	NimAPP() = default;
	~NimAPP() = default;
public:
	int Exec(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow);
	bool InitNim(const std::string& server_conf_file_path = "");
	void CleanupSDKBeforLogin();
private:
	int InitEnvironment(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow);
	//int InitRedistPackages();
	int UninitEnvironment();
	int InitInstance(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow);
	int ExitInstance();
	void CheckRestartApp();
	std::unique_ptr<CComModule> com_module_;
};

