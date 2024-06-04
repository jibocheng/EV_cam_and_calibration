#include "Picdetection.h"
#include <iostream>
#include <windows.h>


void Picdetection::receive_files(QString folder_path)
{
	WIN32_FIND_DATA findFileData;
	QString searchPath = folder_path + "\\*";
	std::wstring searchPathStr = searchPath.toStdWString();
	HANDLE hFind = FindFirstFile(searchPathStr.c_str(), &findFileData);

}


