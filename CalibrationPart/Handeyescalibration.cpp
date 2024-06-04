#include "Handeyescalibration.h"
// 信号和槽记得连起来 现在还都没链接上 singal and slot connection not build

chauroCrack::chauroCrack(cv::aruco::PredefinedDictionaryType dict, cv::Size ps, float sl, float mkl) :dictype(dict), picSize(ps), sqL(sl), mkL(mkl) 
{

}

bool chauroCrack::detector(int num)
{
	cv::Mat frame;
	cv::Mat Processedframe;
	int count = 1;
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(dictype);
	cv::Ptr<cv::aruco::CharucoBoard> board = new cv::aruco::CharucoBoard(picSize, sqL, mkL, dictionary);
	cv::Ptr<cv::aruco::DetectorParameters>params = cv::makePtr<cv::aruco::DetectorParameters>();
	

	return 1;

}

void chauroCrack::created_dataPath_rec(const QString& dataPath)
{
	createdDataPath = dataPath;
}

void chauroCrack::exist_dataPath_rec(const std::string & exist_file_name)
{
	existfile = exist_file_name;
}


void chauroCrack::camInterMax_rec(const cv::Mat& cameraMatrix)
{
	camMatrix = cameraMatrix;
}

void chauroCrack::disCoeff_rec(const cv::Mat& distCoeffs)
{
	dist = distCoeffs;
}

void chauroCrack::appendTvecRvecToFile(const cv::Vec3d& tvec, const cv::Vec3d& rvec, int i)
{
	QFile* file = new QFile(createdDataPath);
	if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
	{
		return;
	}

	QString fileContent;
	QTextStream in(file);
	QStringList fileLines = in.readAll().split("\n");

	int targetLine = 2 * i;
	while (fileLines.size() < targetLine)
	{
		fileLines.append("");
	}

	QString eyeData = QString("eye,%1,%2,%3,%4,%5,%6")
		.arg(tvec[0])
		.arg(tvec[1])
		.arg(tvec[2])
		.arg(rvec[0])
		.arg(rvec[1])
		.arg(rvec[2]);

	fileLines[targetLine - 1] = eyeData;

	file->resize(0);
	QTextStream out(file);
	for (const QString& line : fileLines)
	{
		out << line << "\n";
	}
	delete file;
}

void chauroCrack::ImgAndNum_rec(const QString& imgsetpath, const int& imgNum)
{
	imgPath = imgsetpath.toStdString();
	imgsNum = imgNum;
}

void chauroCrack::calculate_tvec_rvec()
{
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(dictype);
	cv::Ptr<cv::aruco::CharucoBoard> board = new cv::aruco::CharucoBoard(picSize, sqL, mkL, dictionary);
	cv::Ptr<cv::aruco::DetectorParameters>params = cv::makePtr<cv::aruco::DetectorParameters>();
	int count = 1;
	for (int i = 1; i <= imgsNum; i++)
	{
		std::string imagelocation = imgPath + "/Image" + std::to_string(i) + ".png";
		cv::Mat frame = cv::imread(imagelocation);
		cv::Mat Processedframe;
		cv::imshow("input image" + std::to_string(i), frame);
		frame.copyTo(Processedframe);
		std::vector<int> markerIds;
		std::vector<std::vector<cv::Point2f> > markerCorners;
		cv::aruco::detectMarkers(frame, cv::makePtr<cv::aruco::Dictionary>(board->getDictionary()), markerCorners, markerIds, params);
		std::vector<cv::Point2f> charucoCorners;
		std::vector<int> charucoIds;
		if (markerIds.size() > 0)
		{
			cv::aruco::drawDetectedMarkers(Processedframe, markerCorners, markerIds);
			cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, frame, board, charucoCorners, charucoIds, camMatrix, dist);
		}
		if (charucoIds.size() > 0) {
			cv::Scalar color = cv::Scalar(255, 0, 0);
			cv::aruco::drawDetectedCornersCharuco(Processedframe, charucoCorners, charucoIds, color);
			cv::Vec3d rvec0, tvec0;
			bool valid = cv::aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, camMatrix, dist, rvec0, tvec0);
			if (valid)
			{

				cv::drawFrameAxes(Processedframe, camMatrix, dist, rvec0, tvec0, 0.1f);
				cv::Vec3d rvecs = rvec0;
				cv::Vec3d tvecs = tvec0;
				appendTvecRvecToFile(tvecs, rvecs,i);
			}
		}
		cv::imshow("output image" + std::to_string(i), Processedframe);
	}
}

