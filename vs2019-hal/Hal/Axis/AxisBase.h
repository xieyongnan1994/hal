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
		deviceID = -1;       //����
		axisNo = -1;         //���
		moveType = -1;       //�˶�ģʽ��0��ԣ�1����
		speed = -1;		  //�ƶ��ٶ�
		speed2 = -1;
		AccTime = 0.001;
		DecTime = 0.001;
		acc = -1;            //�ƶ����ٶ�
		jac = -1;            //�ƶ��Ӽ��ٶ�
		homedir = 0;		  //���㷽��
		homeSpeedHigh = -1;  //�������
		homeSpeedLow = -1;   //�������
		homeOffset = 0;	  //����ƫ��
		gearNum = -1;        //���ֱȷ���
		gearDen = -1;        //���ֱȷ�ĸ
		homeMode = -1;		  //����ģʽ
		EncoderMode = 0;
		NegLimit = -1;
		PosLimit = -1;
		AlmMode = 0;
	}

	int deviceID;			//����
	int axisNo;			//���
	int moveType;			//�˶�ģʽ��0:��ԣ�1:����
	double speed;			//�ƶ��ٶ�
	double speed2;			//�ڶ��ƶ��ٶ�
	double jogSpeed;		//Jog�˶��ٶ�
	double AccTime;        //��ʱ��
	double DecTime;		//��ʱ��
	double acc;            //�ƶ����ٶ�
	double dec;			//���ٶ�
	double jac;            //�ƶ��Ӽ��ٶ�
	double jec;			//������
	int homedir;			//���㷽��
	double homeSpeedHigh;  //�������
	double homeSpeedLow;   //�������
	double homeOffset;		//����ƫ��
	int homeMode;			//����ģʽ
	double gearNum;		//���ֱȷ���
	double gearDen;		//���ֱȷ�ĸ
	int AlmMode;			//ALM
	int EncoderMode;		//EncoderMode
	double NegLimit;		//���������
	double PosLimit;		//���������
	int AcessRight;		//����Ȩ��
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
	��ʼ����
	������1.Para :�����
	����ֵ������
	*******************************************************************/
	virtual void Init(AxisPara& Para) = 0;

	virtual int AxisEnable(int type = 0) = 0;

	/*******************************************************************
	int GetEncValue()
	��ȡ��������ֵ
	������1.Value :������ֵ���
	����ֵ������
	*******************************************************************/
	virtual int GetEncValue() = 0;
	virtual double GetEncPosMm() = 0;
	/*******************************************************************
	int GetLogicValue()
	��ȡ�߼������ֵ
	������1.Value :�߼�����ֵ���
	����ֵ������
	*******************************************************************/												
	virtual int GetLogicValue() = 0;			
	virtual double GetLogicMm() = 0;
	virtual int GetAlm(bool& bAlm) = 0;								//��ȡ����																		
	virtual int GetEnableLimit() = 0;								//��λ����
	virtual int SoftLmtEnable(bool bEnable) = 0;					//����λ����
	virtual int GetAxisPosLmt(bool& LmtValue) = 0;					//��ȡ����λ																			
	virtual int GetAxisNegLmt(bool& LmtValue) = 0;					//��ȡ����λ	
	virtual int GetHomeSignal(bool& HomeValue) = 0;					//��ȡԭ��
	virtual int GetAxisState() = 0;									//��ȡ��״̬
	virtual int Stop(int type = 0) = 0;								//ֹͣ�˶�

	virtual int SoftLmtPos() = 0;									//��������λ
	virtual int SetEncValue(int Value) = 0;							//���ñ�����	
	virtual double SetEncPosMm(double Value) = 0;
	virtual int SetLogicValue(int Value) = 0;						//�����߼�����
	virtual double SetLogicMm(double Value) = 0;
	virtual int SetEnableLimit(bool bEnable) = 0;					//������λʹ��																					
	virtual int MoveRel(int nPos, int nSpeedLevel = 100) = 0;		//����˶�
	virtual int MoveAbs(int nPos, int nSpeedLevel = 100) = 0;		//�����˶�
	virtual int Jog(int nDirection, int nSpeedLevel = 100) = 0;		//jog�˶�
	virtual int Home(BOOL bHighSpeed = TRUE) = 0;

	virtual double GetAxisPos()					//������λ�ã�������ӳ��ֱȺ��λ��
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
		int deviceID;	//����
		int axisNo;		//���
		double gearDen;	//���ֱȷ�ĸ

		virtual int MoveRel(int nPos, int nSpeedLevel = 100) = 0;  //����˶�
		virtual double MoveRelMm(double dPos, int nSpeedLevel = 100) = 0;
		virtual int MoveRel(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1) = 0;
		virtual double MoveRelMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0) = 0;		//����˶�

		virtual int MoveAbs(int nPos, int nSpeedLevel = 100) = 0;  //�����˶�
		virtual double MoveAbsMm(double dPos, int nSpeedLevel = 100) = 0;
		virtual int MoveAbs(int nPos, int nSpeed = 100, int nSpeedStart = 0, int nSpeedAcc = 0.1, int nSpeedDec = 0.1) = 0;
		virtual double MoveAbsMm(double dPos, double dSpeed, double dSpeedStart, double dSpeedAcc = 0, double dSpeedDec = 0) = 0;		//�����˶�

		virtual int Jog(int nDirection, int nSpeedLevel = 100) = 0; //jog�˶�
		virtual int Jog(int nDirection, double nSpeed, double nSpeedStart, double nSpeedAcc = 0, double nSpeedDec = 0) = 0;			//�㶯Dir����0Ϊ����1Ϊ��

		virtual int GetPos() = 0;																									//��ȡ��ǰλ��
		virtual double GetPosMm() = 0;																								//��ȡ��ǰλ��

		virtual int GetEncValue() = 0;			//��ȡ������																			
		virtual int SetEncValue(int Value) = 0;
		virtual double GetEncPosMm() = 0;
		virtual double SetEncPosMm(double Value) = 0;

		virtual int GetLogicValue() = 0;		//��ȡ�߼�����																			
		virtual int SetLogicValue(int Value) = 0;
		virtual double GetLogicMm() = 0;
		virtual double SetLogicMm(double Value) = 0;

		virtual int GetAlm(bool& bAlm) = 0;		//��ȡ����																					

		virtual  int SetEnableLimit(bool bEnable) = 0;		//������λʹ��													
		virtual  int GetEnableLimit() = 0;																				

		virtual int IsStop() = 0;				//�Ƿ�ֹͣ																					

		virtual int GetAxisPosLmt(bool& LmtValue) = 0;	//��ȡ����λ																			
		virtual int GetAxisNegLmt(bool& LmtValue) = 0;	//��ȡ����λ																			

};

#endif // ! _AXISBASE_H