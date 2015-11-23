/*
   Demo Name:  Game Project 7
      Author:  Allen Sherrod
     Chapter:  Chapter 8
*/


#include"Ray.h"


bool CRay::intersect(CVector3 &pos, float radius, float *dist)
{
   CVector3 RSDir;

   float RayToSphereLength = 0.0f;
   float intersectPoint = 0.0f;
   float SquaredPoint = 0.0f;

   // Get the ray/sphere direction.
   RSDir = pos - m_origin;

   RayToSphereLength = RSDir.DotProduct3(RSDir);
   intersectPoint = RSDir.DotProduct3(m_direction);

   // If true no collision.
   if(intersectPoint < 0 ) return false;

   SquaredPoint = (radius * radius) - RayToSphereLength +
                  (intersectPoint * intersectPoint);

   // If this is less than zero then the ray does not hit.
   if(SquaredPoint < 0) return false;

   // Else it does hit the sphere and we record the results.
   if(dist) *dist = intersectPoint - (float)sqrt(SquaredPoint);

   return true;
}


bool CRay::intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3,
                     bool cull, float *dist)
{
   // Store these vectors between the points for future use.
   CVector3 vecAB = p2 - p1;
   CVector3 vecAC = p3 - p1;

   // Get the corss product bewteen the p3 - p1 vec and the ray dir.
   CVector3 cross;
   cross.CrossProduct(m_direction, vecAC);

   // Calculate the determinate.
   float det = vecAB.DotProduct3(cross);
   
   // If either true, no collision.
   if(cull && det < 0.0001f) return false;
   else if(det < 0.0001f && det > -0.0001f) return false;

   CVector3 rayPointVec = m_origin - p1;
   float test1 = rayPointVec.DotProduct3(cross);
   
   // Ray behind, no collision.
   if(test1 < 0.0f || test1 > det) return false;
   
   CVector3 cross2;
   cross2.CrossProduct(rayPointVec , vecAB);
   float test2 = m_direction.DotProduct3(cross2);

   // No collision.
   if(test2 < 0.0f || test1 + test2 > det) return false;
   
   // Get collision dist.
   float inverseDet = 1.0f / det;
   
   if(dist)
      {
         *dist = vecAC.DotProduct3(cross2);
         *dist *= inverseDet;
      }

   return true;
}


bool CRay::intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3,
                     bool cull, float length, float *dist)
{
   // Store these vectors between the points for future use.
   CVector3 vecAB = p2 - p1;
   CVector3 vecAC = p3 - p1;

   // Get the corss product bewteen the p3 - p1 vec and the ray dir.
   CVector3 cross;
   cross.CrossProduct(m_direction, vecAC);

   // Calculate the determinate.
   float det = vecAB.DotProduct3(cross);
   
   // If either true, no collision.
   if(cull && det < 0.0001f) return false;
   else if(det < 0.0001f && det > -0.0001f) return false;
      
   CVector3 rayPointVec = m_origin - p1;
   float test1 = rayPointVec.DotProduct3(cross);
   
   // Ray behind, no collision.
   if(test1 < 0.0f || test1 > det) return false;
   
   CVector3 cross2;
   cross2.CrossProduct(rayPointVec , vecAB);
   float test2 = m_direction.DotProduct3(cross2);

   // No collision.
   if(test2 < 0.0f || test1 + test2 > det) return false;
   
   // Get distance. If smaller than ray length then collision.
   float inverseDet = 1.0f / det;
   
   if(dist)
      {
         *dist = vecAC.DotProduct3(cross2);
         *dist *= inverseDet;

         if(*dist > length) return false;
      }
   else
      {
         float l = vecAC.DotProduct3(cross2) * inverseDet;
         if(l > length) return false;
      }

   return true;
}


bool CRay::intersect(CPlane &pl, bool cull,
                     CVector3 *intersectPoint, float *dist){
   CVector3 normal(pl.a, pl.b, pl.c);

   float rayD = normal.DotProduct3(m_direction);
   
   // If true then ray is parallel to plane, no intersection.
   if(fabs(rayD) < 0.00001f) return false;
   if(cull && rayD > 0.0f) return false;

   float originD = -(normal.DotProduct3(m_origin) + pl.d);
   float intersectDist = originD / rayD;
   
   // If less then 0, no intersection.  Behind ray origin.
   if(intersectDist < 0.001f) return false;
   
   // Else we have a hit.
   if(dist) *dist = intersectDist;
   if(intersectPoint)
      *intersectPoint = m_origin + (m_direction * intersectDist);
   
   return true;
}


bool CRay::intersect(CPlane &pl, bool cull, float length,
                     CVector3 *intersectPoint, float *dist)
{
   CVector3 normal(pl.a, pl.b, pl.c);

   float rayD = normal.DotProduct3(m_direction);

   // If true then ray is parallel to plane, no intersection.
   if(fabs(rayD) < 0.00001f) return false;
   if(cull && rayD > 0.0f) return false;

   float originD = -(normal.DotProduct3(m_origin) + pl.d);
   
   float intersectDist = originD / rayD;
   
   // If less then 0, no intersection.  Behind ray origin.
   if(intersectDist < 0.001f || intersectDist > length) return false;
   
   // Else we have a hit.
   if(dist) *dist = intersectDist;
   if(intersectPoint)
      *intersectPoint = m_origin + (m_direction * intersectDist);
   
   return true;
}