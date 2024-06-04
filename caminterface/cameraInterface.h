#pragma once
#include "ui_caminter.h"
#include "cameraManager.h"
#include "cameraSetup.h"


class cameraInterface:public QWidget
{
	Q_OBJECT
public:
	cameraInterface(QWidget* parent = nullptr);
	~cameraInterface();



private slots:
	void updateRunButtonstate();
	// from another class signal
	void slot_receive_it(const Imagetype it_set);
	void slot_image_display(const QImage& Img2dis);
	void slot_images_display(const QImage& Img2dis, const int imgIdx);
	void slot_image_rgbd_display(const QImage& RGB, const QImage& D);
	void slot_images_rgbd_display(const QImage& RGB, const QImage& D,const int imgIdx);

private slots:
	void cameraName();
	void cameraSettings();
	void cameraRun();
	void cameraStop();
	void runAll();
	void stopAll();
	

private:
	Ui_camdisplay* m_camdisplay = NULL;
	ManagerRealSense* m_rs = NULL;
	camera_info* p_cameraInfo_list = NULL;
	QImage compositeImage;
	Imagetype* p_itList = NULL;
	// setting windows
	cameraSetup* m_camSet = NULL;
};



