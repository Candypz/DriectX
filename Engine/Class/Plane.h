/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#ifndef _UGP_PLANE_H_
#define _UGP_PLANE_H_

#include"MathLibrary.h"


class CPlane
{
   public:
      CPlane();
      CPlane(float A, float B, float C, float D);
      CPlane(CVector3 &n, float D);

      void CreatePlaneFromTri(CVector3 &v1, CVector3 &v2,
                              CVector3 &v3);

      void SetPlaneNormal(float A, float B, float C)
      {
         a = A; b = B; c = C;
      }
      void SetPlaneIntercept(float D) { d = D; }

      bool intersect(CPlane &p2, CPlane &p3,
                     CVector3 *intersectPoint);
      bool intersect(CPlane &pl, CVector3 *intersectPoint);
      bool intersect(CPlane &pl, CRay *intersect);
      bool intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3);

      int ClassifyPolygon(CPolygon &pol);
      int ClassifyPoint(CVector3 &v);
      int ClassifyPoint(float x, float y, float z);

      void Lerp(CPlane &p1, CPlane &p2, float amount);

      float GetDistance(CVector3 &v)
      {
         return a * v.x + b * v.y + c * v.z + d;
      }

      float GetDistance(float x, float y, float z)
      {
         return a * x + b * y + c * z + d;
      }

      float a, b, c, d;
};

#endif