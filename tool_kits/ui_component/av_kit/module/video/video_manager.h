﻿#pragma once

#include "video_frame_mng.h"
#include "audio_frame_mng.h"
#include "av_kit/gui/video/multi_video_form.h"
#include "av_kit/gui/video/video_form.h"
#include "av_kit/gui/video/video_setting.h"
#include "av_kit/gui/video/multi_video_invite.h"
#include "av_kit/gui/video/multi_video_form.h"

enum MultiVchatMsgType
{
	kMsgMultiVchatAudio = 2001,//语音
	kMsgMultiVchatVideo = 2002,//视频 发送群视频邀请
	kMsgMultiVchatAck = 2100,//消息2 收到被邀请方的同意，拒绝群视频邀请
	kMsgMultiVchatJoin = 2101,//消息3 给被邀请方发准许进入或失败的通知
	kMsgMultiVchatTeamChangeMode = 2102,//消息4 群消息，本人音视频模式切换通知
	kMsgMultiVchatP2PChangeMode = 2103,//消息5 单点通知音视频模式切换
};

namespace nim_comp
{
	struct MEDIA_DEVICE_DRIVE_INFO
	{
		std::string device_path_;
		std::string friendly_name_;
	};
	enum DeviceSessionType
	{
		kDeviceSessionTypeNone = 0x0,
		kDeviceSessionTypeChat = 0x1,
		kDeviceSessionTypeSetting = 0x2,
		kDeviceSessionTypeRts = 0x4,
		kDeviceSessionTypeChatRoom = 0x8,
	};

	enum MultiVChatStatusType
	{
		kMultiVChatInvite, //当前处于邀请状态
		kMultiVChatJoin,   //当前处于Join状态
		kMultiVChating,    //正在聊天
		kMultiVChatEnd     //聊天结束
	};

	typedef std::function<void(int code)>  ConnectCallback;
	typedef std::function<void(std::string uid, bool join_type)>  PeopleChangeCallback;
	typedef std::function<void(int code)>  MultiRoomCreatedCallback;

	/** @class VideoManager
	  * @brief 音视频管理器
	  * @copyright (c) 2016, NetEase Inc. All rights reserved
	  * @date 2016/09/18
	  */
	class VideoManager : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(VideoManager);

		VideoManager();
		~VideoManager();

		/**
		* 显示群视频人员选择窗口
		*/
		bool ShowMultiVideoSelectForm();

		/**
		* 显示一个音视频窗口
		* @param[in] id 对方用户id
		* @param[in] video 是否包含视频功能
		* @param[in] channel_id 通道id
		* @return bool true 成功，false 失败
		*/
		bool ShowVideoChatForm(std::string id, 
			bool video, 
			uint64_t channel_id = 0, 
			const std::list<UTF8String>& friend_list = std::list<UTF8String>(),
			const MultiRoomCreatedCallback& cb = MultiRoomCreatedCallback());

        /**
        * 关闭所有正在聊天的窗口，包括群组和一对一音视频聊天
        * @return void 无返回值
        */
        void DestroyVideoChatForm();

		/**
		* 显示音视频设置窗口
		* @param[in] video 是否包含视频功能
		* @return bool true 成功，false 失败
		*/
		bool ShowVideoSetting(bool video = false);

		/**
		* 是否存在音视频窗口
		* @param[in] show 如果存在并且不处于显示状态，是否让窗口自动显示
		* @return bool true 存在，false 不存在
		*/
		bool IsVideoChatFormExist(bool show);

		/**
		* 是否存在群聊窗口
		* @param[in] show 如果存在并且不处于显示状态，是否让窗口自动显示
		* @return bool true 存在，false 不存在
		*/
		bool IsMultiVideoChatFormExist(bool show);

		//监听
		//void OnVideoDataCb(bool capture, const std::string& data, unsigned int width, unsigned int height, const std::string& json);

