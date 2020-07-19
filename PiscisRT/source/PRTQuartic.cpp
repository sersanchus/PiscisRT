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
#include "PRTQuartic.h"

PRTQuartic::PRTQuartic(PRTVector p,PRTFloat aaux,PRTFloat baux,int t,PRTMaterial* m)
	:PRTObject(p,PRT_OBJECT_QUARTIC,m)	
{
	a=aaux;
	b=baux;
	squarea=a*a;
	squareb=b*b;

	center=p;

	/*PRTFloat u=p.x;
	PRTFloat v=p.y;
	PRTFloat w=p.z;

	switch(t)
	{
	case PRT_QUARTIC_TORUS:
		{
				
		}
		break;
	}*/

	/*PRTFloat mas=aaux+baux;
	convexhull.chbeg=p-PRTVector(mas,mas,mas);
	convexhull.chend=p+PRTVector(mas,mas,mas);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.chcen=p;*/

	//ENHANCED

	faux1=aaux+baux;
	convexhull.chbeg=p-PRTVector(faux1,faux1,faux1);
	convexhull.chend=p+PRTVector(faux1,faux1,faux1);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.chcen=p;
	convexhull.SolvePrecision();

	quartictype=t;
/*	type=PRT_OBJECT_QUARTIC;
	material=m;*/

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=0.0;//*TODO*
	repv=0.0;//*TODO*
}

PRTTexCoord PRTQuartic::ComputeTexCoord(PRTVector p)
{
	/*PRTVector sn=(p-center).Normalize();
	PRTVector sp(0,1,0);//vector normalized pointing to north of the sphere
	PRTVector se(-1,0,0);//¿? begining of texture
	PRTFloat latid=PRTACos(-sn*sp);
	PRTFloat v=latid*0.31830988618379067153776752674503;//latid/3.1415926535897932384626433832795;
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

	u=(u*4);
	u=u-(int)u;
	v=v*4;
	v=v-(int)v;

	return PRTTexCoord(u,v);*/

	//ENHANCED

	vaux1=(p-center).Normalize();
	vaux2=PRTVector(0,1,0);//vector normalized pointing to north of the sphere
	vaux3=PRTVector(-1,0,0);//¿? begining of texture
	faux1=PRTACos(-vaux1*vaux2);
	faux2=faux1*PRTFloat(0.31830988618379067153776752674503);//faux1/3.1415926535897932384626433832795;
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

	faux3=(faux3*4);
	faux3=faux3-(int)faux3;
	faux2=faux2*4;
	faux2=faux2-(int)faux2;

	return PRTTexCoord(faux3,faux2);	
}

PRTVector PRTQuartic::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuartic::ComputeNormal(PRTVector p)
{
	/*PRTVector despl=-center;
	PRTVector auxp(p.x+despl.x,p.y+despl.y,0);
	PRTFloat lenght=auxp.Module();
	despl=auxp*a/lenght;
	return (p-center-despl).Normalize();*/

	//ENHANCED

	vaux2=-center;
	vaux1=PRTVector(p.x+vaux2.x,p.y+vaux2.y,0);
	faux1=vaux1.Module();
	vaux1=vaux1*a/faux1;
	return (p+vaux2-vaux1).Normalize();
}

PRTVector PRTQuartic::ComputeTangent(PRTVector p)
{
	if (ComputeNormal(p)%PRTVector(1,0,0)>0.9)
		return ComputeNormal(p)^PRTVector(0,0,1);
	else
		return ComputeNormal(p)^PRTVector(1,0,0);
	//*TODO*
}

PRTVector PRTQuartic::ComputeBinormal(PRTVector p)
{
	return ComputeNormal(p)^ComputeTangent(p);
	//*TODO*
}

PRTVector PRTQuartic::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

bool PRTQuartic::ComputeIntersection(const PRTRay& r,bool testcull, PRTIntersectPoint& result)
{
	result.distance = PRTINFINITE;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return false;

	// NOT ENHANCED //*TODO*

	PRTVector despl=-center;
	PRTVector origaux=r.orig+despl;
	
	PRTFloat aux0=origaux*origaux;
	PRTFloat aux1=r.dir*r.dir;
	PRTFloat auxr=squarea+squareb;
	PRTFloat auxa2=squarea;
	PRTFloat auxb2=squareb;
	PRTFloat auxx02=origaux.x*origaux.x;
	PRTFloat auxy02=origaux.y*origaux.y;
	PRTFloat auxz02=origaux.z*origaux.z;
	PRTFloat auxx12=r.dir.x*r.dir.x;
	PRTFloat auxy12=r.dir.y*r.dir.y;
	PRTFloat auxz12=r.dir.z*r.dir.z;
	PRTFloat auxod=origaux*r.dir;
	
	PRTFloat a0=(aux0-auxr)*(aux0-auxr)-4*auxa2*(auxb2-auxz02);
	PRTFloat a1=4*(auxod)*(aux0-auxr)+8*auxa2*origaux.z*r.dir.z;
	PRTFloat a2=2*aux1*(aux0-auxr)+4*(auxod*auxod)+4*auxa2*auxz12;
	PRTFloat a3=4*auxod*aux1;
	PRTFloat a4=aux1*aux1;
				
	PRTFloat c[5]={a0,a1,a2,a3,a4};	
	PRTFloat s[4];
	
	int num=SolveQuartic(c,s);

	PRTFloat EPSILON=PRTFloat(0.000001); //*TODO*
	PRTFloat fint=PRTINFINITE;
	for (int penemore=0;penemore<4;penemore++)
	{
		if (num>penemore)
		{
			if(s[penemore]>EPSILON && s[penemore]<fint) 	
				fint=s[penemore];
			else if (testcull && s[penemore]<-EPSILON) //we are inside
				return result.collision;
		}
	}
	/*if (num>1 && s[1]>EPSILON && s[1]<fint)
		fint=s[1];
	if (num>2 && s[2]>EPSILON && s[2]<fint)
		fint=s[2];
	if (num>3 && s[3]>EPSILON && s[3]<fint)
		fint=s[3];
*/
	if (fint!=PRTINFINITE)
	{
		//fint=40;
		result.collision=true;
		result.distance=fint;
		result.point=r.orig+r.dir*fint;
		return true;
	}

	return result.collision;
}

