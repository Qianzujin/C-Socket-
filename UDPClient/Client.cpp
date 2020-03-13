#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"

#include "Client.h"

#include <iostream>

#pragma comment(lib, "ws2_32.lib")

CClient::CClient()
{
	//必须进行如下初始化， 否则socket()会返回10093错误
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) //通过一个进程初始化ws2_32.dll
	{
		std::cout << "Initialize WSA failed" << std::endl;
		return;
	}

	m_sockClient = socket(AF_INET, SOCK_DGRAM, 0);

	m_TargetServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	m_TargetServer.sin_family = AF_INET;
	m_TargetServer.sin_port = htons(8090);

	//不需要创建连接，直接发送消息即可

	if (m_sockClient == -1)
	{
		std::cout << "Create socket failed!" << std::endl;
		WSACleanup();
	}
	else
	{
		//发送信息与服务器建立连接(必须加)
		sendto(m_sockClient, "hello server", strlen("hello server"), 0, (sockaddr*)&m_TargetServer, sizeof(m_TargetServer));
	}
}

void CClient::SendMsg(const char sendBuf[])
{
	sendto(m_sockClient, sendBuf, strlen(sendBuf), 0, (sockaddr*)&m_TargetServer, sizeof(m_TargetServer));
}
void CClient::RecMsg()
{
	char recBuf[1025];

	while (1)
	{
		//std::cout << "Begin rec...(client)" << std::endl;
		int len = recvfrom(m_sockClient, recBuf, 1024, 0, 0, 0);
		if (len > 0)
		{
			recBuf[len] = '\0';
			std::cout << "Server say: " << recBuf << std::endl;
		}
	}

}
CClient::~CClient()
{
	closesocket(m_sockClient);
	WSACleanup();
}