#ifndef _LIGHT_H_
#define _LIGHT_H_

struct stLight {
    stLight() {
        type = 0;
        posX = 0; posY = 0; posZ = 0;
        dirX = 0; dirY = 0; dirZ = 0;
        ambientR = ambientG = ambientB = ambientA = 1.0f;
        diffuseR = diffuseG = diffuseB = diffuseA = 1.0f;
        specularR = specularG = specularB = specularA = 0;
        range = 0;
        falloff = 0;
        attenuation0 = 0; attenuation1 = 0; attenuation2 = 0;
        theta = 0;
        phi = 0;
    }

    int type;//类型
    float posX, posY, posZ;//位置
    float dirX, dirY, dirZ;//方向
    float ambientR, ambientG, ambientB, ambientA;//环境光
    float diffuseR, diffuseG, diffuseB, diffuseA;//漫射光
    float specularR, specularG, specularB, specularA;//镜面光
    float range;//光层
    float falloff;//光墙
    float attenuation0;//点光源和聚光灯
    float attenuation1;
    float attenuation2;
    float theta;//聚光灯,内部锥形
    float phi;//外部锥形的圆锥角
};

#endif