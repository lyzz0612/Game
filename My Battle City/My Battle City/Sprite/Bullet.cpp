//
//  Bullet.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-13.
//
//

#include "Bullet.h"


bool Bullet::init()
{
    if (!CCSprite::initWithSpriteFrameName("bullet.png")) {
        return false;
    }

    return true;
}

void Bullet::animationBang(CCPoint cp)
{
    CCSpriteFrame * f1;
    CCAnimation * anim = CCAnimation::create();
    for (int i=1; i<=3; i++)
    {
        const char * name = CCString::createWithFormat("explode-%d.png",i)->getCString();
        f1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        anim->addSpriteFrame(f1);
    }
    anim->setDelayPerUnit(0.1f);
    anim->setRestoreOriginalFrame(true);
    CCAnimate * animate = CCAnimate::create(anim);
    
    //Bang Sprite
	CCSprite * t_bang = CCSprite::create();
	t_bang->retain();
	t_bang->setPosition(cp);
	this->getParent()->addChild(t_bang);
    
    
	CCSequence * seq = CCSequence::createWithTwoActions(animate,
                                                        CCCallFuncN::create(t_bang, callfuncN_selector(Bullet::deleteAnimationCallBack)));
    
	t_bang->runAction(seq);
    
}

void Bullet::deleteAnimationCallBack(CCNode * pSennder){
	pSennder->removeFromParentAndCleanup(true);
}


