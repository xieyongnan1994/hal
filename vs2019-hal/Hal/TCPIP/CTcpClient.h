#pragma once
#ifndef _CTCPCLIENT_H
#define _CTCPCLIENT_H
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") //������̬��
//�ͻ��˷�����Ϣ���ȷ���������
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
public:
	int InitNetParam(int portNum = 9600, CString ipAddr = _T("192.168.0.1"));
	int SendData(char* sendbuff, int nSendLen, char* recbuff, int nRecLen);
	void SetIPAddr(CString ip);
	void SetPortNum(int num);
	void Colse();
private:
	SOCKET m_socketClient;
	CString m_sIPaddr;
	int m_nPortNum;
	CRITICAL_SECTION m_gSaveSection;


};

#endif // !_CTCPCLIENT_H
