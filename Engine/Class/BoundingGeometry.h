#ifndef _BOUNDING_GEOMETRY_H_
#define _BOUNDING_GEOMETRY_H_

#include "MathLibrary.h"

class BoundingBase {
public:
    BoundingBase();
    virtual ~BoundingBase();
    virtual void createFromPoints(CVector3 *pointList, int numPoints) = 0;
    virtual bool isPointInside(CVector3 &v) = 0;

    //�Ƿ��ཻ
    virtual bool intersect(CRay ray, float *dist) = 0;
    virtual bool intersect(CRay ray, float length,float *dist) = 0;
    
    //��ȡÿ����
    virtual void getPlanes(CPlane *plane) = 0;
    //һ�������Ƿ��ڱ߽�ͼ����
    virtual bool isRayInside(CRay &ray, float length) = 0;
};

class BoudingBox : public BoundingBase {
public:
    BoudingBox() {}
    ~BoudingBox() {}
    void createFromPoints(CVector3 *pointList, int numPoints);
    bool isPointInside(CVector3 &v);
    bool intersect(CRay ray, float *dist);
    bool intersect(CRay ray, float length, float *dist);
    void getPlanes(CPlane *plane);
    bool isRayInside(CRay &ray, float length);
    CVector3 m_min, m_max;
};


class BoundingSphere : public BoundingBase {
public:
    BoundingSphere() :m_radius(0) {}
    ~BoundingSphere() {}
    void createFromPoints(CVector3 *pointList, int numPoints);
    bool isPointInside(CVector3 &v);
    bool intersect(CRay ray, float *dist);
    bool intersect(CRay ray, float length, float *dist);
    void getPlanes(CPlane *plane) {}
    bool isRayInside(CRay &ray, float length);

    CVector3 m_center;//����
    float m_radius;//�뾶
};

bool boxToBoxIntersect(BoudingBox &bb1, BoudingBox bb2);
bool sphereToSphereIntersect(BoundingSphere &bs1, BoundingSphere &bs2);

#endif