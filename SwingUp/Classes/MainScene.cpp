#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Agent.h"
#include "AgentReader.h"
#include "RailReader.h"
#include "Rail.h"
#include <iostream>
#include "math.h"

USING_NS_CC;

using namespace cocos2d;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // draw physics outlines
    
        // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    
    // add layer as a child to scene
    scene->addChild(layer);
    layer->setupWorld();
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    this->gameState = GameState::Playing;
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("AgentReader", (ObjectFactory::Instance) AgentReader::getInstance);

    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);
    this->rootNode = rootNode;

    
    // setup event dispatcher
    this->setupKeyboardHandling();
    return true;
}


void MainScene::setPhysicsWorld(b2World* world)
{
    this->sceneWorld = world;
}


// Let's setup our keyboard handling:
// In our game, the user controls the agent with the arrow keys.
// We'll handle four scenarios here:

// No keys pressed priorly
// =================================
// + User presses left arrow key -> agent moves left
// + User presses right arrow key -> agent moves right

// Keys pressed priorly
// =================================
// + User presse
void MainScene::setupKeyboardHandling()
{
    EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
    
    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            {
                this->pressedKeys.left = true;
                this->agentBody->SetLinearVelocity(b2Vec2(-VELOCTY, 0));
            }
                break;
                
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            {
                this->pressedKeys.right = true;
                this->agentBody->SetLinearVelocity(b2Vec2(VELOCTY, 0));
            }
                break;
            case EventKeyboard::KeyCode::KEY_SPACE:
            {
                if (this->gameState == GameState::Playing)
                {
                    this->gameState = GameState::Paused;
                }
                else
                {
                    this->gameState = GameState::Playing;
                }
            }
                break;
                
        }
    };
    keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            {
                this->pressedKeys.left = false;
                
                if(this->pressedKeys.right)
                {
                    this->agentBody->SetLinearVelocity(b2Vec2(VELOCTY, 0));
                }
                else {
                    this->agentBody->SetLinearVelocity(b2Vec2(0, 0));
                }
            }
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            {
                this->pressedKeys.right = false;
                
                if(this->pressedKeys.left)
                {
                    this->agentBody->SetLinearVelocity(b2Vec2(-VELOCTY, 0));
                }
                else {
                    this->agentBody->SetLinearVelocity(b2Vec2(0, 0));
                }
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

/*
* @desc A simple getter class to gain access to the simulation's physics world
* @return type b2World*
* @params: None
*/
b2World* MainScene::getPhysicsWorld()
{
    b2World* world = this->sceneWorld;
    return world;
}


/*
 * @desc
 * This is where the magic happens: First, we set up our physics world.
 * After, we initialize all bodies which are needed for simulation:
 * + The edge boxes preventing the agent from falling off the rail
 * + The rail on which the agent rides along
 * + The agent itself
 * + The pole
 * + The revolute joint between the agent and the pole.
 * @return type b2World*
 * @params: None
 */
void MainScene::setupWorld()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
    // Let's initialize our world:
    // It takes one argument of type b2Vec2 which defines
    // our gravity. After, we call setPhysicsWorld to attach
    // the world to the MainScene.
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    b2World* world = new b2World(gravity);

    this->setPhysicsWorld(world);
    this->_debugDraw = new GLESDebugDraw(PTM_RATIO);
    this->_debugDraw->SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit|b2Draw::e_pairBit);
    this->getPhysicsWorld()->SetDebugDraw(this->_debugDraw);
    
    
    // let's add a physically active outline to our window so that the agent cannot pass the edge of the screen
    // to do so, we first create a variable called edgeBoxDefinition of type b2BodyDef and create an instance of
    // type b2Body*, an b2EdgeShape and a b2FixtureDef.
    b2Vec2 center = b2Vec2((origin.x + visibleSize.width * 0.5) / PTM_RATIO, (origin.y + visibleSize.height * 0.5)/ PTM_RATIO);
    b2BodyDef edgeBoxBodyDefinition;
    b2Body* edgeBoxBody = world->CreateBody(&edgeBoxBodyDefinition);
    
    b2EdgeShape edgeBox;
    b2FixtureDef edgeBoxDef;
    edgeBoxDef.shape = &edgeBox;
    edgeBoxDef.filter.categoryBits = Categories::BOUNDARY;
    edgeBoxDef.filter.maskBits = Categories::CART | Categories::POLE;
    
    //right
    edgeBox.Set(b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO), b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, origin.y / PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    //left
    edgeBox.Set(b2Vec2(origin.x / PTM_RATIO, origin.y / PTM_RATIO), b2Vec2(origin.x / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    
    // let's implement the rail on which our agent will move along
    // to do so, we first create a b2BodyDef and a b2Body* and assign
    // the rail sprite to it:
    
    
    // Sprite* railSprite = this->rootNode->getChildByName<Sprite*>("rail");
    
    b2BodyDef railBodyDef;
    railBodyDef.type = b2_staticBody;
    //railBodyDef.userData = railSprite;
    railBodyDef.position.Set(center.x, (center.y - 0.08f));
    b2Body* railBody = world->CreateBody(&railBodyDef);
    b2PolygonShape railShape;
    railShape.SetAsBox(2.0f, 0.02f);
    
    b2FixtureDef railFixtureDef;
    railFixtureDef.shape = &railShape;
    railFixtureDef.density = 0.0f;
    railFixtureDef.filter.categoryBits = Categories::RAIL;
    railFixtureDef.filter.maskBits = Categories::CART;
    railFixtureDef.friction = 0.001f;
    railFixtureDef.restitution = 0.0f;
    railBody->CreateFixture(&railFixtureDef);
    
    // Now lets create our agent, define it's physical body and shape
    // To do so, we first intiantiate an agent and assign it to the scene
    // by calling this->agent = .... Note that we use static_cast in order
    // downcast the type from cocos2d::Node* to our own Agent*
    // After, we get the agent's sprite, assign it to its position and add
    // it to our Scene. Now we create a b2BodyDef called agentBoxdef,
    // set its type to dynamic, as we want our agent to experience forces,
    // position it and assign the agentSprite as userData. This will create a link
    // between the agent body and sprite.
    // Finally, we create the actual pyhsical body by calling `CreateBody` on world.
    
    this->agent = static_cast<Agent*>(CSLoader::createNode("Agent.csb"));
    Sprite* agentSprite = this->agent->getChildByName<Sprite*>("agent");
    agentSprite->setTag(1);
    agentSprite->setPosition(Vec2(center.x, center.y));
    
    //this->addChild(agentSprite);
        
    b2BodyDef agentBodyDef;
    agentBodyDef.type = b2_dynamicBody;
    agentBodyDef.fixedRotation = true;
    agentBodyDef.allowSleep = true;
    
    //agentBodyDef.userData = agentSprite;
    agentBodyDef.position.Set(center.x, center.y);
    b2Body* agentBody = world->CreateBody(&agentBodyDef);
    
    
    // Now that we have created a physical body, we need to attach
    // a shape to it so that our physics engine knows its collision
    // geometry. We achieve this by creating a b2PolygonShape called agentShape
    // and call the SetAsBox method. Note that SetAsBox takes the HALF-width
    // and HALF-height as its arguments.
    
    
    b2PolygonShape agentShape;
    agentShape.SetAsBox(AGENT_WIDTH/2, AGENT_HEIGHT/2);
    
    b2FixtureDef agentFixtureDef;
    agentFixtureDef.shape = &agentShape;
    agentFixtureDef.density = AGENT_MASS;
    agentFixtureDef.friction = AGENT_FRICTION;
    agentFixtureDef.restitution = AGENT_RESTITUTION;
    agentFixtureDef.filter.categoryBits = Categories::CART;
    agentFixtureDef.filter.maskBits = Categories::RAIL | Categories::BOUNDARY;
    agentBody->CreateFixture(&agentFixtureDef);
    
    // Call setBody on this->agent to have later access to the body
    // in case we need to modify it. Finally, schedule an update
    this->agentBody = agentBody;
    
    
    // Now we have created our rail and agent, it's time to
    // initialize our pole, which we'll later constrain to
    // our agent. There's nothing new here to point out.
    
    // Pole Dimensions: 2cm in width, 30cm in length.
    // Pole Physics:
    // + Restitution: 0.1,
    // + Mass: 1kg
    
    b2BodyDef poleBodyDef;
    poleBodyDef.type = b2_dynamicBody;
    poleBodyDef.allowSleep = true;
    poleBodyDef.position.Set(center.x, (center.y - POLE_HEIGHT/2));
    b2Body* poleBody = world->CreateBody(&poleBodyDef);
    
    b2PolygonShape poleShape;
    poleShape.SetAsBox(POLE_WIDTH/2, POLE_HEIGHT/2);
    
    b2FixtureDef poleFixtureDef;
    poleFixtureDef.shape = &poleShape;
    poleFixtureDef.filter.categoryBits = Categories::POLE;
    poleFixtureDef.filter.maskBits = Categories::BOUNDARY;
    poleFixtureDef.density = POLE_MASS;
    poleFixtureDef.friction = POLE_FRICTION;
    poleFixtureDef.restitution = POLE_RESTITUTION;
    poleBody->CreateFixture(&poleFixtureDef);
    this->poleBody = poleBody;
    
    
    // joint definition
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = agentBody;
    jointDef.bodyB = poleBody;
    jointDef.localAnchorA = agentBody->GetLocalCenter();
    
    b2Vec2 polePosition = b2Vec2(poleBody->GetLocalCenter().x, poleBody->GetLocalCenter().y + POLE_HEIGHT/2);
    jointDef.localAnchorB = polePosition;
    b2RevoluteJoint* joint = static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef));
    
    
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::update));
}

void MainScene::update(float dt)
{
    if (this->gameState == GameState::Playing)
    {
        Layer::update(dt);
        int velocityIterations =10;
        int positionIterations = 10;
        
        this->getPhysicsWorld()->Step(dt, velocityIterations, positionIterations);
        
        CCLOG("Angle: %.2f", this->normalizeAngle(this->r2d(this->poleBody->GetAngle())));
        for (b2Body* body = this->getPhysicsWorld()->GetBodyList(); body; body = body->GetNext())
        {
            
            
            if (body->GetUserData() != NULL) {
                
                Sprite* sprite = (Sprite*)body->GetUserData();
                
                Vec2 position = Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO);
                
                CCLOG("Position:%.2f,%.2f", position.x, position.y);
                sprite->setPosition(position);
                sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            }
        }
    }
}


void MainScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    this->getPhysicsWorld()->DrawDebugData();
}

float MainScene::normalizeAngle(float angle)
{
    angle = fmod(angle + 180,360);
    if (angle < 0)
        angle += 360;
    return angle - 180.0f;
}

float MainScene::r2d(float r)
{
    float ret = r*180.0f/M_PI;
    return ret;
}


void MainScene::pausePlay()
{
    this->gameState = GameState::Paused;
}
