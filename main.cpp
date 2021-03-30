#include <iostream>
#include <opencv2/opencv.hpp>

#include "DataCollect.h"

using namespace cv;
using namespace std;

#define AVG_BUFF_SIZE 250            //
#define WIN_NAME_ADT "adaptive"
#define WIN_NAME_TARGET "target_size"

struct Adaptive
{
    int blockSize;
    int constValue;
};

struct TargetSize
{
    int wmin;
    int wmax;
    int hmin;
    int hmax;
    int radiol;
    int radioh;
};

int main()
{
    // 图像源获取及判断
    cv::Mat Image, ImageGray;
    VideoCapture cap(0);
    // 局部二值化参数调整
    Adaptive adt;
    adt.blockSize = 360;
    adt.constValue = 20;
    cv::namedWindow(WIN_NAME_ADT);
    cv::createTrackbar("blockSize", WIN_NAME_ADT, &adt.blockSize, 720);
    cv::createTrackbar("constValue", WIN_NAME_ADT, &adt.constValue, 50);
    // 目标大小及长宽比调整
    TargetSize tsize;
    tsize.wmin=50;
    tsize.wmax=450;
    tsize.hmin=50;
    tsize.hmax=450;
    tsize.radiol=5;
    tsize.radioh=15;
    namedWindow(WIN_NAME_TARGET);
    cv::createTrackbar("wmin",WIN_NAME_TARGET,&tsize.wmin, 1000);
    cv::createTrackbar("wmax",WIN_NAME_TARGET,&tsize.wmax, 1000);
    cv::createTrackbar("hmin",WIN_NAME_TARGET,&tsize.hmin, 1000);
    cv::createTrackbar("hmax",WIN_NAME_TARGET,&tsize.hmax, 1000);
    cv::createTrackbar("radiol",WIN_NAME_TARGET,&tsize.radiol, 10);
    cv::createTrackbar("radioh",WIN_NAME_TARGET,&tsize.radioh, 20);

    //初始化画布，以显示采集的数据
    //画布宽度
    //画布高度
    //显示数据个数???-1 画布宽度???
    //显示数据范围???-1 画布高度???
    DataCollect data_col(1000,510,-1,400);

    while(1)
    {
        cap>>Image;

        if(Image.empty())
          return -1;
        cv::imshow("原图",Image);
        // 转换为灰度图
        Mat ImageGray;
        cv::cvtColor(Image,ImageGray,CV_BGR2GRAY);

        //局部二值化
        Mat bin;
        cv::adaptiveThreshold(ImageGray, bin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, adt.blockSize*2+1, adt.constValue);
        cv::imshow("二值图",bin);

        //寻找边缘
        std::vector<std::vector<cv::Point2i>>contours;
        std::vector<cv::Vec4i>hierarchy;
        cv::findContours(bin,contours,hierarchy,cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        //拟合矩形
        Rect rrect;
        Mat show_img=Image.clone();
        for(int i=0;i<contours.size();i++)
        {
            cv::Rect rect = boundingRect(contours[i]);
            //筛选出符合条件的矩形
            if(rect.width >= tsize.wmin&&rect.width <= tsize.wmax&&
                rect.height >= tsize.hmin&&rect.width <= tsize.hmax&&
                (rect.width * 1.0 / rect.height) >= tsize.radiol*0.1&&(rect.width * 1.0 / rect.height) <= tsize.radioh*0.1)
            {
                rrect=rect;
                rectangle(show_img,rect,Scalar(0,255,0),2);//画在show_img上，符合条件->绿色
                char trect[10];
                sprintf(trect,"%d %d",rect.width,rect.height);
                putText(show_img,trect,rect.br(),1,2,Scalar(0,255,0),1);
            }
            else
            {
                rectangle(show_img,rect,Scalar(0,0,255),2);//画在show_img上，不符合条件->红色
            }
        }
        imshow("rect",show_img);//显示结果

        /**
         * 实现：实时数据  rect.x/rect.y  的滤波
         * 1/过滤高频噪声
         * 2/数据发生阶越跳变时，使数据变化平缓
         * 卡尔曼滤波动态轨迹预测
         * 在show_img中画出滤波前与滤波后的轨迹
         * @date   2018.11.19
         */

        //显示
        data_col.dataCollect(rrect.x,Scalar(0,255,0),"rrect.x");
        waitKey(1);
    }
    return 0;
}