void chauroCrack::calculate_trigger_rec()
{
	calculate_tvec_rvec();
}

void chauroCrack::calculation_trigger_rec(const QString& RobotInputFormat, const QString& CalibrationType, const int & dataNum)
{
	robotInputFormat = RobotInputFormat.toStdString();
	calibrationType = CalibrationType.toStdString();
	int datanumber = dataNum;
	cv::Mat_<double> toolPose;
	cv::Mat_<double> camPose;
	readCalibrationData(toolPose, camPose, existfile);
	if (calibrationType == "EYESINHAND")
	{
		docalibrationEIH(camPose, toolPose, dataNum, robotInputFormat);
		const QString msg = "Done Eyes-In-Hand-Calibration";
		emit result_sender(msg, Hcg);
	}
	else if (calibrationType == "EYESTOHAND")
	{
		docalibrationETH(camPose, toolPose, dataNum, robotInputFormat);
		const QString msg = "Done Eyes-To-Hand-Calibration";
		emit result_sender(msg, Hcb);
	}
	else
	{
		const QString msg = "No this calibraitonType";
	}



}




// calibration calculation part
cv::Mat chauroCrack::R_T2RT(cv::Mat& R, cv::Mat& T)
{
	cv::Mat RT;
	cv::Mat_<double> R1 = (cv::Mat_<double>(4, 3) << R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2),
		R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2),
		R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2),
		0.0, 0.0, 0.0);
	cv::Mat_<double> T1 = (cv::Mat_<double>(4, 1) << T.at<double>(0, 0), T.at<double>(1, 0), T.at<double>(2, 0), 1.0);

	cv::hconcat(R1, T1, RT);
	return RT;
}

void chauroCrack::RT2R_T(cv::Mat& RT, cv::Mat& R, cv::Mat& T)
{
	cv::Rect R_rect(0, 0, 3, 3);
	cv::Rect T_rect(3, 0, 1, 3);
	R = RT(R_rect);
	T = RT(T_rect);
}

bool chauroCrack::isRotationMatrix(const cv::Mat& R)
{
	cv::Mat tmp33 = R({ 0,0,3,3 });
	cv::Mat shouldBeIdentity;

	shouldBeIdentity = tmp33.t() * tmp33;

	cv::Mat I = cv::Mat::eye(3, 3, shouldBeIdentity.type());

	return  cv::norm(I, shouldBeIdentity) < 1e-6;
}

cv::Mat chauroCrack::eulerAngleToRotatedMatrix(const cv::Mat& eulerAngle, const std::string& seq)
{
	CV_Assert(eulerAngle.rows == 1 && eulerAngle.cols == 3);

	//eulerAngle /= 180 / CV_PI;
	cv::Matx13d m(eulerAngle);
	auto rx = m(0, 0), ry = m(0, 1), rz = m(0, 2);
	auto xs = std::sin(rx), xc = std::cos(rx);
	auto ys = std::sin(ry), yc = std::cos(ry);
	auto zs = std::sin(rz), zc = std::cos(rz);

	cv::Mat rotX = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, xc, -xs, 0, xs, xc);
	cv::Mat rotY = (cv::Mat_<double>(3, 3) << yc, 0, ys, 0, 1, 0, -ys, 0, yc);
	cv::Mat rotZ = (cv::Mat_<double>(3, 3) << zc, -zs, 0, zs, zc, 0, 0, 0, 1);

	cv::Mat rotMat;

	if (seq == "zyx")		rotMat = rotX * rotY * rotZ;
	else if (seq == "yzx")	rotMat = rotX * rotZ * rotY;
	else if (seq == "zxy")	rotMat = rotY * rotX * rotZ;
	else if (seq == "xzy")	rotMat = rotY * rotZ * rotX;
	else if (seq == "yxz")	rotMat = rotZ * rotX * rotY;
	else if (seq == "xyz")	rotMat = rotZ * rotY * rotX;
	else if (seq == "zyz")  rotMat = rotZ * rotY * rotZ;
	else {
		cv::error(cv::Error::StsAssert, "Euler angle sequence string is wrong.",
			__FUNCTION__, __FILE__, __LINE__);
	}

	if (!isRotationMatrix(rotMat)) {
		cv::error(cv::Error::StsAssert, "Euler angle can not convert to rotated matrix",
			__FUNCTION__, __FILE__, __LINE__);
	}

	return rotMat;
}

