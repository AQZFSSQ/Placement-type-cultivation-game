#pragma once
#include<iostream>
#include<easyx.h>
#include "datastruct.h"
#include"global.h"

/**
 * 判断点 (mx, my) 是否在矩形区域内
 * @param mx, my 待检测点的坐标
 * @param x, y 矩形左上角坐标
 * @param w, h 矩形的宽和高
 * @return 点在矩形内返回 true，否则返回 false
 */
bool inArea(int mx, int my, int x, int y, int w, int h) {
    if (mx > x && mx < x + w && my > y && my < y + h) {
        return true;
    }
    return false;
}

/**
 * 绘制按钮并检测点击事件
 * @param x, y 按钮左上角坐标
 * @param w, h 按钮的宽和高
 * @param text 按钮显示的文本
 * @param msg 鼠标消息（用于检测悬停和点击）
 * @return 如果按钮被点击返回 true，否则返回 false
 */
bool button(int x, int y, int w, int h, const char* text, ExMessage& msg) {
    // 根据鼠标是否悬停设置按钮颜色
    if (inArea(msg.x, msg.y, x, y, w, h)) {
        setfillcolor(RGB(179, 188, 211)); // 悬停状态颜色
    }
    else {
        setfillcolor(YELLOW); // 默认颜色
    }
    fillroundrect(x, y, x + w, y + h, 5, 5); // 绘制圆角矩形按钮

    // 计算文本居中位置并绘制
    int hSpace = (w - textwidth(text)) / 2;
    int vSpace = (h - textheight(text)) / 2;
    settextcolor(BLACK);
    outtextxy(x + hSpace, y + vSpace, text);

    // 检测鼠标左键点击事件
    if (msg.message == WM_LBUTTONDOWN && inArea(msg.x, msg.y, x, y, w, h)) {
        return true; // 按钮被点击
    }
    return false;
}
//绘制透明按钮
/*
 * 绘制一个透明按钮，按钮有圆角矩形背景和文本
 * x, y 按钮左上角坐标
 * w, h 按钮的宽和高
 * text 按钮显示的文本
 * msg 鼠标消息（用于检测悬停和点击）
 * return 如果按钮被点击返回 true，否则返回 false
 */ 
bool transparent_button(int x, int y, int w, int h, const char* text, ExMessage& msg) {
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 2);
    if (inArea(msg.x, msg.y, x, y, w, h)) {
        x -= 10;
        y -= 10;
        w += 20;
        h += 20;
    }
    roundrect(x, y, x + w, y + h, 5, 5);
    settextcolor(BLACK);
    int tx = x + (w - textwidth(text)) / 2;
    int ty = y + (h - textheight(text)) / 2;
    LOGFONT originalFont;
    gettextstyle(&originalFont); // 保存原始字体样式
    settextstyle(12, 0, "宋体");
    outtextxy(tx, ty, text);
    settextstyle(&originalFont); // 恢复原始字体样式
    setlinestyle(PS_SOLID, 1);
    if (msg.message == WM_LBUTTONDOWN && inArea(msg.x, msg.y, x, y, w, h)) {
        return true;
    }
    return false;
}