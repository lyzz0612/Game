//
//  GameWinLayer.h
//  My Battle City
//
//  Created by Diro.K on 13-7-18.
//
//

#ifndef My_Battle_City_GameWinLayer_h
#define My_Battle_City_GameWinLayer_h

#include "cocos2d.h"

class GameWinLayer : public cocos2d::CCLayerColor
{
public:
    
    GameWinLayer();
    virtual bool init(bool _isGameOver, int level);
    
    static GameWinLayer * create(bool _isGameOver, int level);
    
    void menuBackCallback(cocos2d::CCObject* pSender);
    
    void menuNextCallback(cocos2d::CCObject* pSender);
    void menuRetryCallback(cocos2d::CCObject* pSender);
    void menuLayout();
    
    void setResult();
    
    int a,b,c,d,e,f,g;
private:
    
    bool isGameOver;
    int m_level;
    
};

#endif
