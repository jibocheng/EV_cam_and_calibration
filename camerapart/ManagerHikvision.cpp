#include "ManagerHikvision.h"
using std::cout;
using std::endl;
using std::string;
using cv::Mat;
using std::vector;
ManagerHikvision3D::ManagerHikvision3D()
{
	if (detection())
		cout << "GOOD!" << endl;
	else
		cout << "BAD" << endl;

	frames.resize(frames_length);
}
ManagerHikvision3D::~ManagerHikvision3D()
{
	if (camInfoList != NULL)
	{
		delete camInfoList;
	}
	MV3D_RGBD_CloseDevice(&handles[0]);
	MV3D_RGBD_Release();
}
void ManagerHikvision3D::setup(Imagetype it[], int num)
{
	nRet = MV_STA_CreateHandleByCameraInfo(&handles[num], camInfoList->pDeviceInfo[num]);
	if (nRet != MV_STA_OK)
	{
		cout << "BAD" << nRet << endl;
		return;
	}
	cout << "GOOD" << endl;
}
bool ManagerHikvision3D::detection()
{
	camInfoList = new MV_STEREOCAM_NET_INFO_LIST;
	nRet = MV_STA_EnumStereoCam(camInfoList);

	if (nRet != 0)
	{
		cout << "Error code : " << nRet << endl;
		return 0;
	}

	cout << "Got camera info successfully!" << endl;
	total_num = camInfoList->nDeviceNum;
	cout << "Total : " << total_num << " cameras are connected!" << endl;
	for (int i = 0; i < total_num; i++)
	{
		cout << "The device's ip adress: " << camInfoList->pDeviceInfo[i]->nCurrentIp << endl;
		cout << "The device's Name: " << camInfoList->pDeviceInfo[i]->chModelName << endl;
		cout << "The device's serialNum: " << camInfoList->pDeviceInfo[i]->chSerialNumber << endl;
		void* handle;
		handles.push_back(handle);
	}


	return 1;

}
void ManagerHikvision3D::capture(int which_one, Imagetype it)
{


	nRet = MV_STA_OpenDevice(handles[which_one]);
	if ( nRet != MV_STA_OK)
	{
		cout << "cant open camera" << endl;
		return;
	}
	cout << "GOOD" << endl;

	MV_STA_ENUMVALUE * pixeltype = {0};
	nRet = MV_STA_SetEnumValue(handles[which_one], "PixelFormat", STC_PixelType_Gvsp_BGR8_Packed);
	if (nRet != MV_STA_OK)
	{
		cout << "can't get this type" << endl;
		cout << nRet << endl;
		return;
	}
	cout << "GOOD" << endl;


	nRet = MV_STA_Start(handles[which_one]);

	if (nRet != MV_STA_OK)
	{
		cout << "Cant start this handle" << endl;
		return;
	}
	
	STC_DataSet pDataSet = NULL;
	STC_Object DataObj;
	unsigned int nResultCount = 0;
	unsigned int nMsgType;
	
	

	while (true)
	{
		int key = cv::waitKey(1);
		if (MV_STA_ReceiveDataTimeout(handles[which_one], &pDataSet, 1000))
		{
			cout << "frame asqution failed!" << endl;
			break;
		}
		if (pDataSet != NULL)
		{
			nResultCount = MV_STA_DataSetCount(pDataSet);
			for (unsigned int i = 0; i < nResultCount; i++)
			{
				DataObj = MV_STA_DataSetAt(pDataSet, i);
				nMsgType = MV_STA_DataMsgType(DataObj);


				if (STC_DATA_MSG_TYPE_IMG_RAW == nMsgType)
				{
					STC_DATA_IMAGE stImg = { 0 };
					nRet = MV_STA_GetImage(DataObj, &stImg);
					//stImg.enPixelType = STC_PixelType_Gvsp_RGB8_Planar;
					if (!nRet)
						cout << "Good" << endl;
					else
					{
						cout << "Bad" << endl;
						break;
					}

					bool isMono;
					switch (stImg.enPixelType)
					{
					case STC_PixelType_Gvsp_Mono8:
					case STC_PixelType_Gvsp_Mono10:
					case STC_PixelType_Gvsp_Mono10_Packed:
					case STC_PixelType_Gvsp_Mono12:
					case STC_PixelType_Gvsp_Mono12_Packed:
						isMono = true;
						break;
					default:
						isMono = false;
						break;
					}
					
					if (isMono)
					{
						Mat frame = Mat(stImg.nHeight, stImg.nWidth, CV_8UC1, stImg.pData);
						cv::imshow("frame", frame);
					}
					else
					{
						
						Mat frame = Mat(stImg.nHeight, stImg.nWidth, CV_8UC3, stImg.pData);
						cv::imshow("frame", frame);
					}

				}
				else if (STC_DATA_MSG_TYPE_DWS_VOLUME == nMsgType)
				{
					STC_DWS_VOLUME_INFO stVolumInfo = { 0 };
					nRet = MV_STA_GetDwsVolumeInfo(DataObj, &stVolumInfo);
					if (!nRet)
						cout << "Good" << endl;
					else
					{
						cout << "Bad" << endl;
						break;
					}
				}
				else if (STC_DATA_MSG_TYPE_STEREO_VOLUME == nMsgType)
				{
					STC_DWS_VOLUME_INFO stVolumInfo = { 0 };
					nRet = MV_STA_GetDwsVolumeInfo(DataObj, &stVolumInfo);
					if (!nRet)
						cout << "Good" << endl;
					else
					{
						cout << "Bad" << endl;
						break;
					}
				}
				else
				{
					cout << "no support msg type" << endl;
				}

			}

		}


		int dataObjectType = MV_STA_DataMsgType(pDataSet);
		cout << "Now the type of the data is " << dataObjectType << endl;
		if (key == 's')
		{
			break;
		}
		
		
	}

	nRet = MV_STA_Stop(handles[which_one]);
	nRet = MV_STA_DestroyHandle(handles[which_one]);

}
ManagerHikvision3D::ManagerHikvision3D(Imagetype it)
{
	if (it != RGB)
	{
		return;
	}

	if (!detectionRGB())
	{
		return;
	}
	cout << "Good" << endl;
}
bool ManagerHikvision3D::detectionRGB()
{
	nRet = MV3D_RGBD_Initialize();
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "can't initialize the camera" << endl;
		return 0;
	}
	unsigned int pDeviceNumber = 0;
	nRet = MV3D_RGBD_GetDeviceNumber(DeviceType_Ethernet, &pDeviceNumber);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "can't get devices information" << endl;
		return 0;
	}
	cout << "device number : " << pDeviceNumber << endl;

	nRet = MV3D_RGBD_GetDeviceList(DeviceType_Ethernet, &pstDeviceInfos, pDeviceNumber, &pDeviceNumber);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "Get device infomation failed " << endl;
		return 0;
	}
	total_num = pDeviceNumber;
	cout << "Total connected is : " << total_num << endl;
	cout << "Device name : " << pstDeviceInfos.chModelName << endl;
	cout << "Device serialNum : " << pstDeviceInfos.chSerialNumber << endl;
	void* handle;
	handles.push_back(handle);

	return 1;
	
}
void ManagerHikvision3D::setupRGB(Imagetype it[], int num)
{
	nRet = MV3D_RGBD_OpenDevice(&handles[num], &pstDeviceInfos);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant open device" << endl;
		return;
	}

	MV3D_RGBD_PARAM pstValue;
	memset(&pstValue, 0, sizeof(MV3D_RGBD_PARAM));

	
	nRet = MV3D_RGBD_GetParam(handles[num], MV3D_RGBD_ENUM_WORKINGMODE, &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant get information" << endl;
	}

	cout << "Now the working mode is: " << pstValue.ParamInfo.stEnumParam.nCurValue << endl;
	cout << "Now the working mode support num: " << pstValue.ParamInfo.stEnumParam.nSupportedNum << endl;
	cout << "Now the working mode support value: " << pstValue.ParamInfo.stEnumParam.nSupportValue << endl;

	pstValue.enParamType = ParamType_Enum;
	pstValue.ParamInfo.stEnumParam.nCurValue = 5;
	nRet = MV3D_RGBD_SetParam(handles[num], MV3D_RGBD_ENUM_WORKINGMODE, &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant set information" << endl;
	}

	nRet = MV3D_RGBD_GetParam(handles[num], MV3D_RGBD_ENUM_WORKINGMODE, &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant get information" << endl;
	}
	cout << "Now the working mode is: " << pstValue.ParamInfo.stEnumParam.nCurValue << endl;
	cout << "Now the working mode support num: " << pstValue.ParamInfo.stEnumParam.nSupportedNum << endl;
	cout << "Now the working mode support value: " << pstValue.ParamInfo.stEnumParam.nSupportValue << endl;

	
	nRet = MV3D_RGBD_GetParam(handles[num], MV3D_RGBD_ENUM_IMAGEMODE, &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant get information" << endl;
	}

	cout << "Now the Image Mode is: " << pstValue.ParamInfo.stEnumParam.nCurValue << endl;
	cout << "Now the Image Mode support num: " << pstValue.ParamInfo.stEnumParam.nSupportedNum << endl;
	cout << "Now the Image Mode support value: " << pstValue.ParamInfo.stEnumParam.nSupportValue << endl;
	
	nRet = MV3D_RGBD_GetParam(handles[num], "PixelFormat", &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant get information" << endl;
	}

	cout << "Now the Pixel format is: " << pstValue.ParamInfo.stEnumParam.nCurValue << endl;
	cout << "Now the Pixel format support num: " << pstValue.ParamInfo.stEnumParam.nSupportedNum << endl;
	cout << "Now the Pixel format support value: " << pstValue.ParamInfo.stEnumParam.nSupportValue << endl;

	pstValue.enParamType = ParamType_Enum;
	pstValue.ParamInfo.stEnumParam.nCurValue = MV3D_RGBD_PIXEL_COLOR;

	nRet = MV3D_RGBD_SetParam(handles[num], "PixelFormat", &pstValue);
	
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant set information" << endl;
	}
	nRet = MV3D_RGBD_GetParam(handles[num], "PixelFormat", &pstValue);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "cant get information" << endl;
	}

	cout << "Now the Pixel format is: " << pstValue.ParamInfo.stEnumParam.nCurValue << endl;
	cout << "Now the Pixel format support num: " << pstValue.ParamInfo.stEnumParam.nSupportedNum << endl;
	cout << "Now the Pixel format support value: " << pstValue.ParamInfo.stEnumParam.nSupportValue << endl;

	//nRet = MV3D_RGBD_ExportAllParam(handles[num], "HikParams");
	//if (nRet != MV3D_RGBD_OK)
	//{
	//	cout << "can't Export all the params" << endl;
	//}

	//nRet = MV3D_RGBD_SetParam(handles[num], "PixelFormat", &pstValue);

}
void ManagerHikvision3D::captureRGB(int which_one, Imagetype it)
{
	nRet = MV3D_RGBD_Start(handles[which_one]);
	if (nRet != MV3D_RGBD_OK)
	{
		cout << "can't start device " << endl;
	}

	cout << "camera starts !" << endl;

	MV3D_RGBD_FRAME_DATA stFrameData = { 0 };
	while (true)
	{
		int key = cv::waitKey(1);
		nRet = MV3D_RGBD_FetchFrame(handles[which_one], &stFrameData, 5000);
		if (nRet != MV3D_RGBD_OK)
		{
			cout << "can't catch the image" << endl;
			return;
		}

		cout << "total image:" << stFrameData.nImageCount << endl;
		bool isMono = true;
		cout << "Image type:" << stFrameData.stImageData->enImageType << endl;
		cout << "Stream type:" << stFrameData.stImageData->enStreamType << endl;
		switch (stFrameData.stImageData->enImageType)
		{
		case ImageType_Mono8:
		case ImageType_Depth:
			break;
		case ImageType_RGB8_Planar:
			isMono = false;
		default:
			break;
		}
		if (isMono)
		{
			
			Mat stImg = Mat(stFrameData.stImageData->nHeight, stFrameData.stImageData->nWidth, CV_8UC1, stFrameData.stImageData->pData);
			cv::imshow("detecting", stImg);
		}
		else
		{
			Mat stImg = Mat(stFrameData.stImageData->nHeight, stFrameData.stImageData->nWidth, CV_8UC3, stFrameData.stImageData->pData);
			cv::imshow("detecting", stImg);
		}

		if (key == 's')
		{
			MV3D_RGBD_Stop(handles[which_one]);
			break;
		}
	}

}



/*orignial sdk*/




