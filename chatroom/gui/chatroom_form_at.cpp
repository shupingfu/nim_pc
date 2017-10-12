#include "chatroom_form.h"
#include "gui/session/control/atme_view.h"
#include "gui/session/atlist/at_list_form.h"

using namespace ui;
using namespace nim_comp;

namespace nim_chatroom
{
	//����������@��Ϣ
	bool ChatroomForm::HandleAtMsg(WPARAM wParam, LPARAM lParam)
	{
		if ((input_edit_ == NULL) || !input_edit_->IsFocused())
			return false;

		AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::Int64ToString16(room_id_));
		if (NULL == at_list_form)
			return false;

		if (wParam == '@')
		{
			RECT rc = input_edit_->GetCaretRect();
			POINT pt = { rc.left, rc.bottom };
			::ClientToScreen(this->GetHWND(), &pt);
			at_list_form->SetShowPos(pt);

			return false;
		}
		else if (wParam == VK_BACK)
		{
			// ��ȡ��ǰ���֮ǰ�������ı�
			long start = 0;
			long end = 0;
			input_edit_->GetSel(start, end);
			std::wstring sel_str = input_edit_->GetTextRange(0, end);
			size_t pos = sel_str.rfind(L"@");

			if (pos == std::wstring::npos || end == 0)
			{
				at_list_form->ShowWindow(false, false);
				return false;
			}

			std::wstring at_str = input_edit_->GetTextRange(pos + 1, end);
			if (!at_str.empty())
			{
				if (at_str.length() == 1)
				{
					// ���ֻ��һ���ַ�����ɾ�����ʣ����@����ʱ��ʾ����@��
					ui::CPoint pt = input_edit_->PosFromChar(end - 1);
					pt.y = input_edit_->GetCaretRect().bottom;
					::ClientToScreen(this->GetHWND(), &pt);
					at_list_form->SetShowPos(pt);
					return false;
				}

				wchar_t last_char = at_str.at(at_str.length() - 1);
				std::wstring at_name = at_str.substr(0, at_str.length() - 1);

				if (last_char == L' ')
				{
					//�����ǰҪɾ���ǿո񣬶��ҿո�ǰ������������@ĳ�˵���Ϣ����ɾ��������Ϣ�����Ҳ�����Ϣ��������			
					if (uid_at_someone_.find(nbase::UTF16ToUTF8(at_name)) != uid_at_someone_.end())
					{
						input_edit_->SetSel(pos, end);
						input_edit_->ReplaceSel(L"", true);
						at_list_form->ShowWindow(false, false);
						return true;
					}
				}
				else
				{
					wchar_t last_second_char = at_str.at(at_str.length() - 2);
					if (last_second_char == L' ')
					{
						std::wstring at_name = at_str.substr(0, at_str.length() - 2);
						//�����ǰҪɾ���ǿո�ĺ�һλ�����ҿո�ǰ������������@ĳ�˵���Ϣ���Ͳ�����ƥ�������@�б�ֱ�ӷ���		
						if (uid_at_someone_.find(nbase::UTF16ToUTF8(at_name)) != uid_at_someone_.end())
						{
							return false;
						}
					}

					ui::CPoint pt = input_edit_->PosFromChar(end - 1);
					pt.y = input_edit_->GetCaretRect().bottom;
					::ClientToScreen(this->GetHWND(), &pt);
					if (at_list_form->Match(at_name, false))
					{
						at_list_form->SetShowPos(pt);
					}
					else
					{
						at_list_form->ShowWindow(false, false);
					}

				}
			}
			else
			{
				at_list_form->ShowWindow(false, false);
			}

			return false;
		}
		else if (wParam == VK_RETURN)
		{
			return at_list_form->HandleKeyEnter();
		}
		else if (wParam == VK_UP)
		{
			return at_list_form->HandleKeyUp();
		}
		else if (wParam == VK_DOWN)
		{
			return at_list_form->HandleKeyDown();
		}
		else
		{
			// ��ȡ��ǰ���֮ǰ�������ı�
			long start = 0;
			long end = 0;
			input_edit_->GetSel(start, end);
			std::wstring sel_str = input_edit_->GetTextRange(0, end);
			size_t pos = sel_str.rfind(L"@");

			if (pos == std::wstring::npos || end == 0)
			{
				at_list_form->ShowWindow(false, false);
				return false;
			}

			std::wstring at_str = input_edit_->GetTextRange(pos + 1, end);
			if (!at_str.empty())
			{
				RECT rc = input_edit_->GetCaretRect();
				POINT pt = { rc.left, rc.bottom };
				::ClientToScreen(this->GetHWND(), &pt);

				// ���ƥ��Ĺؼ���ֻ��һ�������ǵ�һ��ƥ��
				// ������Ӧ@ʱShowMemberItems�����������б���������Ե�5���ˣ����Ե�һ��ƥ��ʱ��match_visible��Ϊfalse����Match��������������
				bool match_visible = at_str.length() > 1;
				if (at_list_form->Match(at_str, match_visible))
				{
					at_list_form->SetShowPos(pt);
				}
				else
				{
					at_list_form->ShowWindow(false, false);
				}
			}
		}

