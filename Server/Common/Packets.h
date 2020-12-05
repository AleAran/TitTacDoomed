#pragma once
#include <string>

enum class GameState
{
	CONNECTED,
	COIN_TOSS,
	PLAYER_ONE,
	PLAYER_TWO,
	LOBBY,
	IDLE,
	FINISH
};

enum class Command
{
	REGISTER_PLAYER,
	START_GAME,
	REENTER_PLAYER
};

struct GenericPacket
{
	GameState gameState;
	short int aux;
	std::string textAux;
};

struct PlayerPacket
{
	Command cmdState;
	std::string name;
};
