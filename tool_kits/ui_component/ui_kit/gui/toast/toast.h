#pragma once

/** @class WindowsManager
* @brief �ṩһ�����׵���ʾ���壬�൱��MessageBox�ļ򻯰�
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/8/24
*/
namespace nim_comp
{
class Toast : public ui::WindowImplBase
{
public:
	/**
	* ����Ļ�м���ʾһ����ʾ��
	* @param[in] content ��ʾ����
	* @param[in] duration ����ʱ��(����),0����һֱ��ʾ
	* @param[in] parent �����ھ��������ṩ�Ļ�toast����ʾ�ڸ����ڵ�����
	* @return void �޷���ֵ
	*/
	static void ShowToast(const std::wstring &content, int duration = 0, HWND parent = NULL);
public:
	Toast(){};
	~Toast(){};

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual ui::UILIB_RESOURCETYPE GetResourceType() const;
	virtual std::wstring GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const /*override*/;
	virtual UINT GetClassStyle() const override;

	virtual void InitWindow() override;
	void SetDuration(int duration);

private:
	bool OnClicked(ui::EventArgs* msg);
	void SetContent(const std::wstring &str);

public:
	static const LPCTSTR kClassName;
private:
	ui::RichEdit	*content_;
	ui::Button		*close_button_;
};
}