cv::Mat chauroCrack::quaternionToRotatedMatrix(const cv::Vec4d& q)
{
	double w = q[0], x = q[1], y = q[2], z = q[3];

	double x2 = x * x, y2 = y * y, z2 = z * z;
	double xy = x * y, xz = x * z, yz = y * z;
	double wx = w * x, wy = w * y, wz = w * z;

	cv::Matx33d res{
		1 - 2 * (y2 + z2),	2 * (xy - wz),		2 * (xz + wy),
		2 * (xy + wz),		1 - 2 * (x2 + z2),	2 * (yz - wx),
		2 * (xz - wy),		2 * (yz + wx),		1 - 2 * (x2 + y2),
	};
	return cv::Mat(res);
}

cv::Mat chauroCrack::attitudeVectorToMatrix(cv::Mat m, bool useQuaternion, const std::string& seq)
{
	CV_Assert(m.total() == 6 || m.total() == 10);
	if (m.cols == 1)
		m = m.t();
	cv::Mat tmp = cv::Mat::eye(4, 4, CV_64FC1);

	if (useQuaternion)
	{
		cv::Vec4d quaternionVec = m({ 3, 0, 4, 1 });
		quaternionToRotatedMatrix(quaternionVec).copyTo(tmp({ 0, 0, 3, 3 }));
	}
	else
	{
		cv::Mat rotVec;
		if (m.total() == 6)
			rotVec = m({ 3, 0, 3, 1 });
		else
			rotVec = m({ 7, 0, 3, 1 });

		if (0 == seq.compare("rotVec"))
			cv::Rodrigues(rotVec, tmp({ 0, 0, 3, 3 }));
		else
			eulerAngleToRotatedMatrix(rotVec, seq).copyTo(tmp({ 0, 0, 3, 3 }));
	}
	tmp({ 3, 0, 1, 3 }) = m({ 0, 0, 3, 1 }).t();

	return tmp;
}

void chauroCrack::docalibrationEIH(cv::Mat_<double>& CalPose, cv::Mat_<double>& ToolPose, size_t num_points, const std::string robotinputformat)
{
	std::vector<cv::Mat> R_gripper2base;
	std::vector<cv::Mat> t_gripper2base;
	std::vector<cv::Mat> R_target2cam;
	std::vector<cv::Mat> t_target2cam;
	cv::Mat R_cam2gripper = (cv::Mat_<double>(3, 3));
	cv::Mat t_cam2gripper = (cv::Mat_<double>(3, 1));

	cv::Mat tempR, tempT;

	for (size_t i = 0; i < num_points; i++)
	{
		//cv::Mat tmp = attitudeVectorToMatrix(CalPose.row(i), false, "zyx");
		cv::Mat tmp = attitudeVectorToMatrix(CalPose.row(i), false, "rotVec");
		vecHc.push_back(tmp);
		RT2R_T(tmp, tempR, tempT);

		R_target2cam.push_back(tempR);
		t_target2cam.push_back(tempT);
	}

	for (size_t i = 0; i < num_points; i++)
	{
		//cv::Mat tmp = attitudeVectorToMatrix(ToolPose.row(i), false, "zyx");
		cv::Mat tmp = attitudeVectorToMatrix(ToolPose.row(i), false, robotinputformat);
		vecHg.push_back(tmp);
		RT2R_T(tmp, tempR, tempT);

		R_gripper2base.push_back(tempR);
		t_gripper2base.push_back(tempT);
	}

	calibrateHandEye(R_gripper2base, t_gripper2base, R_target2cam, t_target2cam, R_cam2gripper, t_cam2gripper, cv::CALIB_HAND_EYE_TSAI);

	Hcg = R_T2RT(R_cam2gripper, t_cam2gripper);

	//std::cout << "Hcg matrix is:" << std::endl;
	//std::cout << Hcg << std::endl;
	//std::cout << "rotation matrix or not:" << isRotationMatrix(Hcg) << std::endl << std::endl;

	//std::cout << "verify the first and second datasheet:" << std::endl;
	//std::cout << vecHg[0] * Hcg * vecHc[0] << std::endl << vecHg[1] * Hcg * vecHc[1] << std::endl << std::endl;

	//std::cout << "calibrationboardpostion arcodding to the camera coordinate:" << std::endl;
	//std::cout << vecHc[1] << std::endl;
	//std::cout << "handeyes translate matrix compare:" << std::endl;
	//std::cout << Hcg.inv() * vecHg[1].inv() * vecHg[0] * Hcg * vecHc[0] << std::endl << std::endl;

	//std::cout << "----hand eyes system testing----" << std::endl;
	//std::cout << "XYZ according to robot coordinate:" << std::endl;
	//for (int i = 0; i < vecHc.size(); ++i)
	//{
	//	cv::Mat cheesePos{ 0.0,0.0,0.0,1.0 };
	//	cv::Mat worldPos = vecHg[i] * Hcg * vecHc[i] * cheesePos;
	//	std::cout << i << ": " << worldPos.t() << std::endl;
}



