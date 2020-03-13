// Server4.0.cpp : 定义控制台应用程序的入口点。
//

#define WIN32_LEAN_AND_MEAN
#define SOCKET int  //VS2015已经定义好的，可以直接用

#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;



// 新增
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
//*************************************
int main()
{
	WORD ver = MAKEWORD(2, 2);//调用API2代创建2.x版本
	WSADATA dat;
	WSAStartup(ver, &dat);//启动
						  //创建套接字
	SOCKET  seradd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port和sin_addr都必须是网络字节序（NBO），一般可视化的数字都是主机字节序。所以需要转换host to net unsigned.数字可改 
	_sin.sin_port = htons(1234);
	_sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//访问本机任何网络地址都可以，具体也可以改为本机的某个特定地址
	if (SOCKET_ERROR == bind(seradd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "绑定失败" << endl;
	}
	//监听
	if (SOCKET_ERROR == listen(seradd, 5))
	{
		std::cout << "绑定失败" << endl;
	}
	//接受
	cout << "等待连接..." << endl;
	sockaddr_in clentAddr = {};//不用赋值了，因为是监听到的，不是给定的
	int nAddrLen = sizeof(sockaddr_in);//给定长度
	SOCKET _cSock = INVALID_SOCKET;
	//发送内容需要预先说定
	char msgbuf[] = "XXXXXXX";
	_cSock = accept(seradd, (sockaddr *)&clentAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "获取失败" << endl;
	}

	cout << "连接成功..." << endl;

	char _recvBuf[128] = {};
	while (true)
	{
		//修正，先接受包头
		DataHeader header = {};
		int nLen = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			//cout << "客户端已经退出，任务结束" << endl;
		}
		cout << "收到的数据长度为：" << header.dataLength << endl;
		//修正：处理请求
		switch (header.cmd)
		{
			case CMD_LOGIN:
			{
				cout << "收到的命令为：Login" << endl;
				Login login = {};
				int nLen = recv(_cSock, (char *)&login, sizeof(Login), 0);
				if (nLen <= 0)
				{
					cout << "客户端已经退出，任务结束" << endl;
				}
				else
				{
					//判断用户名和密码是否正确；
					if (0 == strcmp(login.username, "wushuomin") && 0 == strcmp(login.password, "enter"))
					{
						LoginResult msgBuf = { 1 };
						DataPackage sendBuf = { 1,"wangjinjian" };
						//发送也是先发包头
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
				cout << "客户端已经退出，任务结束" << endl;
			//这里heder的长度为0，返回一个错误类型。
			}

	}
	closesocket(seradd);
	WSACleanup();//关闭
	return 0;
}

