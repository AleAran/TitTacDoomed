#include "BaseNetworkLogic.h"

int main(int argc, char* argv[])
{
	BaseNetworkLogic mSL;
	struct addrinfo server;


	server.ai_family = AF_INET;
	server.ai_socktype = SOCK_STREAM;
	server.ai_protocol = IPPROTO_TCP;
	server.ai_flags = AI_PASSIVE;

	mSL.BindSocket(mSL.mFirstSocket, server);

	closesocket(mSL.mFirstSocket);
}

