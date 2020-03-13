// Server4.0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define WIN32_LEAN_AND_MEAN
#define SOCKET int  //VS2015�Ѿ�����õģ�����ֱ����

#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;



// ����
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
//*************************************
int main()
{
	WORD ver = MAKEWORD(2, 2);//����API2������2.x�汾
	WSADATA dat;
	WSAStartup(ver, &dat);//����
						  //�����׽���
	SOCKET  seradd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���������Ҫת��host to net unsigned.���ֿɸ� 
	_sin.sin_port = htons(1234);
	_sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���ʱ����κ������ַ�����ԣ�����Ҳ���Ը�Ϊ������ĳ���ض���ַ
	if (SOCKET_ERROR == bind(seradd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "��ʧ��" << endl;
	}
	//����
	if (SOCKET_ERROR == listen(seradd, 5))
	{
		std::cout << "��ʧ��" << endl;
	}
	//����
	cout << "�ȴ�����..." << endl;
	sockaddr_in clentAddr = {};//���ø�ֵ�ˣ���Ϊ�Ǽ������ģ����Ǹ�����
	int nAddrLen = sizeof(sockaddr_in);//��������
	SOCKET _cSock = INVALID_SOCKET;
	//����������ҪԤ��˵��
	char msgbuf[] = "XXXXXXX";
	_cSock = accept(seradd, (sockaddr *)&clentAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "��ȡʧ��" << endl;
	}

	cout << "���ӳɹ�..." << endl;

	char _recvBuf[128] = {};
	while (true)
	{
		//�������Ƚ��ܰ�ͷ
		DataHeader header = {};
		int nLen = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			//cout << "�ͻ����Ѿ��˳����������" << endl;
		}
		cout << "�յ������ݳ���Ϊ��" << header.dataLength << endl;
		//��������������
		switch (header.cmd)
		{
			case CMD_LOGIN:
			{
				cout << "�յ�������Ϊ��Login" << endl;
				Login login = {};
				int nLen = recv(_cSock, (char *)&login, sizeof(Login), 0);
				if (nLen <= 0)
				{
					cout << "�ͻ����Ѿ��˳����������" << endl;
				}
				else
				{
					//�ж��û����������Ƿ���ȷ��
					if (0 == strcmp(login.username, "wushuomin") && 0 == strcmp(login.password, "enter"))
					{
						LoginResult msgBuf = { 1 };
						DataPackage sendBuf = { 1,"wangjinjian" };
						//����Ҳ���ȷ���ͷ
						//send(_cSock, (char *)&header, sizeof(DataHeader), 0);
						send(_cSock, (char *)&sendBuf, sizeof(sendBuf), 0);
					}
					else
					{
						LoginResult msgBuf = { 0 };
						send(_cSock, (char *)&msgBuf, sizeof(msgBuf), 0);
					}
				}
			break;
			}
		
			case CMD_LOGOUT:
			{
				break;
			}
		
			case CMD_GETINFO:
			{
				break;
			}
		
			default:
				cout << "�ͻ����Ѿ��˳����������" << endl;
			//����heder�ĳ���Ϊ0������һ���������͡�
			}

	}
	closesocket(seradd);
	WSACleanup();//�ر�
	return 0;
}

