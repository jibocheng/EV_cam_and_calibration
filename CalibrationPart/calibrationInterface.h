#pragma once
#include "ui_calibrationInterface.h"
#include "Boardgenerate.h"
#include "Handeyescalibration.h"
#include <QtWidgets/QFileDialog>
#include <QFile>
#include <QTextStream>


class calibrationInterface:public QWidget
{
	Q_OBJECT
public:
	calibrationInterface();
	~calibrationInterface();

signals:
	void cameraMatrix_sender(const cv::Mat& cameraMatrix);
	void distCoeffs_sender(const cv::Mat& distCoeffs);
	void ImagePath_sender(const QString& imagePath, const int& num);
	void created_dataPath_sender(const QString& dataPath);
	void exist_dataPath_sender(const QString& dataPath);
	void calculater_trigger_sender();
	void exist_fileName_sender(const std::string& fileName);
	void final_calculation_trigger(const QString& robotInputFormat, const QString& calibrationType, const int & dataNum);



private slots: // for button
	void generate_press();
	void selectDataFilePath();
	void createTxt();
	void robotPoseInput();
	void imageRead2dataInput();
	void matrixSender();
	void imageNumInput();
	void calculateImageVector();
	void readExistdatafile();
	void calculateFinal();
	void result_rec(const QString& msg, const cv::Mat& Hc);

private slots: //for data transfor
	void board_generation_result(const QString& message);
private:
	Ui_calibrationInterface* m_calibration = NULL;
	Boardgenerate* m_board = NULL;
	QFile * dataFile = NULL;
	chauroCrack* pchauroCrack = NULL;

	
	
};

