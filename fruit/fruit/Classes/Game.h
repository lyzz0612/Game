//
//  Game.h
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#ifndef fruit_Game_h
#define fruit_Game_h
#include "cocos2d.h"
#include "Factory.h"
USING_NS_CC;

class Game : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene();
    virtual bool init();
    Game();
    ~Game();
    void Product();
    void CheckError();
    void Boom();
    void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *ptouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *ptouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *ptouch, CCEvent *pEvent);
    CREATE_FUNC(Game);
protected:
    CCArray* myFruit;
    CCSprite* quit;
    CCSprite* pause;
    CCLabelTTF* showScore;
    CCParticleSun* par;
    int score;
    int error;
    bool ispause;
    void GameOver();
    void ReturnToMenu();
    bool isInside(CCPoint,CCSprite*);
};

#endif
