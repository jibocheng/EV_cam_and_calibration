#include "Boardgenerate.h"

Boardgenerate::Boardgenerate() :dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250)), board(cv::Size(5,7),0.03f,0.02f,dictionary),imageSize(768, 960)
{
}

void Boardgenerate::createBoard() 
{
	cv::Mat calibrationboard;
	QString board_msg;
	board.generateImage(imageSize, calibrationboard, 10, 1);
	if (cv::imwrite("D:/cameraIntegrator/camerapart/CalibrationPart/outputfiles/calibrationboard.png", calibrationboard))
		board_msg = "calibrationboard has already been generated!";
	else
		board_msg = "calibrationboard can't be generated!";

	emit message(board_msg);
}

