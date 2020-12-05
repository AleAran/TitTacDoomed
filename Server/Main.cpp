#include "Common\BaseNetworkLogic.h"
#include "Common/Packets.h"
#include "Game.h"
#include <iostream>
#include <thread>

using namespace std;

struct GameBundle
{
	Game* ticTacToe;
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int clientSize;
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

unsigned __stdcall ClientHandler(void* data)
{
	GenericPacket recvbuf[sizeof(GenericPacket)];
	int recvbuflen = sizeof(GenericPacket);

	PlayerPacket playerbuf[sizeof(PlayerPacket)];
	int playerbuflen = sizeof(PlayerPacket);

	GameBundle *mBundle = (GameBundle*)data;

	SOCKET mClientSocket = mBundle->clientSocket;
	sockaddr_in mClientAddr = mBundle->clientAddr;
	int clientSize = mBundle->clientSize;
	Game* mGame = mBundle->ticTacToe;
	Match* match;
	int mConnectionValue = 1;
	int mSendValue;

	mConnectionValue = recv(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
	if (mConnectionValue > 0) {
		cout << "Sucess! bytes received = " + mConnectionValue << endl;

		switch (recvbuf->gameState)
		{
		case GameState::CONNECTED:
			recvbuf->gameState = GameState::LOBBY;
			recvbuf->aux = 0;
			mSendValue = send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);

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
	}

	mConnectionValue = recv(mClientSocket, (char*)&playerbuf, playerbuflen, 0);
	if (mConnectionValue > 0) {
		cout << "Sucess! bytes received = " + mConnectionValue << endl;
		switch (playerbuf->cmdState)
		{
		case Command::REGISTER_PLAYER:
			if (mGame->mLobby.RegisterPlayer(playerbuf->name, mClientSocket, clientSize, mClientAddr))
			{
				mGame->CreateMatch(mGame->mLobby.GetMatchedPlayers());
				Match* match = mGame->GetMatch(playerbuf->name);
				recvbuf->gameState = GameState::COIN_TOSS;
				recvbuf->aux = 0;
				Broadcast(match->GetPlayers(), *recvbuf, recvbuflen);
			}
			break;

		case Command::REENTER_PLAYER:
			mGame->mLobby.ReenterPlayer(playerbuf->name);

			break;

		case Command::START_GAME:
			recvbuf->gameState = GameState::PLAYER_ONE;
			recvbuf->aux = -1;
			send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);

			break;
		default:
			break;
		}
	}
	else 
	{
		printf("Error! " + WSAGetLastError());
		return 1;
	}
	//If we don't end the thread, things could get messy;
	_endthreadex(0);
}

int main(int argc, char* argv[])
{
#pragma region Members
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

	// Accept a client socket
	while (mClientSocket = accept(mServerSocket, (struct sockaddr*)&mClientAddr, &clientSize))
	{
		mBundle.clientSocket = mClientSocket;
		mBundle.clientAddr = mClientAddr;
		mBundle.clientSize = clientSize;

		unsigned threadID;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ClientHandler, &mBundle, 0, &threadID);
	}

	closesocket(mClientSocket);

	//Once the binding is done, we close the socket to release it's descriptor.
	closesocket(mServerSocket);

	WSACleanup();

	return 0;
}
