#pragma once
#ifndef _CSHAREDMEMORY_H
#define _CSHAREDMEMORY_H

#include <afxwin.h>
#include <iostream>
using namespace std;

#define DATALENGTH 128

class CSharedMemory
{
public:
	CSharedMemory();
	~CSharedMemory();

public:
	bool CreateMemoryCourse(CString strcourname = _T("ServerMemory.exe")); //�����µĽ���
	void CloseClientCoerse(CString strcourname = _T("ServerMemory.exe"));  //�رս���
	void SendLargeMessage(CString strServer);   //����ؼ����ʹ�������ʹ��
	
	bool CreateMemory(CString strmename);		//���������ڴ�	
	int WriteToMemory(CString strdata);         //д�����ݵ������ڴ�
	CString ReadMemory(CString strdata);		//��ȡ�����ڴ�����
	void CloseMemory();							//�رչ����ڴ���


private:
	CString m_edit1;  //���崫���������������ؼ�����
	HANDLE m_hMap;
	HANDLE hDataMutex;

public:
	double GetD(int Index);
	BOOL SetD(int Index, double Value);

	BOOL GetGBit(int Index, int BitNum);
	void SetGBit(int Index, int BitNum);
	void ClrGBit(int Index, int BitNum);
	void MoveGData(int Index, unsigned char Value);
	unsigned char GetGData(int Index);
	void SetRiseG(int Index, int BitNum);
	BOOL GetRiseG(int Index, int BitNum);

	unsigned char* GetGAddr();

private:
	void SetBit(unsigned char* buff, int Index, int BitNum);
	void ClrBit(unsigned char* buff, int Index, int BitNum);
	BOOL GetBit(unsigned char* buff, int Index, int BitNum);
	void Movn(unsigned char* buff, int Index, unsigned char Num);
	unsigned char Back(unsigned char* buff, int Index);

private:
	double D_R[DATALENGTH];
	unsigned char G_R[DATALENGTH];
	CRITICAL_SECTION m_BitLock1;
	CRITICAL_SECTION m_BitLock2;
	CRITICAL_SECTION m_BitLock3;

};

#endif // !_SERVERMEMORY_H
