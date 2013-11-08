//
//  MoveObject.h
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#ifndef My_Battle_City_MoveObject_h
#define My_Battle_City_MoveObject_h

#include "cocos2d.h"

class MoveObject:public cocos2d::CCSprite
{
	CC_SYNTHESIZE(float, m_speed, Speed);
	CC_SYNTHESIZE(int, m_dir, Direction);
public:
    
	MoveObject(void);
	~MoveObject(void);
    
	bool CheckInside(cocos2d::CCPoint pos);
    
	cocos2d::CCPoint getNextPos();
    
	int getNextDirection();
    
	void setDirction(int ang);
    
	cocos2d::CCPoint getCheckPos(int p);
    
protected:
	int angToDir(int ang);
};


#endif
