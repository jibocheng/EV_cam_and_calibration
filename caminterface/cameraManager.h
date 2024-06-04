#ifndef CAMERAMAGAGER_H
#define CAMERAMAGAGER_H
#include <librealsense2/rs.hpp>
#include <librealsense2/h/rs_types.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include "sharedate.h"



#define MAX_FRAMES_INLIST 5

class ManagerRealSense : public QObject
{
	Q_OBJECT
public:
	ManagerRealSense();
	~ManagerRealSense();
	//achieve parameters:
	std::vector<std::string> getCamNames() { return camera_names; };
	std::vector<std::string> getSerialNum() { return serial_numbers; };
	int getCamNum() { return devices.size(); }
	//get camera running:
	bool setup(Imagetype it, int num);
	void run(int which_one, Imagetype it = RGB);
	void stop(int which_one);
	
	//for all camera
	void setupAll(Imagetype it[], int num);
	void captureAll(Imagetype it[], int num);
	void demostrateAll(Imagetype it[], int num);
	void run_all(Imagetype it[], int num);
	void stop_all(Imagetype it[], int num);

signals:
	void camName_go(const QString camName);
	void serial_go(const QString serialNum);
	void image_go(const QImage& img2dis);
	void imgIdx_go(const QImage& img2dis, const int i);
	void imgrgbd_go(const QImage& RGB, const QImage& D);
	void imgrgbdIdx_go(const QImage& RGB, const QImage& D, const int i);

private:
	// setup camera parameters:
	std::vector<std::string> camera_names;
	std::vector<std::string> serial_numbers;
	//constant parameters:
	int width = 640;
	int length = 480;
	//stream set up:

	//vectors or list:
	rs2::device_list devices;
	rs2::context ctx;
	std::vector<rs2::config> cfgs;
	std::vector<rs2::pipeline> pipelines;
	std::vector<std::list<rs2::frame>> framesets_RGB;
	std::vector<std::list<rs2::frame>> framesets_D;
	std::vector<std::list<rs2::frame>> framesets;
	//threads:
	std::thread captureThread;
	std::thread demostrateThread;
	std::atomic<bool> stopCaptureThread;
	std::thread cthread;
	std::thread dthread;
	//mutex:
	std::mutex mutex_CD;
	std::mutex mutex_all;
	//private function:
	bool detection();
	//run camera one by one
	void capture(int which_one, Imagetype it);
	void demostrate(int which_one, Imagetype it);
};



#endif

