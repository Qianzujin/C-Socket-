// Server3.0.cpp : 定义控制台应用程序的入口点。
//

#define WIN32_LEAN_AND_MEAN
#define SOCKET int  //VS2015已经定义好的，可以直接用

#include "stdafx.h"
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<string.h>

#pragma comment(lib,"ws2_32.lib")
//using namespace::std;

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
		std::cout << "初始化套接字库失败！" << std::endl;
	}
	else {
		std::cout << "初始化套接字库成功！" << std::endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "套接字库版本号不符！" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "套接字库版本正确！" << std::endl;
	}
	//填充服务端地址信息

}
int main()
{
	//初始化套接字库
	initialization();

	SOCKET  seradd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind
	SOCKADDR_IN _sin = {};
	_sin.sin_family = AF_INET;
	//sin_port和sin_addr都必须是网络字节序（NBO），一般可视化的数字都是主机字节序。所以需要转换host to net unsigned.数字可改 
	_sin.sin_port = htons(1234);
	_sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//访问本机任何网络地址都可以，具体也可以改为本机的某个特定地址
	if (SOCKET_ERROR == ::bind(seradd, (sockaddr *)&_sin, sizeof(_sin)))
	{
		std::cout << "绑定失败" << std::endl;
	}
	//监听
	if (SOCKET_ERROR == listen(seradd, 5))
	{
		std::cout << "绑定失败" << std::endl;
	}

	std::cout << "服务端正在监听连接，请稍候...." << std::endl;

	//接受
	sockaddr_in clentAddr = {};//不用赋值了，因为是监听到的，不是给定的
	int nAddrLen = sizeof(sockaddr_in);//给定长度
	SOCKET _cSock = INVALID_SOCKET;
	//发送内容需要预先说定
	char msgbuf[] = "XXXXXXX";
	_cSock = accept(seradd, (sockaddr *)&clentAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "获取失败" << std::endl;
	}
	else
	{
		std::cout << "连接建立，准备接受数据" << std::endl;
	}

	

	char _recvBuf[128] = {};
	
	while (true)
	{
		//先接受客户端数据（新增）
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		//if (nLen <= 0)
		//{
		//	cout << "客户端已经退出，任务结束" << endl;
		//}
		//新增：处理请求
		if (0 == strcmp(_recvBuf, "XXX"))
		{
			DataPackage  msgBuf = { 10,"Tom" };
			send(_cSock, (char *)&msgBuf, sizeof(msgBuf), 0);
		}
		else if (0 == strcmp(_recvBuf, "exit"))
		{
			std::cout << "客户端已退出" << std::endl;
		}
		else
		{
			
			send(_cSock, msgbuf, strlen(msgbuf) + 1, 0);//发送msgbuf中的内容。如果长度确定则不应该每次都重新计算
		}

		if (nLen > 0)
		{
			std::cout << "接受到的数据：" << _recvBuf << std::endl;
		}
	}
	closesocket(seradd);
	WSACleanup();//关闭
	return 0;
}
