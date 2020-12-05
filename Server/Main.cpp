#include "Common\BaseNetworkLogic.h"
#include "Common/Packets.h"
#include "Game.h"
#include <iostream>
#include <thread>
#include <fcntl.h>

using namespace std;

struct GameBundle
{
	Game* ticTacToe;
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int clientSize;
	GenericPacket recvbuf;
};


bool Broadcast(MatchedPlayers players, GenericPacket recvbuf, int recvbuflen)
{
	GameState state = (GameState)recvbuf.gameState;
		for (int i = 0; i < 2; i++)
		{
			LoginInfo ClientInfo = players.mMatchedPlayers[i]->GetLogInInfo();

			if (state == GameState::COIN_TOSS)
			{
				memset(&recvbuf, 0, sizeof(recvbuf));
				string position = players.mMatchedPlayers[i]->GetPosition() == PlayerPosition::PLAYER_ONE ? "One" : "Two";
				memcpy(recvbuf.textAux, position.c_str(), sizeof(position));
			}

			int mSendValue = sendto(ClientInfo.clientSocket, (char*)&recvbuf, recvbuflen, 0, (struct sockaddr*)&ClientInfo.mClientAddr, ClientInfo.clientSize);
			if (mSendValue == SOCKET_ERROR) {
				printf("Error! bytes not sent");
			}

		}

	return true;
}

unsigned __stdcall GenericPacketHanlder(void* data)
{

	int recvbuflen = sizeof(GenericPacket);


	GameBundle* mBundle = (GameBundle*)data;
	GenericPacket* recvbuf = &mBundle->recvbuf;
	SOCKET mClientSocket = mBundle->clientSocket;
	sockaddr_in mClientAddr = mBundle->clientAddr;

	int clientSize = mBundle->clientSize;
	Game* mGame = mBundle->ticTacToe;

	Match* match;
	int mConnectionValue = 1;
	int mSendValue;
	GameState state = (GameState)recvbuf->gameState;
	switch (state)
	{
	case GameState::CONNECTED:
		memset(&recvbuflen, 0, sizeof(GenericPacket));
		recvbuf->gameState = 4; //GameState::LOBBY;
		recvbuf->aux = 0;

		unsigned char buffer[sizeof(GenericPacket)];
		memcpy(&buffer, &recvbuf, sizeof(recvbuf));


		mSendValue = sendto(mClientSocket, (char*)&recvbuf, recvbuflen, 0, (struct sockaddr*)&mClientAddr, clientSize);

		if (mSendValue == SOCKET_ERROR) {
			printf("Error! bytes not sent");
		}

		break;

	case GameState::PLAYER_ONE:

		match = mGame->GetMatch(recvbuf->textAux);
		match->Input(recvbuf->aux, match->GetPlayer(recvbuf->textAux));

		if (match->CheckResult())
		{
			recvbuf->gameState = 5;// GameState::FINISH;
		}
		else
		{
			memset(&recvbuf, 0, sizeof(recvbuf));
			memcpy(recvbuf->textAux, "X", sizeof("X"));
			recvbuf->gameState = 3;// GameState::PLAYER_TWO;
		}

		Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);
		break;

	case GameState::PLAYER_TWO:

		match = mGame->GetMatch(recvbuf->textAux);
		match->Input(recvbuf->aux, match->GetPlayer(recvbuf->textAux));

		if (match->CheckResult())
		{
			recvbuf->gameState = 5;// GameState::FINISH;
		}
		else
		{
			memset(&recvbuf, 0, sizeof(recvbuf));
			memcpy(recvbuf->textAux, "Y", sizeof("Y"));
			recvbuf->gameState = 2;// GameState::PLAYER_ONE;
		}

		Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);

		break;

	case GameState::REGISTER_PLAYER:
		if (mGame->mLobby.RegisterPlayer(recvbuf->textAux, mClientSocket, clientSize, mClientAddr))
		{
			mGame->CreateMatch(mGame->mLobby.GetMatchedPlayers());
			match = mGame->GetMatch(recvbuf->textAux);
			recvbuf->gameState = 1; //COIN_TOSS
			recvbuf->aux = 0;
			Broadcast(match->GetPlayers(), *recvbuf, sizeof(GenericPacket));
		}
		break;

	case GameState::REENTER_PLAYER:
		mGame->mLobby.ReenterPlayer(recvbuf->textAux);
		break;

	case GameState::START_GAME:

		mGame->CreateMatch(mGame->mLobby.GetMatchedPlayers());
		match = mGame->GetMatch(recvbuf->textAux);
		recvbuf->gameState = 1;
		recvbuf->aux = 0;
		Broadcast(match->GetPlayers(), *recvbuf, sizeof(recvbuf));

		break;

	case GameState::FINISH:

		break;

	default:
		break;
	}

	//If we don't end the thread, things could get messy;
	_endthreadex(0);
	return 1;
}

int main(int argc, char* argv[])
{
#pragma region Members
	GenericPacket recvbuf[sizeof(GenericPacket)];
	int recvbuflen = sizeof(GenericPacket);

	BaseNetworkLogic mSL;
	Game mTicTacToe;
	
	struct sockaddr_in mServerAddr, mClientAddr;
	SOCKET mServerSocket, mClientSocket;

	//Integer that contains the length of structure pointed to by the addr parameter.
	int mSizeOfAddrStruct = sizeof(struct addrinfo);
	int mConnectionValue = 1;

	GameBundle mBundle;
	mBundle.ticTacToe = &mTicTacToe;

#pragma endregion

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(8080);

	//Create and bind socket
	mServerSocket = mSL.createSocket();
	mSL.BindSocket(mServerSocket, mServerAddr);
	int clientSize = sizeof(mClientAddr);

	//We put this socket on listening mode
	listen(mServerSocket, 3);


	unsigned long mode = 1;
	ioctlsocket(mServerSocket, FIONBIO, &mode);

	// Accept a client socketfcntl() 
	while (mClientSocket = accept(mServerSocket, (struct sockaddr*)&mClientAddr, &clientSize))
	{
		mConnectionValue = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
		if (mConnectionValue>0)
		{
			mBundle.clientSocket = mClientSocket;
			mBundle.clientAddr = mClientAddr;
			mBundle.clientSize = clientSize;

			unsigned threadID;
			HANDLE Thread = (HANDLE)_beginthreadex(NULL, 0, &GenericPacketHanlder, (void*)&mBundle, 0, &threadID);
		}
	}

	closesocket(mClientSocket);

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}