#ifndef UI_UTILS_DPIMANAGER_H_
#define UI_UTILS_DPIMANAGER_H_

#pragma once

namespace ui
{
/** @class DpiManager
  * @brief DPI���������
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/10/10
  */
class UILIB_API DpiManager
{
public:
	static DpiManager* GetInstance();

	/**
	* ��ȡϵͳDPI(����DPI��֪����Ч)
	* @return UINT	DPI
	*/
	static UINT GetSystemDpi();

	/**
	* ��ȡĳ����ʾ����DPI(����DPI��֪����Ч)
	* @param[in] HMONITOR���
	* @return UINT	DPI
	*/
	static UINT GetMonitorDpi(HMONITOR hMonitor);

	/**
	* ��ȡ����ʾ��DPI(����DPI��֪����Ч)
	* @return UINT	DPI
	*/
	static UINT GetMainMonitorDpi();
		
	/**
	* ��ע����ȡDPI(������DPI��֪Ҳ��Ч)
	* @return UINT	DPI
	*/
	static UINT GetDpiFromRegistry();

	/**
	* �Ƿ�����Լ�����DPI����Ч��
	* @return bool true �ǣ�false ��
	*/
	bool IsAdaptDPI();

	/**
	* �����Ƿ�����Լ�����DPI���ţ�Ĭ�ϲ��Լ�����
	* @return bool true ���óɹ���false ����ʧ��
	*/
	bool SetAdaptDPI();

	/**
	* ��ȡ��ǰ�������ű�
	* @return UINT	���ű�
	*/
	UINT GetScale();

	/**
	* ����DPIֵ���ý������űȣ�ֻ�г����Լ�����DPI����ʱ����Ч��
	* @param[in] uDPI DPIֵ
	* @return void	�޷���ֵ
	*/
	void SetScale(UINT uDPI);

	/**
	* ���ݽ������ű�������������ֻ�г����Լ�����DPI����ʱ����Ч��
	* @param[in] iValue ����
	* @return int	���ź��ֵ
	*/
	int ScaleInt(int &iValue);

	/**
	* ���ݽ������ű�������SIZE��ֻ�г����Լ�����DPI����ʱ����Ч��
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScaleSize(SIZE &size);

	void ScaleSize(CSize &size);

	/**
	* ���ݽ������ű�������POINT��ֻ�г����Լ�����DPI����ʱ����Ч��
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScalePoint(POINT &point);

	void ScalePoint(CPoint &point);

	/**
	* ���ݽ������ű�������RECT��ֻ�г����Լ�����DPI����ʱ����Ч��
	* @param[in] pSize SIZEָ��
	* @return void	�޷���ֵ
	*/
	void ScaleRect(RECT &rect);

	void ScaleRect(UiRect &rect);

private:
	DpiManager();
	~DpiManager() {};	
	DpiManager(const DpiManager&) = delete;
	DpiManager& operator = (const DpiManager&) = delete;

private:
	int		m_nScaleFactor;
	bool	m_bAdaptDPI;
};
}
#endif //UI_UTILS_DPIMANAGER_H_