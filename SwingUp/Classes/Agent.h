//
//  Agent.hpp
//  SwingUp
//
//  Created by Deniz Saner on 3/13/17.
//
//

#ifndef Agent_h
#define Agent_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Agent : public cocos2d::Node
{
public:
    bool init() override;
    
    // setter and getter function
    // for the agent's box2d physics body
    void setBody(b2Body* physicsBody);
    b2Body* getBody();
    
    
    CREATE_FUNC(Agent);

protected:
    b2Body* body;
};

#endif /* Agent_h */
