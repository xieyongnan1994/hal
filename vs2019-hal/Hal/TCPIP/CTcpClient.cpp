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
	//每次使用socket前必须重新加载new初始化
	m_socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP 使用SOCK_STREAM   UDP使用SOCK_DGRAM
	if (m_socketClient == INVALID_SOCKET)
	{
		closesocket(m_socketClient);
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//设置socket为阻塞模式，等待数据直到超时
	ULONG ul = 0;
	err = ioctlsocket(m_socketClient, FIONBIO, (ULONG*)&ul);
	if (err == SOCKET_ERROR)
	{
		closesocket(m_socketClient);
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//设置阻塞时间ms
	int nNettime = 200;
	setsockopt(m_socketClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&nNettime, sizeof(int));
	setsockopt(m_socketClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNettime, sizeof(int));
	//IP及端口加载
	SOCKADDR_IN addr;
	char* IpName;
	USES_CONVERSION;
	IpName = T2A(m_sIPaddr);
	addr.sin_addr.S_un.S_addr = inet_addr(IpName);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPortNum);
	//向服务器申请连接
	err = connect(m_socketClient, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (err == SOCKET_ERROR)
	{
		//错误信息：
		//EACCES, EPERM：用户试图在套接字广播标志没有设置的情况下连接广播地址或由于防火墙策略导致连接失败。
		//EADDRINUSE：本地地址处于使用状态。
		//EAFNOSUPPORT：参数serv_add中的地址非合法地址。
		//EAGAIN：没有足够空闲的本地端口。
		//EALREADY：套接字为非阻塞套接字，并且原来的连接请求还未完成。
		//EBADF：非法的文件描述符。
		//ECONNREFUSED：远程地址并没有处于监听状态。
		//EFAULT：指向套接字结构体的地址非法。
		//EINPROGRESS：套接字为非阻塞套接字，且连接请求没有立即完成。
		//EINTR：系统调用的执行由于捕获中断而中止。
		//EISCONN：已经连接到该套接字。
		//ENETUNREACH：网络不可到达。
		//ENOTSOCK：文件描述符不与套接字相关。
		//ETIMEDOUT：连接超时。
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
	//发送数据
	err = send(m_socketClient, sendbuff, nSendLen, 0);
	if (err == SOCKET_ERROR)
	{
		//AfxMessageBox(_T("socket send fail !"));
		LeaveCriticalSection(&m_gSaveSection);
		return SOCKET_ERROR;
	}
	//接收服务器反馈数据
	err = recv(m_socketClient, recbuff, nRecLen, 0);
	if ((err == SOCKET_ERROR) || (err == 0))
	{
		//AfxMessageBox(_T("socket recv fail !"));
		//err == SOCKET_ERROR 没有接收到数据
		//err == 0 关闭了链接请求
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
