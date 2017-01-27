#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <memory>
#include "Pathfind.h"

class Robot : Pathfind
{
public:
	Robot();
	~Robot();

private:
	shared_ptr<bool> startPathfinding = make_shared<bool>(false);
	shared_ptr<bool> searchComplete = make_shared<bool>(false);
};


#endif
