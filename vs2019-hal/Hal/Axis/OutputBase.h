#pragma once
#ifndef _OUTPUTBASE_H
#define _OUTPUTBASE_H

typedef struct tagOutputPara
{
	int deviceID;
	int bitNum;
	int flashTime;
	int polarity;  //������ԣ�0Ϊ������1Ϊ��ת
	int AcessRight;//����Ȩ��
}OutputPara;

class OutputBase
{
public:
	OutputBase()
	{
		m_Para.deviceID = -1;
		m_Para.bitNum = -1;
		m_Para.flashTime = -1;
		m_Para.polarity = 0;
	}

	virtual ~OutputBase()
	{

	}
protected:
	OutputPara m_Para;
public:
	int GetDeviceID()
	{
		return m_Para.deviceID;
	}
	void SetDeviceID(int ID)
	{
		m_Para.deviceID = ID;
	}

	int GetBitNum()
	{
		return m_Para.bitNum;
	}
	void SetBitNum(int Num)
	{
		m_Para.bitNum = Num;
	}

	int GetFlashTime()
	{
		return m_Para.flashTime;
	}
	void SetFlashTime(int TimeValue)
	{
		m_Para.flashTime = TimeValue;
	}

	void GetPara(OutputPara& Para)
	{
		Para = m_Para;
	}

	void SetPara(OutputPara& Para)
	{
		m_Para = Para;
	}

	virtual void Init(OutputPara& Para) = 0;  //��ʼ��
	virtual int SetOn() = 0;				//���λ1
	virtual int SetOff() = 0;				//���λ0
	virtual void SetFlash() = 0;			//��˸���
	virtual int GetOutput() = 0;			//��ȡ���״̬
	virtual void ClrFlash() = 0;			//ֹͣ��˸
};
#endif