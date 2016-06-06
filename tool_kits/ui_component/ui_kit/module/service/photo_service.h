#pragma once
#include "shared/threads.h"
#include "shared/auto_unregister.h"

enum PhotoType
{
	kUser,
	kTeam,
};

typedef std::function<void(PhotoType type, const std::string& id, const std::wstring &photo_path)> OnPhotoReadyCallback;

namespace nim_comp
{

class PhotoService : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(PhotoService);

private:
	PhotoService() {}
	~PhotoService() {}

public:
	UnregisterCallback RegPhotoReady(const OnPhotoReadyCallback& callback); // ע��ͷ��������ɵĻص�
	std::wstring GetUserPhoto(const std::string &accid); // ��UserService::all_user_�в�ѯ�û�ͷ��
	std::wstring GetTeamPhoto(const std::string &tid, bool full_path = true);

public:
	//������
	void DownloadUserPhoto(const nim::UserNameCard &info); //��ȡ�û���Ϣ�󣬻����û���Ϣ�޸ĺ������û�ͷ��
	void DownloadTeamIcon(const nim::TeamInfo &info);
	std::wstring GetPhotoDir(PhotoType type);

private:
	bool CheckPhotoOK(std::wstring photo_path); // ���ĳ��ͷ��ͼƬ�Ƿ����
	int CheckForDownload(PhotoType type, const std::string& url);

private:
	std::map<int, std::unique_ptr<OnPhotoReadyCallback>> photo_ready_cb_list_; //�û�ͷ��������ɻص��б�
};


}