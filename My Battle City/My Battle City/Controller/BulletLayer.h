//
//  BulletLayer.h
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#ifndef My_Battle_City_BulletLayer_h
#define My_Battle_City_BulletLayer_h

#include "cocos2d.h"
class Tank;
class TankLayer;
class MapLayer;
class Bullet;

class BulletLayer: public cocos2d::CCLayer
{
public:
    BulletLayer(void);
	~BulletLayer(void);
    
    virtual bool init();
	CREATE_FUNC(BulletLayer);
    
	void setTank(TankLayer * tanks){ myTanks = tanks;}
	void setMap(MapLayer * map){ myMap = map; }

	
	void update(float dt);

	void addBullet(Tank * tank,cocos2d::CCPoint cp,int dir);
    
	Bullet * checkBulletConflict(cocos2d::CCPoint pos);
    
    void stopBulletsAction();
    
private:
	cocos2d::CCArray * bullets;
	MapLayer * myMap;
	TankLayer * myTanks;
};

#endif
