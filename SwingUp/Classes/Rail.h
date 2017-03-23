//
//  Rail.hpp
//  SwingUp
//
//  Created by Deniz Saner on 3/15/17.
//
//

#ifndef Rail_h
#define Rail_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

// This class will represent the rail
// on which the agent is driving on.
// Right now, it's not used anywhere in the project.
class Rail : public cocos2d::Node
{
public:
    bool init() override;
    
    CREATE_FUNC(Rail);
};


#endif /* Rail_h */
