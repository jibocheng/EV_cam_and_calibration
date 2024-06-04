#pragma once
#include <thread>
#include <mutex>
#include <list>
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include "sharedate.h"
#include "MvCameraControl.h"

 
class ManagerHikvision:public QObject 
{
	Q_OBJECT
public:
	ManagerHikvision();
	~ManagerHikvision();
	std::vector<QString>getCamNames() { return camera_names; };
	std::vector<QString>getSerialNum() { return serial_numbers; };
	std::vector<QString>getIp() { return ip_adress; }
	unsigned int get_camNum() { return camNum; }
	//get camera running:
	bool setup(Imagetype it, int num);
	void run(int which_one, Imagetype = RGB);
	void stop(int which_one);

private:
	std::vector<QString> camera_names;
	std::vector<QString> serial_numbers;
	std::vector<QString> ip_adress;
	std::vector <void*> handles;

	unsigned int camNum;
	//constant parameters:
	int width = 640;
	int length = 480;

private:
	bool detection();
	void capture(int which_one, Imagetype it);
	void demostrate(int which_one, Imagetype it);
};

