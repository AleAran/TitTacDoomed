#pragma 
#include <string>
#include <iostream>

using namespace std;

enum class Player
{
	PLAYER_ONE,
	PLAYER_TWO
};

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
	bool CheckResult(Player player);

private:
	Slot mSlots[3][3];

};