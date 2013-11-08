//
//  MoveObject.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-15.
//
//

#include "MoveObject.h"

using namespace cocos2d;

MoveObject::MoveObject(void)
{
}


MoveObject::~MoveObject(void)
{
}


bool MoveObject::CheckInside(CCPoint pos)
{
	CCRect rect = this->boundingBox();
	return rect.containsPoint(pos);
}


CCPoint MoveObject::getNextPos()
{
	CCPoint nPos = this->getPosition();
	int dir = getNextDirection();
	CCPoint retPos = nPos;
    
	if(dir == 1 || dir == 3)
		retPos.x =nPos.x + (2-dir)*m_speed;
	else//0 || 2
		retPos.y =nPos.y + (1-dir)*m_speed;
	return retPos;
}

int MoveObject::getNextDirection()
{
	return angToDir(this->getRotation());
}


void MoveObject::setDirction(int ang)
{
	this->setRotation(angToDir(ang)*90);
}

//dir :0\1\2\3
int MoveObject::angToDir(int ang)
{
	return ((ang+45)/90+4)%4;
}



//dt : 0 -Front, 1 -leftFront, -1 -RightFront
CCPoint MoveObject::getCheckPos(int dt)
{
	CCPoint nPos = this->getPosition();
	int dir = getNextDirection();
	CCPoint retPos = nPos;
	CCSize size = this->getContentSize();
    
	if(dir == 1 || dir == 3)
    {
		retPos.x =nPos.x + (2-dir)*(m_speed+size.width/2);
		retPos.y =nPos.y + dt*(2-dir)*(size.height/2-5);
	}
	else// 0 || 2
    {
		retPos.y =nPos.y + (1-dir)*(m_speed+size.height/2);
		retPos.x =nPos.x + dt*(dir-1)*(size.width/2-5);
	}
	return retPos;
}
