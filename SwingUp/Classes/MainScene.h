#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "Agent.h"
#include "GLES-Render.h"
#include "ui/CocosGUI.h"
#include <Box2D/Box2D.h>


// Globals definition
// ==================
// PTM_RATIO: Pixel to meter ratio
// As we're initiliazing our display 1136px wide, a PTM ratio
// of 1136/3 sets our screen width to equal 3 meters.

// VELOCITY
// Horizontal Velocity of the agent
const static float PTM_RATIO = 1136.0f / 3.0f;
const static float VELOCTY = 1.7f;
const static int VELOCITY_ITERATIONS =10;
const static int POSITION_ITERATIONS = 10;

// Agent attribtues
// ==================
// AGENT_MASS: Mass of the agent [kg]
// AGENT_FRICTION: Friction of the agent [-]
// AGENT_RESTITUTION: Agent restitution [-]
// AGENT_WIDTH: Agent width [m]
// AGENT_HEIGHT: Agent height [m]
const static float AGENT_MASS = 2.8f;
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
const static float POLE_MASS = 1.0f;
const static float POLE_FRICTION = 0.0f;
const static float POLE_RESTITUTION = 0.0f;
const static float POLE_WIDTH = 0.01f;
const static float POLE_HEIGHT = 0.35f;
const static float POLE_ANGULAR_DAMPING = 0.2f;


// The KeyList is used to keep track
// on user input, since the keyboard listener
// can only listen to key strokes and releases,
// we need a structure we can consult to implement
// following edge cases:
// + Press L Press R Release L, expect to go R again
// + Press R Press L Release R, expect to go L again
class KeyList
{
public:
    bool left = false;
    bool right = false;
};


// Here we define our mask and category bits
// for our physics model. These are used to define
// every physics body in following ways:
// The categoryBits define WHAT the body IS, e.g.
// a boundry, a rail, cart or pole.
// The `maskBits` on the other hand defined with which
// other body the body will COLLIDE with
enum Categories {
    BOUNDARY = 0x0001,
    RAIL =     0x0002,
    CART =     0x0004,
    POLE =     0x0008,
};


// We utilize ths GameState class to play/ pause the game
// by pressing space in order we want to check
// e.g. the reward value or similar.
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
    void setPhysicsWorld(b2World* world);
    b2World* getPhysicsWorld();
    float normalizeAngle(float angle);
    float r2d(float radiant);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
    // reward functions
    // These are used to calculate and set the reward
    // internally plus updating UI Label
    void setReward(float reward);
    float calcLinearReward(float angle);
    float calcCosReward(float angle);
    float calcCombinedReward(float angle);
    
    
private:
    GLESDebugDraw* _debugDraw;
    b2World* sceneWorld;
    void update(float dt) override;
    Agent* agent;
    b2Body* agentBody;
    KeyList pressedKeys;
    cocos2d::Node* rootNode;
    GameState gameState;
    b2Body* poleBody;
    cocos2d::ui::Text* rewardLabel;
    float reward;
};


#endif // __MainScene_SCENE_H__
