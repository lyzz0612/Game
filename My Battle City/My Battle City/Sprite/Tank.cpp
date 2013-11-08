//
//  Tank.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#include "Tank.h"
#include "MapLayer.h"
#include "BulletLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

Tank::Tank()
:m_life(1)
, FireCount(1)
, m_score(100)
{   }

Tank::~Tank()
{
    
}

Tank* Tank::create(int type)
{
    Tank* pRet = new Tank();
    if (pRet && pRet->initWithType(type)) {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool Tank::initWithType(int Ttype)
{
    const char * filename;
    m_type = Ttype;
    
    switch (m_type) {
        case 0:
            filename = "p1-a-cell.png";
            break;
        case 1:
            filename = "en1.png";
            break;
        case 2:
            filename = "en5.png";
            break;
        case 3:
            filename = "en6.png";
            break;
            
        default:
            break;
    }
    if (!CCSprite::initWithSpriteFrameName(filename)) {
        return false;
    }
    SetTank(m_type);
    
	FireCount = 0;
    
    this->schedule(schedule_selector(Tank::refreshFireCount), 1.0f);
    return true;
}

void Tank::SetTank(int type)
{
    switch (type)
    {
        case 0:
            this->setScore(0);
            this->setSpeed(USER_SPEED);
            break;
            
        case 1:
            this->setColor(ccc3(202 ,225, 255));
            this->setScore(100);
            this->setLife(1);
            this->setSpeed(FAST_SPEED);
            break;
            
        case 2:
            this->setColor(ccc3(34,139,34));
            this->setScore(200);
            this->setLife(2);
            this->setSpeed(NORMAL_SPEED);
            break;
        case 3:
            this->setColor(ccc3(238,44,44));
            this->setScore(300);
            this->setLife(3);
            this->setSpeed(SLOW_SPEED);
            break;
            
        default:
            break;
    }
}


void Tank::refreshFireCount(float dt)
{
    FireCount -= dt;
}

void Tank::setFireCount(float count)
{
    FireCount = count;
}

bool Tank::canFire(){
	if(FireCount<=0)return true;
	return false;
}


void Tank::fire(BulletLayer * bullets,int coldTime)
{
    SimpleAudioEngine::sharedEngine()->playEffect("bullet.aif");
	bullets->addBullet(this,this->getCheckPos(0),this->getRotation());
	this->setFireCount(coldTime);
}


void Tank::animationBorn()
{
    CCSpriteFrame * f1;
    CCAnimation * anim = CCAnimation::create();
	
    for (int i=1; i<=4; i++)
    {
        const char * name = CCString::createWithFormat("xing%d.png",i)->getCString();
        f1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        anim->addSpriteFrame(f1);
    }
    
    anim->setDelayPerUnit(0.2f);
    anim->setRestoreOriginalFrame(true);
    CCAnimate * animate = CCAnimate::create(anim);
    this->runAction(animate);
}



