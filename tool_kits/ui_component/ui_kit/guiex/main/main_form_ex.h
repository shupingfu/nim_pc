#pragma once
#include "module/plugins/main_plugins_base.h"
#include "module/tray_icon/tray_icon_manager.h"
namespace nim_comp
{
	class IMainPlugin;
	class IMainFormMenuHandler;
	class MainFormEx : public WindowEx, public ITrayIconEventHandler
	{
	public:
		MainFormEx(IMainFormMenuHandler* main_menu_handler);
		virtual ~MainFormEx();
	public:
		//�����麯��
		virtual std::wstring GetSkinFolder() override;
		virtual std::wstring GetSkinFile() override;
		virtual std::wstring GetWindowClassName() const override;
		virtual std::wstring GetWindowId() const override;	

		/**
		* ���ڳ�ʼ������
		* @return void	�޷���ֵ
		*/
		virtual void InitWindow() override;
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		/**
		* ������������Ϣ
		* @return void	�޷���ֵ
		*/
		virtual void OnFinalMessage(HWND hWnd);

		/**
		* ���ز�����WM_CLOSE��Ϣ
		* @param[in] wParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in] lParam ���Ӳ���
		* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
		* @return void	�޷���ֵ
		*/
		LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		/**
		* ����ESC������Ϣ����
		* @param[in out] bHandled �Ƿ�������Ϣ����������˲�����������Ϣ
		* @return void	�޷���ֵ
		*/
		virtual void OnEsc(BOOL &bHandled);
	protected:
		//����
		virtual bool OnLeftClick() override;
		virtual bool OnLeftDoubleClick() override;
		virtual bool OnRightClick() override;
	private:
		void LoadPlugins();
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
		* ��Ӧ�������ϸı�Ļص�����
		* @param[in] uinfos �µĸ��������б�
		* @return void �޷���ֵ
		*/
		void OnUserInfoChange(const std::list<nim::UserNameCard> &uinfos);

		/**
		* ��Ӧͷ��������ɵĻص�����
		* @param[in] type ͷ������
		* @param[in] account ͷ��������ɵ��û�id
		* @param[in] photo_path ͷ�񱾵�·��
		* @return void �޷���ֵ
		*/
		void OnUserPhotoReady(PhotoType type, const std::string& account, const std::wstring& photo_path);		

		/**
		* ��ʼ���û�ͷ��
		* @return void	�޷���ֵ
		*/
		void InitHeader();

		bool OnReturnEventsClick(ui::EventArgs* param);
		/**
		* �������̲˵�
		* @param[in] point ���λ��
		* @return void
		*/
		void PopupTrayMenu(POINT point);
		/**
		* ����״̬�˵���ť�ĵ�����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnlineStateMenuButtonClick(ui::EventArgs* param);
		/**
		* ����״̬�˵���ĵ�����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnlineStateMenuItemClick(ui::EventArgs* param);
		/**
		* ��������״̬
		* @return void	�޷���ֵ
		*/
		void SetOnlineState();
		/**
		* ���������ڵ����˵���ť�ĵ�����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool MainMenuButtonClick(ui::EventArgs* param);
		/**
		* ��ʼ��������		
		* @return void
		*/
		void InitSearchBar();
		/**
		* �������������ݸı����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool SearchEditChange(ui::EventArgs* param);

		/**
		* ����������������ť�ĵ�����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnClearInputBtnClicked(ui::EventArgs* param);
		void OnWndSizeMax(bool max);
	public:
		static const LPCTSTR kClassName;
	private:
		bool is_trayicon_left_double_clicked_;
		bool			is_busy_;
		ui::VBox* main_bar_;
		ui::VBox* simple_plugin_bar_;
		ui::VBox* main_plugins_bar_;
		ui::TabBox* main_plugins_showpage_;
		ui::Button*		btn_header_;
		ui::Button*		btn_online_state_;
		ui::Label*		label_name_;
		ui::RichEdit*	search_edit_;
		ui::Button*		btn_clear_input_;
		ui::ListBox*	search_result_list_;
		ui::ButtonBox* search_bar_;
		ui::Button* btn_max_restore_;
		AutoUnregister	unregister_cb;
	private:
		IMainFormMenuHandler* main_menu_handler_;
	};
}