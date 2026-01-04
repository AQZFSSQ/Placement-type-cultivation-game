#pragma once
#include<iostream>
#include<easyx.h>
#include "tools.h"
#include "service.h"
#include"global.h"


using namespace std;
//核心-------------------------------------------

/*
    负责人:ted
    功能: 打印登录界面，依次绘制开始游戏，关于我们，退出游戏的按钮
          鼠标点击开始游戏时调用drawMenu();
          鼠标点击退出游戏时则return;
          鼠标点击关于我们则调用drawAboutUs();
    参数: void
    返回值: void
*/
void logonInterface();

/*
    负责人:H K
    功能: 将cheatMode初始化为false
          绘制创建存档按钮 点击跳转信息设置界面,调用setMessageItf()函数，如果返回值为 1 则调用drawGameItf()
          绘制读取存档按钮，点击调用fileItf(),跳转存档界面，如果返回值为 1 则调用drawGameItf()
          绘制设置按钮，点击跳转设置页面，调用setItf()，如果返回值为 1 则调用drawGameItf()
          绘制退出登录按钮 点击调用return;返回登陆界面
    参数: void
    返回值: void
*/
void drawMenu();

/*
    负责人:ted
    功能: 展示开发者信息
          返回按钮return（返回登陆界面）
    参数: void
    返回值: void
*/
void drawAboutUs();

/*
    负责人:ted
    功能: 显示操作说明（不可修改）
          鼠标拖动滑块调整音乐和音效大小
        作弊者模式：开启时可以进入作弊文档，作弊文档为固定单独文件
                    选定作弊模式可直接调用usefile(0)传输作弊存档，参数：0为作弊存档的编号
                    然后将cheatMode置为true
                    然后调用return 1;跳转到菜单页面
         返回按钮，点击后return 0;返回菜单
    参数: void
    返回值: int   返回为0表示不打开游戏主页，1为打开游戏主页
*/
int setItf();


/*
    负责人:弈
    功能: 信息设置窗口函数应包含以下功能
          返回菜单按钮,return 0;
          用户输入姓名
          选择性别按钮
          当用户输入姓名和选择性别后绘制开始游戏按钮，点击后调用createfile(&name,&sex)创建一个新的角色
          用int接收createfile的返回值id
          创建后调用usefile(id)传入新创建的存档id
          最后调用return 1;打开游戏主页
    参数: void
    返回值: int  返回为0表示不打开游戏主页，1为打开游戏主页
*/
int setMessageItf();

/*
    负责人:弈
    功能: 遍历全局的Role数组，显示存档的名称及编号（编号为所在Role数组的下标）
        绘制选择存档按钮，点击加载存档数据，并调用usefile(id)传入用户选择存档的编号,然后调用  return  1;
        绘制返回按钮，跳转回菜单界面 return 0;
    参数: void
    返回值: int 返回为0表示不打开游戏主页，1为打开游戏主页
*/
int fileItf();

/*
    负责人:寂夜
    功能: 每次循环调用HangUp(),挂机函数，时刻更新挂机奖励池
          展示全局currRole的信息(灵石，名称，经验值),并展示人物形象
          应绘制一下按钮：
          修仙奇遇（事件），点击调用eventItf();
          祈愿（抽卡），点击调用drawingCardItf();
          突破，点击调用breakthroughItf();
          背包，点击调用bagItf();
          保存游戏，点击调用savefile();
          返回菜单，点击调用savefile(),return
          挂机奖励:点击调用drawWindow(char*);传参为字符串"挂机时长xxx,你获得了\n灵石*hang_up_stone\nEXP*hang_up_exp"
                   然后再调用get_hung_up_reward();
                   挂机时长的计算：先获取当前时间存入currtime，挂机时长为（currtime-hang_up_alltime）/1000,单位为秒
    参数: void
    返回值: void
*/
void drawGameItf();


/*
    负责人:流水
    功能: （通用弹窗）绘制文本提示弹窗，将文本打印
          弹窗下绘制确认按钮，点击返回
    参数: void
    返回值: void
*/
void drawWindow(const char* text);


/*
    负责人:H K
    功能: 判断是否达到突破条件，usePillnum的数量是否足够，经验数量是否足够
           绘制是否突破成功或者失败
                突破条件达到调用upgrade_level(),然后展示当前境界
                如果条件没达到，打印什么条件没满足
        绘制确认按钮，点击return
    参数: void
    返回值: void
*/
void breakthroughItf();

/*
    负责人:流水
    功能: 绘制背包界面，包含灵石数量，各物品展示
          鼠标放在物品上会显示信息和数量，点击丹药调用use_pill(int)传入丹药的编号，点击功法调用switch_book(int)传入功法的编号
          根据功法/丹药不同等级，名称为不同颜色（0 白色 1 紫色 2 红色）
          绘制返回按钮
    参数: void
    返回值: void
*/
void bagItf();

//核心-------------------------------------------


//非核心（抽卡系统）-------------------------------------------

/*
    负责人：Ted
    功能：while(1){
            1.打印卡池界面。界面包括卡池信息（展示奖池物品及爆率：SSR（稀有度为2）暴率为0.5%，SR(稀有度为1)10%,剩下为R），贴图，幸运值。以及三个按钮，返回，祈愿一次（标明300灵石），抽十发（标明2850灵石）。
            2.接收用户信息
            3.根据用户分析调用函数：
                返回调用return
                祈愿一次，调用drawOnceView()
                祈愿十次，调用drawTenView()
            }

    参数：void
    返回值：void


*/
void drawingCardItf();

/*
    负责人：chen
    功能：调用后将抽卡动画信息读入到代码中
    参数：void
    返回值：void
*/
void inputAnimation();

/*
    负责人：smokky,chen
    功能： 调用抽卡动画函数showAnimation()
           调用一次抽卡函数int get_card();返回值为该卡片在卡池数组cards中的索引
           将抽卡结果显示在弹窗中
           弹窗中有确认按钮，关闭弹窗

    参数：void
    返回值：void
*/
void drawOnceView();

/*
    负责人：smokky, chen
    功能： 调用抽卡动画函数showAnimation()
           调用十次抽卡函数int get_card();返回值为该卡片在卡池数组cards中的索引
           将抽卡结果显示在弹窗中
           弹窗中有确认按钮，关闭弹窗

    参数：void
    返回值：void
*/
void drawTenView();

/*
    负责人：chen
    功能：展示抽卡动画，展示完自动退出
    参数：void
    返回值：void
*/
void showAnimation();

/*
    得到随机数，放置图片
*/
void showloading();

//非核心（抽卡系统）-------------------------------------------
void eventItf();
