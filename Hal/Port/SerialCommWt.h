#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Name: CSerialComm
// Version: 1.0
// Comment: This class is responsible for provide I/O operation with
// a serial port. It is implemented with Synchronous I/O viz. both
// input and output operations block. Both read and write operations
// are supported.
//////////////////////////////////////////////////////////////////////

class CSerialCommWt
{
public:
	CSerialCommWt();
	CSerialCommWt(const CString sPortName);
	virtual ~CSerialCommWt();
public:
	HRESULT ClearWriteBuffer();
	HRESULT ClearReadBuffer();
	HRESULT Flush(DWORD dwFlag = PURGE_TXCLEAR | PURGE_RXCLEAR);
	HRESULT Write(const unsigned char *pszBuf, DWORD dwSize);
	HRESULT Read(unsigned char *pszBuf, DWORD dwSize);
	HRESULT ConfigPort(DWORD dwBaudRate = CBR_57600, DWORD dwTimeOutInSec = 5);
	HRESULT Close();	
	HRESULT Open();

private:
	CString m_sPortName;
	DCB m_dcb;
	HANDLE m_hSerialComm; //串口句柄
	OVERLAPPED m_OverlappedRead;
	OVERLAPPED m_OverlappedWrite; //两个信号全局变量（串口操作用）
};

