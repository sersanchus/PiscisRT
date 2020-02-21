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
#include "PRTPlane.h"

PRTPlane::PRTPlane(PRTVector cen,PRTVector nor,PRTFloat ru,PRTFloat rv,PRTMaterial* mat)
	:PRTObject(cen,PRT_OBJECT_PLANE,mat)
{
	center=cen;
	normal=nor;

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=ru;
	repv=rv;

	/*PRTFloat infi=PRTINFINITE;
	convexhull.chbeg=centro-PRTVector(infi,infi,infi);
	convexhull.chend=centro+PRTVector(infi,infi,infi);
	convexhull.chrad=infi;
	convexhull.chcen=cen;*/

	// ENHANCED

	convexhull.chbeg=center-PRTVector(PRTINFINITE,PRTINFINITE,PRTINFINITE);
	convexhull.chend=center+PRTVector(PRTINFINITE,PRTINFINITE,PRTINFINITE);
	convexhull.chrad=PRTINFINITE;
	convexhull.chcen=cen;
	convexhull.SolvePrecision();

	normalpercenter=(nor*cen);
}

PRTTexCoord PRTPlane::ComputeTexCoord(PRTVector p)
{
	return PRTTexCoord();//*TODO*
}

PRTVector PRTPlane::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTPlane::ComputeNormal(PRTVector p)
{
	return normal;
}

PRTVector PRTPlane::ComputeTangent(PRTVector p)
{
	return normal;//*TODO*
}

PRTVector PRTPlane::ComputeBinormal(PRTVector p)
{
	return normal;//*TODO*
}

PRTVector PRTPlane::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

PRTIntersectPoint PRTPlane::ComputeIntersection(PRTRay r,bool testcull)
{
	PRTIntersectPoint aux;

	/*PRTFloat vd=normal*r.dir;
	PRTFloat EPSILON=0.000001;
	if (vd<-EPSILON || (vd>EPSILON && !testcull)) // if testcull and vd>EPSILON then testcull *TODO*
	{
		//PRTFloat d=(p->normal*p->centro);
		//PRTFloat vo=d-(p->normal*orig);	
		//PRTFloat aux1;
		PRTFloat vo=normalpercenter-(normal*r.orig);	
		PRTFloat t=vo/vd;
		if (t>EPSILON)
		{
			aux.collision=true;
			aux.distance=t;
			aux.point=r.orig+r.dir*t;
			return aux;
		}
		else
			return aux;
	}
	else
		return aux;*/

	// ENHANCED

	faux1=normal*r.dir;
	faux2=PRTFloat(0.000001);
	if (faux1<-faux2 || (faux1>faux2 && !testcull)) // if testcull and vd>EPSILON then testcull *TODO*
	{
		//PRTFloat d=(p->normal*p->centro);
		//PRTFloat vo=d-(p->normal*orig);	
		//PRTFloat aux1;
		faux3=normalpercenter-(normal*r.orig);	
		faux3=faux3/faux1;
		if (faux3>faux2)
		{
			aux.collision=true;
			aux.distance=faux3;
			aux.point=r.orig+r.dir*faux3;
			return aux;
		}
		else
			return aux;
	}
	else
		return aux;
}

