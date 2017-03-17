//
//  Agent.cpp
//  SwingUp
//
//  Created by Deniz Saner on 3/13/17.
//
//

#include "Agent.h"
#include <Box2D/Box2D.h>
#include <iostream>
using namespace cocos2d;

// init function by cocos2d studio
bool Agent::init()
{
    if (! Node::init())
    {
        return false;
    }
    return true;
}
