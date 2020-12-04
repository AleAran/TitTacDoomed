#include "Match.h"

Match::Match()
{
}

Match::~Match()
{
}


void Match::StartMatch(MatchedPlayers players)
{
	mPlayers = players;
	int pos = 0;
	for (size_t i = 0; i < sizeof(mSlots) / sizeof(mSlots[0]); i++)
	{
		for (size_t j = 0; j < sizeof(mSlots[0]) / sizeof(Slot); j++)
		{
			mSlots[i][j].mPosition = pos;
			mSlots[i][j].mValue = " ";
			pos++;
		}
	}
}

void Match::Input(int position, Player *player)
{
	for (size_t i = 0; i < sizeof(mSlots) / sizeof(mSlots[0]); i++)
	{
		for (size_t j = 0; j < sizeof(mSlots[0]) / sizeof(Slot); j++)
		{
			mSlots[i][j].mValue = (mSlots[i][j].mPosition == position)? (player->GetPosition() == PlayerPosition::PLAYER_ONE)? "X" : "O" : mSlots[i][j].mValue;
		}
	}
}

void Match::Draw()
{
	std::cout << endl
		<< " " + mSlots[0][0].mValue + " | " + mSlots[0][1].mValue + " | " + mSlots[0][2].mValue + " " << endl
		<< "-----------" << endl
		<< " " + mSlots[1][0].mValue + " | " + mSlots[1][1].mValue + " | " + mSlots[1][2].mValue + " " << endl
		<< "-----------" << endl
		<< " " + mSlots[2][0].mValue + " | " + mSlots[2][1].mValue + " | " + mSlots[2][2].mValue + " " << endl
		<< endl;
}

 bool Match::CheckResult()
{

	for (size_t i = 0; i < sizeof(mSlots) / sizeof(mSlots[0]); i++)
	{
		if (mSlots[i][0].mValue == mSlots[i][1].mValue && mSlots[i][0].mValue == mSlots[i][2].mValue && mSlots[i][0].mValue != " ")
		{
			ResultMessage(mSlots[i][0].mValue);
			return true;
		}
	}

	for (size_t i = 0; i < sizeof(mSlots[0]) / sizeof(Slot); i++)
	{
		if (mSlots[0][i].mValue == mSlots[1][i].mValue && mSlots[0][i].mValue == mSlots[2][i].mValue && mSlots[0][i].mValue != " ")
		{
			ResultMessage(mSlots[0][i].mValue);
			return true;
		}
	}

	if (mSlots[0][0].mValue == mSlots[1][1].mValue && mSlots[0][0].mValue == mSlots[2][2].mValue && mSlots[0][0].mValue != " ")
	{
		ResultMessage(mSlots[0][0].mValue);
		return true;
	}

	if (mSlots[0][2].mValue == mSlots[1][1].mValue && mSlots[0][2].mValue == mSlots[2][0].mValue && mSlots[0][2].mValue !=  " ")
	{
		ResultMessage(mSlots[0][2].mValue);
		return true;
	}

	return false;
}


 Player* Match::GetPlayer(string name)
 {
	 return (name == mPlayers.mMatchedPlayers[0]->GetName())? mPlayers.mMatchedPlayers[0] : mPlayers.mMatchedPlayers[1];
 }

 void Match::ResultMessage(string Value)
 {
	 string playerName = (Value == "X") ? mPlayers.mMatchedPlayers[0]->GetName() : mPlayers.mMatchedPlayers[1]->GetName();

	 cout << playerName + " Wins!!!!" << endl;
 }
