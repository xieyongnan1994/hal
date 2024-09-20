#include "CThreadManage.h"

CThreadManage::CThreadManage()
{
	ThreadFlg = FALSE;
	ThreadFinishEvt = CreateEventW(NULL, TRUE, FALSE, NULL);
}

CThreadManage::~CThreadManage()
{
}

int CThreadManage::CheckThreadExistOrNot()
{
	return ThreadFlg;
}

void CThreadManage::SetThreadExistFlg(int Flg)
{
	ThreadFlg = Flg;
}

void CThreadManage::SetFinishEvt()
{
	ThreadFlg = FALSE;
	SetEvent(ThreadFinishEvt);
}

void CThreadManage::Create(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nPriority, UINT nStackSize, DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
	if (ThreadFlg == FALSE)
	{
		ResetEvent(ThreadFinishEvt);
		ThreadFlg = TRUE;
		AfxBeginThread(pfnThreadProc, pParam, nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);
	}
}

int CThreadManage::CloseThread()
{
	if (ThreadFlg == TRUE)
	{
		ThreadFlg = FALSE;
		WaitForSingleObject(ThreadFinishEvt, INFINITE);
		ResetEvent(ThreadFinishEvt);
	}

	return TRUE;
}

int CThreadManage::WaitFinish(int ScanTime)
{
	if (WaitForSingleObject(ThreadFinishEvt, ScanTime) == WAIT_OBJECT_0)
	{
		ResetEvent(ThreadFinishEvt);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
