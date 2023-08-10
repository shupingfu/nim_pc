#include "stdafx.h"
#include "bubble_finger.h"
#include "gui/profile_form/profile_form.h"


using namespace ui;

namespace nim_comp
{
void MsgBubbleFinger::InitControl(bool bubble_right)
{
	__super::InitControl(bubble_right);

	msg_finger_ = new ButtonBox;
	if(bubble_right)
		GlobalManager::FillBoxWithCache(msg_finger_, L"session/finger_right.xml");
	else
		GlobalManager::FillBoxWithCache(msg_finger_, L"session/finger_left.xml");

	msg_finger_->AttachMenu(nbase::Bind(&MsgBubbleFinger::OnMenu, this, std::placeholders::_1));
    msg_finger_->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&MsgBubbleFinger::OnClick, this, std::placeholders::_1));
	bubble_box_->Add(msg_finger_);

	finger_ = msg_finger_->FindSubControl(L"finger");

    user_box_name_ = (Label*)msg_finger_->FindSubControl(L"file_info");
    user_id_ = (Label*)msg_finger_->FindSubControl(L"file_info_size");
}

void MsgBubbleFinger::InitInfo(const nim::IMMessage &msg)
{
	__super::InitInfo(msg);

	Json::Value json;
	if (StringToJson(msg.attach_, json) && json.isObject())
	{
		int sub_type = json["type"].asInt();
		if (sub_type == CustomMsgType_Jsb && json["data"].isObject()) //finger
		{
			//int v = json["data"]["value"].asInt();

			std::string uid = json["data"]["uid"].asString();
            user_id_->SetText(nbase::UTF8ToUTF16(uid));

            std::wstring photo = PhotoService::GetInstance()->GetUserPhoto(uid);
            finger_->SetBkImage(photo);

			std::string usenamae = json["data"]["username"].asString();
            user_box_name_->SetText(nbase::UTF8ToUTF16(usenamae));

			return;
		}
	}
}

bool MsgBubbleFinger::OnMenu( ui::EventArgs* arg )
{
	PopupMenu(false, true, false);
	return false;
}

bool MsgBubbleFinger::OnClick(ui::EventArgs* arg) {
    Box* member_item = static_cast<Box*>(arg->pSender);
    ui::Label* name = (ui::Label*)member_item->FindSubControl(L"file_info");
    nim_comp::ProfileForm::ShowProfileForm(nbase::UTF16ToUTF8(name->GetText()));
    return true;
}
}