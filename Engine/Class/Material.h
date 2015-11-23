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

    float ambientR, ambientG, ambientB, ambientA;//������
    float diffuseR, diffuseG, diffuseB, diffuseA;//�����
    float specularR, specularG, specularB, specularA;//�����
    float emissiveR, emissiveG, emissiveB, emissiveA;//�������ȣ��Է���
    float power;//���
};


#endif