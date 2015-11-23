/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#include"Vector.h"
#include"MathDefines.h"


CVector3::CVector3()
{
   x = y = z = 0.0;
}


CVector3::CVector3(float X, float Y, float Z)
{
   x = X;
   y = Y;
   z = Z;
}


void CVector3::operator =(CVector3 &v)
{
   x = v.x;
   y = v.y;
   z = v.z;
}


CVector3 CVector3::operator -(CVector3 &v)
{
   return CVector3(x - v.x, y - v.y, z - v.z);
}


CVector3 CVector3::operator +(CVector3 &v)
{
   return CVector3(x + v.x, y + v.y, z + v.z);
}


CVector3 CVector3::operator *(CVector3 &v)
{
   return CVector3(x * v.x, y * v.y, z * v.z);
}


CVector3 CVector3::operator /(CVector3 &v)
{
   return CVector3(x / v.x, y / v.y, z / v.z);
}


CVector3 CVector3::operator +(float f)
{
   return CVector3(x + f, y + f, z + f);
}


CVector3 CVector3::operator -(float f)
{
   return CVector3(x - f, y - f, z - f);
}


CVector3 CVector3::operator *(float f)
{
   return CVector3(x * f, y * f, z * f);
}


CVector3 CVector3::operator /(float f)
{
   f = 1/f;

   return CVector3(x * f, y * f, z * f);
}


void CVector3::operator +=(CVector3 &v)
{
   x += v.x;
   y += v.y;
   z += v.z;
}


void CVector3::operator -=(CVector3 &v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
}


void CVector3::operator *=(CVector3 &v)
{
   x *= v.x;
   y *= v.y;
   z *= v.z;
}


void CVector3::operator /=(CVector3 &v)
{
   x /= v.x;
   y /= v.y;
   z /= v.z;
}


void CVector3::operator +=(float f)
{
   x += f;
   y += f;
   z += f;
}


void CVector3::operator -=(float f)
{
   x -= f;
   y -= f;
   z -= f;
}


void CVector3::operator *=(float f)
{
   x *= f;
   y *= f;
   z *= f;
}


void CVector3::operator /=(float f)
{
   f = 1/f;

   x *= f;
   y *= f;
   z *= f;
}


void CVector3::CrossProduct(CVector3 &v1, CVector3 &v2)
{
   x = ((v1.y * v2.z) - (v1.z * v2.y));
   y = ((v1.z * v2.x) - (v1.x * v2.z));
   z = ((v1.x * v2.y) - (v1.y * v2.x));
}


float CVector3::DotProduct3(CVector3 &v1)
{
   return x * v1.x + y * v1.y + z * v1.z;
}


float CVector3::GetLength()
{
   return (float)sqrt((x * x + y * y + z * z));
}


void CVector3::Normal()
{
   float lenght = GetLength();

   if(lenght == 0.0f) lenght = 1.0f;

   x = x/lenght;
   y = y/lenght;
   z = z/lenght;
}


void CVector3::Normalize(CVector3 &v1, CVector3 &v2, CVector3 &v3)
{
   CVector3 e1, e2;

   e1.x = v2.x - v1.x;
   e1.y = v2.y - v1.y;
   e1.z = v2.z - v1.z;
   e1.Normal();

   e2.x = v3.x - v1.x;
   e2.y = v3.y - v1.y;
   e2.z = v3.z - v1.z;
   e2.Normal();

   CrossProduct(e1, e2);
   Normal();
}