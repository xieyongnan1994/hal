#include "Zmotion.h"


Zmotion::Zmotion()
{
}

Zmotion::~Zmotion()
{
}

int Zmotion::MoveRel(int nPos, int nSpeedLevel)
{
	return 0;
}

double Zmotion::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Zmotion::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Zmotion::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Zmotion::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double Zmotion::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Zmotion::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Zmotion::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Zmotion::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int Zmotion::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int Zmotion::GetPos()
{
	return 0;
}

double Zmotion::GetPosMm()
{
	return 0.0;
}

int Zmotion::GetEncValue()
{
	return 0;
}

int Zmotion::SetEncValue(int Value)
{
	return 0;
}

double Zmotion::GetEncPosMm()
{
	return 0.0;
}

double Zmotion::SetEncPosMm(double Value)
{
	return 0.0;
}

int Zmotion::GetLogicValue()
{
	return 0;
}

int Zmotion::SetLogicValue(int Value)
{
	return 0;
}

double Zmotion::GetLogicMm()
{
	return 0.0;
}

double Zmotion::SetLogicMm(double Value)
{
	return 0.0;
}

int Zmotion::GetAlm(bool& bAlm)
{
	return 0;
}

int Zmotion::SetEnableLimit(bool bEnable)
{
	return 0;
}

int Zmotion::GetEnableLimit()
{
	return 0;
}

int Zmotion::IsStop()
{
	return 0;
}

int Zmotion::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int Zmotion::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}
