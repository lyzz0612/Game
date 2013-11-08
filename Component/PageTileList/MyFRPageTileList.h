/*
 *  FRPageTileList.h
 *  MyFRPageTileList
 *
 *  Created by yun ling on 13-8-8.
 *
 */

#ifndef __MyPageTileList__FRPageTileList__
#define __MyPageTileList__FRPageTileList__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "FRScrollView.h"
#include "FRUIComponent.h"

NS_CC_BEGIN
/* 翻页的格子栏
 *
 */
class MyFRPageTileList : public FRUIComponent
{
public:
    MyFRPageTileList();
    ~MyFRPageTileList();
    
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
     *      9. dyncre   :boll    ,  是否动态创建对象
     */
    static MyFRPageTileList* create(CCArray*,int,int,int,int,int,float,bool,bool);
    virtual bool init(CCArray*,int,int,int,int,int,float,bool,bool);
    
    //更新布局
    void needLayout();
    //递归设置item
    void recusiveNode(CCNode*);
    
    //替换指定item
    void replaceItem(CCNode*,CCNode* new_item);
    
    //页面改变的lua回调函数
    void registerPageChangeScriptCallFunc(int);
    void unregisterPageChangeScriptCallFunc();
    //item被触摸的lua回调函数
    void registerItemTapScriptCallFunc(int);
    void unregisterItemTapScriptCallFunc();
    
    //触摸
    bool isTouchInside(CCTouch*);
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch*,CCEvent*);
    virtual void ccTouchMoved(CCTouch*,CCEvent*);
    virtual void ccTouchEnded(CCTouch*,CCEvent*);
    
    //item进入的动画
    void loadAnimation();

    /*  @brief 设置页码风格
     *  @params:
     *      1. selected         :char*      ,选中状态的图片名
     *      2. unselected       :char*      ,未被选中状态的图片名
     *      3. height           :int        ,高度
     *      4. codegap          :float      ,页码之间的距离
     *      5. selrect          :CCRect     ,选中状态的截取区域
     *      6. unselrect        :CCRect     ,未被选中状态的截取区域
     *      7. selrectoffset    :float      ,页码在图片中的间距
     *      8. unselrectoffset  :float      ,同上
     */
    void setPageCodeStyle(char*,char*,float,float,CCRect,CCRect,float,float);
    //更新页码显示
    void PageCodeLayout();
    
    /*  @类成员变量
     *  m_pscrollview       :FRScrollView   ,滚动显示界面
     *  m_pscrolllayer      :CCLayer        ,滚动层
     *  m_datalist          :CCArray*       ,item列表，每个都是CCNode,大小是3*row*col
     *  m_numofpage         :int            ,页数
     *  m_itemsize          :CCSize         ,item的大小
     *  m_currentpage       :int            ,当前页号
     *  m_codevisible       :bool           ,页码是否可见
     *  m_cols              :int            ,列数
     *  m_rows              :int            ,行数
     *  m_viewsize          :CCSize         ,可视区域大小
     *  m_itemgap           :float          ,item间距
     *  m_paddingx          :float          ,与水平边框的距离
     *  m_paddingy          :float          ,与重置边框的距离
     *  m_beganpoint        :CCPoint        ,触摸开始位置
     *  m_endpoint          :CCPoint        ,触摸结束位置
     *  m_begantime         :float          ,触摸开始时间
     *  m_endtime           :float          ,触摸结束时间
     *  m_itemtap_func      :int            ,触摸到item的lua回调函数
     *  m_pagechange_func   :int            ,切换页的回调函数
     *  m_code_selected     :char*          ,页码被选中的图片名
     *  m_code_unselected   :char*          ,页码未被选中的图片名
     *  m_code_height       :float          ,页码高度
     *  m_code_gap          :float          ,页码间距
     *  m_code_selrect      :CCRect         ,页码被选中的图片截取区域
     *  m_code_unselrect    :CCRect         ,页码未被选中的图片截取区域
     *  m_code_seloffset    :float          ,不同页码在图片中的间距
     *  m_code_unseloffset  :float          ,同上
     *  m_dyn_create        :bool           ,是否动态创建对象
     */
    CC_PROPERTY(CCArray*, m_datalist, DataList);
    CC_PROPERTY(int, m_numofpage, NumOfPage);
    CC_PROPERTY(bool, m_codevisible, CodeVisible);
    CC_SYNTHESIZE(FRScrollView*, m_pscrollview, ScrollView);
    CC_SYNTHESIZE(CCLayer*, m_pscrolllayer, ScrollLayer);
    CC_SYNTHESIZE(CCSize, m_itemsize, ItemSize);
    CC_SYNTHESIZE(int, m_currentpage, CurrentPage);
    CC_SYNTHESIZE(int, m_cols, NumOfCol);
    CC_SYNTHESIZE(int, m_rows, NumofRow);
    CC_SYNTHESIZE(float, m_itemgap, ItemGap);
    CC_SYNTHESIZE(float, m_paddingx, PaddingX);
    CC_SYNTHESIZE(float, m_paddingy, PaddingY);
    CC_SYNTHESIZE(bool, m_dyn_create, DynCreate);
    
    void setViewSize(float view_width, float view_height);
    CCSize getViewSize();
    
protected:
    CCSize m_viewsize;
    CCPoint m_beganpoint;
    CCPoint m_endpoint;
    float m_begantime;
    float m_endtime;
    int m_itemtap_func;
    int m_pagechange_func;
    char* m_code_selected;
    char* m_code_unselected;
    float m_code_height;
    float m_code_gap;
    CCRect m_code_selrect;
    CCRect m_code_unselrect;
    float m_code_seloffset;
    float m_code_unseloffset;
    int animation_Index;
private:
    //移除旧页码
    void removeOldPageCode(int);
    /*  @brief更新一页显示
     *  @params:
     *      1. page         :int    ,第几页
     *      2. start_index  :int    ,在datalist里开始取值的位置
     */
    void pageLayout(int,int);
    //根据移动距离和时间更新显示
    void updateView(float,float);
    
    void singleAnimation(float);
    //计算每个item大小
    void calculateItemSize();
    
};
NS_CC_END
#endif
