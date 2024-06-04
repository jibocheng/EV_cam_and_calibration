#include "cameraManagerHikvision.h"


ManagerHikvision::ManagerHikvision()
{
	if (!detection())
	{
		return;
	}
}

ManagerHikvision::~ManagerHikvision()
{

}

bool ManagerHikvision::detection()
{
	if (MV_CC_Initialize() != MV_OK)
	{
		return 0;
	}

	MV_CC_DEVICE_INFO_LIST* pstDevList = new MV_CC_DEVICE_INFO_LIST();
	if (MV_CC_EnumDevices(MV_GIGE_DEVICE || MV_USB_DEVICE, pstDevList) != MV_OK)
	{
		return 0;
	}
	camNum = pstDevList->nDeviceNum;
	MV_CC_DEVICE_INFO** pDeviceInfo = pstDevList->pDeviceInfo;
	handles.resize(camNum);

	for (int i = 0; i < camNum; i++)
	{
		if (pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
		{
			unsigned char* Modelname = pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName;
			unsigned int currentIp = pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp;
			unsigned char* Serialnum = pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;
			
			size_t lengthm = strlen(reinterpret_cast<char*>(Modelname));
			QString modelName = QString::fromUtf8(reinterpret_cast<char*>(Modelname), lengthm);
			
			size_t lengths = strlen(reinterpret_cast<char*>(Serialnum));
			QString serialNum = QString::fromUtf8(reinterpret_cast<char*>(Serialnum), lengths);
			
			QString Ip = QString::number(currentIp);

			camera_names.push_back(modelName);
			serial_numbers.push_back(serialNum);
			ip_adress.push_back(Ip);
		}


		if (MV_CC_CreateHandle(&handles[i], pDeviceInfo[i]) != MV_OK)
			return 0;
	}

	delete pstDevList;
	return 1;

}

bool ManagerHikvision::setup(Imagetype it, int num)
{
	if (MV_CC_OpenDevice(handles[num]) != MV_OK)
		return 0;


	return 1;
}

void ManagerHikvision::capture(int which_one, Imagetype it)
{
	unsigned int bufSize = 0;
	MVCC_INTVALUE streamInfo;
	if (MV_CC_GetIntValue(handles[which_one], "PayloadSize", &streamInfo) != MV_OK)
		return;

	bufSize = streamInfo.nCurValue;
	unsigned char* pData = new unsigned char[bufSize];
	MV_FRAME_OUT_INFO_EX ImageInfo = { 0 };
	if (MV_CC_StartGrabbing(handles[which_one]) != MV_OK)
		return;

	while (true)
	{

	}
}