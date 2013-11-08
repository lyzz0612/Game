//
//  GameMenu.h
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#ifndef My_Battle_City_GameMenu_h
#define My_Battle_City_GameMenu_h

#include "cocos2d.h"


class GameMenu : public cocos2d::CCLayer
{
    
    CC_SYNTHESIZE_READONLY(int, m_level, Levle);
public:

    GameMenu();
    virtual bool init();

    static cocos2d::CCScene* scene();

    void menuCloseCallback(cocos2d::CCObject* pSender);

    CREATE_FUNC(GameMenu);
    
    void menuStartCallback(cocos2d::CCObject* pSender);
    
    void selectNext(cocos2d::CCObject* pSender);
    void selectBack(cocos2d::CCObject* pSender);


};

#endif
