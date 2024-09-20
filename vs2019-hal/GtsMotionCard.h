#pragma once
#include "gts.h"


//自定义的固高控制卡全局变量和函数20160320
#define GTSMOTION_API extern "C" short __stdcall

#define CARD_NO1                        0          //卡号
#define CARD_NO2                        1
#define CARD_NO3                        2
#define CARD_NO4                        3

#define AXIS_NO1                        1          //轴号
#define AXIS_NO2                        2
#define AXIS_NO3                        3
#define AXIS_NO4                        4
#define AXIS_NO5                        5
#define AXIS_NO6                        6
#define AXIS_NO7                        7
#define AXIS_NO8                        8

#define IM_STOP                         0          //平滑停止
#define EM_STOP                         1          //紧急停止

#define DIF_PULSE                       1000         //规划器与编码器差值
#define HOME_OFFSET                     300        //原点偏移量

#define SMOOTHTIME                      20         //单轴平滑时间(ms)

#define SYNVELMAX                       10000      //直线插补最大合成速度(pulse/ms)
#define SYNACCMAX                       100        //直线插补最大合成加速度(pulse/ms^2)
#define EVENTIME                        40         //直线插补最小匀速时间(ms)

//运动停止标志位
extern volatile BOOL m_motor_stop;
extern CCriticalSection m_CriticalCard;
//固高卡异常标志位
extern BOOL g_MotionError;
//软件的错误信息
extern CString g_ErrorMessage;
//软件的一般信息
extern CString g_GeneralMessage;

extern DWORD g_AlarmBeginTime;
extern BOOL  g_AlarmHappen;

//板卡初始化成功标志
extern volatile BOOL g_bInitCardOK;
//消息循环
GTSMOTION_API UCT_PeekAndPump();

//打开运动控制卡
GTSMOTION_API UCT_OpenMotionCard(short cardNum,short count=1);
//关闭运动控制卡
GTSMOTION_API UCT_CloseMotionCard(short cardNum,short count=1);
//复位运动控制卡
GTSMOTION_API UCT_ResetMotionCard(short cardNum,short count=1);
//加载配置文件
GTSMOTION_API UCT_LoadConfig(short cardNum,char *pFile);
//清除报警状态
GTSMOTION_API UCT_ClrSts(short cardNum,short axis,short count=1);

//伺服On
GTSMOTION_API UCT_AxisOn(short cardNum,short axis,short count=1);
//伺服Off
GTSMOTION_API UCT_AxisOff(short cardNum,short axis,short count=1);

//写输出
GTSMOTION_API UCT_WriteOutBit(short cardNum,short bit,short value,short count=1);
//写脉冲输出
GTSMOTION_API UCT_WriteTimeOutBit(short cardNum,short bit,short value,short reverseTime);
//读输入
GTSMOTION_API UCT_ReadInBit(short cardNum,short bit,BOOL *pValue);
//读输出
GTSMOTION_API UCT_ReadOutBit(short cardNum,short bit,BOOL *pValue);

//平滑停止
GTSMOTION_API UCT_IMStopMove(short cardNum,short axis,short count=1);
//紧急停止
GTSMOTION_API UCT_EMStopMove(short cardNum,short axis,short count=1);

//读取轴状态
GTSMOTION_API UCT_GetAxisSts(short cardNum,short axis,long *pValue);
//读取轴伺服报警
GTSMOTION_API UCT_GetServerAlarm(short cardNum,short axis,BOOL *pValue);
//读取轴正限位报警
GTSMOTION_API UCT_GetPosLimitAlarm(short cardNum,short axis,BOOL *pValue);
//读取轴负限位报警
GTSMOTION_API UCT_GetNegLimitAlarm(short cardNum,short axis,BOOL *pValue);
//获取正限位值
GTSMOTION_API UCT_GetPosLimitValue(short cardNum,short axis,BOOL *pValue);
//获取负限位值
GTSMOTION_API UCT_GetNegLimitValue(short cardNum,short axis,BOOL *pValue);
//获取原点值
GTSMOTION_API UCT_GetHomeValue(short cardNum,short axis,BOOL *pValue);
//获取规划器的值
GTSMOTION_API UCT_GetPrfPos(short cardNum,short axis,double *pValue);
//获取编码器的值
GTSMOTION_API UCT_GetEncPos(short cardNum,short axis,double *pValue);

