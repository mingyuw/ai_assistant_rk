#include "nunova_static_field.h"
#include "nunova_mix.h"




//物质同一平面反射率几乎相同L'=aL,但还不能解决阴影的影响
//参数：模板平面和对比平面
//返回相似度
int PlaneSimilarityRatio(cv::Mat &tempPlane, cv::Mat &compPlane, cv::Mat &simR, double luminFilterT, int flag)
{
	if((tempPlane.channels() != compPlane.channels()) || (tempPlane.channels() != 3 && tempPlane.channels() != 1))
	{
		PrintERR("mat error %d - %d!\n",tempPlane.channels(),compPlane.channels());
		return -1;
	}

	if(simR.empty())
	{
		if(tempPlane.channels() == 3)
		{
			simR.create(1, 3, CV_64F);
		}
		else
		{
			simR.create(1, 1, CV_64F);
		}
		
	}

	if(simR.cols != tempPlane.channels())
	{
		PrintERR("simR channels is not same to tempPlane!\n");
		return -2;
	}

	int width = tempPlane.cols;
	int height = tempPlane.rows;
	std::vector<cv::Mat> channelsTemp;
	std::vector<cv::Mat> channelsComp;
	cv::split(tempPlane, channelsTemp);
	cv::split(compPlane, channelsComp);
	cv::Scalar tempMean = cv::mean(tempPlane);
	cv::Scalar compMean = cv::mean(compPlane);
	for(int iChannel = 0; iChannel < simR.cols; iChannel++)
	{
		//std::cout << tempMean[iChannel] << std::endl;
		//std::cout << compMean[iChannel] << std::endl;
		double alpha = compMean[iChannel] - tempMean[iChannel];//compMean[iChannel] / tempMean[iChannel];
		//std::cout << alpha << std::endl;
		cv::Mat aResult;
		cv::addWeighted(channelsTemp[iChannel], 1.0, channelsComp[iChannel], -1.0, alpha, aResult, CV_64F);
		//std::cout << "###########################################" << std::endl;
		//std::cout << aResult.rows << std::endl;
		//std::cout << aResult.cols << std::endl;
		for(int ir = 0; ir < aResult.rows; ir++)
		{
			for(int ic = 0; ic < aResult.cols; ic++)
			{
				if(fabs(aResult.at<double>(ir, ic)) <= luminFilterT)//亮度过滤门限
				{
					aResult.at<double>(ir, ic) = 0.0;
				}
			}
		}

		for(int ir = 0; ir < aResult.rows; ir++)
		{
			for(int ic = 0; ic < aResult.cols; ic++)
			{
				//当中心点不等于0
				if(aResult.at<double>(ir, ic) != 0.0)
				{
					int lonersCnt = 0;//孤独者
					for(int sr = -1; sr <= 1; sr++)
					{
						for(int sc = -1; sc <= 1; sc++)
						{
							int tmpR = ir + sr;
							int tmpC = ic + sc;
							if((tmpR >= 0 && tmpR <= (aResult.rows - 1)) && (tmpC >= 0 && tmpC <= (aResult.cols - 1)) )
							{
								if(aResult.at<double>(tmpR, tmpC) != 0.0)
								{
									lonersCnt++;
								}
							}

						}
					}

					if(lonersCnt <= 2)
					{
						aResult.at<double>(ir, ic) = 0.0;
					}
				}
				

				/*
				if(ir != 0 && ir != (aResult.rows - 1) && ic != 0 && ic != (aResult.cols - 1) && aResult.at<double>(ir, ic) != 0.0)
				{
					if(aResult.at<double>(ir-1, ic) == 0.0 && aResult.at<double>(ir+1, ic) == 0.0 && aResult.at<double>(ir, ic-1) == 0.0 && aResult.at<double>(ir, ic+1) == 0.0)
					{
						aResult.at<double>(ir, ic) = 0.0;
					}
				}
				*/

			}
		}


		int cnt = cv::countNonZero(aResult);
		//std::cout << cnt << std::endl;
		//std::cout << "###########################################" << std::endl;
		simR.at<double>(0, iChannel) = (double(aResult.rows * aResult.cols) - double(cnt)) / double(aResult.rows * aResult.cols);
		if(flag == 1)
		{
			if(iChannel == 0)
			{
				WriteCSV("aResult0.csv", aResult);
			}
			else if(iChannel == 1)
			{
				WriteCSV("aResult1.csv", aResult);
			}
			else if(iChannel == 2)
			{
				WriteCSV("aResult2.csv", aResult);
			}
		}

		
	}

	return 0;
}



