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
#include "PRTQuadric.h"

PRTQuadric::PRTQuadric(PRTVector p,PRTFloat a,PRTFloat b,PRTFloat c,int t,PRTMaterial* m)
	:PRTObject(p,PRT_OBJECT_QUADRIC,m)	
{
	/*PRTFloat u=p.x;
	PRTFloat v=p.y;
	PRTFloat w=p.z;

	switch(t)
	{
	case PRT_QUADRIC_CYLINDER:
		{
			A=b*b;B=0;C=0;D=-(b*b*u);E=a*a;F=0;G=-(a*a*v);H=0;I=0;J=a*a*v*v+b*b*u*u-a*a*b*b;
		}
		break;
	case PRT_QUADRIC_CONE:
		{
			A=b*b*c*c;B=0;C=0;D=-(b*b*c*c*u);E=a*a*c*c;F=0;G=-(a*a*c*c*v);H=-(a*a*b*b);I=a*a*b*b*w;J=a*a*(c*c*v*v-b*b*w*w)+b*b*c*c*u*u;
		}
		break;
	case PRT_QUADRIC_ELLIPSOID:
		{
			A=b*b*c*c;B=0;C=0;D=-(b*b*c*c*u);E=a*a*c*c;F=0;G=-(a*a*c*c*v);H=a*a*b*b;I=-(a*a*b*b*w);J=a*a*(b*b*w*w+c*c*v*v)+b*b*c*c*u*u-a*a*b*b*c*c;
		}
		break;
	case PRT_QUADRIC_PARABOLOID:
		{
			A=b*b;B=0;C=0;D=-(b*b*u);E=a*a;F=0;G=-(a*a*v);H=0;I=-(a*a*b*b*c);J=a*a*(v*v-2*b*b*w)+b*b*u*u;
		}
		break;
	case PRT_QUADRIC_HYPERBOLOID1:
		{
			A=b*b*c*c;B=0;C=0;D=-(b*b*c*c*u);E=a*a*c*c;F=0;G=-(a*a*c*c*v);H=-(a*a*b*b);I=a*a*b*b*w;J=a*a*(c*c*v*v-b*b*w*w)+b*b*c*c*u*u-a*a*b*b*c*c;
		}
		break;
	case PRT_QUADRIC_HYPERBOLOID2:
		{
			A=b*b*c*c;B=0;C=0;D=-(b*b*c*c*u);E=a*a*c*c;F=0;G=-(a*a*c*c*v);H=-(a*a*b*b);I=a*a*b*b*w;J=a*a*(c*c*v*v-b*b*w*w)+b*b*c*c*u*u+a*a*b*b*c*c;
		}
		break;
	}*/

	// ENHANCED

	faux1=a*a;
	faux2=b*b;
	faux3=c*c;

	switch(t)
	{
	case PRT_QUADRIC_CYLINDER:
		{
			A=faux2;B=0;C=0;D=-(faux2*p.x);E=faux1;F=0;G=-(faux1*p.y);H=0;I=0;J=-G*p.y-D*p.x-faux1*faux2;
		}
		break;
	case PRT_QUADRIC_CONE:
		{
			A=faux2*faux3;B=0;C=0;D=-(A*p.x);E=faux1*faux3;F=0;G=-(E*p.y);H=-(faux1*faux2);I=-H*p.z;J=faux1*(faux3*p.y*p.y-faux2*p.z*p.z)+A*p.x*p.x;
		}
		break;
	case PRT_QUADRIC_ELLIPSOID:
		{
			A=faux2*faux3;B=0;C=0;D=-(A*p.x);E=faux1*faux3;F=0;G=-(E*p.y);H=faux1*faux2;I=-(H*p.z);J=faux1*(faux2*p.z*p.z+faux3*p.y*p.y)+A*p.x*p.x-faux1*A;			
		}
		break;
	case PRT_QUADRIC_PARABOLOID:
		{
			A=faux2;B=0;C=0;D=-(faux2*p.x);E=faux1;F=0;G=-(faux1*p.y);H=0;I=-(faux1*faux2*c);J=faux1*(p.y*p.y-2*faux2*p.z)+faux2*p.x*p.x;
		}
		break;
	case PRT_QUADRIC_HYPERBOLOID1:
		{
			A=faux2*faux3;B=0;C=0;D=-(A*p.x);E=faux1*faux3;F=0;G=-(E*p.y);H=-(faux1*faux2);I=-H*p.z;J=faux1*(faux3*p.y*p.y-faux2*p.z*p.z)+A*p.x*p.x-faux1*A;
		}
		break;
	case PRT_QUADRIC_HYPERBOLOID2:
		{
			A=faux2*faux3;B=0;C=0;D=-(A*p.x);E=faux1*faux3;F=0;G=-(E*p.y);H=-(faux1*faux2);I=-H*p.z;J=faux1*(faux3*p.y*p.y-faux2*p.z*p.z)+A*p.x*p.x+faux1*A;
		}
		break;
	}

	/*PRTFloat mas=a;
	if (b>a)
		mas=b;
	if (c>mas)
		mas=c;
	convexhull.chbeg=p-PRTVector(mas,mas,mas);
	convexhull.chend=p+PRTVector(mas,mas,mas);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.chcen=p;*/

	//ENHANCED

	faux1=a;
	if (b>a)
		faux1=b;
	if (c>faux1)
		faux1=c;
	convexhull.chbeg=p-PRTVector(faux1,faux1,faux1);
	convexhull.chend=p+PRTVector(faux1,faux1,faux1);
	convexhull.chrad=(convexhull.chbeg-convexhull.chcen).Module();
	convexhull.chcen=p;
	convexhull.SolvePrecision();

	quadrictype=t;
	/*type=PRT_OBJECT_QUADRIC;
	material=m;*/

	despu=0.0;//*TODO*
	despv=0.0;//*TODO*
	repu=0.0;//*TODO*
	repv=0.0;//*TODO*
}

