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
#include "PRTCylinder.h"
#include "PRTQuadric.h"

PRTCylinder::PRTCylinder(PRTVector cen,PRTFloat rad,PRTFloat al,PRTFloat ru,PRTFloat rv,PRTMaterial* mat)
	:PRTObject(cen,PRT_OBJECT_CYLINDER,mat)
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
	convexhull.chcen=cen;
	convexhull.chrad=(convexhull.chbeg-convexhull.chcen).Module();
	convexhull.SolvePrecision();
}

PRTTexCoord PRTCylinder::ComputeTexCoord(PRTVector p)
{
/*	PRTFloat v=p.z/altitude;
	PRTFloat u=(PRTACos(p.x/radius))/6.283185307179586476925286766559;
	if (p.y<0)
		u=1-u;

	return PRTTexCoord(u,v);*/

	// ENHANCED

	faux1=p.z/altitude;
	faux2=(PRTACos(p.x/radius))*PRTFloat(0.15915494309189533576888376337251);///6.283185307179586476925286766559;
	if (p.y<0)
		faux2=1-faux2;

	return PRTTexCoord(faux2,faux1);	
}

PRTVector PRTCylinder::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCylinder::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCylinder::ComputeNormal(PRTVector p)
{
	return (p-PRTVector(center.x,center.y,p.z)).Normalize();
}

PRTVector PRTCylinder::ComputeTangent(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCylinder::ComputeBinormal(PRTVector p)
{
	return PRTVector();//*TODO*
}

bool PRTCylinder::ComputeIntersection(const PRTRay& r,bool testcull, PRTIntersectPoint& result)
{
//	PRTIntersectPoint aux;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return false;

	// AHORA INTERSECCION PROPIA DEL OBJETO -------------------------------------------------------------------

	/*PRTQuadric auxq(center,radius,radius,0,PRT_QUADRIC_CYLINDER,NULL);
	aux=r.Intersect(&auxq,testcull);
	if (aux.collision)
	{
		PRTFloat EPSILON=0.000001;
		if (aux.point.z<(center.z-(altitude*0.5)-EPSILON) || aux.point.z>(center.z+(altitude*0.5)+EPSILON))
		{
			//PRTRay rayaux(aux.punto,dir);
			//aux=rayaux.Intersect(&auxq,testcull,oct,ch);
			//if (aux.colision)
			//	if (aux.punto.z<(c->center.z-(c->altitude/2)-EPSILON) || aux.punto.z>(c->center.z+(c->altitude/2)+EPSILON))
					aux.collision=false;
		}
	}*/

	// ENHANCED

	PRTQuadric auxq(center,radius,radius,0,PRT_QUADRIC_CYLINDER,NULL);
	result=r.Intersect(&auxq,testcull);
	if (result.collision)
	{
		faux1=PRTFloat(0.000001);
		if (result.point.z<(center.z-(altitude*0.5)-faux1) || result.point.z>(center.z+(altitude*0.5)+faux1))
		{
			//PRTRay rayaux(aux.punto,dir);
			//aux=rayaux.Intersect(&auxq,testcull,oct,ch);
			//if (aux.colision)
			//	if (aux.punto.z<(c->center.z-(c->altitude/2)-EPSILON) || aux.punto.z>(c->center.z+(c->altitude/2)+EPSILON))
//					aux.collision=false;
			return false;
		}
	}

	return result.collision;
}

