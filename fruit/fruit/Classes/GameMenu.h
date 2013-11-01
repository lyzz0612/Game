//
//  GameMenu.h
//  fruit
//  菜单场景，选择开始游戏或者结束游戏
//  Created by yun ling on 13-7-8.
//
//

#ifndef __fruit__GameMenu__
#define __fruit__GameMenu__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Game.h"
USING_NS_CC;

class GameMenu : public CCLayer
{
public:
    static CCScene* scene();
    static GameMenu* create();
//    ~GameMenu();
    virtual bool init();
    virtual void registerWithTouchDispatcher(void);                 //注册触摸事件
    virtual bool ccTouchBegan(CCTouch *ptouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *ptouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *ptouch, CCEvent *pEvent);
protected:
    CCSprite* start;                //开始按钮
    CCSprite* quit;                 //结束按钮
    CCParticleSun* par;             //刀痕粒子效果
    bool isOnStart(CCPoint);        //判断该点是否在开始按钮上
    bool isOnQuit(CCPoint);         //判断该点是否在结束按钮上
    void StartFun();                //开始游戏
    void QuitFun();                 //结束游戏
};
#endif /* defined(__fruit__GameMenu__) */
