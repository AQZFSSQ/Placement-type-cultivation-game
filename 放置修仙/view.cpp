#pragma once
#include<iostream>
#include<easyx.h>
#include "datastruct.h"
#include"service.h"
#include"global.h"
#include "tools.h"
#include "view.h"
#include<ctime>
#include<conio.h>//需要接受键盘输入时的_getch()与_kbhit()函数（目前是这样）
#include<Windows.h>
#include <mmsystem.h>   
#pragma comment(lib, "winmm.lib")
#include <string>
#include <random>

using namespace std;


//--------------弈------------------------------------

/*
    负责人:弈
    功能: 突破界面，展示当前境界，突破条件（经验值和丹药），点击突破按钮调用breakthroughItf()函数
          绘制返回按钮，点击返回菜单
    参数: void
    返回值: void
*/
void breakthroughItf() {
    if (currRole.level_id == 30) {
        drawWindow("已经达到最高境界：\n大罗金仙");
        return;
    }
    // 是否满足突破条件
    bool can = (currRole.EXP >= realms[currRole.level_id].expRequired &&
        currRole.useBag.p[0] >= realms[currRole.level_id].pillRequired);
    // 消息
    string msg;
    if (can) {
        upgrade_level();//突破
        msg = "突破成功！\n新境界: " + realms[currRole.level_id].name +      ////突破成功消息
            "\n消耗经验: " + to_string((int)realms[currRole.level_id - 1].expRequired) +
            "\n消耗丹药: " + to_string(realms[currRole.level_id - 1].pillRequired);
        drawWindow(msg.c_str());// 显示结果
        savefile(); //保存进度
    }
    else {
        msg = "突破条件不足!\n";
        if (currRole.EXP < realms[currRole.level_id].expRequired) {
            msg += "经验不足: \n需要" + to_string((int)realms[currRole.level_id].expRequired) +
                ",当前" + to_string(currRole.EXP) + "\n";
        }
        if (currRole.useBag.p[0] < realms[currRole.level_id].pillRequired) {
            msg += "突破丹药不足: \n需要" + to_string(realms[currRole.level_id].pillRequired) +
                ",当前" + to_string(currRole.useBag.p[0]);
        }
        drawWindow(msg.c_str());
    }
}

// 文本框结构体
struct TextBox {
    int x, y, width, height;
    string text;
    bool isActive;
    bool showCursor;
    DWORD lastCursorTime;
};

// 初始化文本框
void initTextBox(TextBox& box, int x, int y, int width, int height) {
    box.x = x;
    box.y = y;
    box.width = width;
    box.height = height;
    box.text = "";
    box.isActive = false;
    box.showCursor = false;
    box.lastCursorTime = GetTickCount();
}

