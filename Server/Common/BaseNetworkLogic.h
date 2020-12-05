#pragma once

#include<stdio.h>
#include<winsock2.h>
#include <ws2tcpip.h>

/// <summary>
/// Socket Logic handles simple winsock stuff.
/// While yes, we can create all these functions in main.cpp, having any other function outside of Main(), is not good design. 
/// </summary>
class BaseNetworkLogic
{
public:
	BaseNetworkLogic();
	~BaseNetworkLogic();
	bool InitWsa();
	SOCKET createSocket();
	bool BindSocket(SOCKET socket, sockaddr_in server);

	WSADATA mWSA;
	SOCKET mFirstSocket;

private:

};