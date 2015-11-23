#include "main.h"

HWND g_hWnd;
HINSTANCE g_hInst;//实例句柄
RenderInterface *g_render = NULL;

int g_mainGUI = -1;
int g_startGUI = -1;
int g_creditsGUI = -1;
int g_currentGUI = GUI_MAIN_SCREEN;

int g_arialID = -1;

bool g_LMBDown = false;
int g_mouseX = 0;
int g_mouseY = 0;

CInputInterface *g_inputSystem = NULL;

stGameWorld g_gameWorld;

enum GAME_STATE {
    GS_MENUI = 1,
    GS_LEVEL,
    GS_LEVEL_1_SWITCH,
};
GAME_STATE g_gameState = GS_MENUI;

LRESULT WINAPI MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY://要处理的消息,销毁窗口
            PostQuitMessage(0);
            return 0;
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);//不需要的转给windows处理
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
        hInstance, NULL, NULL, NULL /*(HBRUSH)GetStockObject(WHITE_BRUSH)*/, NULL, WINDOW_CLASS, NULL };

    RegisterClassEx(&wc);

    if (WINDOW_FULLSCREEN) {
        g_hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE,
            NULL, NULL, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    }
    else {
        g_hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            NULL, NULL, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    }

    if (g_hWnd) {
        ShowWindow(g_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(g_hWnd);
    }

    g_hInst = wc.hInstance;

    if (initializeEngine()) {
        if (gameInitialize()) {
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));

            SetCursorPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);//鼠标位置

            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);//翻译
                    DispatchMessage(&msg);//转发
                }
                else {
                    //没消息处理时，就可以做其他的工作
                    gameLoop();
                }
            }
        }
    }
    gameShudown();
    shutdownEngine();
    UnregisterClass(WINDOW_CLASS, hInstance);//取消注册

    return 0;
}

bool initializeEngine() {
    if (!createD3DRenderer(&g_render)) {
        return false;
    }

    if (!g_render->initialize(WINDOW_WIDTH, WINDOW_HEIGHT, g_hWnd, DEFINES_FAIL, MS_SAMPLES_4)) {
        return false;
    }

    g_render->setClearColor(0, 0, 0);

    if (!g_render->createText("Arial", 0, true, 18, g_arialID)) {
        return false;
    }

    if (!CreateDIInput(&g_inputSystem, g_hWnd, g_hInst, false)) {
        return false;
    }
    if (!g_inputSystem->Initialize()) {
        return false;
    }

    return true;
}

void shutdownEngine() {
    if (g_render) {
        g_render->shutDown();
        delete g_render;
        g_render = NULL;
    }
    if (g_inputSystem) {
        g_inputSystem->Shutdown();
        delete g_inputSystem;
        g_inputSystem = NULL;
    }
}

bool gameInitialize() {
    if (!initializeMainMenu()) {
        return false;
    }

    return true;
}


void gameLoop() {
    processInput();
    if (g_gameState == GS_LEVEL) {
        levelRender();
    }
    else if (g_gameState == GS_MENUI) {
        mainMenuRender();
    }
    else if (g_gameState == GS_LEVEL_1_SWITCH) {
        gameReleaseAll();
        if (loadLevel("maps/level1/level1.lvl")) {
            g_gameState = GS_LEVEL;
        }
        else {
            g_gameState = GS_MENUI;
        }
    }
}

void gameShudown() {
    gameReleaseAll();
}



bool initializeMainMenu() {
    if (!g_render->createGUI(g_mainGUI)) {
        return false;
    }
    if (!g_render->createGUI(g_startGUI)) {
        return false;
    }
    if (!g_render->createGUI(g_creditsGUI)) {
        return false;
    }

    if (!g_render->addGUIBackdrop(g_mainGUI, "menu/mainMenu.jpg")) {
        return false;
    }
    if (!g_render->addGUIBackdrop(g_startGUI, "menu/startMenu.jpg")) {
        return false;
    }
    if (!g_render->addGUIBackdrop(g_creditsGUI, "menu/creditsMenu.jpg")) {
        return false;
    }

    if (!g_render->addGUIStaticText(g_mainGUI, STATIC_TEXT_ID, "Version:1.0", PERCENT_OF(WINDOW_WIDTH, 0.85), PERCENT_OF(WINDOW_HEIGHT, 0.05), UGPCOLOR_ARGB(255, 255, 255, 255), g_arialID)) {
        return false;
    }

    if (!g_render->addGUIButton(g_mainGUI, BUTTON_CREDITS_ID, 80, 260, "menu/creditsUp.png",
        "menu/creditsOver.png", "menu/creditsDown.png")) {
        return false;
    }

    if (!g_render->addGUIButton(g_mainGUI, BUTTON_QUIT_ID, 80, 180, "menu/quitUp.png",
        "menu/quitOver.png", "menu/quitDown.png")) {
        return false;
    }

    if (!g_render->addGUIButton(g_mainGUI, BUTTON_STAT_ID, 80, 100, "menu/startUp.png",
        "menu/startOver.png", "menu/startDown.png")) {
        return false;
    }
    if (!g_render->addGUIButton(g_startGUI, BUTTON_LEVEL_1_ID, 80, 100, "menu/level1Up.png", "menu/level1Over.png", "menu/level1Down.png")) {
        return false;
    }

    return true;
}


