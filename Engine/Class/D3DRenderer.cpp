#include "D3DRenderer.h"

inline unsigned long ftoDw(float v) {
    return *((unsigned long*)&v);
}

bool createD3DRenderer(RenderInterface **pRender) {
    if (!*pRender) {
        *pRender = new D3DRenderer();
    }
    else {
        return false;
    }
    return true;
}

D3DMULTISAMPLE_TYPE getD3DMultiSampleType(LPDIRECT3D9 d3d, MS_TYPE ms, D3DDEVTYPE evType, D3DFORMAT format, bool fullscreen) {
    D3DMULTISAMPLE_TYPE type = D3DMULTISAMPLE_NONE;
    if (d3d) {
        switch (ms) {
            case  MS_NONE:
                type = D3DMULTISAMPLE_NONE;
                break;
            case MS_SAMPLES_2:
                if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, evType, format, !fullscreen, D3DMULTISAMPLE_2_SAMPLES, NULL) == D3D_OK) {
                    type = D3DMULTISAMPLE_2_SAMPLES;
                }
                break;
            case MS_SAMPLES_4:
                if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, evType, format, !fullscreen, D3DMULTISAMPLE_4_SAMPLES, NULL) == D3D_OK) {
                    type = D3DMULTISAMPLE_4_SAMPLES;
                }
                break;
            case MS_SAMPLES_8:
                if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, evType, format, !fullscreen, D3DMULTISAMPLE_8_SAMPLES, NULL) == D3D_OK) {
                    type = D3DMULTISAMPLE_8_SAMPLES;
                }
                break;
            case MS_SAMPLES_16:
                if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, evType, format, !fullscreen, D3DMULTISAMPLE_16_SAMPLES, NULL) == D3D_OK) {
                    type = D3DMULTISAMPLE_16_SAMPLES;
                }
                break;
        }
    }
    return type;
}

//根据顶点类型创建不通的fvf
unsigned long createD3DFvf(int flags) {
    unsigned long fvf = 0;
    if (flags == GUI_FVF) {
        fvf = D3DFVF_GUI;
    }
    if (flags == MV_FVF) {
        fvf = D3DFVF_MV;
    }
    return fvf;
}

D3DRenderer::D3DRenderer() {
    m_direct3D = NULL;
    m_device = NULL;
    m_renderingScene = false;
    m_numStaticBuffers = 0;
    m_activeStaticBuffer = DEFINES_INVALID;
    m_staticBufferList = NULL;

    m_textureList = NULL;
    m_numTextures = 0;

    m_xModels = NULL;
    m_numXModels = 0;

    m_fonts = NULL;
    m_totlFonts = 0;

    m_guiList = NULL;
    m_totalGUIs = 0;
}

D3DRenderer::~D3DRenderer() {
    shutDown();
}

bool D3DRenderer::initialize(int w, int h, WinHWND hWnd, bool full, MS_TYPE ms) {
    //重新开始时关闭
    shutDown();

    m_mainHandle = hWnd;
    if (!m_mainHandle) {
        return false;
    }
    m_fullscreen = full;

    D3DDISPLAYMODE mode;//显示模式
    D3DCAPS9 caps;//查询显卡功能
    D3DPRESENT_PARAMETERS params;//显示参数

    ZeroMemory(&params, sizeof(params));

    m_direct3D = Direct3DCreate9(D3D_SDK_VERSION);//创建d3d对象
    if (!m_direct3D) {
        return false;
    }

    if (FAILED(m_direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode))) {
        return false;
    }

    if (FAILED(m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps))) {
        return false;
    }

    DWORD processing = 0;
    //是否有硬件处理能力
    if (caps.VertexProcessingCaps != 0) {
        processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    }
    else {
        processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    //全屏
    if (m_fullscreen) {
        params.FullScreen_RefreshRateInHz = mode.RefreshRate;
        //显示速率
        params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    }
    else {
        //立即显示
        params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    params.Windowed = !m_fullscreen;
    params.BackBufferWidth = w;
    params.BackBufferHeight = h;
    params.hDeviceWindow = m_mainHandle;//游戏窗口
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换
    params.BackBufferFormat = mode.Format;//显示模式
    params.BackBufferCount = 1;
    params.EnableAutoDepthStencil = TRUE;
    params.AutoDepthStencilFormat = D3DFMT_D16;//格式
    params.MultiSampleType = getD3DMultiSampleType(m_direct3D, ms, D3DDEVTYPE_HAL, mode.Format, m_fullscreen);

    m_screenWidth = w;
    m_screenHeight = h;

    if (FAILED(m_direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_mainHandle, processing, &params, &m_device))) {
        return false;
    }

    if (m_device == NULL) {
        return false;
    }

    oneTimeInit();

    return true;
}

