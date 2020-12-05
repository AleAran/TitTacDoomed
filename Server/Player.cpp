#include "Player.h"

Player::Player()
{
	mPlayerPos = PlayerPosition::UNDEFINED;
	mPlayerName = "Default";
	mInMatch = false;
}

Player::~Player()
{
}

void Player::SetLogInInfo(SOCKET dataId, sockaddr_in clientAddr, int size)
{
	mDataId.clientSize = size;
	mDataId.clientSocket = dataId;
	mDataId.mClientAddr = clientAddr;
}
