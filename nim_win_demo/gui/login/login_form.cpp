#include "login_form.h"
#include "gui/main/main_form.h"


using namespace ui;

void LoginForm::DoBeforeLogin()
{
	std::string username = user_name_edit_->GetUTF8Text();
	StringHelper::Trim( username );
	std::wstring user_name_temp = nbase::UTF8ToUTF16(username);
	user_name_temp = StringHelper::MakeLowerString(user_name_temp);
	username = nbase::UTF16ToUTF8(user_name_temp);
	if( username.empty() )
	{
		usericon_->SetEnabled(false);
		ShowLoginTip(L"�˺�Ϊ��");
		return;
	}

	std::string password = password_edit_->GetUTF8Text();
	StringHelper::Trim( password );
	if( password.empty() )
	{
		passwordicon_->SetEnabled(false);
		ShowLoginTip(L"����Ϊ��");
		return;
	}

	usericon_->SetEnabled(true);
	passwordicon_->SetEnabled(true);

	StartLogin(username, password);
}

void LoginForm::DoRegisterAccount()
{
	std::string username = user_name_edit_->GetUTF8Text();
	StringHelper::Trim(username);
	std::string password = password_edit_->GetUTF8Text();
	StringHelper::Trim(password);
	std::string nickname = nick_name_edit_->GetUTF8Text();
	StringHelper::Trim(nickname);
	if (password.length() < 6 || password.length() > 16)
	{
		ShowLoginTip(L"����Ϊ6~16λ��ĸ�����������");
	}
	else if (nickname.empty())
	{
		ShowLoginTip(L"�ǳ�Ϊ10λ���֡���ĸ�����������");
	}
	else {
		password = QString::GetMd5(password);
		nim_ui::UserManager::GetInstance()->InvokeRegisterAccount(username, password, nickname, ToWeakCallback([this](int res, const std::string& err_msg) {
			if (res == 200) {
				SetTaskbarTitle(L"��¼");
				FindControl(L"enter_panel")->SetBkImage(L"user_password.png");
				FindControl(L"nick_name_panel")->SetVisible(false);
				FindControl(L"enter_login")->SetVisible(false);
				FindControl(L"register_account")->SetVisible(true);
				btn_register_->SetVisible(false);
				btn_login_->SetVisible();
			}
			else if (res == 601) {
				ShowLoginTip(L"�˺�Ϊ6~16λ��ĸ�����������");
			}
			else if (res == 602) {
				ShowLoginTip(L"���˺��Ѵ���");
			}
			else if (res == 603) {
				ShowLoginTip(L"�ǳ�Ϊ10λ���֡���ĸ�����������");
			}
			else {
				ShowLoginTip(nbase::UTF8ToUTF16(err_msg));
			}

		}));
	}

}

void LoginForm::StartLogin( std::string username, std::string password )
{
	user_name_edit_->SetEnabled(false);
	password_edit_->SetEnabled(false);

	login_error_tip_->SetVisible(false);
	login_ing_tip_->SetVisible(true);

	btn_login_->SetVisible(false);
	btn_cancel_->SetVisible(true);

	nim_ui::LoginManager::GetInstance()->DoLogin(username, password);
}

void LoginForm::OnLoginResult( int error )
{
	Reset();

	if (error == nim::kNIMResUidPassError)
	{
		usericon_->SetEnabled(false);
		passwordicon_->SetEnabled(false);
		ShowLoginTip(L"�û����������������������");
	}
	else if (error == nim::kNIMResConnectionError)
	{
		ShowLoginTip(L"����������⣬��ȷ����������");
	}
	else
	{
		std::wstring tip = nbase::StringPrintf(L"��½ʧ�ܣ������룺%d", error);
		ShowLoginTip(tip);
	}
}

void LoginForm::Reset()
{
	usericon_->SetEnabled(true);
	passwordicon_->SetEnabled(true);

	user_name_edit_->SetEnabled(true);
	password_edit_->SetEnabled(true);

	login_ing_tip_->SetVisible(false);
	login_error_tip_->SetVisible(false);

	btn_login_->SetVisible(true);
	btn_cancel_->SetVisible(false);
	btn_cancel_->SetEnabled(true);
}

void LoginForm::ShowLoginTip(std::wstring tip_text)
{
	login_ing_tip_->SetVisible(false);

	login_error_tip_->SetText(tip_text);
	login_error_tip_->SetVisible(true);
}