		/**
		* 设备状态改变的回调
		* @param[in] type 设备类型NIMDeviceType，其中kNIMDeviceTypeAudioIn和kNIMDeviceTypeVideo有效
		* @param[in] status 为NIMDeviceStatus的多状态
		* @param[in] path 当kNIMDeviceStatusReset状态时需要关注此参数，kNIMDeviceStatusReset时有可能选用了非用户选定的设备，这里返回的是重新启动的设备地址
		* @return void 无返回值
		*/
		void OnDeviceStatus(nim::NIMDeviceType type, UINT status, std::string path);

		/**
		* 启动设备的回调
		* @param[in] type 设备类型NIMDeviceType
		* @param[in] ret 启动结果，true表示成功
		* @return void 无返回值
		*/
		void OnStartDeviceCb(nim::NIMDeviceType type, bool ret);

		/**
		* 音视频通话中状态返回的回调
		* @param[in] type NIMVideoChatSessionType
		* @param[in] channel_id 通话的通道id
		* @param[in] code 结果类型或错误类型
		* @param[in] json_extension Json string 扩展
		* @return void 无返回值
		*/
		void OnVChatEvent(nim::NIMVideoChatSessionType type, uint64_t channel_id, int code, const std::string& json);

		/**
		* 获取默认设备路径
		* @param[out] no 获取到的设备的序号
		* @param[out] device_path 获取到的设备路径
		* @param[in] type 设备类型
		* @return bool true 成功，false 失败
		*/
		bool GetDefaultDevicePath(int &no, std::string &device_path, nim::NIMDeviceType type);

		/**
		* 设置默认设备路径
		* @param[in] device_path 设备路径
		* @param[in] type 设备类型
		* @return bool true 成功，false 失败
		*/
		void SetDefaultDevicePath(std::string device_path, nim::NIMDeviceType type);

		/**
		* 获取设备信息
		* @param[in] type 设备类型
		* @return std::list<MEDIA_DEVICE_DRIVE_INFO> 设备信息列表
		*/
		std::list<MEDIA_DEVICE_DRIVE_INFO> GetDeviceInfo(nim::NIMDeviceType type);

		/**
		* 枚举设备回调结果
		* @param[in] ret 结果代码，true表示成功
		* @param[in] type 设备类型NIMDeviceType，其中kNIMDeviceTypeAudioOut和kNIMDeviceTypeAudioOutChat等效
		* @param[in] json Json string 设备列表，可能为空"", 例：json_extension = "[{"name":"Webcam","path":"\\\\?\\usb......"},{"name":"Webcam2","path":"\\\\?\\usb......"}]
		* @return void 无返回值
		*/
		void GetDeviceByJson(bool ret, nim::NIMDeviceType type, const char* json);

		/**
		* 设置音量
		* @param[in] volumn 音量，默认255
		* @param[in] capture true 标识设置麦克风音量，false 标识设置播放音量
		* @return void 无返回值
		*/
		void SetAudioVolumn(unsigned char volumn, bool capture);

		/**
		* 设置自动调节麦克风音量
		* @param[in] auto_volumn 是否自动调节音量
		* @return void 无返回值
		*/
		void SetAudioInputAutoVolumn(bool auto_volumn);

		/**
		* 设置视频分辨率
		* @param[in] 视频分辨率，参见 NIMVChatVideoQuality
		* @return void 无返回值
		*/
		void SetVideoQuality(int video_quality);

		/**
		* 启动设备
		* @param[in] type 设备类型
		* @param[in] device_path 设备路径
		* @param[in] session_type 会话类型
		* @return void	无返回值
		*/
		void StartDevice(nim::NIMDeviceType type, std::string device_path, DeviceSessionType session_type);

		/**
		* 结束设备
		* @param[in] type 设备类型
		* @param[in] session_type 会话类型
		* @param[in] forced 是否强制结束
		* @return void	无返回值
		*/
		void EndDevice(nim::NIMDeviceType type, DeviceSessionType session_type, bool forced = false);