void D3DRenderer::oneTimeInit() {
    if (!m_device) {
        return;
    }
    //渲染状态
    m_device->SetRenderState(D3DRS_LIGHTING, false);
    m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    calculateProjMatrix(D3DX_PI / 4, 0.1f, 1000);
}

void D3DRenderer::calculateProjMatrix(float fov, float n, float f) {
    if (!m_device) {
        return;
    }
    D3DXMATRIX projection;

    D3DXMatrixPerspectiveFovLH(&projection, fov, (float)(m_screenWidth / m_screenHeight), n, f);
    m_device->SetTransform(D3DTS_PROJECTION, &projection);
}

void D3DRenderer::calculateOrthoMatrix(float n, float f) {
    if (!m_device) {
        return;
    }
    D3DXMATRIX ortho;
    D3DXMatrixOrthoLH(&ortho, (float)m_screenWidth, (float)m_screenHeight, n, f);
    m_device->SetTransform(D3DTS_PROJECTION, &ortho);
}

void D3DRenderer::setClearColor(float r, float g, float b) {
    m_color = D3DCOLOR_COLORVALUE(r, g, b, 1.0f);
}

void D3DRenderer::startRender(bool bColor, bool bDepth, bool bStencil) {
    if (!m_device) {
        return;
    }

    //决定是否清除传入的是否
    unsigned int buffers = 0;
    if (bColor) {
        buffers |= D3DCLEAR_TARGET;
    }
    if (bDepth) {
        buffers |= D3DCLEAR_ZBUFFER;
    }
    if (bStencil) {
        buffers |= D3DCLEAR_STENCIL;
    }

    if (FAILED(m_device->Clear(0, NULL, buffers, m_color, 1, 0))) {
        return;
    }
    if (FAILED(m_device->BeginScene())) {
        return;
    }
    m_renderingScene = true;
}

void D3DRenderer::endRender() {
    if (!m_device) {
        return;
    }

    m_device->EndScene();
    //显示场景
    m_device->Present(NULL, NULL, NULL, NULL);
    m_renderingScene = false;
}

void D3DRenderer::clearBuffers(bool bColor, bool bDepth, bool bStencil) {
    if (!m_device) {
        return;
    }

    //决定是否清除传入的是否
    unsigned int buffers = 0;
    if (bColor) {
        buffers |= D3DCLEAR_TARGET;
    }
    if (bDepth) {
        buffers |= D3DCLEAR_ZBUFFER;
    }
    if (bStencil) {
        buffers |= D3DCLEAR_STENCIL;
    }
    if (m_renderingScene) {
        m_device->EndScene();
    }

    if (FAILED(m_device->Clear(0, NULL, buffers, m_color, 1.0f, 0))) {
        return;
    }

    if (m_renderingScene) {
        if (FAILED(m_device->BeginScene())) {
            return;
        }
    }
}

int D3DRenderer::createStaticBuffer(VertexType vType, PrimType primType, int totalVerts, int totalIndices, int stride, void **data, unsigned int *indices, int *staticId) {
    int index = m_numStaticBuffers;
    void **ptr;

    if (!m_staticBufferList) {
        m_staticBufferList = new stD3DStaticBuffer[1];
        if (!m_staticBufferList) {
            return DEFINES_FAIL;
        }
    }
    else {
        //动态数组
        stD3DStaticBuffer *temp;
        temp = new stD3DStaticBuffer[m_numStaticBuffers + 1];

        memcpy(temp, m_staticBufferList, sizeof(stD3DStaticBuffer)*m_numStaticBuffers);
        delete[] m_staticBufferList;
        m_staticBufferList = temp;
    }
    m_staticBufferList[index].nubVerts = totalVerts;
    m_staticBufferList[index].numIndices = totalIndices;
    m_staticBufferList[index].primType = primType;
    m_staticBufferList[index].stride = stride;
    m_staticBufferList[index].fvf = createD3DFvf(vType);

    if (totalIndices > 0) {
        //创建顶点缓存
        if (FAILED(m_device->CreateIndexBuffer(sizeof(unsigned int)*totalIndices, D3DUSAGE_WRITEONLY, D3DFMT_D16, D3DPOOL_DEFAULT,
            &m_staticBufferList[index].ibPtr, NULL))) {
            return DEFINES_FAIL;
        }
        if (FAILED(m_staticBufferList[index].ibPtr->Lock(0, 0, (void**)&ptr, 0))) {
            return DEFINES_FAIL;
        }
        memcpy(ptr, indices, sizeof(unsigned int)*totalIndices);
        m_staticBufferList[index].ibPtr->Unlock();
    }
    else {
        m_staticBufferList[index].ibPtr = NULL;
    }
    if (FAILED(m_device->CreateVertexBuffer(totalVerts*stride, D3DUSAGE_WRITEONLY, m_staticBufferList[index].fvf,
        D3DPOOL_DEFAULT, &m_staticBufferList[index].vbPtr, NULL))) {
        return DEFINES_FAIL;
    }

    if (m_staticBufferList[index].vbPtr->Lock(0, 0, (void**)&ptr, 0)) {
        return DEFINES_FAIL;
    }
    memcpy(ptr, data, totalVerts*stride);
    m_staticBufferList[index].vbPtr->Unlock();

    //最新的
    *staticId = m_numStaticBuffers;
    m_numStaticBuffers++;//为下一次添加图形
    return DEFINES_OK;
}

