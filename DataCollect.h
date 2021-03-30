#ifndef DATACOLLECT_H
#define DATACOLLECT_H

#include "opencv2/opencv.hpp"

using namespace cv;

#define DATA_BUFF_SIZE_MAX 2000

class DataCollect
{
public:
    DataCollect(int canvas_w,int canvas_h,int ndataSize,int ndataRange);

    /**
     * @brief  调用私有方法dataCollect，通过多次调用，传入多个数据，在一个窗口绘制该组数据的波形
     * @param  data    显示的数据向量
     * @param  color   对应的颜色向量
     * @param  winname 窗口名
     * @return
     * @autor  参与开发的人员
     * @date   2018-11-14
     */
    void dataCollectProc(vector<float> data,vector<Scalar> color,const String winname);

    /**
     * @brief  调用私有方法dataCollect，通过传入一组数据，在一个窗口绘制该组数据的波形
     * @param  data    显示的数据
     * @param  color   对应的颜色
     * @param  winname 窗口名
     * @return
     * @autor  参与开发的人员
     * @date   2018-11-14
     */
    void dataCollectInvProc(float data,Scalar color,const String winname);

    /**
     * @brief  私有方法，实现核心的数据波形绘制操作
     * @param  data    显示的数据
     * @param  color   对应的颜色
     * @param  winname 窗口名
     * @return
     * @autor  参与开发的人员
     * @date   2018-11-14
     */
    void dataCollect(float data,Scalar color,const String winname);

    /**
     * @brief
     * @param  data    显示的数据
     * @param  color   对应的颜色
     * @return
     * @autor  参与开发的人员
     * @date   2018-11-14
     */
    void gui(float data,Scalar color);

    /**
     * @brief  设置画布
     * @return
     * @autor  参与开发的人员
     * @date   2018-11-14
     */
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