		/**
		* 开始音视频通话
		* @param[in] mode 音视频通话类型
		* @param[in] apns_text 暂时无用
		* @param[in] custom_info 暂时无用
		* @param[in] uid 对方用户id
		* @param[in] session_id 会话id
		* @return bool true 成功，false 失败
		*/
		bool StartChat(nim::NIMVideoChatMode mode, const std::string& apns_text, const std::string& custom_info, const std::string& uid, const std::string& session_id);
		//bool JoinRoomChat(nim::NIMVideoChatMode mode, const std::string& room_id, const std::string& master, nim::NIMVChatVideoQuality quality_type, const std::string& apns_text, const std::string& custom_info);

		/**
		* 设置视频类型
		* @param[in] mode 音视频通话类型
		* @return bool true 成功，false 失败
		*/
		bool SetVoipMode(nim::NIMVideoChatMode mode);

		/**
		* 回应音视频邀请
		* @param[in] channel_id 通道id
		* @param[in] accept 是否接受
		* @param[in] session_id 会话id
		* @return bool true 成功，false 失败
		*/
		bool VChatCalleeAck(uint64_t channel_id, bool accept, const std::string& session_id);

		/**
		* 音视频通话控制
		* @param[in] channel_id 通道id
		* @param[in] type 会话控制类型
		* @return bool true 成功，false 失败
		*/
		bool VChatControl(uint64_t channel_id, nim::NIMVChatControlType type);

		/**
		* 结束音视频通话
		* @param[in] session_id 会话id
		* @param[in] is_time_out 做为发起方时是否因呼叫超时而结束
		* @return void	无返回值
		*/
		void EndChat(const std::string& session_id,bool is_time_out = false);

		/**
		* 设置观众模式
		* @param[in] viewer 是否为观众
		* @return void	无返回值
		*/
		void SetViewerMode(bool viewer);

		/**
		* 获取观众模式
		* @return bool true 是，false 否
		*/
		bool GetViewerMode();

		/**
		* 设置音频静音
		* @param[in] muted 是否静音
		* @return void	无返回值
		*/
		void SetAudioMuted(bool muted);

		/**
		* 获取是否音频静音
		* @return bool true 静音，false 不静音
		*/
		bool GetAudioMuteEnabled();

		/**
		* 设置单个成员的黑名单状态，当前通话有效(只能设置进入过房间的成员)
		* @param[in] uid 成员用户id
		* @param[in] add true表示添加到黑名单，false表示从黑名单移除
		* @param[in] audio true表示音频黑名单，false表示视频黑名单
		* @param[in] cb 操作结果回调函数
		* @return void 无返回值
		*/
		void SetMemberBlacklist(const std::string& uid, bool add, bool audio, nim::VChat::OptCallback cb);

		/**
		* 创建一个多人房间
		* @param[in] room_name 房间名
		* @param[in] custom_info 自定义的房间信息（加入房间的时候会返回）
		* @param[in] cb 操作结果回调函数
		* @return void 无返回值
		*/
		void CreateRoom(const std::string& room_name, const std::string& custom_info, nim::VChat::Opt2Callback cb);

		/**
		* 加入一个多人房间
		* @param[in] mode 音视频通话类型
		* @param[in] room_name 房间名
		* @param[in] session_id 会话id
		* @param[in] cb 操作结果回调函数
		* @return bool true 调用成功，false 调用失败可能有正在进行的通话
		*/
		bool JoinRoom(nim::NIMVideoChatMode mode, const std::string& room_name, const std::string& session_id, nim::VChat::Opt2Callback cb);

		/**
		* 设置多人音视频回调函数
		* @param[in] connect_cb 连接状态改变的回调函数
		* @param[in] people_cb 成员改变的回调函数
		* @return void	无返回值
		*/
		void SetChatRoomCb(ConnectCallback connect_cb = nullptr, PeopleChangeCallback people_cb = nullptr);

		/**
		* 设置webrtc开关
		* @param[in] webrtc 是否webrtc兼容
		* @return void	无返回值
		*/
		void SetWebrtc(bool webrtc){ webrtc_setting_ = webrtc; }
		/**
		* 获取webrtc开关
		* @return bool	是否打开webrtc开关
		*/
		bool GetWebrtc(){ return webrtc_setting_; }