void D3DRenderer::shutDown() {
    releaseAllXModels();
    releaseAllStaticBuffers();

    for (int i = 0; i < m_totalGUIs; ++i) {
        m_guiList[i].shutDown();
    }
    m_totalGUIs = 0;
    if (m_guiList) {
        delete[]m_guiList;
        m_guiList = NULL;
    }

    for (int i = 0; i < m_totlFonts; ++i) {
        if (m_fonts[i]) {
            m_fonts[i]->Release();
            m_fonts[i] = NULL;
        }
    }
    m_totlFonts = 0;
    if (m_fonts) {
        delete[] m_fonts;
        m_fonts = NULL;
    }

    for (unsigned int i = 0; i < m_numTextures; ++i) {
        if (m_textureList[i].fileName) {
            delete[]m_textureList[i].fileName;
            m_textureList[i].fileName = NULL;
        }
        if (m_textureList[i].image) {
            m_textureList[i].image->Release();
            m_textureList[i].image = NULL;
        }
    }
    m_numTextures = 0;
    if (m_textureList) {
        delete[] m_textureList;
        m_textureList = NULL;
    }

    if (m_device) {
        m_device->Release();
        m_device = NULL;
    }
    if (m_direct3D) {
        m_direct3D->Release();
        m_direct3D = NULL;
    }
}

int D3DRenderer::endRendering(int staticId) {
    /*m_device->Clear();
    m_device->BeginScene();

    m_device->SetIndices();
    m_device->SetStreamSource();
    m_device->SetFVF();
    m_device->DrawIndexedPrimitive();
    m_device->DrawPrimitive();


    m_device->EndScene();
    m_device->Present();
    */
    if (staticId >= m_numStaticBuffers) {
        return DEFINES_FAIL;
    }
    if (m_activeStaticBuffer != staticId) {
        if (m_staticBufferList[staticId].ibPtr != NULL) {
            m_device->SetIndices(m_staticBufferList[staticId].ibPtr);
        }

        m_device->SetStreamSource(0, m_staticBufferList[staticId].vbPtr, 0, m_staticBufferList[staticId].stride);
        m_device->SetFVF(m_staticBufferList[staticId].fvf);

        m_activeStaticBuffer = staticId;
    }
    if (m_staticBufferList[staticId].ibPtr != NULL) {
        switch (m_staticBufferList[staticId].primType) {
            case POINT_LIST:
                if (FAILED(m_device->DrawPrimitive(D3DPT_POINTLIST, 0, m_staticBufferList[staticId].nubVerts))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_LIST:
                if (FAILED(m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_staticBufferList[staticId].nubVerts / 3, 0, m_staticBufferList[staticId].numIndices))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_STRIP:
                if (FAILED(m_device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_staticBufferList[staticId].nubVerts / 2, 0, m_staticBufferList[staticId].numIndices))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_FAN:
                if (FAILED(m_device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_staticBufferList[staticId].nubVerts / 2, 0, m_staticBufferList[staticId].numIndices))) {
                    return DEFINES_FAIL;
                }
                break;
            case  LINE_LIST:
                if (FAILED(m_device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_staticBufferList[staticId].nubVerts / 2, 0, m_staticBufferList[staticId].numIndices))) {
                    return DEFINES_FAIL;
                }
                break;
            case  LINE_STRIP:
                if (FAILED(m_device->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_staticBufferList[staticId].nubVerts, 0, m_staticBufferList[staticId].numIndices))) {
                    return DEFINES_FAIL;
                }
                break;
            default:
                return DEFINES_FAIL;
                break;
        }
    }
    else {
        switch (m_staticBufferList[staticId].primType) {
            case POINT_LIST:
                if (FAILED(m_device->DrawPrimitive(D3DPT_POINTLIST, 0, m_staticBufferList[staticId].nubVerts))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_LIST:
                if (FAILED(m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (int)(m_staticBufferList[staticId].nubVerts) / 3))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_STRIP:
                if (FAILED(m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, (int)(m_staticBufferList[staticId].nubVerts) / 2))) {
                    return DEFINES_FAIL;
                }
                break;
            case  TRIANGLE_FAN:
                if (FAILED(m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, (int)(m_staticBufferList[staticId].nubVerts) / 2))) {
                    return DEFINES_FAIL;
                }
                break;
            case  LINE_LIST:
                if (FAILED(m_device->DrawPrimitive(D3DPT_LINELIST, 0, (int)(m_staticBufferList[staticId].nubVerts) / 2))) {
                    return DEFINES_FAIL;
                }
                break;
            case  LINE_STRIP:
                if (FAILED(m_device->DrawPrimitive(D3DPT_LINESTRIP, 0, (int)(m_staticBufferList[staticId].nubVerts)))) {
                    return DEFINES_FAIL;
                }
                break;
            default:
                return DEFINES_FAIL;
                break;
        }
    }
    return DEFINES_OK;
}

