#pragma once
#include"datastruct.h"


extern int currRole_id;                                                    //当前角色在数组Role的索引，即角色的编号
extern int hang_up_stone;                                                  //挂机灵石数
extern int hang_up_exp;                                                    //挂机经验数
extern int get_stone_speed;                                                //挂机获取灵石的速率，即每过一个挂机周期获得多少灵石
extern clock_t hang_up_alltime;                                            //记录从上次领取挂机奖励的时间
extern clock_t hang_up_priod;                                       //挂机周期，表示每多少毫秒更新一下挂机奖励池,这里默认为5秒
extern clock_t hang_up_time;                                               //记录从上个挂机周期结束的时间
extern const int MAX;                                               //物品最大数量限制
extern int volume;                                                     //音效设置
extern int BGM;                                                        //背景音乐设置
extern bool cheatMode;                                                     //作弊模式
extern ExMessage msg;                                              //easyx库对象声明
extern Magician currRole;                                                  //游戏当前游玩玩家的存档
extern Magician Role[8];                                                   //所有存档构成的
extern const book methods[BOOK_NUM];
extern const pill pills[PILL_NUM];
extern const Event events[EVENT_NUM];
extern const EventMap maps[MAP_NUM];
extern const GachaCard cards[CARD_NUM];
extern const level realms[LEVEL_NUM];
extern book book_0;
extern book book_1;
extern book book_2;
extern book book_3;
extern book book_4;
extern pill pill_0;
extern pill pill_1;
extern pill pill_2;
extern pill pill_3;
extern pill pill_4;
extern pill pill_5;
extern pill pill_6;
extern pill pill_7;
extern pill pill_8;
extern pill pill_9;
extern Event event_0;
extern Event event_1;
extern Event event_2;
extern Event event_3;
extern Event event_4;
extern Event event_5;
extern Event event_6;
extern Event event_7;
extern Event event_8;
extern Event event_9;
extern Event event_10;
extern Event event_11;
extern Event event_12;
extern Event event_13;
extern Event event_14;
extern Event event_15;
extern Event event_16;
extern Event event_17;
extern Event event_18;
extern Event event_19;
extern EventMap map_0;
extern EventMap map_1;
extern EventMap map_2;
extern EventMap map_3;
extern EventMap map_4;
extern EventMap map_5;
extern EventMap map_6;
extern EventMap map_7;
extern EventMap map_8;
extern EventMap map_9;
extern EventMap map_10;
extern EventMap map_11;
extern EventMap map_12;
extern EventMap map_13;
extern EventMap map_14;
extern EventMap map_15;
extern EventMap map_16;
extern EventMap map_17;
extern EventMap map_18;
extern EventMap map_19;
extern GachaCard card_0;
//抽卡相关全局
extern IMAGE frames[223];//抽卡动画存放数组

//事件相关全局
extern std::vector<Event> eventList; // 事件列表
extern std::vector<EventMap> gameMap; // 地图格子
extern std::vector<std::string> gridImgPaths; // 格子贴图路径
extern std::string playerImgPath; // 玩家头像路径
extern IMAGE diceImg; // 骰子图片
extern int currEventId; // 当前事件id

//视频资源
extern IMAGE img_menu[645];
extern int img_p ;//视频指针
extern clock_t lst_frames;//视频帧率计数器
extern IMAGE vdo_role[505];