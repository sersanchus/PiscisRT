//---------------------------------------------------------------------------//
//                                                                           //
// PiscisRT (Piscis Ray Tracer)                                              //
//                                                                           //
// Copyright (C) 2003  Sergio Sancho Chust                                   //
//                                                                           //
// This program is free software; you can redistribute it and/or modify      //
// it under the terms of the GNU General Public License as published by      //
// the Free Software Foundation; either version 2 of the License, or         //
// (at your option) any later version.                                       //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program; if not, write to the Free Software               //
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA //
//                                                                           //
// Contact with sergiosancho@terra.es                                        //
//                                                                           //
// Almazora-Castellón-Spain                                                  //
//                                                                           //
//---------------------------------------------------------------------------//

#include "PRTUtil.h"
#include "PRTSphere.h"

PRTSphere::PRTSphere(PRTVector cen,PRTFloat rad,PRTMaterial* mat)
	:PRTObject(cen,PRT_OBJECT_SPHERE,mat)	
{
	center=cen;
	radius=rad;

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=1;
	repv=1;

	squareradius=radius*radius;

	convexhull.chbeg=center-PRTVector(radius,radius,radius);
	convexhull.chend=center+PRTVector(radius,radius,radius);
	convexhull.chrad=rad;
	convexhull.chcen=cen;
	convexhull.SolvePrecision();
}

PRTSphere::PRTSphere(PRTVector cen,PRTFloat rad,PRTFloat ru,PRTFloat rv,PRTMaterial* mat)
	:PRTObject(cen,PRT_OBJECT_SPHERE,mat)	
{
	*this=PRTSphere(cen,rad,mat);
	repu=ru;
	repv=rv;
}

PRTTexCoord PRTSphere::ComputeTexCoord(PRTVector p)
{
	// is assumed that the point is on the surface
	/*PRTVector sn=(p-center).Normalize();
	PRTVector sp(0,1,0);//normalized vector pointing to the north of the sphere
	PRTVector se(-1,0,0);//¿? texture begin
	PRTFloat latid=PRTACos(-sn*sp);
	PRTFloat v=latid/3.1415926535897932384626433832795;
	PRTFloat u;
	if (v==1 || v==0)
		u=0;
	else
	{
		PRTFloat longit=(PRTACos((se*sn)/PRTSin(latid))/6.283185307179586476925286766559);
		if (((sp^se)*sn)>0)
			u=longit;
		else
			u=1-longit;
	}

	u=(u*repu);
	u=u-(int)u;
	v=v*repv;
	v=v-(int)v;

	return PRTTexCoord(u,v);*/

	//ENHANCED

	vaux1=(p-center).Normalize();
	vaux2=PRTVector(0,1,0);//normalized vector pointing to the north of the sphere
	vaux3=PRTVector(-1,0,0);//¿? texture begin
	faux1=PRTACos(-vaux1*vaux2);
	faux2=faux1/PRTPI;
	if (faux2==1 || faux2==0)
		faux3=0;
	else
	{
		PRTFloat longit=(PRTACos((vaux3*vaux1)/PRTSin(faux1))/PRTFloat(6.283185307179586476925286766559));
		if (((vaux2^vaux3)*vaux1)>0)
			faux3=longit;
		else
			faux3=1-longit;
	}

	faux3=(faux3*repu);
	faux3=faux3-(int)faux3;
	faux2=faux2*repv;
	faux2=faux2-(int)faux2;

	return PRTTexCoord(faux3,faux2);	
}

PRTVector PRTSphere::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTSphere::ComputeNormal(PRTVector p)
{
	return PRTVector((p.x-center.x)/radius,(p.y-center.y)/radius,(p.z-center.z)/radius);
}

PRTVector PRTSphere::ComputeTangent(PRTVector p)
{
	if (ComputeNormal(p)%PRTVector(1,0,0)>0.9)
		return ComputeNormal(p)^PRTVector(0,0,1);
	else
		return ComputeNormal(p)^PRTVector(1,0,0);
	//*TODO*
}

PRTVector PRTSphere::ComputeBinormal(PRTVector p)
{
	return ComputeNormal(p)^ComputeTangent(p);
	//*TODO*
}

PRTVector PRTSphere::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