// 绘制文本框
void drawTextBox(const TextBox& box) {
    // 绘制边框
    setfillcolor(box.isActive ? RGB(229, 241, 251) : WHITE);
    fillrectangle(box.x, box.y, box.x + box.width, box.y + box.height);
    setlinecolor(box.isActive ? RGB(0, 120, 215) : RGB(122, 122, 122));
    rectangle(box.x, box.y, box.x + box.width, box.y + box.height);

    // 绘制文本
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    RECT rect = { box.x + 5, box.y, box.x + box.width - 5, box.y + box.height };
    drawtext(box.text.c_str(), &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 绘制光标
    if (box.isActive && box.showCursor) {
        int cursorX = box.x + 5 + textwidth(box.text.c_str());
        line(cursorX, box.y + 5, cursorX, box.y + box.height - 5);
    }
}

// 更新文本框光标状态
void updateTextBoxCursor(TextBox& box) {
    DWORD currentTime = GetTickCount();
    if (currentTime - box.lastCursorTime >= 500) {
        box.showCursor = !box.showCursor;
        box.lastCursorTime = currentTime;
    }
}

// 处理文本框输入
void processTextBoxInput(TextBox& box) {
    ExMessage msg;
    while (peekmessage(&msg, EX_CHAR)) {
        if (msg.message == WM_CHAR && box.isActive) {
            // 普通字符
            if (msg.ch >= 32 && msg.ch <= 126) {
                box.text += (char)msg.ch;
            }
            // 退格键
            else if (msg.ch == 8 && !box.text.empty()) {
                box.text.pop_back();
            }
            box.showCursor = true;
            box.lastCursorTime = GetTickCount();
        }
    }
}

// 检查是否点击了文本框
bool checkTextBoxClick(TextBox& box, int mouseX, int mouseY) {
    bool clicked = (mouseX >= box.x && mouseX <= box.x + box.width &&
        mouseY >= box.y && mouseY <= box.y + box.height);
    box.isActive = clicked;
    if (clicked) {
        box.showCursor = true;
        box.lastCursorTime = GetTickCount();
    }
    return clicked;
}

/*
    负责人:弈
    功能: 信息设置窗口函数应包含以下功能
          返回菜单按钮,return 0;
          用户输入姓名
          选择性别按钮
          当用户输入姓名和选择性别后绘制开始游戏按钮，点击后调用creatfile(&name,&sex)创建一个新的角色
          创建后调用usefile(Role.size-1)传入新创建的存档id
          最后调用return 1;打开游戏主页
    参数: void
    返回值: int  返回为0表示不打开游戏主页，1为打开游戏主页
*/
int setMessageItf() {

    // 性别选择按钮状态
    bool maleSelected = false;
    bool femaleSelected = false;

    // 初始化文本框
    TextBox nameTextBox;
    initTextBox(nameTextBox, 170, 300, 250, 40);

    // 主循环
    ExMessage msg;
    
    int returnValue = -1;
    bool shouldExit = false;

    while (!shouldExit) {
        BeginBatchDraw();

        // 处理消息
        while (peekmessage(&msg, EX_MOUSE | EX_CHAR)) {

            switch (msg.message) {
            case WM_MOUSEMOVE:
                break;

            case WM_LBUTTONDOWN:
                // 检查是否点击了文本框
                checkTextBoxClick(nameTextBox, msg.x, msg.y);

                // 检查性别选择按钮
                if (inArea(msg.x, msg.y, 150, 210, 100, 40)) {
                    maleSelected = true;
                    femaleSelected = false;
                }
                if (inArea(msg.x, msg.y, 350, 210, 100, 40)) {
                    femaleSelected = true;
                    maleSelected = false;
                }

                // 开始游戏按钮
                if (inArea(msg.x, msg.y, 200, 360, 200, 50) &&
                    !nameTextBox.text.empty() && (maleSelected || femaleSelected)) {
                    // 创建角色文件
                    if (maleSelected) {
                         usefile(createfile((nameTextBox.text.c_str()), "男"));
                    }
                    else {
                         usefile(createfile((nameTextBox.text.c_str()), "女"));
                    }

                    returnValue = 1;
                    shouldExit = true;
                }

                // 返回按钮
                if (inArea(msg.x, msg.y, 200, 415, 200, 50)) {
                    returnValue = 0;
                    shouldExit = true;
                }
                break;

            case WM_CHAR:
                // 文本框输入处理
                if (nameTextBox.isActive) {
                    // 普通字符
                    if (msg.ch >= 32 && msg.ch <= 126) {
                        nameTextBox.text += (char)msg.ch;
                    }
                    // 退格键
                    else if (msg.ch == 8 && !nameTextBox.text.empty()) {
                        nameTextBox.text.pop_back();
                    }
                    nameTextBox.showCursor = true;
                    nameTextBox.lastCursorTime = GetTickCount();
                }
                break;
            }
        }

        // 更新文本框光标状态
        if (nameTextBox.isActive) {
            updateTextBoxCursor(nameTextBox);
        }

        // 绘制界面
        cleardevice();

        //播放视频
        clock_t now = clock();
        if (now - lst_frames >= 16) {
            img_p++;
            lst_frames = now;
            if (img_p == 645) {
                img_p = 0;
            }
        }
        putimage(0, 0, img_menu + img_p);
        settextstyle(40, 0, "黑体", 0, 0, 10, 0, 0, 0);
        outtextxy(320-textwidth("角色设置")/2, 50, "角色设置");
        settextstyle(20, 0, "黑体", 0, 0, 10, 0, 0, 0);
        outtextxy(50, 310, "角色名称:");
        drawTextBox(nameTextBox);


        outtextxy(50, 220, "选择性别:");
        button(150, 210, 100, 40, "男", msg);
        button(350, 210, 100, 40, "女", msg);


        if (maleSelected) {
            outtextxy(150, 260, "已选择: 男");
        }
        else if (femaleSelected) {
            outtextxy(350, 260, "已选择: 女");
        }

        if (!nameTextBox.text.empty() && (maleSelected || femaleSelected)) {
            button(200, 360, 200, 50, "开始游戏", msg);
        }

        button(200, 415, 200, 50, "返回菜单", msg);

        FlushBatchDraw();
        Sleep(10);
    }

    EndBatchDraw();
    return returnValue;
}
//---------------------------------
/*
    负责人:弈
    功能: 遍历全局的Role数组，显示存档的名称及编号（编号为所在Role数组的下标）
        绘制选择存档按钮，点击加载存档数据，并调用usefile(id)传入用户选择存档的编号,然后调用  return  1;
        绘制返回按钮，跳转回菜单界面 return 0;
    参数: void
    返回值: int 返回为0表示不打开游戏主页，1为打开游戏主页
*/
int fileItf() {
    bool flag = false;
    // 初始化界面
    cleardevice();
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    // 标题
    settextcolor(BLACK);
    //outtextxy(100, 50, "存档界面");
    //// 显示所有存档
    //for (int i = 1; i <=6; i++) {
    //    char roleInfo[200];
    //    sprintf_s(roleInfo, "存档%d: %s", i , Role[i].name);
    //    outtextxy(100, 100 + (i-1) * 50, roleInfo);
    //    // 绘制选择按钮
    //    if (button(400, 100 + (i-1) * 50, 100, 40, "进入", msg)) {
    //        usefile(i); // 加载选中存档
    //        msg.message = 0;
    //        return 1;   // 进入游戏主页
    //    }
    //}
    //// 返回按钮
    //if (button(200, 400, 150, 50, "返回", msg)) {
    //    return 0; // 返回菜单界面
    //}
    ////刷新
    //FlushBatchDraw();
    IMAGE img_tmp;
    loadimage(&img_tmp, "pic\\bk1.png", 640, 480);
    // 循环
    while (true) {
        BeginBatchDraw();
        cleardevice();
        putimage(0, 0, &img_tmp);
        if (peekmessage(&msg, EX_MOUSE)) {
            // 重新绘制界面
            outtextxy(100, 50, "存档界面");
            for (int i = 1; i <=6; i++) {
                char roleInfo[200];
                sprintf_s(roleInfo, "存档%d: %s", i , Role[i].name);
                outtextxy(100, 100 + (i-1) * 50, roleInfo);

                if (button(400, 100 + (i-1) * 50, 100, 40, "进入", msg)) {
                    if (Role[i].sex[0]!='\0') {
                        usefile(i);
                        EndBatchDraw();
                        cleardevice();

                        return 1;
                    }
                    else {
                        flag = true;
                    }
                }
                if (button(520, 100 + (i - 1) * 50, 100, 40, "删除", msg)) {
                    Magician tmp = {0};
                    Role[i] = tmp;
                    msg.message = 0;
                }
            }
            if (button(200, 400, 150, 50, "返回", msg)) {
                return 0;
            }
            EndBatchDraw();
        }
        if (flag) {
            flag = false;
            drawWindow("存档为空\n请创建新存档\n或者选其他存档");
        }
    }
    return 0;
}







//--------------------------------------------------------弈----------------------------------------

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
void drawGameItf()
{
    //开始播放音乐---------
    mciSendString("seek bgm_drawGameItf to start", NULL, 0, NULL);
    mciSendString("play bgm_drawGameItf repeat", NULL, 0, NULL);
    //开始播放音乐--------
    string a;
    int vSpace;
    string text_p;
    bool flag = false;
    clock_t last = clock();
    int p = 1;
    while (true)
    {
        HangUp();
        if (peekmessage(&msg, 1))
        {

        }
        BeginBatchDraw();
        cleardevice();
        clock_t now = clock();
        if (now - last >= 10) {
            p++;
            last = now;
            if (p == 505) {
                p = 1;
            }
        }
        putimage(0, 0, vdo_role + p);
        setfillcolor(WHITE);
        setlinecolor(BLACK);
        fillrectangle(10, 10, 190, 140);
        setfillcolor(YELLOW);
        settextstyle(20, 0, "SimSum");
        a = string("名称:") + currRole.name;
        vSpace = (40 - textheight(a.c_str())) / 2;
        outtextxy(30, 20 + vSpace, a.c_str());
        a = string("灵石:") + to_string(currRole.stone);
        vSpace = (40 - textheight(a.c_str())) / 2;
        outtextxy(30, 60 + vSpace, a.c_str());
        a = string("exp:") + to_string(currRole.EXP);
        vSpace = (40 - textheight(a.c_str())) / 2;
        outtextxy(30, 100 + vSpace, a.c_str());
        if (button(295, 400, 50, 40, "突破", msg))
        {
            msg.message = 0;
            //突破接口
            breakthroughItf();
        }
        if (button(420, 10, 50, 40, "背包", msg))
        {
            msg.message = 0;
            bagItf();
        }
        if (button(200, 10, 100, 40, "修仙奇遇", msg))
        {
            msg.message = 0;

            EndBatchDraw();
            showloading();
            settextcolor(BLACK);
            outtextxy(180, 390, "正在加载游戏资源，请稍后.............");
            //暂停音乐-----
            mciSendString("stop bgm_drawGameItf", NULL, 0, NULL);
            //暂停音乐-----
            mciSendString("seek bgm_eventItf repeat to start", NULL, 0, NULL);
            mciSendString("play bgm_eventItf repeat", NULL, 0, NULL);
            eventItf();
            mciSendString("stop bgm_eventItf", NULL, 0, NULL);
            //继续音乐-----
            mciSendString("resume bgm_drawGameItf", NULL, 0, NULL);
            //继续音乐-----
        }
        if (button(310, 10, 100, 40, "挂机奖励", msg))
        {
            msg.message = 0;
            char text[500];
            clock_t currtime = clock();
            int duration = static_cast<int>(currtime - hang_up_alltime) / 1000;
            hang_up_alltime = currtime;
            sprintf_s(text, "挂机时长%d秒,你获得了\n灵石%d\nEXP%d", duration, hang_up_stone, hang_up_exp);
            text_p = text;
            flag = true;
            get_hung_up_reward();
        }
        if (button(490, 10, 50, 40, "祈愿", msg))
        {
            msg.message = 0;
            //暂停音乐-----
            mciSendString("stop bgm_drawGameItf", NULL, 0, NULL);
            //暂停音乐-----
            drawingCardItf();
            //继续音乐-----
            mciSendString("resume bgm_drawGameItf", NULL, 0, NULL);
            //继续音乐-----
        }
        if (button(10, 420, 100, 40, "保存游戏", msg))
        {
            msg.message = 0;
             savefile();
        }
        if (button(520, 420, 100, 40, "返回菜单", msg))
        {
            msg.message = 0;
            if (!cheatMode)
            {
                 savefile();
            }
            //暂停音乐-----
            mciSendString("stop bgm_drawGameItf", NULL, 0, NULL);
            //暂停音乐-----
            return;
        }
        if (flag) {
            drawWindow(text_p.c_str());
            flag = false;
        }
        msg.message = 0;
        EndBatchDraw();
    }
}




//------------------------------------------------

/*
    负责人:ted
    功能: 打印登录界面，依次绘制开始游戏，关于我们，退出游戏的按钮
          鼠标点击开始游戏时调用drawMenu();
          鼠标点击退出游戏时则return;
          鼠标点击关于我们则调用drawAboutUs();
    参数: void
    返回值: void
*/
//用来判断鼠标是否在按钮的位置
bool isArea(int lx, int ly, int rx, int ry) {
    if (msg.x >= lx && msg.x <= rx && msg.y >= ly && msg.y <= ry) {
        return true;
    }
    return false;
}
//用于在登录界面来实现鼠标悬停变色效果
void logon_Botton(int lx, int ly, int rx, int ry) {
    if (isArea(lx, ly, rx, ry)) {
        setfillcolor(RGB(230, 231, 231));

    }
    else {
        setfillcolor(WHITE);
    }
    solidroundrect(lx, ly, rx, ry, 18, 18);

}


/*
    负责人:ted
    功能: 展示开发者信息
          返回按钮return（返回登陆界面）
    参数: void
    返回值: void
*/
void drawAboutUs() {
    while (true) {
        BeginBatchDraw();
        cleardevice();
        settextcolor(BLACK);
        //播放视频
        clock_t now = clock();
        if (now - lst_frames >= 16) {
            img_p++;
            lst_frames = now;
            if (img_p == 645) {
                img_p = 0;
            }
        }
        putimage(0, 0, img_menu + img_p);

        drawSimpleDashedRect(50, 80, 500, 300);//***半透明矩形框***

        settextstyle(40, 0, "微软雅黑");
        outtextxy(30, 10, "关于我们 ");

        settextstyle(21, 0, "黑体", 0, 0, 20, 0, 0, 0);
        string imformations[11] = { "花海-组长","流水 - 副组长兼监督官",
            "陆畏因-信息官","骁-产品经理","Smokky-产品经理","Chen-技术官",
             "寂夜-技术官","Hollow Knight-会议记录官",
           "Ted-产品经理","xdhking-技术官","弈-技术官" };

        for (int i = 0; i < 11; i++) {

            outtextxy(50 + (i % 2) * 170, 90 + 30 * (i / 2), imformations[i].c_str());

        }

        setlinecolor(RGB(230, 231, 231));
        setfillcolor(RGB(230, 231, 231));


        peekmessage(&msg, EX_MOUSE);

        //用来实现悬停变色
        if (isArea(30, 400, 130, 460)) {
            fillroundrect(30, 400, 130, 460, 30, 30);
        }
        roundrect(30, 400, 130, 460, 30, 30);
        outtextxy(80 - textwidth("返回") / 2, 430 - textheight("返回") / 2, "返回");

        EndBatchDraw();

        
        //返回登录
        if (msg.message == WM_LBUTTONDOWN && isArea(30, 40, 130, 460)) {
            //logonInterface();
            settextcolor(BLACK);
            return;
        }
    }
    settextstyle(25, 0, "微软雅黑", 0, 0, 0, 0, 0, 0);
}

/*
    负责人:ted
    功能: 显示操作说明（不可修改）
          鼠标拖动滑块调整音乐和音效大小
        作弊者模式：开启时可以进入作弊文档，作弊文档为固定单独文件
                    选定作弊模式可直接调用usefile(0)传输作弊存档，参数：0为作弊存档的编号
                    然后调用return 1;跳转到菜单页面
         返回按钮，点击后return 0;返回菜单
    参数: void
    返回值: int   返回为0表示不打开游戏主页，1为打开游戏主页
*/

//和上面的logon_Botton逻辑一模一样，因为设置和登录关系比较远，重命名了一个方便整合
//同样是为了方便悬停变色
void settings_Botton(int lx, int ly, int rx, int ry) {
    if (isArea(lx, ly, rx, ry)) {
        setfillcolor(RGB(230, 231, 231));
    }
    else {
        setfillcolor(WHITE);
    }
    solidroundrect(lx, ly, rx, ry, 18, 18);
}

//用滑块来展现调节音量的效果
void drawSlider(int lx, int ly, int rx, int ry, int& value, bool isDragging) {
    int thumbPos = 150 + value * 30;
    if (thumbPos < lx) thumbPos = lx;
    if (thumbPos > rx) thumbPos = rx;


    setfillcolor(RGB(200, 200, 200));
    solidrectangle(lx, ly, rx, ry);


    setfillcolor(isDragging ? RGB(150, 180, 255) : RGB(180, 200, 255));
    int thumbSize = (ry - ly) * 2;
    solidroundrect(
        thumbPos - thumbSize / 2, ly - (thumbSize - (ry - ly)) / 2,
        thumbPos + thumbSize / 2, ry + (thumbSize - (ry - ly)) / 2,
        10, 10
    );
}


int setItf() {
    int returnCode = 0;
    bool isDraggingVol = false;
    bool isDraggingBgm = false;

    const char* HELP_TEXT[] = {
        "操作说明：",
        "1. 拖动滑块调整音乐和音效大小",
        "2. 点击按钮启用作弊者模式"
    };

    while (true) {
        BeginBatchDraw();
        cleardevice();

        //播放视频
        clock_t now = clock();
        if (now - lst_frames >= 16) {
            img_p++;
            lst_frames = now;
            if (img_p == 645) {
                img_p = 0;
            }
        }
        putimage(0, 0, img_menu + img_p);

        setlinecolor(BLACK);
        settextcolor(BLACK);
        drawSimpleDashedRect(30, 60, 300, 120);
        settextstyle(40, 0, "微软雅黑");
        outtextxy(30, 10, "设置");

        settextstyle(25, 0, "微软雅黑");
        for (int i = 0; i < 3; i++) {
            outtextxy(30, 60 + i * 20, HELP_TEXT[i]);
        }


        settextstyle(20, 0, "微软雅黑");
        outtextxy(50, 150, "音效");
        drawSlider(150, 155, 450, 175, volume, isDraggingVol);

        outtextxy(50, 200, "背景音乐");
        drawSlider(150, 205, 450, 225, BGM, isDraggingBgm);

        roundrect(200, 250, 350, 280, 5, 5);
        settings_Botton(200, 250, 350, 280);
        outtextxy(275 - textwidth("启用作弊") / 2, 265 - textheight("启用作弊") / 2,
            cheatMode ? "已启用作弊" : "启用作弊");

        roundrect(30, 350, 130, 390, 5, 5);
        settings_Botton(30, 350, 130, 390);
        outtextxy(80 - textwidth("返回") / 2, 365 - textheight("返回") / 2, "返回");

        EndBatchDraw();


        peekmessage(&msg, EX_MOUSE);

        if (msg.message == WM_LBUTTONDOWN) {

            int volThumb = 150 + volume * 30;
            if (isArea(volThumb - 40, 140, volThumb + 40, 180)) {
                isDraggingVol = true;
            }

            int bgmThumb = 150 + BGM * 30;
            if (isArea(bgmThumb - 40, 190, bgmThumb + 40, 230)) {
                isDraggingBgm = true;
            }

            if (isArea(200, 250, 350, 280)) {
                if (!cheatMode) {
                    cheatMode = true;
                    returnCode = 1;
                    usefile(0);
                    break;
                }
            }

            if (isArea(30, 350, 130, 390)) {
                returnCode = 0;
                //返回菜单
                break;
            }
        }
        //这里滑块位置来对应音量和音效大小，最多可以控制0~10
        if (isDraggingVol && msg.message == WM_MOUSEMOVE) {
            int newPos = msg.x;
            if (newPos < 150) newPos = 150;
            if (newPos > 450) newPos = 450;
            volume = (newPos - 150) / 30;
        }

        if (isDraggingBgm && msg.message == WM_MOUSEMOVE) {
            int newPos = msg.x;
            if (newPos < 150) newPos = 150;
            if (newPos > 450) newPos = 450;
            BGM = (newPos - 150) / 30;
        }

        if (msg.message == WM_LBUTTONUP) {
            isDraggingVol = false;
            isDraggingBgm = false;
        }
        changeBGM();
    }

    return returnCode;
}

//----------------------------------------------------
/*
    负责人:xdking
    功能: 打印登录界面，依次绘制开始游戏，关于我们，退出游戏的按钮
          鼠标点击开始游戏时调用drawMenu();
          鼠标点击退出游戏时则return;
          鼠标点击关于我们则调用drawAboutUs();
    参数: void
    返回值: void
*/
void logonInterface() {
	clock_t lst_frames = clock();//获取程序运行到现在的时间
    while (true) {
        peekmessage(&msg, EX_KEY | EX_MOUSE);
        BeginBatchDraw();
        cleardevice();
        //播放视频
        clock_t now = clock();       
		if (now - lst_frames >= 16) {// 每16毫秒更新一次图片  或者sleep(now-lst_frames);
            img_p++;                 
            lst_frames = now;
			if (img_p == 645) {   //循环播放视频
                img_p = 0;
            }
        }
		putimage(0, 0, img_menu + img_p); //通过数组下标确定播放的视频帧

        if (transparent_button(245,170,150,35,"开始游戏",msg)) {   //***透明动态按钮***
            msg.message = 0;
            drawMenu();
        }
        if (transparent_button(245, 270, 150, 35, "关于我们", msg)) {
            msg.message = 0;
			drawAboutUs();                                         //***绘制半透明矩形框***
        }
        if (transparent_button(245, 370, 150, 35, "退出游戏", msg)) {
            return;
        }
        EndBatchDraw();
        msg.message = 0;
    }
}

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
void drawMenu() {
    auto celect_role=[&]() {
        cleardevice();
        showloading();
        settextcolor(BLACK);
        outtextxy(180, 400, "正在加载人物资源，请稍后.............");
        char path[100];
        string s = currRole.sex[0] == -60 ? "male" : "female";  //判断存档性别 产生不同初始页面
        for (int i = 1; i <= 504; i++) {
            sprintf_s(path, "pic\\%svdo\\image%d.jpg", s.c_str(),i);
            loadimage(vdo_role+i, path, 640, 480);
        }
    };
    while (true) {
        peekmessage(&msg, EX_KEY | EX_MOUSE);
        BeginBatchDraw();
        cleardevice();

        //播放视频
        clock_t now = clock();
        if (now - lst_frames >= 16) {
            img_p++;
            lst_frames = now;
            if (img_p == 645) {
                img_p = 0;
            }
        }
        putimage(0, 0, img_menu + img_p);

        if (transparent_button(245, 60, 150, 35, "开始新游戏", msg)) {
            msg.message = 0;
            if (setMessageItf()) {
                celect_role();
                //停止音频--------
                mciSendString("stop bgm_menuitf", NULL, 0, NULL);
                //停止音频--------
                drawGameItf();
                //继续播放--------
                mciSendString("resume bgm_menuitf", NULL, 0, NULL);
                //继续播放--------
            }
        }
        if (transparent_button(245, 160, 150, 35, "游玩存档", msg)) {
            msg.message = 0;
            if (fileItf()) {
                celect_role();
                //停止音频--------
                mciSendString("stop bgm_menuitf", NULL, 0, NULL);
                //停止音频--------
                drawGameItf();
                //继续播放--------
                mciSendString("resume bgm_menuitf", NULL, 0, NULL);
                //继续播放--------
            }
        }
        if (transparent_button(245, 260, 150, 35, "设置", msg)) {
            msg.message = 0;
            if (setItf()) {
                celect_role();
                //停止音频--------
                mciSendString("stop bgm_menuitf", NULL, 0, NULL);
                //停止音频--------
                drawGameItf();
                //继续播放--------
                mciSendString("resume bgm_menuitf", NULL, 0, NULL);
                //继续播放--------
            }
            cheatMode = false;
        }
        if (transparent_button(245, 360, 150, 35, "退出登录", msg)) {
            msg.message = 0;
            return;
        }
        EndBatchDraw();
    }
}

/*
    负责人:流水
    功能: （通用弹窗）绘制文本提示弹窗，将文本打印
          弹窗下绘制确认按钮，点击返回
    参数: void
    返回值: void
*/
void drawWindow(const char* text) {
    auto tmp_button = [&](int x,int y,int w,int h,const char* text) {
        COLORREF init_text_color = gettextcolor();
        COLORREF init_fill_color = getfillcolor();
        if (inArea(msg.x, msg.y, x, y, w, h)) {
            setfillcolor(RGB(81, 180, 255));
            settextcolor(WHITE);
        }
        else {
            setfillcolor(WHITE);
            settextcolor(BLACK);
        }
        fillrectangle(x, y, x + w, y + h);
        int tx = x + (w - textwidth(text))/2;
        int ty = y + (h - textheight(text)) / 2;
        outtextxy(tx, ty , text);
        setfillcolor(init_fill_color);
        settextcolor(init_text_color);
        if (inArea(msg.x, msg.y, x, y, w, h) && msg.message == WM_LBUTTONDOWN) {
            return true;
        }
        return false;
        };
    while (true) {
        peekmessage(&msg, EX_MOUSE);
        
        BeginBatchDraw();
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 2);
        fillrectangle(220, 140, 220 + 200, 140 + 200);
        setlinestyle(PS_SOLID, 1);
        //outtextxy(230, 150, text);
        int i = 0, j = 0;
        string s;
        while (true) {
            s += text[i++];
            if (s[s.size() - 1] == '\n') {
                outtextxy(230, 150 + j * 30, s.c_str());
                s.clear();
                j++;
            }
            else if (s[s.size() - 1] == '\0') {
                outtextxy(230, 150 + j * 30, s.c_str());
                s.clear();
                break;
            }
        }
        if (tmp_button(280, 300, 80, 30, "本座知晓")) {
            return;
        }
        EndBatchDraw();
     }
}


