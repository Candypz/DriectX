#ifndef _STRUCTS_H_
#define _STRUCTS_H_

//GUI顶点结构
struct stGUIVerter {
    float x, y, z, rhw;
    unsigned long color;
    float u, v;
};

//x模型顶点
struct stModelVertex {
    float x, y, z;
    float nx, ny, nz;//顶点法线坐标
    unsigned long color;
    float u, v;
};




#endif