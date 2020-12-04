#pragma once
#include <iostream>
#include "Player.h"

using namespace std;

struct Slot
{
	int mPosition;
	string mValue;
};

class Game
{
public:
	Game();
	~Game();
	void Draw();
	void Input(int position, Player player);
	bool CheckResult();
	void SetPlayer(string name);
	Player GetPlayer(string name);

private:
	Slot mSlots[3][3];
	Player mPlayer;
	Player mPlayer2;

	void ResultMessage(string Value);
};