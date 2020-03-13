// UDPServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <string>

#include "Server_UDP.h"

using namespace std;

int main()
{
	CServer_UDP server_UDP;

	thread recProc(&CServer_UDP::RecMsg, &server_UDP);

	while (1)
	{
		cout << "Pleaes input content:" << endl;
		string content;
		cin >> content;
		server_UDP.SendMsg(content.c_str());
	}

	recProc.join();
	cout << "I love china!" << endl;
	system("pause");
	return 0;
}