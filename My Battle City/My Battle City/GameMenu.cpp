//
//  GameMenu.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#include "GameMenu.h"
#include "MainGame.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

GameMenu::GameMenu()
{
    m_level = 1;
}
CCScene* GameMenu::scene()
{
    CCScene *MenuScene = CCScene::create();
    
    GameMenu *MenuLayer = GameMenu::create();

    MenuScene->addChild(MenuLayer);

    return MenuScene;
}

bool GameMenu::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemFont *pCloseItem = CCMenuItemFont::create("Quit Game",
                                                        this,
                                                        menu_selector(GameMenu::menuCloseCallback) );
    pCloseItem->setPosition( ccp(size.width/2, size.height/8) );
    pCloseItem->setFontSizeObj(25);
    pCloseItem->setFontNameObj("Georgia-Bold");

    CCMenuItemFont* pStartItem = CCMenuItemFont::create("Start Game",
                                                        this,
                                                        menu_selector(GameMenu::menuStartCallback) );
    pStartItem->setPosition(ccp(size.width/2,size.height/4));
    pStartItem->setFontSizeObj(40);
    pStartItem->setFontNameObj("Georgia-Bold");
    
    
    
    CCMenuItemFont* pNextItem = CCMenuItemFont::create(">",
                                                        this,
                                                        menu_selector(GameMenu::selectNext) );
    pNextItem->setPosition(ccp(size.width/3*2,size.height/5*2));
    pNextItem->setFontSizeObj(80);
    pNextItem->setFontNameObj("Georgia-Bold");
    
    CCMenuItemFont* pBackItem = CCMenuItemFont::create("<",
                                                       this,
                                                       menu_selector(GameMenu::selectBack) );
    pBackItem->setPosition(ccp(size.width/3,size.height/5*2));
    pBackItem->setFontSizeObj(80);
    pBackItem->setFontNameObj("Georgia-Bold");
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pStartItem, pNextItem, pBackItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    
    CCSprite* pGameName = CCSprite::create("BattleCity.png");
    pGameName->setScale(2.0f);
    pGameName->setPosition(ccp(size.width/2,size.height/3*2));
    this->addChild(pGameName,1);
    
    CCLabelTTF *LevelLabel = CCLabelTTF::create("Level 1", "Georgia-Bold", 55);
    LevelLabel->setString(CCString::createWithFormat("Level %d", m_level)->getCString());
    LevelLabel->setPosition(ccp(size.width/2,size.height/5*2));
    this->addChild(LevelLabel,2,888);
    
    return true;
}


void GameMenu::selectNext(cocos2d::CCObject* pSender)
{
    CCLabelTTF* LevelLabel=(CCLabelTTF*)this->getChildByTag(888);
//    CCLog("Next");
    if (m_level <5 ) {
        m_level++;
//        m_enNum+=3;
        LevelLabel->setString(CCString::createWithFormat("Level %d", m_level)->getCString());
        SimpleAudioEngine::sharedEngine()->playEffect("move.aif");
    }
//    CCLog("DATA : %d // %d",m_level);
}

void GameMenu::selectBack(cocos2d::CCObject* pSender)
{
    CCLabelTTF* LevelLabel=(CCLabelTTF*)this->getChildByTag(888);
//    CCLog("Back");
    if (m_level >1 ) {
        m_level--;
//        m_enNum-=3;
        LevelLabel->setString(CCString::createWithFormat("Level %d", m_level)->getCString());
        SimpleAudioEngine::sharedEngine()->playEffect("move.aif");
    }
//    CCLog("DATA : %d // %d",m_level);
}



void GameMenu::menuStartCallback(CCObject* pSender)
{
    CCLog("Start Game");
//    CCScene* mainGame = MainGame::scene();
    
    CCScene* mainGame = CCScene::create();
    MainGame* GameLayer = MainGame::create(m_level);
    mainGame->addChild(GameLayer);
    
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, mainGame));
//    SimpleAudioEngine::sharedEngine()->playEffect("02 start.aif");
  
}

void GameMenu::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}