void D3DRenderer::setMaterial(stMaterial *material) {
    if (!material || !m_device) {
        return;
    }
    D3DMATERIAL9 d3dMaterial = {
        material->diffuseR, material->diffuseG, material->diffuseB, material->diffuseA,
        material->ambientR, material->ambientG, material->ambientB, material->ambientA,
        material->specularR, material->specularG, material->specularB, material->specularA,
        material->emissiveR, material->emissiveG, material->emissiveB, material->emissiveA,
        material->power
    };
    m_device->SetMaterial(&d3dMaterial);
}

void D3DRenderer::setLight(stLight *light, int index) {
    if (!light || !m_device || index < 0) {
        return;
    }
    D3DLIGHT9 d3dLight;

    d3dLight.Ambient.a = light->ambientA;
    d3dLight.Ambient.r = light->ambientR;
    d3dLight.Ambient.g = light->ambientG;
    d3dLight.Ambient.b = light->ambientB;

    d3dLight.Attenuation0 = light->attenuation0;
    d3dLight.Attenuation1 = light->attenuation1;
    d3dLight.Attenuation2 = light->attenuation2;

    d3dLight.Diffuse.a = light->diffuseA;
    d3dLight.Diffuse.g = light->diffuseG;
    d3dLight.Diffuse.b = light->diffuseB;
    d3dLight.Diffuse.r = light->diffuseR;

    d3dLight.Direction.x = light->dirX;
    d3dLight.Direction.y = light->dirY;
    d3dLight.Direction.z = light->dirZ;

    d3dLight.Falloff = light->falloff;
    d3dLight.Phi = light->phi;

    d3dLight.Position.x = light->posX;
    d3dLight.Position.y = light->posY;
    d3dLight.Position.z = light->posZ;

    d3dLight.Range = light->range;

    d3dLight.Specular.a = light->specularA;
    d3dLight.Specular.r = light->specularR;
    d3dLight.Specular.g = light->specularG;
    d3dLight.Specular.b = light->specularB;

    d3dLight.Theta = light->theta;
    if (light->type == LIGHT_POINT) {
        d3dLight.Type = D3DLIGHT_POINT;
    }
    else if (light->type == LIGHT_SPOT) {
        d3dLight.Type = D3DLIGHT_SPOT;
    }
    else if (light->type == LIGHT_DIRECTIONAL) {
        d3dLight.Type = D3DLIGHT_DIRECTIONAL;
    }
    m_device->SetLight(index, &d3dLight);
    m_device->LightEnable(index, TRUE);
}

void D3DRenderer::disableLight(int index) {
    if (!m_device) {
        return;
    }
    m_device->LightEnable(index, FALSE);
}

