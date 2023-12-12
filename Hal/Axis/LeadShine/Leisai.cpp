#include "Leisai.h"

Leisai::Leisai()
{
}

Leisai::~Leisai()
{
}

int Leisai::MoveRel(int nPos, int nSpeedLevel)
{
	if (IsStop() == 1)
	{
		d1000_start_t_move(axisNo, nPos,0,nSpeedLevel,0.02);
	}
	return 0;
}

double Leisai::MoveRelMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Leisai::MoveRel(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Leisai::MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Leisai::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}

double Leisai::MoveAbsMm(double dPos, int nSpeedLevel)
{
	return 0.0;
}

int Leisai::MoveAbs(int nPos, int nSpeed, int nSpeedStart, int nSpeedAcc, int nSpeedDec)
{
	return 0;
}

double Leisai::MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc, double dSpeedDec)
{
	return 0.0;
}

int Leisai::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}

int Leisai::Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc, double nSpeedDec)
{
	return 0;
}

int Leisai::GetPos()
{
	return 0;
}

double Leisai::GetPosMm()
{
	return 0.0;
}

int Leisai::GetEncValue()
{
	return 0;
}

int Leisai::SetEncValue(int Value)
{
	return 0;
}

double Leisai::GetEncPosMm()
{
	return 0.0;
}

double Leisai::SetEncPosMm(double Value)
{
	return 0.0;
}

int Leisai::GetLogicValue()
{
	return 0;
}

int Leisai::SetLogicValue(int Value)
{
	return 0;
}

double Leisai::GetLogicMm()
{
	return 0.0;
}

double Leisai::SetLogicMm(double Value)
{
	return 0.0;
}

int Leisai::GetAlm(bool& bAlm)
{
	return 0;
}

int Leisai::SetEnableLimit(bool bEnable)
{
	return 0;
}

int Leisai::GetEnableLimit()
{
	return 0;
}

//返回值 0运动中... 1正常停止  2异常停止
int Leisai::IsStop()
{
	int nRet = 0;
	DWORD dwStatus = 0;
	dwStatus = d1000_check_done(axisNo);
	if (dwStatus == 0)
	{
		nRet = 0;
	}
	else if (dwStatus == 3)
	{
		nRet = 2;
	}
	else
	{
		nRet = 1;
	}
	return nRet;
}

int Leisai::GetAxisPosLmt(bool& LmtValue)
{
	return 0;
}

int Leisai::GetAxisNegLmt(bool& LmtValue)
{
	return 0;
}
