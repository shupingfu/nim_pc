#pragma once
#include "util/window_ex.h"

/** @class ProxyForm
  * @brief �������ô���
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author wp
  * @date 2014/02/14
  */
class ProxyForm : public nim_comp::WindowEx
{
public:
	ProxyForm();
	virtual ~ProxyForm();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override { return kClassName; };
	virtual std::wstring GetWindowId() const override { return kClassName; };
	virtual UINT GetClassStyle() const override	{ return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;

private:
	/**
	* �������пؼ���������Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	virtual bool Notify(ui::EventArgs* msg);

	/**
	* �������������Ͽ�ѡ�����Ϣ
	* @param[in] msg ��Ϣ�������Ϣ
	* @return bool true �������ݿؼ���Ϣ��false ֹͣ���ݿؼ���Ϣ
	*/
	bool OnProxyTypeSelected(ui::EventArgs* msg);

	/**
	* �������Ĵ����ַ�Ͷ˿��Ƿ�Ϸ�
	* @return bool true �Ϸ���false ���Ϸ�
	*/
	bool CheckProxyLegality();

	/**
	* �Ѵ���������Ͽ��ѡ������ת��Ϊ��Ӧ�Ĵ�������
	* @param[in] index ѡ������
	* @return nim::NIMProxyTyp ��������
	*/
	nim::NIMProxyType ConvertIndexToProxyType(int index);

	/**
	* �Ѵ�������ת��Ϊ����������Ͽ��ѡ������
	* @param[in] nim::NIMProxyTyp ��������
	* @return int ����
	*/
	int ConvertProxyTypeToIndex(nim::NIMProxyType type);

	/**
	* �������õĴ�����Ϣ������nim��nim_http��chatroomģ��Ĵ�����Ϣ
	* @return void	�޷���ֵ
	*/
	void ApplyProxySetting();

private:
	ui::Combo*    proxy_type_comb_;
	ui::RichEdit* addr_ctrl_;
	ui::RichEdit* port_ctrl_;
	ui::RichEdit* user_ctrl_;
	ui::RichEdit* pass_ctrl_;
	ui::Button*	 test_connect_btn_;
	ui::Button*	 confirm_btn_;
	ui::Button*   apply_btn_;
	ui::Control*  gifanim_ctrl_;

public:
	static const LPCTSTR kClassName;
	
	static nim::NIMProxyType	cur_type;
	static std::string			cur_host;
	static short				cur_port;
	static std::string			cur_user;
	static std::string			cur_pass;
};