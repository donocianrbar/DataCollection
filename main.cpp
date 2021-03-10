#include <iostream>
#include <opencv2/opencv.hpp>

#include "DataCollect.h"

using namespace cv;
using namespace std;

#define AVG_BUFF_SIZE 250

int main()
{
    // 图像源获取及判断
    cv::Mat Image, ImageGray;
    VideoCapture cap(0);

    int gray_avg=0;
    DataCollect data_col(1000,510,-1,255);

    while(1)
    {
        cap>>Image;

        if(Image.empty())
          return -1;
        cv::imshow("Image",Image);
        // 转换为灰度图像
        cv::cvtColor(Image,ImageGray,CV_BGR2GRAY);

        //计算均灰度值
        int sum=0;
        for(int i=0;i<ImageGray.rows;i++)
        {
            uchar *data=ImageGray.ptr<uchar>(i);
            for(int j=0;j<ImageGray.cols;j++)
            {
                sum+=data[j];
            }
        }

        gray_avg=sum/(ImageGray.rows*ImageGray.cols);
        data_col.dataCollect(gray_avg,Scalar(0,255,0),"gray_avg");
        waitKey(1);
    }
    return 0;
}
