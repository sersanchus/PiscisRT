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
#include "PRTTriangle.h"

PRTTriangle::PRTTriangle(PRTVector auxp1,PRTVector auxp2,PRTVector auxp3,PRTVector auxn1,PRTVector auxn2,PRTVector auxn3,PRTMaterial* material)
	:PRTObject(((auxp1+auxp2)/2)+((auxp2+auxp3)/2),PRT_OBJECT_TRIANGLE,material)
{
	p1=auxp1;
	p2=auxp2;
	p3=auxp3;
	n1=auxn1;
	n2=auxn2;
	n3=auxn3;
	areaper2=((p2-p1)^(p3-p1)).Module();
	facenormal=((p2-p1)^(p3-p1));
	c1=PRTVector();
	c2=PRTVector();
	c3=PRTVector();
	t1=PRTTexCoord();
	t2=PRTTexCoord();
	t3=PRTTexCoord();
	
	//CONVEX HULL
	/*PRTFloat inix,finx,iniy,finy,iniz,finz;
	if (p1.x<p2.x){if (p3.x<p1.x)inix=p3.x;else inix=p1.x;}else{if (p3.x<p2.x)inix=p3.x;else inix=p2.x;}
	if (p1.x>p2.x){if (p3.x>p1.x)finx=p3.x;else finx=p1.x;}else{if (p3.x>p2.x)finx=p3.x;else finx=p2.x;}
	if (p1.y<p2.y){if (p3.y<p1.y)iniy=p3.y;else iniy=p1.y;}else{if (p3.y<p2.y)iniy=p3.y;else iniy=p2.y;}
	if (p1.y>p2.y){if (p3.y>p1.y)finy=p3.y;else finy=p1.y;}else{if (p3.y>p2.y)finy=p3.y;else finy=p2.y;}
	if (p1.z<p2.z){if (p3.z<p1.z)iniz=p3.z;else iniz=p1.z;}else{if (p3.z<p2.z)iniz=p3.z;else iniz=p2.z;}
	if (p1.z>p2.z){if (p3.z>p1.z)finz=p3.z;else finz=p1.z;}else{if (p3.z>p2.z)finz=p3.z;else finz=p2.z;}		
	convexhull.chbeg=PRTVector(inix,iniy,iniz);
	convexhull.chend=PRTVector(finx,finy,finz);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;*/

	//ENHANCED

	if (p1.x<p2.x){if (p3.x<p1.x)faux1=p3.x;else faux1=p1.x;}else{if (p3.x<p2.x)faux1=p3.x;else faux1=p2.x;}
	if (p1.x>p2.x){if (p3.x>p1.x)faux2=p3.x;else faux2=p1.x;}else{if (p3.x>p2.x)faux2=p3.x;else faux2=p2.x;}
	if (p1.y<p2.y){if (p3.y<p1.y)faux3=p3.y;else faux3=p1.y;}else{if (p3.y<p2.y)faux3=p3.y;else faux3=p2.y;}
	if (p1.y>p2.y){if (p3.y>p1.y)faux4=p3.y;else faux4=p1.y;}else{if (p3.y>p2.y)faux4=p3.y;else faux4=p2.y;}
	if (p1.z<p2.z){if (p3.z<p1.z)faux5=p3.z;else faux5=p1.z;}else{if (p3.z<p2.z)faux5=p3.z;else faux5=p2.z;}
	if (p1.z>p2.z){if (p3.z>p1.z)faux6=p3.z;else faux6=p1.z;}else{if (p3.z>p2.z)faux6=p3.z;else faux6=p2.z;}		
	convexhull.chbeg=PRTVector(faux1,faux3,faux5);
	convexhull.chend=PRTVector(faux2,faux4,faux6);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	convexhull.SolvePrecision();

	t2ulesst1u=0;
	t2vlesst1v=0;
	t3ulesst1u=0;
	t3vlesst1v=0;

	n2lessn1=n2-n1;
	n3lessn1=n3-n1;
}

