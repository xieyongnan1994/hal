#include "Adlink.h"

Adlink::Adlink()
{
}

Adlink::~Adlink()
{
}

int Adlink::MoveRel(int nPos, int nSpeedLevel)
{
	return 0;
}

double Adlink::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Adlink::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Adlink::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Adlink::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double Adlink::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Adlink::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Adlink::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Adlink::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int Adlink::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int Adlink::GetPos()
{
	return 0;
}

double Adlink::GetPosMm()
{
	return 0.0;
}

int Adlink::GetEncValue()
{
	return 0;
}

int Adlink::SetEncValue(int Value)
{
	return 0;
}

double Adlink::GetEncPosMm()
{
	return 0.0;
}

double Adlink::SetEncPosMm(double Value)
{
	return 0.0;
}

int Adlink::GetLogicValue()
{
	return 0;
}

int Adlink::SetLogicValue(int Value)
{
	return 0;
}

double Adlink::GetLogicMm()
{
	return 0.0;
}

double Adlink::SetLogicMm(double Value)
{
	return 0.0;
}

int Adlink::GetAlm(bool& bAlm)
{
	return 0;
}

int Adlink::SetEnableLimit(bool bEnable)
{
	return 0;
}

int Adlink::GetEnableLimit()
{
	return 0;
}

int Adlink::IsStop()
{
	return 0;
}

int Adlink::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int Adlink::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}
