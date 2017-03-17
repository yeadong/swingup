#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Agent.h"
#include "AgentReader.h"
#include <iostream>

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
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("AgentReader", (ObjectFactory::Instance) AgentReader::getInstance);

    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);
    

    
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
                this->agentBody->SetLinearVelocity(b2Vec2(-1.5, 0));
            }
                break;
                
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            {
                this->pressedKeys.right = true;
                this->agentBody->SetLinearVelocity(b2Vec2(1.5, 0));
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
                    this->agentBody->SetLinearVelocity(b2Vec2(1.5, 0));
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
                    this->agentBody->SetLinearVelocity(b2Vec2(-1.5, 0));
                }
                else {
                    this->agentBody->SetLinearVelocity(b2Vec2(0, 0));
                }
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

b2World* MainScene::getSceneWorld()
{
    b2World* world = this->sceneWorld;
    return world;
}


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
    
    
    // let's add a physically active outline to our window so that the agent cannot pass the edge of the screen
    // to do so, we first create a variable called edgeBoxDefinition of type b2BodyDef and create an instance of
    // type b2Body*, an b2EdgeShape and a b2FixtureDef.
    
    b2BodyDef edgeBoxBodyDefinition;
    b2Body* edgeBoxBody = world->CreateBody(&edgeBoxBodyDefinition);
    
    b2EdgeShape edgeBox;
    b2FixtureDef edgeBoxDef;
    edgeBoxDef.shape = &edgeBox;
    
    //top
    edgeBox.Set(b2Vec2(origin.x  / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO),
                  b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, (origin.y + visibleSize.height)/ PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    
    //right
    edgeBox.Set(b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO),
                  b2Vec2((origin.x + visibleSize.width) / PTM_RATIO, origin.y / PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    //bottom
    edgeBox.Set(b2Vec2(origin.x / PTM_RATIO, -17.0f/PTM_RATIO), b2Vec2((origin.x + visibleSize.width)/PTM_RATIO, -17.0f/PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    //left
    edgeBox.Set(b2Vec2(origin.x / PTM_RATIO, origin.y / PTM_RATIO), b2Vec2(origin.x / PTM_RATIO, (origin.y + visibleSize.height) / PTM_RATIO));
    edgeBoxBody->CreateFixture(&edgeBoxDef);
    
    
    
    
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
    
    b2Vec2 center = b2Vec2((origin.x + visibleSize.width * 0.5) / PTM_RATIO, (origin.y + visibleSize.height * 0.5)/ PTM_RATIO);
    this->agent = static_cast<Agent*>(CSLoader::createNode("Agent.csb"));
    Sprite* agentSprite = this->agent->getChildByName<Sprite*>("agent");
    agentSprite->setTag(1);
    agentSprite->setPosition(Vec2(center.x, center.y));
    
    this->addChild(agentSprite);
        
    b2BodyDef agentBodyDef;
    agentBodyDef.type = b2_dynamicBody;
    agentBodyDef.fixedRotation = true;
    agentBodyDef.allowSleep = true;
    agentBodyDef.userData = agentSprite;
    agentBodyDef.position.Set(center.x, center.y);
    b2Body* agentBody = world->CreateBody(&agentBodyDef);
    
    
    
    // Now that we have created a physical body, we need to attach
    // a shape to it so that our physics engine knows its collision
    // geometry. We achieve this by creating a b2PolygonShape called agentShape
    // and call the SetAsBox method. Note that SetAsBox takes the HALF-width
    // and HALF-height as its arguments.
    // Hence, our agent will be 200px wide and 100px tall, which approximately
    // equals a width of 200px/ 1134 (px/m) = 17.6cm and a height of
    // 100px/ 1134 (px/m) = 8.81cm. These values are currently arbitrarily set.
    
    b2PolygonShape agentShape;
    agentShape.SetAsBox(100.0f/ PTM_RATIO, 50.0f/ PTM_RATIO);
    
    b2FixtureDef agentFixtureDef;
    agentFixtureDef.shape = &agentShape;
    agentFixtureDef.density = 10.0f;
    agentFixtureDef.friction = 0.0f;
    agentFixtureDef.restitution = 0.1f;
    agentBody->CreateFixture(&agentFixtureDef);
    
    // Call setBody on this->agent to have later access to the body
    // in case we need to modify it. Finally, schedule an update
    this->agentBody = agentBody;
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::update));
}

void MainScene::update(float dt)
{
    Layer::update(dt);
    int velocityIterations =10;
    int positionIterations = 10;
    
    this->getSceneWorld()->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* body = this->getSceneWorld()->GetBodyList(); body; body = body->GetNext())
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
