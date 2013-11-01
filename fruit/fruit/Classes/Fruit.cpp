//
//  Fruit.cpp
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#include "Fruit.h"

//Fruit::~Fruit()
//{
//    CC_SAFE_RELEASE(act);
//}
Fruit* Fruit::create(FruitType ft, ActionType at)
{
    Fruit* ret = new Fruit();
    if(ret && ret->initWithType(ft,at))
    {
        ret->setTag(1);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}
Fruit* Fruit::create(const char *filename)
{
    Fruit* ret = new Fruit();
    if(ret && ret->initWithFile(filename))
    {
        ret->setTag(1);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}
bool Fruit::initWithType(FruitType ft, ActionType at)
{
    const char* filename;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    switch (ft) {
        case PEACH:
            filename = "peach.png";
            value = 1;
            break;
        case SANDIA:
            filename = "sandia.png";
            value = 1;
            break;
        case BANANA:
            filename = "banana.png";
            value = 1;
            break;
        case APPLE:
            filename = "apple.png";
            value = 1;
            break;
        case BASAHA:
            filename = "basaha.png";
            value = 1;
            break;
        case BOOM:
            filename = "boom.png";
            value = 0;
            break;
        default:
            CCAssert(false, "Missing FruitType");
            break;
    }
    if(!CCSprite::initWithFile(filename))
        return false;
    
    this->setType(ft);
    
    CCActionInterval* action;
    CCActionInterval* reverseAct;
    
    int randpos1 = CCRANDOM_0_1() * (size.width - 200) + 100;
    int randpos2 = CCRANDOM_0_1() * (size.width - 50);
    float dt = CCRANDOM_0_1();
    if(dt < 0.6)
        dt += 0.6;
    switch (at) {
        case JUMP:
            action = CCJumpTo::create(dt, ccp(randpos1,size.height - 100), size.height / 2 - 100, 0);
            reverseAct = CCJumpTo::create(dt,ccp(randpos2,-20),size.height / 2 - 100, 0);
            break;
        case MOVE:
            action = CCMoveTo::create(dt,ccp(randpos1,size.height - 100));
            reverseAct = CCMoveTo::create(dt,ccp(randpos2,-40));
            break;
        default:
            CCAssert(false, "Error ActionType");
            break;
    }
    CCActionInterval* rotate1 = CCRotateBy::create(dt, 270);
    CCActionInterval* rotate2 = CCRotateBy::create(dt, 270);
    CCSpawn* spawn1 = CCSpawn::createWithTwoActions(action,rotate1);
    CCSpawn* spawn2 = CCSpawn::createWithTwoActions(reverseAct,rotate2);
    CCSequence* myact = CCSequence::create(spawn1,spawn2,CCCallFunc::create(this, callfunc_selector(Fruit::delSelf)),NULL);
    act = myact;
    return true;
}
void Fruit::setType(FruitType ft)
{
    this->fruit = ft;
}
FruitType Fruit::getType()
{
    return this->fruit;
}
void Fruit::startAction()
{
    this->runAction(act);
}
int Fruit::getVal()
{
    return value;
}
void Fruit::delSelf()
{
    if(this->getType() == BOOM)
    {
        this->removeFromParent();
        return;
    }
    int tag = this->getParent()->getTag();
    this->getParent()->setTag(tag + 1);
    
    Fruit* errorShow = Fruit::create("x_filled-hd.png");
    CCPoint cur = this->getPosition();
    errorShow->setPosition(ccp(cur.x, errorShow->getContentSize().height / 2));
    CCActionInterval* scalebig = CCScaleTo::create(0.5, 1.4);
    CCActionInterval* scalenormal = CCScaleTo::create(0.5, 1.0);
    CCActionInterval* fadeout = CCFadeOut::create(0.2);
    CCSequence* errorAction = CCSequence::create(scalebig,scalenormal,fadeout,CCCallFunc::create(errorShow, callfunc_selector(Fruit::RemoveAction)),NULL);
    errorShow->runAction(errorAction);
    this->getParent()->addChild(errorShow, 9);
    
    this->removeFromParent();
}
void Fruit::RemoveAction()
{
    this->removeFromParent();
}
bool Fruit::isBoom()
{
    return (this->getType() == BOOM);
}
void Fruit::delAction(CCPoint cutPoint)
{
    if(this->getType() == BOOM)
        return;
    CCPoint cur = this->getPosition();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    const char* filename1;
    const char* filename2;
    
    switch (this->getType()) {
        case PEACH:
            filename1 = "peach-1.png";
            filename2 = "peach-2.png";
            break;
        case SANDIA:
            filename1 = "sandia-1.png";
            filename2 = "sandia-2.png";
            break;
        case APPLE:
            filename1 = "apple-1.png";
            filename2 = "apple-2.png";
            break;
        case BANANA:
            filename1 = "banana-1.png";
            filename2 = "banana-2.png";
            break;
        case BASAHA:
            filename1 = "basaha-1.png";
            filename2 = "basaha-2.png";
            break;
        default:
            CCAssert(false, "Error FruitType");
            break;
    }
    Fruit* half1 = Fruit::create(filename1);
    Fruit* half2 = Fruit::create(filename2);
    Fruit* flash = Fruit::create("flash.png");
    
    float tan = (cur.y - cutPoint.y) / (cur.x - cutPoint.x);
    float degree = atan(tan);
    degree = degree / 3.24159f * 180;
    if(degree >= 360)
        degree -= 360;
    
    half1->setPosition(ccp(cur.x - half1->getContentSize().width / 2, cur.y));
    half1->setRotation(degree);
    half2->setPosition(ccp(cur.x + half2->getContentSize().width / 2, cur.y));
    half2->setRotation(degree);
    flash->setPosition(this->getPosition());
    flash->setRotation(degree);
    
    float randpos = CCRANDOM_0_1() * 200;
    CCActionInterval* leftItem = CCMoveTo::create(0.8, ccp(randpos, -50));
    CCSequence* left = CCSequence::create(leftItem,CCCallFunc::create(half1, callfunc_selector(Fruit::RemoveAction)),NULL);
    CCActionInterval* rightItem = CCMoveTo::create(0.8, ccp(size.width - randpos, -50));
    CCSequence* right = CCSequence::create(rightItem,CCCallFunc::create(half2, callfunc_selector(Fruit::RemoveAction)),NULL);
    CCActionInterval* flashItem = CCFadeIn::create(0.1);
    CCSequence* flashSeq = CCSequence::create(flashItem,CCCallFunc::create(flash, callfunc_selector(Fruit::RemoveAction)),NULL);
    
    half1->runAction(left);
    half2->runAction(right);
    flash->runAction(flashSeq);
    this->getParent()->addChild(half1,9);
    this->getParent()->addChild(half2,9);
    this->getParent()->addChild(flash,9);
}



