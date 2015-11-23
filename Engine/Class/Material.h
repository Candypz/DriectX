#ifndef _MATERIAL_H_
#define _MATERIAL_H_

struct stMaterial {
    stMaterial() {
        ambientR = ambientG = ambientB = ambientA = 1.0f;
        diffuseR = diffuseG = diffuseB = diffuseA = 1.0f;
        specularR = specularG = specularB = specularA = 0;
        emissiveR = emissiveG = emissiveB = emissiveA = 0;
        power = 0;
    }

    float ambientR, ambientG, ambientB, ambientA;//环境光
    float diffuseR, diffuseG, diffuseB, diffuseA;//漫射光
    float specularR, specularG, specularB, specularA;//镜面光
    float emissiveR, emissiveG, emissiveB, emissiveA;//物体亮度，自发光
    float power;//锐度
};


#endif