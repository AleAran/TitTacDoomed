#pragma once
#include "Lobby.h"

class Game
{
public:
	Lobby mLobby;
	list<Match*> mMatchList;

	void CreateMatch(MatchedPlayers players);

	Match* GetMatch(string name);

	Game();
	~Game();

private:

};
