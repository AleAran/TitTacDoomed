#include "Common\BaseNetworkLogic.h"
#include "Common/Packets.h"
#include <iostream>
#include <thread>

using namespace std;
#define DEFAULT_BUFLEN 512

unsigned __stdcall ClientHandler(void* data)
{
	Packet recvbuf[sizeof(Packet)];
	int recvbuflen = sizeof(Packet);

	SOCKET mClientSocket = (SOCKET)data;

	int mConnectionValue = 1;
	int mSendValue;

	memset(&recvbuf, 0, recvbuflen);
	mConnectionValue = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
	if (mConnectionValue > 0) {
		cout<<"Sucess! bytes received = " + mConnectionValue<<endl;

		// Echo the buffer back to the sender
		mSendValue = send(mClientSocket, (char*)&recvbuf, mConnectionValue, 0);
		if (mSendValue == SOCKET_ERROR) {
			printf("Error! bytes not sent");
		}
		cout << "Sucess! bytes received = " + mSendValue << endl;
	}

	else {
		printf("Error! " + WSAGetLastError());
		return 1;
	}
}


int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;

	struct sockaddr_in mServerAddr, mClientAddr;
	SOCKET mServerSocket, mClientSocket;

	//Integer that contains the length of structure pointed to by the addr parameter.
	int mSizeOfAddrStruct = sizeof(struct addrinfo);


	int mConnectionValue = 1;

#pragma endregion

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(8080);

	//Create and bind socket
	mServerSocket = mSL.createSocket();
	mSL.BindSocket(mServerSocket, mServerAddr);
	int senderSize = sizeof(mClientAddr);

	//We put this socket on listening mode
	listen(mServerSocket, 3);

	// Accept a client socket
	while (mClientSocket = accept(mServerSocket, NULL, NULL)) 
	{
		unsigned threadID;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ClientHandler, (void*)mClientSocket, 0, &threadID);
	}

	closesocket(mClientSocket);

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}