/*
    负责人:流水
    功能: 绘制背包界面，包含灵石数量，各物品展示
          鼠标放在物品上会显示信息和数量，点击丹药调用use_pill(int)传入丹药的编号，点击功法调用switch_book(int)传入功法的编号
          根据功法/丹药不同等级，名称为不同颜色（0 白色 1 紫色 2 红色）
          绘制返回按钮
    参数: void
    返回值: void
*/
void bagItf() {
    auto tmp_window = [&](int mx,int my,int of,int id) {
        COLORREF clr[3] = { GREEN,MAGENTA,RED };
        int num;
        string name;
        int lev;
        string text;
        if (of) {
            num = currRole.useBag.b[id];
            name = methods[id].name;
            lev = methods[id].level;
            text = methods[id].introduction;
        }
        else {
            num = currRole.useBag.p[id];
            name = pills[id].name;
            lev = pills[id].level;
            text = pills[id].introduction;
        }
        int x, y;
        if (mx + 200 <= 640 && my + 200 <= 480) {
            x = mx;
            y = my;
        }
        else if (my + 200 <= 480 && mx - 200-70 >= 0) {
            x = mx-200-70;
            y = my;
        }
        else if (mx + 200 <= 640 && my - 200 -70>= 0) {
            x = mx;
            y = my - 200-70;
        }
        else {
            x = mx - 200 - 70;
            y = my - 200 - 70;
        }
        COLORREF init_clr = gettextcolor();
        settextcolor(clr[lev]);
        setfillcolor(WHITE);
        solidrectangle(x, y, x + 200, y + 200);
        outtextxy(x, y, name.c_str());
        y += 20;
        char tmp[100];
        sprintf_s(tmp, "剩余数量：%d", num);
        outtextxy(x, y, tmp);
        string s;
        for (int i = 0; i < text.size(); i++) {
            s += text[i];
            if (textwidth(s.c_str()) > 200) {
                s.pop_back();
                y += 20;
                outtextxy(x, y, s.c_str());
                s.clear();
                s += text[i];
            }
        }
        if (s.size()) {
            y += 20;
            outtextxy(x,y,s.c_str());
        }
        settextcolor(init_clr);
        };

    cleardevice();

    IMAGE img_pill[10];
    IMAGE img_pill_cover[10];
    IMAGE img_book[5];
    IMAGE img_book_cover[5];
    IMAGE img_bag;
    IMAGE img_role;
    int lev_imge;
    IMAGE img_stone[2];
    loadimage(&img_bag, "pic\\bag.png", 463, 480);
    if (currRole.level_id >= 0 && currRole.level_id <= 5) {
        lev_imge = 1;
    }
    else if (currRole.level_id >= 6 && currRole.level_id <= 14) {
        lev_imge = 2;
    }
    else if (currRole.level_id >= 15 && currRole.level_id <= 23) {
        lev_imge = 3;
    }
    else if (currRole.level_id >= 24 && currRole.level_id <= 30) {
        lev_imge = 4;
    }
    char path[100];
    string s = currRole.sex[0] == -60 ? "male" : "female";
    sprintf_s(path, "pic\\role\\%s%d.png", s.c_str(), lev_imge);
    loadimage(&img_role, path, 177, 266);
    loadimage(img_stone, "pic\\stone.png", 60, 60);
    loadimage(img_stone+1, "pic\\stone_cover.png", 60, 60);
    //加载图片
    
    for (int i = 0; i < 10; i++) {
        sprintf_s(path, "pic\\pill\\pill_%d.png", i);
        loadimage(img_pill + i, path,60,60);
        sprintf_s(path, "pic\\pill\\pill_cover_%d.png", i);
        loadimage(img_pill_cover + i, path, 60, 60);
    }
    for (int i = 0; i < 5; i++) {
        sprintf_s(path, "pic\\book\\book_%d.png", i);
        loadimage(img_book + i, path, 60, 60);
        sprintf_s(path, "pic\\book\\book_cover_%d.png", i);
        loadimage(img_book_cover + i, path, 60, 60);
    }

    //绘图
    int x, y;
    bool inThis = false;
    bool clickThis = false;
    int mx, my,len=60,of,id;
    while (true) {
        inThis = false;
        clickThis = false;
        peekmessage(&msg, EX_MOUSE | EX_KEY);
        int x = 185, y = 15,cnt=0;
        BeginBatchDraw();
        cleardevice();
        //绘制图标
        putimage(0, 215, &img_role);
        putimage(0, 214 - 70, img_stone + 1, NOTSRCERASE);
        putimage(0, 214 - 70, img_stone , SRCINVERT);
        sprintf_s(path, "：%d", currRole.stone);
        outtextxy(61, 214 - 60, path);
        sprintf_s(path, "EXP：%d", currRole.EXP);
        outtextxy(2, 214 - 60 - 60, path);
        sprintf_s(path, "境界：%s", realms[currRole.level_id].name.c_str());
        outtextxy(0, 214 - 60 - 60-20, path);
        sprintf_s(path, "功法：%s", methods[currRole.uses_book_id].name.c_str());
        outtextxy(0, 214 - 100, path);
        sprintf_s(path, "根骨：%.2f", currRole.basic_speed);
        outtextxy(0, 214 - 80, path);
        putimage(178, 0, &img_bag);
        if (transparent_button(20, 20, 100, 35, "返回主页",msg)) {
            msg.message = 0;
            return;
        }
        for (int i = 0; i < 10; i++) {
            if (currRole.useBag.p[i] == 0)
                continue;
            if (inArea(msg.x, msg.y, x, y, len, len)) {
                inThis = true;
                mx = x, my = y;
                of = 0;
                id = i;
                IMAGE tmp[2];
                sprintf_s(path, "pic\\pill\\pill_%d.png", i);
                loadimage(tmp , path, 70, 70);
                sprintf_s(path, "pic\\pill\\pill_cover_%d.png", i);
                loadimage(tmp + 1, path, 70, 70);
                putimage(x-10, y-10, tmp+1, NOTSRCERASE);
                putimage(x-10, y-10, tmp, SRCINVERT);
                if (msg.message == WM_LBUTTONDOWN) {
                    clickThis = true;
                }
            }
            else {
                putimage(x, y, img_pill_cover + i, NOTSRCERASE);
                putimage(x, y, img_pill + i, SRCINVERT);
            }
            x += 63;
            cnt++;
            if (cnt == 7) {
                x = 185;
                y += 68;
                cnt = 0;
            }
        }
        for (int i = 0; i < 5; i++) {
            if (currRole.useBag.b[i] == 0)
                continue;
            if (inArea(msg.x, msg.y, x, y, len, len)) {
                inThis = true;
                mx = x, my = y;
                of = 1;
                id = i;
                IMAGE tmp[2];
                sprintf_s(path, "pic\\book\\book_%d.png", i);
                loadimage(tmp, path, 70, 70);
                sprintf_s(path, "pic\\book\\book_cover_%d.png", i);
                loadimage(tmp + 1, path, 70, 70);
                putimage(x - 10, y - 10, tmp + 1, NOTSRCERASE);
                putimage(x - 10, y - 10, tmp, SRCINVERT);
                if (msg.message == WM_LBUTTONDOWN) {
                    clickThis = true;
                }
            }
            else {
                putimage(x, y, img_book_cover + i, NOTSRCERASE);
                putimage(x, y, img_book + i, SRCINVERT);
            }
            x += 63;
            cnt++;
            if (cnt == 7) {
                x = 185;
                y += 68;
                cnt = 0;
            }
        }
        if (inThis) {
            tmp_window(mx+70,my+70,of,id);
            if (clickThis) {
                if (of) {
                    msg.message = 0;
                    switch_book(id);
                }
                else {
                    msg.message = 0;
                    use_pill(id);
                }
            }
        }
        EndBatchDraw();
        msg.message = 0;
    }
}

