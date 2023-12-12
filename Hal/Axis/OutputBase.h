#pragma once
#ifndef _OUTPUTBASE_H
#define _OUTPUTBASE_H

typedef struct tagOutputPara
{
	int deviceID;
	int bitNum;
	int flashTime;
	int polarity;  //输出极性，0为正常，1为反转
	int AcessRight;//更改权限
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

	virtual void Init(OutputPara& Para) = 0;  //初始化
	virtual int SetOn() = 0;				//输出位1
	virtual int SetOff() = 0;				//输出位0
	virtual void SetFlash() = 0;			//闪烁输出
	virtual int GetOutput() = 0;			//获取输出状态
	virtual void ClrFlash() = 0;			//停止闪烁
};
#endif