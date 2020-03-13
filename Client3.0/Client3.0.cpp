// Client3.0.cpp : 定义控制台应用程序的入口点。
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

struct DataPackage  //以最简单的情形为例，可以依据实际工程需求做自定义定制。
{
	int age;
	char name[32];
};


void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息

}
int main()
{
	//初始化套接字库
	initialization();

	SOCKET  cliadd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port和sin_addr都必须是网络字节序（NBO），一般可视化的数字都是主机字节序。所以需要转换host to net unsigned.数字可改 
	_sin.sin_port = htons(1234);//和拂去其不同
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//绑定服务器地址
	if (SOCKET_ERROR == connect(cliadd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		cout << "连接失败" << endl;
	}
	else 
	{
		cout << "连接成功" << endl;
	}
	//新增，接收用户输入数据

	while (true)
	{
		char inputBuf[128] = {};
		cout << "请发送信息：" << endl;
		cin >> inputBuf;
		//接收用户数据，如果是exit就退出
		if (0 == strcmp(inputBuf, "exit"))
		{
			send(cliadd, inputBuf, sizeof(inputBuf) + 1, 0);
			break;
		}
		else
		{
			send(cliadd, inputBuf, sizeof(inputBuf) + 1, 0);
		}
		//接受数据
		char recvBuf[128] = {};//最多可以接受的字符
		int nLen = recv(cliadd, recvBuf, sizeof(recvBuf),0);
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

