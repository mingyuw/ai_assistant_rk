
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

#include "nunova_algo_export.h"

using namespace std;
using namespace cv;


int PushTrashCanDataToScreen(stSOM &m_som, cv::Mat eMat, int subType)
{
	char cmd[512];
	memset(cmd, 0, 512);
	cv::imwrite("exception.jpg", eMat);
	sync();
	sprintf(cmd,"curl --connect-timeout 3 -m 3 -X POST -F 'image=@exception.jpg' -F 'display_time=5' -F 'image_width=%d' -F 'image_height=%d' -F 'level=1'  http://192.168.5.13:15223/smc_api/post_image", eMat.cols, eMat.rows);

	FILE *fp = nullptr;
	char buf[128];
	memset(buf, 0, 128);
	std::string res;
	if ((fp = popen(cmd, "r")) == nullptr)
	{
		std::cout << "error: curl image popen fail" << std::endl;
	}
	while (fgets(buf, 128, fp) != nullptr)
	{
		res += buf;
	}
	std::cout << "curl image return: " << res << std::endl;
	if( 0 != pclose(fp) )
	{
		std::cout << "error: curl image pclose fail" << std::endl;
	}

	if(subType == int(eTrashCanSecondaryT::CAN_SPILL))
	{
		sprintf(cmd, "curl --connect-timeout 3 -m 3 -X POST -F 'audio=@111.mp3' -F 'times=2' -F 'volume=15' -F 'level=1' http://192.168.5.13:15223/smc_api/post_audio");
	}
	else if(subType == int(eTrashCanSecondaryT::ON_THE_GROUND_GARBAGE))
	{
		sprintf(cmd, "curl --connect-timeout 3 -m 3 -X POST -F 'audio=@222.mp3' -F 'times=2' -F 'volume=15' -F 'level=1' http://192.168.5.13:15223/smc_api/post_audio");
	}

	if ((fp = popen(cmd, "r")) == nullptr)
	{
		std::cout << "error: curl image popen fail" << std::endl;
	}
	while (fgets(buf, 128, fp) != nullptr)
	{
		res += buf;
	}
	std::cout << "curl image return: " << res << std::endl;
	if( 0 != pclose(fp) )
	{
		std::cout << "error: curl image pclose fail" << std::endl;
	}
	return 0;
}

