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
    // ͼ��Դ��ȡ���ж�
    cv::Mat Image, ImageGray;
    VideoCapture cap(0);
    // �ֲ���ֵ����������
    Adaptive adt;
    adt.blockSize = 360;
    adt.constValue = 20;
    cv::namedWindow(WIN_NAME_ADT);
    cv::createTrackbar("blockSize", WIN_NAME_ADT, &adt.blockSize, 720);
    cv::createTrackbar("constValue", WIN_NAME_ADT, &adt.constValue, 50);
    // Ŀ���С������ȵ���
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

    //��ʼ������������ʾ�ɼ�������
    //�������
    //�����߶�
    //��ʾ���ݸ���???-1 �������???
    //��ʾ���ݷ�Χ???-1 �����߶�???
    DataCollect data_col(1000,510,-1,400);

    while(1)
    {
        cap>>Image;

        if(Image.empty())
          return -1;
        cv::imshow("ԭͼ",Image);
        // ת��Ϊ�Ҷ�ͼ
        Mat ImageGray;
        cv::cvtColor(Image,ImageGray,CV_BGR2GRAY);

        //�ֲ���ֵ��
        Mat bin;
        cv::adaptiveThreshold(ImageGray, bin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, adt.blockSize*2+1, adt.constValue);
        cv::imshow("��ֵͼ",bin);

        //Ѱ�ұ�Ե
        std::vector<std::vector<cv::Point2i>>contours;
        std::vector<cv::Vec4i>hierarchy;
        cv::findContours(bin,contours,hierarchy,cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

        //��Ͼ���
        Rect rrect;
        Mat show_img=Image.clone();
        for(int i=0;i<contours.size();i++)
        {
            cv::Rect rect = boundingRect(contours[i]);
            //ɸѡ�����������ľ���
            if(rect.width >= tsize.wmin&&rect.width <= tsize.wmax&&
                rect.height >= tsize.hmin&&rect.width <= tsize.hmax&&
                (rect.width * 1.0 / rect.height) >= tsize.radiol*0.1&&(rect.width * 1.0 / rect.height) <= tsize.radioh*0.1)
            {
                rrect=rect;
                rectangle(show_img,rect,Scalar(0,255,0),2);//����show_img�ϣ���������->��ɫ
                char trect[10];
                sprintf(trect,"%d %d",rect.width,rect.height);
                putText(show_img,trect,rect.br(),1,2,Scalar(0,255,0),1);
            }
            else
            {
                rectangle(show_img,rect,Scalar(0,0,255),2);//����show_img�ϣ�����������->��ɫ
            }
        }
        imshow("rect",show_img);//��ʾ���

        /**
         * ʵ�֣�ʵʱ����  rect.x/rect.y  ���˲�
         * 1/���˸�Ƶ����
         * 2/���ݷ�����Խ����ʱ��ʹ���ݱ仯ƽ��
         * �������˲���̬�켣Ԥ��
         * ��show_img�л����˲�ǰ���˲���Ĺ켣
         * @date   2018.11.19
         */

        //��ʾ
        data_col.dataCollect(rrect.x,Scalar(0,255,0),"rrect.x");
        waitKey(1);
    }
    return 0;
}
