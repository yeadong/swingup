//
//  Agent.hpp
//  SwingUp
//
//  Created by Deniz Saner on 3/13/17.
//
//

#ifndef Agent_hpp
#define Agent_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Agent : public cocos2d::Node
{
public:
    bool init() override;
    
    CREATE_FUNC(Agent);
};

#endif /* Agent_hpp */
