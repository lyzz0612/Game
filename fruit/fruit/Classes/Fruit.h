//
//  Fruit.h
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#ifndef __fruit__Fruit__
#define __fruit__Fruit__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
enum FruitType{
    BASAHA,
    BANANA,
    APPLE,
    PEACH,
    SANDIA,
    BOOM
};
enum ActionType{
    JUMP,
    MOVE
};
class Fruit : public CCSprite
{
public:
//    ~Fruit();
    static Fruit* create(FruitType, ActionType);
    static Fruit* create(const char* filename);
    virtual bool initWithType(FruitType, ActionType);
    void startAction();
    bool isBoom();
    int getVal();
    void delSelf();
    void RemoveAction();
    void delAction(CCPoint);
protected:
    CCActionInterval* act;
    FruitType fruit;
    int value;
    FruitType getType();
    void setType(FruitType);
};
#endif /* defined(__fruit__Fruit__) */