//单轴回原点（有原点感应器）
GTSMOTION_API UCT_BackToHomeORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed,short sHomeCapture = 0);
//单轴回原点（有原点感应器）
GTSMOTION_API UCT_BackToHomeINDEX(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);
//单轴回原点（正限位作为原点）
GTSMOTION_API UCT_BackToHomePOSITIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);
//单轴回原点（负限位作为原点）
GTSMOTION_API UCT_BackToHomeNEGATIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);

//单轴运动到停止（bRelative=true相对运动，bRelative=false绝对运动，start_speed起点速度，speed目标速度）
GTSMOTION_API UCT_1AxisMoveDone(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative=false);
//单轴开始运动（bRelative=true相对运动，bRelative=false绝对运动，start_speed起点速度，speed目标速度）
GTSMOTION_API UCT_1AxisStartMove(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative=false);
//单轴运动状态(pValue=0停止 pValue=1运动中)
GTSMOTION_API UCT_1AxisCheckMoveSts(short cardNum,short axis,long *pValue);
//检查单轴运动是否到位
GTSMOTION_API UCT_1AxisCheckMoveDone(short cardNum,short axis,long *pValue);

//坐标系对应的直线插补开始运动
GTSMOTION_API UCT_LMotionStart(short cardNum, short coordinate=1, short buffer=0);
//插补运动坐标系运动状态（pRun=1运动中，pSegment当前已经完成的插补段数）
GTSMOTION_API UCT_CheckLMotionSts(short cardNum,short *pRun,long *pSegment, short coordinate=1, short buffer=0);
//清除此坐标系缓存区中的数据
GTSMOTION_API UCT_CrdClear(short cardNum, short coordinate=1, short buffer=0);
// 缓存区延时指令
GTSMOTION_API UCT_BufDelay(short cardNum, short delay,short coordinate=1, short buffer=0);

//两轴直线插补初始化
GTSMOTION_API UCT_2AxisLMotionInit(short cardNum, short axis1, short axis2, short coordinate=1, short buffer=0);
//两轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_2AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data, short coordinate=1, short buffer=0);

//三轴直线插补初始化
GTSMOTION_API UCT_3AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3,short coordinate=1, short buffer=0);
//三轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据，axis3_data轴3数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_3AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data, short coordinate=1, short buffer=0);

//四轴直线插补初始化
GTSMOTION_API UCT_4AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3, short axis4,short coordinate=1, short buffer=0);
//四轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据，axis3_data轴2数据,axis4_data轴4数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_4AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data,long axis4_data,short coordinate=1, short buffer=0);


//二维位置比较功能初始化
GTSMOTION_API UCT_2DCompareInit(short cardNum, short chn,short axis1, short axis2);
//二维位置比较功能写入数据(cardNum卡号，chn通道号，databuf二维数据,fifo缓冲区)
GTSMOTION_API UCT_2DCompareData(short cardNum,short chn,short count,T2DCompareData *databuf,short fifo=0);
//启动二维位置比较功能
GTSMOTION_API UCT_2DCompareStart(short cardNum, short chn);
//停止二维位置比较功能
GTSMOTION_API UCT_2DCompareStop(short cardNum, short chn);

//打开扩展IO卡
GTSMOTION_API UCT_OpenExtCard(short cardNum,char *pDllName=NULL,short count=1);
//加载扩展IO卡文件
GTSMOTION_API UCT_LoadExtConfig(short cardNum,char *pFileName=NULL,short count=1);
//写输出
GTSMOTION_API UCT_WriteExtOutBit(short cardNum,short mdl,short bit,short value,short count=1);
//读输入
GTSMOTION_API UCT_ReadExtInBit(short cardNum,short mdl,short bit,BOOL *pValue);
//读输出
GTSMOTION_API UCT_ReadExtOutBit(short cardNum,short mdl,short bit,BOOL *pValue);


//Jog模式单轴运动到停止
GTSMOTION_API JCT_1AxisMoveDone(short cardNum,short axis,long LDis,double speed,double acc,double dec);
//Jog模式单轴开始运动
GTSMOTION_API JCT_1AxisStartMove(short cardNum,short axis,long LDis,double speed,double acc,double dec);


GTSMOTION_API UCT_CompareData(short cardNum,short encoder,short source,short pulseType,short startLevel,short time,long *pBuf,short count);


GTSMOTION_API UCT_BackToHomeNegORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed, short home_capture =0,double acc_speed = 0.5, double dec_speed = 0.5);


GTSMOTION_API UCT_Calc1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis,double dbSpeed,double dbAcc, double dbDecDis,double dbDec,bool bRelative=false);

GTSMOTION_API UCT_1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis[],long lTime[],short sDataNum);