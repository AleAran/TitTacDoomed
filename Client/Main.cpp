#include "BaseNetworkLogic.cpp"
#include <ws2tcpip.h>
#include "Lobby.h"

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;
	Match mTicTacToe;
	Lobby mLobby;

	struct sockaddr_in mServerAddr;
	SOCKET mClientSocket;
	const char* message = "Loud and Clear!\n";
#pragma endregion

	mServerAddr.sin_port = htons(80);
	mServerAddr.sin_family = AF_INET;

	inet_pton(AF_INET, "127.0.0.1", &mServerAddr.sin_addr);

	//Create socket
	mClientSocket = mSL.createSocket();

	//Connect to remote server
	if (connect(mClientSocket, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr))<0)
	{
		puts("connect error");

	}

	sendto(mClientSocket, message, strlen(message), 0, (sockaddr*)&mServerAddr, sizeof(mServerAddr));

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