//-----------------------------------------以下为抽卡函数-----------------------------------
void inputAnimation() {
    // 加载所有图片
    for (int i = 0; i < 223; i++) {
        char path[100];
        sprintf_s(path, "pic\\Cardvedio\\%04d.png", i);
        loadimage(&frames[i], path, 640, 480);  // 固定尺寸加载更快
    }
}

void showAnimation() {
    const int FRAME_DELAY = 33;
    const int AUDIO_DELAY_MS = 200;

    // 播放音乐
    mciSendString("seek bgm_Animation to start", NULL, 0, NULL);
    mciSendString("play bgm_Animation", NULL, 0, NULL);
    Sleep(AUDIO_DELAY_MS);

    // 逐帧播放
    for (int j = 0; j < 223; j++) {
        DWORD start = GetTickCount();
        putimage(0, 0, &frames[j]);

        // 固定帧率控制
        int elapsed = GetTickCount() - start;
        if (elapsed < FRAME_DELAY) {
            Sleep(FRAME_DELAY - elapsed);
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            mciSendString("stop bgm_Animation", NULL, 0, NULL);
            return;
        }
    }
    mciSendString("stop bgm_Animation", NULL, 0, NULL);
}

void showPicture_Card_Chen(int targetX, int targetY, int targetWidth, int targetHeight, int id) {
    IMAGE img;
    TCHAR path_img[MAX_PATH] = _T("pic\\");
    TCHAR output_text[50] = _T("");
    int colors[3] = { GREEN,RGB(112,81,167),RED };
    // 根据卡片类型拼接路径
    if (cards[id].card_flag) {
        _tcscat_s(path_img, _T("pill\\pill_"));
        _stprintf_s(path_img + _tcslen(path_img), MAX_PATH - _tcslen(path_img), _T("%d"), cards[id].id);
        _tcscat_s(path_img, _T(".png"));
        settextcolor(colors[pills[cards[id].id].level]);
        _tcscpy_s(output_text, pills[cards[id].id].name.c_str());
    }
    else {
        _tcscat_s(path_img, _T("book\\book_"));
        _stprintf_s(path_img + _tcslen(path_img), MAX_PATH - _tcslen(path_img), _T("%d"), cards[id].id);
        _tcscat_s(path_img, _T(".png"));
        settextcolor(colors[methods[cards[id].id].level]);
        _tcscpy_s(output_text, methods[cards[id].id].name.c_str());
    }

    // 加载图片
    loadimage(&img, path_img, targetWidth, targetHeight);
    putimage(targetX, targetY, targetWidth, targetHeight, &img, 0, 0, SRCCOPY);
    //加载文字
    settextstyle(targetHeight / 6, 0, "宋体");
    outtextxy(targetX + targetWidth / 2 - textwidth(output_text) / 2, targetY + targetHeight, output_text);
}

