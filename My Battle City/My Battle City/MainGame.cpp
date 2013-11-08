//
//  MainGame.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#include "MainGame.h"
#include "TankLayer.h"
#include "BulletLayer.h"
#include "MapLayer.h"
#include "RockerController.h"
#include "GameMenu.h"
#include "Tank.h"
#include "SimpleAudioEngine.h"
#include "GameWinLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

MainGame::MainGame()
{
    myLife  = 3;
    m_level = 1;
}

MainGame::~MainGame()
{
    CC_SAFE_RELEASE(m_bulletlayer);
    CC_SAFE_RELEASE(m_tanklayer);
}


MainGame* MainGame::create(int level)
{
    MainGame* pRet = new MainGame();
    if (pRet && pRet->init(level))
    {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool MainGame::init(int level)
{
    if (!CCLayer::init()) {
        return false;
    }
    
    SimpleAudioEngine::sharedEngine()->playEffect("02 start.aif");
    
    int temp[6] = {0,10,13,16,19,22};
    for (int i=0; i<6; i++) {
        enNum[i] = temp[i];
    }
    
    
    m_level = level;
    NowEnNum = enNum[m_level];
    
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
	//why halt change,half not?
    CCLayerColor* background = CCLayerColor::create(ccc4(255, 235, 205, 255));
    this->addChild(background,1);
    
    this->setMenu(size);
    
    
    
    CCSpriteFrameCache *frameCache=CCSpriteFrameCache::sharedSpriteFrameCache();
    
    frameCache->addSpriteFramesWithFile("images.plist");
    
    this->setTable(size);
    
    
    
    //----------- MapLayer -----------
    m_Maplayer = MapLayer::create();
    //level
    m_Maplayer->initWithMap(m_level);
    
    m_Maplayer->setPosition(ccp(size.width/8,0));
    
    this->addChild(m_Maplayer,1,999);
    
    //----------- BulletLayer -----------
    
    m_bulletlayer = BulletLayer::create();
	m_bulletlayer->retain();
	m_bulletlayer->setAnchorPoint(ccp(0,0));
	m_bulletlayer->setPosition(ccp(size.width/8,0));
    
	this->addChild(m_bulletlayer,30);
    
	//----------- TankLayer -------------
	m_tanklayer = TankLayer::create();
	m_tanklayer->retain();
	m_tanklayer->setMap(m_Maplayer);
	m_tanklayer->setAnchorPoint(ccp(0,0));
	m_tanklayer->setPosition(ccp(size.width/8,0));
	this->addChild(m_tanklayer,20);
    
    
	
	m_bulletlayer->setMap(m_Maplayer);
	m_bulletlayer->setTank(m_tanklayer);
	m_tanklayer->setBullets(m_bulletlayer);
    
    //----------- controller ------------
    
    m_controller = RockerController::create("control_bg.png",
                                            "cen.png",
                                            ccp(100,100),
                                            "fire_button_press.png",
                                            "fire_button_default.png",
                                            ccp(size.width-100,100));
    
	m_controller ->EnableControl();
    this->addChild(m_controller,30);
    m_controller->setTankLayer(m_tanklayer);

    //enemyCount
    m_tanklayer->startGame(NowEnNum);
    
    this->schedule(schedule_selector(MainGame::update), 0.1f);
    
    return true;
}



//------------Menu CallBack  Functions-------------

void MainGame::menuPause(CCObject* pSender)
{
    
    CCLog("Game Pause");
    //    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CCDirector::sharedDirector()->pause();
    m_controller->DisableControl();
    pPauseItem->setVisible(false);
    pResumeItem->setVisible(true);
    
}

void MainGame::menuResume(CCObject* pSender)
{
    
    CCLog("Game Resum");
    //    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    CCDirector::sharedDirector()->resume();
    m_controller->EnableControl();
    
    pPauseItem->setVisible(true);
    pResumeItem->setVisible(false);
}

void MainGame::menuCallback(CCObject* pSender)
{
    CCScene* pGameMenu = GameMenu::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pGameMenu));
    
}

//-----------------------------------------------------


void MainGame::update(float dt)
{
    int EnemyCount = m_tanklayer->getEnemyCount();
    int Mylife = m_tanklayer->getMyTank()->getLife();
    int _Score = m_tanklayer->getScore();
    
    if (socre != _Score) {
        socre = _Score;
        CCLabelTTF* _Scorelabel = (CCLabelTTF*)this->getChildByTag(404);
        _Scorelabel->setString(CCString::createWithFormat("%5d", _Score)->getCString());
    }
    
    if (NowEnNum != EnemyCount)
    {
        NowEnNum = EnemyCount;
        CCLabelTTF* enemylabel = (CCLabelTTF*)this->getChildByTag(301);
        enemylabel->setString(CCString::createWithFormat(": %d" ,EnemyCount)->getCString());
    }
    if (myLife != Mylife)
    {
        myLife = Mylife;
        CCLabelTTF* mylifelabel = (CCLabelTTF*)this->getChildByTag(302);
        mylifelabel->setString(CCString::createWithFormat(": %d" ,Mylife)->getCString());
    }
    
    
    
    if (NowEnNum == 0) {
        GameWin();
    }
    if (myLife == 0) {
        GameOver();
    }
}

