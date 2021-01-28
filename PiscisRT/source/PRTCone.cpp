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
#include "PRTCone.h"
#include "PRTQuadric.h"

PRTCone::PRTCone(PRTVector cen,PRTFloat rad,PRTFloat al,PRTFloat ru,PRTFloat rv,PRTMaterial* mat)
	:PRTObject(cen,PRT_OBJECT_CONE,mat)
{
	center=cen;
	radius=rad;
	altitude=al;

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=ru;
	repv=rv;

	squareradius=radius*radius;

	// ENHANCED

	faux1=radius;
	if ((al/2)>radius)
		faux1=al/2;

	convexhull.chbeg=center-PRTVector(faux1,faux1,faux1);
	convexhull.chend=center+PRTVector(faux1,faux1,faux1);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.chcen=cen;
	convexhull.SolvePrecision();
}

PRTTexCoord PRTCone::ComputeTexCoord(PRTVector p)
{
	/*PRTFloat v=(p.z-center.z-altitude/2)/altitude;
	PRTFloat u=(PRTACos((p.x-center.x)/(v*radius)))/6.283185307179586476925286766559;
	if ((p.y-center.y)<0)
		u=1-u;

	return PRTTexCoord(u,v);*/

	// ENHANCED

	faux1=(p.z-center.z-altitude/2)/altitude;
	faux2=(PRTACos((p.x-center.x)/(faux1*radius)))*PRTFloat(0.15915494309189533576888376337251);///6.283185307179586476925286766559;
	if ((p.y-center.y)<0)
		faux2=1-faux2;

	return PRTTexCoord(faux2,faux1);
}

PRTVector PRTCone::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCone::ComputeTangent(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCone::ComputeBinormal(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCone::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCone::ComputeNormal(PRTVector p)
{
	/*//return (p-PRTVector(centro.x,centro.y,p.z)).Normalizado();//*TODO*
	PRTVector aux1(0,0,-1);
	PRTVector aux2=(p-(center+PRTVector(0,0,altitude*0.5))).Normalize();
	PRTVector aux3=aux2^aux1;
	return (aux2^aux3).Normalize();*/

	// ENHANCED

	//return (p-PRTVector(centro.x,centro.y,p.z)).Normalizado();//*TODO*
	vaux1=PRTVector(0,0,-1);
	vaux2=(p-(center+PRTVector(0,0,altitude*PRTFloat(0.5)))).Normalize();
	vaux3=vaux2^vaux1;
	return (vaux2^vaux3).Normalize();
}

bool PRTCone::ComputeIntersection(const PRTRay& r,bool testcull, PRTIntersectPoint& result)
{
	// RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return false;

	// NOW INTERSECT WITH THE OBJECT -------------------------------------------------------------------
	
	/*PRTVector newcenter=center+PRTVector(0,0,altitude*0.5);
	PRTQuadric auxq(newcenter,radius,radius,altitude,PRT_QUADRIC_CONE,NULL);
	aux=r.Intersect(&auxq,testcull);
	if (aux.collision)
	{
		PRTFloat EPSILON=0.000001;
		if (aux.point.z<(newcenter.z-(altitude)-EPSILON) || aux.point.z>(newcenter.z+EPSILON))
		{
			PRTRay rayaux(aux.point,r.dir);
			PRTIntersectPoint aux2=rayaux.Intersect(&auxq,testcull);
			if (aux2.collision)
			{
				if (aux2.point.z<(newcenter.z-(altitude)-EPSILON) || aux2.point.z>(newcenter.z)+EPSILON)
				{
					aux.collision=false;
					return aux;
				}
				else
				{
					//ha habido colision en aux2 con el segundo rayo
					aux.distance+=aux2.distance;
					aux.point=aux2.point;
				}
			}
			else
				aux.collision=false;
		}
	}*/

	// ENHANCED

	vaux1=center+PRTVector(0,0,altitude*PRTFloat(0.5));
	PRTQuadric auxq(vaux1,radius,radius,altitude,PRT_QUADRIC_CONE,NULL);
	result=r.Intersect(&auxq,testcull);
	if (result.collision)
	{
		faux1=PRTFloat(0.000001);
		if (result.point.z<(vaux1.z-(altitude)-faux1) || result.point.z>(vaux1.z+faux1))
		{
			PRTRay rayaux(result.point,r.dir);
			PRTIntersectPoint aux2=rayaux.Intersect(&auxq,testcull);
			if (aux2.collision)
			{
				if (aux2.point.z<(vaux1.z-(altitude)-faux1) || aux2.point.z>(vaux1.z)+faux1)
				{
//					aux.collision=false;
					return false;
				}
				else
				{
					//ha habido colision en aux2 con el segundo rayo
					result.distance+=aux2.distance;
					result.point=aux2.point;
				}
			}
			else
				return false;
//				aux.collision=false;
		}
	}

	return result.collision;
}

