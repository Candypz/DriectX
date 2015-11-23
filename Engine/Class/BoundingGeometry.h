#ifndef _BOUNDING_GEOMETRY_H_
#define _BOUNDING_GEOMETRY_H_

#include "MathLibrary.h"

class BoundingBase {
public:
    BoundingBase();
    virtual ~BoundingBase();
    virtual void createFromPoints(CVector3 *pointList, int numPoints) = 0;
    virtual bool isPointInside(CVector3 &v) = 0;

    //是否相交
    virtual bool intersect(CRay ray, float *dist) = 0;
    virtual bool intersect(CRay ray, float length,float *dist) = 0;
    
    //获取每个面
    virtual void getPlanes(CPlane *plane) = 0;
    //一段射线是否在边界图形内
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

    CVector3 m_center;//球心
    float m_radius;//半径
};

bool boxToBoxIntersect(BoudingBox &bb1, BoudingBox bb2);
bool sphereToSphereIntersect(BoundingSphere &bs1, BoundingSphere &bs2);

#endif