/*
    负责人：
    功能： 调用抽卡动画函数showAnimation()
           调用一次抽卡函数int get_card();返回值为该卡片在卡池数组cards中的索引
           将抽卡结果显示在弹窗中
           弹窗中有确认按钮，关闭弹窗

    参数：void
    返回值：void
*/
void drawOnceView() {
    if (currRole.stone >= 300) {
        // 调用抽卡动画函数
        showAnimation();
    }

    // 绘制半透明背景
    setfillcolor(RGB(218, 201, 183));
    fillrectangle(0, 0, 640, 480);

    // 绘制弹窗背景
    setfillcolor(WHITE);
    fillroundrect(80, 100, 560, 380, 10, 10);

    // 绘制边框
    setlinecolor(RGB(97, 16, 36));
    roundrect(80, 100, 560, 380, 10, 10);

    //绘制标题
    settextcolor(BLACK);
    settextstyle(16, 0, "宋体");
    outtextxy(320 - textwidth("◆╔☆╗◆=====★ 仙 缘 独 照 ★=====◆╔☆╗◆ ") / 2, 105, "◆╔☆╗◆=====★ 仙 缘 独 照 ★=====◆╔☆╗◆");
    // 绘制文本
    settextcolor(BLACK);
    settextstyle(16, 0, "宋体");
    // 绘制确认按钮
    button(280, 340, 80, 30, "确认", msg);
    if (currRole.stone >= 300) {
        currRole.stone -= 300;
        // 调用一次抽卡函数
        int card_id = get_card();
        savefile();
        //绘制抽中物品
        showPicture_Card_Chen(270, 190, 100, 100, card_id);
    }
    else {
        settextcolor(BLACK);
        settextstyle(24, 0, "宋体");
        outtextxy(320 - textwidth("您的灵石不足，祈愿没有得到回应！") / 2, 228, "您的灵石不足，祈愿没有得到回应！");
    }

    // 等待用户点击确认按钮
    while (true) {
        if (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (inArea(msg.x, msg.y, 280, 340, 80, 30)) {
                    break;
                }
            }
        }
        Sleep(10);
    }

}

