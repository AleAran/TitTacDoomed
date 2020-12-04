#include "BaseNetworkLogic.cpp"
#include "Packets.h"
#include <ws2tcpip.h>
#include "Lobby.h"
#define DEFAULT_BUFLEN 512

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;
	Match mTicTacToe;
	Lobby mLobby;

	sockaddr_in mServerAddr;
	SOCKET mClientSocket = INVALID_SOCKET;
	Packet recvbuf[sizeof(Packet)];
	int recvbuflen = sizeof(Packet);
	const char* sendbuf = "this is a test";
	int iResult;

#pragma endregion

	mServerAddr.sin_port = htons(8080);
	mServerAddr.sin_family = AF_INET;

	inet_pton(AF_INET, "127.0.0.1", &mServerAddr.sin_addr);

	mClientSocket = mSL.createSocket();

	recvbuf->aux = 9;
	recvbuf->cmdState = State::LOBBY;

	memset(&recvbuf, 0, recvbuflen);

	iResult = connect(mClientSocket, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr));
	if (iResult == SOCKET_ERROR) {
		closesocket(mClientSocket);
		mClientSocket = INVALID_SOCKET;
		return 1;
	}

	// Send an initial buffer
	iResult = send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(mClientSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	mLobby.RegisterPlayer("Hola");
	mLobby.RegisterPlayer("Hola2");

	mTicTacToe.StartMatch(mLobby.GetMatchedPlayers());

	mTicTacToe.Draw();
	int position;

	do
	{
		cout << "Player One's Turn" << endl;
		cin >> position;
		mTicTacToe.Input(position, mTicTacToe.GetPlayer("Hola"));
		cout.clear();
		mTicTacToe.Draw();


		cout << "Player Two's Turn" << endl;
		cin >> position;
		mTicTacToe.Input(position, mTicTacToe.GetPlayer("Hola2"));
		cout.clear();
		mTicTacToe.Draw();
	}
	while (!mTicTacToe.CheckResult());


	return 0;
}