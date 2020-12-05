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
	PlayerPacket playerbuf;
};


bool Broadcast(MatchedPlayers players, GenericPacket recvbuf, int recvbuflen)
{

		for (int i = 0; i < 2; i++)
		{
			LoginInfo ClientInfo = players.mMatchedPlayers[i]->GetLogInInfo();

			if (recvbuf.gameState == GameState::COIN_TOSS)
				recvbuf.textAux = players.mMatchedPlayers[i]->GetPosition() == PlayerPosition::PLAYER_ONE ? "One" : "Two";

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

	switch (recvbuf->gameState)
	{
	case GameState::CONNECTED:
		memset(&recvbuflen, 0, sizeof(GenericPacket));
		recvbuf->gameState = GameState::LOBBY;
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
			recvbuf->gameState = GameState::FINISH;
		}
		else
		{
			recvbuf->textAux = "X";
			recvbuf->gameState = GameState::PLAYER_TWO;
		}

		Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);
		break;

	case GameState::PLAYER_TWO:

		match = mGame->GetMatch(recvbuf->textAux);
		match->Input(recvbuf->aux, match->GetPlayer(recvbuf->textAux));

		if (match->CheckResult())
		{
			recvbuf->gameState = GameState::FINISH;
		}
		else
		{
			recvbuf->textAux = "X";
			recvbuf->gameState = GameState::PLAYER_ONE;
		}

		Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);

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

unsigned __stdcall PlayerPacketHanlder(void* data)
{
	GenericPacket recvbuf[sizeof(GenericPacket)];
	int recvbuflen = sizeof(GenericPacket);

	int playerbuflen = sizeof(PlayerPacket);

	GameBundle* mBundle = (GameBundle*)data;

	PlayerPacket* playerbuf = &mBundle->playerbuf;
	SOCKET mClientSocket = mBundle->clientSocket;
	sockaddr_in mClientAddr = mBundle->clientAddr;
	int clientSize = mBundle->clientSize;
	Game* mGame = mBundle->ticTacToe;
	Match* match;
	int mConnectionValue = 1;
	int mSendValue;

		cout << "Sucess! bytes received = " + mConnectionValue << endl;
		switch (playerbuf->cmdState)
		{
		case Command::REGISTER_PLAYER:
			if (mGame->mLobby.RegisterPlayer(playerbuf->name, mClientSocket, clientSize, mClientAddr))
			{
				mGame->CreateMatch(mGame->mLobby.GetMatchedPlayers());
				match = mGame->GetMatch(playerbuf->name);
				recvbuf->gameState = GameState::COIN_TOSS;
				recvbuf->aux = 0;
				Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);
			}
			break;

		case Command::REENTER_PLAYER:
			mGame->mLobby.ReenterPlayer(playerbuf->name);
			break;

		case Command::START_GAME:

			mGame->CreateMatch(mGame->mLobby.GetMatchedPlayers());
			match = mGame->GetMatch(playerbuf->name);
			recvbuf->gameState = GameState::PLAYER_ONE;
			recvbuf->aux = 0;
			Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);

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

	PlayerPacket playerbuf[sizeof(PlayerPacket)];
	int playerbuflen = sizeof(PlayerPacket);

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
			mBundle.recvbuf = *recvbuf;
			mBundle.playerbuf = *playerbuf;

			unsigned threadID;
			HANDLE Thread = (HANDLE)_beginthreadex(NULL, 0, &GenericPacketHanlder, (void*)&mBundle, 0, &threadID);
		}

		mConnectionValue = recv(mClientSocket, (char*)&playerbuf, playerbuflen, 0);
		if (mConnectionValue>0)
		{
			mBundle.clientSocket = mClientSocket;
			mBundle.clientAddr = mClientAddr;
			mBundle.clientSize = clientSize;
			mBundle.playerbuf = *playerbuf;
			mBundle.recvbuf = *recvbuf;

			unsigned threadID;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &PlayerPacketHanlder, &mBundle, 0, &threadID);
		}

	}

	closesocket(mClientSocket);

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}