void MainGame::setMenu(cocos2d::CCSize size)
{
    //-----------  Menu  ----------------------
    //add Quit Menu
    CCMenuItemImage *pQuitItem = CCMenuItemImage::create("back.png", "back.png", this,menu_selector(MainGame::menuCallback) );
    pQuitItem->setScale(0.5);
    pQuitItem->setPosition(ccp(40, size.height-30));
    
    //add Pause Menu
    pPauseItem = CCMenuItemImage::create("pause.png", "resume.png", this,menu_selector(MainGame::menuPause) );
    pPauseItem->setScale(0.5);
    pPauseItem->setPosition(ccp(size.width-40, size.height-30));
    
    //add Resume Menu
    pResumeItem = CCMenuItemImage::create("resume.png", "pause.png", this,menu_selector(MainGame::menuResume) );
    pResumeItem->setScale(0.5);
    pResumeItem->setVisible(false);
    pResumeItem->setPosition(ccp(size.width-40, size.height-30));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pQuitItem, pPauseItem, pResumeItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 2);
    
    //----------- * Menu-End  * ----------------
}

void MainGame::setTable(cocos2d::CCSize size)
{
    //-----------Tank label------------------
    Tank * enemylabel = Tank::create(1);
    enemylabel->setPosition(ccp(size.width/32,size.height/5*4));
    this->addChild(enemylabel,2,201);
    
    
    CCLabelTTF *enemyCount = CCLabelTTF::create(" ", "Georgia-Bold", 25);
    enemyCount->setString(CCString::createWithFormat("%d", enNum[m_level])->getCString());
    enemyCount->setAnchorPoint(ccp(0, 0.5));
    enemyCount->setPosition(ccp(size.width/16,size.height/5*4));
    enemyCount->setColor(ccc3(0, 0, 0));
    this->addChild(enemyCount,2,301);
    
    Tank * mylabel = Tank::create(0);
    mylabel->setPosition(ccp(size.width/32,size.height/4*3));
    this->addChild(mylabel,2,202);
    
    CCLabelTTF *LifeCount = CCLabelTTF::create(" ", "Georgia-Bold", 25);
    LifeCount->setString(CCString::createWithFormat("%d", myLife)->getCString());
    LifeCount->setAnchorPoint(ccp(0, 0.5));
    LifeCount->setPosition(ccp(size.width/16,size.height/4*3));
    LifeCount->setColor(ccc3(0, 0, 0));
    this->addChild(LifeCount,2,302);
    
    
    
    //----------Flag and Score----------------
    CCSprite * flag = CCSprite::createWithSpriteFrameName("flag.png");
    flag->setPosition(ccp(size.width/32,size.height/2));
    this->addChild(flag,2,401);
    
    CCLabelTTF *_level = CCLabelTTF::create(" ", "Georgia-Bold", 25);
    _level->setString(CCString::createWithFormat("%d", m_level)->getCString());
    _level->setPosition(ccp(size.width/16,size.height/2));
    _level->setColor(ccc3(0, 0, 0));
    this->addChild(_level,2,402);
    
    CCLabelTTF *IPscore = CCLabelTTF::create(" IP", "Georgia-Bold", 25);
    IPscore->setPosition(ccp(size.width/32,size.height/3*2));
    IPscore->setColor(ccc3(0, 0, 0));
    this->addChild(IPscore,2,403);
    
    
    CCLabelTTF *score = CCLabelTTF::create("00000", "Georgia-Bold", 25);
    score->setAnchorPoint(ccp(0, 0.5));
    score->setPosition(ccp(size.width/32,size.height/8*5));
    score->setColor(ccc3(0, 0, 0));
    this->addChild(score,2,404);
}



void MainGame::GameOver()
{

    CCSize size = this->getContentSize();
    
    m_tanklayer->stopTanksAction();
    m_bulletlayer->stopBulletsAction();
    m_controller->DisableControl();
    SimpleAudioEngine::sharedEngine()->playEffect("gameover.aif");

    
    CCSprite* gameover = CCSprite::createWithSpriteFrameName("gamedone.png");
    gameover->setScale(10);
    gameover->setPosition(ccp(size.width/2,size.height));
    this->addChild(gameover,10);
    CCActionInterval* MoveDown = CCMoveTo::create(2.0f, ccp(size.width/2,size.height/4*3));
    gameover->runAction(MoveDown);
    CCLog("Game Over");
    this->unschedule(schedule_selector(MainGame::update));

    this->getResultLayer(true,m_level);
}

void MainGame::GameWin()
{
    m_tanklayer->stopTanksAction();
    m_bulletlayer->stopBulletsAction();
    m_controller->DisableControl();
    
    SimpleAudioEngine::sharedEngine()->playEffect("award.aif");
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* gamewin = CCLabelTTF::create("Congratulation!!", "Georgia-Bold", 50);
    gamewin->setColor(ccc3(255, 255, 0));
    gamewin->setPosition(ccp(size.width/2,size.height));
    this->addChild(gamewin,10);
    CCActionInterval* MoveDown = CCMoveTo::create(2.0f, ccp(size.width/2,size.height/4*3));
    gamewin->runAction(MoveDown);
    CCLog("Game Win");
    this->unschedule(schedule_selector(MainGame::update));
    
    this->getResultLayer(false,m_level);
}


void MainGame ::getResultLayer(bool isGamover,int level)
{
    CCSize size = this ->getContentSize();
    GameWinLayer* gg = GameWinLayer::create(isGamover,level);

    gg->a = m_tanklayer->getCount1();
    gg->b = m_tanklayer->getResult(1);
    gg->c = m_tanklayer->getCount2();
    gg->d = m_tanklayer->getResult(2);
    gg->e = m_tanklayer->getCount3();
    gg->f = m_tanklayer->getResult(3);
    gg->g = m_tanklayer->getResult(4);
    
    gg->setResult();
    gg->setPosition( ccp(size.width/6,size.height/8));
    this->addChild(gg,100);
}


