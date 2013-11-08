//
//  Tank.h
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#ifndef My_Battle_City_Tank_h
#define My_Battle_City_Tank_h

#include "MoveObject.h"
#include "cocos2d.h"
//iPhone Simulation
#define SLOW_SPEED 0.5
#define NORMAL_SPEED 0.6
#define USER_SPEED 0.7
#define FAST_SPEED 0.8

////iPad Simulation
//#define SLOW_SPEED 1.0
//#define NORMAL_SPEED 1.3
//#define USER_SPEED 1.6
//#define FAST_SPEED 2.0

class MapLayer;
class BulletLayer;

class Tank : public MoveObject
{
    CC_SYNTHESIZE(int, m_life, Life);
    CC_SYNTHESIZE(int, m_score, Score);
    CC_SYNTHESIZE(int, m_type, TankType);
    
public:
    Tank();
    ~Tank();
    
    virtual bool initWithType(int type);
    static Tank * create(int type);
    
    void animationBorn();
    
    void setFireCount(float count);
    bool canFire();
    void refreshFireCount(float dt);
    
	void fire(BulletLayer * bullets,int coldTime);

protected:
    
    int FireCount;  
    void SetTank(int type);
    
};

#endif
