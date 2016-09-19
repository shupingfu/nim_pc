#pragma once

namespace nim_comp
{
/** @class AtMeView
* @brief �ڻỰ��������ʾ�Լ���@����Ϣ
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/9/6
*/
class AtMeView : public ui::ButtonBox
{
public:
	struct AtMeInfo
	{
		std::wstring msg_body;
		std::wstring sender_name;
		std::wstring uuid;
	};

	void InitControl();
	// ���µ�@ô��Ϣ������׷�ӵ�@me��Ϣ�б�ĩβ
	void AddMessage(const UTF8String &at_me_json);

	void ShowNextMessage();	
	void ShowLoadingTip(bool show);
	UINT GetAtMeCount();
	std::wstring GetUuid();

private:
	void UpdateView();
	
private:
	std::vector<AtMeInfo>	at_me_info_;

	ui::Label		*content_;
	ui::Label		*sender_name_;

};
}