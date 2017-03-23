//
//  Character.cpp
//  SushiNeko-Cpp
//
//  Created by Daniel Haaser on 4/15/15.
//
//

#include "Character.h"

using namespace cocos2d;


void Character::setSide(Side side)
{
    this->side = side;
    if (this->side == Side::Right)
    {
        this->setScaleX(-1.0f);
    }
    else
    {
        this->setScaleX(1.0f);
    }
};

Side Character::getSide()
{
    return this->side;
};

bool Character::init()
{
    if (! Node::init())
    {
        return false;
    }
    this->side = Side::Left;
    
    this->timeline = CSLoader::createTimeline("character.csb");
    
    this->timeline->retain();
    return true;
}

void Character::onExit()
{
    // release the retain we called in init
    this->timeline->release();
    
    Node::onExit();
}

void Character::runChopAnimation()
{
    this->stopAllActions();    
    
    this->runAction(this->timeline);
    
    this->timeline->play("chop", false);
}
