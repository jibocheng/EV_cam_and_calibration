#pragma once
#include "MvCameraControl.h"
#include "MvStereoAppCtrl.h"
#include "MvStereoAppDefine.h"
#include "MvStereoAppDefineEx.h"
#include "MvStereoAppPixelType.h"
#include "MvStereoAppCtrlEx.h"
#include <vector>
#include <iostream>
#include <string>
#include "definition.h"
#include <opencv2/opencv.hpp>
#include "Mv3dRgbdApi.h"
#include "Mv3dRgbdDefine.h"
#include "Mv3dRgbdImgProc.h"
using std::cout;
using std::endl;
using std::string;
using cv::Mat;
using std::vector;

class ManagerHikvision3D
{
public:
	ManagerHikvision3D();
	ManagerHikvision3D(Imagetype it);
	~ManagerHikvision3D();
	void setup(Imagetype it[], int num);
	void capture(int which_one, Imagetype it);
	void run(int which_one, Imagetype it);
	void setupRGB(Imagetype it[], int num);
	void captureRGB(int which_one, Imagetype it);

private:
	int nRet = MV_STA_OK;
	int total_num;
	MV_STEREOCAM_NET_INFO_LIST* camInfoList = NULL;
	MV3D_RGBD_DEVICE_INFO pstDeviceInfos;
	vector<void*> handles;
	vector<Mat> frames;

private:
	bool detection();
	bool detectionRGB();
};

