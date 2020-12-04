#include "BaseNetworkLogic.cpp"
#include <ws2tcpip.h>

int main(int argc, char* argv[])
{
#pragma region Members
	BaseNetworkLogic mSL;

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
	puts("Connected");
	sendto(mClientSocket, message, strlen(message), 0, (sockaddr*)&mServerAddr, sizeof(mServerAddr));

	return 0;
}