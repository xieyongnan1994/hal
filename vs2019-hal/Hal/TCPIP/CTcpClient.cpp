#include "pch.h"
#include "CTcpClient.h"

CTcpClient::CTcpClient()
{
	InitializeCriticalSection(&m_gSaveSection);//
	m_socketClient = INVALID_SOCKET;
}

CTcpClient::~CTcpClient()
{
	this->Colse();
	DeleteCriticalSection(&m_gSaveSection);
}
int CTcpClient::InitNetParam(int portNum, CString ipAddr)
{
	int err = 0;
	WORD wVirson;
	WSADATA wsadata;
	wVirson = MAKEWORD(1, 1);
	err = WSAStartup(wVirson, &wsadata);
	if (err != 0)
	{
		return err;
	}
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	m_sIPaddr = ipAddr;
	m_nPortNum = portNum;
	EnterCriticalSection(&m_gSaveSection);
	//ÿ��ʹ��socketǰ�������¼���new��ʼ��
	m_socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP ʹ��SOCK_STREAM   UDPʹ��SOCK_DGRAM
	if (m_socketClient == INVALID_SOCKET)
	{
		closesocket(m_socketClient);
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//����socketΪ����ģʽ���ȴ�����ֱ����ʱ
	ULONG ul = 0;
	err = ioctlsocket(m_socketClient, FIONBIO, (ULONG*)&ul);
	if (err == SOCKET_ERROR)
	{
		closesocket(m_socketClient);
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//��������ʱ��ms
	int nNettime = 200;
	setsockopt(m_socketClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&nNettime, sizeof(int));
	setsockopt(m_socketClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNettime, sizeof(int));
	//IP���˿ڼ���
	SOCKADDR_IN addr;
	char* IpName;
	USES_CONVERSION;
	IpName = T2A(m_sIPaddr);
	addr.sin_addr.S_un.S_addr = inet_addr(IpName);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPortNum);
	//���������������
	err = connect(m_socketClient, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (err == SOCKET_ERROR)
	{
		//������Ϣ��
		//EACCES, EPERM���û���ͼ���׽��ֹ㲥��־û�����õ���������ӹ㲥��ַ�����ڷ���ǽ���Ե�������ʧ�ܡ�
		//EADDRINUSE�����ص�ַ����ʹ��״̬��
		//EAFNOSUPPORT������serv_add�еĵ�ַ�ǺϷ���ַ��
		//EAGAIN��û���㹻���еı��ض˿ڡ�
		//EALREADY���׽���Ϊ�������׽��֣�����ԭ������������δ��ɡ�
		//EBADF���Ƿ����ļ���������
		//ECONNREFUSED��Զ�̵�ַ��û�д��ڼ���״̬��
		//EFAULT��ָ���׽��ֽṹ��ĵ�ַ�Ƿ���
		//EINPROGRESS���׽���Ϊ�������׽��֣�����������û��������ɡ�
		//EINTR��ϵͳ���õ�ִ�����ڲ����ж϶���ֹ��
		//EISCONN���Ѿ����ӵ����׽��֡�
		//ENETUNREACH�����粻�ɵ��
		//ENOTSOCK���ļ������������׽�����ء�
		//ETIMEDOUT�����ӳ�ʱ��
		int num = 0;
		num = errno;
		closesocket(m_socketClient);
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	LeaveCriticalSection(&m_gSaveSection);
	return err;
}

int CTcpClient::SendData(char* sendbuff, int nSendLen, char* recbuff, int nRecLen)
{
	int err = 0;
	if (m_socketClient == INVALID_SOCKET)
	{
		return SOCKET_ERROR;
	}
	EnterCriticalSection(&m_gSaveSection);
	//��������
	err = send(m_socketClient, sendbuff, nSendLen, 0);
	if (err == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("socket send fail !"));
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//���շ�������������
	err = recv(m_socketClient, recbuff, nRecLen, 0);
	if ((err == SOCKET_ERROR) || (err == 0))
	{
		//AfxMessageBox(_T("socket recv fail !"));
		//err == SOCKET_ERROR û�н��յ�����
		//err == 0 �ر�����������
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	LeaveCriticalSection(&m_gSaveSection);
	return 0;
}

void CTcpClient::SetIPAddr(CString ip)
{
	m_sIPaddr = ip;
}

void CTcpClient::SetPortNum(int num)
{
	m_nPortNum = num;
}

void CTcpClient::Colse()
{
	closesocket(m_socketClient);
	WSACleanup();
}
