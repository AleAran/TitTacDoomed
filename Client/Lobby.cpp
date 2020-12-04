#include "Lobby.h"

string Lobby::StatusMessage() 
{ 
	return " ";
}

void Lobby::RegisterPlayer(string name)
{
	Player *newPlayer = new Player;
	newPlayer->SetName(name);
	newPlayer->SetPosition((mPlayerWaiting) ? PlayerPosition::PLAYER_TWO : PlayerPosition::PLAYER_ONE);
	mPlayers.push_back(newPlayer);

	if (!mPlayerWaiting) 
		mPlayerWaiting = true;
}

MatchedPlayers Lobby::GetMatchedPlayers()
{
	MatchedPlayers playersReady;
	int index = 0;

	if (mPlayers.size() > 1)
	{
		std::list<Player*>::iterator it;
		for (it = mPlayers.begin(); it != mPlayers.end(); ++it) {
			if (!(*it)->GetInMatch())
			{
				(*it)->SetInMatch(true);
				playersReady.mMatchedPlayers[index] = *it;
				index++;
			}
			if (index == 2)
			{
				return playersReady;
			}
		}
	}

	return MatchedPlayers();
}




Lobby::Lobby()
{

}

Lobby::~Lobby()
{
}