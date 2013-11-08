//
//  RockerController.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#include "RockerController.h"

RockerController::RockerController()
{
    m_radius = 0.0;
    isActive = false;
    m_angle = -1.0;
    isFire = false;
}


RockerController* RockerController::create(const char *bgRocker, const char *thumb, cocos2d::CCPoint Rockerposition,
                                           const char* OnfireButton, const char* ReleasefireButton, CCPoint Buttonposition)
{
    RockerController* rocker = RockerController::create();
    if (rocker) {
        rocker->initRockerController(bgRocker, thumb, Rockerposition,OnfireButton,ReleasefireButton,Buttonposition);
        return rocker;
    }
    
    
    CC_SAFE_DELETE(rocker);
    return NULL;
}


void RockerController::initRockerController(const char *bgRocker, const char *thumb, cocos2d::CCPoint Rockerposition,
                                            const char* OnfireButton, const char* ReleasefireButton, CCPoint Buttonposition)
{
    
    m_RockerPosition = Rockerposition;
    
    CCSprite* bgRockerSprite = CCSprite::create(bgRocker);
    bgRockerSprite->setVisible(false);
    bgRockerSprite->setPosition(Rockerposition);
    addChild(bgRockerSprite, 20, 101);
    
    m_radius = bgRockerSprite->getContentSize().width/2;
    
    CCSprite* thumbSprite = CCSprite::create(thumb);
    thumbSprite->setVisible(false);
    thumbSprite->setPosition(Rockerposition);
    addChild(thumbSprite, 21, 102);
    
    
    m_ButtonPosition = Buttonposition;
    
    CCSprite* fireButton_press = CCSprite::create(OnfireButton);
    fireButton_press->setVisible(false);
    fireButton_press->setPosition(Buttonposition);
    addChild(fireButton_press, 20, 103);
    
    CCSprite* fireButton_defualt = CCSprite::create(ReleasefireButton);
    fireButton_defualt->setVisible(false);
    fireButton_defualt->setPosition(Buttonposition);
    addChild(fireButton_defualt, 21, 104);
    
    //    CCLog("defualt angle: %f", m_angle);
}



void RockerController::EnableControl()
{
    if (!isActive) {
        isActive = true;
        this->setTouchEnabled(true);
        
        CCSprite* rocker = dynamic_cast<CCSprite*>(getChildByTag(101));
        rocker->setVisible(true);
        rocker->setOpacity(100);
        
        CCSprite* thumb = dynamic_cast<CCSprite*>(getChildByTag(102));
        thumb->setVisible(true);
        thumb->setOpacity(180);
        
        CCSprite* button = dynamic_cast<CCSprite*>(getChildByTag(104));
        button->setVisible(true);
        button->setOpacity(180);
        
        
        
    }
}

void RockerController::DisableControl()
{
    if (isActive) {
        isActive = false;
        
        this->setTouchEnabled(false);
        
        CCSprite* rocker = dynamic_cast<CCSprite*>(getChildByTag(101));
        rocker->setVisible(false);
        
        
        CCSprite* thumb = dynamic_cast<CCSprite*>(getChildByTag(102));
        thumb->setVisible(false);
        
        CCSprite* button = dynamic_cast<CCSprite*>(getChildByTag(104));
        button->setVisible(false);
        
        isFire = false;
        
    }
}


//----------  Del with Touch  ----------------------

