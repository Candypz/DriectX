/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#ifndef _UGP_RAY_H_
#define _UGP_RAY_H_

#include"MathLibrary.h"


class CRay
{
   public:
      CRay(){}

      bool intersect(CVector3 &pos, float radius, float *dist);
      bool intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3,
                     bool cull, float *dist);
      bool intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3,
                     bool cull, float length, float *dist);
      bool intersect(CPlane &pl, bool cull,
                     CVector3 *intersectPoint, float *dist);
      bool intersect(CPlane &pl, bool cull, float length,
                     CVector3 *intersectPoint, float *dist);

      CVector3 m_origin;
      CVector3 m_direction;
};

#endif