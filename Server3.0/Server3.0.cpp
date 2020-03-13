// Server3.0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define WIN32_LEAN_AND_MEAN
#define SOCKET int  //VS2015�Ѿ�����õģ�����ֱ����

#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

#pragma comment(lib,"ws2_32.lib")
//using namespace::std;

struct DataPackage  //����򵥵�����Ϊ������������ʵ�ʹ����������Զ��嶨�ơ�
{
	int age;
	char name[32];
};

void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		std::cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << std::endl;
	}
	else {
		std::cout << "��ʼ���׽��ֿ�ɹ���" << std::endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "�׽��ֿ�汾�Ų�����" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "�׽��ֿ�汾��ȷ��" << std::endl;
	}
	//������˵�ַ��Ϣ

}
int main()
{
	//��ʼ���׽��ֿ�
	initialization();

	SOCKET  seradd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(1234);
	_sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���ʱ����κ������ַ�����ԣ�����Ҳ���Ը�Ϊ������ĳ���ض���ַ
	if (SOCKET_ERROR == ::bind(seradd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "��ʧ��" << std::endl;
	}
	//����
	if (SOCKET_ERROR == listen(seradd, 5))
	{
		std::cout << "��ʧ��" << std::endl;
	}

	std::cout << "��������ڼ������ӣ����Ժ�...." << std::endl;

	//����
	sockaddr_in clentAddr = {};//���ø�ֵ�ˣ���Ϊ�Ǽ������ģ����Ǹ�����
	int nAddrLen = sizeof(sockaddr_in);//��������
	SOCKET _cSock = INVALID_SOCKET;
	//����������ҪԤ��˵��
	char msgbuf[] = "XXXXXXX";
	_cSock = accept(seradd, (sockaddr *)&clentAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "��ȡʧ��" << std::endl;
	}
	else
	{
		std::cout << "���ӽ�����׼����������" << std::endl;
	}

	

	char _recvBuf[128] = {};
	
	while (true)
	{
		//�Ƚ��ܿͻ������ݣ�������
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		//if (nLen <= 0)
		//{
		//	cout << "�ͻ����Ѿ��˳����������" << endl;
		//}
		//��������������
		if (0 == strcmp(_recvBuf, "XXX"))
		{
			DataPackage  msgBuf = { 10,"Tom" };
			send(_cSock, (char *)&msgBuf, sizeof(msgBuf), 0);
		}
		else if (0 == strcmp(_recvBuf, "exit"))
		{
			std::cout << "�ͻ������˳�" << std::endl;
		}
		else
		{
			
			send(_cSock, msgbuf, strlen(msgbuf) + 1, 0);//����msgbuf�е����ݡ��������ȷ����Ӧ��ÿ�ζ����¼���
		}

		if (nLen > 0)
		{
			std::cout << "���ܵ������ݣ�" << _recvBuf << std::endl;
		}
	}
	closesocket(seradd);
	WSACleanup();//�ر�
	return 0;
}
