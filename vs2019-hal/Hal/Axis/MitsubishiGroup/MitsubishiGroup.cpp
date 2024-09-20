#include "MitsubishiGroup.h"

MitsubishiGroup::MitsubishiGroup()
{
}

MitsubishiGroup::~MitsubishiGroup()
{
}

int MitsubishiGroup::MoveRel(int nPos, int nSpeedLevel)
{
	return 0;
}

double MitsubishiGroup::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int MitsubishiGroup::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double MitsubishiGroup::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int MitsubishiGroup::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double MitsubishiGroup::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int MitsubishiGroup::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double MitsubishiGroup::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int MitsubishiGroup::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int MitsubishiGroup::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int MitsubishiGroup::GetPos()
{
	return 0;
}

double MitsubishiGroup::GetPosMm()
{
	return 0.0;
}

int MitsubishiGroup::GetEncValue()
{
	return 0;
}

int MitsubishiGroup::SetEncValue(int Value)
{
	return 0;
}

double MitsubishiGroup::GetEncPosMm()
{
	return 0.0;
}

double MitsubishiGroup::SetEncPosMm(double Value)
{
	return 0.0;
}

int MitsubishiGroup::GetLogicValue()
{
	return 0;
}

int MitsubishiGroup::SetLogicValue(int Value)
{
	return 0;
}

double MitsubishiGroup::GetLogicMm()
{
	return 0.0;
}

double MitsubishiGroup::SetLogicMm(double Value)
{
	return 0.0;
}

int MitsubishiGroup::GetAlm(bool& bAlm)
{
	return 0;
}

int MitsubishiGroup::SetEnableLimit(bool bEnable)
{
	return 0;
}

int MitsubishiGroup::GetEnableLimit()
{
	return 0;
}

int MitsubishiGroup::IsStop()
{
	return 0;
}

int MitsubishiGroup::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int MitsubishiGroup::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}
