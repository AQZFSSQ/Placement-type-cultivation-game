#pragma once
#include<easyx.h>
#include"datastruct.h"
#include"global.h"

using namespace std;
bool inArea(int mx, int my, int x, int y, int w, int h);
bool button(int x, int y, int w, int h, const char* text, ExMessage& msg);
bool transparent_button(int x, int y, int w, int h, const char* text, ExMessage& msg);