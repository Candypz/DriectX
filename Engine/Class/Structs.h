#ifndef _STRUCTS_H_
#define _STRUCTS_H_

//GUI����ṹ
struct stGUIVerter {
    float x, y, z, rhw;
    unsigned long color;
    float u, v;
};

//xģ�Ͷ���
struct stModelVertex {
    float x, y, z;
    float nx, ny, nz;//���㷨������
    unsigned long color;
    float u, v;
};




#endif