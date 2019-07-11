#pragma once
#include "util/window_ex.h"
class ProxyTip;
class ProxyTipGlobal;
class ProxyTipSub;
class SubModuleProxySetting : public  nbase::ProxySetting
{
public:
	SubModuleProxySetting() : use_global_setting_(true){}
public:
	bool IsUseGlobalSetting() const { return use_global_setting_;	};
	void SetUseGlobalSetting(bool use){ use_global_setting_ = use; }
private:
	bool use_global_setting_;
};
typedef std::map<std::wstring, std::shared_ptr<nbase::ProxySetting>> ProxySettingList;
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
	virtual void Close(UINT nRet  = IDOK) override;
	virtual void AttachBeforClose(const std::function<void()>& cb) { befor_close_cb_ = cb; };
	static std::shared_ptr<nbase::ProxySetting> GetProxySetting(const std::wstring& name)
	{
		if (proxy_setting_list_.find(name) != proxy_setting_list_.end())
			return proxy_setting_list_[name];
		return nullptr;
	}
private:
	ProxyTipSub* CreateSubModuleProxyTip(const std::function<ProxyTipSub*()>& sub_creator,const std::wstring& name);
	void ApplyProxySetting();
	void ConfirmSetting();
public:
	static const LPCTSTR kClassName;
private:
	ui::Button*	 confirm_btn_;
	ui::Button*	 close_btn_;
	ProxyTipGlobal* global_proxy_;
	ProxyTipSub* proxy_chatroom_;
	ProxyTipSub* proxy_vchat_;
	ProxyTipSub* proxy_rts_;
	ProxyTipSub* proxy_httptool_;
	std::function<void()> befor_close_cb_;
	static ProxySettingList proxy_setting_list_;
};
template<typename TProxySetting>
class ProxySettingHost
{
public:
	ProxySettingHost() : proxy_setting_(nullptr){};
	virtual ~ProxySettingHost(){}
	static std::shared_ptr<TProxySetting> CreateSetting()
	{
		return std::make_shared<TProxySetting>();
	}
	void SetProxySetting(const std::shared_ptr<TProxySetting>& setting){ proxy_setting_ = setting; };
	std::shared_ptr<TProxySetting> GetProxySetting() const { return proxy_setting_; };
protected:
	std::shared_ptr<TProxySetting> proxy_setting_;
};
class ProxyTip : public ui::Box
{
public:
	virtual void DoInit() override;
	void SetProxyName(const std::wstring& name);
	std::wstring GetProxyName() const;
	/**
	* �������õĴ�����Ϣ������nim��nim_http��chatroomģ��Ĵ�����Ϣ
	* @return void	�޷���ֵ
	*/
	virtual void ApplyProxySetting() = 0;
	/**
	* ��NIM��������ת��Ϊ��Ӧ��BASE��������
	* @param[in] type nim��������
	* @return nbase::ProxyType BASE��������
	*/
	static nbase::ProxyType ConvertNimProxytypeToBaseProxyType(nim::NIMProxyType type);

	/**
	* ��Base��������ת��Ϊ��Ӧ��Nim��������
	* @param[in] type BASE��������
	* @return nim::NIMProxyType Nim��������
	*/
	static nim::NIMProxyType ConvertBaseProxytypeToNimProxyType(nbase::ProxyType type);
protected:
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
	* ����������
	* @param[in] enable ����
	* @return void	�޷���ֵ
	*/
	void SetPanelEnabled(bool enable);	

	virtual std::vector<std::wstring> GetSupportedProxyTypeName() = 0;
protected:
	ui::Combo*    proxy_type_comb_;
	ui::RichEdit* addr_ctrl_;
	ui::RichEdit* port_ctrl_;
	ui::RichEdit* user_ctrl_;
	ui::RichEdit* pass_ctrl_;	
	std::wstring proxy_name_;
};
class ProxyTipGlobal : public ProxyTip, public ProxySettingHost< nbase::ProxySetting>
{
public:
	virtual void DoInit() override;
	virtual void ApplyProxySetting() override;
protected:
	virtual std::vector<std::wstring> GetSupportedProxyTypeName() override;
private:
	/**
	* �������Ĵ����ַ�Ͷ˿��Ƿ���Ч
	* @return void
	*/
	void DetectProxyLegality();
	void CallbackDetectProxy(bool connect, nim::NIMProxyDetectStep step, const std::string& json_extention);
private:
	ui::Button*	 test_connect_btn_;
	ui::Control*  gifanim_ctrl_;
	bool being_test_connect_;
};
class ProxyTipSub : public ProxyTip, public ProxySettingHost< SubModuleProxySetting>
{
public:
	virtual void DoInit() override;
	virtual void ApplyProxySetting() override;
private:
	ui::CheckBox* use_global_;
};
class ProxyTipSub_VChat : public ProxyTipSub
{
protected:
	virtual std::vector<std::wstring> GetSupportedProxyTypeName() override;
};
class ProxyTipSub_Rts : public ProxyTipSub
{
protected:
	virtual std::vector<std::wstring> GetSupportedProxyTypeName() override;
};
class ProxyTipSub_Chatroom : public ProxyTipSub
{
protected:
	virtual std::vector<std::wstring> GetSupportedProxyTypeName() override;
};
class ProxyTipSub_Httptool : public ProxyTipSub
{
protected:
	virtual std::vector<std::wstring> GetSupportedProxyTypeName() override;
};