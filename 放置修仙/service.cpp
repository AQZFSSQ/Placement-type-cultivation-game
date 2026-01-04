#pragma once
#include<iostream>
#include<easyx.h>
#include "datastruct.h"
#include<ctime>
#include "global.h"
#include<fstream>
#include "service.h"
#include <random>
using namespace std;


void changeBGM() {
    char order_BGM[100];
    sprintf_s(order_BGM, "setaudio bgm_Animation volume to %d", BGM * 100);
    mciSendString(order_BGM, NULL, 0, NULL);
    sprintf_s(order_BGM, "setaudio bgm_drawingCardItf volume to %d", BGM * 100);
    mciSendString(order_BGM, NULL, 0, NULL);
    sprintf_s(order_BGM, "setaudio bgm_drawGameItf volume to %d", BGM * 100);
    mciSendString(order_BGM, NULL, 0, NULL);
    sprintf_s(order_BGM, "setaudio bgm_eventItf volume to %d", BGM * 100);
    mciSendString(order_BGM, NULL, 0, NULL);
    sprintf_s(order_BGM, "setaudio bgm_menuitf volume to %d", BGM * 100);
    mciSendString(order_BGM, NULL, 0, NULL);
}
void inputBGM() {
    mciSendString("open \".\\music\\CardVedio.mp3\" alias bgm_Animation", NULL, 0, NULL);
    mciSendString("open \".\\music\\抽卡界面BGM.mp3\" alias bgm_drawingCardItf", NULL, 0, NULL);
    mciSendString("open \".\\music\\游戏主界面BGM.mp3\" alias bgm_drawGameItf", NULL, 0, NULL);
    mciSendString("open \".\\music\\事件界面BGM.mp3\" alias bgm_eventItf", NULL, 0, NULL);
    mciSendString("open \".\\music\\menuitf.mp3\" alias bgm_menuitf", NULL, 0, NULL);
}
/*
    负责人:xdhking
    功能:  加载视频资源
    参数: void
    返回值:void
*/

void loadvedio() {
    char path[100];
    for (int i = 0; i <= 645; i++) {
        string s = to_string(i);
        while (s.size() < 5) {
            s = '0' + s;
        }
        sprintf_s(path, "pic\\MenuItf\\%s.jpg", s.c_str());
        loadimage(img_menu + i, path, 640, 480);
    }
}
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
void usefile(int id) {
   //这里文件已经加载进来了，不需要再对文件操作
    currRole_id = id;
    currRole = Role[id];
    cout << currRole.name<<" 打开成功" <<'\n';
    hang_up_alltime = clock();
}

/*
    负责人:花海
    功能: 加载所有存档，并将所有存档放入全局vector<Magician> Role;
    参数:void
    返回值: void
*/
void loadfile() {
    ifstream fin("bin\\rolefile.dat", ios::binary);
    if (fin.is_open()) {
        cout << "文件加载成功\n";
        fin.read((char*)&Role,sizeof(Role));
    }
    fin.close();
}

/*
    负责人:花海
    功能: 创建一个新的修仙者结构体，修改角色的名字和性别，并放到全局Magician Role[]
          返回创建存档的编号
    参数: char* char*传入名字和姓名
    返回值: int
*/
int createfile(const char* name, const char* sex) {
    Magician newMagician;
    strncpy_s(newMagician.name, name, sizeof(newMagician.name) - 1);
    newMagician.name[sizeof(newMagician.name) - 1] = '\0';
    strncpy_s(newMagician.sex, sex, sizeof(newMagician.sex) - 1);
    newMagician.sex[sizeof(newMagician.sex) - 1] = '\0';
    newMagician.basic_speed = 1.0; // 初始根骨速率
    newMagician.EXP = 0.0;          // 初始经验值
    newMagician.stone = 10000.0;        // 初始灵石
    newMagician.furtuValue = 0;     // 初始幸运值
    newMagician.event_point = 0;    // 初始事件位置
    newMagician.level_id = 0;       // 初始境界ID
    newMagician.uses_book_id = 0;  // 初始未使用功法
    newMagician.useBag = { {1,0,0,0,0}, { 10,0,0,0,0,0,0,0,0,0 } };        // 初始化背包
    // 将新角色添加到全局角色列表中
    for (int i = 0; i < 6; i++)
    {
        if (Role[i].sex[0] == '\0') { // 找到第一个空位
            Role[i] = newMagician; // 将新角色放入空位
            return i;
        }
    }
    return 0;
}

