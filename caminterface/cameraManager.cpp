#include"cameraManager.h"
#include <opencv2/opencv.hpp>
#include <Windows.h>

ManagerRealSense::ManagerRealSense()
{
	pipelines.reserve(MAX_FRAMES_INLIST);
	cfgs.reserve(MAX_FRAMES_INLIST);
	framesets.reserve(MAX_FRAMES_INLIST);

	if (detection())
	{
		std::cout << "successfully setting all the cameras" << std::endl;
	}
	else
	{
		std::cout << "falied in setting cameras, check your connection please! " << std::endl;
	}

}

ManagerRealSense::~ManagerRealSense()
{

}

bool ManagerRealSense::detection()
{
	devices = ctx.query_devices();
	if (devices.size() == 0)
	{
		std::cout << "No device detected!" << std::endl;
		return false;
	}
	for (int i = 0; i < devices.size(); i++)
	{
		camera_names.push_back(devices[i].get_info(RS2_CAMERA_INFO_NAME));
		serial_numbers.push_back(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
		std::cout << "The " << i + 1 << "st device name : " << devices[i].get_info(RS2_CAMERA_INFO_NAME) << std::endl;
		std::cout << "The " << i + 1 << "st device serial_name : " << devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) << std::endl;
		std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
	}
	return true;
}


// signal camera selection:

