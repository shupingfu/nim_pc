#ifndef UI_IMAGE_VIEW_H__
#define UI_IMAGE_VIEW_H__


class UiImageView : public ui::Control
{
public:
	enum PicDirectionOfRotation
	{
		kClockWise = 0,		//˳ʱ��
		kAntiClockWise		//��ʱ��
	};
	enum PicZoom
	{
		kMagnifyPic = 0,	//�Ŵ�
		kReducePic			//��С
	};

	UiImageView();
	~UiImageView(void);

public:
	virtual void DoInit() override;
	virtual void HandleMessage(ui::EventArgs& event) override;
	virtual void Paint(HDC hDC, const ui::UiRect& rcPaint) override;
	

	//�ⲿ�ӿ�
public:
	//��תͼƬ
	void RotatePic(PicDirectionOfRotation direction);
	//����ͼƬ
	void ScalePic(PicZoom zoom);
	//����Ҫ��ʾ��ͼƬ·��
	int SetPicPath(std::wstring image_path);

	UINT GetImageWidth();
	UINT GetImageHeight();

	//�ж�����Ƿ���ͼƬ���Լ�ͼƬ�Ƿ���Ա��϶�
	bool PtInImageView(POINT point); 
	
	//����ͼƬΪԭʼ�ߴ�
	void SetOriginalSize();

	/*
	*	@brief ���ݴ����Ԥ��ؼ���ߺ�ͼƬ��ߣ����㽫Ҫ��ʾ��ͼƬ�����ű�
	*	@param control_width:Ԥ��Ŀؼ����
	*	@param control_height:Ԥ��Ŀؼ��߶�
	*	@param image_width :Ԥ���ͼƬ���
	*	@param image_height :Ԥ���ͼƬ�߶�
	*/
	float GetDefaultScale(int control_width, int control_height, int image_width, int image_height);

	//������ʾ��ͼƬ��ƫ�����������û��϶�ͼƬ
	void SetPicOffset(int offset_x, int offset_y);

	bool IsAdjustScale(void){ return bAdjustScale_; };
	int GetAngle(void){ return angle_; }
	float GetScale(void){ return scale_; }
	static LPCTSTR kControlName;

private:
	void SetAngle(int angle);
	void SetScale(float scale);

	//����һ����ת���λͼ
	HBITMAP GetRotatedBitmap(HDC hDC, HBITMAP hBitmap, float radians, COLORREF clrBack);

	//��ȡͼƬ����ת����
	static Gdiplus::RotateFlipType GetRotateFlipType(Gdiplus::Image& image);
	static Gdiplus::RotateFlipType GetRotateFlipType(short direction);

	//����ƫ���������ݿؼ��Ĵ�С����ʾ��ͼƬ�Ĵ�С�����������ƫ����
	void CheckOffset(int& nOffset, int nControl, int nImage);

private:

	int angle_;							//ͼƬ����ת�Ƕ�
	float scale_;						//ͼƬ���ű���
	std::wstring image_path_;			//ͼƬ���ص�ַ

	std::unique_ptr<Gdiplus::Image> image_;//���ͼƬ��Ϣ
	std::unique_ptr<Gdiplus::Bitmap> image_transform_;//��Ŵ�����λͼ
	
	RECT rect_image_;					//ͼƬ��С
	bool can_moving_;					//ͼƬ�Ƿ�����ƶ�
	int offset_x_;						//ͼƬ���Ƶĺ���ƫ����
	int offset_y_;						//ͼƬ���Ƶ�����ƫ����


	bool bAdjustScale_;

};
#endif // UI_IMAGE_VIEW_H__