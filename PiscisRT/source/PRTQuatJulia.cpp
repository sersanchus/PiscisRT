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
#include "PRTQuatJulia.h"
#include "PRTPlane.h"

PRTQuatJulia::PRTQuatJulia(PRTVector cen/*,PRTFloat rad*/,PRTQuaternion caux,int itera, PRTMaterial *mat)
	:PRTObject(cen,PRT_OBJECT_QUATJULIA,mat)
{
	center=cen;
	//radius=rad;
	c=caux;
	iterations=itera;
	normal=PRTVector(0,0,0);

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=1;//*TODO*
	repv=1;//*TODO^*

	Sx=new PRTFloat[iterations+1];
	Sy=new PRTFloat[iterations+1];
	Sz=new PRTFloat[iterations+1];
	Sw=new PRTFloat[iterations+1];

	R = PRTFloat(1.0 )+ PRTSqrt(SQR(c.x) + SQR(c.y) + SQR(c.z) + SQR(c.w));
    R += PRTFloat(0.000001);

	if (R > 2.0)
    {
       R = 2.0;
    }

    R = SQR(R);

	convexhull.chbeg=center-PRTVector(R,R,R);
	convexhull.chend=center+PRTVector(R,R,R);
	convexhull.chrad=R;
	convexhull.chcen=cen;
	convexhull.SolvePrecision();
}

PRTQuatJulia::~PRTQuatJulia(void)
{
	delete Sx;
	delete Sy;
	delete Sz;
	delete Sw;
}

PRTQuaternion PRTQuatJulia::P(PRTQuaternion x)
{
	return x.Sqr()+c;
}

PRTFloat PRTQuatJulia::PointInFractal(PRTVector4 p)
{
	PRTQuaternion aux(p.x,p.y,p.z,p.w);
	PRTFloat length;
	PRTFloat temp;
	int m=0;
	do {
		temp=aux.x+aux.x;
		aux.x=aux.x*aux.x-aux.y*aux.y-aux.z*aux.z-aux.w*aux.w+c.x;
		aux.y=temp*aux.y+c.y;
		aux.z=temp*aux.z+c.z;
		aux.w=temp*aux.w+c.w;

		m++;
		length=aux.x*aux.x+aux.y*aux.y+aux.z*aux.z+aux.w*aux.w;
		} while (!((m>iterations) && (length>4)));
	return length;
}

bool PRTQuatJulia::d(PRTVector z,PRTFloat *Dist) // lower bound of the distance from a point to the fractal
{
	PRTFloat xaux,yaux,zaux,waux,xx,yy,zz,x2,d;
	PRTFloat Norm;
	PRTFloat Pow=0;
	xaux=Sx[0]=z.x;
	yaux=Sy[0]=z.y;
	zaux=Sz[0]=z.z;
	waux=Sw[0]=(center.Module()-normal.x*xaux-normal.y*yaux-normal.z*zaux)/1;
	for (int i = 1; i <= iterations; i++)
	{
		count=i;
		d=yaux*yaux+zaux*zaux+waux*waux;
		x2=xaux*xaux;
		if((Norm=/*PointInFractal(z)*/d+x2)>R)// if a point scape a radius of 4 is outside de fractal
		{
			xaux=Sx[0];
			yaux=Sy[0];
			zaux=Sz[0];
			waux=Sw[0];
			
			Pow = 1.0 / 2.0;

			for (int j = 1; j < i; ++j)
			{
				xx		= xaux * Sx[j] - yaux * Sy[j] - zaux * Sz[j] - waux * Sw[j];
				yy		= xaux * Sy[j] + yaux * Sx[j] + waux * Sz[j] - zaux * Sw[j];
				zz		= xaux * Sz[j] + zaux * Sx[j] + yaux * Sw[j] - waux * Sy[j];
				waux	= xaux * Sw[j] + waux * Sx[j] + zaux * Sy[j] - yaux * Sz[j];
	
				xaux = xx;
				yaux = yy;
				zaux = zz;
	
				Pow /= 2.0;
			}
			
			*Dist = Pow * PRTSqrt(Norm / (xaux * xaux + yaux * yaux + zaux * zaux + waux * waux)) * PRTLog(Norm);

			return false;
    	}

		xaux *= 2.0;

		Sy[i] = yaux = xaux * yaux + c.y;
		Sz[i] = zaux = xaux * zaux + c.z;
		Sw[i] = waux = xaux * waux + c.w;
		Sx[i] = xaux = x2 - d + c.x;
	}
	*Dist = PRTFloat(0.0000001);
	
	return true;
}

