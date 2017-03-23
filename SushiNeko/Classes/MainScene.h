#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Piece.h"

enum class GameState
{
    Title,
    Ready,
    Playing,
    GameOver,
};

// forward declarations
class Character;
class Piece;
enum class Side;

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    
    CREATE_FUNC(MainScene);

    
private:
    void onEnter() override;
    cocos2d::Node* pieceNode;
    cocos2d::Vector<Piece*> pieces;
    void setupTouchHandling();
    Character* character;
    
    Side lastObstacleSide;
    Side getSideForObstacle(Side lastSide) const;
    
    int pieceIndex;
    void stepTower();
    
    bool isGameOver();
    void triggerGameOver();
    GameState gameState;
    
    void resetGameState();
    void triggerPlaying();
    
    cocos2d::ui::Text* scoreLabel;
    int score;
    void setScore(int score);
    
    float timeLeft;
    void setTimeLeft(float timeLeft);
    cocos2d::Sprite* timeBar;
    
    void update(float dt) override;
    
    void triggerTitle();
    void triggerReady();
    
    cocos2d::Vec2 flyingPiecePosition;
    void animateHitPiece(Side obstacleSide);
};

#endif // __MAIN_SCENE_H__
