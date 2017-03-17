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

class RailReader : public cocostudio::NodeReader
{
public:
    static RailReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};


#endif /* RailReader_h */
