#include <iostream>
#include "..\Server\Common\BaseNetworkLogic.cpp"
#include "..\Server\Common\Packets.h"
#include "Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;

	sockaddr_in mServerAddr;
	SOCKET mClientSocket = INVALID_SOCKET;
	GenericPacket recvbuf[sizeof(GenericPacket)];
	int recvbuflen = sizeof(GenericPacket);

	bool mEndSession = false;
#pragma endregion

#pragma region Connect To server
	mServerAddr.sin_port = htons(8080);
	mServerAddr.sin_family = AF_INET;

	int mConnectionValue = 1;
	int mSendValue;

	inet_pton(AF_INET, "127.0.0.1", &mServerAddr.sin_addr);

	mClientSocket = mSL.createSocket();

	if (connect(mClientSocket, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) {
		closesocket(mClientSocket);
		mClientSocket = INVALID_SOCKET;
		return 1;
	}
#pragma endregion

	Renderer mGrid;
	GameState mPlayerOrder;
	string mName;
	std::string end;
	recvbuf->aux = 0;
	recvbuf->gameState = 0;

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
		GameState state = (GameState)recvbuf->gameState;
		switch (state)
		{
		case GameState::LOBBY:
			cout << ":::LOBBY:::" << endl	
				 << "Enter Name" << endl;

			cin >> recvbuf->textAux;
			
			cout.clear();
			cout << ":::LOBBY:::" << endl
				<< "Waiting For Players..." << endl;

			recvbuf->gameState = 6; //GameState::REGISTER_PLAYER;
			send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);

			break;

		case GameState::COIN_TOSS:
			mGrid.Draw();
			if (recvbuf->textAux == "One")
			{
				mPlayerOrder = GameState::PLAYER_ONE;
				cout << "Your are (X), enter values from 0 to 8 to mark a Slot." << endl;
			}
			else
			{
				mPlayerOrder = GameState::PLAYER_TWO;
				cout << "Your are (O), enter values from 0 to 8 to mark a Slot." << endl;
			}

			memset(&recvbuf, 0, sizeof(recvbuf));
			memcpy(recvbuf->textAux, mName.c_str(), sizeof(mName));

			recvbuf->gameState = 1; // Command::START_GAME;
			send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
			break;
		case GameState::PLAYER_ONE:
			if (recvbuf->aux != -1)
				mGrid.Input(recvbuf->aux, recvbuf->textAux);

			mGrid.Draw();


			if (mPlayerOrder == GameState::PLAYER_ONE)
			{
				mGrid.Input(recvbuf->aux, recvbuf->textAux);
				
				cin >> recvbuf->aux;

				memset(&recvbuf, 0, sizeof(recvbuf));
				memcpy(recvbuf->textAux, mName.c_str(), sizeof(mName));

				// Send an initial 
				sendto(mClientSocket, (char*)&recvbuf, recvbuflen, 0, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr));
			}
			
			else 
			{
				cout << "Wait for (O) to finish" << endl;
			}
			break;

		case GameState::PLAYER_TWO:
			if (recvbuf ->aux != -1)
				mGrid.Input(recvbuf->aux, recvbuf->textAux);

			mGrid.Draw();

			if (mPlayerOrder == GameState::PLAYER_TWO)
			{
				cout << "Your Turn (o), enter values from 0 to 8 to mark a Slot." << endl;

				cin >> recvbuf->aux;
				memset(&recvbuf, 0, sizeof(recvbuf));
				memcpy(recvbuf->textAux, mName.c_str(), sizeof(mName));

				sendto(mClientSocket, (char*)&recvbuf, recvbuflen, 0, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr));
			}
			
			else 
			{
				cout << "Wait for (X) to finish" << endl;
			}
			break;

		case GameState::FINISH:

			mGrid.Draw();
			//cout << recvbuf->textAux + " IS THE WINNER!" << endl << "En Session? (Y/N)" << endl;
			cin >> end;
			if (end == "y")
			{
				mEndSession = true;
			}
			else
				recvbuf->gameState = 8; //GameState::REENTER_PLAYER;
				send(mClientSocket, (char*)&recvbuf, recvbuflen, 0);
			break;
		default:
			break;
		}

	} while (mEndSession != true );

	return 0;
}