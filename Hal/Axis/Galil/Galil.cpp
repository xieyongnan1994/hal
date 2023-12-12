#include "Galil.h"

Galil::Galil()
{
}

Galil::~Galil()
{
}

int Galil::MoveRel(int nPos, int nSpeedLevel)
{
	return 0;
}

double Galil::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Galil::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Galil::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Galil::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double Galil::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Galil::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Galil::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Galil::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int Galil::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int Galil::GetPos()
{
	return 0;
}

double Galil::GetPosMm()
{
	return 0.0;
}

int Galil::GetEncValue()
{
	return 0;
}

int Galil::SetEncValue(int Value)
{
	return 0;
}

double Galil::GetEncPosMm()
{
	return 0.0;
}

double Galil::SetEncPosMm(double Value)
{
	return 0.0;
}

int Galil::GetLogicValue()
{
	return 0;
}

int Galil::SetLogicValue(int Value)
{
	return 0;
}

double Galil::GetLogicMm()
{
	return 0.0;
}

double Galil::SetLogicMm(double Value)
{
	return 0.0;
}

int Galil::GetAlm(bool& bAlm)
{
	return 0;
}

int Galil::SetEnableLimit(bool bEnable)
{
	return 0;
}

int Galil::GetEnableLimit()
{
	return 0;
}

int Galil::IsStop()
{
	return 0;
}

int Galil::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int Galil::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}

