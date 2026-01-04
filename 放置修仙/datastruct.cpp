#pragma once
#include<iostream>
#include"global.h"
#include"datastruct.h"
using namespace std;
/*功法全局变量设计*/
//基础剑诀
book book_0 = {
    0,                                                                                  //int level;                        //功法等级
    "基础剑诀",                                                                         //std::string name;                 //功法名称
    "0",                                                                                 //std::string pic;                  //功法贴图
    "隐世剑派的入门心法，招式朴实无华却暗含剑道至理招式，虽无惊天威力却胜在根基扎实。修行速度+1.0.",   //std::string introduction = 0;     //功法介绍
    1.0                                                                                 //double speed;                     //速率
};

//紫霞心经
book book_1 = {
    1,
    "紫霞心经",

    "1",
    "华山派内功心法，以朝阳紫气为引，修炼时周身霞光隐现，晨时修炼最佳，可养浩然正气。修行速度+2.0.",
    2.0
};

//血煞魔功
book book_2 = {
    1,
    "血煞魔功",

    "2",
    "魔道速成功法，以血炼气，进境极快但根基不稳。修行速度+2.5.",
    2.5
};

//太虚玄功
book book_3 = {
    2,
    "太虚玄功",
    "3",
    "蜀山无上心法，讲究天人合一，修炼时如坐云端，神游太虚。修行速度+6.0.",
    6.0
};

//混沌道经
book book_4 = {
    2,
    "混沌道经",
    "4",
    "上古大能所留残卷，修炼时引动混沌之气，逆转阴阳，吞噬天地灵气为己用。修行速度+8.0.",

    8.0
};

/*丹药全局变量设计*/
//基础破障丹
pill pill_0 = {
    0,          //int level;                           //丹药等级
    "基础破障丹",         //std::string name;                    //丹药名称
    "元婴修士以本命真火所炼，可助冲破小境界桎梏。用于修为圆满后突破境界。",         //std::string introduction;            //丹药介绍
    "0",         //std::string pic;                     //丹药贴图
    0,          //int kind_flag;                       //丹药种类 （0晋级型 1功能型）
    0,          //int level_up;                        //丹药能直接增加的等级数
    0,          //double EXP_up;                       //丹药能直接增加的经验数
    0,          //double speed_up;                     //丹药能直接增加的根骨（修炼速度）
};

//小还丹
pill pill_1 = {
    0,
    "小还丹",
    "药王谷外门弟子标配，以十年何首乌混合晨露炼制，丹色青灰。服用后修为+80.",
    "1",
    1,
    0,
    80,
    0
};

//易筋丸
pill pill_2 = {
    0,
    "易筋丸",
    "少林达摩院基础丹药，服之可闻筋骨雷鸣之声。服用后根骨+0.03.",
    "2",
    1,
    0,
    0,
    0.03
};

//三转玄元丹
pill pill_3 = {
    1,
    "三转玄元丹",
    "丹霞派秘传，需以三昧真火连续转炼三次，丹纹如螺旋，服之抵一载苦修。服用后修为+1000.",
    "3",
    1,
    0,
    1000,
    0
};

//冲脉散
pill pill_4 = {
    1,
    "冲脉散",
    "江湖神医所创，以银针刺穴激发潜能，服用前请道友三思！！！！服用后提升一重境界，根骨-0.4.",
    "4",
    1,
    1,
    0,
    -0.4
};

//琉璃玉髓丹
pill pill_5 = {
    1,
    "琉璃玉髓丹",
    "昆仑山山主以昆仑玉髓为主药，服后通体如琉璃无垢。服用后使根骨+0.4.",
    "5",
    1,
    0,
    0,
    0.4
};

//九转金丹
pill pill_6 = {
    2,
    "九转金丹",
    "传说道门至高秘药九转金丹，由太上老君丹炉炼制，服之可立地成就半仙之体。服用后修为+30000.",
    "6",
    1,
    0,
    30000,
    0
};

//混沌道体丹
pill pill_7 = {
    2,
    "混沌道体丹",
    "上古体修大能精血所化，服之可铸就先天道体，有大帝之姿。服用后根骨+3.0.",
    "7",
    1,
    0,
    0,
    3.0
};