void mainMenuCallback(int id, int state) {
    switch (id) {
        case BUTTON_STAT_ID:
            if (state == GUI_BUTTON_DOWN) {
                g_currentGUI = GUI_START_SCREEN;
            }
            break;
        case BUTTON_CREDITS_ID:
            if (state == GUI_BUTTON_DOWN) {
                g_currentGUI = GUI_CREDITS_SCREEN;
            }
            break;
        case BUTTON_QUIT_ID:
            if (state == GUI_BUTTON_DOWN) {
                gameReleaseAll();
                PostQuitMessage(0);
            }
            break;
        case BUTTON_LEVEL_1_ID:
            if (state == GUI_BUTTON_DOWN) {
                g_gameState = GS_LEVEL_1_SWITCH;
            }
            break;
    }
}

void mainMenuRender() {
    if (!g_render) {
        return;
    }
    g_render->startRender(1, 1, 0);

    if (g_currentGUI == GUI_MAIN_SCREEN) {
        g_render->processGUI(g_mainGUI, g_LMBDown, g_mouseX, g_mouseY, mainMenuCallback);
    }
    else if (g_currentGUI == GUI_START_SCREEN) {
        g_render->processGUI(g_startGUI, g_LMBDown, g_mouseX, g_mouseY, mainMenuCallback);
    }
    else if (g_currentGUI == GUI_CREDITS_SCREEN) {
        g_render->processGUI(g_creditsGUI, g_LMBDown, g_mouseX, g_mouseX, mainMenuCallback);
    }

    g_render->endRender();
}

void processInput() {
    if (!g_inputSystem) {
        return;
    }
    g_inputSystem->UpdateDevices();

    if (g_inputSystem->KeyUp(DIK_ESCAPE)) {
        if (g_gameState == GS_MENUI) {
            switch (g_currentGUI) {
                case GUI_MAIN_SCREEN:
                    PostQuitMessage(0);
                    break;
                default:
                    g_currentGUI = GUI_MAIN_SCREEN;
                    break;
            }
        }
        else if (g_gameState == GS_LEVEL) {
            gameReleaseAll();

            initializeMainMenu();
            g_gameState = GS_MENUI;
            g_currentGUI = GUI_MAIN_SCREEN;
        }
    }

    POINT pos = { 0, 0 };
    GetCursorPos(&pos);

    g_mouseX = pos.x;
    g_mouseY = pos.y;

    if (g_inputSystem->MouseButtonDown(LEFT_MOUSE_BUTTON)) {
        g_LMBDown = true;
    }
    if (!g_inputSystem->MouseButtonDown(LEFT_MOUSE_BUTTON)) {
        g_LMBDown = false;
    }
}

void gameReleaseAll() {
    if (!g_render) {
        return;
    }
    g_render->releaseAllStaticBuffers();
    g_render->releaseAllXModels();
    g_render->releaseAllGUIs();

    g_gameWorld.m_mlevelID = -1;
    g_gameWorld.m_skyBoxID = -1;

    g_mainGUI = -1;
    g_startGUI = -1;
    g_creditsGUI = -1;
    g_currentGUI = GUI_MAIN_SCREEN;

}

bool loadLevel(char *file) {
    if (!g_render || !file) {
        return false;
    }
    CCommandScript script;
    char command[MAX_COMMAND_SIZE];
    char param[MAX_PARAM_SIZE];

    if (!script.LoadScriptFile(file)) {
        return false;
    }
    for (int i = 0; i < script.GetTotalLines(); ++i) {
        script.ParseCommand(command);
        if (_stricmp(command, "LoadSkyBoxAsX") == 0) {

        }
        else if (_stricmp(command, "LoadTerrainAsX") == 0) {

        }
        else if (_stricmp(command, "LoadStaticAsX") == 0) {

        }
        else if (_stricmp(command, "LoadAnimateModelAsX") == 0) {

        }
        else if (_stricmp(command, "#") == 0) {

        }
        else {
            char err[64];
            sprintf_s(err, "Error loading level on %d", script.GetCurrentLineNum() + 1);
            //写进日志
            script.Shutdown();
            return false;
        }
        script.MoveToNextLine();
    }
    script.MoveToStart();
    for (int i = 0; i < script.GetTotalLines(); ++i) {
        script.ParseCommand(command);

        if (_stricmp(command, "LoadSkyBoxAsX") == 0) {
            script.ParseStringParam(param);
            if (g_gameWorld.m_skyBoxID == -1) {
                if (!g_render->loadXMode(param, &g_gameWorld.m_skyBoxID)) {
                    return false;
                }
            }
            else {
                if (!g_render->loadXMode(param, g_gameWorld.m_skyBoxID)) {
                    return false;
                }
            }
        }
        else if (_stricmp(command, "LoadTerrainAsX") == 0) {
            script.ParseStringParam(param);
            if (g_gameWorld.m_mlevelID == -1) {
                if (!g_render->loadXMode(param, &g_gameWorld.m_mlevelID)) {
                    return false;
                }
            }
            else {
                if (!g_render->loadXMode(param, g_gameWorld.m_mlevelID)) {
                    return false;
                }
            }
        }
        else if (_stricmp(command, "LoadStaticAsX") == 0) {

        }
        else if (_stricmp(command, "LoadAnimateModelAsX") == 0) {

        }
        else if (_stricmp(command, "#") == 0) {

        }
        script.MoveToNextLine();
    }
    script.Shutdown();
    return true;
}

void levelRender() {
    if (!g_render || g_gameState != GS_LEVEL) {
        return;
    }
    g_render->startRender(1, 1, 0);
    g_render->renderXMode(g_gameWorld.m_skyBoxID);
    g_render->renderXMode(g_gameWorld.m_mlevelID);
    g_render->endRender();
}