#pragma once
#include <queue>

class SortVisualizerTool
{
public:
	struct Action
	{
		int type, a, b;
		int v = 0;
	};

	int size;

	int& operator[](const int index);

	float getX(int index);
	float getY(int index);
	float getDrawSize();
	float getTheta(int index);
	float getRadius(int index);
	Action getAction();
	bool nextAction();
	int actCompare(int i, int j);
	void actSwap(int i, int j);
	void actSet(int i, int v);
	int actGet(int i);
	void popAction();
	void reset(int size);

	SortVisualizerTool(int size);
	~SortVisualizerTool();
private:
	int* arr, * bgArr;
	std::queue<Action>actions;
};
