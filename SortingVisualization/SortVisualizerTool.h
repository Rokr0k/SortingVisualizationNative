#pragma once
#include <queue>
#include <mutex>

class SortVisualizerTool
{
public:
	enum class ActionType
	{
		NONE, COMPARE, SWAP, SET
	};

	struct Action
	{
		ActionType type;
		int a, b;
		int v = 0;
	};

	int size;
	std::mutex m;

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
	void pushAction(Action action);
	void popAction();
	void reset(int size);

	SortVisualizerTool(int size);
	~SortVisualizerTool();
private:
	int* arr, * bgArr;
	std::queue<Action>actions;
};
