#pragma once

enum class State
{
	PLAYER_ONE,
	PLAYER_TWO,
	LOBBY,
	GAME,
	FINISH
};

struct Packet
{
	State cmdState;
	short int aux;
};
