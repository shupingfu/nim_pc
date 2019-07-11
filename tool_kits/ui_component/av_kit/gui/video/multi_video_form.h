#pragma once
#include "helper/ui_bitmap_control.h"
#include "shared/ring.h"
#include "shared/ui/msgbox.h"

namespace nim_comp
{
#define MULTIVCHATMEMBERCOUNT 8 
#define MULTIVCHATVIDEOCOUNT 4

	class MultiVideoChatForm :public ui::WindowImplBase
	{
		enum VchatMemberStatus
		{
			kNone	=0,
			kWaiting, //�ȴ�
			kRefuse,	//δ����
			kConnected,	//�����ӣ�����ͨ����
			kDisConnect	//��Ա�Ͽ�����
		};
		//����ͨ��ԭ��
		enum MultiEndCallEnum
		{
			END_CALL_CLOSE,
			END_CALL_HANGUP,
			END_CALL_CONNECTION,
		};

		class VideoUidsInfo
		{
		public:
			std::string uid;
			bool video;
			int32_t audio_volumn;
			VchatMemberStatus status;
			nbase::WeakCallbackFlag call_timer_;
			
			VideoUidsInfo()
			{
				uid = "";
				video = false;
				audio_volumn = 0;
				status = kNone;
			}
		};
	public:
		MultiVideoChatForm(std::string room_name, std::string session_id, int64_t channel_id, bool video = false, bool creator=false,std::string creator_id="");
		~MultiVideoChatForm();

		// �����麯��
		virtual std::wstring GetSkinFolder() override;
		virtual std::wstring GetSkinFile() override;
		virtual std::wstring GetWindowClassName() const override { return kClassName; };
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
		* ���ݿؼ����������Զ���ؼ�
		* @param[in] pstrClass �ؼ�����
		* @return Control* �����Ŀؼ���ָ��
		*/
		virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
		void OnVideoInDeviceChange(uint32_t status);
		void OnAudioInDeviceChange(uint32_t status);
		void OnVideoDeviceStartCallback(bool result);
		void OnAudioDeviceStartCallback(bool result);

		/**
		* ����Ⱥ����Ա����
		* @param[in] uid ��Աuid
		*/
		void OnPeopleJoin(std::string uid);

		/**
		* ����Ⱥ����Ա�˳�      
		* @param[in] uid ��Աuid
		*/
		void OnPeopleLeave(std::string uid);

		/**
		* ���ó�Աuid��bitmap ��Ϣ
		* @param[in] uid ��Աuid
		* @param[in] video �Ƿ�����Ƶģʽ
		* @param[in] refuse �Ƿ�ܾ�������
		*/
		void OnSetShowBitmap(std::string uid, bool video,bool refuse=false);

		/**
		* �����ص���Ϣ
		* @param[in] map_volume uid��Ӧ��vol������Ϣ
		*/
		void OnVChatVolumeCallback(std::map<std::string, int32_t> map_volume);

		/**
		* @���³�Ա��������Ϣ
		*/
		void RefreshMemberAudioStatus();

		/**
		* ��ȡ����ֵ�ĵȼ�
		* @param[in] volume����
		*/
		int32_t GetVolumeStep(uint32_t volume);

		/**
		* ��ȡ�Ѿ�����ĳ�Ա����Ƶ״̬
		* @return map uid ��Ӧ����Ƶ״̬
		*/
		std::map<std::string,bool> GetInvitedMember();

		/**
		* ���������Ա�Ƿ�������Ƶ
		* @param uid ��Աuid
		* @param video �Ƿ�����Ƶģʽ
		*/
		void SetInvitedMember(std::string uid,bool video);

		/**
		* �õ���Ƶ����,��ǰ�������Ƶ����+������Ƶͨ������������δ�����䣩
		* @return ���ص�ǰȺ�ĵĿ�����Ƶ������
		*/
		int GetVideoCount();

		/**
		* ��ȡ����Ⱥ�ĵĳ�Ա����
		* @return set<std::string> �Ѿ���������Ƶ����ĳ�Ա��Ϣ
		*/
		std::set<std::string> GetTalkingMember() { return talking_members_; }

		/**
		* ��������ĳ�Ա��
		* @param[in]  ����ĳ�Աuid����
		*/
		void SetInvitingMember(std::set<std::string> inviting_members);


		/**
		* ��ʼ�������ɼ����ö����UI����
		*/
		void InitVolumnSetting();

		/** 
		* ��ȡ����Ƶ������
		* @return std::string ����Ƶ������
		*/
		std::string  GetRoomName() { return room_name_; }
		
