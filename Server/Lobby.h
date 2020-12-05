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
	string StatusMessage();
	void RegisterPlayer(string name);
	MatchedPlayers GetMatchedPlayers();

private:
	list<Player*> mPlayers;
	bool mPlayerWaiting;
};