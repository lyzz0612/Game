//
//  TankLayer.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#include "TankLayer.h"

#include "Tank.h"
#include "BulletLayer.h"
#include "cocos2d.h"
#include "MapLayer.h"

using namespace cocos2d;

TankLayer::TankLayer()
{
    m_score  = 0;
    counter1 = 0;
    counter2 = 0;
    counter3 = 0;
}


TankLayer::~TankLayer()
{
    CC_SAFE_RELEASE(tanks);
    
}

bool TankLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
	tanks = CCArray::create();
	tanks->retain();

	startMove = false;

	this->schedule(schedule_selector(TankLayer::update));
    
    //random select a enemy to changeDir
    this->schedule(schedule_selector(TankLayer::changeDir), 0.8f);
    
    return true;
}

void TankLayer::startGame(int enemyCount)
{
	m_EnemyCount = enemyCount;
    this->setScale(myMap->getMapScale());
    
	myBullets->setScale((myMap->getMapScale()));
	createMyTank();
    
	for(int i = 0;i<3;i++)
    {
        createEnemyByRand();
        m_EnemyCount--;
    }
    
	startMove = true;
}


void TankLayer::add(Tank * pTank)
{
    //userTank
	if(pTank->getTankType() == 0)
    {
		myTank = pTank;
	}
	tanks->addObject((CCObject*)pTank);
	this->addChild(pTank);
}

void TankLayer::remove(Tank * pTank)
{
	tanks->removeObject((CCObject * )pTank);
	this->removeChild(pTank,true);
}

Tank * TankLayer::checkTankConflict(cocos2d::CCPoint pos)
{
	CCObject * checkTank;
	CCARRAY_FOREACH(tanks,checkTank)
    {
        Tank* t = dynamic_cast<Tank*>(checkTank);
		if(t->CheckInside(pos))
        {
			return t;
		}
	}
	return NULL;
}

void TankLayer::setTankAttribute(int speed,int ang)
{
	if(speed>=0)
        myTank->setSpeed(speed);
	else
        myTank->setSpeed(USER_SPEED);
	if(ang>=0)
        myTank->setDirction(ang);
}


void TankLayer::update(float dt)
{
	if(startMove == false)
        return ;
    if (refreshEnemy()) {
        createEnemyByRand();
        m_EnemyCount--;
    }
    
	CCObject * tank;

    CCARRAY_FOREACH(tanks,tank){

        
		Tank * t = dynamic_cast<Tank * >(tank);

		moveOneStep(t);
		
		if(t->canFire() && t->getTankType() != 0)
        {
			t->fire(myBullets,200);
		}
		else{
			t->refreshFireCount(dt);
		}
	}
	
}

bool TankLayer::myTankFire()
{
	if(myTank->canFire())
    {
		myTank->fire(myBullets,20);
		return true;
	}
	else
        return false;
}


bool  TankLayer::moveOneStep(Tank * tank)
{
    CCPoint pos =tank->getNextPos();
    bool conflict = false;
    
    CCPoint checkPosLeft = tank->getCheckPos(1);
    CCPoint checkPosRight  = tank->getCheckPos(-1);
    
    
    //with Tank
    if(checkTankConflict(checkPosLeft)
       || checkTankConflict(checkPosRight) )
        conflict = true;
    
    //with Map
    if(!conflict)
        if(!myMap->checkCanMove(checkPosLeft)
           || !myMap->checkCanMove(checkPosRight))
            conflict = true;
    
    
    if(!conflict)
    {
        if (myMap->IsGrass(tank->getPosition()))
        {
            tank->setVisible(false);
        }
        else{
            tank->setVisible(true);
        }
        CCActionInterval* moveto = CCMoveTo::create(0.1f, pos);
        tank->runAction(moveto);
        
        return true;
    }
    else{
        if(tank->getTankType())
            searchHome(tank);
        return false;
    }
}

void TankLayer::searchHome(Tank* tank)
{
    int ran = CCRANDOM_0_1()*100;
    int dir = tank->getNextDirection();
    CCPoint homePos = myMap->getHomePos();
    CCPoint curPos = tank->getPosition();
    
    int sign = curPos.x>homePos .x? -1:1;
    
    if (ran>65){
        dir = 2 - sign;
    }else if (ran >50){
        dir = 0;
    }else if (ran >15){
        dir = 2;
    }else {
        dir = 2 + sign;
    }
    
    tank->setDirction(dir*90);

}

void TankLayer::setMap(MapLayer * map)
{
	myMap = map;
}


void TankLayer::setBullets(BulletLayer * bullets)
{
	myBullets = bullets;
}



void TankLayer::createMyTank()
{
	Tank * tank = Tank::create(0);
	tank->retain();
//    CC_SAFE_RETAIN(tank);
	CCPoint cp = myMap->getMyTankPos();
    tank->setLife(3);
	tank->setPosition(cp);
	tank->setSpeed(0);
    tank->animationBorn();
	add(tank);
}

void TankLayer::createEnemyByRand()
{
    
	Tank * tank = Tank::create(setEnemyTypebyRand());
	tank->retain();
    
	CCPoint cp = myMap->getEnemyPosByRand();
	tank->setPosition(cp);
	tank->setDirction(CCRANDOM_0_1()*360);
    tank->animationBorn();
	add(tank);
}

bool TankLayer::refreshEnemy()
{
    if (tanks->count() != 4
        && m_EnemyCount>0)
    {
        return true;
    }
    return false;
}


int TankLayer::getEnemyCount()
{
    return tanks->count()-1 + m_EnemyCount;
}

Tank * TankLayer::getMyTank()
{
    return myTank;
}

void TankLayer::stopTanksAction()
{
//    this->unschedule(schedule_selector(TankLayer::update));
    this->unscheduleAllSelectors();
}

void TankLayer::setScore(int score)
{
    m_score += score;
}

int  TankLayer::setEnemyTypebyRand()
{
    int ran = CCRANDOM_0_1()*100;
    int ret =1;
    
    if (ran >= 80) {
        ret = 3;
    }
    else if (ran >=50)
    {
        ret = 2;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

void TankLayer::changeDir(float dt)
{
    Tank* t = dynamic_cast<Tank*>(tanks->randomObject());
    if (t->getTankType()) {
        searchHome(t);
    }
}

void TankLayer::setCount(int type)
{
    switch (type) {
        case 1:
            counter1++;
            break;
        case 2:
            counter2++;
            break;
        case 3:
            counter3++;
            break;
            
        default:
            break;
    }
}


int TankLayer ::getResult(int type)
{
    int result = 0;
    switch ( type) {
        case 1:
            result = counter1 * 100;
            break;
        case 2:
            result = counter2 * 200;
            break;
        case 3:
            result = counter3 * 300;
            break;
        case 4:
            result = counter1*100 + counter2*200 + counter3*300;
            break;
        default:
            break;
    }
    return result;
}
