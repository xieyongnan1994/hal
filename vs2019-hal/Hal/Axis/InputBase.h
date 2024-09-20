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
	int polarity;  //���ԣ�0Ϊ�������͵�ƽ��Ч��1Ϊ��ת���ߵ�ƽ��Ч
	int warnType;  //��������,0Ϊ��������1Ϊ�����ر�����2Ϊ�½��ر���
	int useornot;	//�Ƿ�ʹ��
	CString Msg;   //��������
	int AcessRight;//����Ȩ��
}InputPara;
class InputBase
{
public:
	InputBase()
	{
		m_Para.deviceID = -1;
		m_Para.bitNum = -1;
		m_Para.polarity = 0;  //Ĭ��Ϊ��������
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
	virtual void Init(InputPara& Para) = 0;		//��ʼ��
	virtual int GetInput(bool& InputValue) = 0;	//��ȡ����״̬,����ֵ��0Ϊû�仯��1Ϊ�����أ�2Ϊ�½���
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
