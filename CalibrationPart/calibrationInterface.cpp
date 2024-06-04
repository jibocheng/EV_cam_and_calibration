#include "calibrationInterface.h"
#include <sstream>


calibrationInterface::calibrationInterface()
{
	m_calibration = new Ui_calibrationInterface();
	m_calibration->setupUi(this);
	m_board = new Boardgenerate();
	pchauroCrack = new chauroCrack(cv::aruco::DICT_5X5_250, cv::Size(5, 7), 0.03f, 0.02f);

	//pushbutton handling:
	connect(m_calibration->GenerateBoard, &QPushButton::clicked, this, &calibrationInterface::generate_press);
	connect(m_board, &Boardgenerate::message, this, &calibrationInterface::board_generation_result);
	connect(m_calibration->selectbutton, &QPushButton::clicked, this, &calibrationInterface::selectDataFilePath);
	connect(m_calibration->poseInput, &QPushButton::clicked, this, &calibrationInterface::robotPoseInput);
	connect(m_calibration->create,& QPushButton::clicked, this, &calibrationInterface::createTxt);
	connect(m_calibration->matrixdisInput, &QPushButton::clicked, this, &calibrationInterface::matrixSender);
	connect(m_calibration->readImageset, &QPushButton::clicked, this, &calibrationInterface::imageRead2dataInput);
	connect(m_calibration->NumInput, &QPushButton::clicked, this, &calibrationInterface::imageNumInput);
	connect(m_calibration->calculateCamPose, &QPushButton::clicked, this, &calibrationInterface::calculateImageVector);
	connect(m_calibration->readDataset, &QPushButton::clicked, this, &calibrationInterface::readExistdatafile);
	connect(m_calibration->calculate, &QPushButton::clicked, this, &calibrationInterface::calculateFinal);
	// slot and signal handling:
	connect(this, &calibrationInterface::cameraMatrix_sender, pchauroCrack, &chauroCrack::camInterMax_rec);
	connect(this, &calibrationInterface::distCoeffs_sender, pchauroCrack, &chauroCrack::disCoeff_rec);
	connect(this, &calibrationInterface::created_dataPath_sender, pchauroCrack, &chauroCrack::created_dataPath_rec);
	connect(this, &calibrationInterface::ImagePath_sender, pchauroCrack, &chauroCrack::ImgAndNum_rec);
	connect(this, &calibrationInterface::calculater_trigger_sender, pchauroCrack, &chauroCrack::calculate_trigger_rec);
	connect(this, &calibrationInterface::exist_fileName_sender, pchauroCrack, &chauroCrack::exist_dataPath_rec);
	connect(this, &calibrationInterface::final_calculation_trigger, pchauroCrack, &chauroCrack::calculation_trigger_rec);
	connect(pchauroCrack, &chauroCrack::result_sender, this, &calibrationInterface::result_rec);
}

calibrationInterface::~calibrationInterface()
{
	if (m_calibration != NULL)
	{
		delete m_calibration;
	}
	if (m_board != NULL)
	{
		delete m_board;
	}
	if (dataFile != NULL)
	{
		if (dataFile->isOpen())
			dataFile->close();

		delete dataFile;
	}
	if (pchauroCrack != NULL)
	{
		delete pchauroCrack;
	}
}

//button handling:
void calibrationInterface::generate_press()
{
	m_board->createBoard();
}

