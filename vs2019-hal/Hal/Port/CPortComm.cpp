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

/*打开串口函数 参数为“COM1”、“COM2”之类的串口名字符串*/
BOOL CPortComm::OpenPortComm()
{ 
    hComm = CreateFile(m_sPortName,            /* 设备名 */
        GENERIC_READ | GENERIC_WRITE,        /* 访问模式,可同时读写 */
        0,                                   /* 共享模式,0表示不共享 */
        0,                                   /*安全性设置,一般使用NULL */
        OPEN_EXISTING,                       /* 该参数表示设备必须存在,否则创建失败 */
        0,
        0);
    if (hComm == INVALID_HANDLE_VALUE) /*判断打开COM操作是否成功*/
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
    CloseHandle(hComm);/*关闭串口*/
    return;
}

BOOL CPortComm::ReadByte(BYTE& byByte)
{
    BYTE m_brx;
    DWORD dwBytesTransferred = 0;/*定义实际读取字节数的变量*/
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
    if (WriteFile(hComm, &bybyte, 1, &iBytesWritten, NULL) == 0)/*写入指定字节个数的数据并判断写入是否成功*/
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
    m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout;                /*读间隔超时时间*/
    m_CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;      /*读时间系数*/
    m_CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;  /*读时间常量*/
    m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;    /*写时间系数*/
    m_CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;/*写时间常量*/
    m_bCommReady = SetCommTimeouts(hComm, &m_CommTimeouts);/*将串口hCom超时参数设置成传入的超时参数*/
    if (m_bCommReady == 0)    /*判断设置是否成功*/
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
    if ((m_bCommReady = GetCommState(hComm, &m_dcb)) == 0) /*判断串口信息是否获取情况*/
    {

        CloseHandle(hComm);
        return false;
    }
    m_bCommReady = 0;
    m_dcb.BaudRate = BaudRate;/*波特率*/
    m_dcb.ByteSize = ByteSize;/*通信字节位数*/
    m_dcb.Parity = Parity;    /*指定奇偶校验位*/
    m_dcb.StopBits = StopBits;/*指定停止位的位数*/
    //m_dcb.fParity = fParity;  /*指定奇偶校验位*/
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
    m_bCommReady = SetCommState(hComm, &m_dcb);/*将hComm的控制参数设置成m_dcb的参数*/
    if (m_bCommReady == 0) /*判断设置是否成功*/
    {
        //MessageBox("SetCommState Error", "Error", MB_OK + MB_ICONERROR);
        CloseHandle(hComm);
        //return false;
    }
    return true;
 
}
