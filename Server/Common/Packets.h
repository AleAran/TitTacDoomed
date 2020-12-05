#pragma once
#include <string>

enum class GameState
{
	CONNECTED,
	PLAYER_ONE,
	PLAYER_TWO,
	LOBBY,
	GAME,
	IDLE,
	FINISH
};

enum class Command
{
	REGISTER_PLAYER,
	DRAW
};

struct GenericPacket
{
	GameState gameState;
	short int aux;
};

struct PlayerPacket
{
	Command cmdState;
	std::string name;
};
