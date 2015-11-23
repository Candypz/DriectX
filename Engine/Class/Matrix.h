/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#ifndef _MATRIX_H_
#define _MATRIX_H_


#include"Vector.h"


class CMatrix4x4
{
   public:
      CMatrix4x4() { Identity(); }
      CMatrix4x4(const CMatrix4x4 &m);
      CMatrix4x4(float r11, float r12, float r13, float r14,
                 float r21, float r22, float r23, float r24,
                 float r31, float r32, float r33, float r34,
                 float r41, float r42, float r43, float r44);
      ~CMatrix4x4() {}

      void Identity();

      void operator=(CMatrix4x4 &m);
      CMatrix4x4 operator-(CMatrix4x4 &m);
      CMatrix4x4 operator+(CMatrix4x4 &m);
      CMatrix4x4 operator*(CMatrix4x4 &m);

      CMatrix4x4 operator*(float f);
      CMatrix4x4 operator/(float f);

      void operator +=(CMatrix4x4 &m);
      void operator -=(CMatrix4x4 &m);
      void operator *=(CMatrix4x4 &m);

      void operator *=(float f);
      void operator /=(float f);

      void Translate(CVector3 &Translate);
      void Translate(float x, float y, float z);
      void inverseTranslate();

      void Rotate(double angle, int x, int y, int z);

      CVector3 VectorMatrixMultiply(CVector3 &v);
      CVector3 VectorMatrixMultiply3x3(CVector3 &v);
      
      float matrix[16];
};

#endif