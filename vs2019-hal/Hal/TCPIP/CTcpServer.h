#pragma once
#ifndef _CTCPSERVER_H
#define _CTCPSERVER_H
#include <WinSock2.h>
#include <thread>
#include <functional>   
#include <socketapi.h>
#pragma comment(lib,"ws2_32.lib") //������̬��

//������������Ϣ��������������Ϣ
class CTcpServer
{
public:
	CTcpServer(CString strIp, unsigned int uPort);
	virtual ~CTcpServer();

	//��ʼ����������
	int InitServer();

	//��������
	bool SendMsg(const char* strMsg);

	//�������ݲ���ӡ
	bool RecvMsg();

private:
	unsigned int m_uPort;//�����˿�
	CString m_strIp;//���ڼ�������ָ��IP��ַ

	SOCKET m_listenSocket = NULL;//�����׽���
	SOCKET m_clientSocket = NULL;//�ͻ����׽���



};


#endif // !_CTCPSERVER_H

