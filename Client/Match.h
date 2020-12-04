#pragma once
#include <iostream>
#include "Player.h"

using namespace std;

struct Slot
{
	int mPosition;
	string mValue;
};

struct MatchedPlayers
{
	Player *mMatchedPlayers [2];
};

class Match
{
public:
	Match();
	~Match();

	void StartMatch(MatchedPlayers players);
	void Input(int position, Player *player);
	void Draw();
	bool CheckResult();

	Player* GetPlayer(string name);

private:
	Slot mSlots[3][3];
	MatchedPlayers mPlayers;

	void ResultMessage(string Value);
};