/*
    负责人:xdhing
    功能: 先获取当前时间存入currtime
          如果时间已经经过了一个挂机周期，即currtime-hang_up_time>=hang_up_priod
          则更新奖励池积累的灵石和经验，hang_up_stone+=get_stone_speed，hang_up_exp+=methods[currRole.uses_book_id].speed+ currRole.basic_speed;
          然后更新挂机时间：hang_up_time=currtime
    参数: void
    返回值: void
*/
void HangUp() {
    clock_t currtime = clock();
    if (currtime - hang_up_time >= hang_up_priod) {
        hang_up_stone += get_stone_speed;
        hang_up_exp += methods[currRole.uses_book_id].speed + currRole.basic_speed;
        hang_up_time = currtime;
    }
}

/*
    负责人:xdhing
    功能: 将所有的灵石hang_up_stone，和经验hang_up_exp加到currRole中后，再将hang_up_stone，hang_up_exp清零
          更新领取奖励的时间：hang_up_alltime=clock();
    参数: void
    返回值: void
*/
void get_hung_up_reward() {
    currRole.stone += hang_up_stone;
    currRole.EXP += hang_up_exp;
    hang_up_stone = hang_up_exp = 0;
    hang_up_alltime = clock();
}

/*
    负责人:xdhing
    功能:  判断作弊者模式cheatMode的bool值，如果为true，return；
           将currRole更新到Role数组里面，方法：Role[currRole_id]=currRole
           然后将Role数组写入文档中保存
    参数: void
    返回值: void
*/
void savefile() {
    if (cheatMode) return;
    Role[currRole_id] = currRole;
    ofstream fout;
    fout.open("bin\\rolefile.dat", ios::out | ios::binary);
    if (fout.is_open()) {
        fout.write((char*)&Role, sizeof(Role));
    }
    fout.close();
}

/*
    负责人:chen
    功能: 更新到下一个境界，并更改经验值和突破丹药服用数量
    参数: void
    返回值: void
*/
void upgrade_level() {
    if (currRole.level_id == LEVEL_NUM - 1) return;                         //判断是否满级
    currRole.EXP -= realms[currRole.level_id].expRequired;                  //扣除需要的经验值，保留剩余经验值
    currRole.useBag.p[0] -= realms[currRole.level_id].pillRequired;         //在角色背包中扣除需要的经验丹
    currRole.level_id += 1;                                                 //角色等级+1
    return;
}

/*
    负责人:chen
    功能: 判断丹药结构体变量中kind_flag成员的值，若值为0，调用upgrade_level()；若为1,背包中该丹药的数量减一，并根据该丹药的加成修改角色信息
    参数: int 丹药的编号
    返回值: void
*/
void use_pill(int id) {
    if (!pills[id].kind_flag) {                                             //判断是否为突破丹药，如果是，执行突破程序
        if (currRole.EXP >= realms[currRole.level_id].expRequired && currRole.useBag.p[0] >= realms[currRole.level_id].pillRequired) upgrade_level();
        return;
    }
    if (currRole.useBag.p[id] <= 0) return;
    currRole.useBag.p[id]--;                                                //背包内该类型丹药数量-1
    currRole.level_id += pills[id].level_up;                                //实现丹药等级增加功能
    if (currRole.level_id >= LEVEL_NUM) currRole.level_id = LEVEL_NUM - 1;  //如果等级超出上限，将等级设置为最大等级  
    currRole.EXP += pills[id].EXP_up;                                       //实现丹药经验增加功能
    currRole.basic_speed += pills[id].speed_up;                             //实现丹药根骨增加功能
    return;
}

/*
    负责人:chen
    功能: 切换功法，并修改角色属性
    参数: int 功法的编号
    返回值: void
*/
void switch_book(int id) {
    currRole.uses_book_id = id;                         //更改功法ID
    return;
}