void D3DRenderer::setTranspency(RenderState state, TransState src, TransState dst) {
    if (!m_device) {
        return;
    }
    if (state == TRANSARENCY_NONE) {
        m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);//关闭不透明度
        return;
    }
    if (state == TRANSARENCY_ENABLE) {
        m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//开启
        switch (src) {//原融合因子
            case TRANS_ZERO:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
                break;
            case TRANS_ONE:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                break;
            case TRANS_SRCCOLOR:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
                break;
            case TRANS_INVSRCCOLOR:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
                break;
            case TRANS_SRCALPHA:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                break;
            case TRANS_INVSRCALPHA:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
                break;
            case  TRANS_DSTALPHA:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
                break;
            case   TRANS_INVDSTCOLOR:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
                break;
            case  TRANS_SRCALPHASAT:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHASAT);
                break;
            case  TRANS_BOTHSRCALPHA:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHSRCALPHA);
                break;
            case   TRANS_INVBOTHSRCALPHA:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHINVSRCALPHA);
                break;
            case  TRANS_BLENDFACTOR:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
                break;
            case  TRANS_INVBLENDFACTOR:
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVBLENDFACTOR);
                break;
            default:
                m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);//关闭
                return;
                break;
        }

        switch (dst) {//目标融合因子
            case TRANS_ZERO:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
                break;
            case TRANS_ONE:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
                break;
            case TRANS_SRCCOLOR:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
                break;
            case TRANS_INVSRCCOLOR:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
                break;
            case TRANS_SRCALPHA:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                break;
            case TRANS_INVSRCALPHA:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                break;
            case  TRANS_DSTALPHA:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
                break;
            case   TRANS_INVDSTCOLOR:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
                break;
            case  TRANS_SRCALPHASAT:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
                break;
            case  TRANS_BOTHSRCALPHA:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHSRCALPHA);
                break;
            case   TRANS_INVBOTHSRCALPHA:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
                break;
            case  TRANS_BLENDFACTOR:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
                break;
            case  TRANS_INVBLENDFACTOR:
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
                break;
            default:
                m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);//关闭
                return;
                break;
        }
    }
}

int  D3DRenderer::addTexture2D(char *file, int *texId) {
    if (!file || !m_device) {
        return DEFINES_FAIL;
    }

    int len = strlen(file);
    if (!len) {
        return DEFINES_FAIL;
    }

    int index = m_numTextures;
    if (!m_textureList) {
        m_textureList = new stD3DTexture[1];
        if (!m_textureList) {
            return DEFINES_FAIL;
        }
    }
    else {
        stD3DTexture *temp;
        temp = new stD3DTexture[m_numTextures + 1];
        memcpy(temp, m_textureList, sizeof(stD3DTexture)*m_numTextures);
        delete[]m_textureList;
        m_textureList = temp;
    }
    m_textureList[index].fileName = new char[len];
    memcpy(m_textureList[index].fileName, file, len);

    D3DCOLOR colorkey = 0xff000000;
    D3DXIMAGE_INFO info;

    if (D3DXCreateTextureFromFileEx(m_device, file, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, colorkey, &info, NULL, &m_textureList[index].image) != D3D_OK) {
        return DEFINES_FAIL;
    }
    m_textureList[index].width = info.Width;
    m_textureList[index].height = info.Height;

    *texId = m_numTextures;
    m_numTextures++;

    return DEFINES_OK;
}

void D3DRenderer::setTextureFilter(int index, int filter, int val) {
    if (!m_device || index < 0) {
        return;
    }
    D3DSAMPLERSTATETYPE fil = D3DSAMP_MINFILTER;
    int v = D3DTEXF_POINT;//过滤器类型

    if (filter == MIN_FILTER) {
        fil = D3DSAMP_MINFILTER;
    }
    if (filter == MAG_FILTER) {
        fil = D3DSAMP_MAGFILTER;
    }
    if (filter == MIP_FILTER) {
        fil = D3DSAMP_MIPFILTER;
    }


    if (val == POINT_TYPE) {
        v = D3DTEXF_POINT;
    }
    if (val == LINEAR_TYPE) {
        v = D3DTEXF_LINEAR;
    }
    if (val == ANISOTROPIC_TYPE) {
        v = D3DTEXF_ANISOTROPIC;
    }
    //采样索引 过滤器 过滤器类型
    m_device->SetSamplerState(index, fil, v);
}

void D3DRenderer::setMultiTexture() {
    if (!m_device) {
        return;
    }
    m_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//颜色相乘
    m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//纹理颜色
    m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);//漫反射颜色

    m_device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);//颜色相乘
    m_device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);//纹理颜色
    m_device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);//上一个的结果

}

void D3DRenderer::applyTexture(int index, int texId) {
    if (!m_device) {
        return;
    }
    if (index < 0 || texId < 0) {
        m_device->SetTexture(0, NULL);
    }
    else {
        m_device->SetTexture(index, m_textureList[texId].image);
    }
}

void D3DRenderer::saveScreenShot(char *file) {
    if (!file) {
        return;
    }

    LPDIRECT3DSURFACE9 surface = NULL;
    D3DDISPLAYMODE disp;

    m_device->GetDisplayMode(0, &disp);
    m_device->CreateOffscreenPlainSurface(disp.Width, disp.Height, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &surface, NULL);
    m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
    D3DXSaveSurfaceToFile(file, D3DXIFF_JPG, surface, NULL, NULL);

    if (surface != NULL) {
        surface->Release();
    }
    surface = NULL;
}

