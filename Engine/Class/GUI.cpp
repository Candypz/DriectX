#include "GUI.h"

int GUISystem::increaseControls() {
    if (!m_guiControls) {
        m_guiControls = new stGUIControl[1];
        if (!m_guiControls) {
            return DEFINES_FAIL;
        }
        memset(&m_guiControls[0], 0, sizeof(stGUIControl));
    }
    else {
        stGUIControl *temp;
        temp = new stGUIControl[m_totalCotrols + 1];
        if (!temp) {
            return DEFINES_FAIL;
        }
        memset(temp, 0, sizeof(stGUIControl)*(m_totalCotrols + 1));
        memcpy(temp, m_guiControls, sizeof(stGUIControl)*m_totalCotrols);
        delete[] m_guiControls;
        m_guiControls = temp;
    }
    return DEFINES_OK;
}

bool GUISystem::addBackDrop(int texID, int staticID) {
    if (texID < 0 || staticID < 0) {
        return false;
    }
    if (m_backDropId < 0) {
        if (!increaseControls()) {
            return false;
        }
        m_guiControls[m_totalCotrols].m_type = GUI_BACKDROP;
        m_guiControls[m_totalCotrols].m_upTex = texID;
        m_guiControls[m_totalCotrols].m_listID = staticID;

        m_backDropId = m_totalCotrols;
        m_totalCotrols++;
    }
    else {//已经有了就替换
        m_guiControls[m_backDropId].m_upTex = texID;
        m_guiControls[m_backDropId].m_listID = staticID;
    }
    return true;
}

bool GUISystem::addStaticText(int id, char *text, int x, int y, unsigned long color, int fontID) {
    if (!text || fontID < 0) {
        return false;
    }
    if (!increaseControls()) {
        return false;
    }
    m_guiControls[m_totalCotrols].m_type = GUI_STATICTEXT;
    m_guiControls[m_totalCotrols].m_id = id;
    m_guiControls[m_totalCotrols].m_color = color;
    m_guiControls[m_totalCotrols].m_x = x;
    m_guiControls[m_totalCotrols].m_y = y;
    m_guiControls[m_totalCotrols].m_listID = fontID;

    int len = strlen(text);
    m_guiControls[m_totalCotrols].m_text = new char[len + 1];
    if (!m_guiControls) {
        return false;
    }
    memcpy(m_guiControls[m_totalCotrols].m_text, text, len);
    m_guiControls[m_totalCotrols].m_text[len] = '\0';

    m_totalCotrols++;
    return true;
}

bool GUISystem::addButton(int id, int x, int y, int width, int height, int upID, int overID, int downID, unsigned int staticID) {
    if (!increaseControls()) {
        return false;
    }
    m_guiControls[m_totalCotrols].m_type = GUI_BUTTON;
    m_guiControls[m_totalCotrols].m_id = id;
    m_guiControls[m_totalCotrols].m_x = x;
    m_guiControls[m_totalCotrols].m_y = y;
    m_guiControls[m_totalCotrols].m_width = width;
    m_guiControls[m_totalCotrols].m_height = height;
    m_guiControls[m_totalCotrols].m_upTex = upID;
    m_guiControls[m_totalCotrols].m_overTex = overID;
    m_guiControls[m_totalCotrols].m_downTex = downID;
    m_guiControls[m_totalCotrols].m_listID = staticID;

    m_totalCotrols++;

    return true;
}

void GUISystem::shutDown() {
    for (int i = 0; i < m_totalCotrols; ++i) {
        if (m_guiControls[i].m_text) {
            delete[] m_guiControls[i].m_text;
            m_guiControls[i].m_text = NULL;
        }
    }
    m_totalCotrols = 0;
    if (m_guiControls) {
        delete[] m_guiControls;
        m_guiControls = NULL;
    }
}