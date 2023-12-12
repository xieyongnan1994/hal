#include "Googol.h"

GTS_Init* GTS_Init::m_pInstance = NULL;
int GTS_Init::m_nGTSInitFlag = FALSE;

GTS_Init::GTS_Init()
{
}

GTS_Init::~GTS_Init()
{
}

void GTS_Init::GTS_HardwareInit()
{
	if (m_nGTSInitFlag == FALSE)
	{
		m_nGTSInitFlag = TRUE;
		for (int i = 0; i < MAXDEVICEID; i++)
		{
			GT_Open(i);
			GT_Reset(i);
			GT_LoadConfig(i, "test.cfg");
			GT_ClrSts(i, 1, 8);
		}
	}
}

void GTS_Init::GTS_HardwareRelease()
{
	for (int i = 0; i < MAXDEVICEID; i++)
	{
		for (int j = 0; j < MAXAXISNO; j++)
		{
			GT_AxisOff(i,j);
			GT_Reset(i);
			GT_Close(i);
		}
	}
}

GTS_Init* GTS_Init::GetInstance()
{

	if (NULL == m_pInstance)
	{
		m_pInstance = new GTS_Init();
	}
	return m_pInstance;
}

Googol::Googol()
{
}

Googol::~Googol()
{
}

void Googol::Init(AxisPara& Para)				//轴配置初始化
{
	this->SetPara(Para);
}

int Googol::AxisEnable(int type)
{
	if (type == 0)
	{
		GT_AxisOn(GetDeviceID(),GetAxisNo());
		return 1;
	}
	else
	{
		GT_AxisOff(GetDeviceID(),GetAxisNo());
	}
	return 0;
}

int Googol::GetEncValue()
{
	int nRet;
	double dVal;
	GT_GetAxisEncPos(GetDeviceID(), GetAxisNo(), &dVal);
	nRet = (int)dVal;
	return nRet;
}

double Googol::GetEncPosMm()
{
	double dVal;
	GT_GetAxisEncPos(GetDeviceID(), GetAxisNo(), &dVal);
	dVal = dVal / GetGear();
	return dVal;
}

int Googol::GetLogicValue()
{
	int nRet;
	double dVal;
	GT_GetAxisEncPos(GetDeviceID(), GetAxisNo(), &dVal);
	nRet = (int)dVal;
	return nRet;
}

double Googol::GetLogicMm()
{
	double dVal;
	GT_GetAxisEncPos(GetDeviceID(), GetAxisNo(), &dVal);
	dVal = dVal / GetGear();
	return dVal;
}

int Googol::GetAlm(bool& bAlm)
{
	short sRtn;
	long lAxisStatus;
	sRtn = GT_GetSts(GetDeviceID(), GetAxisNo(), &lAxisStatus);
	if (lAxisStatus & 0x2)
	{
		bAlm = true;
		return 1;
	}
	
	bAlm = false;
	return 0;
}
int Googol::GetEnableLimit()
{
	return 0;
}
int Googol::GetAxisState()		//返回值 0运动中... 1正常停止  2异常停止
{
	short sRtn;
	long lAxisStatus;
	sRtn = GT_GetSts(GetDeviceID(), GetAxisNo(), &lAxisStatus);
	// 伺服报警标志  急停标志 正向限位 负向限位  跟随误差越限标志
	if ((lAxisStatus & 0x2)||(lAxisStatus & 0x100)||
		(lAxisStatus & 0x20)|| (lAxisStatus & 0x40)||
		(lAxisStatus & 0x10))
	{
		return 2;
	}
	// 平滑停止
	if (lAxisStatus & 0x80)
	{
		return 1;
	}
	// 规划器正在运动标志
	if (lAxisStatus & 0x400)
	{
		return 0;
	}
}

int Googol::GetAxisPosLmt(bool& LmtValue)		
{
	short sRtn;
	long lAxisStatus;
	sRtn = GT_GetSts(GetDeviceID(), GetAxisNo(), &lAxisStatus);
	if (lAxisStatus & 0x20)
	{
		LmtValue = true;
		return 1;
	}
	LmtValue = false;
	return 0;
}
int Googol::GetAxisNegLmt(bool& LmtValue)		
{
	short sRtn;
	long lAxisStatus;
	sRtn = GT_GetSts(GetDeviceID(), GetAxisNo(), &lAxisStatus);
	if (lAxisStatus & 0x40)
	{
		LmtValue = true;
		return 1;
	}
	LmtValue = false;
	return 0;
}
int Googol::GetHomeSignal(bool& HomeValue)	
{
	int nRet = FALSE;

	return nRet;
}
int Googol::Home(BOOL bHighSpeed)
{
	int nRet = FALSE;

	return nRet;
}
int Googol::Stop(int type )
{
	int nRet;
	short sRtn;
	if (type == 0) 
	{
		//平滑停止对应的轴
		sRtn = GT_Stop(GetDeviceID(), 1 << (GetAxisNo() - 1), 0 << (GetAxisNo() - 1));
		nRet = 1;
	}
	else
	{
		//急停对应的轴
		sRtn = GT_Stop(GetDeviceID(), 1 << (GetAxisNo() - 1), 1 << (GetAxisNo() - 1));
		nRet = 2;
	}
	return nRet;
}
int Googol::SoftLmtPos()
{
	int nRet = FALSE;

	return nRet;
}
int Googol::SetEncValue(int Value)
{
	if (GetAxisState() == 1)
	{
		GT_SetEncPos(GetDeviceID(),GetAxisNo(),Value);
		return 1;
	}
	return 0;
}
double Googol::SetEncPosMm(double Value)
{
	double dVal;
	dVal = Value / GetGear();
	if (GetAxisState() == 1)
	{
		GT_SetEncPos(GetDeviceID(), GetAxisNo(), dVal);
		return 1;
	}
	return 0.0;
}
int Googol::SetLogicValue(int Value)
{
	if (GetAxisState() == 1)
	{
		GT_SetEncPos(GetDeviceID(), GetAxisNo(), Value);
		return 1;
	}
	return 0;
}
double Googol::SetLogicMm(double Value)
{
	double dVal;
	dVal = Value / GetGear();
	if (GetAxisState() == 1)
	{
		GT_SetEncPos(GetDeviceID(), GetAxisNo(), dVal);
		return 1;
	}
	return 0.0;
}
int Googol::SetEnableLimit(bool bEnable)
{
	int nRet;
	if (bEnable == true)
	{
		GT_LmtsOn(GetDeviceID(), GetAxisNo());
		nRet = 1;
	}
	else
	{
		GT_LmtsOff(GetDeviceID(), GetAxisNo());
		nRet = 0;
	}
	return nRet;
}
int Googol::MoveRel(int nPos, int nSpeedLevel)
{
	short sRtn;
	TTrapPrm trap;
	GT_PrfTrap(GetDeviceID(), GetAxisNo());
	GT_SetPos(GetDeviceID(), GetAxisNo(), nPos);
	GT_SetVel(GetDeviceID(), GetAxisNo(), nSpeedLevel);
	GT_Update(GetDeviceID(), 1 << (GetAxisNo() - 1));
	return 0;
}
int Googol::MoveAbs(int nPos, int nSpeedLevel)
{
	return 0;
}
int Googol::Jog(int nDirection, int nSpeedLevel)
{
	return 0;
}
int Googol::SoftLmtEnable(bool bEnable)
{
	int nRet = FALSE;

	return nRet;

}

