/*
* Copyright (c) 2022 NUNOVA All Rights Reserved. 
*
* NOTICE: All information contained herein is, and remains the property of 
* NUNOVA, if any. The intellectual and technical concepts 
* contained herein are proprietary to NUNOVA and may be 
* covered by U.S. and Foreign Patents, patents in process, and are protecte
* by trade secret or copyright law. Dissemination of this information or 
* reproduction of this material is strictly forbidden unless prior written 
* permission is obtained from NUNOVA 
*/
//静态场管理
#ifndef __NUNOVA_STATIC_FIELD_H__
#define __NUNOVA_STATIC_FIELD_H__

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


#define SQUARE_LOCATION_OFFSET 50//定位时矩形框偏移值
#define SQUARE_LOCATION_EFFECTIVE_OFFSET 15 //定位时有效范围偏移值
#define RE_LOCATION_CNT 600 //重定位次数（必须连续）
#define LOCATION_FAIL_CNT 600 //定位失败次数（必须连续）
#define LOCATION_MAX_EFFECTIVE_CNT 3//定位时成功的最大有效次数（必须连续）
#define MAX_WARNING_CNT 1//最大报警次数
#define ACCIDENT_LOCATION_MAX_CNT 5//车祸事故定位时最大次数

int PlaneSimilarityRatio(cv::Mat &tempPlane, cv::Mat &compPlane, cv::Mat &simR, double luminFilterT = 25.0, int flag = 0);
std::string GetTimeToStringForLog();
std::string GetTimeToStringForPrint();
cv::Point MinPoint(std::vector<cv::Mat> &locTemp, cv::Mat &locComp);
int InitStaticField(stSOM &m_som, cv::Mat tempImg);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#endif /**< __STATIC_FIELD_H__ */