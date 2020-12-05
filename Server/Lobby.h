#pragma once
#include <string>
#include <list>
#include "Match.h"
using namespace std;

class Lobby
{
public:
	Lobby();
	~Lobby();

	bool RegisterPlayer(string name, SOCKET mSocket, int size, sockaddr_in addr);
	MatchedPlayers GetMatchedPlayers();


private:
	list<Player*> mPlayers;
	bool mPlayerWaiting;
};