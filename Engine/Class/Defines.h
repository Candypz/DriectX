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
//��Դ����
#define LIGHT_POINT       1
#define LIGHT_SPOT        2
#define LIGHT_DIRECTIONAL 3

//GUI
#define PERCENT_OF(a,b) (int)(a*b)
#define UGPCOLOR_ARGB(a,r,g,b) ((unsigned long)((((a)&0xff)<<24)|\
							   (((r)&0xff)<<16)|(((g)&0xff)<<8)|\
							   ((b)&0xff)))
#define GUI_FVF         2
#define MV_FVF          3//ģ�Ͷ���

#define GUI_STATICTEXT  1//��̬�ı�
#define GUI_BUTTON      2//��ť
#define GUI_BACKDROP    3//����

#define GUI_BUTTON_UP   1
#define GUI_BUTTON_OVER 2
#define GUI_BUTTON_DOWN 3

#define KEYS_SIZE       256

#define D3DFVF_GUI (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_MV  (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//��������
typedef long VertexType;

//ͼԴ����
enum PrimType {
    NULL_TYPE,
    POINT_LIST,
    TRIANGLE_LIST,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    LINE_LIST,
    LINE_STRIP
};

//��Ⱦ״̬
enum RenderState {
    CULL_NONE,//�޼���
    CULL_CW,//˳ʱ�����
    CULL_CCW,//��ʱ�����
    DEPTH_NONE,//����Ȼ���
    DEPTH_READONLY,//ֻ������Ȼ���
    DEPTH_READWRITE,//��д����Ȼ���
    SHADE_POINTS,//����ɫ
    SHADE_SOLIDTRI,//ʵ����������ɫ
    SHADE_WIRETR,//�������߿���ɫ
    SHADE_WIREPOLY,//������߿���ɫ
    TRANSARENCY_NONE,//��͸����
    TRANSARENCY_ENABLE,//����͸����
};

/*�����*/
enum MS_TYPE {
    MS_NONE,
    MS_SAMPLES_2,
    MS_SAMPLES_4,
    MS_SAMPLES_8,
    MS_SAMPLES_16,
};

/*�������*/
enum FOG_TYPE {
    VERTEX_FOG,
    PIXEL_FOG,
};

//����͸����,�ں�����
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

//����״̬
enum TextureState {
    MIN_FILTER,//��С
    MAG_FILTER,//�Ŵ�
    MIP_FILTER,//�༶����
};

//����������
enum FilterType {
    POINT_TYPE,
    LINEAR_TYPE,
    ANISOTROPIC_TYPE//�������Թ�����
};

#endif