		return false;
	}

	bool ChatroomForm::HandleAtMouseWheel(WPARAM wParam, LPARAM lParam)
	{
		AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::Int64ToString16(room_id_));
		if (at_list_form == NULL)
			return false;

		if (IsWindowVisible(at_list_form->GetHWND()) && at_list_form->GetHWND() != ::GetFocus())
		{
			int zDelta = (int)(short)HIWORD(wParam);
			return at_list_form->HandleMouseWheel(zDelta > 0);
		}

		return false;
	}

	void ChatroomForm::HideAtListForm()
	{
		AtlistForm* at_list_form = (AtlistForm*)WindowsManager::GetInstance()->GetWindow(AtlistForm::kClassName, nbase::Int64ToString16(room_id_));
		if (at_list_form == NULL)
			return;

		if (IsWindowVisible(at_list_form->GetHWND()) && at_list_form->GetHWND() != ::GetFocus())
		{
			at_list_form->ShowWindow(false, false);
		}
	}

	void ChatroomForm::OnSelectAtItemCallback(const std::string& uid, bool is_robot)
	{
		if (uid.empty())
			return;

		if (!LoginManager::GetInstance()->IsEqual(uid))
		{
			//�ҵ���ǰ���֮ǰ�����һ��@��λ��
			long start = 0;
			long end = 0;
			input_edit_->GetSel(start, end);
			std::wstring sel_str = input_edit_->GetTextRange(0, end);
			size_t pos = sel_str.rfind(L"@");

			if (pos == std::wstring::npos || end == 0)
				return;

			std::string show_name;
			if (is_robot)
			{
				nim::RobotInfo info;
				UserService::GetInstance()->GetRobotInfo(uid, info);
				show_name = info.GetName();
			}

			AtSomeone da;
			da.uid_ = uid;
			da.is_robot_ = is_robot;
			uid_at_someone_[show_name] = da;

			std::wstring show_text = nbase::UTF8ToUTF16(show_name);
			show_text.append(L" ");

			input_edit_->SetSel(pos + 1, end);
			input_edit_->ReplaceSel(show_text, false);
		}
	}

	void ChatroomForm::InitRobots()
	{
		nim::RobotInfos infos;
		UserService::GetInstance()->GetAllRobotInfo(infos);
		if (!infos.empty())
		{
			has_robots_ = true;
			OnSelectAtItem cb = nbase::Bind(&ChatroomForm::OnSelectAtItemCallback, this, std::placeholders::_1, std::placeholders::_2);
			AtlistForm *at_list_form = new AtlistForm(nbase::Int64ToString(room_id_), nim::kNIMSessionTypeP2P, ToWeakCallback(cb));
			at_list_form->Create(this->GetHWND(), L"", WS_POPUPWINDOW, 0L);
			at_list_form->InitRobotInfos(infos);
		}
	}

} // namespace nim_comp
