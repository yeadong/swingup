//
//  Piece.cpp
//  SushiNeko-Cpp
//
//  Created by Daniel Haaser on 4/15/15.
//
//

#include "Piece.h"

using namespace cocos2d;

bool Piece::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    // a piece defaults to having no obstacle
    this->obstacleSide = Side::None;
    
    return true;
}

float Piece::getSpriteHeight() const
{
    // this helper method returns the height of the actual displayed sprite
    
    // first grab a reference to the roll sprite
    Sprite* roll = this->getChildByName<Sprite*>("roll");
    
    // then return the roll sprite's height
    return roll->getContentSize().height;
}

void Piece::setObstacleSide(Side side)
{
    this->obstacleSide = side;
    
    Sprite* roll = this->getChildByName<Sprite*>("roll");
    
    // get references to the chopstick sprites on the left and right sides
    Sprite* leftChopstick = roll->getChildByName<Sprite*>("chopstickLeft");
    Sprite* rightChopstick = roll->getChildByName<Sprite*>("chopstickRight");
    
    // turn on the appropriate chopstick visiblity depending on the side the obstacle should appear on
    switch (this->obstacleSide)
    {
        case Side::None:
            leftChopstick->setVisible(false);
            rightChopstick->setVisible(false);
            break;
            
        case Side::Left:
            leftChopstick->setVisible(true);
            rightChopstick->setVisible(false);
            break;
            
        case Side::Right:
            rightChopstick->setVisible(true);
            leftChopstick->setVisible(false);
            break;
    }
}

Side Piece::getObstacleSide() const
{
    return this->obstacleSide;
}
