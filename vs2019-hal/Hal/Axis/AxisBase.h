#pragma once
#ifndef _AXISBASE_H
#define _AXISBASE_H
#include <stdlib.h>
#include <windows.h>
#include <atlstr.h>

typedef struct tagAxisPara
{
	tagAxisPara()
	{
		deviceID = -1;       //卡号
		axisNo = -1;         //轴号
		moveType = -1;       //运动模式，0相对，1绝对
		speed = -1;		  //移动速度
		speed2 = -1;
		AccTime = 0.001;
		DecTime = 0.001;
		acc = -1;            //移动加速度
		jac = -1;            //移动加加速度
		homedir = 0;		  //回零方向
		homeSpeedHigh = -1;  //回零高速
		homeSpeedLow = -1;   //回零低速
		homeOffset = 0;	  //回零偏移
		gearNum = -1;        //齿轮比分子
		gearDen = -1;        //齿轮比分母
		homeMode = -1;		  //回零模式
		EncoderMode = 0;
		NegLimit = -1;
		PosLimit = -1;
		AlmMode = 0;
	}

	int deviceID;			//卡号
	int axisNo;			//轴号
	int moveType;			//运动模式，0:相对，1:绝对
	double speed;			//移动速度
	double speed2;			//第二移动速度
	double jogSpeed;		//Jog运动速度
	double AccTime;        //加时间
	double DecTime;		//减时间
	double acc;            //移动加速度
	double dec;			//减速度
	double jac;            //移动加加速度
	double jec;			//减减速
	int homedir;			//回零方向
	double homeSpeedHigh;  //回零高速
	double homeSpeedLow;   //回零低速
	double homeOffset;		//回零偏移
	int homeMode;			//回零模式
	double gearNum;		//齿轮比分子
	double gearDen;		//齿轮比分母
	int AlmMode;			//ALM
	int EncoderMode;		//EncoderMode
	double NegLimit;		//软件负极限
	double PosLimit;		//软件正极限
	int AcessRight;		//更改权限
}AxisPara;


class Axis
{
public:
	Axis()
	{
	}
	virtual ~Axis()
	{
	}
protected:
	AxisPara m_AxisPara;

protected:
	double m_LogicOld;
	double m_EncOld;
	bool m_PosLmtOld;
	bool m_NegLmtOld;
	bool m_HomeOld;
#pragma region Property
public:
	int GetDeviceID()
	{
		return m_AxisPara.deviceID;
	}
	void SetDeviceID(int ID)
	{
		m_AxisPara.deviceID = ID;
	}

	int GetAxisNo()
	{
		return m_AxisPara.axisNo;
	}
	void SetAxisNo(int Num)
	{
		m_AxisPara.axisNo = Num;
	}

	int GetMoveType()
	{
		return m_AxisPara.moveType;
	}
	void SetMoveType(int MoveType)
	{
		m_AxisPara.moveType = MoveType;
	}

	double GetSpeed()
	{
		return m_AxisPara.speed;
	}
	void SetSpeed(double Speed)
	{
		m_AxisPara.speed = Speed;
	}

	double GetSpeed2()
	{
		return m_AxisPara.speed2;
	}
	void SetSpeed2(double Speed)
	{
		m_AxisPara.speed2 = Speed;
	}

	void SetJogSpeed(double Speed)
	{
		m_AxisPara.jogSpeed = Speed;
	}

	double GetJogSpeed()
	{
		return m_AxisPara.jogSpeed;
	}

	double GetAcc()
	{
		return m_AxisPara.acc;
	}
	void SetAcc(double Acc)
	{
		m_AxisPara.acc = Acc;
	}

	double GetDec()
	{
		return m_AxisPara.dec;
	}

	void SetDec(double Value)
	{
		m_AxisPara.dec = Value;
	}

	double GetJac()
	{
		return m_AxisPara.jac;
	}
	void SetJac(double Jac)
	{
		m_AxisPara.jac = Jac;
	}

	double GetJec()
	{
		return m_AxisPara.jec;
	}

