// Server2.0.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define WIN32_LEAN_AND_MEAN
#define SOCKET int  //VS2015�Ѿ�����õģ�����ֱ����

#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

using namespace::std;
#pragma comment(lib,"ws2_32.lib")


void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else {
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}
	//������˵�ַ��Ϣ

}

int main()
{
	//WORD ver = MAKEWORD(2, 2);//����API2������2.x�汾
	//WSADATA dat;
	//WSAStartup(ver, &dat);//����
	//					  //�����׽���

	initialization();
	SOCKET  seradd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(2345);
	_sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���ʱ����κ������ַ�����ԣ�����Ҳ���Ը�Ϊ������ĳ���ض���ַ
	if (SOCKET_ERROR == bind(seradd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		cout << "��ʧ��" << endl;
	}
	//����
	if (SOCKET_ERROR == listen(seradd, 5))
	{
		cout << "��ʧ��" << endl;
	}

	cout << "��������ڼ������ӣ����Ժ�...." << endl;

	//����
	sockaddr_in clentAddr = {};//���ø�ֵ�ˣ���Ϊ�Ǽ������ģ����Ǹ�����

	int nAddrLen = sizeof(sockaddr_in);//��������
	SOCKET _cSock = INVALID_SOCKET;
	//����������ҪԤ��˵��
	char msgbuf[] = "XXXXXXX";
	_cSock = accept(seradd, (sockaddr *)&clentAddr,&nAddrLen);

	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "��ȡʧ��" << endl;
	}

	cout << "���ӽ�����׼����������" << endl;

	char _recvBuf[128] = {};
	while (true)
	{
		//�Ƚ��ܿͻ������ݣ�������
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			cout << "�ͻ����Ѿ��˳����������" << endl;
		}
		//��������������
		if (0 == strcmp(_recvBuf, "XXX"))
		{
		}
		else if (0 == strcmp(_recvBuf, "exit"))
		{
			cout << "�ͻ������˳�" << endl;
		}
		else
		{
			send(_cSock, msgbuf, sizeof(msgbuf) + 1, 0);//����msgbuf�е����ݡ��������ȷ����Ӧ��ÿ�ζ����¼���
		}

	}
	closesocket(seradd);
	WSACleanup();//�ر�
}
