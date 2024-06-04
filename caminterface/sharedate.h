#pragma once
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPainter>


typedef enum
{
	RGB,
	DEPTH,
	POINTCLOUD,
	RGBD

}Imagetype;


struct camera_info
{
	QString cameraName;
	QString serialNum;
	Imagetype it;
	int selectedNum;
};

