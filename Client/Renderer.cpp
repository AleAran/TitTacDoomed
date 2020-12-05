#include "Renderer.h"

void Renderer::Draw()
{
	cout.flush();
	cout.clear();
	std::cout << endl
		<< " " + mSlots[0][0].mValue + " | " + mSlots[0][1].mValue + " | " + mSlots[0][2].mValue + " " << endl
		<< "-----------" << endl
		<< " " + mSlots[1][0].mValue + " | " + mSlots[1][1].mValue + " | " + mSlots[1][2].mValue + " " << endl
		<< "-----------" << endl
		<< " " + mSlots[2][0].mValue + " | " + mSlots[2][1].mValue + " | " + mSlots[2][2].mValue + " " << endl
		<< endl;
}

void Renderer::Input(int position, string value)
{
	for (size_t i = 0; i < sizeof(mSlots) / sizeof(mSlots[0]); i++)
	{
		for (size_t j = 0; j < sizeof(mSlots[0]) / sizeof(Slot); j++)
		{
			if (mSlots[i][j].mPosition == position)
				mSlots[i][j].mValue = value;
		}
	}
}

Renderer::Renderer()
{
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

Renderer::~Renderer()
{
}