PRTTriangle::PRTTriangle(PRTVector auxp1,PRTVector auxp2,PRTVector auxp3,PRTVector auxn1,PRTVector auxn2,PRTVector auxn3,PRTVector auxc1,PRTVector auxc2,PRTVector auxc3,PRTMaterial* material)
{
	*this=PRTTriangle(auxp1,auxp2,auxp3,auxn1,auxn2,auxn3,material);
	
	c1=auxc1;
	c2=auxc2;
	c3=auxc3;
	
	c2lessc1=c2-c1;
	c3lessc1=c3-c1;
}

PRTTriangle::PRTTriangle(PRTVector auxp1,PRTVector auxp2,PRTVector auxp3,PRTVector auxn1,PRTVector auxn2,PRTVector auxn3,PRTTexCoord auxt1,PRTTexCoord auxt2,PRTTexCoord auxt3,PRTMaterial* material)
{
	*this=PRTTriangle(auxp1,auxp2,auxp3,auxn1,auxn2,auxn3,material);
	
	t1=auxt1;
	t2=auxt2;
	t3=auxt3;
	
	t2ulesst1u=t2.u-t1.u;
	t2vlesst1v=t2.v-t1.v;
	t3ulesst1u=t3.u-t1.u;
	t3vlesst1v=t3.v-t1.v;
}

PRTTexCoord PRTTriangle::ComputeTexCoord(PRTVector p)
{
	/*
    //PRTFloat tu;
	PRTFloat tv,tw;
	PRTVector p1lessp=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    tv=((p3-p)^(p1lessp)).Module()/areaper2;
    tw=((p1lessp)^(p2-p)).Module()/areaper2;
    PRTTexCoord aux1,aux2;
    aux1.u=(t2ulesst1u)*tv;
	aux1.v=(t2vlesst1v)*tv;
	aux2.u=(t3ulesst1u)*tw;
	aux2.v=(t3vlesst1v)*tw;
    return t1+aux1+aux2;*/

	//ENHANCED

	vaux1=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    faux1=((p3-p)^(vaux1)).Module()/areaper2;
    faux2=((vaux1)^(p2-p)).Module()/areaper2;
    PRTTexCoord aux1,aux2;
    aux1.u=(t2ulesst1u)*faux1;
	aux1.v=(t2vlesst1v)*faux1;
	aux2.u=(t3ulesst1u)*faux2;
	aux2.v=(t3vlesst1v)*faux2;
    return t1+aux1+aux2;
}

PRTVector PRTTriangle::ComputePoint(PRTTexCoord tc)
{
	PRTFloat betau=t2ulesst1u;
	PRTFloat betav=t2vlesst1v;
	PRTFloat omegau=t3.u-t2.u;
	PRTFloat omegav=t3.v-t2.v;

	/*
	if (t2ulesst1u!=0)
		betau=(tc.u-t1.u)/(t2ulesst1u);
	else
		betau=0;
	PRTFloat betav;
	if (t2vlesst1v!=0)
		betav=(tc.v-t1.v)/(t2vlesst1v);
	else
		betav=0;
	PRTFloat omegau;
	if (t3ulesst1u!=0)
		omegau=(tc.u-t1.u)/(t3ulesst1u);
	else
		omegau=0;
	PRTFloat omegav;
	if (t3vlesst1v!=0)
		omegav=(tc.v-t1.v)/(t3vlesst1v);
	else
		omegav=0;*/

	PRTVector tou=(p2-p1)*betau+(p3-p2)*omegau;
	PRTVector tov=(p2-p1)*betav+(p3-p2)*omegav;

	PRTFloat tculesst1u=tc.u-t1.u;
	PRTFloat tcvlesst1v=tc.v-t1.v;
		
	//if (beta+omega<=1)
		return p1+tou*tculesst1u+tov*tcvlesst1v;
	//else
	//	return PRTVector();
}

PRTVector PRTTriangle::ComputeNormal(PRTVector p)
{
	/*
    //PRTFloat tu;
	PRTFloat tv,tw;
	PRTVector p1lessp=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    tv=((p3-p)^(p1lessp)).Module()/areaper2;
    tw=((p1lessp)^(p2-p)).Module()/areaper2;
    PRTVector aux1,aux2;
    aux1=(n2lessn1)*tv;
	aux2=(n3lessn1)*tw;
	return n1+aux1+aux2;*/

	//ENHANCED

	vaux1=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    faux1=((p3-p)^(vaux1)).Module()/areaper2;
    faux2=((vaux1)^(p2-p)).Module()/areaper2;
    vaux2=(n2lessn1)*faux1;
	vaux3=(n3lessn1)*faux2;
	return n1+vaux2+vaux3;	
}

