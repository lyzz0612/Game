//
//  MapLayer.cpp
//  My Battle City
//
//  Created by Diro.K on 13-7-11.
//
//

#include "MapLayer.h"
#include "Tank.h"
#include "Bullet.h"

USING_NS_CC;

MapLayer::MapLayer()
{
    
}

MapLayer::~MapLayer()
{
    CC_SAFE_RELEASE(objects);
    CC_SAFE_RELEASE(gameMap);
}

CCScene* MapLayer::scene()
{
    CCScene* scene = CCScene::create();
    MapLayer* layer = MapLayer::create();
    scene->addChild(layer);
    return scene;
}

bool MapLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)))
    {
        return false;
    }
    return true;
}

void MapLayer::initWithMap(int level)
{
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
	
    gameMap = CCTMXTiledMap::create(CCString::createWithFormat("map%d.tmx",level)->getCString());
	gameMap->retain();

    titleSize = gameMap->getTileSize();
    scale = size.height/gameMap->getContentSize().height;
    
    gameMap->setPosition(ccp(0,0));
    gameMap->setScale(scale);
    
    this->addChild(gameMap,0);
    m_MapLayer1 = gameMap->layerNamed("bg1");
    m_MapLayer2 = gameMap->layerNamed("bg2");
    m_MapLayer2->setVisible(false);
    
    
    objects = gameMap->objectGroupNamed("objects");
	objects->retain();

	tankPos = this->getObjectPosition(objects, "pl1");
	
    enemyPos[0] = this->getObjectPosition(objects, "en1");
    enemyPos[1] = this->getObjectPosition(objects, "en2");
    enemyPos[2] = this->getObjectPosition(objects, "en3");
    enemyPos[3] = this->getObjectPosition(objects, "en4");
    enemyPos[4] = this->getObjectPosition(objects, "en5");
    
    homePos = this->getObjectPosition(objects, "home");
    
}

CCPoint MapLayer::FixPosition(CCPoint cp,CCSize tilesize)
{
	CCPoint retPos;
	retPos.x = cp.x + tilesize.width/2;
	retPos.y = cp.y + tilesize.height/2;
	return retPos;
}


CCPoint MapLayer::getMyTankPos()
{
	return FixPosition(tankPos,titleSize);
}

CCPoint MapLayer::getEnemyPosByRand(){
	
    int randomPos = CCRANDOM_0_1()*5;
    return FixPosition(enemyPos[randomPos],titleSize);
}

CCPoint MapLayer::getHomePos()
{
    return FixPosition(homePos, titleSize);
}


CCPoint MapLayer::getObjectPosition(cocos2d::CCTMXObjectGroup *group, const char *object)
{
    CCPoint point;
    CCDictionary *dic = group->objectNamed(object);
    point.x = dic->valueForKey("x")->intValue();
    point.y = dic->valueForKey("y")->intValue();
    return point;
}



CCPoint MapLayer::Posititon_To_tileCoordinate(CCPoint positon)
{
    //瓦片坐标
    int TP_x, TP_y;
    CCSize mapLayerSize = m_MapLayer1->getLayerSize();
    
    CCSize tiledSize = gameMap->getTileSize();
    
    TP_x = (int)((positon.x+tiledSize.width) / tiledSize.width)-1;
    
    TP_y = mapLayerSize.height -( (int)((positon.y+tiledSize.height)/tiledSize.height)-1)-1;
    
    if (TP_x>=0 && TP_x<mapLayerSize.width
        && TP_y>=0 && TP_y<mapLayerSize.height)
    {
//        CCLog("In the map:(%d, %d)",TP_x,TP_y);
        return ccp(TP_x,TP_y);
    }
    
//    CCLog("Outside the map");
    
    return ccp(-1,-1);
    
}

int MapLayer::Get_TiledValue_By_Key_Position(CCPoint position, const char * key )
{
    CCPoint tileCoordinate = this->Posititon_To_tileCoordinate(position);
//    CCLog("tileCoordinate:(%f, %f)",tileCoordinate.x,tileCoordinate.y);
	CCSize size = gameMap->getMapSize();
	if (tileCoordinate.x<0|| tileCoordinate.y<0
		|| tileCoordinate.x >= size.width || tileCoordinate.y >= size.height)
    {
        return OUT_OF_MAP;
    }
    
    
    int gid = m_MapLayer1->tileGIDAt(tileCoordinate);
    CCDictionary* dict = gameMap->propertiesForGID(gid);
    if (dict == NULL)
    {
        return EMPTY_TILE;
    }
    
    return  dict->valueForKey(key)->intValue();
    
}

bool MapLayer::checkCanMove(CCPoint cp)
{
	int Gid = Get_TiledValue_By_Key_Position(cp);
//    CCLog("GID: %d",Gid);
    if ( Gid>0&&Gid<7)
    {
        return false;
    }
	if(Gid == OUT_OF_MAP)
    {
        return false;
    }
    
    return true;

}

bool MapLayer::HitAndDelete(CCPoint cp)
{
	bool hit = false;
	int Gid = Get_TiledValue_By_Key_Position(cp);
	CCPoint tPos = Posititon_To_tileCoordinate(cp);
//    CCLog("GID: %d",Gid);
    if ( Gid == 1)//brick wall
    {
		m_MapLayer1->removeTileAt(tPos);
        hit =  true;
    }
	if(Gid == 2 )//iron wall
	{
		hit =  true;
	}
    
	if(Gid == OUT_OF_MAP)
    {
        hit = true;
    }
	return hit;

}

bool MapLayer::IsGrass(CCPoint cp)
{
    int Gid = Get_TiledValue_By_Key_Position(cp);
    if (Gid == 8)
    {
        return true;
    }
    return false;
}

bool MapLayer::IsHome(CCPoint cp)
{
    int Gid = Get_TiledValue_By_Key_Position(cp);
    if (Gid == 5)
    {
        m_MapLayer2->setVisible(true);//Broken Home
        return true;
    }
    return false;
}

