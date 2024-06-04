#include <librealsense2/rs.hpp>
#include <librealsense2/h/rs_types.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include "definition.h"



#define MAX_FRAMES_INLIST 5



class RealSense
{
public:
	RealSense(Imagetype it);
	~RealSense() {};
	//run
	void run();
	void show();
	static int total_camera;
	/*get parameters function*/
	int getDetectedNum() { return detectedNum; }
	std::vector<std::string>getDeviceName() { return deviceName; }
	std::vector<std::string>getSerialNum() { return serialNum; }

private:
	//functions:
	void cameraEnable();
	rs2::device_list devices;
	rs2::context ctx;
	rs2::config cfg;
	rs2::colorizer color_map;
	int detectedNum;
	std::vector<std::string> deviceName;
	std::vector<std::string> serialNum;
	int which;
	//enable parameters
	Imagetype itype;
	rs2::pipeline pipe;
	//show up parameters:
	int width = 640;
	int length = 480;
	std::string windows_name;
	std::list<rs2::frame> frame_list;
	//lock
	std::mutex mutex1;

};


class IntelRealSense
{
public:


	IntelRealSense();
	~IntelRealSense() {};
	//camera information function:
	bool getCameraInfo();
	std::vector<std::string> getCameraName() { return cameraName; }
	std::vector<std::string> getSerialNumber() { return serialNumber; }
	int get_total_connected_camera() { return total_connected_camera; }
	//start camera
	bool run(int which_one);
	//void show(int which_one);
	//void stop(int which_one);


private:
	//camera_list info:
	rs2::device_list devices;
	int total_connected_camera;
	std::vector<std::string> cameraName;
	std::vector<std::string> serialNumber;
	int width = 640;
	int length = 480;
	//configuration parameters:
	std::vector<rs2::config> cfgs;
	rs2::colorizer color_map;
	std::vector<rs2::pipeline> piplines;
	//show up parameters
	std::vector<std::list<rs2::frame>> frame_list;
	//configuration
	void setup(int which_one);
	//thread lock
	//std::mutex mutex1;
	//std::mutex mutex2;
	std::mutex mutex_setting;
	std::mutex mutex_running;
	
};



class ManagerRealSense
{
public:
	ManagerRealSense();
	~ManagerRealSense() {};
	//achieve parameters:
	std::vector<std::string> getCamNames() { return camera_names; };
	std::vector<std::string> getSerialNum() { return serial_numbers; };
	int getCamNum() { return devices.size();}
	//get camera running:
	bool setup(Imagetype it[], int num);
	void run(int which_one, Imagetype it = RGB);
	void run_all(Imagetype it[], int i);

private:
	// setup camera parameters:
	std::vector<std::string> camera_names;
	std::vector<std::string> serial_numbers;
	//constant parameters:
	int width = 640;
	int length = 480;
	//vectors or list:
	rs2::device_list devices;
	rs2::context ctx;
	std::vector<rs2::config> cfgs;
	std::vector<rs2::pipeline> pipelines;
	std::vector<std::list<rs2::frame>> framesets;
	//threads:
	//std::vector<std::thread> captureThread;
	//std::vector<std::thread> demostrateThread;
	//mutex:
	std::mutex mutex_CD;
	std::mutex mutex_all;
	//private function:
	bool detection();
		//run camera one by one
	void capture(int which_one, Imagetype it);
	void demostrate(int which_one, Imagetype it);
		// run all the cameras
	void captureAll(Imagetype it[]);
	void demostrateAll(Imagetype it[]);

	//function work:
	//void pointcloud();
	
};




