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
	Close();//�رմ���
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

		//����첽��д����
		memset(&(m_OverlappedRead), 0, sizeof (OVERLAPPED));
		memset(&(m_OverlappedWrite), 0, sizeof (OVERLAPPED));

		//����dcb��
		dcb.DCBlength = sizeof( DCB );         //����
		GetCommState(m_hSerialComm , &dcb );

		//������,��żУ��,����λ,ֹͣλ  �磺57600,n,8,1
		BuildCommDCB((LPCWSTR)(m_sPortName+_T(":9600,n,8,1")),&dcb);
		//BuildCommDCB(_T("baud=9600 parity=N data=8 stop=1.5"), &dcb);
		//------------------------------ 
		dcb.fBinary=TRUE;                      //�����Ʒ�ʽ  
		dcb.fOutxCtsFlow=FALSE;                //����CTS��ⷢ��������
		dcb.fOutxDsrFlow=FALSE;                //����DSR��ⷢ��������
		dcb.fDtrControl=DTR_CONTROL_DISABLE;   //��ֹDTR��������
		dcb.fDsrSensitivity=FALSE;             //��DTR�ź��߲�����
		dcb.fTXContinueOnXoff=TRUE;            //�����ջ�����
		dcb.fOutX=FALSE;                       //���������ַ�����
		dcb.fInX =FALSE;                       //�������տ���
		dcb.fErrorChar=FALSE;                  //�Ƿ���ָ���ַ��滻У�����ַ�
		dcb.fNull=FALSE;                       //����NULL�ַ�
		dcb.fRtsControl=RTS_CONTROL_ENABLE;    //����RTS��������
		dcb.fAbortOnError=FALSE;               //���ʹ���󣬼�����������Ķ�д����
		dcb.fDummy2=0;                         //����
		dcb.wReserved=0;                       //û��ʹ�ã�����Ϊ0
		dcb.XonLim=0;                          //ָ����XOFF�ַ�����֮ǰ���յ��������п��������С�ֽ���
		dcb.XoffLim=0;                         //ָ����XOFF�ַ�����֮ǰ�������п��������С�����ֽ���
		dcb.XonChar=17;                        //���ͺͽ��յ�XON�ַ� 
		dcb.XoffChar=19;                       //���ͺͽ��յ�XOFF�ַ�
		dcb.ErrorChar=0;                       //������յ���żУ�������ַ� 
		dcb.EofChar=0;                         //������ʾ���ݵĽ���
		dcb.EvtChar=0;                         //�¼��ַ������յ����ַ�ʱ�������һ���¼�
		dcb.wReserved1=0;                      //û��ʹ��
		dcb.BaudRate=m_dcb.BaudRate;           //������
		dcb.Parity=m_dcb.Parity;               //��żУ�� 
		dcb.ByteSize=m_dcb.ByteSize;		   //����λ
		dcb.StopBits=m_dcb.StopBits;           //ֹͣλ
		//------------------------------

		if(dcb.Parity==0 )        // 0-4=None,Odd,Even,Mark,Space
		{
			dcb.fParity=FALSE;    //��żУ����Ч
		}
		else
		{
			dcb.fParity=TRUE;     //��żУ����Ч
		}

		//��д��ʱ����
		COMMTIMEOUTS CommTimeOuts;

		CommTimeOuts.ReadIntervalTimeout = 250;           //�ַ�������ms   �ò������Ϊ���ֵ����ʹreadfile������������   
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;      //�ܵĳ�ʱʱ��(�Ե����ֽ�)  
		CommTimeOuts.ReadTotalTimeoutConstant = 2500;     //����ĳ�ʱʱ��ms 
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;     //�ܵĳ�ʱʱ��(�Ե����ֽ�) 
		CommTimeOuts.WriteTotalTimeoutConstant = 2500;    //����ĳ�ʱʱ��

		SetCommTimeouts( m_hSerialComm, &CommTimeOuts );

		//��ȡ�źž��
		m_OverlappedRead.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
		m_OverlappedWrite.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

		if( !SetCommState( m_hSerialComm, &dcb ) ||    //�ж����ò����Ƿ�ɹ�
		!SetupComm( m_hSerialComm, 1024, 1024 ) ||     //�������������������Ƿ�ɹ�
		m_OverlappedRead.hEvent==NULL ||
		m_OverlappedWrite.hEvent==NULL)
		{  
			DWORD dwError = GetLastError();                     //��ȡ���Ĵ�����Ϣ
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

		//������
		fReadStat=ReadFile(m_hSerialComm,pszBuf,dwSize,&lngBytesRead,&(m_OverlappedRead));  
		//Sleep(1);
		if( !fReadStat )
		{
			if( GetLastError() == ERROR_IO_PENDING )//�ص� I/O �����ڽ����� 
			{
				dwRes=WaitForSingleObject(m_OverlappedRead.hEvent,1000);   //�ȴ���ֱ����ʱ
				switch(dwRes)
				{
					case WAIT_OBJECT_0:   //�����   

						if(GetOverlappedResult(m_hSerialComm,&(m_OverlappedRead),&lngBytesRead,FALSE)==0)
						{
							//����
							return -1;
						}

						break;
					case WAIT_TIMEOUT:    //��ʱ
						return -2;
						break;
					default:              //WaitForSingleObject ����
						break;
				}
			}
		}

		return (HRESULT)lngBytesRead;              

	}
	catch(...)
	{
		return -3;//�쳣
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
			if(GetLastError()==ERROR_IO_PENDING)		//�ص� I/O �����ڽ�����
			{
				GetOverlappedResult(m_hSerialComm,&(m_OverlappedWrite),&dwBytesWritten,TRUE); //�ȴ�ֱ���������
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