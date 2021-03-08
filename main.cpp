#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define AVG_BUFF_SIZE 250

int avg[AVG_BUFF_SIZE]={0};
int p_cur_avg=0;

int main()
{
    namedWindow("src",1);

    // 图像源获取及判断
    cv::Mat Image, ImageGray;

    VideoCapture cap(0);

    int i=0;

    // 初始化画布参数
    int hist_w = 500;
    int hist_h = 500;
    int nHistSize = 255;

    while(1)
    {
        i++;

        cap>>Image;

        if(Image.empty())
          return -1;
        cv::imshow("Image",Image);
        // 转换为灰度图像
        cv::cvtColor(Image,ImageGray,CV_BGR2GRAY);

        int sum=0;

        //计算均灰度值
        for(int i=0;i<ImageGray.rows;i++)
        {
            uchar *data=ImageGray.ptr<uchar>(i);
            for(int j=0;j<ImageGray.cols;j++)
            {
                sum+=data[j];
            }
        }

        avg[i%AVG_BUFF_SIZE]=sum/(ImageGray.rows*ImageGray.cols);

        // 区间
        int bin_w = cvRound( (double) hist_w / nHistSize );

        cv::Mat histImage( hist_w, hist_h,
                 CV_8UC3,cv::Scalar( 0,0,0) );

        if(i>0)
        {
            for(int j=0;j<hist_w;j++)
            {
                line(histImage,
                     Point(bin_w*(hist_w-j-1),hist_h-avg[(i-j+AVG_BUFF_SIZE)%AVG_BUFF_SIZE]),
                     Point(bin_w*(hist_w-j),hist_h-avg[(i-j+1+AVG_BUFF_SIZE)%AVG_BUFF_SIZE]),
                     Scalar(0,255,0),1,8,0);

                cout<<(i-j+AVG_BUFF_SIZE)%AVG_BUFF_SIZE<<endl;
                cout<<(i-j+1+AVG_BUFF_SIZE)%AVG_BUFF_SIZE<<endl;
            }

            // 采集图
            cv::imshow("histImage", histImage);
        }

        waitKey(1);


    }
    return 0;
}
