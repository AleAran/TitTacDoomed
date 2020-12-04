#include"BaseNetworkLogic.h"

BaseNetworkLogic::BaseNetworkLogic()
{
	InitWsa();
}

BaseNetworkLogic::~BaseNetworkLogic()
{
}

bool BaseNetworkLogic::InitWsa()
{
	//This function, as it says, initialices wsa. THe first parameter set it's version, the second, the data structure to be used.
	if (WSAStartup(MAKEWORD(2, 2), &mWSA) != 0)
	{
		printf("Error! " + WSAGetLastError());
		return false;
	}

	return true;
}

SOCKET BaseNetworkLogic::createSocket()
{
	SOCKET newSocket;

	//Socket funcition, it set what IPv we are using, UDP/TCP and Protocol
	if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Error! " + WSAGetLastError());
		return false;
	}

	return newSocket;
}

bool BaseNetworkLogic::BindSocket(SOCKET socket, sockaddr_in sockAddress)
{
	//Bind socket, allows us to well, bind, a socket with an address and port.
	if (bind(socket, (struct sockaddr*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
	{
		printf("Error! " + WSAGetLastError());
		return false;
	}
	return true;
}
