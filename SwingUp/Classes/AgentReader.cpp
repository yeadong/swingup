//
//  AgentReader.cpp
//  SwingUp
//
//  Created by Deniz Saner on 3/13/17.
//
//

#include "AgentReader.h"
#include "Agent.h"

using namespace cocos2d;

static AgentReader* _instanceAgentReader = nullptr;

AgentReader* AgentReader::getInstance()
{
    if(!_instanceAgentReader)
    {
        _instanceAgentReader = new AgentReader();
    }
    return _instanceAgentReader;
}

void AgentReader::purge()
{
    CC_SAFE_DELETE(_instanceAgentReader);
}

Node* AgentReader::createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions)
{
    Agent* node = Agent::create();
    setPropsWithFlatBuffers(node, nodeOptions);
    return node;
}
