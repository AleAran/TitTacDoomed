#pragma once
#include <string>
#include "Common/BaseNetworkLogic.h"
using namespace std;

enum class PlayerPosition
{
	PLAYER_ONE,
	PLAYER_TWO,
	UNDEFINED
};

struct LoginInfo
{
	SOCKET clientSocket;
	sockaddr_in mClientAddr;
	int clientSize;
};

class Player
{
public:
	Player();
	~Player();

	void SetName(string name) { mPlayerName = name; };
	void SetPosition(PlayerPosition pos) { mPlayerPos = pos; };
	void SetInMatch(bool inAMatch) { mInMatch = inAMatch; }
	void SetLogInInfo(SOCKET dataId, sockaddr_in clientAddr, int size);

	string GetName() { return mPlayerName; };
	PlayerPosition GetPosition() { return mPlayerPos; };
	bool GetInMatch() { return mInMatch; }
	LoginInfo GetLogInInfo() { return mDataId; }

private:
	PlayerPosition mPlayerPos;
	string mPlayerName;
	bool mInMatch;
	LoginInfo mDataId;
};

