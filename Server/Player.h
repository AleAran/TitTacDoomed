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

	void SetName(string name) { mPlayerName = name; };
	void SetPosition(PlayerPosition pos) { mPlayerPos = pos; };
	void SetInMatch(bool inAMatch) { mInMatch = inAMatch; }

	string GetName() { return mPlayerName; };
	PlayerPosition GetPosition() { return mPlayerPos; };
	bool GetInMatch() { return mInMatch; }

private:
	PlayerPosition mPlayerPos;
	string mPlayerName;
	bool mInMatch;
};

