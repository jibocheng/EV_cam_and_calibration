#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "MvStereoAppDefine.h"
#include "MvStereoAppDefineEx.h"
#include "MvStereoAppPixelType.h"
#include "MvStereoAppCtrl.h"
#include "MvStereoAppCtrlEx.h"
#include "ManagerHikvision.h"
#include "definition.h"


int main()
{
	ManagerHikvision3D m_hk3D(RGB);
	Imagetype it[1] = { RGB };
	m_hk3D.setupRGB(it, 0);
	m_hk3D.captureRGB(0, RGB);
}