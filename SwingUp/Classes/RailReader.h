//
//  RailReader.h
//  SwingUp
//
//  Created by Deniz Saner on 3/15/17.
//
//

#ifndef __SwingUp_Cpp__RailReader__
#define __SwingUp_Cpp__RailReader__

#include "cocos2d.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"

// Note:
// ==========
// This class is required by cocos studio to create a bridge
// between code and sprites.

// This class will be used to initialize the agent rail
// sprites from cocos studios in later stages of the project.
// For now, it's just a scaffold with no use in our main scene.
class RailReader : public cocostudio::NodeReader
{
public:
    static RailReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};


#endif /* RailReader_h */