PRTVector PRTTriangle::ComputeTangent(PRTVector p)
{
	/*PRTVector tangent;

	PRTVector cp;
	PRTVector e0(p2[0]-p1[0],t2[0]-t1[0],t2[1]-t1[1]);
	PRTVector e1(p3[0]-p1[0],t3[0]-t1[0],t3[1]-t1[1]);

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		tangent.x=-cp[1]/cp[0];
	
	e0.x=p2[1]-p1[1];
	e1.x=p3[1]-p1[1];

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		tangent.y=-cp[1]/cp[0];
	
	e0.x=p2[2]-p1[2];
	e1.x=p3[2]-p1[2];

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		tangent.z=-cp[1]/cp[0];
		
	return tangent;*/

	//ENHANCED

	PRTVector tangent;

	PRTVector vaux1;
	PRTVector vaux2(p2[0]-p1[0],t2[0]-t1[0],t2[1]-t1[1]);
	PRTVector vaux3(p3[0]-p1[0],t3[0]-t1[0],t3[1]-t1[1]);

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		tangent.x=-vaux1[1]/vaux1[0];
	
	vaux2.x=p2[1]-p1[1];
	vaux3.x=p3[1]-p1[1];

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		tangent.y=-vaux1[1]/vaux1[0];
	
	vaux2.x=p2[2]-p1[2];
	vaux3.x=p3[2]-p1[2];

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		tangent.z=-vaux1[1]/vaux1[0];
		
	return tangent;
}

PRTVector PRTTriangle::ComputeBinormal(PRTVector p)
{
	/*PRTVector binormal;

	PRTVector cp;
	PRTVector e0(p2[0]-p1[0],t2[0]-t1[0],t2[1]-t1[1]);
	PRTVector e1(p3[0]-p1[0],t3[0]-t1[0],t3[1]-t1[1]);

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		binormal.x=-cp[2]/cp[0];

	e0.x=p2[1]-p1[1];
	e1.x=p3[1]-p1[1];

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		binormal.y=-cp[2]/cp[0];

	e0.x=p2[2]-p1[2];
	e1.x=p3[2]-p1[2];

	cp.x=e0[1]*e1[2]-e0[2]*e1[1];
	cp.y=e0[2]*e1[0]-e0[0]*e1[2];
	cp.z=e0[0]*e1[1]-e0[1]*e1[0];

	if(fabs(cp[0])>0.00001)
		binormal.z=-cp[2]/cp[0];
	
	return binormal;*/

	//ENHANCED

	PRTVector binormal;

	PRTVector vaux1;
	PRTVector vaux2(p2[0]-p1[0],t2[0]-t1[0],t2[1]-t1[1]);
	PRTVector vaux3(p3[0]-p1[0],t3[0]-t1[0],t3[1]-t1[1]);

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		binormal.x=-vaux1[2]/vaux1[0];

	vaux2.x=p2[1]-p1[1];
	vaux3.x=p3[1]-p1[1];

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		binormal.y=-vaux1[2]/vaux1[0];

	vaux2.x=p2[2]-p1[2];
	vaux3.x=p3[2]-p1[2];

	vaux1.x=vaux2[1]*vaux3[2]-vaux2[2]*vaux3[1];
	vaux1.y=vaux2[2]*vaux3[0]-vaux2[0]*vaux3[2];
	vaux1.z=vaux2[0]*vaux3[1]-vaux2[1]*vaux3[0];

	if(fabs(vaux1[0])>0.00001)
		binormal.z=-vaux1[2]/vaux1[0];
	
	return binormal;
}