void D3DRenderer::enablePointSprites(float size, float min, float a, float b, float c) {
    if (!m_device) {
        return;
    }
    m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);//激活点精灵
    m_device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
    m_device->SetRenderState(D3DRS_POINTSIZE, ftoDw(size));
    m_device->SetRenderState(D3DRS_POINTSIZE_MIN, ftoDw(min));
    m_device->SetRenderState(D3DRS_POINTSCALE_A, ftoDw(a));
    m_device->SetRenderState(D3DRS_POINTSCALE_B, ftoDw(b));
    m_device->SetRenderState(D3DRS_POINTSCALE_C, ftoDw(c));
}

void D3DRenderer::disablePointSprites() {
    m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
    m_device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);

}

void D3DRenderer::setNormalmap(int index, int texId) {
    if (!m_device) {
        return;
    }
    if (index < 0 || texId < 0) {
        m_device->SetTexture(0, NULL);
    }
    else {
        D3DSURFACE_DESC desc;
        LPDIRECT3DTEXTURE9 normalTexture;
        m_textureList[texId].image->GetLevelDesc(0, &desc);
        if (D3DXCreateTexture(m_device, desc.Width, desc.Height, 0, 0, D3DFMT_A8B8G8R8, D3DPOOL_MANAGED, &normalTexture) != D3D_OK) {
            return;
        }
        if (D3DXComputeNormalMap(normalTexture, m_textureList[texId].image, 0, D3DX_NORMALMAP_MIRROR, D3DX_CHANNEL_GREEN, 10) != D3D_OK) {
            return;
        }
        m_device->SetTexture(index, normalTexture);
        if (normalTexture) {
            normalTexture->Release();
            normalTexture = NULL;
        }
    }
}

bool D3DRenderer::addGUIBackdrop(int guiID, char *fileName) {
    if (guiID >= m_totalGUIs) {
        return false;
    }
    int texID = -1;
    int staticID = -1;

    if (!addTexture2D(fileName, &texID)) {
        return false;
    }

    unsigned long color = D3DCOLOR_XRGB(255, 255, 255);

    stGUIVerter obj[] = {
        { (float)m_screenWidth, 0, 0, 1, color, 1, 0 },
        { (float)m_screenWidth, (float)m_screenHeight, 0, 1, color, 1, 1 },
        { 0, 0, 0, 1, color, 0, 0 },
        { 0, (float)m_screenHeight, 0, 1, color, 0, 1 },
    };

    if (!createStaticBuffer(GUI_FVF, TRIANGLE_STRIP, 4, 0, sizeof(stGUIVerter), (void**)&obj, NULL, &staticID)) {
        return false;
    }

    return m_guiList[guiID].addBackDrop(texID, staticID);
}

bool D3DRenderer::addGUIStaticText(int guiID, int id, char *text, int x, int y, unsigned long color, int fontID) {
    if (guiID >= m_totalGUIs) {
        return false;
    }

    return m_guiList[guiID].addStaticText(id, text, x, y, color, fontID);
}

bool D3DRenderer::addGUIButton(int guiID, int id, int x, int y, char *up, char *over, char *down) {
    if (guiID >= m_totalGUIs) {
        return false;
    }

    int upID = -1, downID = -1, overID = -1, staticID = -1;

    if (!addTexture2D(up, &upID)) {
        return false;
    }
    if (!addTexture2D(over, &overID)) {
        return false;
    }
    if (!addTexture2D(down, &downID)) {
        return false;
    }

    unsigned long color = D3DCOLOR_XRGB(255, 255, 255);

    int w = m_textureList[upID].width;
    int h = m_textureList[upID].height;

    stGUIVerter obj[] = {
        { (float)(w + x), (float)(0 + y), 0, 1, color, 1, 0 },
        { (float)(w + x), (float)(h + y), 0, 1, color, 1, 1 },
        { (float)(0 + x), (float)(0 + y), 0, 1, color, 0, 0 },
        { (float)(0 + x), (float)(h + y), 0, 1, color, 0, 1 },
    };

    if (!createStaticBuffer(GUI_FVF, TRIANGLE_STRIP, 4, 0, sizeof(stGUIVerter), (void**)&obj, NULL, &staticID)) {
        return false;
    }

    return m_guiList[guiID].addButton(id, x, y, w, h, upID, overID, downID, staticID);
}