PRTTexCoord PRTQuatJulia::ComputeTexCoord(PRTVector p)
{
	return PRTTexCoord();//*TODO*
}

PRTVector PRTQuatJulia::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuatJulia::ComputeColor(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuatJulia::ComputeNormal(PRTVector p)
{
	PRTFloat n11 = 1.0, n12 = 0.0, n13 = 0.0, n14 = 0.0,
	n21 = 0.0, n22 = 1.0, n23 = 0.0, n24 = 0.0,
	n31 = 0.0, n32 = 0.0, n33 = 1.0, n34 = 0.0;
	PRTFloat  tmp;
	PRTFloat  xaux, yaux, zaux, waux;
	
#define Deriv_z2(n1,n2,n3,n4)							 \
	{													 \
  tmp = (n1)*xaux - (n2)*yaux - (n3)*zaux - (n4)*waux;   \
  (n2) = (n1)*yaux + xaux*(n2);							 \
  (n3) = (n1)*zaux + xaux*(n3);							 \
  (n4) = (n1)*waux + xaux*(n4);							 \
  (n1) = tmp;											 \
}

	int i;

	xaux = Sx[0];
	yaux = Sy[0];
	zaux = Sz[0];
	waux = Sw[0];

	for (i = 1; i <= iterations; i++)
	{
		Deriv_z2(n11, n12, n13, n14);
		Deriv_z2(n21, n22, n23, n24);
		Deriv_z2(n31, n32, n33, n34);

	    xaux = Sx[i];
	    yaux = Sy[i];
	    zaux = Sz[i];
	    waux = Sw[i];
	}

	PRTVector Result;
	Result.x = n11 * xaux + n12 * yaux + n13 * zaux + n14 * waux;
	Result.y = n21 * xaux + n22 * yaux + n23 * zaux + n24 * waux;
	Result.z = n31 * xaux + n32 * yaux + n33 * zaux + n34 * waux;

	//return PRTVector(0,1,0);
	
	return Result.Normalize();//*TODO*


/*PRTVector Result;
PRTFloat e=PRTFloat(0.0001);
PRTFloat *dist1=new PRTFloat;
PRTFloat *dist2=new PRTFloat;
d(PRTVector(p.x+e,p.y,p.z),dist1);
d(PRTVector(p.x-e,p.y,p.z),dist2);
Result.x=*dist1-*dist2;
d(PRTVector(p.x,p.y+e,p.z),dist1);
d(PRTVector(p.x,p.y-e,p.z),dist2);
Result.y=*dist1-*dist2;
d(PRTVector(p.x,p.y,p.z+e),dist1);
d(PRTVector(p.x,p.y,p.z-e),dist2);
Result.z=*dist1-*dist2;
delete dist1;
delete dist2;
return Result.Normalize();*/
}

PRTVector PRTQuatJulia::ComputeTangent(PRTVector p)
{
	if (ComputeNormal(p)%PRTVector(1,0,0)>0.9)
		return ComputeNormal(p)^PRTVector(0,0,1);
	else
		return ComputeNormal(p)^PRTVector(1,0,0);
	//*TODO*
}

PRTVector PRTQuatJulia::ComputeBinormal(PRTVector p)
{
	return ComputeNormal(p)^ComputeTangent(p);
	//*TODO*
}

PRTIntersectPoint PRTQuatJulia::ComputeIntersection(PRTRay r,bool testcull)
{
	PRTIntersectPoint aux;		

	// RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return aux;

	// NOW INTERSECT WITH THE OBJECT -------------------------------------------------------------------

	PRTVector pos=r.orig;
	PRTFloat *dist=new PRTFloat;
	PRTFloat dist2=PRTFloat(9999999999999.9);
	*dist=0;
	int naux=0;
	int depth=100;
	bool col=false;
	do
	{
		pos=pos+(r.dir*(*dist));
		col=d(pos,dist);
		//if ((dist2)<(*dist))
		//	return aux;
		naux++;
		dist2=(*dist);
	}while(col==false && naux<depth);

	if (col)
	{	
		aux.collision=true;
		aux.distance=(pos-r.orig).Module();
		aux.point=pos;
	}
	
	delete dist;
	return aux;
}