void RockerController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSprite* thumb = dynamic_cast<CCSprite*>(getChildByTag(102));
    CCSprite* button = dynamic_cast<CCSprite*>(getChildByTag(104));
    CCSprite* button_press = dynamic_cast<CCSprite*>(getChildByTag(103));
    
    if (pTouches->count() == 1)
    {
        CCTouch* pTouch = dynamic_cast<CCTouch*>(pTouches->anyObject());
        CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
        
//        CCLog(" (%f , %f)",curPoint.x,curPoint.y);
        
        if (isInside(curPoint, button))
        {
            button->setVisible(false);
            button_press->setVisible(true);
            isFire = true;
//            if (isFire) {CCLog("fire");}else{CCLog("Not fire");}
            if (isFire) myTankLayer->myTankFire();
        }
        
        if (distance(curPoint, m_RockerPosition) < m_radius)
        {
            thumb->setPosition(curPoint);
            m_angle = GetDirection(m_RockerPosition, curPoint);
            myTankLayer->setTankAttribute(-1,m_angle);
        }
    }
    else
    {
        //more than one touch point
        for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); iter++)
        {
            CCTouch* pTouch = dynamic_cast<CCTouch*>(*iter);
            CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
            
            if (isInside(curPoint, button))
            {
                button->setVisible(false);
                button_press->setVisible(true);
                isFire = true;
//                if (isFire) {CCLog("fire");}else{CCLog("Not fire");}
                if (isFire) myTankLayer->myTankFire();
            }
            
            if (distance(curPoint, m_RockerPosition) < m_radius)
            {
                thumb->setPosition(curPoint);
                m_angle = GetDirection(m_RockerPosition, curPoint);
                myTankLayer->setTankAttribute(-1,m_angle);
            }
        }
    }
}
void RockerController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSprite* thumb = dynamic_cast<CCSprite*>(getChildByTag(102));
    CCSprite* button = dynamic_cast<CCSprite*>(getChildByTag(104));
    
    if (pTouches->count() == 1)
    {
        CCTouch* pTouch = dynamic_cast<CCTouch*>(pTouches->anyObject());
        CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
        
//        CCLog(" (%f , %f)",curPoint.x,curPoint.y);
        
        if (isInside(curPoint, button))
        {
            isFire = true;
//            if (isFire) {CCLog("move fire");}else{CCLog("Not fire");}
            if (isFire) myTankLayer->myTankFire();
        }
        
        if (distance(curPoint, m_RockerPosition) < m_radius)
        {
            thumb->setPosition(curPoint);
            m_angle = GetDirection(m_RockerPosition, curPoint);
            myTankLayer->setTankAttribute(-1,m_angle);
        }
    }
    else
    {
        //more than one touch point
        for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); iter++)
        {
            CCTouch* pTouch = dynamic_cast<CCTouch*>(*iter);
            CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
            
            if (isInside(curPoint, button))
            {
                isFire = true;
//                if (isFire) {CCLog("fire");}else{CCLog("Not fire");}
                if (isFire) myTankLayer->myTankFire();
            }
            
            if (distance(curPoint, m_RockerPosition) < m_radius)
            {
                thumb->setPosition(curPoint);
                m_angle = GetDirection(m_RockerPosition, curPoint);
                myTankLayer->setTankAttribute(-1,m_angle);
            }
        }
    }

}
void RockerController::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCSprite* thumb = dynamic_cast<CCSprite*>(getChildByTag(102));
    CCSprite* button = dynamic_cast<CCSprite*>(getChildByTag(104));
    CCSprite* button_press = dynamic_cast<CCSprite*>(getChildByTag(103));
    
    if (pTouches->count() == 1)
    {
        CCTouch* pTouch = dynamic_cast<CCTouch*>(pTouches->anyObject());
        CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
        
//        CCLog(" (%f , %f)",curPoint.x,curPoint.y);
        
        if (isInside(curPoint, button))
        {
            isFire = false;
            button_press->setVisible(false);
            button->setVisible(true);
//            if (isFire) {CCLog("move fire");}else{CCLog("Not fire");}
            if (isFire) myTankLayer->myTankFire();
        }
        
        else
        {
            thumb->setPosition(m_RockerPosition);
            m_angle = -1;
            myTankLayer->setTankAttribute(0,-1);
//            if (isFire) {CCLog("fire");}else{CCLog("Not fire");}
        }
    }
    else
    {
        //more than one touch point
        for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); iter++)
        {
            CCTouch* pTouch = dynamic_cast<CCTouch*>(*iter);
            CCPoint curPoint = this->convertTouchToNodeSpace(pTouch);
            
            if (isInside(curPoint, button))
            {
                isFire = false;
                button_press->setVisible(false);
                button->setVisible(true);
//                if (isFire) {CCLog("move fire");}else{CCLog("Not fire");}
                if (isFire) myTankLayer->myTankFire();
            }
            
            else
            {
                thumb->setPosition(m_RockerPosition);
                m_angle = -1;
                myTankLayer->setTankAttribute(0,-1);
//                if (isFire) {CCLog("fire");}else{CCLog("Not fire");}
            }
        }
    }

}


//---------- * Del-with-Touch-End * -----------------




bool RockerController::isInside(CCPoint curPos, CCSprite *parent)
{
    CCRect rect = parent->boundingBox();
    return rect.containsPoint(curPos);
}

float RockerController::distance(CCPoint point1, CCPoint point2)
{
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return sqrt(dx*dx + dy*dy);
}

float RockerController::GetDirection(CCPoint point1, CCPoint point2)
{
    float x1 = point1.x;
    float x2 = point2.x;
    
    float y1 = point1.y;
    float y2 = point2.y;
    
    float tan = (y2-y1)/(x2-x1);
    
    float degree = atan(tan);
    degree = degree/3.14159f*180;
    
    if (x2 < x1) {
        degree +=180;
    }
    if (y2<y1 && x2>x1) {
        degree +=360;
    }
    degree = (270 - (int)degree+180)%360;
    return degree;
}

void RockerController::setTankLayer(TankLayer *tankLayer)
{
    myTankLayer = tankLayer;
}
