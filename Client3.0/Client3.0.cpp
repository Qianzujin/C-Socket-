// Client3.0.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<string.h>
#include<iostream>

#pragma comment(lib,"ws2_32.lib")
using namespace::std;

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
	//��ʼ���׽��ֿ�
	initialization();

	SOCKET  cliadd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(1234);//�ͷ�ȥ�䲻ͬ
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//�󶨷�������ַ
	if (SOCKET_ERROR == connect(cliadd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		cout << "����ʧ��" << endl;
	}
	else 
	{
		cout << "���ӳɹ�" << endl;
	}
	//�����������û���������

	while (true)
	{
		char inputBuf[128] = {};
		cout << "�뷢����Ϣ��" << endl;
		cin >> inputBuf;
		//�����û����ݣ������exit���˳�
		if (0 == strcmp(inputBuf, "exit"))
		{
			send(cliadd, inputBuf, sizeof(inputBuf) + 1, 0);
			break;
		}
		else
		{
			send(cliadd, inputBuf, sizeof(inputBuf) + 1, 0);
		}
		//��������
		char recvBuf[128] = {};//�����Խ��ܵ��ַ�
		int nLen = recv(cliadd, recvBuf, sizeof(recvBuf),0);
		if (nLen > 0)
		{
			DataPackage *recv = (DataPackage *)recvBuf;//������
			cout << "���ܵ������ݣ�����=" << recv->age << "����Ϊ��" << recv->name << endl;//�������
		}
	}
	closesocket(cliadd);
	WSACleanup();//�ر�
	return 0;
}