void D3DRenderer::processGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state)) {
    if (guiID >= m_totalGUIs || !m_device) {
        return;
    }

    GUISystem *gui = &m_guiList[guiID];

    stGUIControl *backDrop = gui->getBackDrop();

    if (backDrop) {
        applyTexture(0, backDrop->m_upTex);
        endRendering(backDrop->m_listID);
        applyTexture(0, -1);
    }

    int status = GUI_BUTTON_UP;

    for (int i = 0; i < gui->getTotalControls(); ++i) {
        stGUIControl *pCont = gui->getGUIControl(i);
        if (!pCont) continue;

        switch (pCont->m_type) {
            case GUI_STATICTEXT:
                displayText(pCont->m_listID, pCont->m_x, pCont->m_y, pCont->m_color, pCont->m_text);
                break;
            case GUI_BUTTON:
                status = GUI_BUTTON_UP;

                m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
                m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);

                if (mouseX > pCont->m_x && mouseX < pCont->m_x + pCont->m_width &&
                    mouseY > pCont->m_y && mouseY < pCont->m_y + pCont->m_height) {
                    if (LMBDown) {
                        status = GUI_BUTTON_DOWN;
                    }
                    else {
                        status = GUI_BUTTON_OVER;
                    }
                }
                if (status == GUI_BUTTON_UP) {
                    applyTexture(0, pCont->m_upTex);
                }
                if (status == GUI_BUTTON_OVER) {
                    applyTexture(0, pCont->m_overTex);
                }
                if (status == GUI_BUTTON_DOWN) {
                    applyTexture(0, pCont->m_downTex);
                }

                endRendering(pCont->m_listID);

                m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
                break;
        }
        if (funcPtr) {//调用回掉函数
            funcPtr(pCont->m_id, status);
        }
    }
}

bool D3DRenderer::createText(char *font, int weight, bool italic, int size, int &id) {
    if (!m_fonts) {
        m_fonts = new LPD3DXFONT[1];
        if (!m_fonts) {
            return false;
        }
    }
    else {
        LPD3DXFONT *temp;
        temp = new LPD3DXFONT[m_totlFonts + 1];
        memcpy(temp, m_fonts, sizeof(LPD3DXFONT)*m_totlFonts);
        delete[] m_fonts;
        m_fonts = temp;
    }
    if (FAILED(D3DXCreateFont(m_device, size, 0, weight, 1, italic, 0, 0, 0, 0, font, &m_fonts[m_totlFonts]))) {
        return false;
    }
    id = m_totlFonts;
    m_totlFonts++;

    return true;
}

//显示文本信息
void D3DRenderer::displayText(int id, long x, long y, int r, int g, int b, char *text, ...) {
    RECT fontPosition = { x, y, m_screenWidth, m_screenHeight };
    char message[1024];
    va_list argList;
    if (id >= m_totlFonts) {
        return;
    }
    va_start(argList, text);
    vsprintf_s(message, text, argList);
    va_end(argList);

    m_fonts[id]->DrawText(NULL, message, -1, &fontPosition, DT_SINGLELINE, D3DCOLOR_ARGB(255, r, g, b));
}

void D3DRenderer::displayText(int id, long x, long y, unsigned long color, char *text, ...) {
    RECT fontPosition = { x, y, m_screenWidth, m_screenHeight };
    char message[1024];
    va_list argList;
    if (id >= m_totlFonts) {
        return;
    }
    va_start(argList, text);
    vsprintf_s(message, text, argList);
    va_end(argList);

    m_fonts[id]->DrawText(NULL, message, -1, &fontPosition, DT_SINGLELINE, color);
}

void D3DRenderer::enableFog(float start, float end, FOG_TYPE type, unsigned long color, bool rangeFog) {
    if (!m_device) {
        return;
    }
    D3DCAPS9 caps;
    m_device->GetDeviceCaps(&caps);

    m_device->SetRenderState(D3DRS_FOGENABLE, TRUE);
    m_device->SetRenderState(D3DRS_FOGCOLOR, color);

    m_device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
    m_device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&end));

    if (type == VERTEX_FOG) {
        m_device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    }
    else {
        //表格
        m_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
    }

    if (caps.RasterCaps&D3DPRASTERCAPS_FOGRANGE) {
        if (rangeFog) {
            m_device->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
        }
        else {
            m_device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
        }
    }

}

void D3DRenderer::disableFog() {
    if (!m_device) {
        return;
    }
    m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void D3DRenderer::setDetailMapping() {
    if (!m_device) {
        return;
    }
    m_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_device->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_TEXTURE);
    m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
    m_device->SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_TEXTURE);
    m_device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
}

