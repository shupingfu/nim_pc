#pragma once
namespace nim_comp
{
	class IProfileUI
	{
	public:
		/**
		* ��ʼ���û���Ϣ��ֻ��ʹ��һ�Σ��������ע��ص�
		* @param[in] info �û���Ƭ
		* @return void	�޷���ֵ
		*/
		virtual void InitUserInfo(const nim::UserNameCard & info) = 0;

		/**
		* ��ȡ��չʾ��Ƭ��Ϣ
		* @return UserNameCard	��Ƭ��Ϣ
		*/
		virtual nim::UserNameCard	GetNameCard() const = 0;
		/**
		* ���ñ���������
		* @param[in] title ����������
		* @return void	�޷���ֵ
		*/
		virtual void SetTaskbarTitle(const std::wstring &title) = 0;
		virtual void Close(UINT nRet = 0) = 0;
		/**
		* ��Ӧ����������øı�Ļص�����
		* @param[in] switch_on �Ƿ����������
		* @return void �޷���ֵ
		*/
		virtual void OnMultiportPushConfigChange(bool switch_on) = 0;
	};
}