//#include "MyThread.h"
//
//
//MyThread::MyThread(QObject* parent) : QThread{ parent }
//{
//	myImage = new QImage();
//}
//
//MyThread :: ~MyThread()
//{
//	if (myImage != NULL)
//	{
//		delete myImage;
//	}
//	if (rsPtr != NULL)
//	{
//		delete rsPtr;
//	}
//	if (imgPtr != NULL)
//	{
//		delete imgPtr;
//	}
//}
//
//void MyThread::getCameraPtr(ManagerRealSense* rs)
//{
//	rsPtr = rs;
//}
//
//void MyThread::getImagePtr(cv::Mat* image)
//{
//	imgPtr = image;
//}
//
//void MyThread::run()
//{
//	if (rsPtr == NULL || imgPtr == NULL)
//	{
//		return;
//	}
//
//	if (imgPtr->channels() > 1)
//	{
//		*myImage = QImage((const unsigned char*)(imgPtr->data), imgPtr->cols, imgPtr->rows, QImage::Format_RGB888);
//	}
//	else
//	{
//		*myImage = QImage((const unsigned char*)(imgPtr->data), imgPtr->cols, imgPtr->rows, QImage::Format_Indexed8);
//	}
//
//	
//}