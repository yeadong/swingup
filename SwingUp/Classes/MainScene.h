#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "Agent.h"
#include <Box2D/Box2D.h>

// set's the Pixel to Meter Ratio
// As we're initiliazing our display 1134px wide, a PTM ratio
// of 567 sets our screen width to be 2 meters
const static float PTM_RATIO = 1134.0f;

class KeyList
{
public:
    bool left = false;
    bool right = false;
};

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
    // setup keyboard handling in case we're on a pc
    void setupKeyboardHandling();
    void setupWorld();
    
    
private:
    void setPhysicsWorld(b2World* world);
    b2World* getSceneWorld();
    b2World* sceneWorld;
    void update(float dt) override;
    Agent* agent;
    b2Body* agentBody;
    KeyList pressedKeys;
};

#endif // __MainScene_SCENE_H__
