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
	bool CreateMemoryCourse(CString strcourname = _T("ServerMemory.exe")); //创建新的进程
	void CloseClientCoerse(CString strcourname = _T("ServerMemory.exe"));  //关闭进程
	void SendLargeMessage(CString strServer);   //界面控件发送大量数据使用
	
	bool CreateMemory(CString strmename);		//创建共享内存	
	int WriteToMemory(CString strdata);         //写入数据到共享内存
	CString ReadMemory(CString strdata);		//获取共享内存数据
	void CloseMemory();							//关闭共享内存句柄


private:
	CString m_edit1;  //定义传输大量数据主界面控件变量
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