	double SetJec(double Value)
	{
		m_AxisPara.jec = Value;
	}

	int GetHomeDir()
	{
		return m_AxisPara.homedir;
	}
	void SetHomeDir(int dir)
	{
		m_AxisPara.homedir = dir;
	}


	double GetHomeSpeedHigh()
	{
		return m_AxisPara.homeSpeedHigh;
	}
	void SetHomeSpeedHigh(double HighSpeed)
	{
		m_AxisPara.homeSpeedHigh = HighSpeed;
	}

	double GetHomeSpeedLow()
	{
		return m_AxisPara.homeSpeedLow;
	}
	void SetHomeSpeedLow(double LowSpeed)
	{
		m_AxisPara.homeSpeedLow = LowSpeed;
	}

	double GetHomeOffset()
	{
		return m_AxisPara.homeOffset;
	}

	void SetHomeOffset(double Offset)
	{
		m_AxisPara.homeOffset = Offset;
	}

	double GetGearNum()
	{
		return m_AxisPara.gearNum;
	}
	void SetGearNum(double GearNum)
	{
		m_AxisPara.gearNum = GearNum;
	}

	double GetGearDen()
	{
		return m_AxisPara.gearDen;
	}
	void SetGearDen(double GearDen)
	{
		m_AxisPara.gearDen = GearDen;
	}

	double GetGear()
	{
		double rslt;
		rslt = m_AxisPara.gearNum / m_AxisPara.gearDen;
		return rslt;
	}

	void SetHomeMode(int Mode)
	{
		m_AxisPara.homeMode = Mode;
	}

	int GetHomeMode()
	{
		return m_AxisPara.homeMode;
	}

	double GetSoftNegLimit()
	{
		return m_AxisPara.NegLimit;
	}

	void SetSoftNegLimit(double limit)
	{
		m_AxisPara.NegLimit = limit;
	}

	double GetSoftPosLimit()
	{
		return m_AxisPara.PosLimit;
	}

	void SetSoftPosLimit(double limit)
	{
		m_AxisPara.PosLimit = limit;
	}

	double GetAccTime()
	{
		return m_AxisPara.AccTime;
	}

	void SetAccTime(double time)
	{
		m_AxisPara.AccTime = time;
	}

	double GetDecTime()
	{
		return m_AxisPara.DecTime;
	}

	void SetDecTime(double time)
	{
		m_AxisPara.DecTime = time;
	}
	int GetAxisAlmMode()
	{
		return m_AxisPara.AlmMode;
	}

	int GetAxisEncoderMode()
	{
		return m_AxisPara.EncoderMode;
	}
	void GetPara(AxisPara& Para)
	{
		Para = m_AxisPara;
	}

	void SetPara(AxisPara Para)
	{
		m_AxisPara = Para;
	}

#pragma endregion Property

#pragma region method

	/*******************************************************************
	void Init(AxisPara &Para);
	初始化轴
	参数：1.Para :轴参数
	返回值：暂无
	*******************************************************************/
	virtual void Init(AxisPara& Para) = 0;

	virtual int AxisEnable(int type = 0) = 0;

	/*******************************************************************
	int GetEncValue()
	获取编码器的值
	参数：1.Value :编码器值输出
	返回值：暂无
	*******************************************************************/
	virtual int GetEncValue() = 0;
	virtual double GetEncPosMm() = 0;
	/*******************************************************************
	int GetLogicValue()
	获取逻辑脉冲的值
	参数：1.Value :逻辑脉冲值输出
	返回值：暂无
	*******************************************************************/												
	virtual int GetLogicValue() = 0;			
	virtual double GetLogicMm() = 0;
	virtual int GetAlm(bool& bAlm) = 0;								//获取报警																		
	virtual int GetEnableLimit() = 0;								//限位功能
	virtual int SoftLmtEnable(bool bEnable) = 0;					//软限位功能
	virtual int GetAxisPosLmt(bool& LmtValue) = 0;					//获取正限位																			
	virtual int GetAxisNegLmt(bool& LmtValue) = 0;					//获取负限位	
	virtual int GetHomeSignal(bool& HomeValue) = 0;					//获取原点
	virtual int GetAxisState() = 0;									//获取轴状态
	virtual int Stop(int type = 0) = 0;								//停止运动