bool PRTSphere::ComputeIntersection(const PRTRay& r,bool testcull, PRTIntersectPoint& result)
{
	result.distance = PRTINFINITE;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return false;

	//OBJECT'S INTERSECTION
	

/*	PRTFloat EPSILON=0.000001;
	PRTFloat B=2*(r.dir.x*(r.orig.x-center.x)+r.dir.y*(r.orig.y-center.y)+r.dir.z*(r.orig.z-center.z));
	PRTFloat C=(r.orig.x-center.x)*(r.orig.x-center.x)
			+(r.orig.y-center.y)*(r.orig.y-center.y)
			+(r.orig.z-center.z)*(r.orig.z-center.z)
			-squareradius;
	PRTFloat discr=B*B-4*C;
	if (discr<EPSILON) //no colision
		return aux; 
	PRTFloat sqrtdiscr=PRTSqrt(discr);
	PRTFloat t0=((-B)-sqrtdiscr)*0.5000000000000000;
	//if (t0>EPSILON)
	//{
	//	aux.colision=true;
	//	aux.distancia=t0;
	//	aux.punto=r.orig+r.dir*t0;
	//	return aux;
	//}
	PRTFloat t1=((-B)+sqrtdiscr)*0.5000000000000000;
	//if (t1>EPSILON)
	//{
	//	aux.colision=true;
	//	aux.distancia=t1;
	//	aux.punto=r.orig+r.dir*t1;
	//	return aux;
	//}
	//testcull
	if (t0<t1 && t0>EPSILON )
	{
		aux.collision=true;
		aux.distance=t0;
		aux.point=r.orig+r.dir*t0;
		return aux;
	}
	else if (t1<t0 && t1>EPSILON)
	{
		aux.collision=true;
		aux.distance=t1;
		aux.point=r.orig+r.dir*t1;
		return aux;
	}
	
	// we are inside the sphere
	if (!testcull)
	{
		if (t1>EPSILON)
		{
			aux.collision=true;
			aux.distance=t1;
			aux.point=r.orig+r.dir*t1;
			return aux;
		}
		else if (t0>EPSILON)
		{
			aux.collision=true;
			aux.distance=t0;
			aux.point=r.orig+r.dir*t0;
			return aux;
		}
	}*/

	//ENHANCED

	faux1=PRTFloat(0.000001);
	faux2=2*(r.dir.x*(r.orig.x-center.x)+r.dir.y*(r.orig.y-center.y)+r.dir.z*(r.orig.z-center.z));
	faux3=(r.orig.x-center.x)*(r.orig.x-center.x)
			+(r.orig.y-center.y)*(r.orig.y-center.y)
			+(r.orig.z-center.z)*(r.orig.z-center.z)
			-squareradius;
	faux4=faux2*faux2-4*faux3;
	if (faux4<faux1) //no colision
		return false;
	faux4=PRTSqrt(faux4);
	faux5=((-faux2)-faux4)*PRTFloat(0.5000000000000000);
	//if (faux5>faux1)
	//{
	//	aux.colision=true;
	//	aux.distancia=faux5;
	//	aux.punto=r.orig+r.dir*faux5;
	//	return aux;
	//}
	faux6=((-faux2)+faux4)*PRTFloat(0.5000000000000000);
	//if (faux6>faux1)
	//{
	//	aux.colision=true;
	//	aux.distancia=faux6;
	//	aux.punto=r.orig+r.dir*faux6;
	//	return aux;
	//}
	//testcull
	if (faux5<faux6 && faux5>faux1 )
	{
		result.collision=true;
		result.distance=faux5;
		result.point=r.orig+r.dir*faux5;
		return true;
	}
	else if (faux6<faux5 && faux6>faux1)
	{
		result.collision=true;
		result.distance=faux6;
		result.point=r.orig+r.dir*faux6;
		return true;
	}
	
	// we are inside the sphere	
	if (!testcull)
	{
		if (faux6>faux1)
		{
			result.collision=true;
			result.distance=faux6;
			result.point=r.orig+r.dir*faux6;
			return true;
		}
		else if (faux5>faux1)
		{
			result.collision=true;
			result.distance=faux5;
			result.point=r.orig+r.dir*faux5;
			return true;
		}
	}
	return result.distance;
}

