//
//  Bullet.h
//  My Battle City
//
//  Created by Diro.K on 13-7-13.
//
//

#ifndef My_Battle_City_Bullet_h
#define My_Battle_City_Bullet_h

#include "cocos2d.h"
#include "Tank.h"
#include "MapLayer.h"
#include "MoveObject.h"

#define SPEED 4   //iPhone simulation
//#define SPEED 9  //ipad simulation

class MapLayer;
class Tank;

USING_NS_CC;

class Bullet : public MoveObject
{
public:
    
    virtual bool init();
	CREATE_FUNC(Bullet);
    
    void animationBang(CCPoint cp);
    
    int m_power;
    Tank * m_tank;
    
private:
    void deleteAnimationCallBack(CCNode * pSennder);
};

#endif
