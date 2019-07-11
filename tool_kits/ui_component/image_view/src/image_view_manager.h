#ifndef IMAGE_VIEW_MANAGER_H__
#define IMAGE_VIEW_MANAGER_H__

#include "image_view_form.h"


class ImageViewManager
{
public:
	SINGLETON_DEFINE(ImageViewManager);

	struct ImageViewInfo
	{
		std::wstring id_;
		std::wstring url_;
		std::wstring path_;
	};
	typedef std::vector<ImageViewInfo> ImageViewInfoList;
	typedef std::function<void()> ImageViewSetNoTopMostCB;
	ImageViewManager();
	~ImageViewManager();

	bool StartViewPic(const std::wstring &path_pic, std::wstring message_id, bool size, bool nosave = false);
	ImageViewSetNoTopMostCB StartViewPicEx(const std::wstring &path_pic, std::wstring message_id, bool size, bool nosave = false,bool topmost=false);
	//�ж�form_id�Ƿ���ͼƬԤ�����������һ��
	bool JudgeViewImage(const std::wstring &form_id);
	//ͼƬԤ������ر�ʱ����������ָ��ͼƬԤ�������ָ��ΪNULL
	void ResetImageViewWndPoint(void);

	bool SetImageViewList(ImageViewInfoList info_list, std::wstring message_id);

	//��ʾ��һ��/��һ��ͼƬ
	bool ShowNextImageView(bool next);
	//�ж��Ƿ������һ��/��һ��ͼƬ
	bool IsExistNextImage(bool next);
	void GetImagePage(long& cur, long& sum);

	//ͼƬ׼������
	void OnImageReady(std::wstring &path_pic, bool size);

private:
	ImageViewForm *image_view_window_;
	std::wstring   message_id_;

	ImageViewInfoList image_view_info_list_;
};
#endif // IMAGE_VIEW_MANAGER_H__