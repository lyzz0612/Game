//
//  TankLayer.h
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#ifndef My_Battle_City_TankLayer_h
#define My_Battle_City_TankLayer_h

#include "cocos2d.h"

class Tank;
class MapLayer;
class BulletLayer;

class TankLayer:public cocos2d::CCLayer
{

    CC_SYNTHESIZE_READONLY(int, m_score, Score);
    CC_SYNTHESIZE_READONLY(int, counter1, Count1);
    CC_SYNTHESIZE_READONLY(int, counter2, Count2);
    CC_SYNTHESIZE_READONLY(int, counter3, Count3);
    
    
public:
    TankLayer();
	~TankLayer();
    
    virtual bool init();
    CREATE_FUNC(TankLayer);
    
	bool startMove ;
	void startGame(int enemyCount);
    
	void setMap(MapLayer * map);
	void setBullets(BulletLayer * bullets);
    void setTankAttribute(int speed,int ang);
    
    
	void add(Tank * pTank);
	void remove(Tank * pTank);
    

    Tank * checkTankConflict(cocos2d::CCPoint pos);
    
	bool moveOneStep(Tank * tank);
    void searchHome(Tank* tank);
    void changeDir(float dt);
    
	bool myTankFire();
    
    int getEnemyCount();
	Tank * getMyTank();
    
    void stopTanksAction();
	void update(float dt);
	
    void setScore(int score);
    void setCount(int type);
    int  getResult(int type);
    
private:
	cocos2d::CCArray * tanks;
	Tank * myTank;
	MapLayer * myMap;
	BulletLayer * myBullets;
    int m_EnemyCount;
    
    
    void createMyTank();
	void createEnemyByRand();
    bool refreshEnemy();

    int  setEnemyTypebyRand();

};



#endif
