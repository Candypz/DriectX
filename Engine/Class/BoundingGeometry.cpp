#include "BoundingGeometry.h"

/*
Demo Name:  Game Project 8
Author:  Allen Sherrod
Chapter:  Chapter 9
*/


#include"BoundingGeometry.h"


void BoudingBox::createFromPoints(CVector3 *pointList, int numPoints) {
    // Loop through all of the points to find the min/max values.
    for (int i = 0; i < numPoints; i++) {
        if (pointList[i].x < m_min.x) m_min.x = pointList[i].x;
        if (pointList[i].x > m_max.x) m_max.x = pointList[i].x;

        if (pointList[i].y < m_min.y) m_min.y = pointList[i].y;
        if (pointList[i].y > m_max.y) m_max.y = pointList[i].y;

        if (pointList[i].z < m_min.z) m_min.z = pointList[i].z;
        if (pointList[i].z > m_max.z) m_max.z = pointList[i].z;
    }
}


bool BoudingBox::isPointInside(CVector3 &v) {
    if (m_max.x <= v.x) return false;
    if (m_min.x >= v.x) return false;
    if (m_max.y <= v.y) return false;
    if (m_min.y >= v.y) return false;
    if (m_max.z <= v.z) return false;
    if (m_min.z >= v.z) return false;

    return true;
}


bool BoudingBox::intersect(CRay ray, float *dist) {
    float t0, t1, temp;
    float min = -999999.9f;
    float max = 999999.9f;

    if (fabs(ray.m_direction.x) < 0.00001f) {
        if ((ray.m_origin.x < m_min.x) ||
            (ray.m_origin.x > m_max.x)) return false;
    }

    t0 = (m_min.x - ray.m_origin.x) / ray.m_direction.x;
    t1 = (m_max.x - ray.m_origin.x) / ray.m_direction.x;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max) max = t1;
    if (min > max) return false;
    if (max < 0) return false;


    if (fabs(ray.m_direction.y) < 0.00001f) {
        if ((ray.m_origin.y < m_min.y) ||
            (ray.m_origin.y > m_max.y)) return false;
    }

    t0 = (m_min.y - ray.m_origin.y) / ray.m_direction.y;
    t1 = (m_max.y - ray.m_origin.y) / ray.m_direction.y;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max)  max = t1;
    if (min > max) return false;
    if (max < 0) return false;


    if (fabs(ray.m_direction.z) < 0.00001f) {
        if ((ray.m_origin.z < m_min.z) ||
            (ray.m_origin.z > m_max.z)) return false;
    }

    t0 = (m_min.z - ray.m_origin.z) / ray.m_direction.z;
    t1 = (m_max.z - ray.m_origin.z) / ray.m_direction.z;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max) max = t1;
    if (min > max) return false;
    if (max < 0) return false;


    if (min > 0) if (dist) *dist = min;
    else if (dist) *dist = max;

    return true;
}


bool BoudingBox::intersect(CRay ray, float length, float *dist) {
    float t0, t1, temp;
    float min = -999999.9f;
    float max = 999999.9f;
    float d = 0;

    if (fabs(ray.m_direction.x) < 0.00001f) {
        if ((ray.m_origin.x < m_min.x) ||
            (ray.m_origin.x > m_max.x)) return false;
    }

    t0 = (m_min.x - ray.m_origin.x) / ray.m_direction.x;
    t1 = (m_max.x - ray.m_origin.x) / ray.m_direction.x;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max) max = t1;
    if (min > max) return false;
    if (max < 0) return false;


    if (fabs(ray.m_direction.y) < 0.00001f) {
        if ((ray.m_origin.y < m_min.y) ||
            (ray.m_origin.y > m_max.y)) return false;
    }

    t0 = (m_min.y - ray.m_origin.y) / ray.m_direction.y;
    t1 = (m_max.y - ray.m_origin.y) / ray.m_direction.y;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max)  max = t1;
    if (min > max) return false;
    if (max < 0) return false;


    if (fabs(ray.m_direction.z) < 0.00001f) {
        if ((ray.m_origin.z < m_min.z) ||
            (ray.m_origin.z > m_max.z)) return false;
    }

    t0 = (m_min.z - ray.m_origin.z) / ray.m_direction.z;
    t1 = (m_max.z - ray.m_origin.z) / ray.m_direction.z;

    if (t0 > t1) { temp = t0; t0 = t1; t1 = temp; }
    if (t0 > min) min = t0;
    if (t1 < max) max = t1;
    if (min > max) return false;
    if (max < 0) return false;

    if (min > 0) d = min;
    else d = max;

    if (d > length) return false;
    if (dist) *dist = d;

    return true;
}


void BoudingBox::getPlanes(CPlane *planes) {
    // Right.
    planes[0].a = 1.0f; planes[0].b = 0.0f; planes[0].c = 0.0f;
    planes[0].d = -(1 * m_max.x + 0 * m_max.y + 0 * m_max.z);

    // Left.
    planes[1].a = -1.0f; planes[1].b = 0.0f; planes[1].c = 0.0f;
    planes[1].d = -(-1 * m_min.x + 0 * m_min.y + 0 * m_min.z);

    // Front.
    planes[2].a = 0.0f; planes[2].b = 0.0f; planes[2].c = -1.0f;
    planes[2].d = -(0 * m_min.x + 0 * m_min.y + -1 * m_min.z);

    // Back.
    planes[3].a = 0.0f; planes[3].b = 0.0f; planes[3].c = 1.0f;
    planes[3].d = -(0 * m_max.x + 0 * m_max.y + 1 * m_max.z);

    // Top.
    planes[4].a = 0.0f; planes[4].b = 1.0f; planes[4].c = 0.0f;
    planes[4].d = -(0 * m_max.x + 1 * m_max.y + 0 * m_max.z);

    // Bottom.
    planes[5].a = 0.0f; planes[5].b = -1.0f; planes[5].c = 0.0f;
    planes[5].d = -(0 * m_min.x + -1 * m_min.y + 0 * m_min.z);
}


