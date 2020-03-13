// Client4.0.cpp : 定义控制台应用程序的入口点。
//
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"


#include<WinSock2.h>
#include<windows.h>
#include<string.h>
#include<iostream>


#pragma comment(lib,"ws2_32.lib")
using namespace std;

//新增
enum CMD //里面包含cmd常见的命令
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_GETINFO
};
struct DataHeader
{
	int dataLength;//描述数据长度
	int cmd;//描述数据包的作用
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
struct DataPackage  //以最简单的情形为例，可以依据实际工程需求做自定义定制。
{
	int age;
	char name[32];
};
int main()
{
	WORD ver = MAKEWORD(2, 2);//调用API2代创建2.x版本
	WSADATA dat;
	WSAStartup(ver, &dat);//启动
	SOCKET  cliadd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port和sin_addr都必须是网络字节序（NBO），一般可视化的数字都是主机字节序。所以需要转换host to net unsigned.数字可改 
	_sin.sin_port = htons(1234);//和拂去其不同
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//绑定服务器地址
	if (SOCKET_ERROR == connect(cliadd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "连接失败" << endl;
	}

	cout << "连接成功" << endl;

	//新增，接收用户输入数据
	while (true)
	{
		char inputBuf[128] = {};
		cout  << "请输入数据：" << endl;
		cin >> inputBuf;
		//接收用户数据，如果是exit就退出
		if (0 == strcmp(inputBuf, "exit"))
		{
			cout << "收到退出命令，已退出。";
				break;
		}
		else if (0 == strcmp(inputBuf, "login"))
		{
			Login login = { "wushuomin","enter" };
			DataHeader header = { sizeof(login) ,CMD_LOGIN };

			send(cliadd, (const char *)&header, sizeof(header), 0);

			send(cliadd, (const char *)&login, sizeof(login), 0);
			//接受到服务器的登陆结果
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
		//接受数据
		char recvBuf[128] = {};//最多可以接受的字符
		int nLen = recv(cliadd, recvBuf,sizeof(recvBuf), 0);
		if (nLen > 0)
		{
			DataPackage *recv = (DataPackage *)recvBuf;//有问题
			cout << "接受到的数据：年龄=" << recv->age << "姓名为：" << recv->name << endl;//有问题的
		}
	}
	closesocket(cliadd);
	WSACleanup();//关闭
	return 0;
}


