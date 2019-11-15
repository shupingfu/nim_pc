#pragma once
#include "ui_kit/module/multi_retweet/multi_retweet_define.h"
namespace nim_comp
{
	class MultiMessageRetweetInfo
	{
	public:
		MultiMessageRetweetInfo();
		virtual ~MultiMessageRetweetInfo() = default;
	public:
		std::wstring decrypt_path_;
		std::wstring encrypt_path_;
		std::wstring migrate_temp_directory_;
		std::string url_;
		std::string tag_;
		MultiMessageRetweetProgressCallback progreess_callback_;
		MultiMessageRetweetCompleteCallback complete_callback_;
	};
	using MultiMessageRetweetInfoPtr = std::shared_ptr<MultiMessageRetweetInfo>;

	// ------------------------------------------------------------------- ExportInfo

	class MultiMessageRetweetExportInfo : public MultiMessageRetweetInfo
	{
	public:
		MultiMessageRetweetExportInfo();
		~MultiMessageRetweetExportInfo() = default;
	public:
		MultiMessageRetweetCustomEncryptExportFileCallback encrypt_callback_;
	};
	using ExportInfoPtr = std::shared_ptr<MultiMessageRetweetExportInfo>;

	// ------------------------------------------------------------------- ImportInfo

	class MultiMessageRetweetImportInfo : public MultiMessageRetweetInfo
	{
	public:
		MultiMessageRetweetImportInfo();
		~MultiMessageRetweetImportInfo() = default;
	public:
		MultiMessageRetweetCustomDecryptImportFileCallback decrypt_callback_;
	};
	using ImportInfoPtr = std::shared_ptr<MultiMessageRetweetImportInfo>;

	class MultiMessageRetweetRawFileInfo
	{
	public:
		class Head
		{
		public:
			Head() :version_(0), terminal_(4)
			{
			}
		public:
			int version_;//����,�ļ���ʽ�汾
			int terminal_;//ѡ�� �ն����� PC�ͻ��˿ɹ̶���4
			std::string sdk_version_;//ѡ�� ����SDK�汾��
			std::string app_version_;//ѡ�� Ӧ�ð汾�ţ�������demo�İ汾�ţ�
			int message_count_;//����,����д���ļ��е���Ϣ��
		};
		class Body
		{
		public:
			std::list<nim::IMMessage> message_list_;
		};
	public:
		Head head_;
		Body body_;
	};
}