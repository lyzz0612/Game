//
//  FRPageTileList.cpp
//  MyPageTileList
//
//  Created by yun ling on 13-8-8.
//
//

#include "MyFRPageTileList.h"

//初始化一些变量，变量含义请查看头文件
MyFRPageTileList::MyFRPageTileList() :
    m_pscrolllayer(NULL),
    m_pscrollview(NULL),
    m_datalist(NULL),
    m_numofpage(0),
    m_currentpage(0),
    m_codevisible(true),
    m_cols(0),
    m_rows(0),
    m_paddingx(10),
    m_paddingy(10),
    m_code_seloffset(-1),
    m_code_unseloffset(-1),
    m_dyn_create(false)
{
    
}
MyFRPageTileList::~MyFRPageTileList()
{
    unregisterItemTapScriptCallFunc();
    unregisterPageChangeScriptCallFunc();
    
	CC_SAFE_RELEASE(m_datalist);
}
/*  @brief 初始化
 *  @params:
 *      1. datalist :CCArray*,  item列表，每个都是CCNode，大小是3*row*col
 *      2. width    :int     ,  可视区域宽度
 *      3. height   :int     ,  可视区域高度
 *      4. page     :int     ,  页数
 *      5. row      :int     ,  item行数
 *      6. col      :int     ,  item列数
 *      7. itemGap  :float   ,  item间隔
 *      8. visible  :bool    ,  页码是否可见
 */
MyFRPageTileList* MyFRPageTileList::create(CCArray* datalist, int width, int height, int page, int row, int col, float itemGap, bool visible,bool dyncre)
{
    MyFRPageTileList* ret = new MyFRPageTileList();
    if (ret && ret->init(datalist, width, height, page, row, col, itemGap, visible, dyncre))
    {
        ret->autorelease();
        return ret;
    }
    CCLOG("MyFRPageTileList create failed!");
    CC_SAFE_DELETE(ret);
    return NULL;
}
bool MyFRPageTileList::init(CCArray* datalist, int width, int height, int page, int row, int col, float itemGap, bool visible, bool dyncre)
{
    FRUIComponent::init();
    
    //设置属性
    setDataList(datalist);
    setNumOfPage(page);
    setNumofRow(row);
    setNumOfCol(col);
    setCodeVisible(visible);
    setViewSize(width, height);
    setItemGap(itemGap);
    setTouchEnabled(true);                      //设置触摸有效
    setContentSize(CCSizeMake(width, height));
    setDynCreate(dyncre);
    //计算item大小
    calculateItemSize();
    
    //触摸层
    CCLayer* temp_layer = CCLayer::create();
    temp_layer->setAnchorPoint(CCPointZero);
    temp_layer->setPosition(CCPointZero);
    temp_layer->setContentSize(CCSizeMake(width, height));
    
    //滚动
    FRScrollView* temp_view = FRScrollView::create(temp_layer, CCSizeMake(width, height), CCSizeMake(page * width, height), 0.0f);
    temp_view->setDirection(kCCScrollViewDirectionHorizontal);
    
    setScrollLayer(temp_layer);
    setScrollView(temp_view);
    temp_view->getScrollView()->setTouchEnabled(false);
    addChild(temp_view);

    //根据是否显示页码更新布局
    if(!visible)
        needLayout();
    return true;
}
/**
 *	@brief	计算item大小
 */
void MyFRPageTileList::calculateItemSize()

{
    float temp_width = (m_viewsize.width - m_paddingx * 2 - (m_cols - 1) * m_itemgap) / m_cols;
    float temp_height = (m_viewsize.height - m_paddingy * 2 - (m_rows - 1) * m_itemgap) / m_rows;
    m_itemsize = CCSizeMake(temp_width, temp_height);
}
/*  @brief更新布局
 *  分别更新当前页，上一页，下一页
 *  第一页不更新上一页，最后一页不更新下一页
 */
