//
//  RailReader.cpp
//  SwingUp
//
//  Created by Deniz Saner on 3/15/17.
//
//

#include "RailReader.h"
#include "Rail.h"

using namespace cocos2d;

static RailReader* _instanceRailReader = nullptr;

/**
 * @desc Getter function returning an pointer of RailReader
 * @params: None
 * @return: <RailReader*> -
 */
RailReader* RailReader::getInstance()
{
    if(!_instanceRailReader)
    {
        _instanceRailReader = new RailReader();
    }
    return _instanceRailReader;
}

/**
 * @desc Purge function - this also is a cocos2d-x specific
 * function which has to be implemented to get cocos studio
 * to work with xocde / visual studio
 * @params: None
 * @return: <void> -
 */
void RailReader::purge()
{
    CC_SAFE_DELETE(_instanceRailReader);
}

/**
 * @desc This function creates a <cocos2d::Node*>
 * @params: <cocos2d::flatbuffers::Table*>
 * @return: <AgentReader*> -
 */
Node* RailReader::createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions)
{
    Rail* node = Rail::create();
    setPropsWithFlatBuffers(node, nodeOptions);
    return node;
}
