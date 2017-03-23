//
//  AgentReader.hpp
//  SwingUp
//
//  Created by Deniz Saner on 3/13/17.
//
//

#ifndef __SwingUp_Cpp__AgentReader__
#define __SwingUp_Cpp__AgentReader__

#include "cocos2d.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"


// Note:
// ==========
// This class is required by cocos studio to create a bridge
// between code and sprites.

// This class will be used to initialize the agent
// sprites from cocos studios in later stages of the project.
// For now, it's just a scaffold with no use in our main scene.
class AgentReader : public cocostudio::NodeReader
{
public:
    static AgentReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};


#endif /* AgentReader_hpp */
