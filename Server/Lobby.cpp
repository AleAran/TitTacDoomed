#include "Lobby.h"

bool Lobby::RegisterPlayer(string name, SOCKET mSocket, int size, sockaddr_in addr)
{
	Player *newPlayer = new Player;
	newPlayer->SetLogInInfo(mSocket, addr, size);
	newPlayer->SetName(name);
	newPlayer->SetPosition((mPlayerWaiting) ? PlayerPosition::PLAYER_TWO : PlayerPosition::PLAYER_ONE);
	mPlayers.push_back(newPlayer);

	if (!mPlayerWaiting)
	{
		mPlayerWaiting = true;
		return false;
	}
		
	else
	{
		mPlayerWaiting = false;
		return true;
	}
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
}


Lobby::Lobby()
{

}

Lobby::~Lobby()
{
}