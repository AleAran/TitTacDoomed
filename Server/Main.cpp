#include "Common\BaseNetworkLogic.h"

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;

	struct sockaddr_in mServerAddr, mClientAddr;
	SOCKET mServerSocket, mClientSocket;

	//Integer that contains the length of structure pointed to by the addr parameter.
	int mSizeOfAddrStruct = sizeof(struct addrinfo);

	const char* message = "Loud and Clear!\n";
#pragma endregion

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(80);

	//Create and bind socket
	mServerSocket = mSL.createSocket();
	mSL.BindSocket(mServerSocket, mServerAddr);

	//We put this socket on listening mode
	listen(mServerSocket, 3);

	while ((mClientSocket = accept(mServerSocket, (struct sockaddr*)&mClientAddr, &mSizeOfAddrStruct) != INVALID_SOCKET))
	{
		printf("It works!");

		//Reply!
		send(mClientSocket, message, strlen(message), 0);
	}

	if (mClientSocket == INVALID_SOCKET)
	{
		printf("Error! " + WSAGetLastError());
	}

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}

