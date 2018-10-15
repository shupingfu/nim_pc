#pragma once
#ifdef SUPPORTLOCALPLAYER

#include "controls/ui_video_player_bitmap_control.h"

using namespace nbase;

typedef enum 
{
	PlayerStateStarting = 1,
	PlayerStatePlaying,
	PlayerStatePaused,
	PlayerStateCompleted,
	PlayerStateStopping,
	PlayerStateStopped,
	PlayerStateError,
} PlayerState;

class LocalVideoPlayerForm : public ui::WindowImplBase
{
public:
	LocalVideoPlayerForm(const std::function<void()>& close_cb, const std::wstring& path_ = L"");
	~LocalVideoPlayerForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	virtual ui::UILIB_RESOURCETYPE GetResourceType() const;
	virtual std::wstring GetZIPFileName() const;

	virtual std::wstring GetWindowClassName() const override;
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

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	/**
	* �������пؼ���������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	virtual bool Notify(ui::EventArgs* msg);
	/**
	* �������пؼ�������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	virtual bool OnClicked(ui::EventArgs* msg);

	/**
	* ����ESC����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	virtual void OnEsc(BOOL &bHandled);
	 /**
	 * �����������Ϣ
	 * @param[in] max �Ƿ����
	 * @return void �޷���ֵ
	 */
	void OnMaxWindow(bool max);
	void Play(const std::wstring& path);
	bool SamePath(const std::wstring& path) const;
private:

	/**
	* ��ʼ��Ƶ����
	* @return void	�޷���ֵ
	*/
	bool StartPlay();
	/**
	* ��ʼ��Ƶ����
	* @return void	�޷���ֵ
	*/
	void Shutdown();
	/**
	* ˢ�²���ʱ��ͽ�����
	* @return void	�޷���ֵ
	*/
	void RefreshPlayTime();
	/**
	* ���²�����״̬
	* @return void	�޷���ֵ
	*/
	void ChangePlayerState(PlayerState new_state);
	/**
	* ���Ž���ص�
	* @return void	�޷���ֵ
	*/
	static void PlayerMessageCB(_HNLPSERVICE hNLPService, ST_NELP_MESSAGE msg);
	static void UIPlayerMessageCB(_HNLPSERVICE hNLPService, ST_NELP_MESSAGE msg);
	/**
	* ��Ƶ���ݻص�
	* @return void	�޷���ֵ
	*/
	static void VideoFrameCB(_HNLPSERVICE hNLPService, ST_NELP_FRAME *frame);

public:
	static const LPTSTR kClassName;
	static std::map<_HNLPSERVICE, LocalVideoPlayerForm*> InstFormMap;
	static unsigned int InstanceNum; //���������ڸ���

private:
	std::wstring       path_; //��ǰ����ID
	_HNLPSERVICE       nelp_handle_ = NULL;
	bool               is_max_window_ = false;
	bool               playing_ = false;
	long long          total_duration_ = 0; //��ʱ����������������ֱ����ʼ�㲥
	float              volume_ = 20.0f;
	bool               muted_ = false;
	PlayerState        state_ = PlayerStateStopped;
	WeakCallbackFlag   refresh_time_flag_;
	std::function<void()> close_cb_;

	ui::VBox*		   volume_box_ = NULL;
	ui::TabBox*		   play_pause_box_ = NULL;
	ui::VideoPlayerBitmapControl* canvas_ = NULL;
	ui::Button*        btn_start_ = NULL;
	ui::Slider*        playing_progress_ = NULL;
	ui::Label*         label_duration_ = NULL;
	ui::Button*        btn_volume_ = NULL;
	ui::Button*        btn_mute_ = NULL;
};
#endif