// Client2.x.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"


#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

#pragma comment(lib,"ws2_32.lib")
using namespace::std;



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


	initialization();
	SOCKET  cliadd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(2345);//�ͷ�ȥ�䲻ͬ
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//�󶨷�������ַ
	if (SOCKET_ERROR == connect(cliadd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		cout << "����ʧ��" << endl;
	}

	cout << "���ӳɹ�" << endl;
	//�����������û���������
	while (true)
	{
		char inputBuf[128] = {};
		cout << "�����뷢����Ϣ:";
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
		int nLen = recv(cliadd, recvBuf,sizeof(recvBuf), 0);
		if (nLen > 0)
		{
			cout << "���ܵ������ݣ�" << recvBuf << endl;
		}
	}
	closesocket(cliadd);
	WSACleanup();//�ر�
}

