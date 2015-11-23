#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "Defines.h"
#include "GUI.h"
#include "Structs.h"


//渲染接口

class RenderInterface {
public:
    RenderInterface() :m_screenWidth(0), m_screenHeight(0),
        m_near(0), m_far(0) {
    }
    virtual ~RenderInterface() {
    }

    virtual bool initialize(int w, int h, WinHWND hWnd, bool full, MS_TYPE ms) = 0;
    virtual void oneTimeInit() = 0;
    virtual void setClearColor(float r, float g, float b) = 0;//清屏

    //开始渲染
    virtual void startRender(bool bColor, bool bDepth, bool bStencil) = 0;
    //结束渲染
    virtual void endRender() = 0;
    //关闭
    virtual void shutDown() = 0;
    //清除正在渲染的场景
    virtual void clearBuffers(bool bColor, bool bDepth, bool bStencil) = 0;
    //创建静态缓存
    virtual int createStaticBuffer(VertexType, PrimType, int totalVerts, int totalIndices,
        int stride, void **data, unsigned int *indices, int *staticId) = 0;
    //渲染
    virtual int endRendering(int staticId) = 0;
    //设置材质
    virtual void setMaterial(stMaterial *material) = 0;
    //设置光源
    virtual void setLight(stLight *light, int index) = 0;
    //关闭光源
    virtual void disableLight(int index) = 0;

    //透视矩阵
    virtual void calculateProjMatrix(float fov, float n, float f) = 0;
    //正交矩阵
    virtual void calculateOrthoMatrix(float n, float f) = 0;

    //设置透明度 渲染状态，原，目标
    virtual void setTranspency(RenderState state, TransState src, TransState dst) = 0;
    //添加纹理 文件名，纹理指针
    virtual int addTexture2D(char *file, int *texId) = 0;
    //设置过滤器 索引，多虑器，过滤器类型
    virtual void setTextureFilter(int index, int filter, int val) = 0;
    //设置多重纹理
    virtual void setMultiTexture() = 0;
    //细节映射
    virtual void setDetailMapping() = 0;
    //应用纹理 索引，指定的纹理
    virtual void applyTexture(int index, int texId) = 0;
    //保存截屏
    virtual void saveScreenShot(char *file) = 0;

    /*点精灵*/
    //激活点精灵
    virtual void enablePointSprites(float size, float min, float a, float b, float c) = 0;
    //停用点精灵
    virtual void disablePointSprites() = 0;
    //法线纹理
    virtual void setNormalmap(int index, int texId) = 0;

    /*雾化*/
    //距离,类型,颜色,受否使用范围雾
    virtual void enableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog) = 0;
    virtual void disableFog() = 0;

    /*GUI*/

    //字体
    virtual bool createText(char *font, int weight, bool italic, int size, int &id) = 0;
    virtual void displayText(int id, long x, long y, int r, int g, int b, char *text, ...) = 0;
    virtual void displayText(int id, long x, long y, unsigned long color, char *text, ...) = 0;

    /*X模型*/

    virtual void releaseAllStaticBuffers() = 0;
    virtual void releaseAllXModels() = 0;
    virtual int  releaseStaticBuffer(int staticID) = 0;
    virtual int  releaseXMode(int xModelID) = 0;
    virtual int  loadXMode(char *file, int *xModelID) = 0;
    virtual int  loadXMode(char *file, int xModelID) = 0;

    virtual int  renderXMode(int xModelID) = 0;

    void releaseAllGUIs() {
        for (int i = 0; i < m_totalGUIs; ++i) {
            m_guiList[i].shutDown();
        }
        m_totalGUIs = 0;
        if (m_guiList) {
            delete[] m_guiList;
            m_guiList = NULL;
        }
    }

    bool createGUI(int &id) {
        if (!m_guiList) {
            m_guiList = new GUISystem[1];
            if (!m_guiList) {
                return false;
            }
        }
        else {
            GUISystem *temp;
            temp = new GUISystem[m_totalGUIs + 1];
            memcpy(temp, m_guiList, sizeof(GUISystem)*m_totalGUIs);
            delete[] m_guiList;
            m_guiList = temp;
        }
        id = m_totalGUIs;
        m_totalGUIs++;

        return true;
    }

    virtual bool addGUIBackdrop(int guiID, char *fileName) = 0;
    virtual bool addGUIStaticText(int guiID, int id, char *text, int x, int y, unsigned long color, int fontID) = 0;
    virtual bool addGUIButton(int guiID, int id, int x, int y, char *up, char *over, char *down) = 0;
    //回掉函数
    virtual void processGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state)) = 0;

protected:
    int m_screenWidth;
    int m_screenHeight;
    bool m_fullscreen;

    int m_totlFonts;//字体个数

    GUISystem *m_guiList;//gui列表
    int m_totalGUIs;//gui个数

    WinHWND m_mainHandle;
    float m_near;//近裁剪面
    float m_far;//远裁剪面
};


#endif