bool ManagerRealSense::setup(Imagetype it, int which_one) {
	if (which_one >= devices.size()) {
		std::cout << "Camera index out of range" << std::endl;
		return false;
	}

	cfgs.clear();
	pipelines.clear();
	framesets.clear();


	cfgs.resize(devices.size());
	pipelines.resize(devices.size());
	framesets.resize(devices.size());


	rs2::config cfg_temp;

	cfg_temp.enable_device(devices[which_one].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	if (it == RGB) {
		cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
	}
	else if (it == DEPTH) {
		cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
	}
	else if (it == RGBD) {
		cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
		cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
	}
	else if (it == POINTCLOUD) {
		// 例如：cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
		// cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
		// cfg_temp.enable_stream(RS2_STREAM_INFRARED, width, length, RS2_FORMAT_Y8);
	}

	cfgs[which_one] = cfg_temp;


	rs2::pipeline pipeline_temp;
	pipelines[which_one] = pipeline_temp;

	try {
		pipelines[which_one].start(cfgs[which_one]);
		std::cout << "Camera " << which_one << " setup successful." << std::endl;
	}
	catch (const rs2::error& e) {
		std::cerr << "RealSense error calling rs2::pipeline::start(): " << e.what() << std::endl;
		return false;
	}
	catch (const std::exception& e) {
		std::cerr << "Standard exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}


void ManagerRealSense::capture(int which_one, Imagetype it)
{
	pipelines[which_one].start(cfgs[which_one]);
	//pipelines[which_one].start();
	rs2::frameset framelist;
	rs2::frame frame;
	rs2::frame frameRGB;
	rs2::frame frameD;
	rs2::colorizer color_map;
	
	while (!stopCaptureThread)
	{
		if (pipelines.at(which_one).poll_for_frames(&framelist))
		{
			switch (it)
			{
			case RGB:
				frame = framelist.get_color_frame();
				mutex_CD.lock();
				framesets[which_one].push_back(frame);
				mutex_CD.unlock();
				break;
			case DEPTH:
				frame = framelist.get_depth_frame().apply_filter(color_map);
				mutex_CD.lock();
				framesets[which_one].push_back(frame);
				mutex_CD.unlock();
				break;
			case POINTCLOUD:
				break;
			case RGBD:
				frameRGB = framelist.get_color_frame();
				frameD = framelist.get_depth_frame().apply_filter(color_map);
				mutex_CD.lock();
				framesets_RGB[which_one].push_back(frameRGB); 
				framesets_D[which_one].push_back(frameD);
				mutex_CD.unlock();
				break;
			default:
				break;
			}
			//std::lock_guard<std::mutex> lock(mutex_CD);
		}

	}
	
}

	

void ManagerRealSense::demostrate(int which_one, Imagetype it)
{

	int count = 1;
	while (!stopCaptureThread)
	{

		if (it == RGB || it == DEPTH)
		{
			rs2::frame frameOut;
			cv::Mat image;
			if (framesets[which_one].size() > 0)
			{
				//std::lock_guard<std::mutex> lock(mutex_CD);
				mutex_CD.lock();
				frameOut = framesets[which_one].front();
				framesets[which_one].pop_front();
				mutex_CD.unlock();
				int w = frameOut.as<rs2::video_frame>().get_width();
				int h = frameOut.as<rs2::video_frame>().get_height();
				cv::Mat imageOut(cv::Size(w, h), CV_8UC3, (void*)frameOut.get_data(), cv::Mat::AUTO_STEP);
				imageOut.copyTo(image);
				QImage Img;
				//cv::imshow("Display" + std::to_string(which_one + 1), imageOut);
				if (imageOut.channels() == 3) {
					cv::cvtColor(imageOut, imageOut, cv::COLOR_BGR2RGB);
					Img = QImage((const unsigned char*)imageOut.data, imageOut.cols, imageOut.rows, imageOut.step, QImage::Format_RGB888);
				}
				else if (imageOut.channels() == 1) {
					Img = QImage((const unsigned char*)imageOut.data, imageOut.cols, imageOut.rows, imageOut.step, QImage::Format_Grayscale8);
				}
				emit image_go(Img);
			}
			else
			{
				std::cout << "no image now " << std::endl;
			}
		}
		else if (it == RGBD)
		{
			rs2::frame frameOut_RGB;
			cv::Mat image_RGB;
			rs2::frame frameOut_D;
			cv::Mat image_D;
			framesets_RGB.resize(MAX_FRAMES_INLIST);
			framesets_D.resize(MAX_FRAMES_INLIST);
			if (framesets_RGB[which_one].size()>0)
			{
				if (framesets_D[which_one].size() > 0)
				{//std::lock_guard<std::mutex> lock(mutex_CD);
					mutex_CD.lock();
					frameOut_RGB = framesets_RGB[which_one].front();
					framesets_RGB[which_one].pop_front();
					frameOut_D = framesets_D[which_one].front();
					framesets_D[which_one].pop_front();
					mutex_CD.unlock();
					int w_RGB = frameOut_RGB.as<rs2::video_frame>().get_width();
					int h_RGB = frameOut_RGB.as<rs2::video_frame>().get_height();
					cv::Mat imageOut_RGB(cv::Size(w_RGB, h_RGB), CV_8UC3, (void*)frameOut_RGB.get_data(), cv::Mat::AUTO_STEP);
					imageOut_RGB.copyTo(image_RGB);
					int w_D = frameOut_D.as<rs2::video_frame>().get_width();
					int h_D = frameOut_D.as<rs2::video_frame>().get_height();
					cv::Mat imageOut_D(cv::Size(w_D, h_D), CV_8UC3, (void*)frameOut_D.get_data(), cv::Mat::AUTO_STEP);
					imageOut_D.copyTo(image_D);
					QImage Img_RGB;
					QImage Img_D;
					//cv::imshow("Display" + std::to_string(which_one + 1), imageOut);
					if (imageOut_RGB.channels() == 3) {
						cv::cvtColor(imageOut_RGB, imageOut_RGB, cv::COLOR_BGR2RGB);
						Img_RGB = QImage((const unsigned char*)imageOut_RGB.data, imageOut_RGB.cols, imageOut_RGB.rows, imageOut_RGB.step, QImage::Format_RGB888);
					}
					else if (imageOut_RGB.channels() == 1) {
						Img_RGB = QImage((const unsigned char*)imageOut_RGB.data, imageOut_RGB.cols, imageOut_RGB.rows, imageOut_RGB.step, QImage::Format_Grayscale8);
					}
					if (imageOut_D.channels() == 3) {
						cv::cvtColor(imageOut_D, imageOut_D, cv::COLOR_BGR2RGB);
						Img_D = QImage((const unsigned char*)imageOut_D.data, imageOut_D.cols, imageOut_D.rows, imageOut_D.step, QImage::Format_RGB888);
					}
					else if (imageOut_D.channels() == 1) {
						Img_D = QImage((const unsigned char*)imageOut_D.data, imageOut_D.cols, imageOut_D.rows, imageOut_D.step, QImage::Format_Grayscale8);
					}

					emit imgrgbd_go(Img_RGB, Img_D);
				}
			}
		}
	}

}

void ManagerRealSense::run(int which_one, Imagetype it)
{
	stopCaptureThread = false;
	if (which_one < framesets.size())
	{
		framesets[which_one].clear();
	}
	else
	{
		framesets.resize(which_one + 1);
		pipelines.resize(which_one + 1);
		cfgs.resize(which_one + 1);
	}

	rs2::config cfg_temp;
	cfg_temp.enable_device(devices[which_one].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	switch (it)
	{
	case RGB:
		cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
		break;
	case DEPTH:
		cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
		break;
	case RGBD:
		cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
		cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
		break;
	case POINTCLOUD:
		break;
	default:
		break;
	}
	cfgs[which_one] = cfg_temp;
	pipelines[which_one] = rs2::pipeline();


	captureThread = std::thread(&ManagerRealSense::capture, this, which_one, it);
	demostrateThread = std::thread(&ManagerRealSense::demostrate, this, which_one, it);
	captureThread.detach();
	demostrateThread.detach();

}

void ManagerRealSense::stop(int which_one)
{
	stopCaptureThread = true;

	if (captureThread.joinable()) 
	{
		captureThread.join();
	}
	if (demostrateThread.joinable()) 
	{
		demostrateThread.join();
	}
	
	if (which_one < pipelines.size()) {
		pipelines[which_one].stop();
	}
	if (which_one < cfgs.size()) {
		cfgs.erase(cfgs.begin() + which_one);
	}
	if (which_one < framesets.size()) {
		framesets[which_one].clear();
		framesets.erase(framesets.begin() + which_one);
	}
	pipelines.erase(pipelines.begin() + which_one);
	
}




// muti camera selection:
void ManagerRealSense::setupAll(Imagetype it[], int num)
{
	cfgs.clear();
	pipelines.clear();
	framesets.clear();
	
	cfgs.resize(num);
	pipelines.resize(num);
	framesets.resize(num);
	framesets_RGB.resize(num);
	framesets_D.resize(num);
	for (int i = 0; i < num; i++)
	{


		if (it[i] == RGB)
		{
			rs2::config cfg_temp;
			rs2::pipeline pipeline_temp;
			std::list<rs2::frame> list_temp;
			cfg_temp.enable_device(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
			cfgs[i] = cfg_temp;
			pipelines[i] = pipeline_temp;
			framesets[i] = list_temp;
		}
		else if (it[i] == DEPTH)
		{
			rs2::config cfg_temp;
			rs2::pipeline pipeline_temp;
			std::list<rs2::frame> list_temp;
			cfg_temp.enable_device(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
			cfgs[i] = cfg_temp;
			pipelines[i] = pipeline_temp;
			framesets[i] = list_temp;
		}
		else if (it[i] == POINTCLOUD)
		{

			std::cout << "Not open yet!" << std::endl;
		}
		else if (it[i] == RGBD)
		{
			rs2::config cfg_temp;
			rs2::pipeline pipeline_temp;
			std::list<rs2::frame> list_RGB_temp;
			std::list<rs2::frame> list_D_temp;
			cfg_temp.enable_device(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
			cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
			cfgs[i] = cfg_temp;
			pipelines[i] = pipeline_temp;
			framesets_RGB[i] = list_RGB_temp;
			framesets_D[i] = list_D_temp;
		}
		else
		{
			std::cout << "Not open yet!" << std::endl;
		}

	}

}

void ManagerRealSense::captureAll(Imagetype it[], int num)
{
	int i = 0;
	rs2::colorizer colormap;

	for (auto iter = pipelines.begin(); iter != pipelines.end(); iter++)
	{
		iter->start(cfgs[i++]);
	}


	while (!stopCaptureThread)
	{
		rs2::frameset framelist;
		rs2::frame frame;
		rs2::frame frame_RGB;
		rs2::frame frame_D;
		for (int i = 0; i < pipelines.size(); i++)
		{
			if (pipelines[i].poll_for_frames(&framelist))
			{
				if (it[i] == RGB)
				{
					frame = framelist.get_color_frame();
					mutex_all.lock();
					framesets[i].push_back(frame);
					mutex_all.unlock();
				}
				else if (it[i] == DEPTH)
				{
					frame = framelist.get_depth_frame().apply_filter(colormap);
					mutex_all.lock();
					framesets[i].push_back(frame);
					mutex_all.unlock();
				}
				else if (it[i] == POINTCLOUD)
				{
					std::cout << "Not open yet " << std::endl;
				}
				else if (it[i] == RGBD)
				{
					frame_RGB = framelist.get_color_frame();
					frame_D = framelist.get_depth_frame().apply_filter(colormap);
					mutex_all.lock();
					framesets_RGB[i].push_back(frame_RGB);
					framesets_D[i].push_back(frame_D);
					mutex_all.unlock();
				}
			}
		}

	}

}


void ManagerRealSense::demostrateAll(Imagetype it[], int num)
{
	int count = 1;
	//int num = 1;
	framesets_RGB.resize(MAX_FRAMES_INLIST);
	framesets_D.resize(MAX_FRAMES_INLIST);
	while (!stopCaptureThread)
	{

		for (int i = 0; i < pipelines.size(); i++)
		{
			if (it[i] == RGB || it[i] == DEPTH)
			{
				rs2::frame frameOut;
				cv::Mat image;
				if (framesets[i].size() > 0)
				{
					mutex_all.lock();
					frameOut = framesets[i].front();
					framesets[i].pop_front();
					mutex_all.unlock();
					int w = frameOut.as<rs2::video_frame>().get_width();
					int h = frameOut.as<rs2::video_frame>().get_height();
					cv::Mat imageOut(cv::Size(w, h), CV_8UC3, (void*)frameOut.get_data(), cv::Mat::AUTO_STEP);
					imageOut.copyTo(image);
					QImage Img;
					if (imageOut.channels() == 3) {
						cv::cvtColor(imageOut, imageOut, cv::COLOR_BGR2RGB);
						Img = QImage((const unsigned char*)imageOut.data, imageOut.cols, imageOut.rows, imageOut.step, QImage::Format_RGB888);
					}
					else if (imageOut.channels() == 1) {
						Img = QImage((const unsigned char*)imageOut.data, imageOut.cols, imageOut.rows, imageOut.step, QImage::Format_Grayscale8);
					}
					int index = i;
					emit imgIdx_go(Img, i);
				}
			}
			else if (it[i] == POINTCLOUD)
			{
				std::cout << "Not open yet " << std::endl;
			}
			else if (it[i] == RGBD)
			{
				rs2::frame frameOut_RGB;
				cv::Mat image_RGB;
				rs2::frame frameOut_D;
				cv::Mat image_D;
				if (framesets_RGB[i].size() > 0)
				{
					if (framesets_D[i].size() > 0)
					{//std::lock_guard<std::mutex> lock(mutex_CD);
						mutex_CD.lock();
						frameOut_RGB = framesets_RGB[i].front();
						framesets_RGB[i].pop_front();
						frameOut_D = framesets_D[i].front();
						framesets_D[i].pop_front();
						mutex_CD.unlock();
						int w_RGB = frameOut_RGB.as<rs2::video_frame>().get_width();
						int h_RGB = frameOut_RGB.as<rs2::video_frame>().get_height();
						cv::Mat imageOut_RGB(cv::Size(w_RGB, h_RGB), CV_8UC3, (void*)frameOut_RGB.get_data(), cv::Mat::AUTO_STEP);
						imageOut_RGB.copyTo(image_RGB);
						int w_D = frameOut_D.as<rs2::video_frame>().get_width();
						int h_D = frameOut_D.as<rs2::video_frame>().get_height();
						cv::Mat imageOut_D(cv::Size(w_D, h_D), CV_8UC3, (void*)frameOut_D.get_data(), cv::Mat::AUTO_STEP);
						imageOut_D.copyTo(image_D);
						QImage Img_RGB;
						QImage Img_D;
						//cv::imshow("Display" + std::to_string(which_one + 1), imageOut);
						if (imageOut_RGB.channels() == 3) {
							cv::cvtColor(imageOut_RGB, imageOut_RGB, cv::COLOR_BGR2RGB);
							Img_RGB = QImage((const unsigned char*)imageOut_RGB.data, imageOut_RGB.cols, imageOut_RGB.rows, imageOut_RGB.step, QImage::Format_RGB888);
						}
						else if (imageOut_RGB.channels() == 1) {
							Img_RGB = QImage((const unsigned char*)imageOut_RGB.data, imageOut_RGB.cols, imageOut_RGB.rows, imageOut_RGB.step, QImage::Format_Grayscale8);
						}
						if (imageOut_D.channels() == 3) {
							cv::cvtColor(imageOut_D, imageOut_D, cv::COLOR_BGR2RGB);
							Img_D = QImage((const unsigned char*)imageOut_D.data, imageOut_D.cols, imageOut_D.rows, imageOut_D.step, QImage::Format_RGB888);
						}
						else if (imageOut_D.channels() == 1) {
							Img_D = QImage((const unsigned char*)imageOut_D.data, imageOut_D.cols, imageOut_D.rows, imageOut_D.step, QImage::Format_Grayscale8);
						}
						int index = i;
						emit imgrgbdIdx_go(Img_RGB, Img_D, index);
					}
				}
			}
		}
	}
}





void ManagerRealSense::run_all(Imagetype it[], int num)
{
	// see if the i is equal to the number of the device
	stopCaptureThread = false;
	cthread = std::thread(&ManagerRealSense::captureAll, this, it, num);
	dthread = std::thread(&ManagerRealSense::demostrateAll, this,it,num);
	cthread.detach();
	dthread.detach();
}

void ManagerRealSense::stop_all(Imagetype it[], int num)
{
	stopCaptureThread = true;

	if (cthread.joinable())
	{
		cthread.join();
	}
	if (dthread.joinable())
	{
		dthread.join();
	}

	for (int i = 0; i < getCamNum(); i++)
	{
		pipelines[i].stop();
	}

	for (int i = 0; i < getCamNum(); i++)
	{
		if (it[i] == RGB || it[i] == DEPTH)
		{
			framesets[i].clear();
		}
		else if (it[i] == RGBD)
		{
			framesets_RGB[i].clear();
			framesets_D[i].clear();
		}
	}

	cfgs.clear();
	pipelines.clear();
	framesets.clear();
	framesets_RGB.clear();
	framesets_D.clear();
}