void chauroCrack::docalibrationETH(cv::Mat_<double>& CalPose, cv::Mat_<double>& ToolPose, size_t num_points, const std::string robotinputformat)
{
	std::vector<cv::Mat> R_base2gripper;
	std::vector<cv::Mat> t_base2gripper;
	std::vector<cv::Mat> R_target2cam;
	std::vector<cv::Mat> t_target2cam;

	cv::Mat R_cam2base = (cv::Mat_<double>(3, 3));
	cv::Mat t_cam2base = (cv::Mat_<double>(3, 1));

	cv::Mat tempR, tempT;

	for (size_t i = 0; i < num_points; i++)
	{
		//cv::Mat tmp = attitudeVectorToMatrix(CalPose.row(i), false, "zyx");
		cv::Mat tmp = attitudeVectorToMatrix(CalPose.row(i), false, "rotVec");
		vecHc.push_back(tmp);
		RT2R_T(tmp, tempR, tempT);
		R_target2cam.push_back(tempR);
		t_target2cam.push_back(tempT);
	}

	for (size_t i = 0; i < num_points; i++)
	{
		//cv::Mat tmp = attitudeVectorToMatrix(ToolPose.row(i), false, "zyx");
		cv::Mat tmp = attitudeVectorToMatrix(ToolPose.row(i), false, robotinputformat);
		vecHg.push_back(tmp);
		RT2R_T(tmp, tempR, tempT);
		tempR = tempR.t();
		tempT = -tempR * tempT;

		R_base2gripper.push_back(tempR);
		t_base2gripper.push_back(tempT);
	}



	calibrateHandEye(R_base2gripper, t_base2gripper, R_target2cam, t_target2cam, R_cam2base, t_cam2base, cv::CALIB_HAND_EYE_TSAI);

	Hcb = R_T2RT(R_cam2base, t_cam2base);

	std::cout << "Hcb matrix is:" << std::endl;
	std::cout << Hcb << std::endl;
	std::cout << "rotation matrix or not:" << isRotationMatrix(Hcb) << std::endl << std::endl;

	std::cout << "verify the first and second datasheet:" << std::endl;
	std::cout << vecHg[0] * Hcb * vecHc[0] << std::endl << vecHg[1] * Hcb * vecHc[1] << std::endl << std::endl;

	std::cout << "calibrationboardpostion arcodding to the camera coordinate:" << std::endl;
	std::cout << vecHc[1] << std::endl;
	std::cout << "handeyes translate matrix compare:" << std::endl;
	std::cout << Hcb.inv() * vecHg[1].inv() * vecHg[0] * Hcb * vecHc[0] << std::endl << std::endl;

	std::cout << "----hand eyes system testing----" << std::endl;
	std::cout << "XYZ according to robot coordinate:" << std::endl;
	for (int i = 0; i < vecHc.size(); ++i)
	{
		cv::Mat cheesePos{ 0.0,0.0,0.0,1.0 };
		cv::Mat worldPos = vecHg[i] * Hcb * vecHc[i] * cheesePos;
		std::cout << i << ": " << worldPos.t() << std::endl;
	}
}

