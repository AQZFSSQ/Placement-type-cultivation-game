#pragma once
#include<string>
#include<ctime>
#include<easyx.h>
#include<vector>


/* 宏定义 */
#define BOOK_NUM 100    //功法总体设计数量
#define PILL_NUM 100    //丹药总体设计数量
#define LEVEL_NUM 31   //等级总体设计数量
#define EVENT_NUM 100   //事件总体设计数量
#define CARD_NUM 14    //卡池总体设计数量
#define MAP_NUM 100     //地图总体设计数量
#define MAX_COUNT 1000  //最大物品数量

/* 结构体声明 */
// 背包结构体 - 用于存储角色携带的物品
struct bag {
    int b[BOOK_NUM];  // 存储功法(book)及其数量
    int p[PILL_NUM];  // 存储丹药(pill)及其数量
};

//修仙者结构
struct Magician {
    char name[100];                 //修仙者姓名
    char sex[4];                    //性别（男/女）
    double basic_speed;             //根骨（基础速率，可通过高稀有度丹药增加）
    long long EXP;                     //经验值
    long long stone;                   //灵石 
    int furtuValue;                 //幸运值（用于抽卡）
    int event_point;                //当前事件位置
    int level_id;                   //境界
    int uses_book_id;               //当前正在使用的功法
    bag useBag;                    //修炼者拥有背包
};

// 功法结构体 - 代表角色可以学习的技能/功法
struct book {
    int level;                      //功法等级( 0~2 )
    std::string name;               //功法名称
    std::string pic;                //功法贴图
    std::string introduction;       //功法介绍
    double speed;                   //速率
};

//境界结构
struct level {
    std::string name;           //境界名称
    double expRequired;         //升级需要经验
    int pillRequired;           //升级所需要的升级丹药数量
};

//丹药结构
struct pill {
    int level;                           //丹药等级( 0~ 2 )
    std::string name;                    //丹药名称
    std::string introduction;            //丹药介绍
    std::string pic;                     //丹药贴图
    int kind_flag;                       //丹药种类 （0晋级型 1功能型）
    int level_up;                        //丹药能直接增加的等级数
    double EXP_up;                       //丹药能直接增加的经验数
    double speed_up;                     //丹药能直接增加的根骨（修炼速度）
};

//事件结构
struct Event {
    int event_flag;                      //事件种类（0 数值变化类事件 1 获得功法事件 2 获得丹药事件 3 前进事件 4 后退事件）
    std::string event_title;             //事件标题
    std::string event_intro;             //事件简介
    double event_EXP;                    //事件造成的经验值变化（可以为0）
    int event_level;                     //事件造成的等级变化（可以为0）
    double event_speed;                  //事件造成的根骨变化（可以为0）
    double event_stone;                  //事件造成的灵石变化（可以为0）
    int event_book;                      //事件得到的功法的ID（无功法则为-1）
    int event_pill;                      //事件得到的丹药的ID（无丹药则为-1）
    int event_pill_num;                  //事件得到的丹药数量（无丹药则为0）
    int event_move;                      //事件造成的前进/后退步数
};

//大富翁地图格子结构
struct EventMap {
    bool isTrigger;         //是否触发
    std::string pic;          //格子贴图
    int event_id;         //事件ID（-1表示无事件）
};


//抽卡卡池结构
struct GachaCard {
    int card_flag;          //卡牌种类（0 功法，1 丹药）
    int id;                 //卡牌内容ID
};

// -------------------- 数据设计 --------------------