void MyFRPageTileList::needLayout()
{
    m_pscrolllayer->removeAllChildrenWithCleanup(true);
    int start_index;
    if(m_dyn_create)
        start_index = m_rows * m_cols;
    else
        start_index = m_currentpage * m_rows * m_cols;  //不是动态创建
    if(m_currentpage == 0)
        start_index = 0;
    
    pageLayout(m_currentpage, start_index);
    
    if(m_currentpage < m_numofpage - 1)
        pageLayout(m_currentpage + 1, start_index + m_rows * m_cols);
    if(m_currentpage > 0)
        pageLayout(m_currentpage - 1, start_index - m_rows * m_cols);
}
/*  @brief更新page页布局
 *  @params
 *  page:要更新的页， start_index：在datalist中开始的item位置
 */
void MyFRPageTileList::pageLayout(int page, int start_index)
{
    int row_index,col_index;
    for(row_index = m_rows - 1;row_index >= 0;row_index--)
    {
        if(start_index >= m_datalist->count())
            break;
        for (col_index = 0; col_index < m_cols; col_index++)
        {
            if(start_index >= m_datalist->count())
                break;
            CCNode* node = dynamic_cast<CCNode*>(m_datalist->objectAtIndex(start_index++));
            
            node->setAnchorPoint(CCPointZero);
            float temp_x = page * m_viewsize.width + m_paddingx + col_index * (m_itemsize.width + m_itemgap);
            float temp_y = m_paddingy + row_index * (m_itemsize.height + m_itemgap);
            node->setPosition(CCPointMake(temp_x, temp_y));
            
            this->recusiveNode(node);
            
            m_pscrolllayer->addChild(node);
        }
    }
}

//设置触摸穿透
void MyFRPageTileList::recusiveNode(CCNode* root)
{
    CCArray* children = root->getChildren();
    CCSize temp;
    CCPoint oldpos,newpos,ancpos;
    if (children != NULL) {
        int count = children->count();
        for (int i = 0; i < count; i++) {
            CCNode* child = (CCNode*)children->objectAtIndex(i);
            
            child->setTouchPenetrate(true);     //穿透
            recusiveNode(child);
        }
    }
    root->setTouchPenetrate(true);
}
//替换指定item
void MyFRPageTileList::replaceItem(CCNode* old_item = NULL, CCNode* new_item = NULL)
{
    if(new_item == NULL)
        CCLog("MyFRPageTileList.replaceItemOnIndex: new_item is NULL");
    else
    {
        if(old_item == NULL)
            CCLog("no item on index");
        else
        {
            CCPoint pos = old_item->getPosition();
            int datalist_index = m_datalist->indexOfObject(old_item);
            CCPoint localpos = ccp(pos.x - m_currentpage * m_viewsize.width - m_paddingx, pos.y - m_paddingy);
            if(localpos.x < 0 || localpos.x > m_viewsize.width)
                CCLog("old_item not in view");
            else
            {
                new_item->setAnchorPoint(CCPointZero);
                new_item->setPosition(pos);
                
                this->recusiveNode(new_item);
                
                m_pscrolllayer->removeChild(old_item, true);    //移除旧结点
                
                m_pscrolllayer->addChild(new_item);
                m_datalist->replaceObjectAtIndex(datalist_index, new_item);    //替换datalist里的值
            }
        }
    }
    
}
/*@ brief设置页码风格，更新布局
 *@ params
 *  selected:       存放选中状态页码的图片名
 *  unselected :    存放未被选中状态页码的图片名
 *  height: 页码高度    codegap:页码间的距离
 *  selrect、unselrect:    第一个页码在图片中的截取区域
 *  seloffset,unseloffset:  图片中页码的水平距离，即所有页码在图片中放到同一行，大小、间距相同
 */
