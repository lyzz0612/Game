//
//  MapLayer.h
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#ifndef My_Battle_City_MapLayer_h
#define My_Battle_City_MapLayer_h

#include "cocos2d.h"

class Tank;
class Bullet;


#define OUT_OF_MAP -100
#define EMPTY_TILE -2

class MapLayer :public cocos2d::CCLayerColor
{

    CC_SYNTHESIZE_READONLY(cocos2d::CCTMXLayer*, m_MapLayer1, TMXLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCTMXTiledMap*, gameMap, GameMap);
    
public:
    MapLayer();
    ~MapLayer();
    
    virtual bool init(); 
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MapLayer);
    void initWithMap(int level);
    
    
	bool checkCanMove(cocos2d::CCPoint cp);
	bool HitAndDelete(cocos2d::CCPoint cp);
    bool IsGrass(cocos2d::CCPoint cp);
    bool IsHome(cocos2d::CCPoint cp);
    
	cocos2d::CCPoint getMyTankPos();
	cocos2d::CCPoint getEnemyPosByRand();
    cocos2d::CCPoint getHomePos();
    
	float getMapScale(){return scale;}
    
protected:
    
	float scale;
	cocos2d::CCSize titleSize;
	cocos2d::CCPoint FixPosition(cocos2d::CCPoint cp,cocos2d::CCSize size);
	cocos2d::CCPoint getObjectPosition(cocos2d::CCTMXObjectGroup* group, const char* object);
	cocos2d::CCPoint Posititon_To_tileCoordinate(cocos2d::CCPoint position);
	int Get_TiledValue_By_Key_Position(cocos2d::CCPoint position,const char * key = "value");
    
	cocos2d::CCTMXObjectGroup * objects;
    cocos2d::CCTMXLayer* m_MapLayer2;
    
	cocos2d::CCPoint tankPos;
	cocos2d::CCPoint enemyPos[5];
    cocos2d::CCPoint homePos;
};

#endif