	virtual int SoftLmtPos() = 0;									//设置软限位
	virtual int SetEncValue(int Value) = 0;							//设置编码器	
	virtual double SetEncPosMm(double Value) = 0;
	virtual int SetLogicValue(int Value) = 0;						//设置逻辑脉冲
	virtual double SetLogicMm(double Value) = 0;
	virtual int SetEnableLimit(bool bEnable) = 0;					//设置限位使能																					
	virtual int MoveRel(int nPos, int nSpeedLevel = 100) = 0;		//相对运动
	virtual int MoveAbs(int nPos, int nSpeedLevel = 100) = 0;		//绝对运动
	virtual int Jog(int nDirection, int nSpeedLevel = 100) = 0;		//jog运动
	virtual int Home(BOOL bHighSpeed = TRUE) = 0;

	virtual double GetAxisPos()					//返回轴位置，计算电子齿轮比后的位置
	{
		double Ret = 0;
		double Value;
		Value = GetLogicValue();
		Ret = (double)Value / GetGear();
		return Ret;
	}

	virtual double GetAbsOffset(double TarValue)
	{
		return 0;
	}
	virtual double GetRelOffset(double TarValue, int Dir)
	{
		return 0;
	}
	virtual BOOL LoadCalibrationData(CString Path)
	{
		return TRUE;
	}
	virtual BOOL WriteCalibrationData(CString Path = _T(""))
	{
		return TRUE;
	}

	virtual int GetAxisOffset()
	{
		return 0;
	}

	virtual void SetAxisOffset(int Offset)
	{
	}

	virtual void AxisClr()
	{
	}
#pragma endregion method
};

class AxisBase
{
	protected:
		int deviceID;	//卡号
		int axisNo;		//轴号
		double gearDen;	//齿轮比分母

		virtual int MoveRel(int nPos, int nSpeedLevel = 100) = 0;  //相对运动
		virtual double MoveRelMm(double dPos, int nSpeedLevel = 100) = 0;
		virtual int MoveRel(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1) = 0;
		virtual double MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0) = 0;		//相对运动

		virtual int MoveAbs(int nPos, int nSpeedLevel = 100) = 0;  //绝对运动
		virtual double MoveAbsMm(double dPos, int nSpeedLevel = 100) = 0;
		virtual int MoveAbs(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1) = 0;
		virtual double MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0) = 0;		//绝对运动

		virtual int Jog(int nDirection, int nSpeedLevel = 100) = 0; //jog运动
		virtual int Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc = 0, double nSpeedDec = 0) = 0;			//点动Dir方向：0为正，1为负

		virtual int GetPos() = 0;																									//获取当前位置
		virtual double GetPosMm() = 0;																								//获取当前位置

		virtual int GetEncValue() = 0;			//获取编码器																			
		virtual int SetEncValue(int Value) = 0;
		virtual double GetEncPosMm() = 0;
		virtual double SetEncPosMm(double Value) = 0;

		virtual int GetLogicValue() = 0;		//获取逻辑脉冲																			
		virtual int SetLogicValue(int Value) = 0;
		virtual double GetLogicMm() = 0;
		virtual double SetLogicMm(double Value) = 0;

		virtual int GetAlm(bool& bAlm) = 0;		//获取报警																					

		virtual  int SetEnableLimit(bool bEnable) = 0;		//设置限位使能													
		virtual  int GetEnableLimit() = 0;																				

		virtual int IsStop() = 0;				//是否停止																					

		virtual int GetAxisPosLmt(bool& LmtValue) = 0;	//获取正限位																			
		virtual int GetAxisNegLmt(bool& LmtValue) = 0;	//获取负限位																			

};

#endif // ! _AXISBASE_H