//逆天改命丹
pill pill_8 = {
    2,
    "逆天改命丹",
    "窃阴阳，夺造化，服用者需承受天道反噬。修行切忌急功近利，道友慎用！！！服用后提升三重境界，但根骨-1.0.",
    "8",
    1,
    3,
    0,
    -0.8
};

//两仪造化丹
pill pill_9 = {
    2,
    "两仪造化丹",
    "昆仑瑶池千年一现的奇丹，采阴阳二气然凝结而成。服之可连破两重境界，毫无根基虚浮之虞。",
    "9",
    1,
    2,
    0,
    0
};


/*事件全局变量设计*/
//事件0-灵脉共鸣
Event event_0 = {
    0,                                                         //int event_flag;                      //事件种类（0 数值变化类事件 1 获得功法事件 2 获得丹药事件 3 前进事件 4 后退事件）
    "灵脉共鸣",                                                //std::string event_title;             //事件标题
    "修炼时引动地底灵脉共鸣，海量灵气疯狂涌入体内",            //std::string event_intro;             //事件简介
    500.0,                                                     //double event_EXP;                    //事件造成的经验值变化（可以为0）
    1,                                                         //int event_level;                     //事件造成的等级变化（可以为0）
    0,                                                         //double event_speed;                  //事件造成的根骨变化（可以为0）
    0,                                                         //int event_stone;                     //事件造成的灵石变化（可以为0）
    -1,                                                        //int event_book;                      //事件得到的功法的ID（无功法则为-1）
    -1,                                                        //int event_pill;                      //事件得到的丹药的ID（无丹药则为-1）
    0,                                                         //int event_pill_num;                  //事件得到的丹药数量（无丹药则为0）
    0                                                          //int event_move;                      //事件造成的前进/后退步数
};
//事件1-雷劫淬体
Event event_1 = {
    0,
    "雷劫淬体",
    "冒险以肉身硬接雷劫余波，在痛苦中锤炼根骨",
    600.0,
    0,
    2.0,
    0,
    -1,
    -1,
    0,
    0
};
//事件2秘境夺宝
Event event_2 = {
    0,
    "秘境夺宝",
    "在秘境中击败其他修士，抢夺其携带的灵石与修炼资源",
    400.0,
    0,
    0,
    800,
    -1,
    -1,
    0,
    0
};

//事件3-传承玉简
Event event_3 = {
    1,
    "传承玉简",
    "在古老遗迹中发现刻有基础剑诀的玉简，参悟后习得",
    0,
    0,
    0,
    0,
    0,
    -1,
    0,
    0
};
//事件4-宗门赐功
Event event_4 = {
    1,
    "宗门赐功",
    "因对宗门贡献巨大，长老亲授紫霞心经",
    0,
    0,
    0,
    0,
    1,
    -1,
    0,
    0
};
//事件5-魔道遗藏
Event event_5 = {
    1,
    "魔道遗藏",
    "在魔道巢穴深处找到血煞魔功，修炼后掌握",
    0,
    0,
    0,
    0,
    2,
    -1,
    0,
    0
};
//事件6-上古传承
Event event_6 = {
    1,
    "上古传承",
    "触发上古传承阵，习得完整太虚玄功",
    0,
    0,
    0,
    0,
    3,
    -1,
    0,
    0
};
//事件7-混沌之初
Event event_7 = {
    1,
    "混沌之初",
    "在混沌秘境中，机缘巧合获得混沌道经",
    0,
    0,
    0,
    0,
    4,
    -1,
    0,
    0
};

//事件8-药田寻丹
Event event_8 = {
    2,
    "药田寻丹",
    "在宗门药田发现遗漏的小还丹，小心采摘",
    0,
    0,
    0,
    0,
    -1,
    1,
    3,
    0
};
//事件9-丹炉遗丹
Event event_9 = {
    2,
    "丹炉遗丹",
    "路过废弃丹炉，发现残留的易筋丸",
    0,
    0,
    0,
    0,
    -1,
    2,
    2,
    0
};

