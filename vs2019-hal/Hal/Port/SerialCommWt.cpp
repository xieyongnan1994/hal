// SyncSerialComm.cpp: implementation of the CSyncSerialComm class.
//
//////////////////////////////////////////////////////////////////////

#include <afxinet.h>
#include "SerialCommWt.h"
#include <assert.h>
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialCommWt::CSerialCommWt()
	: m_hSerialComm(INVALID_HANDLE_VALUE)
{
	m_sPortName=_T("COM1");
	m_dcb.BaudRate = CBR_9600;
	m_dcb.Parity = NOPARITY;
	m_dcb.ByteSize = 8;
	m_dcb.StopBits = ONESTOPBIT;

}

CSerialCommWt::CSerialCommWt(const CString sPortName)
	: m_hSerialComm(INVALID_HANDLE_VALUE)
{
	assert(sPortName);

	m_sPortName = sPortName;
	m_dcb.BaudRate = CBR_9600;
	m_dcb.Parity = NOPARITY;
	m_dcb.ByteSize = 8;
	m_dcb.StopBits = ONESTOPBIT;

}

CSerialCommWt::~CSerialCommWt()
{
	Close();//关闭串口
}

//////////////////////////////////////////////////////////////////////
// Name: Open
// Version: 1.0
// Return: HRESULT
// Comment: This function is used open a connection with a serial port.
// Uses non-overlapped i/o, and allows for reading & writing to the 
// port.
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::Open()
{
	try
	{
		DCB dcb;

		m_hSerialComm = CreateFile((LPCWSTR)m_sPortName, /* Port Name */ 
								GENERIC_READ | GENERIC_WRITE, /* Desired Access */ 
								0, /* Shared Mode */
								NULL, /* Security */
								OPEN_EXISTING, /* Creation Disposition */
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
								NULL); /* Non Overlapped */

		if(m_hSerialComm == INVALID_HANDLE_VALUE)
		{
			unsigned long error = ::GetLastError();
			return E_FAIL;			 
		}

		//清空异步读写参数
		memset(&(m_OverlappedRead), 0, sizeof (OVERLAPPED));
		memset(&(m_OverlappedWrite), 0, sizeof (OVERLAPPED));

		//设置dcb块
		dcb.DCBlength = sizeof( DCB );         //长度
		GetCommState(m_hSerialComm , &dcb );

		//波特率,奇偶校验,数据位,停止位  如：57600,n,8,1
		BuildCommDCB((LPCWSTR)(m_sPortName+_T(":9600,n,8,1")),&dcb);
		//BuildCommDCB(_T("baud=9600 parity=N data=8 stop=1.5"), &dcb);
		//------------------------------ 
		dcb.fBinary=TRUE;                      //二进制方式  
		dcb.fOutxCtsFlow=FALSE;                //不用CTS检测发送流控制
		dcb.fOutxDsrFlow=FALSE;                //不用DSR检测发送流控制
		dcb.fDtrControl=DTR_CONTROL_DISABLE;   //禁止DTR流量控制
		dcb.fDsrSensitivity=FALSE;             //对DTR信号线不敏感
		dcb.fTXContinueOnXoff=TRUE;            //检测接收缓冲区
		dcb.fOutX=FALSE;                       //不做发送字符控制
		dcb.fInX =FALSE;                       //不做接收控制
		dcb.fErrorChar=FALSE;                  //是否用指定字符替换校验错的字符
		dcb.fNull=FALSE;                       //保留NULL字符
		dcb.fRtsControl=RTS_CONTROL_ENABLE;    //允许RTS流量控制
		dcb.fAbortOnError=FALSE;               //发送错误后，继续进行下面的读写操作
		dcb.fDummy2=0;                         //保留
		dcb.wReserved=0;                       //没有使用，必须为0
		dcb.XonLim=0;                          //指定在XOFF字符发送之前接收到缓冲区中可允许的最小字节数
		dcb.XoffLim=0;                         //指定在XOFF字符发送之前缓冲区中可允许的最小可用字节数
		dcb.XonChar=17;                        //发送和接收的XON字符 
		dcb.XoffChar=19;                       //发送和接收的XOFF字符
		dcb.ErrorChar=0;                       //代替接收到奇偶校验错误的字符 
		dcb.EofChar=0;                         //用来表示数据的结束
		dcb.EvtChar=0;                         //事件字符，接收到此字符时，会产生一个事件
		dcb.wReserved1=0;                      //没有使用
		dcb.BaudRate=m_dcb.BaudRate;           //波特率
		dcb.Parity=m_dcb.Parity;               //奇偶校验 
		dcb.ByteSize=m_dcb.ByteSize;		   //数据位
		dcb.StopBits=m_dcb.StopBits;           //停止位
		//------------------------------

		if(dcb.Parity==0 )        // 0-4=None,Odd,Even,Mark,Space
		{
			dcb.fParity=FALSE;    //奇偶校验无效
		}
		else
		{
			dcb.fParity=TRUE;     //奇偶校验有效
		}

		//读写超时设置
		COMMTIMEOUTS CommTimeOuts;

		CommTimeOuts.ReadIntervalTimeout = 250;           //字符允许间隔ms   该参数如果为最大值，会使readfile命令立即返回   
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;      //总的超时时间(对单个字节)  
		CommTimeOuts.ReadTotalTimeoutConstant = 2500;     //多余的超时时间ms 
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;     //总的超时时间(对单个字节) 
		CommTimeOuts.WriteTotalTimeoutConstant = 2500;    //多余的超时时间

		SetCommTimeouts( m_hSerialComm, &CommTimeOuts );

		//获取信号句柄
		m_OverlappedRead.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
		m_OverlappedWrite.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

		if( !SetCommState( m_hSerialComm, &dcb ) ||    //判断设置参数是否成功
		!SetupComm( m_hSerialComm, 1024, 1024 ) ||     //设置输入和输出缓冲区是否成功
		m_OverlappedRead.hEvent==NULL ||
		m_OverlappedWrite.hEvent==NULL)
		{  
			DWORD dwError = GetLastError();                     //获取最后的错误信息
			if( m_OverlappedRead.hEvent != NULL )  CloseHandle( m_OverlappedRead.hEvent );
			if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
			CloseHandle( m_hSerialComm );
			m_hSerialComm = INVALID_HANDLE_VALUE;
			return dwError;
		}

		return S_OK;
	}
	catch(...)
	{
		return S_FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Name: Close
// Version: 1.0
// Return: HRESULT
// Comment: This function is used to close the serial port connection
// Note: This function is called with the destructor
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::Close()
{
	try
	{
		if(m_hSerialComm != INVALID_HANDLE_VALUE)
		{
			SetCommMask(m_hSerialComm ,NULL);
			SetEvent(m_OverlappedRead.hEvent);
			SetEvent(m_OverlappedWrite.hEvent);

			if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
			if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
			if (CloseHandle( m_hSerialComm )==FALSE) return S_FALSE;
			m_hSerialComm  = INVALID_HANDLE_VALUE;
		}		
	}
	catch(...)
	{
		return S_FALSE;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// Name: ConfigPort
// Version: 1.0
// Parameter: dwBaudRate - This must be set to the baud rate of the
// serial port connection otherwise invalid reads occur.
// dwTimeOutInSec - Specifies the timeout for read and write of the serial
// port connection in seconds
// Return: HRESULT
// Comment: This function is used configure the serial port connection.
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::ConfigPort(DWORD dwBaudRate, DWORD dwTimeOutInSec)
{
	if(!SetupComm(m_hSerialComm, 1024, 1024))
		return E_FAIL;

	DCB dcbConfig;

	if(GetCommState(m_hSerialComm, &dcbConfig)) /* Configuring Serial Port Settings */
	{
		dcbConfig.BaudRate = dwBaudRate;
		dcbConfig.ByteSize = 8;
		dcbConfig.Parity = NOPARITY;
		dcbConfig.StopBits = ONESTOPBIT;
		dcbConfig.fDtrControl = DTR_CONTROL_DISABLE;

		dcbConfig.fBinary = false;
		dcbConfig.fParity = false;
	}

	else
		return E_FAIL;

	if(!SetCommState(m_hSerialComm, &dcbConfig))
		return E_FAIL;

	COMMTIMEOUTS commTimeout;
				
	if(GetCommTimeouts(m_hSerialComm, &commTimeout)) /* Configuring Read & Write Time Outs */
	{
		commTimeout.ReadIntervalTimeout = 1000*dwTimeOutInSec;
		commTimeout.ReadTotalTimeoutConstant = 1000*dwTimeOutInSec;
		commTimeout.ReadTotalTimeoutMultiplier = 0;
		commTimeout.WriteTotalTimeoutConstant = 1000*dwTimeOutInSec;
		commTimeout.WriteTotalTimeoutMultiplier = 0;
	}
 
	else
		return E_FAIL;

	if(SetCommTimeouts(m_hSerialComm, &commTimeout))
		return S_OK;

	else
		return E_FAIL;
}

//////////////////////////////////////////////////////////////////////
// Name: Read
// Version: 1.0
// Parameter: ppszBuf - The buffer that will have the value that was
// read in from the serial port.
// dwSize - The size of the buffer
// Return: HRESULT
// Comment: This function sets an event that will be signalled if the
// any byte is buffered internally. Once this occurs, the function keeps
// reading multiple a single byte at a time until there is no more furthur
// byte to read from the input stream
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::Read(unsigned char *pszBuf, DWORD dwSize)
{
	try
	{ 
		if( m_hSerialComm  == INVALID_HANDLE_VALUE ) return S_FALSE;

		DWORD   lngBytesRead=dwSize;
		BOOL    fReadStat;
		DWORD   dwRes=0;

		//读数据
		fReadStat=ReadFile(m_hSerialComm,pszBuf,dwSize,&lngBytesRead,&(m_OverlappedRead));  
		//Sleep(1);
		if( !fReadStat )
		{
			if( GetLastError() == ERROR_IO_PENDING )//重叠 I/O 操作在进行中 
			{
				dwRes=WaitForSingleObject(m_OverlappedRead.hEvent,1000);   //等待，直到超时
				switch(dwRes)
				{
					case WAIT_OBJECT_0:   //读完成   

						if(GetOverlappedResult(m_hSerialComm,&(m_OverlappedRead),&lngBytesRead,FALSE)==0)
						{
							//错误
							return -1;
						}

						break;
					case WAIT_TIMEOUT:    //超时
						return -2;
						break;
					default:              //WaitForSingleObject 错误
						break;
				}
			}
		}

		return (HRESULT)lngBytesRead;              

	}
	catch(...)
	{
		return -3;//异常
	}
}

//////////////////////////////////////////////////////////////////////
// Name: Write
// Version: 1.0
// Parameter: szBuf - The buffer holding the bytes to write to the serial
// port connection
// dwSize - The size of the buffer
// Return: HRESULT
// Comment: This function writes one byte at a time until all the bytes
// in the buffer is sent out
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::Write(const unsigned char *pszBuf, DWORD dwSize)
{
	try
	{
		if( m_hSerialComm  == INVALID_HANDLE_VALUE ) return S_FALSE;

		DWORD dwBytesWritten=dwSize;
		BOOL bWriteStat;
		COMSTAT ComStat;
		DWORD   dwErrorFlags;

		ClearCommError(m_hSerialComm,&dwErrorFlags,&ComStat);
		bWriteStat=WriteFile(m_hSerialComm, pszBuf, dwSize, &dwBytesWritten, &(m_OverlappedWrite));

		if(!bWriteStat)
		{
			if(GetLastError()==ERROR_IO_PENDING)		//重叠 I/O 操作在进行中
			{
				GetOverlappedResult(m_hSerialComm,&(m_OverlappedWrite),&dwBytesWritten,TRUE); //等待直到发送完毕
			}
			else
			{
				dwBytesWritten=0;
			}
		}
		return (HRESULT)dwBytesWritten;
	}
	catch(...)
	{
		return S_FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Name: Flush
// Version: 1.0
// Parameter: dwFlag - The flag specifying if the input/output buffer
// to be flushed
// Return: HRESULT
// Comment: This function is flushes the specfied buffer
// Note: By default, both the input and output buffers are flushed
//////////////////////////////////////////////////////////////////////

HRESULT CSerialCommWt::Flush(DWORD dwFlag)
{
	try
	{
		if( m_hSerialComm  == INVALID_HANDLE_VALUE ) return S_FALSE;
		PurgeComm(m_hSerialComm, dwFlag);
	}
	catch(...)
	{
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CSerialCommWt::ClearReadBuffer()
{
	try
	{
		if( m_hSerialComm  == INVALID_HANDLE_VALUE ) return S_FALSE;
		PurgeComm(m_hSerialComm,PURGE_RXABORT | PURGE_RXCLEAR);
	}
	catch(...)
	{
		return S_FALSE;
	}
	return S_OK;
}


HRESULT CSerialCommWt::ClearWriteBuffer()
{
	try
	{
		if( m_hSerialComm  == INVALID_HANDLE_VALUE ) return S_FALSE;
		PurgeComm(m_hSerialComm,PURGE_TXABORT |  PURGE_TXCLEAR);
	}
	catch(...)
	{
		return S_FALSE;
	}
	return S_OK;
}