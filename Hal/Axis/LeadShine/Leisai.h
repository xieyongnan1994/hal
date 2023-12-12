#pragma once
#ifndef _GOOGOL_H
#define _GOOGOL_H
#include "Axis/AxisBase.h"
#include "Dmc1380.h"

class Leisai : AxisBase
{
public:
	Leisai();
	~Leisai();

public:
	virtual int MoveRel(int nPos, int nSpeedLevel = 100);
	virtual double MoveRelMm(double dPos, int nSpeedLevel = 100);
	virtual int MoveRel(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1);
	virtual double MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0);		//相对运动

	virtual int MoveAbs(int nPos, int nSpeedLevel = 100);
	virtual double MoveAbsMm(double dPos, int nSpeedLevel = 100);
	virtual int MoveAbs(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1);
	virtual double MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0);		//绝对运动

	virtual int Jog(int nDirection, int nSpeedLevel = 100);
	virtual int Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc = 0, double nSpeedDec = 0);			//点动Dir方向：0为正，1为负

	virtual int GetPos();																									//获取当前位置
	virtual double GetPosMm();																								//获取当前位置

	virtual int GetEncValue();																								//获取编码器的值
	virtual int SetEncValue(int Value);
	virtual double GetEncPosMm();
	virtual double SetEncPosMm(double Value);

	virtual int GetLogicValue();																							//获取逻辑脉冲的值
	virtual int SetLogicValue(int Value);
	virtual double GetLogicMm();
	virtual double SetLogicMm(double Value);

	virtual int GetAlm(bool& bAlm);																							//报警

	virtual  int SetEnableLimit(bool bEnable);																				//启用限位功能
	virtual  int GetEnableLimit();																							//启用限位功能

	virtual int IsStop();																									//返回值 0：正在运动，1：停止运动

	virtual int GetAxisPosLmt(bool& LmtValue);																				//极限值：LmtValue,返回值：1为上升沿，2为下降沿，0为没变化
	virtual int GetAxisNegLmt(bool& LmtValue);


};
#endif
