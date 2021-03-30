#include "DataCollect.h"

#include <iostream>

using namespace std;

/*
完成功能：
1、完成dataCollectProc方法，实现多个数据同时在同一张图中显示
   完成dataCollectInvProc方法，实现多个数据同时在同一张图中显示，dataCollectInvProc方法可分别调用
2、完善平移时画布坐标线的显示
3、实现水平与垂直方向的缩放功能
   垂直方向超出范围自动缩放
4、实现使用鼠标平移画布
*/

DataCollect::DataCollect(int canvas_w,int canvas_h,int ndataSize,int ndataRange)
{
    for(int i=0;i<DATA_BUFF_SIZE_MAX;i++)
    {
        data_q[i]=0;
    }

    data_q_h=0;

    this->canvas_w=canvas_w;//画布宽度
    if(ndataSize==-1)
    {
        this->ndataSize=canvas_w;//显示数据个数=画布宽度
    }else{
        this->ndataSize=ndataSize;//显示数据个数
    }
    if(ndataRange==-1){
        this->ndataRange=canvas_h;//显示数据范围=画布高度
    }else{
        this->ndataRange=ndataRange;//显示数据范围
    }
    interval=float(canvas_w)/canvas_w;//区间=画布宽度/显示数据个数
    this->canvas_h=canvas_h;//画布高度

    proportion_v=1;//值比例
    proportion_t=1;//时间比例
    offset_v=0;//值偏移
    offset_t=0;//时间偏移
}

void DataCollect::dataCollectProc(vector<float> data,vector<Scalar> color,const String winname)
{
    
}

void DataCollect::dataCollectInvProc(float data,Scalar color,const String winname)
{
    
}

void DataCollect::dataCollect(float data,Scalar color,const String winname)
{
    coll=Mat::zeros(canvas_h,canvas_w,CV_8UC3);
    setCanvas();
    gui(data,color);
    data_q_h=data_q_h%ndataSize;
    data_q[data_q_h]=data;
    data_q[data_q_h]*=(canvas_h/ndataRange);

    int p_data;
    for(int j=0;j<ndataSize-1;j++)
    {
        p_data=data_q_h-j+canvas_w;
        line(coll,Point(interval*(ndataSize-j-1)+offset_t,proportion_v*(canvas_h-data_q[(p_data-1)%canvas_w])+offset_v),
             Point(interval*(ndataSize-j)+offset_t,proportion_t*(canvas_h-data_q[p_data%canvas_w])+offset_v),
             color,1,8,0);
    }
    data_q_h++;
    imshow(winname,coll);
}

void DataCollect::gui(float data,Scalar color)
{
    static int proportion_v_subhalf=100;
    static int proportion_t_subhalf=100;

    static int offset_v_subhalf=canvas_h/2;
    static int offset_t_subhalf=canvas_w/2;

    namedWindow("gui",WINDOW_AUTOSIZE);
    createTrackbar("proportion_v*100","gui",&proportion_v_subhalf,200);
    createTrackbar("proportion_t*100","gui",&proportion_t_subhalf,200);
    proportion_v=proportion_v_subhalf/100.0;
    proportion_t=proportion_t_subhalf/100.0;

    createTrackbar("offset_v+h/2","gui",&offset_v_subhalf,canvas_h);
    createTrackbar("offset_t+w/2","gui",&offset_t_subhalf,canvas_w);
    offset_v=offset_v_subhalf-canvas_h/2.0;
    offset_t=offset_t_subhalf-canvas_w/2.0;

    char fdata[10]="";
    sprintf(fdata,"%.f",data);
    putText(coll,fdata,Point(canvas_w-30+offset_t,canvas_h-data+offset_v),1,1,color,1,8);
}

void DataCollect::setCanvas()
{
    int color_gray=50;

    for(int y=0;y<canvas_h;y+=64)
    {
        //横线
        line(coll,Point(offset_t,y+offset_v),
             Point(canvas_w+offset_t,y+offset_v),
             Scalar(color_gray,color_gray,color_gray),1,8,0);
        //横坐标
        char row_coor[10];
        sprintf(row_coor,"%d",y/2);
        putText(coll,row_coor,Point(canvas_w-50+offset_t,canvas_h-y+offset_v),1,1,Scalar(color_gray,color_gray,color_gray),1,8);
    }

    for(int x=0;x<canvas_w;x+=50)
    {
        //纵线
        line(coll,Point(x+offset_t,offset_v),
             Point(x+offset_t,canvas_h+offset_v),
             Scalar(color_gray,color_gray,color_gray),1,8,0);
        //纵坐标
        char col_coor[10];
        sprintf(col_coor,"%d",x/2);
        putText(coll,col_coor,Point(canvas_w-x+offset_t,canvas_h-50+offset_v),1,1,Scalar(color_gray,color_gray,color_gray),1,8);
    }
}