		void SetMultiVChatCreator(std::string  creator_id) { multi_vchat_creator_id_ = creator_id; }

		std::string GetMultiVChatCreator() { return multi_vchat_creator_id_; }

		/**
		* 获取音视频设置页面中音频输入设备的状态
		* @return bool true:设备已开启，false:设备已关闭
		*/
		bool GetSettingAudioInStatus(){ return setting_audio_in_; }

		/**
		* 获取音视频设置页面中音频输出设备的状态
		* @return bool true:设备已开启，false：设备已关闭
		*/
		bool GetSettingAudioOutStatus() { return setting_audio_out_; }

		/**
		* 获取摄像头设备的状态
		* @return bool true:设备已开启，false：设备已关闭
		*/
		bool GetSettingVideoInStatus() { return setting_video_in_; }

		/**
		* 获取当前设置的视频分辨率
		* @return int 视频分辨率枚举值
		*/
		int GetVideoQuality() { return video_quality_; }

		/**
		* 音视频设置页面中音频输入设备的状态
		* @param[in] setting_audio_in  true:音频输入设备开启 false:音频输入设备关闭
		*/
		void SetSettingAudioInStatus(bool setting_audio_in) { setting_audio_in_ = setting_audio_in; }

		/**
		* 音视频设置页面中音频输出设备的状态
		* @param[in] setting_audio_out  true:音频输出设备开启 false:音频输出设备关闭
		*/
		void SetSettingAudioOutStatus(bool setting_audio_out) { setting_audio_out_ = setting_audio_out; }

		/**
		* 音视频设置页面中视频输入设备的状态
		* @param[in] setting_video_in  true:音频输出设备开启 false:音频输出设备关闭
		*/
		void SetSettingVideoInStatus(bool setting_video_in) { setting_video_in_ = setting_video_in; }

		/**
		* 设置当前多人群聊的状态
		* @param[in] vchat_status 群聊状态
		*/
		void SetMultiVChatStatus(MultiVChatStatusType vchat_status);

		/**
		* 获取当前多人群聊的状态
		* @return  MultiVChatStatusType 群聊状态
		*/
		MultiVChatStatusType GetMultiVChatStatus();

		void EndJoinTimer();
		void StartJoinTimer();
		void JoinTimeOut();

		void InvokeReceiveCustomP2PMessage(const Json::Value &json, const std::string &sender);
		void InvokeReceiveCustomTeamMessage(const Json::Value &json, const std::string &sender);

		std::set<std::string> GetMeetingMember();

	private:
		void DoMultiVChat(const std::list<UTF8String>& friend_list, const MultiRoomCreatedCallback& cb);
	public:
		VideoFrameMng video_frame_mng_;
		AudioFrameMng audio_frame_mng_;
	private:
		std::list<MEDIA_DEVICE_DRIVE_INFO> device_info_list_[4];
		std::string def_device_path_[4];
		DWORD device_session_type_[4];
		ConnectCallback			chatroom_connect_cb_ = nullptr;
		PeopleChangeCallback	chatroom_people_cb_ = nullptr;
		bool webrtc_setting_;
		std::string multi_vchat_session_id_;
		std::string multi_vchat_creator_id_;
		bool setting_audio_in_;
		bool setting_audio_out_;
		bool setting_video_in_;
		MultiVChatStatusType vchat_status_;
		nbase::NLock vchat_status_lock_;
		nbase::WeakCallbackFlag join_vchat_timer_;
		int video_quality_ = 0;

	private:
		std::unique_ptr<VideoForm> video_form_;
		std::unique_ptr<MultiVideoChatForm> multi_video_form_;
		std::unique_ptr<VideoSettingForm> setting_form_;
		std::unique_ptr<MultiVideoInviteForm> multi_vchat_invite_form_;

		std::weak_ptr<nbase::WeakFlag> video_form_flag_;
		std::weak_ptr<nbase::WeakFlag> multi_video_form_flag_;
		std::weak_ptr<nbase::WeakFlag> setting_form_flag_;
		std::weak_ptr<nbase::WeakFlag> multi_vchat_invite_form_flag_;
	};
}