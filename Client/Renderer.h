#pragma once
#include "string"
#include <iostream>
/// <summary>
/// Not a renderer, but draws the grid, so, you get the idea
/// </summary>

using namespace std;

struct Slot
{
	int mPosition;
	string mValue;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Draw();
	void Input(int position, string value);
private:
	Slot mSlots[3][3];
};