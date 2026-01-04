#include<iostream>
#include<algorithm>
#include<windows.h>
#include"global.h"
#include "tools.h"
#include "datastruct.h"
#include "view.h"
#include "service.h"
#include<mmsystem.h>                 // 用于播放音频
#pragma comment(lib, "winmm.lib")    //链接winmm.lib库

/* 全局变量定义 */
int currRole_id;                                                    //当前角色在数组Role的索引，即角色的编号
int hang_up_stone;                                                  //挂机灵石数
int hang_up_exp;                                                    //挂机经验数
int get_stone_speed = 10;                                           //挂机获取灵石的速率，即每过一个挂机周期获得多少灵石
clock_t hang_up_alltime;                                            //记录从上次领取挂机奖励的时间
clock_t hang_up_priod = 5000;                                       //挂机周期，表示每多少毫秒更新一下挂机奖励池,这里默认为5秒
clock_t hang_up_time;                                               //记录从上个挂机周期结束的时间
const int MAX = 1000;                                               //物品最大数量限制
int volume = 5;                                                     //音效设置
int BGM = 5;                                                        //背景音乐设置
bool cheatMode;                                                     //作弊模式
ExMessage msg = { 0 };                                              //easyx库对象声明
Magician currRole;                                                  //游戏当前游玩玩家的存档
Magician Role[8] = {};                                                   //所有存档构成的
const book methods[BOOK_NUM] = { book_0,book_1,book_2,book_3,book_4 };                                                                 //所有的功法
const pill pills[PILL_NUM] = { pill_0,pill_1,pill_2,pill_3,pill_4,pill_5,pill_6,pill_7,pill_8,pill_9 };                                         //所有的丹药
const Event events[EVENT_NUM] = { event_0,event_1,event_2,event_3,event_4,event_5,event_6,event_7,event_8,event_9,event_10,event_11,event_12,event_13,event_14,event_15,event_16, event_17,event_18,event_19 };                                      //所有的事件
const EventMap maps[MAP_NUM] = { map_0,map_1,map_2,map_3,map_4,map_5,map_6,map_7,map_8,map_9,map_10,map_11,map_12,map_13,map_14,map_15,map_16,map_17,map_18,map_19 };
const GachaCard cards[CARD_NUM] = {                                      //所有的卡牌
    {1,0},
    {1,1},
    {1,2},//以上为稀有度为0的物品
    {1,3},
    {1,4},
    {1,5},
    {0,1},
    {0,2},//以上为稀有度为1的物品
    {1,6},
    {1,7},
    {1,8},
    {1,9},
    {0,3},
    {0,4},//以上为稀有度为2的物品
};
const level realms[LEVEL_NUM] = {                                        //所有等级
    {"练气 初期",100,1},
    {"练气 中期",200,2},
    {"练气 巅峰",300,3},
    {"筑基 初期",1000,4},
    {"筑基 中期",2000,5},
    {"筑基 巅峰",3000,6},
    {"金丹 初期",4000,7},
    {"金丹 中期",5000,8},
    {"金丹 巅峰",6000,9},
    {"元婴 初期",7000,10},
    {"元婴 中期",8000,11},
    {"元婴 巅峰",9000,12},
    {"化神 初期",10000,13},
    {"化神 中期",20000,14},
    {"化神 巅峰",30000,15},
    {"炼虚 初期",40000,16},
    {"炼虚 中期",50000,17},
    {"炼虚 巅峰",60000,18},
    {"合体 初期",70000,19},
    {"合体 中期",80000,20},
    {"合体 巅峰",90000,21},
    {"大乘 初期",100000,22},
    {"大乘 中期",200000,23},
    {"大乘 巅峰",300000,24},
    {"渡劫 初期",400000,25},
    {"渡劫 中期",500000,26},
    {"渡劫 巅峰",600000,27},
    {"真仙 初期",700000,28},
    {"真仙 中期",800000,29},
    {"真仙 巅峰",900000,30},
    {"大罗金仙",0,0}
};
IMAGE frames[223] = { 0 };                                          //提前读入的抽卡动画
//视频全局
IMAGE img_menu[645];
IMAGE vdo_role[505];
int img_p ;
clock_t lst_frames;


int main() {
    //easyx相关设置
    initgraph(640, 480, EX_SHOWCONSOLE | EX_DBLCLKS);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();
                                               /*------资源预先载入------*/   //********
    inputBGM();                                 //提前加载播放的音乐
    changeBGM();                                //更改音量                
    mciSendString("play bgm_menuitf repeat", NULL, 0, NULL);                                      
	showloading();                                                  //***随机显示加载界面***
    settextcolor(BLACK);
    outtextxy(180, 400, "正在加载游戏资源，请稍后.............");

    loadvedio();                                //加载视频
    inputAnimation();                           //提前读入的抽卡动画   ***动画/动图***
    loadfile();                                 //加载游戏数据
                                               /*------资源载入完成------*/
    logonInterface();                           //跳转到登录界面
    return 0;
}
