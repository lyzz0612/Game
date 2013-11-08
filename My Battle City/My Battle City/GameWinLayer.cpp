//
//  GameWinLayer.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-18.
//
//

#include "GameWinLayer.h"
#include "GameMenu.h"
#include "Tank.h"
#include "MainGame.h"

USING_NS_CC;


GameWinLayer::GameWinLayer()
{
    a=0;
    b=0;
    c=0;
    d=0;
    e=0;
    f=0;
    g=0;
}

GameWinLayer* GameWinLayer::create(bool _isGameOver, int level)
{
    GameWinLayer* pRet = new GameWinLayer();
    if (pRet && pRet->init(_isGameOver,level))
    {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool GameWinLayer::init(bool _isGameOver, int level)
{
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    if ( !CCLayerColor::initWithColor(ccc4(255 ,228, 181, 200), wsize.width/3*2, wsize.height/2))
    {
        return false;
    }
    
    isGameOver = _isGameOver;
    m_level = level;
    
    menuLayout();
    
    CCSize size = this->getContentSize();
    
    
    Tank* tank1 = Tank::create(1);
    tank1->setPosition(ccp(size.width/8,size.height/8*7));
    tank1->setScale(2.0f);
    this->addChild(tank1);
    
    Tank* tank2 = Tank::create(2);
    tank2->setPosition(ccp(size.width/8,size.height/8*5));
    tank2->setScale(2.0f);
    this->addChild(tank2);
    
    Tank* tank3 = Tank::create(3);
    tank3->setPosition(ccp(size.width/8,size.height/8*3));
    tank3->setScale(2.0f);
    this->addChild(tank3);
    
    
    return true;
}


void GameWinLayer::menuLayout()
{
    CCSize size = this->getContentSize();
    
    CCMenuItemFont *pCloseItem = CCMenuItemFont::create("Back to Menu",
                                                        this,
                                                        menu_selector(GameWinLayer::menuBackCallback) );
    pCloseItem->setPosition( ccp(size.width/7, size.height/16) );
    pCloseItem->setFontSizeObj(25);
    pCloseItem->setFontNameObj("Georgia-Bold");
    pCloseItem->setColor(ccc3(47 ,79, 79));
    
    CCMenuItemFont* pStartItem = CCMenuItemFont::create("Next",
                                                        this,
                                                        menu_selector(GameWinLayer::menuNextCallback) );
    pStartItem->setPosition(ccp(size.width/8*7,size.height/16));
    pStartItem->setFontSizeObj(30);
    pStartItem->setFontNameObj("Georgia-Bold");
    pStartItem->setColor(ccc3(34, 139, 34));
    if (isGameOver) {
        pStartItem->setVisible(false);
    }
    else
    {
        pStartItem->setVisible(true);
    }
    
    CCMenuItemFont* pRetryItem = CCMenuItemFont::create("Retry",
                                                        this,
                                                        menu_selector(GameWinLayer::menuRetryCallback) );
    pRetryItem->setPosition(ccp(size.width/8*7,size.height/16));
    pRetryItem->setFontSizeObj(30);
    pRetryItem->setFontNameObj("Georgia-Bold");
    pRetryItem->setColor(ccc3(139, 58, 58));
    if (isGameOver) {
        pRetryItem->setVisible(true);
    }
    else
    {
        pRetryItem->setVisible(false);
    }
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pStartItem, pRetryItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
}


void GameWinLayer ::setResult()
{
    CCSize size = this ->getContentSize();
    
    CCLabelTTF *_level = CCLabelTTF ::create("Stage ", "Georgia-Bold", 25);
    _level->setString( CCString::createWithFormat("Stage : %d" , m_level)->getCString());
    _level->setAnchorPoint( ccp(0.5, 1.0));
    _level->setPosition( ccp(size.width/2,size.height));
    _level->setColor(ccc3(0, 0, 0));
    this->addChild(_level);
    
    
    CCLabelTTF *score1 = CCLabelTTF ::create(" ", "Georgia-Bold", 25);
    score1->setString( CCString::createWithFormat("%d     x     100     =    %d" , a, b)->getCString());
    score1->setAnchorPoint( ccp(0, 0.5));
    score1->setPosition( ccp(size.width/4,size.height/8*7));
    score1->setColor(ccc3(0, 0, 0));
    this->addChild(score1);
    
    
    CCLabelTTF *score2 = CCLabelTTF ::create(" ", "Georgia-Bold", 25);
    score2->setString( CCString::createWithFormat("%d     x     100     =    %d" , c, d)->getCString());
    score2->setAnchorPoint( ccp(0, 0.5));
    score2->setPosition( ccp(size.width/4,size.height/8*5));
    score2->setColor(ccc3(0, 0, 0));
    this->addChild(score2);
    
    
    
    
    CCLabelTTF *score3 = CCLabelTTF ::create(" ", "Georgia-Bold", 25);
    score3->setString( CCString::createWithFormat("%d     x     100     =    %d" , e, f)->getCString());
    score3->setAnchorPoint( ccp(0, 0.5));
    score3->setPosition( ccp(size.width/4,size.height/8*3));
    score3->setColor(ccc3(0, 0, 0));
    this->addChild(score3);
    
    
    
    
    CCLabelTTF *score4 = CCLabelTTF ::create(" ", "Georgia-Bold", 30);
    score4->setString( CCString::createWithFormat("Totally                      =    %d" , g)->getCString());
    score4->setAnchorPoint( ccp(0, 1));
    score4->setPosition( ccp(size.width/10,size.height/4));
    score4->setColor(ccc3(0, 0, 0));
    this->addChild(score4); 
}

void GameWinLayer::menuNextCallback(CCObject* pSender)
{
    if (m_level < 5)
    {
//        CCLog("Next");
        CCScene* mainGame = CCScene::create();
        
        MainGame* GameLayer = MainGame::create(m_level+1  );
        mainGame->addChild(GameLayer);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, mainGame));
    } 
}

void GameWinLayer::menuRetryCallback(CCObject* pSender)
{
//    CCLog("Retry");
    CCScene* mainGame = CCScene::create();
    MainGame* GameLayer = MainGame::create(m_level);
    mainGame->addChild(GameLayer);
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, mainGame));
}

void GameWinLayer::menuBackCallback(CCObject* pSender)
{
    CCScene* pScene = GameMenu::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);
//    CCLog("back");
}
