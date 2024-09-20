#pragma once
#ifndef _GOOGOL_H
#define _GOOGOL_H
#include "Axis/AxisBase.h"
#include "Axis/Googol/gts.h"

#define  MAXDEVICEID 8
#define  MAXAXISNO 8



class GTS_Init
{
public:
	~GTS_Init();
	static void  GTS_HardwareInit();		//硬件初始化，只调用一次
	static void  GTS_HardwareRelease();	//硬件反初始化，只调用一次
	static GTS_Init* GetInstance();
private:
	GTS_Init();
	static GTS_Init* m_pInstance;
	static int m_nGTSInitFlag;
};

class Googol :public Axis
{
public:
	Googol();
	~Googol();

public:
	virtual void Init(AxisPara& Para);							//轴配置初始化
	virtual int AxisEnable(int type = 0);						//轴使能 默认0使能
	virtual int GetEncValue() ;
	virtual double GetEncPosMm();
	virtual int GetLogicValue();
	virtual double GetLogicMm();
	virtual int GetAlm(bool& bAlm);								//获取报警																		
	virtual int GetEnableLimit();								//限位功能
	virtual int SoftLmtEnable(bool bEnable);					//软限位功能
	virtual int GetAxisPosLmt(bool& LmtValue);					//获取正限位																			
	virtual int GetAxisNegLmt(bool& LmtValue);					//获取负限位	
	virtual int GetHomeSignal(bool& HomeValue);					//获取原点
	virtual int GetAxisState();									//获取轴状态
	virtual int Stop(int type = 0);								//停止运动

	virtual int SoftLmtPos();									//设置软限位
	virtual int SetEncValue(int Value);							//设置编码器	
	virtual double SetEncPosMm(double Value);
	virtual int SetLogicValue(int Value);						//设置逻辑脉冲
	virtual double SetLogicMm(double Value);
	virtual int SetEnableLimit(bool bEnable);					//设置限位使能																					
	virtual int MoveRel(int nPos, int nSpeedLevel = 100);		//相对运动
	virtual int MoveAbs(int nPos, int nSpeedLevel = 100);		//绝对运动
	virtual int Jog(int nDirection, int nSpeedLevel = 100);		//jog运动
	virtual int Home(BOOL bHighSpeed = TRUE);

};
#endif
