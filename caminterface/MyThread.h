//#pragma once
//#include <QObject>
//#include <QThread>
//#include <QtGui/QImage>
//#include "cameraManager.h"
//
//class MyThread:public QThread
//{
//	Q_OBJECT
//public:
//	explicit MyThread(QObject* parent = nullptr);
//	~MyThread();
//
//	void run();
//	void getCameraPtr(ManagerRealSense* p_rs);
//	void getImagePtr(cv::Mat* p_img);
//
//signals:
//	void signal_message();
//	void signal_messImage(QImage myImage);
//
//private:
//	ManagerRealSense* rsPtr = NULL;
//	cv::Mat* imgPtr = NULL;
//	QImage* myImage = NULL;
//};
//
