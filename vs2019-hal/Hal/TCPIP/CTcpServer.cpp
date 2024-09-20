#include "pch.h"
#include "CTcpServer.h"

CTcpServer::CTcpServer(CString strIp, unsigned int uPort)
    :m_strIp(strIp), 
    m_uPort(uPort)
{

}

CTcpServer::~CTcpServer()
{
    if (m_clientSocket)
    {
        closesocket(m_clientSocket);
        m_clientSocket = NULL;
    }
    if (m_listenSocket)
    {
        closesocket(m_listenSocket);
        m_listenSocket = NULL;
    }
    WSACleanup();
}

int CTcpServer::InitServer()
{
	WSADATA	wsaData;
	//1. 初始化环境
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -4;
	}
	//2. 创建监听套接字
	if ((m_listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return -3;
	}
	//协议
	sockaddr_in sockadd = { 0, };
	sockadd.sin_family = AF_INET;//IPV4协议簇
	sockadd.sin_port = htons(m_uPort);//监听端口
	sockadd.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//监听本机任意IP
	//3. 监听套接字与IP地址及端口绑定
	if (bind(m_listenSocket, (struct sockaddr*)&sockadd, sizeof(sockadd)) == SOCKET_ERROR)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
		return -2;
	}
	//4. 监听套接字
	if (listen(m_listenSocket, 1) == SOCKET_ERROR)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
		return -1;
	}
	sockaddr_in addr = { 0, };
	int addrlen = sizeof(addr);
	//5. 等待客户端连接
	m_clientSocket = accept(m_listenSocket, (struct sockaddr*)&addr, &addrlen);
	if (m_clientSocket == SOCKET_ERROR)
	{
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
		return 0;
	}
	return 1;
}

bool CTcpServer::SendMsg(const char* strMsg)
{
	if (!m_clientSocket)
	{
		return false;
	}
	if (send(m_clientSocket, strMsg,sizeof(strMsg), 0) != INVALID_SOCKET)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CTcpServer::RecvMsg()
{
	if (!m_clientSocket)
	{
		return false;
	}
	const int iBufSize = 1024;
	char recvBuf[iBufSize] = { 0, };
	auto iRecvSize = recv(m_clientSocket, recvBuf, iBufSize, 0);//若不支持C++11及以上，auto改为int
	if (iRecvSize <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
