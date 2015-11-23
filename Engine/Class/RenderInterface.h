#ifndef _RENDERINTERFACE_H_
#define _RENDERINTERFACE_H_
#include "Defines.h"
#include "GUI.h"
#include "Structs.h"


//��Ⱦ�ӿ�

class RenderInterface {
public:
    RenderInterface() :m_screenWidth(0), m_screenHeight(0),
        m_near(0), m_far(0) {
    }
    virtual ~RenderInterface() {
    }

    virtual bool initialize(int w, int h, WinHWND hWnd, bool full, MS_TYPE ms) = 0;
    virtual void oneTimeInit() = 0;
    virtual void setClearColor(float r, float g, float b) = 0;//����

    //��ʼ��Ⱦ
    virtual void startRender(bool bColor, bool bDepth, bool bStencil) = 0;
    //������Ⱦ
    virtual void endRender() = 0;
    //�ر�
    virtual void shutDown() = 0;
    //���������Ⱦ�ĳ���
    virtual void clearBuffers(bool bColor, bool bDepth, bool bStencil) = 0;
    //������̬����
    virtual int createStaticBuffer(VertexType, PrimType, int totalVerts, int totalIndices,
        int stride, void **data, unsigned int *indices, int *staticId) = 0;
    //��Ⱦ
    virtual int endRendering(int staticId) = 0;
    //���ò���
    virtual void setMaterial(stMaterial *material) = 0;
    //���ù�Դ
    virtual void setLight(stLight *light, int index) = 0;
    //�رչ�Դ
    virtual void disableLight(int index) = 0;

    //͸�Ӿ���
    virtual void calculateProjMatrix(float fov, float n, float f) = 0;
    //��������
    virtual void calculateOrthoMatrix(float n, float f) = 0;

    //����͸���� ��Ⱦ״̬��ԭ��Ŀ��
    virtual void setTranspency(RenderState state, TransState src, TransState dst) = 0;
    //������� �ļ���������ָ��
    virtual int addTexture2D(char *file, int *texId) = 0;
    //���ù����� ������������������������
    virtual void setTextureFilter(int index, int filter, int val) = 0;
    //���ö�������
    virtual void setMultiTexture() = 0;
    //ϸ��ӳ��
    virtual void setDetailMapping() = 0;
    //Ӧ������ ������ָ��������
    virtual void applyTexture(int index, int texId) = 0;
    //�������
    virtual void saveScreenShot(char *file) = 0;

    /*�㾫��*/
    //����㾫��
    virtual void enablePointSprites(float size, float min, float a, float b, float c) = 0;
    //ͣ�õ㾫��
    virtual void disablePointSprites() = 0;
    //��������
    virtual void setNormalmap(int index, int texId) = 0;

    /*��*/
    //����,����,��ɫ,�ܷ�ʹ�÷�Χ��
    virtual void enableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog) = 0;
    virtual void disableFog() = 0;

    /*GUI*/

    //����
    virtual bool createText(char *font, int weight, bool italic, int size, int &id) = 0;
    virtual void displayText(int id, long x, long y, int r, int g, int b, char *text, ...) = 0;
    virtual void displayText(int id, long x, long y, unsigned long color, char *text, ...) = 0;

    /*Xģ��*/

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
    //�ص�����
    virtual void processGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state)) = 0;

protected:
    int m_screenWidth;
    int m_screenHeight;
    bool m_fullscreen;

    int m_totlFonts;//�������

    GUISystem *m_guiList;//gui�б�
    int m_totalGUIs;//gui����

    WinHWND m_mainHandle;
    float m_near;//���ü���
    float m_far;//Զ�ü���
};


#endif