//事件10-天地孕丹
Event event_10 = {
    2,
    "天地孕丹",
    "在灵气谷发现天地孕育的三转玄元丹",
    0,
    0,
    0,
    0,
    -1,
    3,
    1,
    0
};

//事件11-论道奖励
Event event_11 = {
    2,
    "论道奖励",
    "论道大会夺冠，获赠琉璃玉髓丹",
    0,
    0,
    0,
    0,
    -1,
    5,
    1,
    0
};
//事件12-绝境机缘
Event event_12 = {
    2,
    "绝境机缘",
    "坠崖遇福地，获混沌道体丹",
    0,
    0,
    0,
    0,
    -1,
    7,
    1,
    0
};

//事件13-秘境拾丹
Event event_13 = {
    2,
    "秘境拾丹",
    "误入洞府，在石榻缝隙中捡到一枚玉盒，内藏三枚能提升灵气纯度的基础破障丹",
    0,
    0,
    0,
    0,
    -1,
    0,
    3,
    0
};
//事件14-鬼市获丹
Event event_14 = {
    2,
    "鬼市获丹",
    "埋伏鬼市一位修仙者,杀人越货后,收获5枚冲脉散",
    0,
    0,
    0,
    0,
    -1,
    4,
    5,
    0
};

//事件15-赌石出丹
Event event_15 = {
    2,
    "赌石出丹",
    "切开低价购入的矿石，意外发现内嵌一枚被晶石包裹的九转金丹",
    0,
    0,
    0,
    0,
    -1,
    6,
    1,
    0
};

//事件16-仙藤结丹
Event event_16 = {
    2,
    "仙藤结丹",
    "遇见千年灵藤时，藤蔓根部结出一枚两仪造化丹",
    0,
    0,
    0,
    0,
    -1,
    9,
    1,
    0
};
//事件17-时空乱流
Event event_17 = {
    4,
    "时空乱流",
    "后退2步",
    0,
    0,
    0,
    0,
    -1,
    -1,
    0,
    2
};
//事件18-传送阵法
Event event_18 = {
    3,
    "传送阵法",
    "偶遇传送阵，前进2步",
    0,
    0,
    0,
    0,
    -1,
    -1,
    0,
    2
};
//事件19-时空乱流
Event event_19 = {
    3,
    "时空乱流",
    "遭遇时空乱流，前进1步",
    0,
    0,
    0,
    0,
    -1,
    -1,
    0,
    1
};


/*大富翁格子结构体变量设计*/
//...(名称)
EventMap map_0 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    0	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_1 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",      //格子贴图
    1	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_2 = {
    0,      //bool isTrigger;         //是否触发
     "pic\\eventmap.jpg",      //格子贴图
    2	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_3 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",      //格子贴图
    3	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_4 = {
    0,      //bool isTrigger;         //是否触发
   "pic\\eventmap.jpg",       //格子贴图
    4	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_5 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    5  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_6 = {
    0,      //bool isTrigger;         //是否触发
     "pic\\eventmap.jpg",       //格子贴图
    6	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_7 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    7	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_8 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    8	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_9 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    9	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_10 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",      //格子贴图
    10	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_11 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    11	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_12 = {
    0,      //bool isTrigger;         //是否触发
     "pic\\eventmap.jpg",       //格子贴图
    12	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_13 = {
    0,      //bool isTrigger;         //是否触发
   "pic\\eventmap.jpg",       //格子贴图
    13	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_14 = {
    0,      //bool isTrigger;         //是否触发
     "pic\\eventmap.jpg",       //格子贴图
    14	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_15 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    15	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_16 = {
    0,      //bool isTrigger;         //是否触发
     "pic\\eventmap.jpg",       //格子贴图
    16	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_17 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    17	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_18 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",       //格子贴图
    18	  //int event_id;         //事件ID（-1表示无事件）
};
EventMap map_19 = {
    0,      //bool isTrigger;         //是否触发
    "pic\\eventmap.jpg",     //格子贴图
    19	  //int event_id;         //事件ID（-1表示无事件）
};
//...

