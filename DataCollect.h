#ifndef DATACOLLECT_H
#define DATACOLLECT_H

#include "opencv2/opencv.hpp"

using namespace cv;

#define DATA_BUFF_SIZE_MAX 2000

class DataCollect
{
public:
    DataCollect(int canvas_w,int canvas_h,int ndataSize,int ndataRange);
    void dataCollect(float data,Scalar color,const String winname);
    void gui(float data,Scalar color);
    void setCanvas();
private:
    float data_q[DATA_BUFF_SIZE_MAX];//数据队列
    int data_q_h;
    int canvas_w;//画布宽度
    int ndataSize;//显示数据个数
    int ndataRange;//显示数据范围
    float interval;//区间=画布宽度/显示数据个数
    int canvas_h;//画布高度
    Mat coll;//画布

    float proportion_v;//值比例
    float proportion_t;//时间比例

    int offset_v;//值偏移
    int offset_t;//时间偏移
};

#endif // DATACOLLECT_H
