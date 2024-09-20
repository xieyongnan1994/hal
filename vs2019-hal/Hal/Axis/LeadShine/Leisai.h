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
	virtual double MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0);		//����˶�

	virtual int MoveAbs(int nPos, int nSpeedLevel = 100);
	virtual double MoveAbsMm(double dPos, int nSpeedLevel = 100);
	virtual int MoveAbs(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1);
	virtual double MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0);		//�����˶�

	virtual int Jog(int nDirection, int nSpeedLevel = 100);
	virtual int Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc = 0, double nSpeedDec = 0);			//�㶯Dir����0Ϊ����1Ϊ��

	virtual int GetPos();																									//��ȡ��ǰλ��
	virtual double GetPosMm();																								//��ȡ��ǰλ��

	virtual int GetEncValue();																								//��ȡ��������ֵ
	virtual int SetEncValue(int Value);
	virtual double GetEncPosMm();
	virtual double SetEncPosMm(double Value);

	virtual int GetLogicValue();																							//��ȡ�߼������ֵ
	virtual int SetLogicValue(int Value);
	virtual double GetLogicMm();
	virtual double SetLogicMm(double Value);

	virtual int GetAlm(bool& bAlm);																							//����

	virtual  int SetEnableLimit(bool bEnable);																				//������λ����
	virtual  int GetEnableLimit();																							//������λ����

	virtual int IsStop();																									//����ֵ 0�������˶���1��ֹͣ�˶�

	virtual int GetAxisPosLmt(bool& LmtValue);																				//����ֵ��LmtValue,����ֵ��1Ϊ�����أ�2Ϊ�½��أ�0Ϊû�仯
	virtual int GetAxisNegLmt(bool& LmtValue);


};
#endif
