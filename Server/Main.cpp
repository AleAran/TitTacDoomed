#include "Common\BaseNetworkLogic.h"
#include "Common/Packets.h"
#include <iostream>
#include <thread>

using namespace std;
#define DEFAULT_BUFLEN 512

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;

	struct sockaddr_in mServerAddr, mClientAddr;
	SOCKET mServerSocket, mClientSocket;

	//Integer that contains the length of structure pointed to by the addr parameter.
	int mSizeOfAddrStruct = sizeof(struct addrinfo);
	Packet recvbuf[sizeof(Packet)];
	int recvbuflen = sizeof(Packet);

	int mConnectionValue = 0;
	int mSendValue;

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
	mClientSocket = accept(mServerSocket, NULL, NULL);
	if (mClientSocket == INVALID_SOCKET) {
		return 1;
	}
	
	do {
		memset(&recvbuf, 0, recvbuflen);
		mConnectionValue = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
		if (mConnectionValue > 0) {
			printf("Sucess! bytes received = " + mConnectionValue);

			// Echo the buffer back to the sender
			mSendValue = send(mClientSocket, (char*)&recvbuf, mConnectionValue, 0);
			if (mSendValue == SOCKET_ERROR) {
				printf("Error! bytes not sent");
			}
			printf("Sucess! bytes received = " + mSendValue);
		}

		else {
			printf("Error! " + WSAGetLastError());
			return 1;
		}


	} while (mConnectionValue > 0);


	closesocket(mClientSocket);

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}

