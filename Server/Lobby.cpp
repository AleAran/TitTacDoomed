#include "Lobby.h"

bool Lobby::RegisterPlayer(string name, SOCKET mSocket, int size, sockaddr_in addr)
{
	Player *newPlayer = new Player;
	newPlayer->SetLogInInfo(mSocket, addr, size);
	newPlayer->SetName(name);
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

bool Lobby::ReenterPlayer(string name)
{
	if (mPlayers.size() > 1)
	{
		std::list<Player*>::iterator it;
		for (it = mPlayers.begin(); it != mPlayers.end(); ++it) {
			if ((*it)->GetName() == name)
			{
				(*it)->SetInMatch(false);
				mPlayerWaiting = true;
			}
		}
	}
	return false;
}

MatchedPlayers Lobby::GetMatchedPlayers()
{
	MatchedPlayers playersReady;
	int index = 0;
	int compGuess = rand() % 2 + 3;
	PlayerPosition randomPos = compGuess < 3 ? PlayerPosition::PLAYER_ONE : PlayerPosition::PLAYER_TWO;

	if (mPlayers.size() > 1)
	{
		std::list<Player*>::iterator it;
		for (it = mPlayers.begin(); it != mPlayers.end(); ++it) {
			if (!(*it)->GetInMatch())
			{
				(*it)->SetInMatch(true);
				playersReady.mMatchedPlayers[index] = *it;
				if (index < 1)
				{
					(*it)->SetPosition(randomPos);
				}
				else
				{
					(*it)->SetPosition(randomPos == PlayerPosition::PLAYER_ONE ? PlayerPosition::PLAYER_TWO : PlayerPosition::PLAYER_ONE);
				}
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