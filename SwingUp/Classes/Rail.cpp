//
//  Rail.cpp
//  SwingUp
//
//  Created by Deniz Saner on 3/15/17.
//
//

#include "Rail.h"

using namespace cocos2d;


/**
 * @desc Checker function if the Node was already initialized
 * @params: None
 * @return: <bool>
 */
bool Rail::init()
{
    if (! Node::init())
    {
        return false;
    }
    return true;
}
