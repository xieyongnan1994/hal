#pragma once
#ifndef _INPUTBASE_H
#define _INPUTBASE_H
#include <stdlib.h>
#include <windows.h>
#include <atlstr.h>

typedef struct tagInputPara
{
	int deviceID;
	int bitNum;
	int polarity;  //极性，0为经常，低电平有效，1为反转，高电平有效
	int warnType;  //报警类型,0为不报警，1为上升沿报警，2为下降沿报警
	int useornot;	//是否使用
	CString Msg;   //报警内容
	int AcessRight;//更改权限
}InputPara;
class InputBase
{
public:
	InputBase()
	{
		m_Para.deviceID = -1;
		m_Para.bitNum = -1;
		m_Para.polarity = 0;  //默认为正常极性
		m_Para.warnType = 0;
		m_Para.useornot = 1;
		m_Para.Msg = _T("");
	}

	virtual ~InputBase()
	{

	}
protected:
	InputPara m_Para;

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

	void GetPara(InputPara& Para)
	{
		Para = m_Para;
	}

	void SetPara(InputPara& Para)
	{
		m_Para = Para;
	}
	bool CheckUseOrNot()
	{
		if (m_Para.bitNum == -1)
		{
			return false;
		}
		if (m_Para.useornot == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

public:
	virtual void Init(InputPara& Para) = 0;		//初始化
	virtual int GetInput(bool& InputValue) = 0;	//获取输入状态,返回值：0为没变化，1为上升沿，2为下降沿
	virtual bool GetInput()
	{
		bool Ret;
		this->GetInput(Ret);
		if (Ret == true)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
};
#endif
