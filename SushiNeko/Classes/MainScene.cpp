#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "CharacterReader.h"
#include "PieceReader.h"
#include "Piece.h"
#include "Character.h"

#include <iostream>


USING_NS_CC;
using namespace cocos2d;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    /**  you can create scene with following comment code instead of using csb file.
     // 1. super init first
     if ( !Layer::init() )
     {
     return false;
     }
     
     Size visibleSize = Director::getInstance()->getVisibleSize();
     Vec2 origin = Director::getInstance()->getVisibleOrigin();
     
     /////////////////////////////
     // 2. add a menu item with "X" image, which is clicked to quit the program
     //    you may modify it.
     
     // add a "close" icon to exit the progress. it's an autorelease object
     auto closeItem = MenuItemImage::create(
     "CloseNormal.png",
     "CloseSelected.png",
     CC_CALLBACK_1(MainScene::menuCloseCallback, this));
     
     closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
     origin.y + closeItem->getContentSize().height/2));
     
     // create menu, it's an autorelease object
     auto menu = Menu::create(closeItem, NULL);
     menu->setPosition(Vec2::ZERO);
     this->addChild(menu, 1);
     
     /////////////////////////////
     // 3. add your codes below...
     
     // add a label shows "Hello World"
     // create and initialize a label
     
     auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
     
     // position the label on the center of the screen
     label->setPosition(Vec2(origin.x + visibleSize.width/2,
     origin.y + visibleSize.height - label->getContentSize().height));
     
     // add the label as a child to this layer
     this->addChild(label, 1);
     
     // add "MainScene" splash screen"
     auto sprite = Sprite::create("MainScene.png");
     
     // position the sprite on the center of the screen
     sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
     
     // add the sprite as a child to this layer
     this->addChild(sprite, 0);
     **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    // 2. init pieceIndex to 0
    this->pieceIndex = 0;
    this->gameState = GameState::Title;
    
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("CharacterReader", (ObjectFactory::Instance) CharacterReader::getInstance);
    instance->registReaderObject("PieceReader", (ObjectFactory::Instance) PieceReader::getInstance);
    
    
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    this->pieceNode = rootNode->getChildByName("pieceNode");
    this->character = rootNode->getChildByName<Character*>("cat");
    this->lastObstacleSide = Side::Left;
    this->scoreLabel = rootNode->getChildByName<ui::Text*>("scoreLabel");
    auto lifeBG = rootNode->getChildByName("lifeBG");
    this->timeBar = lifeBG->getChildByName<Sprite*>("lifeBar");
    
    for (int i = 0; i < 10; ++i)
    {
        Piece* piece = static_cast<Piece*>(CSLoader::createNode("Piece.csb"));
        float rollHeight = piece->getSpriteHeight();
        piece->setPosition(0.0f, rollHeight / 2.0f * i);
        
        // let's check where our last obstacle was located at
        this->lastObstacleSide = this->getSideForObstacle(this->lastObstacleSide);
        piece->setObstacleSide(this->lastObstacleSide);
        
        
        this->pieceNode->addChild(piece);
        
        this->pieces.pushBack(piece);
    }
    addChild(rootNode);
    this->resetGameState();
    return true;
}

// setupTouchHandling implementation
void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        switch(this->gameState)
        {
            case GameState::Playing:
            {
                // get the location of the touch in the MainScene's coordinate system
                Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
                
                // check if the touch was on the left or right side of the screen
                // move the character to the appropriate side
                if (touchLocation.x < this->getContentSize().width / 2.0f)
                {
                    
                    this->character->setSide(Side::Left);
                }
                else
                {
                    
                    this->character->setSide(Side::Right);
                }
                
                if(this->isGameOver()){
                    this->triggerGameOver();
                    return true;
                }
                this->character->runChopAnimation();
                this->stepTower();
                
                if(this->isGameOver()){
                    this->triggerGameOver();
                    return true;
                }
                
                this->setScore(this->score + 1);
                this->setTimeLeft(this->timeLeft + 0.25f);
                
            }
                break;

            case GameState::Title:
            {
                this->triggerReady();
            }
                break;

            case GameState::GameOver:
            {
                this->resetGameState();
                this->triggerReady();
            }
                break;
            case GameState::Ready:
                this->triggerPlaying();
        }
        return true;
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

// onEnter

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->triggerTitle();
    this->scheduleUpdate();
    this->flyingPiecePosition = this->pieceNode->getPosition();
};

Side MainScene::getSideForObstacle(Side lastSide) const
{
    Side side;
    
    switch (lastSide)
    {
        case Side::None:
        {
            // generate a random number between 0.0f and 1.0f
            float random = CCRANDOM_0_1();
            
            // if there wasn't an obstacle in the last piece
            // then there's a 45% chance of there being one on the left
            // 45% chance of there being one on the right
            // and 10% chance of there being no obstacle
            if (random < 0.45f)
            {
                side = Side::Left;
            }
            else if (random < 0.9f)
            {
                side = Side::Right;
            }
            else
            {
                side = Side::None;
            }
        }
            break;
            
            // if there was an obstacle in the last piece, then there isn't one for this piece
        case Side::Left:
        case Side::Right:
            side = Side::None;
            break;
    }
    
    return side;
}


void MainScene::stepTower()
{
    Piece* currentPiece = this->pieces.at(this->pieceIndex);
    this->animateHitPiece(currentPiece->getObstacleSide());
    
    // move the lowest piece to the top of the tower
    currentPiece->setPosition(currentPiece->getPosition() + Vec2(0.0f, currentPiece->getSpriteHeight() / 2.0f * 10.0f));
    
    // set the zOrder of the piece so that it appears on top of the others
    currentPiece->setLocalZOrder(currentPiece->getLocalZOrder() + 1);
    
    // set the side of the obstacle, based on the side of the obstacle of the piece right before this one
    currentPiece->setObstacleSide(this->getSideForObstacle(this->lastObstacleSide));
    this->lastObstacleSide = currentPiece->getObstacleSide();
    
    // move pieceNode down so that the whole tower moves down
    cocos2d::MoveBy* moveAction = cocos2d::MoveBy::create(0.15f, Vec2(0.0f, -1.0f * currentPiece->getSpriteHeight() / 2.0f));
    this->pieceNode->runAction(moveAction);
    // change the index referencing the lowest piece
    this->pieceIndex = (this->pieceIndex + 1) % 10;

    
};


bool MainScene::isGameOver()
{
    bool gameOver = false;
    
    // get a reference to the lowest piece
    Piece* currentPiece = this->pieces.at(this->pieceIndex);
    
    // if the obstacle and the character are touching, then it's game over!
    if (currentPiece->getObstacleSide() == this->character->getSide())
    {
        gameOver = true;
    }

    return gameOver;
};

void MainScene::triggerGameOver()
{
    this->setTimeLeft(0.0f);
    this->gameState = GameState::GameOver;
    // get a reference to the top-most node
    auto scene = this->getChildByName("Scene");
    
    // get a reference to tha mat sprite
    auto mat = scene->getChildByName("mat");
    
    // get a reference to the game over score label
    cocos2d::ui::Text* gameOverScoreLabel = mat->getChildByName<cocos2d::ui::Text*>("gameOverScoreLabel");
    
    // set the score label to the user's score
    gameOverScoreLabel->setString(std::to_string(this->score));
    
    // load and run the game over animations
    cocostudio::timeline::ActionTimeline* gameOverTimeline = CSLoader::createTimeline("MainScene.csb");
    this->stopAllActions();
    this->runAction(gameOverTimeline);
    gameOverTimeline->play("gameOver", false);
};

void MainScene::resetGameState()
{
    this->setScore(0);
    this->setTimeLeft(5.0f);
    Piece* currentPiece = this->pieces.at(this->pieceIndex);
    currentPiece->setObstacleSide(Side::None);
};

void MainScene::triggerPlaying()
{
    
    this->gameState = GameState::Playing;
};


void MainScene::setScore(int score)
{
    // update the score instance variable
    this->score = score;
    
    this->scoreLabel->setString(std::to_string(this->score));
}

void MainScene::setTimeLeft(float timeLeft)
{
    if (timeLeft >= 0.0f)
    {
        this->timeLeft = clampf(timeLeft, 0.0f, 10.0f);
        this->timeBar->setScaleX(timeLeft / 10.0f);
    }
    
}

void MainScene::update(float dt)
{
    Layer::update(dt);
    if (this->gameState == GameState::Playing)
    {
        this->setTimeLeft(this->timeLeft - dt);
        
        if (this->timeLeft <= 0.0f)
        {
            this->triggerGameOver();
        }
    }
}

void MainScene::triggerTitle()
{
    this->gameState = GameState::Title;
    cocostudio::timeline::ActionTimeline* titleTimeline = CSLoader::createTimeline("MainScene.csb");
    this->stopAllActions();
    this->runAction(titleTimeline);
    titleTimeline->play("title", false);
}

void MainScene::triggerReady()
{
    this->gameState = GameState::Ready;
    cocostudio::timeline::ActionTimeline* readyTimeline = CSLoader::createTimeline("MainScene.csb");
    this->stopAllActions();
    this->runAction(readyTimeline);
    readyTimeline->play("ready", true);
}

void MainScene::animateHitPiece(Side obstacleSide)
{
    Piece* flyingPiece = static_cast<Piece*>(CSLoader::createNode("Piece.csb"));
    flyingPiece->setObstacleSide(obstacleSide);
    flyingPiece->setPosition(this->flyingPiecePosition);
    this->addChild(flyingPiece);
    
    cocostudio::timeline::ActionTimeline* pieceTimeline = CSLoader::createTimeline("Piece.csb");
    
    Side charSide = this->character->getSide();
    std::string animationName = (charSide == Side::Left) ? std::string("moveRight") : std::string("moveLeft");
    
    flyingPiece->runAction(pieceTimeline);
    
    pieceTimeline->play(animationName, false);
    // on the last frame of the animation, remove the piece from the scene
    pieceTimeline->setLastFrameCallFunc([this, &flyingPiece]() {
        this->removeChild(flyingPiece);
    });
}