/*
    负责人：
    功能： 调用抽卡动画函数showAnimation()
           调用十次抽卡函数int get_card();返回值为该卡片在卡池数组cards中的索引
           将抽卡结果显示在弹窗中
           弹窗中有确认按钮，关闭弹窗

    参数：void
    返回值：void
*/
void drawTenView() {
    if (currRole.stone >= 2850) {
        // 调用抽卡动画函数
        showAnimation();
    }
    // 绘制半透明背景
    setfillcolor(RGB(218, 201, 183));
    fillrectangle(0, 0, 640, 480);

    // 绘制弹窗背景
    setfillcolor(WHITE);
    fillroundrect(60, 70, 580, 410, 10, 10);

    // 绘制边框
    setlinecolor(RGB(97, 16, 36));
    roundrect(60, 70, 580, 410, 10, 10);

    //绘制标题
    settextcolor(BLACK);
    settextstyle(16, 0, "宋体");
    outtextxy(320 - textwidth("◆╔☆╗◆=====★ 天 道 赐 福 ★=====◆╔☆╗◆ ") / 2, 80, "◆╔☆╗◆=====★ 天 道 赐 福 ★=====◆╔☆╗◆");
    // 绘制确认按钮
    settextcolor(BLACK);
    settextstyle(16, 0, "宋体");
    button(280, 370, 80, 30, "确认", msg);
    if (currRole.stone >= 2850) {
        currRole.stone -= 2850;
        // 调用十次抽卡函数
        for (int i = 0; i < 5; i++) {
            showPicture_Card_Chen(75 + 100 * i, 110, 90, 90, get_card());
            savefile();
        }
        for (int i = 0; i < 5; i++) {
            showPicture_Card_Chen(75 + 100 * i, 240, 90, 90, get_card());
            savefile();
        }
    }
    else {
        settextcolor(BLACK);
        settextstyle(24, 0, "宋体");
        outtextxy(320 - textwidth("您的灵石不足，祈愿没有得到回应！") / 2, 228, "您的灵石不足，祈愿没有得到回应！");

    }
    // 绘制确认按钮
    settextcolor(BLACK);
    settextstyle(16, 0, "宋体");
    button(280, 370, 80, 30, "确认", msg);

    // 等待用户点击确认按钮
    while (true) {
        if (peekmessage(&msg, EX_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (inArea(msg.x, msg.y, 280, 370, 80, 30)) {
                    break;
                }
            }
        }
        Sleep(10);
    }

}



