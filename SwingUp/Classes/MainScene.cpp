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
    auto scene = Scene::createWithPhysics();
    // draw physics outlines
    
    // ------------- remove this to remove the outlines --------
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -9810.0f));
    std::cout << "Aber ich bin zuerst \n";
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    
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


void MainScene::setPhysicsWorld(PhysicsWorld* world)
{
    std::cout << "Getting caaaaled\n";
    this->sceneWorld = world;
}


void MainScene::setupKeyboardHandling()
{
    EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
    
    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            {
                this->agent->getPhysicsBody()->applyImpulse(Vec2(-4000000.0f, 0));
            }
                break;
                
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            {
                this->agent->getPhysicsBody()->applyImpulse(Vec2(4000000.0f, 0));
            }
                break;
                
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

PhysicsWorld* MainScene::getSceneWorld()
{
    PhysicsWorld* world = this->sceneWorld;
    return world;
}


void MainScene::setupWorld()
{   Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /** let's add a physically active outline to our window so that the agent cannot pass the edge of the screen
     ** to do so, we first create a physicsbody of type cocos2d::PhysicsBody* called edge body and create an edge box
     ** after, we create an edgeNode of type cocos2d::Node*, set the position to the origin and assign the edgeBody as
     ** the edgeNode's physics body.
     **/
    Node* edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    PhysicsBody* edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);
    
    // let's define our rail body and shape. As our rail is not going to move,
    // its body will be static. Hence, we set our dynamic bool to false
    // after we have declared the railBody. After, we create a box shape and add it to the body.
    // Finally, let's create a node, set its position and call the addChild method on it
    PhysicsBody* railBody = PhysicsBody::createBox(Size(visibleSize.width, 20.0f), PhysicsMaterial(0.1f, 0, 0.001f));
    railBody->setDynamic(false);
    PhysicsShapeBox* railShape = PhysicsShapeBox::create(Size(visibleSize.width, 20.0f), PhysicsMaterial(0.1f, 0, 0.001f));
    railBody->addShape(railShape);
    railBody->setCategoryBitmask(1);
    
    Node* railNode = Node::create();
    railNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    railNode->setPhysicsBody(railBody);
    this->addChild(railNode);
    
    // pole setup
    PhysicsBody* poleBody = PhysicsBody::createBox(Size(10.0f, 150.0f), PhysicsMaterial(0.1f, 0, 0.001f));
    
    PhysicsShapeBox* poleShape = PhysicsShapeBox::create(Size(10.5, 150.0f), PhysicsMaterial(0.1f, 0, 0.001f));
    poleBody->addShape(poleShape);
    poleBody->setCategoryBitmask(0);
    //poleBody->setDynamic(false);
    Node* poleNode = Node::create();
    poleNode->setPhysicsBody(poleBody);
    poleNode->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height/2 - 45.0f)));
    this->addChild(poleNode);
    
    // now let's create our agent:
    this->agent = static_cast<Agent*>(Node::create());
    this->agent->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + 30.0f));
    
    PhysicsBody* agentBody = PhysicsBody::createBox(Size(100.0f, 40.0f), PhysicsMaterial(2.0f, 1.0f, 0.001f));
    //agentBody->setDynamic(false);
    PhysicsShapeBox* agentShape = PhysicsShapeBox::create(Size(100.0f, 40.0f), PhysicsMaterial(0.1f, 1.0f, 0.001f));
    agentBody->addShape(agentShape);
    poleBody->setCategoryBitmask(0);
    
    this->agent->setPhysicsBody(agentBody);
    this->addChild(this->agent);
    
    PhysicsJointPin* pivotJoint = PhysicsJointPin::construct(agentBody, poleBody, Vec2(0, 0), Vec2(0, 75.0f));
    pivotJoint->setCollisionEnable(false);
    pivotJoint->createConstraints();
    PhysicsWorld* world = this->getScene()->getPhysicsWorld();
    world->addJoint(pivotJoint);    
    
}