int  D3DRenderer::loadXMode(char *file, int xModelID) {
    if (!file || xModelID < 0 || xModelID >= m_numXModels || !m_xModels) {
        return DEFINES_FAIL;
    }
    m_xModels[xModelID].shoutDown();

    LPD3DXBUFFER matBuffer = NULL;
    unsigned long numMats = 0;

    if (FAILED(D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, m_device, NULL, &matBuffer, NULL, &numMats, &m_xModels[xModelID].model))) {
        return DEFINES_FAIL;
    }
    m_xModels[xModelID].numMaterials = numMats;
    m_xModels[xModelID].matList = new D3DMATERIAL9[numMats];
    m_xModels[xModelID].textureList = new LPDIRECT3DTEXTURE9[numMats];

    D3DXMATERIAL *mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
    for (unsigned long i = 0; i < numMats; ++i) {
        m_xModels[xModelID].matList[i] = mat[i].MatD3D;

        if (FAILED(D3DXCreateTextureFromFile(m_device, mat[i].pTextureFilename, &m_xModels[xModelID].textureList[i]))) {
            m_xModels[xModelID].textureList[i] = NULL;
        }
    }
    if (matBuffer != NULL) {
        matBuffer->Release();
        matBuffer = NULL;
    }

    return DEFINES_OK;

}

int  D3DRenderer::loadXMode(char *file, int *xModelID) {
    if (!file) {
        return DEFINES_FAIL;
    }
    if (!m_xModels) {
        m_xModels = new XModel[1];
        if (!m_xModels) {
            return DEFINES_FAIL;
        }
    }
    else {
        XModel *temp;
        temp = new XModel[m_numXModels + 1];
        memcpy(temp, m_xModels, sizeof(XModel)*m_numXModels);
        delete[]m_xModels;
        m_xModels = temp;
    }
    LPD3DXBUFFER matBuffer = NULL;
    unsigned long numMats = 0;

    if (FAILED(D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, m_device, NULL, &matBuffer, NULL, &numMats, &m_xModels[m_numXModels].model))) {
        return DEFINES_FAIL;
    }
    m_xModels[m_numXModels].numMaterials = numMats;
    m_xModels[m_numXModels].matList = new D3DMATERIAL9[numMats];
    m_xModels[m_numXModels].textureList = new LPDIRECT3DTEXTURE9[numMats];

    D3DXMATERIAL *mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
    for (unsigned long i = 0; i < numMats; ++i) {
        m_xModels[m_numXModels].matList[i] = mat[i].MatD3D;

        if (FAILED(D3DXCreateTextureFromFile(m_device, mat[i].pTextureFilename, &m_xModels[m_numXModels].textureList[i]))) {
            m_xModels[m_numXModels].textureList[i] = NULL;
        }
    }
    if (matBuffer != NULL) {
        matBuffer->Release();
        matBuffer = NULL;
    }
    *xModelID = m_numXModels;
    m_numXModels++;

    return DEFINES_OK;
}

int D3DRenderer::renderXMode(int xModelID) {
    if (!m_xModels || xModelID>m_numXModels || xModelID < 0) {
        return DEFINES_FAIL;
    }
    m_device->SetIndices(NULL);
    m_device->SetStreamSource(0, NULL, 0, 0);
    m_device->SetFVF(0);

    for (unsigned long i = 0; i < m_xModels[xModelID].numMaterials; ++i) {
        m_device->SetMaterial(&m_xModels[xModelID].matList[i]);
        m_device->SetTexture(0, m_xModels[xModelID].textureList[i]);
        m_xModels[xModelID].model->DrawSubset(i);
    }
    return DEFINES_OK;
}

int  D3DRenderer::releaseXMode(int xModelID) {
    if (!m_xModels || xModelID >= m_numXModels || xModelID < 0) {
        return DEFINES_FAIL;
    }
    m_xModels[xModelID].shoutDown();

    return DEFINES_OK;
}

void D3DRenderer::releaseAllXModels() {
    for (int i = 0; i < m_numXModels; ++i) {
        m_xModels[i].shoutDown();
    }
    m_numXModels = 0;
    if (m_xModels) {
        delete[]m_xModels;
        m_xModels = NULL;
    }
}

void D3DRenderer::releaseAllStaticBuffers() {
    for (int i = 0; i < m_numStaticBuffers; ++i) {
        releaseStaticBuffer(i);
    }
    m_numStaticBuffers = 0;
    if (m_numStaticBuffers) {
        delete[] m_staticBufferList;
        m_staticBufferList = NULL;
    }
}


int  D3DRenderer::releaseStaticBuffer(int staticID) {
    if (staticID < 0 || staticID >= m_numStaticBuffers || !m_staticBufferList) {
        return DEFINES_FAIL;
    }

    if (m_staticBufferList[staticID].vbPtr) {
        m_staticBufferList[staticID].vbPtr->Release();
        m_staticBufferList[staticID].vbPtr = NULL;
    }
    if (m_staticBufferList[staticID].ibPtr) {
        m_staticBufferList[staticID].ibPtr->Release();
        m_staticBufferList[staticID].ibPtr = NULL;

    }
    m_numStaticBuffers = 0;
    if (m_staticBufferList) {
        delete[] m_staticBufferList;
        m_staticBufferList = NULL;
    }
    return DEFINES_OK;
}