void chauroCrack::readCalibrationData(cv::Mat_<double>& poses, std::string file_name)
{
	cv::FileStorage fs(file_name, cv::FileStorage::READ);

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open calibration data's yaml file " << std::endl;
		return;
	}

	int count = 0;
	int totalPoints = 0;
	std::vector<std::vector<cv::Vec3d>> all_tvecs, all_rvecs;

	while (true)
	{
		std::string tvecKey = "tvec_" + std::to_string(count);
		std::string rvecKey = "rvec_" + std::to_string(count);

		std::vector<cv::Vec3d> tvecs, rvecs;
		if (!fs[tvecKey].empty() && !fs[rvecKey].empty())
		{
			fs[tvecKey] >> tvecs;
			fs[rvecKey] >> rvecs;


			all_tvecs.push_back(tvecs);
			all_rvecs.push_back(rvecs);

			totalPoints += tvecs.size();
			count++;
		}
		else
		{
			break;
		}
	}

	poses.create(totalPoints, 6);

	int rowIndex = 0;
	for (size_t i = 0; i < all_tvecs.size(); i++)
	{
		for (size_t j = 0; j < all_tvecs[i].size(); j++)
		{
			poses(rowIndex, 0) = all_tvecs[i][j][0];
			poses(rowIndex, 1) = all_tvecs[i][j][1];
			poses(rowIndex, 2) = all_tvecs[i][j][2];
			poses(rowIndex, 3) = all_rvecs[i][j][0];
			poses(rowIndex, 4) = all_rvecs[i][j][1];
			poses(rowIndex, 5) = all_rvecs[i][j][2];
			rowIndex++;
		}
	}
	fs.release();
	std::cout << "Calibration data loaded from calibration's yaml file" << std::endl;

}

void chauroCrack::readCalibrationData(cv::Mat_<double>& handPoses, cv::Mat_<double>& eyePoses, const std::string& file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open()) {
		std::cerr << "Failed to open calibration data file: " << file_name << std::endl;
		return;
	}

	std::string line;
	std::vector<std::vector<double>> hand_data;
	std::vector<std::vector<double>> eye_data;

	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string type;
		double x, y, z, rx, ry, rz;

		// Split the line by commas
		std::vector<std::string> tokens;
		std::string token;
		while (std::getline(ss, token, ',')) {
			tokens.push_back(token);
		}

		// Check if we have exactly 7 tokens (type, x, y, z, rx, ry, rz)
		if (tokens.size() == 7) {
			type = tokens[0];
			try {
				x = std::stod(tokens[1]);
				y = std::stod(tokens[2]);
				z = std::stod(tokens[3]);
				rx = std::stod(tokens[4]);
				ry = std::stod(tokens[5]);
				rz = std::stod(tokens[6]);


				//rx = rx * CV_PI / 180.0;
				//ry = ry * CV_PI / 180.0;
				//rz = rz * CV_PI / 180.0;

				std::vector<double> data = { x, y, z, rx, ry, rz };
				if (type == "hand") {
					hand_data.push_back(data);
				}
				else if (type == "eye") {
					eye_data.push_back(data);
				}
				std::cout << "Read data: " << type << ", " << x << ", " << y << ", " << z << ", " << rx << ", " << ry << ", " << rz << std::endl;
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid number format in line: " << line << std::endl;
			}
		}
		else {
			std::cerr << "Invalid line format: " << line << std::endl;
		}
	}

	handPoses.create(hand_data.size(), 6);
	for (size_t i = 0; i < hand_data.size(); ++i) {
		for (size_t j = 0; j < 6; ++j) {
			handPoses(i, j) = hand_data[i][j];

		}
	}

	eyePoses.create(eye_data.size(), 6);
	for (size_t i = 0; i < eye_data.size(); ++i) {
		for (size_t j = 0; j < 6; ++j) {
			eyePoses(i, j) = eye_data[i][j];
		}
	}

	std::cout << "Calibration data loaded from " << file_name << std::endl;
}
	