/*
    负责人:chen
    功能: 1.角色幸运值+1
          2.判断幸运值是否达到80满值。若幸运值已满，则幸运值归0，在生成随机数在6个稀有度为2的卡片中抽取一个，同时将该奖品放入背包，并return该卡片id（卡片id为cards数组中卡片的索引）
          3.抽取0~999的随机数，根据随机数及每个卡片掉落概率返回卡片id(SSR（稀有度为2）暴率为0.5%，每张SR(稀有度为1)2%,剩下为R),并将相关卡片放入背包
    参数: void
    返回值: int 返回值为抽出卡片的索引
*/
int get_card() {
    random_device rd;       //获取随机数种子
    mt19937 gen(rd());      //初始化随机数变量
    int randomInt;

    currRole.furtuValue++;
    if (currRole.furtuValue == 80) {
        uniform_int_distribution<> distrib(8, 13);
        randomInt = distrib(gen);
        currRole.furtuValue = 0;
        if (cards[randomInt].card_flag)currRole.useBag.p[cards[randomInt].id]++;
        else currRole.useBag.b[cards[randomInt].id]++;
        return randomInt;
    }

    uniform_int_distribution<> distrib(0, 999);
    randomInt = distrib(gen);
    if (randomInt >= 0 && randomInt < 5) { currRole.useBag.p[cards[8].id]++; currRole.furtuValue = 0;return 8; }
    else if (randomInt >= 5 && randomInt < 10) { currRole.useBag.p[cards[9].id]++;currRole.furtuValue = 0;return 9; }
    else if (randomInt >= 10 && randomInt < 15) { currRole.useBag.p[cards[10].id]++;currRole.furtuValue = 0;return 10; }
    else if (randomInt >= 15 && randomInt < 20) { currRole.useBag.p[cards[11].id]++;currRole.furtuValue = 0;return 11; }
    else if (randomInt >= 20 && randomInt < 25) { currRole.useBag.b[cards[12].id]++;currRole.furtuValue = 0;return 12; }
    else if (randomInt >= 25 && randomInt < 30) { currRole.useBag.b[cards[13].id]++;currRole.furtuValue = 0;return 13; }
    else if (randomInt >= 30 && randomInt < 50) { currRole.useBag.p[cards[3].id]++;return 3; }
    else if (randomInt >= 50 && randomInt < 70) { currRole.useBag.p[cards[4].id]++;return 4; }
    else if (randomInt >= 70 && randomInt < 90) { currRole.useBag.p[cards[5].id]++;return 5; }
    else if (randomInt >= 90 && randomInt < 110) { currRole.useBag.b[cards[6].id]++;return 6; }
    else if (randomInt >= 110 && randomInt < 130) { currRole.useBag.b[cards[7].id]++;return 7; }
    else if (randomInt >= 130 && randomInt < 420) { currRole.useBag.p[cards[0].id]++;return 0; }
    else if (randomInt >= 420 && randomInt < 710) { currRole.useBag.p[cards[1].id]++;return 1; }
    else if (randomInt >= 710 && randomInt < 1000) { currRole.useBag.p[cards[2].id]++;return 2; }
}
/*
    负责人:花海
    功能: 扣除灵石，并且返回步数（1——6）
    参数: void
    返回值: int 返回步数
*/
int get_step()
{
    if (currRole.stone < 500)
    {
        return -1;
    }
    else {
        srand(time(0)); // 设置随机数种子
        int step = rand() % 6 + 1; // 随机生成1到6的整数
        currRole.stone -= 500; // 扣除500灵石
        return step; // 返回掷骰子的结果
    }
}

void triggerEvent(int pos, std::string& eventResult) {
    // 事件描述拼接
    eventResult.clear();
    auto& ev = events[maps[pos].event_id];
    auto& resultp = pills[ev.event_pill].name;
    auto& resultb = methods[ev.event_book].name;
    eventResult += ev.event_title + "\n";
    eventResult += ev.event_intro + "\n";
    switch (ev.event_flag) {
    case 0: // 经验/等级
        currRole.EXP += ev.event_EXP;
        currRole.level_id += ev.event_level;
        eventResult += "获得经验: " + std::to_string((int)ev.event_EXP) + "\n";
        eventResult += "提升等级: " + std::to_string((int)ev.event_level) + "\n";
        break;
    case 1: // 经验/根骨/功法
        currRole.EXP += ev.event_EXP;
        currRole.basic_speed += ev.event_speed;
        currRole.useBag.b[ev.event_book]++;
        eventResult += "获得功法: " + resultb + "\n";
        break;
    case 2: // 丹药
        currRole.useBag.p[ev.event_pill] += ev.event_pill_num;
        eventResult += "获得丹药: " + resultp + " * " + std::to_string(ev.event_pill_num) + "\n";
        break;
    case 3: // 前进
        eventResult += "前进" + std::to_string(ev.event_move) + "格\n";
        currRole.event_point = (currRole.event_point + ev.event_move + 20) % 20;
        triggerEvent(currRole.event_point, eventResult); // 递归触发新事件
        break;
    case 4: // 后退
        eventResult += "后退" + std::to_string(ev.event_move) + "格\n";
        currRole.event_point = (currRole.event_point - ev.event_move + 20) % 20;
        triggerEvent(currRole.event_point, eventResult); // 递归触发新事件
        break;
    default:
        eventResult += "无特殊事件\n";
        break;
    }
}

void drawSimpleDashedRect(int x1, int y1, int x2, int y2)
{
    setlinestyle(PS_SOLID, 1);
    // 设置线条颜色 
    setlinecolor(WHITE);
    for (int y = y1; y <= y2; y += 2) {
        line(x1, y, x2, y);
    }
    for (int x = x1;x <= x2;x += 2) {
        line(x, y1, x, y2);
    }
}