PRTVector PRTTriangle::ComputeColor(PRTVector p)
{
/*	// Y ESTO COMO PUEDE IR !!!!¿¿¿¿
	
	//if (areaper2==0) areaper2=((p2-p1)^(p3-p1)).Module();
	// u esta entre p2 y p3
	PRTFloat v=((p2-p)^(p3-p)).Module()/areaper2;
	// v esta entre p3 y p1
	PRTFloat w=((p3-p)^(p1-p)).Module()/areaper2;	
	// w esta entre p1 y p2
	PRTFloat u=((p1-p)^(p2-p)).Module()/areaper2;

	return PRTVector(c1.z+w*(c2.z-c1.z)+v*(c3.z-c1.z),c1.y+w*(c2.y-c1.y)+v*(c3.y-c1.y),c1.x+w*(c2.x-c1.x)+v*(c3.x-c1.x));
*/

	/*
    //PRTFloat tu;
	PRTFloat tv,tw;
	PRTVector p1lessp=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    tv=((p3-p)^(p1lessp)).Module()/areaper2;
    tw=((p1lessp)^(p2-p)).Module()/areaper2;
    PRTVector aux1,aux2;
    aux1=(c2lessc1)*tv;
	aux2=(c3lessc1)*tw;
	return c1+aux1+aux2;*/

	//ENHANCED

	vaux1=p1-p;
    //tu=((p2-p)^(p3-p)).Module()/areaper2;
    faux1=((p3-p)^(vaux1)).Module()/areaper2;
    faux2=((vaux1)^(p2-p)).Module()/areaper2;
    vaux2=(c2lessc1)*faux1;
	vaux3=(c3lessc1)*faux2;
	return c1+vaux2+vaux3;
}

PRTIntersectPoint PRTTriangle::ComputeIntersection(PRTRay r,bool testcull)
{
	// NOT ENHANCED

	PRTIntersectPoint aux;

	//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

	if (!convexhull.IntersectWithRay(r))
		return aux;

	// OBJECT'S INTERSECTION -------------------------------------------------------------------

	if (!testcull || r.dir*facenormal<=0) //object is visible
	{
		PRTFloat EPSILON=PRTFloat(0.000001);
		PRTFloat DELTA=-PRTFloat(0.000001); //abertura de los triangulos negativa ¿? *TODO* para los TriangleLights ha de ser positiva
		/*if (material->color==PRTVector(69,69,69))
			DELTA=0.000001;*/
		PRTFloat LAMBDA=PRTFloat(0.000001); // para la reflexion negativo, para la refraccion y alpha positivo? *TODO*
		/*if (o->material.reflection!=0.0 && o->material.refraction==0.0)
			LAMBDA=-0.000001;
		*/PRTVector edge1,edge2,edge3,tvec,pvec,qvec;
		PRTFloat det,inv_det;
		PRTFloat t,u,v;
		edge1=(p2-p1);
		edge2=(p3-p1);
		pvec=r.dir^edge2;
		det=edge1*pvec;
		if (testcull) //testcull activated
		{
			if(det<EPSILON)
				return aux;
			tvec=r.orig-p1;
			u=tvec*pvec;
			PRTFloat liminf=(/*0.0+*/DELTA*det);
			PRTFloat limsup=(det-DELTA*det);
			if (u<liminf || u>limsup)
				return aux;
			qvec=tvec^edge1;
			v=r.dir*qvec;
			if (v<liminf || u+v>limsup)
				return aux;
			t=edge2*qvec;
			if (t<LAMBDA)
				return aux; //*TODO* only in the ray's direction
			inv_det=PRTFloat(1.0)/det;
			t*=inv_det;
			//u*=inv_det;
			//v*=inv_det;
		}
		else
		{
			if(det>-EPSILON && det<EPSILON)
				return aux;
			inv_det=PRTFloat(1.0)/det;
			tvec=r.orig-p1;
			u=(tvec*pvec)*inv_det;
			PRTFloat liminf=/*0.0+*/DELTA;
			PRTFloat limsup=PRTFloat(1.0)-DELTA;
			if(u<liminf || u>limsup)
				return aux;
			qvec=tvec^edge1;
			v=(r.dir*qvec)*inv_det;
			if(v<liminf||u+v>limsup)
				return aux;
			t=(edge2*qvec)*inv_det;
			if (t<LAMBDA)
				return aux;
		}

		aux.collision=true;
		aux.distance=t;
		aux.point=r.orig+r.dir*t;
		//aux.u=u;
		//aux.v=v;
	}

	return aux;
}

