//
//  MainGame.h
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#ifndef My_Battle_City_MainGame_h
#define My_Battle_City_MainGame_h

#include "cocos2d.h"

class GameMenu;
class MapLayer;
class RockerController;
class Tank;
class TankLayer;
class BulletLayer;


class MainGame : public cocos2d::CCLayer
{
public:
    
    MainGame();
    ~MainGame();
    
    virtual bool init(int level);
    static MainGame * create(int level);
    
    void menuCallback(cocos2d::CCObject* pSender);
    
    void menuPause(cocos2d::CCObject* pSender);
    
    void menuResume(cocos2d::CCObject* pSender);
    
    MapLayer* getMayLayer(){ return m_Maplayer; }
    
    RockerController* getController(){ return m_controller; }
    
    void update(float dt);

    void GameOver();
    void GameWin();
    void getResultLayer(bool isGamover,int level);
    
protected:
    void setMenu(cocos2d::CCSize size);
    void setTable(cocos2d::CCSize size);
    
    cocos2d::CCMenuItemImage *pPauseItem;
    cocos2d::CCMenuItemImage *pResumeItem;
    
    MapLayer* m_Maplayer;
    RockerController* m_controller;
    BulletLayer * m_bulletlayer;
    TankLayer * m_tanklayer;
    Tank* tanklabel;
    
    int enNum[6];
    int NowEnNum;
    int myLife;
    int m_level;
    int socre;
};

#endif
