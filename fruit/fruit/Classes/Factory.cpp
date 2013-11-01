//
//  Factory.cpp
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#include "Factory.h"
FruitType Fruittype[] =
{
    BANANA,
    APPLE,
    BASAHA,
    PEACH,
    SANDIA,
    BOOM
};
ActionType Actiontype[] = {
    JUMP,
    MOVE
};

Factory* Factory::factory = NULL;
Factory::Factory()
{
    
}
Factory* Factory::sharedFactory()
{
    if(!factory)
    {
        factory = new Factory();
    }
    return factory;
}

Fruit* Factory::product()
{
    int cho,actcho;
    Fruit* fruit = NULL;
    do
    {
        cho = (int)(CCRANDOM_0_1() * 6);
        actcho = (int)(CCRANDOM_0_1() * 2);
        fruit= Fruit::create(Fruittype[cho], Actiontype[actcho]);
    }while (!fruit);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    cho = CCRANDOM_0_1() * (size.width - 40) + 10 ;
    fruit->setPosition(ccp(cho, -20));
    fruit->startAction();
    return  fruit;
}