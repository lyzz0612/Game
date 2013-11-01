//
//  Game.cpp
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#include "Game.h"
#include "GameMenu.h"
Game::Game()
{
    
}

Game::~Game()
{
    CC_SAFE_RELEASE(myFruit);
}

CCScene* Game::scene()
{
    CCScene* scene = CCScene::create();
    CCLayer* layer = Game::create();
    scene->addChild(layer);
    return scene;
}
bool Game::init()
{
    if (!CCLayer::init())
        return false;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* bgimage = CCSprite::create("background.jpg");
    bgimage->setPosition(ccp(size.width / 2, size.height / 2));
    this->addChild(bgimage,0);
    
    showScore = CCLabelTTF::create("Score: 0", "Helvetica", 20);
    showScore->setPosition(ccp(50,size.height - 30));
    this->addChild(showScore,3);
    
    pause = CCSprite::create("restart.png");
    pause->setPosition(ccp(size.width / 2 - 50, size.height - 30));
    this->addChild(pause, 3);
    
    CCSprite* errorshow1 = CCSprite::create("x_unfilled-hd.png");
    errorshow1->setPosition(ccp(size.width - 50,size.height - 30));
    this->addChild(errorshow1, 3);
    CCSprite* errorshow2 = CCSprite::create("x_unfilled-hd.png");
    errorshow2->setPosition(ccp(size.width - 100,size.height - 30));
    this->addChild(errorshow2, 3);
    CCSprite* errorshow3 = CCSprite::create("x_unfilled-hd.png");
    errorshow3->setPosition(ccp(size.width - 150,size.height - 30));
    this->addChild(errorshow3, 3);
    
    score = 0;
    error = 0;
    quit = NULL;
    par = NULL;
    ispause = false;
    myFruit = new CCArray();
    myFruit->retain();
    this->setTag(0);
    srand(time(NULL));
    
    this->schedule(schedule_selector(Game::Product), 0.5);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    return true;
}
void Game::Product()
{
    CheckError();
    if(!this->isTouchEnabled())
        this->setTouchEnabled(true);
    
    int ran = CCRANDOM_0_1() * 100;
    int num = 0;
    if( ran < 70)
        num = 0;
    else if(ran < 90)
        num = 1;
    else
        num = 2;
    for(int i = 0;i < num;i++)
    {
        Fruit* item = Factory::sharedFactory()->product();
        this->addChild(item,2);
        myFruit->addObject(item);
    }
}
void Game::CheckError()
{
    if(error != this->getTag())
    {
        error = this->getTag();
        CCSprite* errorNum = CCSprite::create("x_filled-hd.png");
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        float x;
        switch (error) {
            case 1:
                x = size.width - 150;
                break;
            case 2:
                x = size.width - 100;
                break;
            case 3:
                x = size.width - 50;
                break;
            default:
                x = 0;
                break;
        }
        if(x != 0)
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("lose_life.caf", false);
            errorNum->setPosition(ccp(x,size.height - 30));
            this->addChild(errorNum, 4);
        }
        CCObject* pObject;
        CCARRAY_FOREACH(myFruit, pObject)
        {
            if(!pObject)
                myFruit->removeObject(pObject);
        }
    }
    if(error >= 3)
        GameOver();
}

void Game::Boom()
{
    this->unscheduleAllSelectors();
    CCObject* pObject;
    CCARRAY_FOREACH(myFruit, pObject)
    {
        Fruit* item = (Fruit*)(pObject);
        CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(item);
        myFruit->removeObject(pObject);
        this->removeChild(item);
    }
    this->error = 4;
    
    CCActionInterval* white = CCFadeIn::create(1);
    CCSequence* act = CCSequence::create(white, CCCallFunc::create(this, callfunc_selector(Game::GameOver)),NULL);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* AllWhite = CCSprite::create("AllWhite.png");
    AllWhite->setScale(100);
    AllWhite->setTag(1000);
    AllWhite->setPosition(ccp(size.width / 2, size.height / 2));
    AllWhite->runAction(act);
    this->addChild(AllWhite, 10);
}
void Game::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool Game::ccTouchBegan(CCTouch *ptouch, CCEvent *pEvent)
{
    CCPoint cur = this->convertTouchToNodeSpace(ptouch);
    if(isInside(cur, this->pause))
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        if(ispause == false)
        {
            this->removeChild(pause);
            pause = CCSprite::create("pause.png");
            pause->setPosition(ccp(size.width / 2 - 50, size.height - 30));
            this->addChild(pause, 3);
            ispause = true;
            CCDirector::sharedDirector()->pause();
        }
        else
        {
            this->removeChild(pause);
            pause = CCSprite::create("restart.png");
            pause->setPosition(ccp(size.width / 2 - 50, size.height - 30));
            this->addChild(pause, 3);
            ispause = false;
            CCDirector::sharedDirector()->resume();
        }
        return false;
    }
    if(ispause == true)
        return false;
    if(error < 3)
    {
        par = CCParticleSun::create();
        par->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png"));
        this->addChild(par, 10);
    }
    return true;
}
void Game::ccTouchMoved(CCTouch *ptouch, CCEvent *pEvent)
{
    CCPoint cur = this->convertTouchToNodeSpace(ptouch);
    if (error >= 3 && quit)
    {
        if ( isInside(cur, quit))
            ReturnToMenu();
        return ;
    }
    par->setPosition(cur);

    CCObject* pObject;
    CCARRAY_FOREACH(myFruit, pObject)
    {
        Fruit* item = (Fruit*)pObject;
        if(item && isInside(cur, item))
        {
            this->score += item->getVal();
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(item);
            item->delAction(cur);
            if(item->isBoom())
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("explosion.caf", false);
                this->Boom();
                break;
            }
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("squash.caf", false);
            myFruit->removeObject(item);
            item->removeFromParent();
        }
    }
    char* str = new char[10];
    std::sprintf(str, "Score: %d",this->score);
    showScore->setString(str);
}
void Game::ccTouchEnded(CCTouch *ptouch, CCEvent *pEvent)
{
    if(par)
        this->removeChild(par);
}

bool Game::isInside(CCPoint cur, CCSprite* target)
{
    CCRect rect = target->boundingBox();
    return rect.containsPoint(cur);
}

void Game::ReturnToMenu()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("swoosh.caf", false);
    CCScene* lastScene = GameMenu::scene();
    CCDirector::sharedDirector()->replaceScene(lastScene);
}

void Game::GameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Game-over.mp3", false);
    
    this->unscheduleAllSelectors();
    CCDirector::sharedDirector()->getActionManager()->removeAllActions();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->removeChildByTag(1000);
    quit = CCSprite::create("boom.png");
    quit->setPosition(ccp(size.width - 50, 50));
    this->addChild(quit, 5);
    
    CCSprite* gameover = CCSprite::create("game-over.png");
    gameover->setPosition(ccp(size.width / 2, size.height / 2));
    this->addChild(gameover, 5);
}