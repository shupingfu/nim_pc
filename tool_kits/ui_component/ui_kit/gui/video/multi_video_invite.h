#pragma once
#include "module/session/ring.h"
namespace nim_comp
{
#define MULTI_VCHAT_INVITE_TIMEOUT 45
	class MultiVchatInfo
	{
	public:
		std::string team_id_;
		uint64_t channel_id_;
		std::string creator_id_;
		std::string team_name_;
		std::string room_name_;
		int member_count_;
		MultiVchatInfo() 
		{
			member_count_ = 0;
		}
	};

	class MultiVideoInviteForm :public WindowEx
	{

	public:
		MultiVideoInviteForm(MultiVchatInfo vchat_info);
		~MultiVideoInviteForm();

		// �����麯��
		virtual std::wstring GetSkinFolder() override;
		virtual std::wstring GetSkinFile() override;
		virtual std::wstring GetWindowClassName() const override { return kClassName; };
		virtual std::wstring GetWindowId() const override { return kClassName; };
		virtual UINT GetClassStyle() const override { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

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
		* �����ڱ����ٵ���Ϣ
		* @param[in] hWnd ���ھ��
		* @return void	�޷���ֵ
		*/
		virtual void OnFinalMessage(HWND hWnd) override;

		/** 
		* ��ȡȺ����Ϣ
		* @return MultiVchatInfo Ⱥ����Ϣ
		*/
		MultiVchatInfo GetVchatInfo() { return vchat_info_; }

		void AddInviteMembers(std::set<std::string> uids);

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
		* ����Ⱥ�Ľ���֪ͨ
		* @param[in]accept
		*/
		void SendMultiVchatNotify(bool accept);

		/**
		* �����������ͷ����Ϣ
		*/
		void CheckHeadIcon();


		/**
		* ��ʼ���ؼ���Ϣ
		*/
		void InitControl();

		/**
		* ��ת���Ựҳ��
		*/
		void JumpSession();

		/**
		* ���볬ʱ����
		*/
		void InviteTimeOut();

		/** 
		*  ��Ա��Ϣ���Ļص�����
		*/
		void OnUserInfoChange(const std::list<nim::UserNameCard>& uinfos);

		void SetInviteName();

		
	public:
		static const LPCTSTR kClassName;
		MultiVchatInfo vchat_info_;

	private:
		nbase::WeakCallbackFlag invite_game_timer_;
		Ring	  voip_ring_;
		

	private:
		std::set<std::string> invite_members_;
		ui::Button* btn_jump_session_;
		ui::Button* btn_accept_;
		ui::Button* btn_refuse_;
		ui::Button*	headicon_btn_;
		ui::Label* lb_multi_vchat_member_info_;
		ui::Label* lb_multi_vchat_member_count_;
		ui::Label* lb_multi_vchat_inviter_;
		AutoUnregister	unregister_cb_;
	};
}