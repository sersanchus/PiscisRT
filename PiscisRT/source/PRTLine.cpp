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
#include "PRTLine.h"
#include "PRTRandom.h"

PRTLine::PRTLine(PRTVector auxp1,PRTVector auxp2,PRTVector auxn1,PRTVector auxn2,PRTMaterial* material)
	:PRTObject((auxp1+auxp2)/2,PRT_OBJECT_LINE,material)
{
	p1=auxp1;
	p2=auxp2;
	n1=auxn1;
	n2=auxn2;
	c1=PRTVector();
	c2=PRTVector();
	t1=PRTTexCoord();
	t2=PRTTexCoord();
	
	//CONVEX HULL
	/*PRTFloat inix,finx,iniy,finy,iniz,finz;
	if (p1.x<p2.x) inix=p1.x; else inix=p2.x;
	if (p1.x>p2.x) finx=p1.x; else finx=p2.x;
	if (p1.y<p2.y) iniy=p1.y; else iniy=p2.y;
	if (p1.y>p2.y) finy=p1.y; else finy=p2.y;
	if (p1.z<p2.z) iniz=p1.z; else iniz=p2.z;
	if (p1.z>p2.z) finz=p1.z; else finz=p2.z;		
	convexhull.chbeg=PRTVector(inix,iniy,iniz);
	convexhull.chend=PRTVector(finx,finy,finz);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;*/

	//ENHANCED

	if (p1.x<p2.x) faux1=p1.x; else faux1=p2.x;
	if (p1.x>p2.x) faux2=p1.x; else faux2=p2.x;
	if (p1.y<p2.y) faux3=p1.y; else faux3=p2.y;
	if (p1.y>p2.y) faux4=p1.y; else faux4=p2.y;
	if (p1.z<p2.z) faux5=p1.z; else faux5=p2.z;
	if (p1.z>p2.z) faux6=p1.z; else faux6=p2.z;		
	convexhull.chbeg=PRTVector(faux1,faux3,faux5);
	convexhull.chend=PRTVector(faux2,faux4,faux6);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.SolvePrecision();
}

PRTLine::PRTLine(PRTVector auxp1,PRTVector auxp2,PRTVector auxn1,PRTVector auxn2,PRTVector auxc1,PRTVector auxc2,PRTMaterial* material)
{
	*this=PRTLine(auxp1,auxp2,auxn1,auxn2,material);
	
	c1=auxc1;
	c2=auxc2;
}

PRTLine::PRTLine(PRTVector auxp1,PRTVector auxp2,PRTVector auxn1,PRTVector auxn2,PRTTexCoord auxt1,PRTTexCoord auxt2,PRTMaterial* material)
{
	*this=PRTLine(auxp1,auxp2,auxn1,auxn2,material);
	
	t1=auxt1;
	t2=auxt2;
}

PRTTexCoord PRTLine::ComputeTexCoord(PRTVector p)
{
    return PRTTexCoord();//*TODO*
}

PRTVector PRTLine::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTLine::ComputeNormal(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTLine::ComputeTangent(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTLine::ComputeBinormal(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTLine::ComputeColor(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTIntersectPoint PRTLine::ComputeIntersection(PRTRay r,bool testcull)
{

	PRTIntersectPoint aux;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return aux;

	// OBJECT'S INTERSECTION -------------------------------------------------------------------

	//http://www.loria.fr/~lazard/ARC-Visi3D/Pant-project/files/Line_Segment_Line.html //*TODO*
	//http://mathworld.wolfram.com/Line-LineIntersection.html
	//http://www.delphiforfun.org/Programs/Math_Topics/intersecting_lines.htm

	// sin testcull *TODO*?
	
	// NOT ENHANCED *TODO*
	
	PRTVector trans=PRTVector(0,0,0)-p1;

	PRTMatrix mat;
	mat.LoadIdentity();
	PRTFloat ang1=PRTACos(PRTVector((p2-p1).x,(p2-p1).y,0).Normalize()%PRTVector(1,0,0));
	mat.RotateZ(PRTRadToGrad(ang1));
	PRTFloat ang2=PRTACos(mat*(PRTVector4)(p2-p1).Normalize()%PRTVector(0,0,1));
	mat.RotateY(PRTRadToGrad(ang2));
	
	PRTMatrix mat2;
	mat2.LoadIdentity();
	mat2=mat2*mat; //antes ponia postmultiply, como se hace? o ya esta bien?
	
	PRTVector4 p1t=mat*(p1+trans);
	PRTVector4 p2t=mat*(p2+trans);

	PRTConvexHull ch2;
	PRTFloat infini=5;//*TODO* infini*2=ancho de linea en 3d
	PRTVector4 aux1=p2t+PRTVector4(-infini,-infini,0,0);
	PRTVector4 aux2=p1t+PRTVector4(infini,infini,0,0);
	ch2.chbeg=PRTVector(aux1.x,aux1.y,aux1.z);
	ch2.chend=PRTVector(aux2.x,aux2.y,aux2.z);

	PRTVector4 aux3=mat*(PRTVector4)(r.orig+trans);
	PRTVector4 aux4=mat*(PRTVector4)(r.dir);

	PRTVector ret;
	if (HitBoundingBox(ch2.chbeg,ch2.chend,PRTVector(aux3.x,aux3.y,aux3.z),PRTVector(aux4.x,aux4.y,aux4.z),ret))
	{
		aux.collision=true;
		PRTVector4 aux5=mat2*ret-trans;
		aux.point=PRTVector(aux5.x,aux5.y,aux5.z);
		aux.distance=(aux.point-r.orig).Module();
	}
	
	return aux;
}

