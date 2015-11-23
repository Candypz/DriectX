/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#ifndef __UGP_POLYGON_H_
#define __UGP_POLYGON_H_

#include"Vector.h"
#include"Ray.h"
#include"Plane.h"
#include"MathDefines.h"


class CPolygon
{
   public:
      CPolygon() : m_flag(0) { }

      void SetFlag(unsigned int fl) { m_flag = fl; }
      unsigned int GetFlag() { return m_flag; }
      void Copy(CPolygon &pol);

      void Clip(CPlane &pl, CPolygon *front, CPolygon *back);
      
      bool intersect(CRay &ray, bool cull, float *dist);

      unsigned int m_flag;
      CVector3 m_vertexList[3];
};

#endif