void MyFRPageTileList::setPageCodeStyle(char* selected,char* unselected,float height,float codegap,CCRect selrect,CCRect unselrect,float seloffset,float unseloffset)
{
    m_code_selected = selected;
    m_code_unselected = unselected;
    m_code_height = height;
    m_code_gap = codegap;
    m_code_selrect = selrect;               //CCRect重载了=
    m_code_unselrect = unselrect;
    m_code_seloffset = seloffset;
    m_code_unseloffset = unseloffset;
    
    //如果页码需要显示
    if(m_codevisible)
    {
        //设置scrollview位置和contentsize
        m_pscrollview->CCNode::setPosition(0, m_code_height);
        setContentSize(CCSizeMake(m_viewsize.width, m_viewsize.height + m_code_height));
        removeOldPageCode(m_numofpage);     //移除旧页码
        PageCodeLayout();
        needLayout();
    }
}
//设置页码布局
void MyFRPageTileList::PageCodeLayout()
{
    CCSpriteFrameCache* framecache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCRect temp_rect;
    
    float beginPos = (m_viewsize.width - (m_numofpage - 1) * m_code_gap) / 2;
    for(int i = 0;i < m_numofpage;i++)
    {
        //根据页码设置截取区域
        temp_rect.setRect(m_code_selrect.getMinX() + m_code_seloffset * i, m_code_selrect.getMinY(), m_code_selrect.getMaxX() - m_code_selrect.getMinX(), m_code_selrect.getMaxY() - m_code_selrect.getMinY());
        //每个页码有对应的spriteframename
        framecache->addSpriteFrame(CCSpriteFrame::create(m_code_selected, temp_rect), CCString::createWithFormat("selected%d",i)->getCString());
        
        temp_rect.setRect(m_code_unselrect.getMinX() + m_code_unseloffset * i, m_code_unselrect.getMinY(), m_code_unselrect.getMaxX() - m_code_unselrect.getMinX(), m_code_unselrect.getMaxY() - m_code_unselrect.getMinY());
        framecache->addSpriteFrame(CCSpriteFrame::create(m_code_unselected, temp_rect), CCString::createWithFormat("unselected%d",i)->getCString());
        
        CCSprite* temp_sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("unselected%d",i)->getCString());
        //设置tag
        temp_sprite->setTag(100 + i);
        temp_sprite->CCNode::setPosition(beginPos+ i * m_code_gap, m_code_height / 2);
        addChild(temp_sprite);
    }
    CCSprite* temp_sprite = dynamic_cast<CCSprite*>(this->getChildByTag(100 + m_currentpage));
    temp_sprite->setDisplayFrame(framecache->spriteFrameByName(CCString::createWithFormat("selected%d",m_currentpage)->getCString()));
}
//移除旧的页码
void MyFRPageTileList::removeOldPageCode(int oldpage)
{
    for (int i = 0; i < oldpage; i++) {
        removeChildByTag(100 + i, true);
    }
}
//设置页数
void MyFRPageTileList::setNumOfPage(int page)
{
    int oldpage = m_numofpage;
    m_numofpage = page;
    if(oldpage != 0)        //不是在init中调用
    {
        //如果有设置改变页数的回调函数的话
        if(m_pagechange_func > 0)
        {
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(this, "self");
            dict->setObject(CCInteger::create(m_currentpage), "currentpage");
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeFunctionByHandlerWithTable(m_pagechange_func,dict);
        }
        //如果当前页大于最大页的话，到最后一页
        if(m_currentpage >= m_numofpage)
            m_currentpage = m_numofpage - 1;
        else
            m_currentpage = 0;
        //页码需要显示的话，更新
        if(m_codevisible)
        {
            removeOldPageCode(oldpage);
            PageCodeLayout();
        }
        //移动scrollview
        m_pscrollview->setContentOffset(ccp(-m_viewsize.width * m_currentpage, 0),true);
        m_pscrollview->setContentSize(CCSizeMake(m_numofpage * m_viewsize.width, m_viewsize.height));
        needLayout();
    }
}
//设置页码是否可见
void MyFRPageTileList::setCodeVisible(bool var)
{
    if(m_codevisible != var)
    {
        m_codevisible = var;
        //要设为可见并且style有初始化
        if(m_codevisible == true && m_code_unseloffset >= 0)
        {
            m_pscrollview->CCNode::setPosition(0, m_code_height);
            setContentSize(CCSizeMake(m_viewsize.width, m_viewsize.height + m_code_height));
            removeOldPageCode(m_numofpage);
            PageCodeLayout();
            needLayout();
        }
        //如果要设为不可见，且style已被初始化
        else if(m_codevisible == false && m_code_unseloffset >= 0)
        {
            m_pscrollview->CCNode::setPosition(0, 0);
            setContentSize(CCSizeMake(m_viewsize.width, m_viewsize.height));
            removeOldPageCode(m_numofpage);
            needLayout();
        }
    }
}
//获取页码是否可见
bool MyFRPageTileList::getCodeVisible()
{
    return m_codevisible;
}
//设置数据源
void MyFRPageTileList::setDataList(CCArray* var)
{
    if (m_datalist != var) {
        CC_SAFE_RETAIN(var);
        CC_SAFE_RELEASE(m_datalist);
        m_datalist = var;
    }
}
//获取数据源
CCArray* MyFRPageTileList::getDataList()
{
    return m_datalist;
}
//获取页数
int MyFRPageTileList::getNumOfPage()
{
    return m_numofpage;
}
//设置滚动可视区域大小，不包括页码
void MyFRPageTileList::setViewSize(float view_width,float view_height)
{
    m_viewsize = CCSizeMake(view_width, view_height);
}
CCSize MyFRPageTileList::getViewSize()
{
    return m_itemsize;
}
//注册页码改变的回调函数
void MyFRPageTileList::registerPageChangeScriptCallFunc(int func)
{
    m_pagechange_func = func;
}
//移除页码改变的回调函数
void MyFRPageTileList::unregisterPageChangeScriptCallFunc()
{
    if (m_pagechange_func) {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_pagechange_func);
    }
    m_pagechange_func = 0;
}
//注册itep被tap的回调函数
void MyFRPageTileList::registerItemTapScriptCallFunc(int func)
{
    m_itemtap_func = func;
}
void MyFRPageTileList::unregisterItemTapScriptCallFunc()
{
    if (m_itemtap_func) {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_itemtap_func);
    }
    m_itemtap_func = 0;
}
//触摸点是否在可视区域内
bool MyFRPageTileList::isTouchInside(CCTouch* ptouch)
{
    return m_pscrollview->getScrollView()->isTouchInside(ptouch);
}
void MyFRPageTileList::registerWithTouchDispatcher()
{
    FRUIComponent::registerWithTouchDispatcher();
}
bool MyFRPageTileList::ccTouchBegan(CCTouch* ptouch,CCEvent* event)
{
    if(isTouchInside(ptouch))
    {
        m_begantime = millisecondNow();     //记录触摸开始时间
        m_beganpoint = ptouch->getLocation();
        m_pscrollview->getScrollView()->ccTouchBegan(ptouch, event);
        return true;
    }
    return false;
}
void MyFRPageTileList::ccTouchMoved(CCTouch* ptouch,CCEvent* event)
{
    m_pscrollview->getScrollView()->ccTouchMoved(ptouch, event);
}
void MyFRPageTileList::ccTouchEnded(CCTouch* ptouch,CCEvent* event)
{
    m_pscrollview->getScrollView()->ccTouchEnded(ptouch, event);
    m_endtime = millisecondNow();       //触摸结束时间
    m_endpoint = ptouch->getLocation();
    
    bool tomovepage = true;
    CCPoint diff = ccpSub(m_endpoint, m_beganpoint);
    float moverange = m_itemsize.width;
    //如果移动距离短并且触摸持续时间也短的话
    if(diff.x > -moverange  && diff.x < moverange && diff.y > -moverange && diff.y < moverange)
    {
       if(m_endtime - m_begantime <= 180)
            tomovepage = false;
    }
    //判断是否触摸在item上
    if(!tomovepage)
    {
        CCPoint pos = m_pscrollview->getScrollView()->getContainer()->convertTouchToNodeSpace(ptouch);
        
		CCPoint localpos = ccp(pos.x - m_currentpage * m_viewsize.width - m_paddingx, pos.y - m_paddingy);
		float offsetX = fmodf(localpos.x, (m_itemsize.width + m_itemgap));
		float offsetY = fmodf(localpos.y, (m_itemsize.height + m_itemgap));
        //点击在某个item上
		if (offsetX < m_itemsize.width && offsetY < m_itemsize.height && m_itemtap_func > 0)
        {
            int tapItemX = localpos.x / (m_itemsize.width + m_itemgap);
            int tapItemY = localpos.y / (m_itemsize.height + m_itemgap);
            int temp_index = 0;
            if(m_currentpage != 0)
                temp_index = 1;
            int item = temp_index * m_rows * m_cols + (m_rows - tapItemY - 1) * m_cols + tapItemX;
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(this, "self");
            dict->setObject(m_datalist->objectAtIndex(item), "tapindex");
            //执行注册的回调函数
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeFunctionByHandlerWithTable(m_itemtap_func,dict);
        }
    }
    else
    {
        CCPoint endPos = CCDirector::sharedDirector()->convertToGL(m_endpoint);
        CCPoint startpPos = CCDirector::sharedDirector()->convertToGL(m_beganpoint);
		updateView(endPos.x - startpPos.x,m_endtime - m_begantime);  
    }
}
//根据移动距离和触摸持续时间更新显示
void MyFRPageTileList::updateView(float offsetx,float time)
{
    float moverange = m_itemsize.width * 2;
    
    int lastpage = m_currentpage;
    bool pagechange = false;
    //如果时间短，移动距离页没到一半，但是却在规定范围内，也算是翻页
    if((offsetx <= -m_viewsize.width / 2 || (offsetx <= -moverange && time <= 300) ) && m_currentpage < m_numofpage - 1)
    {
        m_currentpage++;
        pagechange = true;
    }
    if((offsetx >= m_viewsize.width / 2 || (offsetx >= moverange && time <= 300) ) && m_currentpage > 0)
    {
        m_currentpage--;
        pagechange = true;
    }
    m_pscrollview->setContentOffset(ccp(-m_viewsize.width * m_currentpage, 0),true);
    if(pagechange)
    {
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        if(m_pagechange_func > 0)
        {
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(this, "self");
            dict->setObject(CCInteger::create(m_currentpage), "currentpage");
            dict->setObject(CCBoolean::create(false), "init");
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeFunctionByHandlerWithTable(m_pagechange_func,dict);
        }
        needLayout();
        if(m_codevisible)
        {
            CCSprite* temp_sprite = dynamic_cast<CCSprite*>(this->getChildByTag(100+lastpage));
            temp_sprite->setDisplayFrame(cache->spriteFrameByName(CCString::createWithFormat("unselected%d",lastpage)->getCString()));
            
            temp_sprite = dynamic_cast<CCSprite*>(this->getChildByTag(100+m_currentpage));
            temp_sprite->setDisplayFrame(cache->spriteFrameByName(CCString::createWithFormat("selected%d",m_currentpage)->getCString()));
        }
    }
    long temp_time = millisecondNow();
}
//item进入时的特效
void MyFRPageTileList::loadAnimation()
{
    if (m_currentpage != 0) {
        return;
    }
    animation_Index = 0;
    
    for (int i = 0, count = m_rows * m_cols; i < count; i++) {
        CCNode* tileItem = dynamic_cast<CCNode*>(m_datalist->objectAtIndex(i));
        CCPoint oriPos = tileItem->getPosition();
        CCPoint startPos = ccpAdd(oriPos, CCPointMake(m_viewsize.width, 0));
        tileItem->setPosition(startPos);
    }
    this->schedule(schedule_selector(MyFRPageTileList::singleAnimation), 0.01f);
}
//每个item进入的特效，每隔0.01调用一次
void MyFRPageTileList::singleAnimation(float dt)
{
    if (animation_Index >= m_rows * m_cols) {
        animation_Index = 0;
        this->unschedule(schedule_selector(MyFRPageTileList::singleAnimation));
        return;
    }
    CCNode* cell = dynamic_cast<CCNode*>(m_datalist->objectAtIndex(animation_Index));
    CCPoint targetPos = CCPointMake(cell->getPosition().x - m_viewsize.width, cell->getPosition().y);
    CCActionInterval* move = CCMoveTo::create(1, targetPos);
    CCActionInterval* move_ease_in = CCEaseExponentialInOut::create(move);
    CCActionInterval* skew = CCSkewTo::create(1, 0, 0);
    
    CCAction* spawn = CCSpawn::create(move_ease_in, skew, NULL);
    cell->runAction(spawn);
    CCDelayTime *delay = CCDelayTime::create(0.01f);
    
    cell->runAction(delay);
    
    animation_Index++;
}



