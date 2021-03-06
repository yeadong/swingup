//
//  Piece.h
//  SushiNeko-Cpp
//
//  Created by Daniel Haaser on 4/15/15.
//
//

#ifndef __SushiNeko_Cpp__Piece__
#define __SushiNeko_Cpp__Piece__

#include "cocos2d.h"
#include "Constants.h"

class Piece : public cocos2d::Node
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(Piece);
    
    void setObstacleSide(Side side);
    Side getObstacleSide() const;
    
    float getSpriteHeight() const;
    
protected:
    Side obstacleSide;

};

#endif /* defined(__SushiNeko_Cpp__Piece__) */
