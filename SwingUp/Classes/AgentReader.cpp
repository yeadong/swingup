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

// Initilaize a null pointer of type AgentReader*
static AgentReader* _instanceAgentReader = nullptr;


/**
 * @desc Getter function returning an pointer of AgentReader
 * @params: None
 * @return: <AgentReader*> -
 */
AgentReader* AgentReader::getInstance()
{
    if(!_instanceAgentReader)
    {
        _instanceAgentReader = new AgentReader();
    }
    return _instanceAgentReader;
}

/**
 * @desc Purge function - this also is a cocos2d-x specific
 * function which has to be implemented to get cocos studio
 * to work with xocde / visual studio
 * @params: None
 * @return: <void> -
 */
void AgentReader::purge()
{
    CC_SAFE_DELETE(_instanceAgentReader);
}

/**
 * @desc This function creates a <cocos2d::Node*>
 * @params: <cocos2d::flatbuffers::Table*>
 * @return: <AgentReader*> -
 */
Node* AgentReader::createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions)
{
    Agent* node = Agent::create();
    setPropsWithFlatBuffers(node, nodeOptions);
    return node;
}