cv::Point MinVByMatchMat(cv::Mat temp, cv::Mat comp)
{
	cv::Mat result;
	cv::matchTemplate(temp, comp, result, cv::TM_SQDIFF);
	double maxV;
	double minV;
	cv::Point maxP;
	cv::Point minP;
	cv::minMaxLoc(result, &minV, &maxV, &minP, &maxP);
	PrintINFO("#min V:%lf(%d, %d)\n", double(result.at<float>(minP.y, minP.x)), minP.x, minP.y);
	return minP;
}

cv::Point MinPoint(std::vector<cv::Mat> &locTemp, cv::Mat &locComp)
{
	std::vector<cv::Mat> locChannels;
	cv::split(locComp, locChannels);
    PrintDBG("**************************************************************!\n");
	cv::Point minP0 = MinVByMatchMat(locTemp[0], locChannels[0]);
	cv::Point minP1 = MinVByMatchMat(locTemp[1], locChannels[1]);
	cv::Point minP2 = MinVByMatchMat(locTemp[2], locChannels[2]);
	cv::Point minP(-1, -1);
	if(minP0 == minP1)
	{
		minP = minP0;
	}
	else if(minP1 == minP2)
	{
		minP = minP1;
	}
	else if(minP2 == minP0)
	{
		minP = minP2;
	}
    else
    {

    }

	return minP;
}


//要添加边界检查
int InitStaticField(stSOM &m_som, cv::Mat tempImg)
{
	m_som.p_locRect = cv::Rect(m_som.p_rect.x - SQUARE_LOCATION_OFFSET, m_som.p_rect.y - SQUARE_LOCATION_OFFSET, m_som.p_rect.width + SQUARE_LOCATION_OFFSET*2+1, m_som.p_rect.height + SQUARE_LOCATION_OFFSET*2+1);
	m_som.p_locTrigeCnt = 0;
	m_som.p_locRealCnt = 0;
    m_som.p_locFailCnt = 0;
	m_som.p_locP = cv::Point(100000, 100000);



    m_som.s_luminFilterT = 30.0;
	m_som.beginHour = 8;
	m_som.endHour = 19;

    m_som.stageW = tempImg.cols;
    m_som.stageH = tempImg.rows;
    m_som.stageStartTime = time(NULL);
	m_som.stageTimeStamp = time(NULL);

    memset(m_som.s_state, 0, sizeof(int)*SECONDARY_SCENE_MAX_NUM);
    memset(m_som.s_warnTrigger, 0, sizeof(int)*SECONDARY_SCENE_MAX_NUM);

    cv::GaussianBlur(tempImg(m_som.p_rect), m_som.p_tempGB, cv::Size(3, 3), 0);
	cv::split(m_som.p_tempGB, m_som.p_locTemp);

    for(int i = 0; i < m_som.s_sceneNum; i++)
    {
        m_som.s_tempNormal[i] = (m_som.p_tempGB(m_som.s_rect[i])).clone();
    }
    return 0;
}

