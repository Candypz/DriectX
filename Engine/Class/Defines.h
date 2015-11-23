#ifndef _DEFINES_H_
#define _DEFINES_H_
#include <windows.h>
#include "Material.h"
#include "Light.h"

#define DEFINES_INVALID  -1
#define DEFINES_OK        1
#define DEFINES_FAIL      0
#define WinHWND           HWND

#define LEFT_MOUSE_BUTTON       0
#define RIGHT_MOUSE_BUTTON      1

#pragma  warning(disable:4996)
//光源类型
#define LIGHT_POINT       1
#define LIGHT_SPOT        2
#define LIGHT_DIRECTIONAL 3

//GUI
#define PERCENT_OF(a,b) (int)(a*b)
#define UGPCOLOR_ARGB(a,r,g,b) ((unsigned long)((((a)&0xff)<<24)|\
							   (((r)&0xff)<<16)|(((g)&0xff)<<8)|\
							   ((b)&0xff)))
#define GUI_FVF         2
#define MV_FVF          3//模型顶点

#define GUI_STATICTEXT  1//静态文本
#define GUI_BUTTON      2//按钮
#define GUI_BACKDROP    3//背景

#define GUI_BUTTON_UP   1
#define GUI_BUTTON_OVER 2
#define GUI_BUTTON_DOWN 3

#define KEYS_SIZE       256

#define D3DFVF_GUI (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_MV  (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//顶点类型
typedef long VertexType;

//图源类型
enum PrimType {
    NULL_TYPE,
    POINT_LIST,
    TRIANGLE_LIST,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    LINE_LIST,
    LINE_STRIP
};

//渲染状态
enum RenderState {
    CULL_NONE,//无剪裁
    CULL_CW,//顺时针剪裁
    CULL_CCW,//逆时针剪裁
    DEPTH_NONE,//无深度缓存
    DEPTH_READONLY,//只读的深度缓存
    DEPTH_READWRITE,//读写的深度缓存
    SHADE_POINTS,//点着色
    SHADE_SOLIDTRI,//实心三角形着色
    SHADE_WIRETR,//三角形线框着色
    SHADE_WIREPOLY,//多边形线框着色
    TRANSARENCY_NONE,//无透明度
    TRANSARENCY_ENABLE,//激活透明度
};

/*多采样*/
enum MS_TYPE {
    MS_NONE,
    MS_SAMPLES_2,
    MS_SAMPLES_4,
    MS_SAMPLES_8,
    MS_SAMPLES_16,
};

/*雾的类型*/
enum FOG_TYPE {
    VERTEX_FOG,
    PIXEL_FOG,
};

//纹理透明度,融合因子
enum TransState {
    TRANS_ZERO = 1,
    TRANS_ONE,
    TRANS_SRCCOLOR,
    TRANS_INVSRCCOLOR,
    TRANS_SRCALPHA,
    TRANS_INVSRCALPHA,
    TRANS_DSTALPHA,
    TRANS_INVDSTCOLOR,
    TRANS_SRCALPHASAT,
    TRANS_BOTHSRCALPHA,
    TRANS_INVBOTHSRCALPHA,
    TRANS_BLENDFACTOR,
    TRANS_INVBLENDFACTOR,
};

//纹理状态
enum TextureState {
    MIN_FILTER,//缩小
    MAG_FILTER,//放大
    MIP_FILTER,//多级过滤
};

//过滤器类型
enum FilterType {
    POINT_TYPE,
    LINEAR_TYPE,
    ANISOTROPIC_TYPE//各项异性过滤器
};

#endif