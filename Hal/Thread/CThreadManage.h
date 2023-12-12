#pragma once
#ifndef _CTHREADMANAGE_H
#define _CTHREADMANAGE_H

#include <afxwin.h>

class CThreadManage
{
public:
	CThreadManage();
	~CThreadManage();

public:
	int CheckThreadExistOrNot();
	void SetThreadExistFlg(int Flg);
	void SetFinishEvt();
	void Create(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
	int CloseThread();
	int WaitFinish(int ScanTime);
private:
	int ThreadFlg;
	HANDLE ThreadFinishEvt;

};
#endif // !_CTHREADMANAGE_H

