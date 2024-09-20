#pragma once
#ifndef _GOOGOL_H
#define _GOOGOL_H
#include "Axis/AxisBase.h"
#include "Axis/Googol/gts.h"

#define  MAXDEVICEID 8
#define  MAXAXISNO 8



class GTS_Init
{
public:
	~GTS_Init();
	static void  GTS_HardwareInit();		//Ӳ����ʼ����ֻ����һ��
	static void  GTS_HardwareRelease();	//Ӳ������ʼ����ֻ����һ��
	static GTS_Init* GetInstance();
private:
	GTS_Init();
	static GTS_Init* m_pInstance;
	static int m_nGTSInitFlag;
};

class Googol :public Axis
{
public:
	Googol();
	~Googol();

public:
	virtual void Init(AxisPara& Para);							//�����ó�ʼ��
	virtual int AxisEnable(int type = 0);						//��ʹ�� Ĭ��0ʹ��
	virtual int GetEncValue() ;
	virtual double GetEncPosMm();
	virtual int GetLogicValue();
	virtual double GetLogicMm();
	virtual int GetAlm(bool& bAlm);								//��ȡ����																		
	virtual int GetEnableLimit();								//��λ����
	virtual int SoftLmtEnable(bool bEnable);					//����λ����
	virtual int GetAxisPosLmt(bool& LmtValue);					//��ȡ����λ																			
	virtual int GetAxisNegLmt(bool& LmtValue);					//��ȡ����λ	
	virtual int GetHomeSignal(bool& HomeValue);					//��ȡԭ��
	virtual int GetAxisState();									//��ȡ��״̬
	virtual int Stop(int type = 0);								//ֹͣ�˶�

	virtual int SoftLmtPos();									//��������λ
	virtual int SetEncValue(int Value);							//���ñ�����	
	virtual double SetEncPosMm(double Value);
	virtual int SetLogicValue(int Value);						//�����߼�����
	virtual double SetLogicMm(double Value);
	virtual int SetEnableLimit(bool bEnable);					//������λʹ��																					
	virtual int MoveRel(int nPos, int nSpeedLevel = 100);		//����˶�
	virtual int MoveAbs(int nPos, int nSpeedLevel = 100);		//�����˶�
	virtual int Jog(int nDirection, int nSpeedLevel = 100);		//jog�˶�
	virtual int Home(BOOL bHighSpeed = TRUE);

};
#endif
