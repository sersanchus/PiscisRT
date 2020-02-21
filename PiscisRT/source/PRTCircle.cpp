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
#include "PRTCircle.h"
#include "PRTPlane.h"

PRTCircle::PRTCircle(PRTVector cen,PRTFloat rad,PRTVector nor,PRTFloat ru,PRTFloat rv,PRTMaterial *mat)
	:PRTObject(cen,PRT_OBJECT_CIRCLE,mat)
{
	center=cen;
	radius=rad;
	normal=nor;

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=ru;
	repv=rv;

	squareradius=radius*radius;

	convexhull.chbeg=center-PRTVector(radius,radius,radius);
	convexhull.chend=center+PRTVector(radius,radius,radius);
	convexhull.chrad=rad;
	convexhull.chcen=cen;
	convexhull.SolvePrecision();
}

PRTTexCoord PRTCircle::ComputeTexCoord(PRTVector p)
{
	/*PRTVector p2=p-center;
	PRTFloat aux=(p2.x*p2.x+p2.y*p2.y);
	PRTFloat v=PRTFSqrt(aux/squareradius);
	PRTFloat u=PRTACos(p2.x/PRTFSqrt(aux))/6.283185307179586476925286766559; //*TODO*
	return PRTTexCoord(u,v);*/

	// ENHANCED

	vaux1=p-center;
	faux1=(vaux1.x*vaux1.x+vaux1.y*vaux1.y);
	faux1=PRTFSqrt(faux1/squareradius);
	faux2=PRTACos(vaux1.x/PRTFSqrt(faux1))*PRTFloat(0.15915494309189533576888376337251);///6.283185307179586476925286766559; //*TODO*
	return PRTTexCoord(faux2,faux1);
}

PRTVector PRTCircle::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCircle::ComputeColor(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCircle::ComputeNormal(PRTVector p)
{
	return normal;
}

PRTVector PRTCircle::ComputeTangent(PRTVector p)
{
	return normal;//*TODO*
}

PRTVector PRTCircle::ComputeBinormal(PRTVector p)
{
	return normal;//*TODO*
}

PRTIntersectPoint PRTCircle::ComputeIntersection(PRTRay r,bool testcull)
{
	PRTIntersectPoint aux;		

	// RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return aux;

	// NOW INTERSECT WITH THE OBJECT -------------------------------------------------------------------

	PRTPlane auxp(center,normal,repu,repv,NULL);
	aux=r.Intersect(&auxp,testcull);
	if (aux.collision==true && ((aux.point-center).Module()>radius))
	{
		aux.collision=false;
	}
	return aux;
}