int main(int argc, char **argv)
{
	string rtsp_src_addr = "rtsp://admin:htzm123456@112.44.218.103:354/0/0/0";//绵阳
	//string rtsp_src_addr = "rtsp://admin:htzm123456@171.221.205.72:14154";//华体
	//string rtsp_src_addr = "rtsp://192.168.5.17";//xilinx实验室
	//string rtsp_src_addr = " rtsp://admin:htzm123456@183.252.19.171:12004";//福州
	string file_src_addr = "33333.mp4";
	string uvc_src_addr = "/dev/video0";
    char file_str[512] = {0};
	char rtsp_str[512] = {0};
	char uvc_str[512] = {0};
    //sprintf(m_file, "filesrc location=%s ! decodebin ! nvvidconv ! video/x-raw, width=1920, height=1080, format=BGRx ! videoconvert ! appsink", src_addr.c_str());
	sprintf(file_str, "filesrc location=%s ! decodebin ! nvvidconv ! video/x-raw, width=1920, height=1080, format=BGRx ! videoconvert ! appsink", file_src_addr.c_str());
	//sprintf(rtsp_str, "rtspsrc location=%s latency=100 ! decodebin ! nvvidconv ! video/x-raw, width=1280, height=720, format=BGRx ! videoconvert ! appsink", rtsp_src_addr.c_str());
	//sprintf(rtsp_str, "rtspsrc location=%s latency=100 tcp-timeout=500 ! decodebin ! nvvidconv ! video/x-raw, width=1920, height=1080, format=BGRx ! videoconvert ! appsink", rtsp_src_addr.c_str());
	sprintf(rtsp_str, "rtspsrc location=%s latency=100 ! decodebin ! nvvidconv ! video/x-raw, width=1920, height=1080, format=BGRx ! videoconvert ! appsink", rtsp_src_addr.c_str());
	sprintf(uvc_str, "v4l2src device=%s ! video/x-raw, width=1280, height=720, format=YUY2, framerate=25/1 ! videoconvert ! video/x-raw, width=1280, height=720, format=BGRx ! videoconvert ! appsink", uvc_src_addr.c_str());

	cv::VideoCapture cap;
	cap.open(rtsp_str, CAP_GSTREAMER);
	
	stSOM m_som;
	m_som.p_type = ePrimarySceneType::TRASH_CAN;
	m_som.p_rect = cv::Rect(1310, 175, 192, 224);//绵阳
	//m_som.p_rect = cv::Rect(825, 127, 179, 114);//华体
	//m_som.p_rect = cv::Rect(775, 304, 261, 274);//xilinx实验室
	//m_som.p_rect = cv::Rect(1008, 467, 188, 183);//福州


	//绵阳
	m_som.s_sceneNum = 3;
	m_som.s_type[0] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[0] = cv::Rect(12, 54, 73, 20);

	m_som.s_type[1] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[1] = cv::Rect(91, 58, 87, 19);

	m_som.s_type[2] = int(eTrashCanSecondaryT::ON_THE_GROUND_GARBAGE);
	m_som.s_rect[2] = cv::Rect(6, 178, 180, 29);
	

	//华体
	/*
	m_som.s_sceneNum = 3;
	m_som.s_type[0] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[0] = cv::Rect(6, 16, 75, 30);
	m_som.s_tempNormal[0] = (m_som.p_tempGB(m_som.s_rect[0])).clone();
	m_som.s_state[0] = 0;

	m_som.s_type[1] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[1] = cv::Rect(85, 18, 74, 33);
	m_som.s_tempNormal[1] = (m_som.p_tempGB(m_som.s_rect[1])).clone();
	m_som.s_state[1] = 0;

	m_som.s_type[2] = int(eTrashCanSecondaryT::ON_THE_CAN_GARBAGE);
	m_som.s_rect[2] = cv::Rect(40, 1, 131, 15);
	m_som.s_tempNormal[2] = (m_som.p_tempGB(m_som.s_rect[2])).clone();
	m_som.s_state[2] = 0;
	*/

	//xilinx实验室
	/*
	m_som.s_sceneNum = 4;
	m_som.s_type[0] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[0] = cv::Rect(33, 75, 75, 26);
	m_som.s_tempNormal[0] = (m_som.p_tempGB(m_som.s_rect[0])).clone();
	m_som.s_state[0] = 0;

	m_som.s_type[1] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[1] = cv::Rect(140, 79, 90, 24);
	m_som.s_tempNormal[1] = (m_som.p_tempGB(m_som.s_rect[1])).clone();
	m_som.s_state[1] = 0;

	m_som.s_type[2] = int(eTrashCanSecondaryT::ON_THE_GROUND_GARBAGE);
	m_som.s_rect[2] = cv::Rect(15, 238, 110, 33);
	m_som.s_tempNormal[2] = (m_som.p_tempGB(m_som.s_rect[2])).clone();
	m_som.s_state[2] = 0;

	m_som.s_type[3] = int(eTrashCanSecondaryT::ON_THE_GROUND_GARBAGE);
	m_som.s_rect[3] = cv::Rect(125, 238, 110, 33);
	m_som.s_tempNormal[3] = (m_som.p_tempGB(m_som.s_rect[3])).clone();
	m_som.s_state[3] = 0;
	*/

	//福州
	/*
	m_som.s_sceneNum = 2;
	m_som.s_type[0] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[0] = cv::Rect(31, 58, 58, 25);
	m_som.s_tempNormal[0] = (m_som.p_tempGB(m_som.s_rect[0])).clone();
	m_som.s_state[0] = 0;

	m_som.s_type[1] = int(eTrashCanSecondaryT::CAN_SPILL);
	m_som.s_rect[1] = cv::Rect(114, 53, 59, 24);
	m_som.s_tempNormal[1] = (m_som.p_tempGB(m_som.s_rect[1])).clone();
	m_som.s_state[1] = 0;
	*/




	cv::Mat stageTempImg = cv::imread("stageTemplate1.bmp");//绵阳
	//cv::Mat stageTempImg = cv::imread("stageTemplate2.bmp");//华体
	//cv::Mat stageTempImg = cv::imread("stageTemplate3.bmp");//华体
	//cv::Mat stageTempImg = cv::imread("stageTemplate4.bmp");//福州


	InitStaticField(m_som, stageTempImg);
	m_som.s_luminFilterT = 30.0;
	m_som.beginHour = 8;
	m_som.endHour = 19;

	cv::Mat frame;
	int runFlag = 0;
	printf("Video %s started\n", rtsp_src_addr.c_str());	
	
	while(1)
	{
		printf("Checking cap status");
		if(cap.isOpened() != 1)
		{
			printf("cap open failed");
			cap.open(rtsp_str, CAP_GSTREAMER);
			if(cap.isOpened() != 1)
			{
				sleep(10);
				continue;
			}
		}
		else
		{
			printf("cap reading frame");
			cap.read(frame);
			if(frame.empty())
			{
				printf("######video %s is end, restarting...######!\n", rtsp_src_addr.c_str());
				cap.release();
				sleep(60);
				continue;
			}

			int keyV = cv::waitKey(5);
			if(keyV == 27)
			{	
				imwrite("stageTemplate.bmp", frame);
				printf("save full pic and end!\n");
				break;
			}
			else if(keyV == 32)
			{
				runFlag = 1;
			}
			else
			{
				if(runFlag == 1)
				{
					cv::Mat eMat;
					int subType;
					int ret = StaticFieldLogicRun(frame, m_som, eMat, subType, 1);
					if(ret == 2)
					{
						PushTrashCanDataToScreen(m_som, eMat, subType);
					}
				}

			}
			
			cv::rectangle(frame, m_som.p_rect, cv::Scalar(0,0,255), 2, 8, 0);
			cv::Mat frameTmp;
			cv::resize(frame, frameTmp, cv::Size(1280, 720));
			imshow("LIVE", frameTmp);
		}
		

	}
	
	return 0;
}

