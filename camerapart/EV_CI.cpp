#include"EV_CI.h"
#include <opencv2/opencv.hpp>




/*realsense camera*/
int RealSense::total_camera = 0;

RealSense::RealSense(Imagetype it) :itype(it)
{
	which = total_camera;
	total_camera++;
	devices = ctx.query_devices();
	detectedNum = devices.size();
	for (int i = 0; i < detectedNum; i++)
	{
		deviceName.push_back(devices[i].get_info(RS2_CAMERA_INFO_NAME));
		serialNum.push_back(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	}
}


void RealSense::cameraEnable()
{

	switch (itype)
	{
	case RGB:
		cfg.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
		cfg.enable_device(devices[which].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
		windows_name = "RGB" + std::to_string(which + 1);
		break;
	case DEPTH:
		cfg.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
		cfg.enable_device(devices[which].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
		windows_name = "DEPTH" + std::to_string(which + 1);
	default:
		break;
	}

}


void RealSense::run()
{
	
	rs2::frame frame;
	cameraEnable();
	pipe.start(cfg);
	while (true)
	{
		//std::cout << "run thread is runing " << std::endl;
		rs2::frameset fs = pipe.wait_for_frames();
		mutex1.lock();
		switch (itype)
		{
			case RGB:
				frame = fs.get_color_frame();
				frame_list.push_back(frame);
				break;
			case DEPTH:
				frame = fs.get_depth_frame().apply_filter(color_map);
				frame_list.push_back(frame);
				break;
			default:
				break;
		}


		mutex1.unlock();
		
	}



}

void RealSense::show()
{
	
	while (true)
	{
		mutex1.lock();
		if (!frame_list.empty())
		{
			
			rs2::frame frame = frame_list.front();
			frame_list.pop_front();
			const int w = frame.as<rs2::video_frame>().get_width();
			const int h = frame.as<rs2::video_frame>().get_height();

			cv::Mat image(cv::Size(w, h), CV_8UC3, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
			cv::imshow(windows_name, image);
			mutex1.unlock();
			//keyboard input
			int interrupt = cv::waitKey(1);
			if (interrupt == 113 || interrupt == 27)
			{
				break;
			}

		}
		else
		{
			std::cout << "No image now....." << std::endl;
			mutex1.unlock();
		}
	}
	cv::destroyAllWindows();
}

/*IntelRealSense:*/

IntelRealSense::IntelRealSense()
{
	piplines.reserve(MAX_FRAMES_INLIST);
	cfgs.reserve(MAX_FRAMES_INLIST);
	frame_list.reserve(MAX_FRAMES_INLIST);
}

bool IntelRealSense::getCameraInfo()
{
	rs2::context ctx;
	devices = ctx.query_devices();
	total_connected_camera = devices.size();
	if (total_connected_camera > MAX_FRAMES_INLIST)
	{
		std::cout << "camera number are too much to run " << std::endl;
		return 0;
	}
	else if (total_connected_camera == 0)
	{
		std::cout << "No device detected ! check your connection please" << std::endl;
		return 0;
	}
	piplines.reserve(total_connected_camera);
	frame_list.reserve(total_connected_camera);
	for (int i = 0; i < total_connected_camera; i++)
	{
		cameraName.push_back(devices[i].get_info(RS2_CAMERA_INFO_NAME));
		serialNumber.push_back(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	}
	

	return 1;
}

void IntelRealSense::setup(int which_one)
{
	std::string device2control = devices[which_one].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
	rs2::config cfg_temp;
	cfg_temp.enable_device(device2control);
	cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
	cfgs.push_back(cfg_temp);
	
}


bool IntelRealSense::run(int which_one)
{
	if (which_one > devices.size())
	{
		std::cout << "This device not found !" << std::endl;
		return 0;
	}
	mutex_setting.lock();
	setup(which_one);
	rs2::pipeline pipeline_temp;
	pipeline_temp.start(cfgs.at(which_one));
	piplines.push_back(pipeline_temp);
	std::list<rs2::frame> frames_tube;
	frame_list.push_back(frames_tube);
	mutex_setting.unlock();
	int count = 1;
	//int i = 0;
	while (true)
	{
		
		rs2::frameset framesets = piplines.at(which_one).wait_for_frames();
		rs2::frame frames = framesets.get_color_frame();
		// put frame into correspond list;
		mutex_running.lock();
		frame_list[which_one].push_back(frames);
		std::cout << "the size of this one seems to be " << frame_list.at(which_one).size() << std::endl;
		if (frame_list[which_one].size() > MAX_FRAMES_INLIST)
		{
			std::cout << "get one frame out " << std::endl;
			frame_list[which_one].pop_front();
		}
		rs2::frame frame_out = frame_list[which_one].front();
		frame_list[which_one].pop_front();
		int w = frame_out.as<rs2::video_frame>().get_width();
		int h = frame_out.as<rs2::video_frame>().get_height();
		cv::Mat image(cv::Size(w, h), CV_8UC3, (void*)frame_out.get_data(), cv::Mat::AUTO_STEP);
		cv::imshow("Display" + std::to_string(which_one), image);
		mutex_running.unlock();
		int key = cv::waitKey(1);
		if (key == 27 || key == 113)
		{
			break;
		}
		else if (key == 115)
		{
			std::string name = "SavedImage" + std::to_string(count++) + ".png";
			std::cout << name << std::endl;
			cv::imwrite(name, image);
		}
	
		
		//i++;
		//std::cout << "Already run " << i << " times" << std::endl;
	}
}

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

bool ManagerRealSense::detection()
{
	devices = ctx.query_devices();
	if (devices.size() == 0)
	{
		std::cout << "No device detected!" << std::endl;
		return 0;
	}
	for (int i = 0; i < devices.size(); i++)
	{
		camera_names.push_back(devices[i].get_info(RS2_CAMERA_INFO_NAME));
		serial_numbers.push_back(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
		std::cout << "The " << i + 1 << "st device name : " << devices[i].get_info(RS2_CAMERA_INFO_NAME) << std::endl;
		std::cout << "The " << i + 1 << "st device serial_name : " << devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) << std::endl;
		std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
	}
	return 1;
}


bool ManagerRealSense::setup(Imagetype it[], int num)
{
	if (num > devices.size())
	{
		std::cout << "Not that much camera " << std::endl;
		return 0;
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			rs2::config cfg_temp;
			rs2::pipeline pipeline_temp;
			std::list<rs2::frame> list_temp;
			cfg_temp.enable_device(devices[i].get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			switch (it[i])
			{
			case RGB:
				cfg_temp.enable_stream(RS2_STREAM_COLOR, width, length, RS2_FORMAT_BGR8);
				break;
			case DEPTH:
				cfg_temp.enable_stream(RS2_STREAM_DEPTH, width, length, RS2_FORMAT_Z16);
			case POINTCLOUD:
				break;
			default:
				break;
			}
			cfgs.push_back(cfg_temp);
			pipelines.push_back(pipeline_temp);
			framesets.push_back(list_temp);
		}
		return 1;
	}
}

void ManagerRealSense::capture(int which_one, Imagetype it)
{
	pipelines.at(which_one).start(cfgs.at(which_one));
	int count = 1;
	rs2::frameset framelist;
	rs2::frame frame;
	rs2::colorizer color_map;
	//std::string windowsname = std::to_string(which_one) + "'s window";

	

	while (true)
	{
		std::string savedname = std::to_string(which_one) + "'s Image" + std::to_string(count++) + ".PNG";
		rs2::pipeline pTemp = pipelines.at(which_one);
		if (pTemp.poll_for_frames(&framelist))
		{
			switch (it)
			{
			case RGB:
				frame = framelist.get_color_frame();
				break;
			case DEPTH:
				frame = framelist.get_depth_frame().apply_filter(color_map);
				break;
			case POINTCLOUD:
				break;
			default:
				break;
			}
			mutex_CD.lock();
			framesets[which_one].push_back(frame);
			mutex_CD.unlock();
			//int w = frame.as<rs2::video_frame>().get_width();
			//int h = frame.as<rs2::video_frame>().get_height();
			//cv::Mat imageOut(cv::Size(w, h), CV_8UC3, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
			//cv::imshow(windowsname, imageOut);
			//if (key == 113 || key == 27)
			//{
			//	std::cout << "quit in detecting" << std::endl;
			//	cv::destroyAllWindows();
			//	pipelines[which_one].stop();
			//}
			//else if (key == 115)
			//{
			//	cv::imwrite(savedname, imageOut);
			//	std::cout << "image" + std::to_string(count) << "has been successful saved! " << std::endl;
			//}

		}

	}
}

void ManagerRealSense::demostrate(int which_one, Imagetype it)
{
	
	int count = 1;
	while (true)
	{
		int key = cv::waitKey(1);
		std::string savedname = "Image" + std::to_string(count++) + ".PNG";
		rs2::frame frameOut;
		cv::Mat image;
		
		if (framesets[which_one].size() > 0)
		{
			mutex_CD.lock();
			frameOut = framesets[which_one].front();
			framesets[which_one].pop_front();
			mutex_CD.unlock();
			int w = frameOut.as<rs2::video_frame>().get_width();
			int h = frameOut.as<rs2::video_frame>().get_height();
			cv::Mat imageOut(cv::Size(w, h), CV_8UC3, (void*)frameOut.get_data(), cv::Mat::AUTO_STEP);
			imageOut.copyTo(image);
			cv::imshow("Display" + std::to_string(which_one+1), imageOut);

		}
		else if (key == 113 || key == 27)
		{
			std::cout << "quit in detecting" << std::endl;
			cv::destroyAllWindows();
			//pipelines[which_one].stop();
			break;
		}
		else if (key == 115)
		{
			cv::imwrite(savedname, image);
			std::cout << "image" + std::to_string(count) << "has been successful saved! " << std::endl;
		}
		
	}
	
}

void ManagerRealSense::run(int which_one, Imagetype it)
{
	std::thread captureThread(&ManagerRealSense::capture, this, which_one, it);
	std::thread demostrateThread(&ManagerRealSense::demostrate, this, which_one, it);
	captureThread.join();
	demostrateThread.join();
}



void ManagerRealSense::captureAll(Imagetype it[])
{
	int num = devices.size();
	// prepare to collection frame
	int i = 0;
	rs2::colorizer colormap;
	
	// start to stream configuration
	for (auto iter = pipelines.begin(); iter != pipelines.end(); iter++)
	{
		iter->start(cfgs[i++]);
	}


	while (true)
	{
		rs2::frameset framelist;
		rs2::frame frame;
		for (int i = 0; i < pipelines.size(); i++)
		{
			if (pipelines[i].poll_for_frames(&framelist))
			{
				switch (it[i])
				{
				case RGB:
					frame = framelist.get_color_frame();
					break;
				case DEPTH:
					frame = framelist.get_depth_frame().apply_filter(colormap);
					break;
				default:
					break;
				}
				mutex_all.lock();
				framesets[i].push_back(frame);
				mutex_all.unlock();
			}
		}

	}

}


void ManagerRealSense::demostrateAll(Imagetype it[])
{
	int count = 1;
	int num = 1;
	while (true)
	{
		int key = cv::waitKey(1);
		
		rs2::frame frameOut;
		cv::Mat image;
		std::string savedname = "Image from devices " + std::to_string(num++) + std::to_string(count) + ".PNG";
		for (int i = 0; i < pipelines.size(); i++)
		{
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
				cv::imshow("Display" + std::to_string(i + 1), imageOut);

			}
		}
		if (key == 113 || key == 27)
		{
			std::cout << "quit in detecting" << std::endl;
			cv::destroyAllWindows();
			//pipelines[which_one].stop();
			break;
		}
		else if (key == 115)
		{
			cv::imwrite(savedname, image);
			std::cout << "image" + std::to_string(count++) << "from devices" << num << "has been successful saved! " << std::endl;
		}
	}
	
}

void ManagerRealSense::run_all(Imagetype it[], int i)
{
	// see if the i is equal to the number of the device
	int num = devices.size();
	if (num != i)
	{
		std::cout << "Wrong devices numbers, please check connection information or condition!" << std::endl;
		return;
	}
	
	std::thread cthread(&ManagerRealSense::captureAll,this, it);
	std::thread dthread(&ManagerRealSense::demostrateAll,this,it);
	cthread.join();
	dthread.join();
}



//function realize:
//void ManagerRealSense::pointcloud()
//{
//	return;
//}
