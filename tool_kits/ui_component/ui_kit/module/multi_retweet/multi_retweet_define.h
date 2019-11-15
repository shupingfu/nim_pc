#pragma once
namespace nim_comp
{	
	enum class MultiMessageRetweetState
	{
		MultiMessageRetweetState_Begin = -2,
		MultiMessageRetweetState_UnDef,			// δ����
		MultiMessageRetweetState_Start,				// ��ʼ
		MultiMessageRetweetState_ExportRaw,	// �������ļ�
		MultiMessageRetweetState_Uploading,			// �ϴ���nos
		MultiMessageRetweetState_Downloading,	// ��nos�����ļ�
		MultiMessageRetweetState_Canceled = 5,	// ��ȡ��
		MultiMessageRetweetState_Error,					// ����
		//����״̬
		MultiMessageRetweetState_IMP_NoFile,//û�б����ļ�
		MultiMessageRetweetState_IMP_DownloadFailed,//���ر����ļ�����
		MultiMessageRetweetState_IMP_RAWError,//����/��ѹ������Դ�ļ���ʽ����
		MultiMessageRetweetState_IMP_ParseRAWError,//����Դ�ļ���ʽ����
		//����״̬
		MultiMessageRetweetState_EXP_RAWError,//������Դ�ļ�ʧ��
		MultiMessageRetweetState_EXP_UploadFailed,//�ϴ��ļ�����

		MultiMessageRetweetState_Done,				// ���
		MultiMessageRetweetState_End,
		MultiMessageRetweetState_FinalState_Begin = MultiMessageRetweetState_Canceled//�Ƿ���������״̬��һ����ʶ�������ж�state�Ƿ�Ϊ��̬(state >= MultiMessageRetweetState_FinalState_Begin)

	};
	using MultiMessageRetweetProgressCallback = std::function<void(float)>;
	using MultiMessageRetweetCompleteCallback = std::function<void(MultiMessageRetweetState)>;
	using MultiMessageRetweetCustomEncryptExportFileCallback = std::function<bool(const std::wstring& file_path, const std::wstring& temp_dir,const std::string& encrypt_key)>;
	using MultiMessageRetweetCustomDecryptImportFileCallback = std::function<bool(const std::wstring& file_path, const std::wstring& temp_dir, const std::string& encrypt_key)>;

	
}