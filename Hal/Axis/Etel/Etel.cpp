#include "Etel.h"

Etel::Etel()
{
}

Etel::~Etel()
{
}

int Etel::MoveRel(int nPos, int nSpeedLevel)
{
	return 0;
}

double Etel::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Etel::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Etel::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Etel::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double Etel::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Etel::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Etel::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Etel::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int Etel::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int Etel::GetPos()
{
	return 0;
}

double Etel::GetPosMm()
{
	return 0.0;
}

int Etel::GetEncValue()
{
	return 0;
}

int Etel::SetEncValue(int Value)
{
	return 0;
}

double Etel::GetEncPosMm()
{
	return 0.0;
}

double Etel::SetEncPosMm(double Value)
{
	return 0.0;
}

int Etel::GetLogicValue()
{
	return 0;
}

int Etel::SetLogicValue(int Value)
{
	return 0;
}

double Etel::GetLogicMm()
{
	return 0.0;
}

double Etel::SetLogicMm(double Value)
{
	return 0.0;
}

int Etel::GetAlm(bool& bAlm)
{
	return 0;
}

int Etel::SetEnableLimit(bool bEnable)
{
	return 0;
}

int Etel::GetEnableLimit()
{
	return 0;
}

int Etel::IsStop()
{
	return 0;
}

int Etel::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int Etel::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}

