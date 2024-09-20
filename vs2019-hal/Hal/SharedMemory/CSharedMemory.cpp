#include "CSharedMemory.h"

/******************************************************************************************
Function:        ConvertLPWSTRToLPSTR
Description:     LPWSTR转char*
Input:           lpwszStrIn:待转化的LPWSTR类型
Return:          转化后的char*类型
*******************************************************************************************/
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	try
	{
		if (lpwszStrIn != NULL)
		{
			int nInputStrLen = wcslen(lpwszStrIn);

			// Double NULL Termination  
			int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
			pszOut = new char[nOutputStrLen];

			if (pszOut)
			{
				memset(pszOut, 0x00, nOutputStrLen);
				WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
			}
		}
	}
	catch (std::exception e)
	{
	}

	return pszOut;
}


CSharedMemory::CSharedMemory()
{
	hDataMutex = NULL;
	m_hMap = NULL;
	InitializeCriticalSection(&m_BitLock1);
	InitializeCriticalSection(&m_BitLock2);
	InitializeCriticalSection(&m_BitLock3);
}

CSharedMemory::~CSharedMemory()
{
	CloseHandle(hDataMutex);
	CloseHandle(m_hMap);
	DeleteCriticalSection(&m_BitLock1);
	DeleteCriticalSection(&m_BitLock2);
	DeleteCriticalSection(&m_BitLock3);
}

bool CSharedMemory::CreateMemoryCourse(CString strcourname)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	si.dwFlags = STARTF_USEPOSITION;
	si.dwX = 300;
	si.dwY = 500;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	bool fRet = CreateProcess(NULL,
		(LPWSTR)(LPCTSTR)strcourname,
		NULL, 
		NULL, 
		FALSE,	
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, 
		NULL, 
		NULL, 
		&si, 
		&pi);
	if (fRet != 0)
	{
		return true;
	}
	return false;
}

void CSharedMemory::CloseClientCoerse(CString strcourname)
{
	CWnd* pWnd = CWnd::FindWindow(NULL, strcourname);
	if (pWnd) 
	{
		pWnd->SendMessage(WM_CLOSE, 0, 0);
	}
}

void CSharedMemory::SendLargeMessage(CString strServer)
{
	CWnd* pWnd = CWnd::FindWindow(NULL, strServer);
	if (pWnd)
	{
		COPYDATASTRUCT buf;
		char* s = new char[m_edit1.GetLength()];   //m_edit1为CString类型的变量
		s = ConvertLPWSTRToLPSTR (m_edit1.GetBuffer(0));
		buf.cbData = strlen(s) + 1;
		buf.lpData = s;
		pWnd->SendMessage(WM_COPYDATA, 0, (LPARAM)&buf); //传送大量数据要用WM_COPYDATA消息
	}
}

bool CSharedMemory::CreateMemory(CString strmename)
{
	m_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 0x200, strmename);
	if (m_hMap != NULL)
	{
		return true;
	}
	return false;
}

int CSharedMemory::WriteToMemory(CString strdata)
{
	char str[1024];
	LPSTR  m_lpData;	
	//创建互斥锁
	hDataMutex =OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("WriteData Mutex"));
	if (hDataMutex == NULL)
	{
		hDataMutex = CreateMutex(NULL, TRUE,_T( "WriteData Mutex"));
	}
	WaitForSingleObject(hDataMutex, INFINITE);
	//将文件的视图映射到一个进程的地址空间上，返回LPVOID类型的内存指针
	m_lpData = (LPSTR)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_lpData == NULL)
	{
		return 0;
	}
	wsprintfA(str, "%ls", strdata);
	//给这段映像内存写数据
	sprintf(m_lpData, str);
	//解除文件映射
	UnmapViewOfFile(m_lpData);
	//关闭互斥锁
	ReleaseMutex(hDataMutex);
	return 1;
}

CString CSharedMemory::ReadMemory(CString strdata)
{
	CString str;
	LPSTR  m_lpData;
	hDataMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("ReadData Mutex"));
	if (hDataMutex == NULL)
	{
		hDataMutex = CreateMutex(NULL, TRUE, _T("ReadData Mutex"));
	}
	WaitForSingleObject(hDataMutex, INFINITE);
	//获取共享内存
	m_hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, strdata);
	if (m_hMap == NULL)
	{
		str = _T("获取共享内存失败！");
		ReleaseMutex(hDataMutex);
		return str;
	}
	//将文件的视图映射到一个进程的地址空间上，返回LPVOID类型的内存指针
	m_lpData = (LPSTR)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_lpData == NULL)
	{
		str = _T("共享内存映射失败！");
		ReleaseMutex(hDataMutex);
		return str;
	}
	str = m_lpData;
	//释放映像内存
	UnmapViewOfFile(m_lpData);
	//关闭互斥锁
	ReleaseMutex(hDataMutex);
	return str;
}

void CSharedMemory::CloseMemory()
{
	CloseHandle(m_hMap);
}

double CSharedMemory::GetD(int Index)
{
	return D_R[Index];
}

BOOL CSharedMemory::SetD(int Index, double Value)
{
	D_R[Index] = Value;
	return TRUE;
}

BOOL CSharedMemory::GetGBit(int Index, int BitNum)
{
	return GetBit(GetGAddr(), Index, BitNum);
}

void CSharedMemory::SetGBit(int Index, int BitNum)
{
	this->SetBit(GetGAddr(), Index, BitNum);
}

void CSharedMemory::ClrGBit(int Index, int BitNum)
{
	this->ClrBit(GetGAddr(), Index, BitNum);
}

void CSharedMemory::MoveGData(int Index, unsigned char Value)
{
	this->Movn(GetGAddr(), Index, Value);
}

unsigned char CSharedMemory::GetGData(int Index)
{
	return this->Back(GetGAddr(), Index);
}

void CSharedMemory::SetRiseG(int Index, int BitNum)
{
	SetGBit(Index, BitNum);
}

BOOL CSharedMemory::GetRiseG(int Index, int BitNum)
{
	if (GetGBit(Index, BitNum))
	{
		ClrGBit(Index, BitNum);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

unsigned char* CSharedMemory::GetGAddr()
{
	return G_R;
}

void CSharedMemory::SetBit(unsigned char* buff, int Index, int BitNum)
{
	EnterCriticalSection(&m_BitLock1);
	buff[Index] |= (1 << BitNum);
	LeaveCriticalSection(&m_BitLock1);
}

void CSharedMemory::ClrBit(unsigned char* buff, int Index, int BitNum)
{
	EnterCriticalSection(&m_BitLock2);
	buff[Index] &= ~(1 << BitNum);
	LeaveCriticalSection(&m_BitLock2);
}

BOOL CSharedMemory::GetBit(unsigned char* buff, int Index, int BitNum)
{
	BOOL bRet;
	EnterCriticalSection(&m_BitLock3);
	bRet = (buff[Index] & (1 << BitNum)) >> BitNum;
	LeaveCriticalSection(&m_BitLock3);
	return bRet;
}

void CSharedMemory::Movn(unsigned char* buff, int Index, unsigned char Num)
{
	buff[Index] = Num;
}

unsigned char CSharedMemory::Back(unsigned char* buff, int Index)
{
	return buff[Index];
}
