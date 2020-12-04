#pragma once
#include <string>

using namespace std;

enum class PlayerPosition
{
	PLAYER_ONE,
	PLAYER_TWO,
	UNDEFINED
};

class Player
{
public:
	Player();
	~Player();
	string GetName() { return mPlayerName; };
	PlayerPosition GetPosition() { return mPlayerPos; };
	void SetName(string name) { mPlayerName = name; };
	void SetPosition(PlayerPosition pos) { mPlayerPos = pos; };

private:
	PlayerPosition mPlayerPos;
	string mPlayerName;
};

