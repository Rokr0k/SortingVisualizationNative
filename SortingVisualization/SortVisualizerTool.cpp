#include "SortVisualizerTool.h"

int& SortVisualizerTool::operator[](const int index)
{
	return this->arr[index];
}

float SortVisualizerTool::getX(int index)
{
	return this->getDrawSize() * index - 1.0f;
}

float SortVisualizerTool::getY(int index)
{
	return 2.0f / this->size * (this->operator[](index) + 1) - 1.0f;
}

float SortVisualizerTool::getDrawSize()
{
	return 2.0f / this->size;
}

float SortVisualizerTool::getTheta(int index)
{
	const float pi = 3.1415926f;
	return pi * 2 / this->size * index;
}

float SortVisualizerTool::getRadius(int index)
{
	return 1.0f / this->size * (this->operator[](index) + 1);
}

SortVisualizerTool::Action SortVisualizerTool::getAction()
{
	if (this->nextAction())
	{
		this->m.lock();
		auto r = this->actions.front();
		this->m.unlock();
		return r;
	}
	return { SortVisualizerTool::ActionType::NONE, -1, -1 };
}

bool SortVisualizerTool::nextAction()
{
	this->m.lock();
	auto r = this->actions.empty();
	this->m.unlock();
	return !r;
}

int SortVisualizerTool::actCompare(int i, int j)
{
	this->m.lock();
	this->actions.push({ SortVisualizerTool::ActionType::COMPARE, i, j });
	this->m.unlock();
	return this->bgArr[i] < this->bgArr[j];
}

void SortVisualizerTool::actSwap(int i, int j)
{
	this->m.lock();
	this->actions.push({ SortVisualizerTool::ActionType::SWAP, i, j });
	this->m.unlock();
	int tmp = this->bgArr[i];
	this->bgArr[i] = this->bgArr[j];
	this->bgArr[j] = tmp;
}

void SortVisualizerTool::actSet(int i, int v)
{
	this->m.lock();
	this->actions.push({ SortVisualizerTool::ActionType::SET, i, -1, v });
	this->m.unlock();
	this->bgArr[i]  = v;
}

int SortVisualizerTool::actGet(int i)
{
	return this->bgArr[i];
}

void SortVisualizerTool::pushAction(Action action)
{
	this->m.lock();
	this->actions.push(action);
	this->m.unlock();
}

void SortVisualizerTool::popAction()
{
	if (this->nextAction())
	{
		SortVisualizerTool::Action action = this->getAction();
		if (action.type != SortVisualizerTool::ActionType::NONE)
		{
			this->m.lock();
			this->actions.pop();
			this->m.unlock();
			switch (action.type)
			{
			case SortVisualizerTool::ActionType::SWAP:
			{
				int tmp = this->operator[](action.a);
				this->operator[](action.a) = this->operator[](action.b);
				this->operator[](action.b) = tmp;
			}
				break;
			case SortVisualizerTool::ActionType::SET:
				this->operator[](action.a) = action.v;
				break;
			}
		}
	}
}

void SortVisualizerTool::reset(int size)
{
	this->size = size;
	delete[]this->arr;
	this->arr = new int[size];
	delete[]this->bgArr;
	this->bgArr = new int[size];
	for (int i = 0; i < size; i++)
		this->arr[i] = this->bgArr[i] = i;
	while (!this->actions.empty())
		this->actions.pop();
}

SortVisualizerTool::SortVisualizerTool(int size)
{
	this->size = size;
	this->arr = new int[size];
	this->bgArr = new int[size];
	for (int i = 0; i < size; i++)
		this->arr[i] = this->bgArr[i] = i;
}

SortVisualizerTool::~SortVisualizerTool()
{
	delete[]this->arr;
	delete[]this->bgArr;
}