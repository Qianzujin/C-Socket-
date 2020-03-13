// UDPClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <thread>

#include "Client.h"

using namespace std;

int main()
{
	CClient client_UDP;

	thread RecProc(&CClient::RecMsg, &client_UDP);

	while (1)
	{
		cout << "Please input content:" << endl;
		string content;
		cin >> content;

		client_UDP.SendMsg(content.c_str());
	}


	RecProc.join();
	cout << "I love china!" << endl;
	system("pause");
	return 0;
}