PRTTexCoord PRTQuadric::ComputeTexCoord(PRTVector p)
{
	return PRTTexCoord();//*TODO*
}

PRTVector PRTQuadric::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuadric::ComputeNormal(PRTVector p)
{
	/*PRTFloat xn=2*(A*p.x+B*p.y+C*p.z+D);
	PRTFloat yn=2*(B*p.x+E*p.y+F*p.z+G);
	PRTFloat zn=2*(C*p.x+F*p.y+H*p.z+I);*/

	faux1=2*(A*p.x+B*p.y+C*p.z+D);
	faux2=2*(B*p.x+E*p.y+F*p.z+G);
	faux3=2*(C*p.x+F*p.y+H*p.z+I);

	return PRTVector(faux1,faux2,faux3).Normalize();
}

PRTVector PRTQuadric::ComputeTangent(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuadric::ComputeBinormal(PRTVector p)
{
	return PRTVector();//*TODO*
}

PRTVector PRTQuadric::ComputeColor(PRTVector)
{
	return PRTVector();//*TODO*
}

PRTIntersectPoint PRTQuadric::ComputeIntersection(PRTRay r,bool testcull)
{
	PRTIntersectPoint aux;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	//if (!convexhull.IntersectWithRay(r)) // infinite quadrics! no convex hull
	//	return aux;
	
	/*PRTFloat AQ=A*r.dir.x*r.dir.x
		+2*B*r.dir.x*r.dir.y
		+2*C*r.dir.x*r.dir.z
		+E*r.dir.y*r.dir.y
		+2*F*r.dir.y*r.dir.z
		+H*r.dir.z*r.dir.z;
	
	PRTFloat BQ=2*(A*r.orig.x*r.dir.x
		+B*(r.orig.x*r.dir.y+r.dir.x*r.orig.y)
		+C*(r.orig.x*r.dir.z+r.dir.x*r.orig.z)
		+D*r.dir.x
		+E*r.orig.y*r.dir.y
		+F*(r.orig.y*r.dir.z+r.dir.y*r.orig.z)
		+G*r.dir.y
		+H*r.orig.z*r.dir.z
		+I*r.dir.z);

	PRTFloat CQ=A*r.orig.x*r.orig.x
		+2*B*r.orig.x*r.orig.y
		+2*C*r.orig.x*r.orig.z
		+2*D*r.orig.x
		+E*r.orig.y*r.orig.y
		+2*F*r.orig.y*r.orig.z
		+2*G*r.orig.y
		+H*r.orig.z*r.orig.z
		+2*I*r.orig.z
		+J;

	PRTFloat t;
	//PRTFloat EPSILON=0.000001;
	if (AQ!=0)
	{
		PRTFloat discr=(BQ*BQ)-(4*AQ*CQ);
		if (discr<0)
			return aux;
		else
		{
			PRTFloat sqrtdiscr=PRTSqrt(discr);
			PRTFloat AQpor2=AQ*2;
			PRTFloat t0=(-BQ-sqrtdiscr)/(AQpor2);
			PRTFloat t1=(-BQ+sqrtdiscr)/(AQpor2);
			if (t0<0 && t1<0) //no colision
				return aux;
			else
			{
				if (t0>0 && t1>0) //we are outside the quadric
				{
					if (t0<t1)
						t=t0;
					else
						t=t1;
				}
				else 
				{
					if (t0>0) //we are inside the quadric
						t=t0;
					else //if (!testcull) // ¿?
						t=t1;
				}
			}
			//if (t0<EPSILON)
			//{
			//	if (t1>EPSILON)
			//		t=t1;
			//	else
			//		return aux;
			//}
			//else
			//{
			//	if (t1<EPSILON)
			//		t=t0;
			//	else if(t0<t1)
			//		t=t0;
			//	else
			//		t=t1;
			//
			//}

		}
	}
	else
		t=-CQ/BQ;
	

	if (t>0)
	{
		aux.collision=true;
		aux.distance=t;
		aux.point=r.orig+r.dir*t;
		return aux;
	}*/

	//ENHANCED

	faux1=A*r.dir.x*r.dir.x
		+2*B*r.dir.x*r.dir.y
		+2*C*r.dir.x*r.dir.z
		+E*r.dir.y*r.dir.y
		+2*F*r.dir.y*r.dir.z
		+H*r.dir.z*r.dir.z;
	
	faux2=2*(A*r.orig.x*r.dir.x
		+B*(r.orig.x*r.dir.y+r.dir.x*r.orig.y)
		+C*(r.orig.x*r.dir.z+r.dir.x*r.orig.z)
		+D*r.dir.x
		+E*r.orig.y*r.dir.y
		+F*(r.orig.y*r.dir.z+r.dir.y*r.orig.z)
		+G*r.dir.y
		+H*r.orig.z*r.dir.z
		+I*r.dir.z);

	faux3=A*r.orig.x*r.orig.x
		+2*B*r.orig.x*r.orig.y
		+2*C*r.orig.x*r.orig.z
		+2*D*r.orig.x
		+E*r.orig.y*r.orig.y
		+2*F*r.orig.y*r.orig.z
		+2*G*r.orig.y
		+H*r.orig.z*r.orig.z
		+2*I*r.orig.z
		+J;

	PRTFloat t; // t,t0 and t1 not using faux* //*TODO*
	//faux4=PRTFloat(0.000001);
	if (faux1!=0/*>faux4 || faux1<-faux4*/)
	{
		faux5=(faux2*faux2)-(4*faux1*faux3);
		if (faux5<0/*faux4*/)
			return aux;
		else
		{
			faux5=PRTSqrt(faux5);
			faux6=faux1*2;
			PRTFloat t0=(-faux2-faux5)/(faux6);
			PRTFloat t1=(-faux2+faux5)/(faux6);
			if (t0<0 && t1<0) //no collision
				return aux;
			else
			{
				if (t0>0 && t1>0) //we are outside the quadric
				{
					if (t0<t1)
						t=t0;
					else
						t=t1;
				}
				else 
				{
					if (t0>0) //we are inside the quadric
						t=t0;
					else //if (!testcull) // ¿?
						t=t1;
				}
			}
			/*if (t0<faux4)
			{
				if (t1>faux4)
					t=t1;
				else
					return aux;
			}
			else
			{
				if (t1<faux4)
					t=t0;
				else if(t0<t1)
					t=t0;
				else
					t=t1;
			}*/

		}
	}
	else
		t=-faux3/faux2;
	

	if (t>0/*faux4*/)
	{
		aux.collision=true;
		aux.distance=t;
		aux.point=r.orig+r.dir*t;
		return aux;
	}

	return aux;
}

