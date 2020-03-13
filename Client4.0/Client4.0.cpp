// Client4.0.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"


#include<WinSock2.h>
#include<windows.h>
#include<string.h>
#include<iostream>


#pragma comment(lib,"ws2_32.lib")
using namespace std;

//����
enum CMD //�������cmd����������
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_GETINFO
};
struct DataHeader
{
	int dataLength;//�������ݳ���
	int cmd;//�������ݰ�������
};
struct Login
{
	char username[32];
	char password[32];
};
struct LoginResult
{
	int res;
};
struct LogoutResult
{
	int res;
};
struct Logout
{
	char username[32];
};
struct DataPackage  //����򵥵�����Ϊ������������ʵ�ʹ����������Զ��嶨�ơ�
{
	int age;
	char name[32];
};
int main()
{
	WORD ver = MAKEWORD(2, 2);//����API2������2.x�汾
	WSADATA dat;
	WSAStartup(ver, &dat);//����
	SOCKET  cliadd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(1234);//�ͷ�ȥ�䲻ͬ
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//�󶨷�������ַ
	if (SOCKET_ERROR == connect(cliadd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "����ʧ��" << endl;
	}

	cout << "���ӳɹ�" << endl;

	//�����������û���������
	while (true)
	{
		char inputBuf[128] = {};
		cout  << "���������ݣ�" << endl;
		cin >> inputBuf;
		//�����û����ݣ������exit���˳�
		if (0 == strcmp(inputBuf, "exit"))
		{
			cout << "�յ��˳�������˳���";
				break;
		}
		else if (0 == strcmp(inputBuf, "login"))
		{
			Login login = { "wushuomin","enter" };
			DataHeader header = { sizeof(login) ,CMD_LOGIN };

			send(cliadd, (const char *)&header, sizeof(header), 0);

			send(cliadd, (const char *)&login, sizeof(login), 0);
			//���ܵ��������ĵ�½���
			DataHeader retheader = {};
			LoginResult res = {};
			//recv(cliadd, (char *)retheader,sizeof(retheader), 0);
			//recv(cliadd, (char *)res,sizeof(res), 0);
		}
		else if (0 == strcmp(inputBuf, "logout"))
		{

		}
		else
		{
		//	XXX;
		}
		//��������
		char recvBuf[128] = {};//�����Խ��ܵ��ַ�
		int nLen = recv(cliadd, recvBuf,sizeof(recvBuf), 0);
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