void calibrationInterface::selectDataFilePath()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
	{
		m_calibration->DataFliespath->setText(dir);
	}
}
void calibrationInterface::createTxt()
{
	if (m_calibration->FileName->text().isEmpty())
	{
		m_calibration->commandWindow->append("Please enter filename to save robot pose data and camera pose data!");
	}
	else
	{
		const QString filePath = m_calibration->DataFliespath->text() + "/" + m_calibration->FileName->text() + ".txt";
		
		emit created_dataPath_sender(filePath);

		if (dataFile == NULL)
		{
			dataFile = new QFile(filePath);
		}
		QString msg;
		if (dataFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		{
			msg = "select FilePath success";
		}
		else
		{		
			msg = "select FilePath failed please try again!";
			msg += "\nError: " + dataFile->errorString();
		}

		m_calibration->commandWindow->append(msg);
	}
	 
}

void calibrationInterface::robotPoseInput()
{
	if (!dataFile->isOpen())
	{
		dataFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	}

	QTextStream out(dataFile);
	if (m_calibration->x->text().isEmpty() || m_calibration->y->text().isEmpty() || m_calibration->z->text().isEmpty() || m_calibration->rz->text().isEmpty())
	{
		m_calibration->commandWindow->append("Check your Robot pose. Blank will not be accept!");
	}
	else
	{
		QString robotPose = "hand," + m_calibration->x->text() + "," + m_calibration->y->text() + "," + m_calibration->z->text() + "," + m_calibration->rx->text() + "," + m_calibration->ry->text() + "," + m_calibration->rz->text();
		if (dataFile->size() == 0)
		{
			out << robotPose << "\n";
		}
		else
		{
			out << "\n" << robotPose << "\n";
		}
		m_calibration->commandWindow->append("success input robotpose");
	}
}

void calibrationInterface::matrixSender()
{
	if (m_calibration->fx->text().isEmpty() || m_calibration->fy->text().isEmpty() || m_calibration->cx->text().isEmpty() || m_calibration->cy->text().isEmpty() || m_calibration->d1->text().isEmpty() || m_calibration->d2->text().isEmpty() || m_calibration->d3->text().isEmpty() || m_calibration->d4->text().isEmpty() || m_calibration->d5->text().isEmpty())
	{
		m_calibration->commandWindow->append("camera internistic or distcoefficient can't be empty");
	}
	else
	{
		double fx, fy, cx, cy, d1, d2, d3, d4, d5;
		fx = m_calibration->fx->text().toDouble();
		fy = m_calibration->fy->text().toDouble();
		cx = m_calibration->cx->text().toDouble();
		cy = m_calibration->cy->text().toDouble();
		d1 = m_calibration->d1->text().toDouble();
		d2 = m_calibration->d2->text().toDouble();
		d3 = m_calibration->d4->text().toDouble();
		d4 = m_calibration->d4->text().toDouble();
		d5 = m_calibration->d5->text().toDouble();
		const cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
		const cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << d1, d2, d3, d4, d5);
		emit cameraMatrix_sender(cameraMatrix);
		emit distCoeffs_sender(distCoeffs);
		m_calibration->commandWindow->append("camera internistic matrix and distcoefficents are already sent!");
	}
}

void calibrationInterface::imageRead2dataInput()
{
	QString Imgdir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!Imgdir.isEmpty())
	{
		m_calibration->Imagesetflie->setText(Imgdir);
	}
}

void calibrationInterface::imageNumInput()
{
	if (!m_calibration->Imagesetflie->text().isEmpty() && !m_calibration->picnum->text().isEmpty())
	{
		const QString imgPath = m_calibration->Imagesetflie->text();
		const int picNum = m_calibration->picnum->text().toInt();
		emit ImagePath_sender(imgPath,picNum);
	}
	else
	{
		m_calibration->commandWindow->append("Please enter imageset path or image nummber!");
	}
}

void calibrationInterface::calculateImageVector()
{
	emit calculater_trigger_sender();
}

void calibrationInterface::readExistdatafile()
{
	QString exist_file_path0 = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));
	if (!exist_file_path0.isEmpty())
	{
		m_calibration->camerafilepath->setText(exist_file_path0);
		const std::string exist_file_path = exist_file_path0.toStdString();
		emit exist_fileName_sender(exist_file_path);
	}
}

void calibrationInterface::calculateFinal()
{
	const QString robotinputformat = m_calibration->robotinputformat->currentText();
	const QString calibrationtype = m_calibration->calibrationtype->currentText();
	if (!m_calibration->dataNum->text().isEmpty())
	{
		const int dataNum = m_calibration->dataNum->text().toInt();
		emit final_calculation_trigger(robotinputformat, calibrationtype,dataNum);
	}
	else
	{
		m_calibration->commandWindow->append("Please give the number of data!");
	}
}

void calibrationInterface::result_rec(const QString& msg,const cv::Mat& Hc)
{
	m_calibration->commandWindow->append(msg);
	std::ostringstream oss;
	for (int i = 0; i < Hc.rows; ++i)
	{
		for (int j = 0; j < Hc.cols; ++j)
		{
			oss << Hc.at<double>(i, j);
			if (j < Hc.cols - 1)
			{
				oss << "\t"; 
			}
		}
		oss << "\n";
	}
	QString Hc_qstr = QString::fromStdString(oss.str());

	if (msg == "Done Eyes-In-Hand-Calibration")
	{
		m_calibration->commandWindow->append("The final Matrix Hcg:");
		m_calibration->commandWindow->append(Hc_qstr);
	}
	else if (msg == "Done Eyes-To-Hand-Calibration")
	{
		m_calibration->commandWindow->append("The final Matrix Hcb:");
		m_calibration->commandWindow->append(Hc_qstr);
	}
}

// message function:
void calibrationInterface::board_generation_result(const QString& message)
{
	m_calibration->commandWindow->append(message);
}