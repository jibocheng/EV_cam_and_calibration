#pragma once
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <string>
#include <fstream>
#include <QtWidgets/QWidget>
#include <QFile>
#include <QTextStream>

class chauroCrack : public QWidget
{
	Q_OBJECT
public:
	chauroCrack(cv::aruco::PredefinedDictionaryType dict, cv::Size ps, float sl, float mkl);
	~chauroCrack() {};
	bool detector(int num);
	void appendTvecRvecToFile(const cv::Vec3d& tvec, const cv::Vec3d& rvec, int i);
	void calculate_tvec_rvec();
	


signals:
	void tvec_signal(const cv::Vec3d& tvecs);
	void rvec_signal(const cv::Vec3d& rvecs);
	void result_sender(const QString& msg, const cv::Mat& Hc);

public slots:
	void camInterMax_rec(const cv::Mat& cameraMatrix);
	void disCoeff_rec(const cv::Mat& distCoeffs);
	void ImgAndNum_rec(const QString& imgsetpath, const int& imgNum);
	void calculate_trigger_rec();
	void created_dataPath_rec(const QString& dataPath);
	void exist_dataPath_rec(const std::string& exist_file_name);
	void calculation_trigger_rec(const QString& RobotInputFormat, const QString& calibrationType,const int& dataNum);
private:
	cv::Size picSize;
	float sqL;
	float mkL;
	cv::aruco::PredefinedDictionaryType dictype;
	cv::Mat camMatrix;
	cv::Mat dist;
	std::string imgPath;
	int imgsNum;
	QString createdDataPath;
	std::string existfile;
	std::string robotInputFormat;
	std::string calibrationType;


// calculation part:
public:
	//data handling:
	void readCalibrationData(cv::Mat_<double>& poses, std::string file_name);
	void readCalibrationData(cv::Mat_<double>& handPoses, cv::Mat_<double>& eyePoses, const std::string& file_name);
	cv::Mat R_T2RT(cv::Mat& R, cv::Mat& T);
	void RT2R_T(cv::Mat& RT, cv::Mat& R, cv::Mat& T);
	bool isRotationMatrix(const cv::Mat& R);
	cv::Mat eulerAngleToRotatedMatrix(const cv::Mat& eulerAngle, const std::string& seq);
	cv::Mat quaternionToRotatedMatrix(const cv::Vec4d& q);
	cv::Mat attitudeVectorToMatrix(cv::Mat m, bool useQuaternion, const std::string& seq);
	void docalibrationEIH(cv::Mat_<double>& CalPose, cv::Mat_<double>& ToolPose, size_t num_points, const std::string robotinputformat);
	void docalibrationETH(cv::Mat_<double>& CalPose, cv::Mat_<double>& ToolPose, size_t num_points, const std::string robotinputformat);
	cv::Mat getHcg() { return Hcg; }
	cv::Mat getHcb() { return Hcb; }

private:
	cv::Mat Hcg;
	cv::Mat Hcb;
	std::vector<cv::Mat> vecHg;
	std::vector<cv::Mat> vecHc;
};

