#include "BaseNetworkLogic.cpp"
#include "Packets.h"
#include <iostream>

#define DEFAULT_BUFLEN 512
using namespace std;

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;
	//Match mTicTacToe;

	sockaddr_in mServerAddr;
	SOCKET mClientSocket = INVALID_SOCKET;
	GenericPacket recvbuf[sizeof(GenericPacket)];
	int recvbuflen = sizeof(GenericPacket);

	PlayerPacket playerbuf[sizeof(PlayerPacket)];
	int playerbuflen = sizeof(PlayerPacket);
	int mEndSession = 0;
#pragma endregion

#pragma region Connect To server
	mServerAddr.sin_port = htons(8080);
	mServerAddr.sin_family = AF_INET;

	int mConnectionValue = 1;
	int mSendValue;

	inet_pton(AF_INET, "127.0.0.1", &mServerAddr.sin_addr);

	mClientSocket = mSL.createSocket();

	memset(&recvbuf, 0, recvbuflen);

	if (connect(mClientSocket, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) {
		closesocket(mClientSocket);
		mClientSocket = INVALID_SOCKET;
		return 1;
	}
#pragma region Connect To server


	recvbuf->aux = 0;
	recvbuf->gameState = GameState::CONNECTED;

	// Send an initial buffer
	if (send(mClientSocket, (char*)&recvbuf, recvbuflen, 0) == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(mClientSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		mConnectionValue = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
		if (mConnectionValue > 0)
			printf("Bytes received: %d\n", mConnectionValue);
		switch (recvbuf->gameState)
		{
		case GameState::LOBBY:
			cout << ":::LOBBY:::" << endl	
				 << "Enter Name" << endl;

			cin >> playerbuf->name;
		
			cout.clear();
			cout << ":::LOBBY:::" << endl
				<< "Waiting For Players..." << endl;

			playerbuf->cmdState = Command::REGISTER_PLAYER;
			send(mClientSocket, (char*)&playerbuf, playerbuflen, 0);

			break;

		case GameState::GAME:
			break;
		case GameState::FINISH:
			break;
		default:
			break;
		}

	} while (mEndSession < 1 );

	//mTicTacToe.StartMatch(mLobby.GetMatchedPlayers());

	/*mTicTacToe.Draw();
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
	while (!mTicTacToe.CheckResult());*/


	return 0;
}