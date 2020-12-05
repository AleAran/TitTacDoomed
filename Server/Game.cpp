#include "Game.h"

void Game::CreateMatch(MatchedPlayers players)
{
	Match* newMatch = new Match();
	newMatch->StartMatch(players);

	mMatchList.push_back(newMatch);
}

Match*Game::GetMatch(string name)
{
	std::list<Match*>::iterator it;
	for (it = mMatchList.begin(); it != mMatchList.end(); ++it) {
		if ((*it)->GetPlayer(name))
		{
			return (*it);
		}
	}
}

Game::Game()

{
}

Game::~Game()
{
}