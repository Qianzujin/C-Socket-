#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"

#include "Server_UDP.h"

#include <iostream>

#pragma comment(lib, "ws2_32.lib")


//���캯��,��ʼ��m_terminal����Ϊfalse
CServer_UDP::CServer_UDP() :m_terminal(false)
{
	//����������³�ʼ���� ����socket()�᷵��10093����
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2); //ȷ��socket�汾
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) //ͨ��һ�����̳�ʼ��ws2_32.dll
	{
		std::cout << "Initialize WSA failed" << std::endl;
		return;
	}
	//��ʼ��UDDP�׽���
	m_sServer = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in m_SocAddrserver;
	m_SocAddrserver.sin_addr.S_un.S_addr = 0;//htonl(INADDR_ANY);
	m_SocAddrserver.sin_family = AF_INET;
	m_SocAddrserver.sin_port = htons(8090);

	//��IP/Port ����Ҫlisten���� Ҳ����Ҫaccept�������� ֱ��recv������Ϣ
	int ret = bind(m_sServer, (sockaddr*)&m_SocAddrserver, sizeof(m_SocAddrserver));
	if (ret == -1)
	{
		std::cout << "bind failed!" << std::endl;
		WSACleanup();
	}
	else
	{
		//�˴����븳��ֵ����Ȼ�ᵼ�·��������޷���������
		int len_Client = sizeof(sockaddr);
		char recBuf[1025];
		int len = recvfrom(m_sServer, recBuf, 1024, 0, (sockaddr*)&m_SocAddrClient, &len_Client);
		if (len > 0)
		{
			recBuf[len] = '\0';
			std::cout << "Client say:" << recBuf << std::endl;
		}
	}


}
void CServer_UDP::SendMsg(const char sendBuf[])
{
	int ret = sendto(m_sServer, sendBuf, strlen(sendBuf), 0, (sockaddr*)&m_SocAddrClient, sizeof(m_SocAddrClient));
	if (ret == -1)
	{
		std::cout << "send failed" << std::endl;
		std::cout << GetLastError() << std::endl;
	}
}
void CServer_UDP::RecMsg()
{
	char recBuf[1025];
	while (!m_terminal)
	{
		//std::cout << "Begin rec...(server)" << std::endl;
		int len = recvfrom(m_sServer, recBuf, 1024, 0, 0, 0);
		if (len > 0)
		{
			recBuf[len] = '\0';
			std::cout << "Client say:" << recBuf << std::endl;
		}
	}

}
CServer_UDP::~CServer_UDP()
{
	closesocket(m_sServer);
	WSACleanup();

}