		/** 
		* �����Ա����Ƶ��Ϣ��ps,��ֹ�Է�������Ƶģʽ֪ͨ��Ϣ����ϢkMsgMultiVchatP2PChangeMode�����ڸó�Ա���뷿���֪ͨ�ϱ�������Ҫ���� 
		* @param[in] uid ��Աuid
		* @param[in] video true:��Ƶ false:��Ƶ
		*/
		void SetCacheMemberVideo(std::string uid, bool video) { cache_members_info_[uid] = video; }

        /**
        * ֱ�ӹرմ���
        * @return void	�޷���ֵ
        */
        void DirectQuit();
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
		* ��ʾ����ͨ��ҳ��
		* @param[in] end_reason ����ԭ��
		* @return void	�޷���ֵ
		*/
		void EnterEndCallPage(MultiEndCallEnum end_reason);

		/**
		* �Զ��رմ���
		*/
		void OnAutoCloseWnd();
		
		/** 
		* �ر���Ƶ�豸
		*/
		void FreeVideo();

		/**
		* �ر���Ƶ�豸
		*/
		void FreeAudio();

		/**
		* ��ʼ������ͷ�ؼ�
		*/
		void InitCbCameraSetting();

		/** 
		* ��ʼ������Ƶ���ò���
		*/
		void InitSetting();

		/**
		* �����Ƶ�����豸
		*/
		int CheckVideoInputDevice();

		/** 
		*  ��˷������ؼ�ʧȥ����ʱ�Ĵ����¼�
		*/
		bool OnMicrophoneVolumnKillFocus(ui::EventArgs* arg);

		/**
		*  �����������ؼ�ʧȥ����ʱ�Ĵ����¼�
		*/
		bool OnSpeakerVolumnKillFocus(ui::EventArgs* arg);

		/** 
		* �رմ���ǰ�����Ĳ���
		*/
		void BeforeClose();

		/** 
		* ȷ���˳������ǵ���ʾ��
		*/
		void OnQuitMsgBox(MsgBoxRet ret);

		/**  
		* ��󻯴��ڣ���ʱûʵ��
		*/
		void OnMaxWindow(bool _max);

		/**
		* �������пؼ�ѡ����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnSelect(ui::EventArgs* arg);

		/**
		* �������пؼ���ѡ����Ϣ
		* @param[in] msg ��Ϣ�������Ϣ
		* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
		*/
		bool OnUnSelect(ui::EventArgs* arg);

		/**
		* ��ʼ����Ƶ�豸
		* @return bool true �ɹ���false ʧ��
		*/
		bool InitVideo();

		/**
		* ��ʼ����Ƶ�豸
		* @return bool true �ɹ���false ʧ��
		*/
		bool InitAudio();

		/** 
		* ��Ƶ��Ⱦ�ؼ�
		*/
		void PaintVideo();
		
		/**
		* ��������¼�
		* @param[in] msg ��Ϣ�������Ϣ
		*/
		bool OnMouseMessage(ui::EventArgs* arg);
		
		/** 
		* ����Աuid��������Ƶ�л�֪ͨ
		* @param[in] ��Աuid
		* @param[in] team true:Ⱥ͸����Ϣ false:p2p��Ϣ
		*/
		void SendNotifyToMember(std::string uid, bool team);
		
		/** 
		* bitmap��Ⱦ���ڷ���
		* @param[in] uid ��Աuid
		* @param[in] uid remove  true:����,false:�Ƴ�,ps.Ŀǰ������δ�Ƴ�����������ʾ
		* @param[in] video  true:��Ƶ false:��Ƶ
		* @param[in] status uid��ͨ��״̬
		*/
		void AdjustVideoBitmap(std::string uid, bool remove, bool video, VchatMemberStatus status);
		
		/** 
		* bitmap��Ⱦ���ڷ���
		* @param[in] uid ��Աuid
		* @param[in] uid remove  true:�Ƴ�,false:���ӻ����
		* @param[in] video  true:��Ƶ false:��Ƶ
		* @param[in] status uid��ͨ��״̬
		*/
		void AdjustVideoBitmap2(std::string uid, bool remove, bool video, VchatMemberStatus status);
		
		/** 
		* �����ƵȺ�ĵ�����
		*/
		int  CheckVChatMembers();

		/**
		* ���Ƶ�����ʾ
		* @param[in] video true:������Ƶ������ʾ false:����Ⱥ������������ʾ
		*/
		void ShowLimitNotify(bool video=true);

