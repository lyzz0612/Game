//
//  RockerController.h
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#ifndef My_Battle_City_RockerController_h
#define My_Battle_City_RockerController_h

#include "cocos2d.h"
#include "TankLayer.h"
USING_NS_CC;

class RockerController :public CCLayer
{
	
public:
    
    RockerController();
    
    CREATE_FUNC(RockerController);
    
    static RockerController * create(const char * bgRocker, const char * thumb, CCPoint Rockerposition, const char * OnfireButton, const char * ReleasefireButton, CCPoint Buttonposition);
    
    void EnableControl();
    
    void DisableControl();
    
    
    bool isInside(CCPoint curPos, CCSprite* parent);
    
    float distance(CCPoint point1, CCPoint point2);
    
    float GetDirection(CCPoint point1, CCPoint point2);
    
	void setTankLayer(TankLayer *tankLayer);
    
protected:
    
    void initRockerController(const char * bgRocker, const char * thumb, CCPoint Rockerposition , const char * OnfireButton, const char * ReleasefireButton, CCPoint Buttonposition);

    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    TankLayer * myTankLayer;
    
private:
    float m_radius;
    CCPoint m_RockerPosition;
    CCPoint m_ButtonPosition;
    bool isActive;

    float m_angle;
    
    bool isFire;
    
};


#endif
