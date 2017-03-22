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

RailReader* RailReader::getInstance()
{
    if(!_instanceRailReader)
    {
        _instanceRailReader = new RailReader();
    }
    return _instanceRailReader;
}

void RailReader::purge()
{
    CC_SAFE_DELETE(_instanceRailReader);
}

Node* RailReader::createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions)
{
    Rail* node = Rail::create();
    setPropsWithFlatBuffers(node, nodeOptions);
    return node;
}
