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

// This class represents the agent of our little game.
// Right now, it merely has a getter and setter class
// for the corresponding physic's body, which will change
// in the future. NOTE: I am planning on moving all the
// logic and code which deals with initializing and positioning
// of the agent into a function of the Agent class.
class Agent : public cocos2d::Node
{
public:
    bool init() override;
    
    // setter and getter function
    // for the agent's box2d physics body
    void setBody(b2Body* physicsBody);
    b2Body* getBody();
    
    
    CREATE_FUNC(Agent);

private:
    b2Body* body;
};

#endif /* Agent_h */
