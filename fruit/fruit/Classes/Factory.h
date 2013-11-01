//
//  Factory.h
//  fruit
//
//  Created by yun ling on 13-7-5.
//
//

#ifndef __fruit__Factory__
#define __fruit__Factory__
#include "cocos2d.h"
#include "Fruit.h"
USING_NS_CC;

class Factory
{
public:
    Factory();
    static Factory* sharedFactory();
    Fruit* product();
protected:
    static Factory* factory;
};

#endif /* defined(__fruit__Factory__) */
