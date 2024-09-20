#include "CPortComm.h"
#include <assert.h>

CPortComm::CPortComm()
{
    m_sPortName = _T("COM1");
    m_dcb.BaudRate = CBR_9600;
    m_dcb.Parity = NOPARITY;
    m_dcb.ByteSize = 8;
    m_dcb.StopBits = ONESTOPBIT;
}

CPortComm::CPortComm(const CString sPortName)
{
    assert(sPortName);

    m_sPortName = sPortName;
    m_dcb.BaudRate = CBR_9600;
    m_dcb.Parity = NOPARITY;
    m_dcb.ByteSize = 8;
    m_dcb.StopBits = ONESTOPBIT;
}

CPortComm::~CPortComm()
{
    ClosePortComm();
}

/*�򿪴��ں��� ����Ϊ��COM1������COM2��֮��Ĵ������ַ���*/
BOOL CPortComm::OpenPortComm()
{ 
    hComm = CreateFile(m_sPortName,            /* �豸�� */
        GENERIC_READ | GENERIC_WRITE,        /* ����ģʽ,��ͬʱ��д */
        0,                                   /* ����ģʽ,0��ʾ������ */
        0,                                   /*��ȫ������,һ��ʹ��NULL */
        OPEN_EXISTING,                       /* �ò�����ʾ�豸�������,���򴴽�ʧ�� */
        0,
        0);
    if (hComm == INVALID_HANDLE_VALUE) /*�жϴ�COM�����Ƿ�ɹ�*/
    {
        //MessageBox("Cannot open Communication Port.Please\nQuit the program and Re-start your PC.","Com Port Error",MB_OK+MB_ICONERROR);
        return false;
    }
    else
        return true;
}

void CPortComm::ClosePortComm()
{
    if (hComm == INVALID_HANDLE_VALUE)
        return;
    CloseHandle(hComm);/*�رմ���*/
    return;
}

BOOL CPortComm::ReadByte(BYTE& byByte)
{
    BYTE m_brx;
    DWORD dwBytesTransferred = 0;/*����ʵ�ʶ�ȡ�ֽ����ı���*/
    if (ReadFile(hComm, &m_brx, 1, &dwBytesTransferred, 0))
    {
        if (dwBytesTransferred == 1)
        {
            byByte = m_brx;
            return true;
        }
    }
    return false;
}

BOOL CPortComm::WriteByte(BYTE bybyte)
{
    if (WriteFile(hComm, &bybyte, 1, &iBytesWritten, NULL) == 0)/*д��ָ���ֽڸ��������ݲ��ж�д���Ƿ�ɹ�*/
        return false;
    else return true;
}

BOOL CPortComm::CleanPortComm()
{
    return PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

BOOL CPortComm::SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
{
    if ((m_bCommReady = GetCommTimeouts(hComm, &m_CommTimeouts)) == 0)
        return false;
    m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout;                /*�������ʱʱ��*/
    m_CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;      /*��ʱ��ϵ��*/
    m_CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;  /*��ʱ�䳣��*/
    m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;    /*дʱ��ϵ��*/
    m_CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;/*дʱ�䳣��*/
    m_bCommReady = SetCommTimeouts(hComm, &m_CommTimeouts);/*������hCom��ʱ�������óɴ���ĳ�ʱ����*/
    if (m_bCommReady == 0)    /*�ж������Ƿ�ɹ�*/
    {
        //MessageBox("StCommTimeouts function failed", "Com Port Error", MB_OK + MB_ICONERROR);
        CloseHandle(hComm);
        return false;
    }
    return true;  
}

BOOL CPortComm::ConfigureComm(DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits)
{
    char szDCBparam[50];
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", BaudRate, Parity, ByteSize, StopBits);  
    
    size_t len = strlen(szDCBparam) + 1;
    size_t converted = 0; wchar_t* WStr;
    WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
    mbstowcs_s(&converted, WStr, len, szDCBparam, _TRUNCATE);

    if (GetCommState(hComm, &m_dcb) && BuildCommDCB(WStr, &m_dcb))
    {
        return true;
    }
   /* CloseHandle(hComm);
    return false;*/

    //    SetupComm(hComm, 1024, 1024);
    if ((m_bCommReady = GetCommState(hComm, &m_dcb)) == 0) /*�жϴ�����Ϣ�Ƿ��ȡ���*/
    {

        CloseHandle(hComm);
        return false;
    }
    m_bCommReady = 0;
    m_dcb.BaudRate = BaudRate;/*������*/
    m_dcb.ByteSize = ByteSize;/*ͨ���ֽ�λ��*/
    m_dcb.Parity = Parity;    /*ָ����żУ��λ*/
    m_dcb.StopBits = StopBits;/*ָ��ֹͣλ��λ��*/
    //m_dcb.fParity = fParity;  /*ָ����żУ��λ*/
#if 0
    m_dcb.fBinary = TRUE;
    m_dcb.fDsrSensitivity = false;
    m_dcb.fOutX = false;
    m_dcb.fInX = false;
    m_dcb.fNull = false;
    m_dcb.fAbortOnError = TRUE;
    m_dcb.fOutxCtsFlow = FALSE;
    m_dcb.fOutxDsrFlow = false;
    m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
    m_dcb.fDsrSensitivity = false;
    m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
    m_dcb.fOutxCtsFlow = false;
    m_dcb.fOutxCtsFlow = false;
#endif
    m_bCommReady = SetCommState(hComm, &m_dcb);/*��hComm�Ŀ��Ʋ������ó�m_dcb�Ĳ���*/
    if (m_bCommReady == 0) /*�ж������Ƿ�ɹ�*/
    {
        //MessageBox("SetCommState Error", "Error", MB_OK + MB_ICONERROR);
        CloseHandle(hComm);
        //return false;
    }
    return true;
 
}
