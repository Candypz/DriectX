#ifndef _GUI_H_
#define _GUI_H_

#include "Defines.h"

//控件类型
struct stGUIControl {
    int m_type;
    int m_id;
    unsigned long m_color;

    int m_listID;
    float m_x, m_y;
    float m_width, m_height;
    char *m_text;

    int m_upTex, m_downTex, m_overTex;
};

class GUISystem {
public:
    GUISystem() :m_guiControls(0), m_totalCotrols(0), m_backDropId(-1) {}
    ~GUISystem() { shutDown(); }

    //扩大控件列表
    int increaseControls();

    bool addBackDrop(int texID, int staticID);
    bool addStaticText(int id, char *text, int x, int y, unsigned long color, int fontID);
    bool addButton(int id, int x, int y, int width, int height, int upID, int overID, int downID,unsigned int staticID);
    void shutDown();

    stGUIControl *getGUIControl(int id) {
        if (id < 0 || id >= m_totalCotrols) {
            return NULL;
        }
        return &m_guiControls[id];
    }

    int getTotalControls() {
        return m_totalCotrols;
    }

    stGUIControl *getBackDrop() {
        if (m_backDropId >= 0 && m_totalCotrols) {
            return &m_guiControls[m_backDropId];
        }
        return NULL;
    }
private:
    stGUIControl *m_guiControls;//控件列表
    int m_totalCotrols;//控件总数
    int m_backDropId;
};

#endif