		/** 
		* �����Աѡ���괦���¼�
		*/
		void SelectedCompleted(const std::list<UTF8String>& friend_list, const std::list<UTF8String>& team_list, bool delete_enable);

		/** 
		* ��󴰿��е���ƵԴ���н���
		* @param[in] bitmap_i_index  small_camera_page_tip_ ��ά����������
		* @param[in] bitmap_j_index  small_camera_page_tip_ ��ά����������
		*/
		void ExChangeBitmapControl(int bitmap_i_index, int bitmap_j_index);

		/** 
		* �ö�����
		* @param[in] forever true:�ö� false:���ö�
		*/
		void SetTopMast(bool forever);

		/** 
		* �������ڴ�С
		*/
		void AdjustWindowSize();

		/** 
		* ˢ������Ⱥ�ĵ���Ա��¼
		* @param[in] uid ��Աuid remove true:�Ƴ� false ���Ӽ�¼
		*/
		void RefreshTalkingMembers(std::string uid, bool remove);

		/** 
		* ��������ͷ�豸
		*/
		void StartCameraDevice();

		/** 
		* �������е���ʱ
		* @param[in] uid_info ��Ա�����Ϣ
		*/
		void StartCallTimer(VideoUidsInfo& uid_info);

		/**
		* �ر�uid�ĺ��ж�ʱ��
		*/
		void EndCallTimer(std::string uid);

		/** 
		* �ر����졣ps 45s�����˽�����ô�͹رջỰ
		*/
		void EndVChat();

		/** 
		* ����Ⱥ��Ƶ�����ߵ���Ƶ��Ⱦλ�� 
		* @param[in] uid ��Աuid
		*/
		void AdjustCreatorVideoBitmap(std::string uid);
		
		/** 
		* �û���Ϣ�޸Ļص�
		* @param[in] uinfos ��Ա��Ϣ
		*/
		void OnUserInfoChange(const std::list<nim::UserNameCard>& uinfos);

		std::wstring OnHandlerUserName(std::wstring user_name);
	public:
		static const LPCTSTR kClassName;
	private:
		std::string room_name_;
		std::string session_id_;
		uint64_t channel_id_;
		bool  creator_;//�Ƿ���Ⱥ��Ƶ������
		std::string creator_id_;//
		bool	camera_is_open_;  //����ͷ�Ƿ������ر�
		bool is_max_window_;
		bool current_video_mode_;
		nbase::WeakCallbackFlag paint_video_timer_;
		nbase::WeakCallbackFlag start_video_timer_;//1s��������ͷ
		nbase::WeakCallbackFlag start_call_timer_;
		VideoUidsInfo video_uids[2][4];
		VideoUidsInfo big_wnd_info_;
		nbase::NLock data_lock_;
		nbase::NLock invited_members_lock_;
		std::map<std::string, bool> invited_members_;
		std::set<std::string> talking_members_;
		std::map<std::string, bool> cache_members_info_;//������ǰ�������Ϣ֪ͨ
		std::set<std::string> members_info_error_;
		shared::Ring	  voip_ring_;
		VchatMemberStatus current_status_;
		AutoUnregister	unregister_cb_;
	private:
		ui::Button* btn_microphone_;
		ui::Button* btn_speaker_;
		ui::Button* btn_camera_;
		ui::Button* btn_add_member_;
		ui::Button* btn_end_chat_;
		ui::Button*	btn_win_top_;
		ui::CheckBox* cb_camera_;
		ui::Slider*		input_volumn_slider_;
		ui::VBox*		vbox_of_input_;
		ui::Control*	place_ctrl_1_;
		ui::Slider*		output_volumn_slider_;
		ui::VBox*		vbox_of_output_;
		ui::Control*	place_ctrl_2_;
		ui::BitmapControl* video_ctrl_screen_;  // ��Ƶ��Ⱦ���ڣ��󴰿�
		ui::Control*		camera_page_tip_;
		ui::Label*			lb_ctrl_screen_;
		ui::BitmapControl* video_ctrl_preview_[2][4];
		ui::Control*	small_camera_page_tip_[2][4];
		ui::Control*		video_border_ctrl_[2][4];
		ui::Button*			swap_photo_btn_[2][4];
		ui::Label*			lb_member_name_[2][4];
		ui::Label*			lb_member_status_prompt_[2][4];
		ui::Control*		member_volume_[2][4];
		ui::Control* big_wnd_member_volume_;
		ui::HBox*		hbox_camera_fail_;

		
	};
}