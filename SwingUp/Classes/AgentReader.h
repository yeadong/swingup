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

class AgentReader : public cocostudio::NodeReader
{
public:
    static AgentReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};


#endif /* AgentReader_hpp */
