#pragma once
#include "bubble_item.h"

namespace nim_comp
{
/** @class MsgBubbleFinger
  * @brief 会话窗体中聊天框内的石头剪刀布消息项
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2015/9/11
  */
class MsgBubbleFinger : public MsgBubbleItem
{
public:
	/**
	* 初始化控件内部指针
	* @param[in] bubble_right 是否显示到右侧
	* @return void 无返回值
	*/
	virtual void InitControl(bool bubble_right);

	/**
	* 初始化控件外观
	* @param[in] msg 消息信息结构体
	* @return void 无返回值
	*/
	virtual void InitInfo(const nim::IMMessage &msg);

	/**
	* 响应此消息项的右击消息，弹出菜单
	* @param[in] param 被单击的菜单项的相关信息
	* @return bool 返回值true: 继续传递控件消息， false: 停止传递控件消息
	*/
	bool OnMenu(ui::EventArgs* arg);


	bool OnClick(ui::EventArgs* arg);

 private:
    ui::ButtonBox*  msg_finger_;
	ui::Control*	finger_;
    ui::Label* user_name_a;
    ui::Label* user_id_;
    ui::Label* user_box_name_;
};

}