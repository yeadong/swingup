#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "Agent.h"
#include "GLES-Render.h"
#include <Box2D/Box2D.h>


// Globals definition
// ==================
// PTM_RATIO: Pixel to meter ratio
// As we're initiliazing our display 1134px wide, a PTM ratio
// of 567 sets our screen width to equal 2 meters

// VELOCITY
// Horizontal Velocity of the agent
const static float PTM_RATIO = 1134.0f / 2.0f;
const static float VELOCTY = 1.2f;


// Agent attribtues
// ==================
// AGENT_MASS: Mass of the agent [kg]
// AGENT_FRICTION: Friction of the agent [-]
// AGENT_RESTITUTION: Agent restitution [-]
// AGENT_WIDTH: Agent width [m]
// AGENT_HEIGHT: Agent height [m]
const static float AGENT_MASS = 1.0f;
const static float AGENT_FRICTION = 0.001f;
const static float AGENT_RESTITUTION = 0.0f;
const static float AGENT_WIDTH = 0.1f;
const static float AGENT_HEIGHT = 0.05f;

// Pole attribtues
// ==================
// POLE_MASS: Mass of the agent [kg]
// POLE_FRICTION: Friction of the agent [-]
// POLE_RESTITUTION: Agent restitution [-]
// POLE_WIDTH: Agent width [m]
// POLE_HEIGHT: Agent height [m]
const static float POLE_MASS = 0.1f;
const static float POLE_FRICTION = 0.0f;
const static float POLE_RESTITUTION = 0.0f;
const static float POLE_WIDTH = 0.01f;
const static float POLE_HEIGHT = 0.18;



class KeyList
{
public:
    bool left = false;
    bool right = false;
};

enum Categories {
    BOUNDARY = 0x0001,
    RAIL =     0x0002,
    CART =     0x0004,
    POLE =     0x0008,
};

enum class GameState {
    Playing,
    Paused
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
    void pausePlay();
    float r2d(float radiant);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
private:
    GLESDebugDraw* _debugDraw;
    void setPhysicsWorld(b2World* world);
    b2World* getPhysicsWorld();
    b2World* sceneWorld;
    void update(float dt) override;
    Agent* agent;
    b2Body* agentBody;
    KeyList pressedKeys;
    float normalizeAngle(float angle);
    cocos2d::Node* rootNode;
    GameState gameState;
    b2Body* poleBody;
};


#endif // __MainScene_SCENE_H__
