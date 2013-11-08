//
//  BulletLayer.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#include "BulletLayer.h"
#include "MapLayer.h"
#include "Tank.h"
#include "TankLayer.h"
#include "Bullet.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


BulletLayer::BulletLayer(void)
{
}


BulletLayer::~BulletLayer(void)
{
	CC_SAFE_RELEASE(bullets);
}

bool BulletLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    bullets = CCArray::create();
	bullets->retain();

	this->schedule(schedule_selector(BulletLayer::update));
    
    return true;
}


void BulletLayer::update(float dt)
{
	CCObject* bullet;
	CCARRAY_FOREACH(bullets,bullet)
    {
        Bullet* _bullet = dynamic_cast<Bullet*>(bullet);
		CCPoint nPos = _bullet->getNextPos();
        CCActionInterval* moveto = CCMoveTo::create(0.1f, nPos);
		_bullet->runAction(moveto);
	}
    
    
	CCArray * clearBullets = CCArray::create();
    
	CCARRAY_FOREACH(bullets,bullet)
    {
        Bullet* b = dynamic_cast<Bullet*>(bullet);
        
		//With other Bullet
		Bullet * conflictBullet = checkBulletConflict(b->getPosition());
		if(conflictBullet &&
           conflictBullet!= b)
        {
			clearBullets->addObject(conflictBullet);
			clearBullets->addObject(b);
			continue;
		}
        
		//With Tank
		Tank * conflictTank =  myTanks->checkTankConflict(b->getPosition());
		int cid = b->m_tank->getTankType();
        
        //from user shoot
        if (conflictTank && cid==0)
        {
            if (conflictTank->getTankType() != 0)
            {
                SimpleAudioEngine::sharedEngine()->playEffect("tankbomb.aif");
                
                b->animationBang(conflictTank->getPosition());
                
                
                int curLife = conflictTank->getLife();
                conflictTank->setLife(curLife-1);
                
                
                if (conflictTank->getLife() == 0)
                {
                    myTanks->setCount(conflictTank->getTankType());
                    myTanks->setScore(conflictTank->getScore());
                    myTanks->remove(conflictTank);
                    conflictTank->removeFromParentAndCleanup(true);
                }
                else if (conflictTank->getLife() == 1)
                {
                    conflictTank->setColor(ccc3(255,165,0));
                }
                else if (conflictTank->getLife() == 2)
                {
                    conflictTank->setColor(ccc3(34,139,34));
                }

                
                clearBullets->addObject(b);
                continue;
            }
        }
        
        //from enemy shoot
        if (conflictTank && cid!=0)
        {
            if (conflictTank->getTankType() == 0)
            {
                SimpleAudioEngine::sharedEngine()->playEffect("tankbomb.aif");
                //bong
                b->animationBang(conflictTank->getPosition());
                
                //cut user life
                int preLife = conflictTank->getLife();
                conflictTank->setLife(preLife-1);
                conflictTank->setPosition(myMap->getMyTankPos());
                conflictTank->setDirction(0);
                clearBullets->addObject(b);

                //animation
                conflictTank->animationBorn();
                
                continue;
            }
        }
        
		//With map
		if(myMap->HitAndDelete(b->getPosition()))
        {
            b->animationBang(b->getPosition());
            SimpleAudioEngine::sharedEngine()->playEffect("blast.aif");
			clearBullets->addObject(b);
            continue;
		}
        
        //With Grass
        if (myMap->IsGrass(b->getPosition()))
        {
            b->setVisible(false);
        }
        else{
            b->setVisible(true);
        }
        
        //With Home
        if (myMap->IsHome(b->getPosition()))
        {
            SimpleAudioEngine::sharedEngine()->playEffect("blast.aif");
            b->animationBang(b->getPosition());
            myTanks->getMyTank()->setLife(0);
            clearBullets->addObject(b);
            continue;
        }
        
	}
    
	CCARRAY_FOREACH(clearBullets,bullet)
    {
        Bullet* b = dynamic_cast<Bullet*>(bullet);
		b->removeFromParentAndCleanup(true);
		bullets->removeObject(b);
		b->m_tank->refreshFireCount(999999);

	}
}

// add bullet to the layer
void BulletLayer::addBullet(Tank * tank,CCPoint cp,int dir)
{
	Bullet * bullet = Bullet::create();
	bullet->retain();
//    CC_SAFE_RETAIN(bullet);
	bullet->m_tank = tank;
	bullet->setSpeed(SPEED);
	bullet->setDirction(dir);
	bullet->setPosition(cp);
	bullets->addObject(bullet);
	this->addChild(bullet,10);
}



Bullet * BulletLayer::checkBulletConflict(cocos2d::CCPoint pos)
{
	CCObject * bullet;
	CCARRAY_FOREACH(bullets,bullet)
    {
        Bullet* b = dynamic_cast<Bullet*>(bullet);
		if(b->CheckInside(pos))
        {
			return b;
		}
	}
	return NULL;
}



void BulletLayer::stopBulletsAction()
{
    this->unschedule(schedule_selector(BulletLayer::update));
}
