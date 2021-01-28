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
#include "PRTPolygon.h"

PRTPolygon::PRTPolygon(PRTTriangle *t)
	:PRTObject(t->p1,PRT_OBJECT_POLYGON,t->material)
{
	despu=0;
	despv=0;
	repu=1;
	repv=1;
	convexhull.chbeg=t->convexhull.chbeg;
	convexhull.chend=t->convexhull.chend;
	convexhull.chcen=t->convexhull.chcen;
	convexhull.chrad=t->convexhull.chrad;
	position=t->position;
	what=0;
	tris.AddAtEnd(t);
}

void PRTPolygon::AddVertex(PRTVector p,PRTVector n,PRTTexCoord t)
{
	tris.AddAtEnd(new PRTTriangle(((PRTTriangle*)tris.first->object)->p1,
								  ((PRTTriangle*)tris.last->object)->p3,
								  p,
								  ((PRTTriangle*)tris.first->object)->n1,
								  ((PRTTriangle*)tris.last->object)->n3,
								  n,
								  ((PRTTriangle*)tris.first->object)->t1,
								  ((PRTTriangle*)tris.last->object)->t3,
								  t,
								  ((PRTTriangle*)tris.first->object)->material));
	
	if (p.x<convexhull.chbeg.x)	convexhull.chbeg.x=p.x;
	if (p.y<convexhull.chbeg.y)	convexhull.chbeg.y=p.y;
	if (p.z<convexhull.chbeg.z)	convexhull.chbeg.z=p.z;
	if (p.x>convexhull.chend.x)	convexhull.chend.x=p.x;
	if (p.y>convexhull.chend.y)	convexhull.chend.y=p.y;
	if (p.z>convexhull.chend.z)	convexhull.chend.z=p.z;
	convexhull.chcen=(convexhull.chbeg+convexhull.chend)*PRTFloat(0.5);
	convexhull.chrad=(convexhull.chbeg-convexhull.chcen).Module();
	position=convexhull.chcen;
	convexhull.SolvePrecision();
}

PRTPolygon::~PRTPolygon(void)
{
	for (unsigned int i=0;i<tris.Lenght();i++)
		delete ((PRTTriangle*)(tris.GetAtPos(i)));
	tris.Free();
}

PRTTexCoord PRTPolygon::ComputeTexCoord(PRTVector p)
{
	return what->ComputeTexCoord(p);
}

PRTVector PRTPolygon::ComputePoint(PRTTexCoord tc)
{
	return what->ComputePoint(tc);
}

PRTVector PRTPolygon::ComputeNormal(PRTVector p)
{
	return what->ComputeNormal(p);
}

PRTVector PRTPolygon::ComputeTangent(PRTVector p)
{
	return what->ComputeTangent(p);
}

PRTVector PRTPolygon::ComputeBinormal(PRTVector p)
{
	return what->ComputeBinormal(p);
}

PRTVector PRTPolygon::ComputeColor(PRTVector p)
{
	return what->ComputeColor(p);
}

bool PRTPolygon::ComputeIntersection(const PRTRay& r,bool testcull, PRTIntersectPoint& result)
{
	float distance = PRTINFINITE;
	
	if (!convexhull.IntersectWithRay(r))
		return false;
	
	for (unsigned int i=0;i<tris.Lenght();i++)
	{
		PRTIntersectPoint aux2;
		bool collision = ((PRTTriangle*)(tris.GetAtPos(i)))->ComputeIntersection(r,testcull, aux2);
		if (collision && aux2.distance<distance)
		{
			distance = aux2.distance;
			result=aux2;
			what=((PRTTriangle*)(tris.GetAtPos(i)));
		}
	}
	
	return distance < PRTINFINITE;
}
