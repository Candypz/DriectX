#ifndef _MAIN_H_
#define _MAIN_H_
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FULLSCREEN 0

#include "../../Engine/Class/Engine.h"
#pragma comment(lib,"../Debug/Engine.lib")

#include <iostream>
#include <stdio.h>

#define WINDOW_CLASS "UGPDX"
#define WINDOW_TITLE "Mak"

#define GUI_MAIN_SCREEN    1
#define GUI_START_SCREEN   2
#define GUI_CREDITS_SCREEN 3

#define STATIC_TEXT_ID     1
#define BUTTON_STAT_ID     2
#define BUTTON_CREDITS_ID  3
#define BUTTON_QUIT_ID     4
#define BUTTON_BACK_ID     5
#define BUTTON_LEVEL_1_ID  6

bool initializeEngine();//初始引擎
void shutdownEngine();//关闭引擎


bool initializeMainMenu();
void mainMenuRender();
void mainMenuCallback(int id, int state);

//处理输入
void processInput();

bool gameInitialize();//初始化游戏
void gameLoop();//游戏循环
void gameShudown();//关闭游戏
void gameReleaseAll();//释放游戏资源

bool loadLevel(char *file);
void levelRender();

struct stGameWorld {
    stGameWorld() :m_mlevelID(-1), m_skyBoxID(-1) {}
    int m_mlevelID;
    int m_skyBoxID;
};

#endif