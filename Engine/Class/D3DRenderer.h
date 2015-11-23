#ifndef _D3DRENDERER_H_
#define _D3DRENDERER_H_
#include "RenderInterface.h"
#include "Defines.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")

struct stD3DStaticBuffer {
    stD3DStaticBuffer() :vbPtr(0), ibPtr(0), nubVerts(0), numIndices(0), stride(0), fvf(0), primType(NULL_TYPE) {}

    LPDIRECT3DVERTEXBUFFER9 vbPtr;//顶点
    LPDIRECT3DINDEXBUFFER9 ibPtr;//顶点索引
    int nubVerts;
    int numIndices;
    int stride;
    unsigned long fvf;
    PrimType primType;//绘制基本类型

};

class XModel {
public:
    XModel() {
        model = NULL;
        numMaterials = 0;
        matList = NULL;
        textureList = NULL;
    }
    LPD3DXMESH model;
    unsigned long numMaterials;
    D3DMATERIAL9 *matList;
    LPDIRECT3DTEXTURE9 *textureList;

    void shoutDown() {
        if (model!=NULL) {
            model->Release();
            model = NULL;
        }
        if (textureList) {
            for (unsigned long i = 0; i < numMaterials; i++) {
                if (textureList[i]) {
                    textureList[i]->Release();
                    textureList[i] = NULL;
                }
            }
        }
        if (matList!=NULL) {
            delete[] matList;
            matList = NULL;
        }
        if (textureList!=NULL) {
            delete[] textureList;
            textureList = NULL;
        }
    }
};

struct stD3DTexture {
    char *fileName;
    int width, height;
    LPDIRECT3DTEXTURE9 image;
    //LPDIRECT3DTEXTURE9 normalTexture;
};

//d3d渲染器

class D3DRenderer :public RenderInterface {
public:
    D3DRenderer();
    ~D3DRenderer();

    bool initialize(int w, int h, WinHWND hWnd, bool full,MS_TYPE ms);
    //透视矩阵
    void calculateProjMatrix(float fov, float n, float f);
    //正交矩阵
    void calculateOrthoMatrix(float n, float f);
    void setClearColor(float r, float g, float b);

    void startRender(bool bColor, bool bDepth, bool bStencil);
    void endRender();

    void shutDown();

    int endRendering(int staticId);

    void clearBuffers(bool bColor, bool bDepth, bool bStencil);

    int createStaticBuffer(VertexType vType, PrimType primType, int totalVerts, int totalIndices, int stride, void **data, unsigned int *indices, int *staticId);

    void setMaterial(stMaterial *material);

    void setLight(stLight *light, int index);

    void disableLight(int index);


    void setTranspency(RenderState state, TransState src, TransState dst);

    int addTexture2D(char *file, int *texId);

    void setTextureFilter(int index, int filter, int val);

    void setMultiTexture();

    void setDetailMapping();

    void applyTexture(int index, int texId);

    void saveScreenShot(char *file);

    void enablePointSprites(float size, float min, float a, float b, float c);

    void disablePointSprites();

    void setNormalmap(int index, int texId);

    void enableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog);
    void disableFog();

    /*GUI*/
    bool createText(char *font, int weight, bool italic, int size, int &id);
    void displayText(int id, long x, long y, int r, int g, int b, char *text, ...);
    void displayText(int id, long x, long y, unsigned long color, char *text, ...);
    bool addGUIBackdrop(int guiID, char *fileName);
    bool addGUIStaticText(int guiID, int id, char *text, int x, int y, unsigned long color, int fontID);
    bool addGUIButton(int guiID, int id, int x, int y, char *up, char *over, char *down);
    void processGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state));

    void releaseAllStaticBuffers();
    void releaseAllXModels();
    int  releaseStaticBuffer(int staticID);
    int  releaseXMode(int xModelID);
    int  loadXMode(char *file, int *xModelID);
    int  loadXMode(char *file, int xModelID);

    int  renderXMode(int xModelID);
private:
    void oneTimeInit();

private:
    D3DCOLOR m_color;
    LPDIRECT3D9 m_direct3D;
    LPDIRECT3DDEVICE9 m_device;
    bool m_renderingScene;

    XModel *m_xModels;
    int m_numXModels;

    LPD3DXFONT *m_fonts;//字体

    stD3DStaticBuffer *m_staticBufferList;//静态缓存
    int m_numStaticBuffers;//静态缓存大小
    int m_activeStaticBuffer;//当前正在使用的

    stD3DTexture *m_textureList;
    unsigned int m_numTextures;
};

bool createD3DRenderer(RenderInterface **pRender);


#endif