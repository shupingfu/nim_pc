#pragma once
#include "window_ex.h"

namespace nim_comp
{
//map<����������map<����id������ָ��>>, ���ͬһ��ֻ��һ�����ڣ�ʹ��������Ϊid
typedef std::map<std::wstring, std::map<std::wstring, WindowEx*>> WindowsMap; 
typedef std::list<WindowEx *> WindowList;


class WindowsManager
{
public:
	SINGLETON_DEFINE(WindowsManager);
	WindowsManager();
	virtual ~WindowsManager();

	//���ݴ���������idע�ᴰ��
	bool RegisterWindow(const std::wstring wnd_class_name, const std::wstring wnd_id, WindowEx *wnd);
	//���ݴ���������idע������
	void UnRegisterWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id, WindowEx *wnd);
	//���ݴ���������id��ȡ����
	WindowEx* GetWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id);
	//��ȡ���д���
	WindowList GetAllWindows();
	//��ȡָ��class��Ӧ�����д���
	WindowList GetWindowsByClassName(LPCTSTR classname);
	//�ر����д���
	void DestroyAllWindows();
	//���ý�ֹ���ڴ���
	void SetStopRegister(bool stop=true){stop_register_ = stop;}
	//�Ƿ��ֹ���ڴ���
	bool IsStopRegister(){return stop_register_;}

	template<typename WindowType>
	static WindowType* SingletonShow(const std::wstring& window_id)
	{
		WindowType *window = (WindowType*)(WindowsManager::GetInstance()->GetWindow(WindowType::kClassName, window_id));
		if (!window)
		{
			window = new WindowType;
			window->Create(NULL, WindowType::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
			window->CenterWindow();
			window->ShowWindow();
		}
		else
		{
			window->ActiveWindow();
		}

		return window;
	}

private:
	WindowsMap					windows_map_;	//���д���
	std::string					user_id_;
	bool						stop_register_;	//��ֹ���ڴ���
};
}