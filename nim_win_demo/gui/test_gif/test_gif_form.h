#pragma once
class TestGifForm : public nim_comp::WindowEx
{
public:
	TestGifForm();
	~TestGifForm();

	//�����麯��
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual UINT GetClassStyle() const override;

	/**
	* ���ڳ�ʼ������
	* @return void	�޷���ֵ
	*/
	virtual void InitWindow() override;
public:
	static const LPCTSTR kClassName;
};