int StaticFieldLogicRun(cv::Mat frame, stSOM &m_som, cv::Mat &excepMat, int &subType, int isDisplay)
{
    int ret = 0;
    struct std::tm m_cutTm = {0};
    std::time_t m_nowTime = std::time(NULL);
	localtime_r(&m_nowTime, &m_cutTm);
    if((m_som.beginHour < m_som.endHour && m_cutTm.tm_hour >= m_som.beginHour && m_cutTm.tm_hour < m_som.endHour) || (m_som.beginHour == m_som.endHour))
    {
        if((time(NULL) - m_som.stageTimeStamp) >= 1)
        {
            cv::Mat locGBImg;
            cv::GaussianBlur(frame(m_som.p_locRect), locGBImg, cv::Size(3, 3), 0);
            
            cv::Point minP = MinPoint(m_som.p_locTemp, locGBImg);
            PrintINFO("######(%d, %d)######\n", minP.x, minP.y);
            if(minP.x != -1 && minP.y != -1)
            {

                if(m_som.p_locP.x == minP.x && m_som.p_locP.y == minP.y)
                {
                    m_som.p_locTrigeCnt++;
                    m_som.p_locRealCnt++;
                }
                else
                {
                    m_som.p_locP.x = minP.x;
                    m_som.p_locP.y = minP.y;
                    m_som.p_locTrigeCnt = 0;
                    m_som.p_locRealCnt = 0;
                }

                int cropX = m_som.p_locP.x - SQUARE_LOCATION_OFFSET;
                int cropY = m_som.p_locP.y - SQUARE_LOCATION_OFFSET;
                if(abs(cropX) <= SQUARE_LOCATION_EFFECTIVE_OFFSET && abs(cropY) <= SQUARE_LOCATION_EFFECTIVE_OFFSET)
                {
                    m_som.p_locFailCnt = 0;
                    if(m_som.p_locRealCnt >= RE_LOCATION_CNT)
                    {
                        cv::Rect tmpR = cv::Rect(m_som.p_rect.x + cropX, m_som.p_rect.y + cropY, m_som.p_rect.width, m_som.p_rect.height);
                        m_som.p_locRect = cv::Rect(tmpR.x - SQUARE_LOCATION_OFFSET, tmpR.y - SQUARE_LOCATION_OFFSET, tmpR.width + SQUARE_LOCATION_OFFSET * 2 + 1, tmpR.height + SQUARE_LOCATION_OFFSET * 2 + 1);
                        m_som.p_locRealCnt = 0;
                        PrintWARN("#re-location: (%d, %d) w(%d) h(%d)!\n", tmpR.x, tmpR.y, tmpR.width, tmpR.height);
                        ret = 1;//重新定位
                    }
                    else
                    {
                        if(m_som.p_type == ePrimarySceneType::TRASH_CAN || m_som.p_type == ePrimarySceneType::WELL_LID || m_som.p_type == ePrimarySceneType::SMART_SEAT)
                        {
                            if(m_som.p_locTrigeCnt >= LOCATION_MAX_EFFECTIVE_CNT)
                            {
                                
                                PrintDBG("##############################################################!\n");
                                cv::Mat compGBImg = locGBImg(cv::Rect(m_som.p_locP.x, m_som.p_locP.y, m_som.p_rect.width, m_som.p_rect.height));
                                for(int si = 0; si < m_som.s_sceneNum; si++)
                                {
                                    
                                    cv::Mat s_compImg = compGBImg(m_som.s_rect[si]);
                                    cv::Mat s_tempNormalImg = m_som.s_tempNormal[si].clone();
                                    
                                    cv::Mat simRatioN;
                                    cv::Mat simRatioNFirst;
                                    cv::Mat tempFirst = m_som.p_tempGB(m_som.s_rect[si]);
                                    cv::Mat simRatioE;
                                    int retSim = -100;
                                    if(m_som.s_state[si] == 0)
                                    {
                                        retSim = PlaneSimilarityRatio(s_tempNormalImg, s_compImg, simRatioN, m_som.s_luminFilterT * 2.0);

                                    }
                                    else if(m_som.s_state[si] == 1)
                                    {
                                        retSim = PlaneSimilarityRatio(s_tempNormalImg, s_compImg, simRatioN, m_som.s_luminFilterT);
                                    }
                                    else
                                    {
                                        retSim = PlaneSimilarityRatio(s_tempNormalImg, s_compImg, simRatioN, m_som.s_luminFilterT);
                                        retSim = PlaneSimilarityRatio(tempFirst, s_compImg, simRatioNFirst, m_som.s_luminFilterT * 2.0);
                                        retSim = PlaneSimilarityRatio(m_som.s_tempException[si], s_compImg, simRatioE, m_som.s_luminFilterT / 2.0);
                                    }


                                    if(simRatioN.cols == 3 && retSim == 0)
                                    {
                                        std::string time_str = GetTimeToStringForPrint();
                                        if(m_som.s_state[si] < 2)
                                        {
                                            PrintINFO(" @%d ret(%d): %d-%lf-%lf-%lf\n", si, retSim, m_som.s_state[si], simRatioN.at<double>(0, 0),simRatioN.at<double>(0, 1),simRatioN.at<double>(0, 2));
                                            if(simRatioN.at<double>(0, 0) >= 0.95 && simRatioN.at<double>(0, 1) >= 0.95 && simRatioN.at<double>(0, 2) >= 0.95)
                                            {
                                                addWeighted(s_tempNormalImg, 0.2, s_compImg, 0.8, 0.0, m_som.s_tempNormal[si]);
                                                m_som.s_state[si] = 1;
                                            }
                                            else if(simRatioN.at<double>(0, 0) >= 0.90 && simRatioN.at<double>(0, 1) >= 0.90 && simRatioN.at<double>(0, 2) >= 0.90)
                                            {
                                                addWeighted(s_tempNormalImg, 0.5, s_compImg, 0.5, 0.0, m_som.s_tempNormal[si]);
                                                m_som.s_state[si] = 1;
                                            }
                                            else if(simRatioN.at<double>(0, 0) >= 0.80 && simRatioN.at<double>(0, 1) >= 0.80 && simRatioN.at<double>(0, 2) >= 0.80)
                                            {
                                                addWeighted(s_tempNormalImg, 0.8, s_compImg, 0.2, 0.0, m_som.s_tempNormal[si]);
                                                m_som.s_state[si] = 1;
                                            }
                                            else
                                            {
                                                m_som.s_tempException[si] = s_compImg.clone();
                                                m_som.s_warnTrigger[si] = 0;
                                                m_som.s_state[si] = 2;
                                            }
                                        }
                                        else
                                        {
                                            PrintINFO(" @%d ret(%d): %d-%lf-%lf-%lf\n", si, retSim, m_som.s_state[si], simRatioNFirst.at<double>(0, 0), simRatioNFirst.at<double>(0, 1), simRatioNFirst.at<double>(0, 2));
                                            PrintINFO(" @%d ret(%d): %d-%lf-%lf-%lf\n", si, retSim, m_som.s_state[si], simRatioN.at<double>(0, 0), simRatioN.at<double>(0, 1), simRatioN.at<double>(0, 2));
                                            PrintINFO(" @%d ret(%d): %d-%lf-%lf-%lf\n", si, retSim, m_som.s_state[si], simRatioE.at<double>(0, 0), simRatioE.at<double>(0, 1), simRatioE.at<double>(0, 2));
                                            if(simRatioNFirst.at<double>(0, 0) >= 0.85 && simRatioNFirst.at<double>(0, 1) >= 0.85 && simRatioNFirst.at<double>(0, 2) >= 0.85)
                                            {
                                                addWeighted(tempFirst, 0.5, s_compImg, 0.5, 0.0, m_som.s_tempNormal[si]);
                                                m_som.s_state[si] = 1;
                                            }
                                            else if(simRatioN.at<double>(0, 0) >= 0.85 && simRatioN.at<double>(0, 1) >= 0.85 && simRatioN.at<double>(0, 2) >= 0.85)
                                            {
                                                addWeighted(s_tempNormalImg, 0.5, s_compImg, 0.5, 0.0, m_som.s_tempNormal[si]);
                                                m_som.s_state[si] = 1;
                                            }
                                            else
                                            {

                                                if(simRatioE.at<double>(0, 0) >= 0.90 && simRatioE.at<double>(0, 1) >= 0.90 && simRatioE.at<double>(0, 2) >= 0.90)
                                                {
                                                    m_som.s_warnTrigger[si]++;
                                                    if(m_som.s_warnTrigger[si] != MAX_WARNING_CNT)
                                                    {
                                                        PrintWARN("prime scene(%d) sub(%d) exception cnt is %d!!!\n", m_som.p_type, m_som.s_type[si], m_som.s_warnTrigger[si]);
                                                    }
                                                    else
                                                    {
                                                        PrintWARN("prime scene(%d) sub(%d) exception trigger!!!\n",m_som.p_type, m_som.s_type[si]);
                                                        if(GetIsSave() == 1)
                                                        {
                                                            std::string str = GetTimeToStringForLog() + std::string("_") + std::to_string(int(m_som.p_type)) + std::string("_") + std::to_string(int(m_som.s_type[si])) + std::string(".jpg");
                                                            cv::imwrite(str, frame);
                                                            sync();
                                                        }
                                                        
                                                        excepMat = compGBImg.clone();
                                                        subType = m_som.s_type[si];
                                                        ret = 2;//有异常，输出异常图片和告警类型
                                                        
                                                    }

                                                }
                                                else
                                                {
                                                    m_som.s_tempException[si] = s_compImg.clone();
                                                    m_som.s_warnTrigger[si] = 0;
                                                }
                                                m_som.s_state[si] = 2;

                                            }
                                        }
                                        
                                    }

                                }
                                
                                
                                
                                m_som.p_locTrigeCnt = 0;
                            }

                        }
                        else if(m_som.p_type == ePrimarySceneType::CAR_ACCIDENT)
                        {
                            if(m_som.p_locTrigeCnt >= ACCIDENT_LOCATION_MAX_CNT)
                            {
                                PrintDBG("##############################################################!\n");
                                cv::Mat compGBImg = locGBImg(cv::Rect(m_som.p_locP.x, m_som.p_locP.y, m_som.p_rect.width, m_som.p_rect.height));
                                PrintWARN("accident happened!!!\n");
                                if(GetIsSave() == 1)
                                {
                                    std::string str = GetTimeToStringForLog() + std::string("_") + std::to_string(int(m_som.p_type)) + std::string("_") + std::string(".jpg");
                                    cv::imwrite(str, frame);
                                    sync();
                                }
                                excepMat = compGBImg.clone();
                                subType = int(eCarAccidentT::CA_NULL);
                                ret = 2;//发现车辆事故
                                m_som.p_locTrigeCnt = 0;
                            }
                        }
                    
                    }
                }
                else //没有定位到有效范围内也算定位失败
                {
                    m_som.p_locFailCnt ++;
                    if(m_som.p_locFailCnt > LOCATION_FAIL_CNT)
                    {
                        m_som.p_locFailCnt = 0;
                        PrintWARN("Prime Scene change!!!\n");
                        ret = -1;//一直定位失败就报警
                    }
                }
                

                if(isDisplay == 1)
                {
                    cv::Mat compTmp = locGBImg(cv::Rect(m_som.p_locP.x, m_som.p_locP.y, m_som.p_rect.width, m_som.p_rect.height));
                    for(int ii = 0; ii < m_som.s_sceneNum; ii++)
                    {
                        cv::rectangle(compTmp, m_som.s_rect[ii], cv::Scalar(0,0,255), 1, 8, 0);
                        imshow("LIVE_temp" + std::to_string(ii), m_som.s_tempNormal[ii]);
                    }
                    imshow("LIVE_comp", compTmp);
                }

            }
            else
            {
                m_som.p_locFailCnt ++;
                if(m_som.p_locFailCnt > LOCATION_FAIL_CNT)
                {
                    m_som.p_locFailCnt = 0;
                    PrintWARN("Prime Scene change!!!\n");
                    ret = -1;//一直定位失败就报警
                }

            }
            m_som.stageTimeStamp = time(NULL);
            
        }
    }
    


    return ret;
}