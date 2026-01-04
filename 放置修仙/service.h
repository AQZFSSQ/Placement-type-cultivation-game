#pragma once
#include<iostream>
#include<string>
#include "datastruct.h"
#include "tools.h"
#include<ctime>
#include"global.h"


/*
    负责人：chen
    功能：根据BGM修改BGM音量
    参数：void
    返回值：void
*/
void changeBGM();
void inputBGM();
void drawSimpleDashedRect(int x1, int y1, int x2, int y2);
/*
    负责人:xdhking
    功能:  加载视频资源
    参数: void
    返回值:void
*/
void loadvedio();

//核心-------------------------------------------

/*
    负责人:花海
    功能:  判断cheatMode,如果值为1，初始化currRole为作弊数值；如果为false，进行下一步
           根据用户选择的存档，将存档内容赋值给当前游玩的玩家
           传入存档的编号将id赋值给currRole_id;
           然后将Role[id]赋值给全局的currRole
           初始化hang_up_alltime变量
    参数: int (存档编号)
    返回值:void
*/
void usefile(int id);

/*
    负责人:花海
    功能: 加载所有存档，并将所有存档放入全局vector<Magician> Role;
    参数:void
    返回值: void
*/
void loadfile();

/*
    负责人:花海
    功能: 创建一个新的修仙者结构体，修改角色的名字和性别，并放到全局Magician Role[]
          返回创建存档的编号
    参数: char* char*传入名字和姓名
    返回值: int
*/
int createfile(const char* name, const char* sex);

/*
    负责人:xdhing
    功能: 先获取当前时间存入currtime
          如果时间已经经过了一个挂机周期，即currtime-hang_up_time>=hang_up_priod
          则更新奖励池积累的灵石和经验，hang_up_stone+=get_stone_speed，hang_up_exp+=methods[currRole.uses_book_id].speed+ currRole.basic_speed;
          然后更新挂机时间：hang_up_time=currtime
    参数: void
    返回值: void
*/
void HangUp();

/*
    负责人:xdhing
    功能: 将所有的灵石hang_up_stone，和经验hang_up_exp加到currRole中后，再将hang_up_stone，hang_up_exp清零
          更新领取奖励的时间：hang_up_alltime=clock();
    参数: void
    返回值: void
*/
void get_hung_up_reward();

/*
	负责人:xdhing
    功能:  判断作弊者模式cheatMode的bool值，如果为true，return；
           将currRole更新到Role数组里面，方法：Role[currRole_id]=currRole
           然后将Role数组写入文档中保存
    参数: void
    返回值: void
*/
void savefile();

/*
    负责人:chen
    功能: 更新到下一个境界，并清空经验值和突破丹药服用数量
    参数: void
    返回值: void
*/
void upgrade_level();

/*
    负责人:chen
*    功能: 背包中该丹药的数量减一，并根据该丹药的加成修改角色信息        //功能: 判断丹药结构体变量中kind_flag成员的值，若值为0，调用breakthroughItf()；若为1,背包中该丹药的数量减一，并根据该丹药的加成修改角色信息
    参数: int 丹药的编号
    返回值: void
*/
void use_pill(int id);

/*
    负责人:chen
    功能: 切换功法，并修改角色属性
    参数: int 功法的编号
    返回值: void
*/
void switch_book(int id);

//核心-------------------------------------------


//非核心（抽卡）-------------------------------------------

/*
    负责人:Chen
    功能: 1.角色幸运值+1
          2.判断幸运值是否达到80满值。若幸运值已满，则幸运值归0，在生成随机数在4个稀有度为2的卡片中抽取一个，同时将该奖品放入背包，并return该卡片id（卡片id为cards数组中卡片的索引）
          3.抽取0~999的随机数，根据随机数及每个卡片掉落概率返回卡片id(SSR（稀有度为2）暴率为0.5%，SR(稀有度为1)10%,剩下为R)(例如，随机数为0~4，返回结果为九转金丹的卡片id),并将相关卡片放入背包
    参数: void
    返回值: int 返回值为抽出卡片的索引
*/
int get_card();

//非核心（抽卡）-------------------------------------------


//非核心（事件）-------------------------------------------

int get_step();
void triggerEvent(int pos, std::string& eventResult);


//非核心（事件）-------------------------------------------

