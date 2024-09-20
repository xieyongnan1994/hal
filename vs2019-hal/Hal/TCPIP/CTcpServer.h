#pragma once
#ifndef _CTCPSERVER_H
#define _CTCPSERVER_H
#include <WinSock2.h>
#include <thread>
#include <functional>   
#include <socketapi.h>
#pragma comment(lib,"ws2_32.lib") //包含静态库

//服务器接收消息，不主动发送消息
class CTcpServer
{
public:
	CTcpServer(CString strIp, unsigned int uPort);
	virtual ~CTcpServer();

	//初始化网络服务端
	int InitServer();

	//发送数据
	bool SendMsg(const char* strMsg);

	//接收数据并打印
	bool RecvMsg();

private:
	unsigned int m_uPort;//监听端口
	CString m_strIp;//用于监听本机指定IP地址

	SOCKET m_listenSocket = NULL;//监听套接字
	SOCKET m_clientSocket = NULL;//客户端套接字



};


#endif // !_CTCPSERVER_H

