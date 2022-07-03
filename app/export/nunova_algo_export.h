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
#ifndef __NUNOVA_ALGO_EXPORT_H__
#define __NUNOVA_ALGO_EXPORT_H__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <linux/videodev2.h>  //V4L2结构体头文件

//静态场异常管理接口
//主场景，主要用来像素匹配定位
typedef enum primary_scene_type
{
    PST_NULL = 0,
    TRASH_CAN = 1,//垃圾�?
    WELL_LID = 2,//井盖
    SOD_MANAGE = 3,//草皮管理
    CAR_ACCIDENT = 4,//车祸事故
    PST_END
} ePrimarySceneType;

//垃圾桶次级场景类�?
typedef enum
{
    TC_NULL = 0,
    CAN_SPILL = 1,//垃圾溢出
    ON_THE_CAN_GARBAGE = 2,//垃圾桶盖上有垃圾
    ON_THE_GROUND_GARBAGE = 3//垃圾桶周围有垃圾散落
} eTrashCanSecondaryT;

//井盖次级场景类型
typedef enum
{
    WL_NULL = 0,
    SHIFTING = 1,//移动
    LOST = 2 //丢失
} eWellLidSecondaryT;

//草皮次级场景类型
typedef enum
{
    SM_NULL = 0,
    BAD = 1,//被破坏或有垃�?
    YELLOW = 2//枯萎
} eSodSecondaryT;

//车辆事故场景类型
typedef enum
{
    CA_NULL = 0,

} eCarAccidentT;

#define SECONDARY_SCENE_MAX_NUM 32//次级场景个数
typedef struct static_obj_manage
{
    int stageW;//舞台�?
    int stageH;//舞台�?
    int beginHour;//每天开始运行时刻，小时为单�?-23
    int endHour;//每天结束运行时刻，小时为单位0-23
    std::time_t stageTimeStamp;
    std::time_t stageStartTime;
    ePrimarySceneType p_type;//主场景类�?
    cv::Rect p_rect;//主场景矩形区�?
    cv::Rect p_locRect;//主场景定位矩形区�?
    cv::Mat p_tempGB;//主场景高斯模糊模板图
    std::vector<cv::Mat> p_locTemp;//主场景定位模板，一定是灰度�?
    int p_locTrigeCnt;//定位触发次数
    int p_locRealCnt;//定位实际次数
    int p_locFailCnt;//定位失败次数
    cv::Point p_locP;//定位�?

    double s_luminFilterT;//次级场景亮度过滤门限
    int s_sceneNum;//次级场景数量
    int s_type[SECONDARY_SCENE_MAX_NUM];//次级场景类型
    cv::Rect s_rect[SECONDARY_SCENE_MAX_NUM];//次级场景矩形区域
    cv::Mat s_tempNormal[SECONDARY_SCENE_MAX_NUM];//正常模板
    cv::Mat s_tempException[SECONDARY_SCENE_MAX_NUM];//异常模板
    int s_state[SECONDARY_SCENE_MAX_NUM];//0:初始状态，还没进行相似计算�?：表示已经完成了第一次相似计算且正常态；2：表示进入异常态；
    int s_warnTrigger[SECONDARY_SCENE_MAX_NUM];//告警触发器；

} stSOM;

//初始化静态场
extern int InitStaticField(stSOM &m_som, cv::Mat tempImg);
//静态场逻辑执行
//返回�?
//0：正�?
//1:重定�?
//2:异常
//-1:定位不准，可能静态场已变化较大或者摄像头角度变化过大，但还有种情况是静态场被遮挡严�?
extern int StaticFieldLogicRun(cv::Mat frame, stSOM &m_som, cv::Mat &excepMat, int &subType, int isDisplay);







#endif