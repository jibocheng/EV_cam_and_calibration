#pragma once
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include "opencv2/opencv.hpp" 
#include"opencv2/xfeatures2d.hpp"
class Picdetection:public QObject
{
	Q_OBJECT
public:
	Picdetection() {};
	~Picdetection() {};
	bool poseEstimation();

public slots:
	void receive_files(QString folder_path);

private:
	QString path;
	std::vector<cv::Mat> pic_list;
};

