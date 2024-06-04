#pragma once
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include "opencv2/opencv.hpp" 
#include"opencv2/xfeatures2d.hpp"

class Boardgenerate : public QObject
{
	Q_OBJECT
public:
	Boardgenerate();
	~Boardgenerate() {};
	void createBoard();
	cv::aruco::Dictionary get_dict() { return dictionary;}
	cv::aruco::CharucoBoard get_board() { return board; }
signals:
	void message(QString Board_msg);
private:
	cv::aruco::Dictionary dictionary;
	cv::aruco::CharucoBoard board;
	cv::Size imageSize;
	
};