bool BoudingBox::isRayInside(CRay &ray, float length) {
    CVector3 endPos = ray.m_origin + (ray.m_direction * length);
    return (isPointInside(ray.m_origin) && isPointInside(endPos));
}


void BoundingSphere::createFromPoints(CVector3 *pointList, int numPoints) {
    CVector3 min, max;
    float dist = 0, maxDistance = 0.0f;

    // Loop through all of the points to find the min/max values.
    for (int i = 0; i < numPoints; i++) {
        if (pointList[i].x < min.x) min.x = pointList[i].x;
        if (pointList[i].x > max.x) max.x = pointList[i].x;

        if (pointList[i].y < min.y) min.y = pointList[i].y;
        if (pointList[i].y > max.y) max.y = pointList[i].y;

        if (pointList[i].z < min.z) min.z = pointList[i].z;
        if (pointList[i].z > max.z) max.z = pointList[i].z;
    }

    m_center = (max + min) * 0.5f;

    // Find max distance.
    for (int i = 0; i < numPoints; i++) {
        dist = ((pointList[i].x - m_center.x) * (pointList[i].x - m_center.x)) +
            ((pointList[i].y - m_center.y) * (pointList[i].y - m_center.y)) +
            ((pointList[i].z - m_center.z) * (pointList[i].z - m_center.z));

        if (dist > maxDistance)
            maxDistance = dist;
    }

    // Calculate radius.
    m_radius = sqrt(maxDistance);
}


bool BoundingSphere::isPointInside(CVector3 &v) {
    // The distance between the two spheres.
    CVector3 intersect = m_center - v;

    // Test for collision.
    if (sqrt(intersect.x * intersect.x + intersect.y * intersect.y +
        intersect.z * intersect.z) < m_radius)
        return true;

    return false;
}


bool BoundingSphere::intersect(CRay ray, float *dist) {
    CVector3 RayToSphereDir;

    float RayToSphereLength = 0.0f;
    float IntersectPoint = 0.0f;
    float SquaredPoint = 0.0f;

    // Get the direction of the ray to the object.
    RayToSphereDir = m_center - ray.m_origin;

    // Dot product the direction of the ray to current sphere to get the length.
    RayToSphereLength = RayToSphereDir.DotProduct3(RayToSphereDir);

    // Find intersect distance.
    IntersectPoint = RayToSphereDir.DotProduct3(ray.m_direction);

    // If true, no collision.
    if (IntersectPoint < 0) return false;

    // Get the squared sphere intersect distance.
    SquaredPoint = (m_radius * m_radius) - RayToSphereLength +
        (IntersectPoint * IntersectPoint);

    // If true, no collision.
    if (SquaredPoint < 0) return false;

    // Else it does hit the sphere and we record the results.
    if (dist) *dist = IntersectPoint - (float)sqrt(SquaredPoint);

    return true;
}


bool BoundingSphere::intersect(CRay ray, float length, float *dist) {
    CVector3 RayToSphereDir;

    float RayToSphereLength = 0.0f;
    float IntersectPoint = 0.0f;
    float SquaredPoint = 0.0f;

    // Get the direction of the ray to the object.
    RayToSphereDir = m_center - ray.m_origin;

    // Dot product the direction of the ray to current sphere to get the length.
    RayToSphereLength = RayToSphereDir.DotProduct3(RayToSphereDir);

    // Find intersect distance.
    IntersectPoint = RayToSphereDir.DotProduct3(ray.m_direction);

    // If true, no collision.
    if (IntersectPoint < 0) return false;

    // Get the squared sphere intersect distance.
    SquaredPoint = (m_radius * m_radius) - RayToSphereLength +
        (IntersectPoint * IntersectPoint);

    // If true, no collision.
    if (SquaredPoint < 0) return false;

    // Calculate intersection distance.
    float d = IntersectPoint - (float)sqrt(SquaredPoint);

    // If distance is > than the length of the ray return false.
    if (d > length) return false;

    // Else it does hit the sphere and we record the results.
    if (dist) *dist = d;

    return true;
}


bool BoundingSphere::isRayInside(CRay &ray, float length) {
    CVector3 endPos = ray.m_origin + (ray.m_direction * length);
    return (isPointInside(ray.m_origin) && isPointInside(endPos));
}


bool boxToBoxIntersect(BoudingBox &bb1, BoudingBox &bb2) {
    if ((bb1.m_min.x > bb2.m_max.x) || (bb2.m_min.x > bb1.m_max.x))
        return false;
    if ((bb1.m_min.y > bb2.m_max.y) || (bb2.m_min.y > bb1.m_max.y))
        return false;
    if ((bb1.m_min.z > bb2.m_max.z) || (bb2.m_min.z > bb1.m_max.z))
        return false;

    return true;
}


bool sphereToSphereIntersect(BoundingSphere &bs1, BoundingSphere &bs2) {
    // The distance between the two spheres.
    CVector3 intersect = bs1.m_center - bs2.m_center;

    // Test for collision.
    if (sqrt(intersect.x * intersect.x + intersect.y * intersect.y +
        intersect.z * intersect.z) < bs1.m_radius + bs2.m_radius)
        return true;

    return false;
}