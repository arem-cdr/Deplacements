#pragma once

#include"Point.h"

enum action_type { IDLE = 0, MOVE_TO_POINT, GO_FORWARD };

class PossibleAction
{
private:
	action_type action;

	float* time;
	Point* point;
	float* distEnCm;

public:
	PossibleAction(action_type action);
	~PossibleAction();

	
};

class Strategy
{
private:
	std::vector <PossibleAction> actions;

public:


};