/*
    负责人：
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
void drawingCardItf() {
    //播放音乐------------------
    mciSendString("seek bgm_drawingCardItf to start", NULL, 0, NULL);
    mciSendString("play bgm_drawingCardItf repeat", NULL, 0, NULL);
    //播放音乐------------------
    IMAGE img_bk;
    loadimage(&img_bk, "pic/CardBackground.png", 426.3, 300);

    // 定义按钮绘制的匿名函数
    auto button = [](int x, int y, int w, int h, const char* text, ExMessage& msg) {
        // 根据鼠标是否悬停设置按钮颜色
        bool isHovered = (msg.x >= x && msg.x <= x + w && msg.y >= y && msg.y <= y + h);

        // 设置填充颜色并绘制按钮背景
        setfillcolor(isHovered ? RGB(97, 16, 36) : WHITE);
        fillroundrect(x, y, x + w, y + h, 5, 5);

        // 设置边框颜色并绘制边框
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID, 1); // 1像素宽的实线
        roundrect(x, y, x + w, y + h, 5, 5);

        // 计算文本居中位置并绘制
        int hSpace = (w - textwidth(text)) / 2;
        int vSpace = (h - textheight(text)) / 2;
        settextcolor(BLACK);
        outtextxy(x + hSpace, y + vSpace, text);
        return (msg.message == WM_LBUTTONDOWN && isHovered);
        };

    while (true) {
        peekmessage(&msg, EX_MOUSE);
        BeginBatchDraw();
        cleardevice();

        putimage(210, 60, &img_bk);

        settextcolor(BLACK);
        string number = "灵石数量:";
        number += to_string(currRole.stone);
        outtextxy(450, 30, number.c_str());

        //抽卡词信息
        settextcolor(RGB(97, 16, 36));

        string lucky = "祈愿60次必出SSR（";
        lucky += to_string(currRole.furtuValue);
        lucky += "/60）";
        outtextxy(200, 410, lucky.c_str());

        settextstyle(12, 0, "");
        string mothod[] = { "◆══ 天机阁·祈愿卡池 ══◆","【天道至宝】SSR（天赐机缘·3%）","o 九转金丹 o 混沌道体丹",
            "o 太虚玄功 o 混沌道经","o 逆天改命丹 o 两仪造化丹","【玄妙灵物】SR（地脉灵韵·10%）","◇ 三转玄元丹 ◇ 冲脉散","◇ 琉璃玉髓丹 ◇ 紫霞心经 ","◇ 血煞魔功","【凡尘妙药】R（众生道基·87%）","◆ 基础破障丹 ◆ 小还丹","◆ 易筋丸","◆════════════◆" };
        for (int i = 0; i < 13; i++) {
            outtextxy(20, 45 + i * 20, mothod[i].c_str());
        }
        outtextxy(30, 350, "玄不改非，氪不改命");
        outtextxy(30, 370, "及时止损，见好就收");

        if (button(30, 400, 60, 30, "返回", msg) && msg.message == WM_LBUTTONDOWN) {
            EndBatchDraw();
            //停止播放-----
            mciSendString("stop bgm_drawingCardItf", NULL, 0, NULL);
            //停止播放-----
            return;
        }

        if (button(340, 400, 100, 30, "祈愿/300灵石", msg) && msg.message == WM_LBUTTONDOWN) {
            EndBatchDraw();
            //停止播放-----
            mciSendString("stop bgm_drawingCardItf", NULL, 0, NULL);
            //停止播放-----
            drawOnceView();
            //继续播放-----
            mciSendString("resume bgm_drawingCardItf", NULL, 0, NULL);
            //继续播放-----
            continue;
        }

        if (button(480, 400, 140, 30, "10连祈愿/2850灵石", msg) && msg.message == WM_LBUTTONDOWN) {
            EndBatchDraw();
            //停止播放-----
            mciSendString("stop bgm_drawingCardItf", NULL, 0, NULL);
            //停止播放-----
            drawTenView();
            //继续播放-----
            mciSendString("resume bgm_drawingCardItf", NULL, 0, NULL);
            //继续播放-----
            BeginBatchDraw();
            continue;
        }
        EndBatchDraw();
    }
}

//-----------------------------------------以上为抽卡函数-----------------------------------

//-----------------------------------------以下事件有关接口--------------------
void eventItf() {
    int width = 640, height = 480;
    RECT btnBack = { 20, 20, 120, 60 };                         //返回按钮
    RECT textBox = { width - 600, 80, width - 200, 200 };         //文字显示部分
    RECT eventBox = { 120, height - 150, width - 120, height - 20 };//事件栏
    RECT diceBox = { 520, height - 150, 580, height - 90 };                       //筛子
    RECT mapArea = { 120, height - 400, 520, height - 160 };
    const int mapGridNum = 20;
    const int gridW = 80, gridH = 50;
    POINT mapPos[mapGridNum];
    for (int i = 0; i < 5; ++i) mapPos[i] = { mapArea.left + i * gridW, mapArea.top };
    for (int i = 0; i < 5; ++i) mapPos[5 + i] = { mapArea.left + 4 * gridW, mapArea.top + i * gridH };
    for (int i = 0; i < 5; ++i) mapPos[10 + i] = { mapArea.left + 4 * gridW - i * gridW, mapArea.top + 4 * gridH };
    for (int i = 0; i < 5; ++i) mapPos[15 + i] = { mapArea.left, mapArea.top + 4 * gridH - i * gridH };

    IMAGE cellImg[20];
    for (int i = 0; i < mapGridNum; ++i) {
        int x = mapPos[i].x, y = mapPos[i].y;
        if (!maps[i].pic.empty()) {
            loadimage(&cellImg[i], maps[i].pic.c_str(), gridW, gridH);
        }
    }
    IMAGE playerImg;
    loadimage(&playerImg, "pic\\玩家.png", 50, 50);

    // 加载背景图
    IMAGE bgImg;
    loadimage(&bgImg, "pic\\eventbk.jpg", width, height);

    IMAGE centerbk;
    loadimage(&centerbk, "pic\\event_center.png", 240, 160, TRUE);

    std::string eventResult; // 事件栏显示内容


    while (true) {
        peekmessage(&msg, EX_MOUSE);

        BeginBatchDraw();
        cleardevice();

        // 绘制背景图
        putimage(0, 0, &bgImg);
        putimage(200, 125, &centerbk);

        // 返回按钮
        if (transparent_button(20, 20, 100, 40, "返回", msg)) {
            msg.message = 0;
            return;
        }
        // 掷筛子
        if (transparent_button(540, height - 150, 80, 80, "500灵石/次", msg)) {
            msg.message = 0;
            int step = get_step();
            currRole.event_point = (currRole.event_point + step + mapGridNum) % mapGridNum;
            triggerEvent(currRole.event_point, eventResult);
        }

        settextcolor(BLACK);
        settextstyle(36, 0, "华文行楷-优雅手写风格");
        outtextxy(width / 2 - 60, 40, "修仙奇遇");

        // 事件栏
        setfillcolor(WHITE);
        fillrectangle(eventBox.left, eventBox.top, eventBox.right, eventBox.bottom);
        settextcolor(BLACK);
        settextstyle(12, 0, "华文行楷-优雅手写风格");
        // 优先显示事件结果，否则显示当前位置事件
        if (!eventResult.empty()) {
            // 多行输出
            int y = eventBox.top + 10;
            size_t start = 0, end;
            while ((end = eventResult.find('\n', start)) != std::string::npos) {
                std::string line = eventResult.substr(start, end - start);
                outtextxy(eventBox.left + 10, y, line.c_str());
                y += 28;
                start = end + 1;
            }
        }
        else {
            int curEventId = maps[currRole.event_point].event_id;
            if (curEventId >= 0 && curEventId < EVENT_NUM) {
                outtextxy(eventBox.left + 10, eventBox.top + 10, events[curEventId].event_title.c_str());
                outtextxy(eventBox.left + 10, eventBox.top + 40, events[curEventId].event_intro.c_str());
            }
            else {
                outtextxy(eventBox.left + 10, eventBox.top + 10, "暂无事件");
            }
        }

        // 地图与格子
        for (int i = 0; i < mapGridNum; ++i) {
            int x = mapPos[i].x, y = mapPos[i].y;
            if (!maps[i].pic.empty()) {
                putimage(x, y, &cellImg[i]);
            }
            else {
                setfillcolor(RGB(220, 220, 220));
                fillrectangle(x, y, x + gridW, y + gridH);
            }
            setlinecolor(BLACK);
            rectangle(x, y, x + gridW, y + gridH);

            int eid = maps[i].event_id;
            if (eid >= 0 && eid < EVENT_NUM) {
                settextcolor(BLACK);
                settextstyle(12, 0, "微软雅黑");
                outtextxy(x + 2, y + 2, events[eid].event_title.c_str());
            }
        }
        // 玩家头像
        int px = mapPos[currRole.event_point].x + (gridW - 50) / 2;
        int py = mapPos[currRole.event_point].y + (gridH - 50) / 2;
        putimage(px, py, &playerImg);
        EndBatchDraw();
    }
}
//-----------------------------------------------以上事件有关--------------------------------------
/*
    得到随机数，放置图片
*/
void showloading() {
    random_device rd;       //获取随机数种子
    mt19937 gen(rd());      //初始化随机数变量
    int randomInt;
    uniform_int_distribution<> distrib(1, 7);
    randomInt = distrib(gen);
    IMAGE tmp;
    char path[100];
    sprintf_s(path, "pic\\loaditf\\loaditf%d.png", randomInt);
    loadimage(&tmp, path, 640, 480);
    putimage(0, 0, &tmp);
}
