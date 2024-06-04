#pragma once
#include <string>

static const int frames_length = 7;
typedef enum
{
	RGB,
	DEPTH,
	POINTCLOUD,
	RGBD

}Imagetype;


struct camera_info
{
	std::string cameraName;
	std::string serialNum;
	Imagetype it;
	int selectedNum;
};

