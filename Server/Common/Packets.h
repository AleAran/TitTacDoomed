#pragma once
#include <string>

enum class GameState
{
	CONNECTED,
	COIN_TOSS,
	PLAYER_ONE,
	PLAYER_TWO,
	LOBBY, 
	FINISH,
	REGISTER_PLAYER,
	START_GAME,
	REENTER_PLAYER,
	
};

enum class Command
{

};

struct GenericPacket
{
	uint32_t gameState;
	short int aux;
	char textAux[255];
};