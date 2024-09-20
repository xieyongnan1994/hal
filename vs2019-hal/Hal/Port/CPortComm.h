#pragma once
#ifndef _CPORTCOMM_H
#define _CPORTCOMM_H
#include  <iostream>
#include  <Windows.h>
#include  <string>
#include  <atlstr.h>
using namespace std;

/***********************************************
�ļ�����CPortComm.h
���ͣ�C++ͷ�ļ�
���ܣ�ʵ�ֶԴ��ڲ����ķ�װ
ʱ�䣺2023.10.30
**********************************************/


class CPortComm
{
public:
    CPortComm();
    CPortComm(const CString sPortName);
    ~CPortComm();

public:
    BOOL OpenPortComm();                            /*���ڴ򿪺���*/
    void ClosePortComm();                           /**���ڹرպ���*/
    BOOL ReadByte(BYTE& byByte);                    /*��ȡ���ݺ���*/
    BOOL WriteByte(BYTE bybyte);                    /*д�����ݺ���*/
    BOOL CleanPortComm();                           /*�����ڻ�����*/

    /*�Դ��ڳ�ʱ������������*/
    BOOL SetCommunicationTimeouts(DWORD ReadIntervalTimeout,         /*�������ʱʱ��*/
                                  DWORD ReadTotalTimeoutMultiplier,  /*��ʱ��ϵ��*/
                                  DWORD ReadTotalTimeoutConstant,    /*��ʱ�䳣��*/
                                  DWORD WriteTotalTimeoutMultiplier, /*дʱ��ϵ��*/
                                  DWORD WriteTotalTimeoutConstant);  /*дʱ�䳣��*/

     /*�Դ��ڲ�����������*/
    BOOL ConfigureComm(
        DWORD BaudRate,  /*������*/
        BYTE ByteSize,   /*ͨ���ֽ�λ��*/
        BYTE  Parity,    /*ָ����żУ��λ*/
        BYTE StopBits);  /*ָ��ֹͣλ��λ��*/

    HANDLE hComm;               /*����COM���*/
    DCB    m_dcb;               /*����״̬��Ϣ*/
    COMMTIMEOUTS m_CommTimeouts;
    BOOL  m_bCommReady;
    BOOL  bWriteRC;
    BOOL  bReadRC;
    DWORD  iBytesWritten;
    DWORD  iBytesRead;
    DWORD  dwBytesRead;
    CString m_sPortName;
 
};

#endif // !_CPORTCOMM_H

