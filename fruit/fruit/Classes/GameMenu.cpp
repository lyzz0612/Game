//
//  GameMenu.cpp
//  fruit
//  游戏菜单
//  Created by yun ling on 13-7-8.
//
//

#include "GameMenu.h"

CCScene* GameMenu::scene()
{
    CCScene* pscene = new CCScene();
    GameMenu* player = GameMenu::create();
    pscene->addChild(player);
    return pscene;
}

GameMenu* GameMenu::create()
{
    GameMenu* myGame = new GameMenu();
    if(myGame && myGame->init())
    {
        myGame->autorelease();
        return myGame;
    }
    CC_SAFE_DELETE(myGame);
    return NULL;
}

bool GameMenu::init()
{
    if(!CCLayer::init())
        return false;
    //游戏背景
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* bgimage = CCSprite::create("background.jpg");
    bgimage->setPosition(ccp(size.width / 2, size.height / 2));
    this->addChild(bgimage,0);
    
    //最上面的遮罩，它是慢慢进入显示界面的
    CCSprite* homeMask = CCSprite::create("home-mask.png");
    CCActionInterval* maskto = CCMoveTo::create(1, ccp(size.width / 2, size.height - homeMask->getContentSize().height / 2));
    homeMask->setPosition(ccp(size.width / 2, size.height + homeMask->getContentSize().height / 2));
    homeMask->runAction(maskto);
    this->addChild(homeMask, 1);
    
    //ninja图片，它进入显示界面后有跳动效果
    CCSprite* ninja = CCSprite::create("ninja.png");
    ninja->setPosition(ccp(size.width - ninja->getContentSize().width / 2, size.height + homeMask->getContentSize().height / 2));
    CCActionInterval* delay = CCMoveTo::create(1,ninja->getPosition());
    CCActionInterval* ninjajump = CCJumpTo::create(1, ccp(size.width - ninja->getContentSize().width / 2, size.height - ninja->getContentSize().height),-100,1);
    ninja->runAction(CCSequence::create(delay,ninjajump,NULL));
    this->addChild(ninja, 2);
    
    CCActionInterval* startRotate = CCRotateBy::create(2, 360);
    CCActionInterval* newGameRotate = CCRotateBy::create(2, 90);
    CCActionInterval* QuitGameRotate = CCRotateBy::create(2, 90);
    CCRepeatForever* rotatefor = CCRepeatForever::create(startRotate);
    
    start = CCSprite::create("sandia.png");
    start->setPosition(ccp(100, 100));
    CCSprite* newGame = CCSprite::create("new-game.png");
    newGame->setPosition(start->getPosition());
    start->runAction(rotatefor);
    newGame->runAction(CCRepeatForever::create(newGameRotate));
    this->addChild(start,1);
    this->addChild(newGame, 1);
    
    quit = CCSprite::create("boom.png");
    quit->setPosition(ccp(size.width - 100, 100));
    CCSprite* quitGame = CCSprite::create("quit.png");
    quitGame->setPosition(quit->getPosition());
    quitGame->runAction(CCRepeatForever::create(QuitGameRotate));
    this->addChild(quit,1);
    this->addChild(quitGame, 1);
    
    this->setTouchEnabled(true);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menu.mp3", true);
    return true;
}
void GameMenu::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
bool GameMenu::ccTouchBegan(CCTouch *ptouch, CCEvent *pEvent)
{
    par = CCParticleSun::create();
    par->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png"));
    this->addChild(par, 5);
    return true;
}
void GameMenu::ccTouchMoved(CCTouch *ptouch, CCEvent *pEvent)
{
    CCPoint cur = this->convertTouchToNodeSpace(ptouch);
    par->setPosition(cur);
    if(isOnStart(cur))
        StartFun();
    else if(isOnQuit(cur))
        QuitFun();
        
}
void GameMenu::ccTouchEnded(CCTouch *ptouch, CCEvent *pEvent)
{
    if(par)
        this->removeChild(par);
}
bool GameMenu::isOnStart(CCPoint cur)
{
    CCSize startSize = start->getContentSize();
    CCPoint startPoint = start->getPosition();
    CCRect* rect = new CCRect(startPoint.x - startSize.width / 2, startPoint.y - startSize.height / 2,startSize.width,startSize.height);
    return rect->containsPoint(cur);
}
bool GameMenu::isOnQuit(CCPoint cur)
{
    CCSize quitSize = quit->getContentSize();
    CCPoint quitPoint = quit->getPosition();
    CCRect* rect = new CCRect(quitPoint.x - quitSize.width / 2, quitPoint.y - quitSize.height / 2,quitSize.width,quitSize.height);
    return rect->containsPoint(cur);
}
void GameMenu::StartFun()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("swoosh.caf", false);
    CCScene* myGame = Game::scene();
    CCDirector::sharedDirector()->replaceScene(myGame);
}
void GameMenu::QuitFun()
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
