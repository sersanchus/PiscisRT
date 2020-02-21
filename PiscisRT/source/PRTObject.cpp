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
#include "PRTObject.h"

PRTObject::PRTObject(PRTVector posaux,int t,PRTMaterial* m)
{
	type=t;
	material=m;
	position=posaux;
	despu=0;
	despv=0;
	repu=1;
	repv=1;
}

/*PRTVector PRTObject::ComputeTransformatedNormal(PRTVector point)
{
	PRTMatrix inv;
	
	inv=trans.Invert();
		
	PRTVector p=inv*point;

	PRTVector normal=ComputeNormal(p);

	return ((trans*(p+normal))-point).Normalize();

}*/

PRTIntersectPoint PRTObject::ComputeTransformatedIntersection(PRTRay r,bool doublesided)
{
	PRTRay ray;
					
	ray.orig=invtrans*r.orig;
	ray.dir=invtransrot*r.dir;//((inv*(r.orig+r.dir))-ray.orig).Normalize();
		
	PRTIntersectPoint aux=ComputeIntersection(ray,doublesided);	
	if (aux.collision)
	{
		aux.point=trans*aux.point;
	}
	return aux;
}

void PRTObject::Rotate(PRTVector axis,PRTFloat angle,PRTVector ref)// *TODO* ref
{
	PRTMatrix PosRot;
	
	/*PRTMatrix PosRotZ;
	PosRotZ.RotateZ(axis.z*-angle);//in degrees
	PRTMatrix PosRotY;
	PosRotY.RotateY(axis.y*-angle);
	PRTMatrix PosRotX;
	PosRotX.RotateX(axis.x*-angle);

	PosRot=PosRotZ*PosRotY*PosRotX; //in order z,y,x*/

	PRTFloat c=PRTCos(PRTGradToRad(angle));
	PRTFloat s=PRTSin(PRTGradToRad(angle));
	PRTFloat t=1-c;
	PosRot.col[0]=PRTVector4(t*axis.x*axis.x+c,t*axis.x*axis.y+axis.z*s,t*axis.x*axis.z-axis.y*s,0);
	PosRot.col[1]=PRTVector4(t*axis.x*axis.y-axis.z*s,t*axis.y*axis.y+c,t*axis.y*axis.z+axis.x*s,0);
	PosRot.col[2]=PRTVector4(t*axis.x*axis.z+axis.y*s,t*axis.y*axis.z-axis.x*s,t*axis.z*axis.z+c,0);

	trans=trans*PosRot;
}

void PRTObject::Translate(PRTVector vector)
{
	PRTMatrix aux;
	aux.col[3]=vector;
	trans=aux*trans;
}

void PRTObject::Scale(PRTVector factor,PRTVector ref)// *TODO* ref
{
	PRTMatrix auxscal;
	auxscal.col[0]=PRTVector4(factor.x,0,0,0);
	auxscal.col[1]=PRTVector4(0,factor.y,0,0);
	auxscal.col[2]=PRTVector4(0,0,factor.z,0);
	trans=auxscal*trans;
}

/*

void PRTTriangle::Rotate(PRTVector eje,PRTFloat angle,PRTVector ref)
{

	// NOT ENHANCED

	//Euler angles to rotation matrix (transform matrix)
	PRTMatrix PosRot;
	
	PRTMatrix PosRotZ;
	PosRotZ.RotateZ(eje.z*angle);//in degrees
	PRTMatrix PosRotY;
	PosRotY.RotateY(eje.y*angle);
	PRTMatrix PosRotX;
	PosRotX.RotateX(eje.x*angle);

	PosRot=PosRotZ*PosRotY*PosRotX; //in order z,y,x

	//PosRot.TranslateAbsolute(pos,type);

	//chcen=chcen-ref;
	//chini=chini-ref;
	//chfin=chfin-ref;
	p1=p1-ref;
	p2=p2-ref;
	p3=p3-ref;
	position=position-ref;
	//n1=n1-ref;
	//n2=n2-ref;
	//n3=n3-ref;

	//chcen=PosRot*chcen;
	//chini=PosRot*chini;
	//chfin=PosRot*chfin;


	p1=PosRot*p1;
	p2=PosRot*p2;
	p3=PosRot*p3;
	
	n1=PosRot*n1;
	n2=PosRot*n2;
	n3=PosRot*n3;

	position=PosRot*position;

	//chcen=chcen+ref;
	//chini=chini+ref;
	//chfin=chfin+ref;
	p1=p1+ref;
	p2=p2+ref;
	p3=p3+ref;
	//n1=n1+ref;
	//n2=n2+ref;
	//n3=n3+ref;
	position=position+ref;


	//recalcular convex hull :(
	//PRTFloat inix,finx,iniy,finy,iniz,finz;
	//if (p1.x<p2.x){if (p3.x<p1.x)inix=p3.x;else inix=p1.x;}else{if (p3.x<p2.x)inix=p3.x;else inix=p2.x;}
	//if (p1.x>p2.x){if (p3.x>p1.x)finx=p3.x;else finx=p1.x;}else{if (p3.x>p2.x)finx=p3.x;else finx=p2.x;}
	//if (p1.y<p2.y){if (p3.y<p1.y)iniy=p3.y;else iniy=p1.y;}else{if (p3.y<p2.y)iniy=p3.y;else iniy=p2.y;}
	//if (p1.y>p2.y){if (p3.y>p1.y)finy=p3.y;else finy=p1.y;}else{if (p3.y>p2.y)finy=p3.y;else finy=p2.y;}
	//if (p1.z<p2.z){if (p3.z<p1.z)iniz=p3.z;else iniz=p1.z;}else{if (p3.z<p2.z)iniz=p3.z;else iniz=p2.z;}
	//if (p1.z>p2.z){if (p3.z>p1.z)finz=p3.z;else finz=p1.z;}else{if (p3.z>p2.z)finz=p3.z;else finz=p2.z;}		
	//convexhull.chbeg=PRTVector(inix,iniy,iniz);
	//convexhull.chend=PRTVector(finx,finy,finz);
	//convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	//convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;

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
}

void PRTTriangle::Translate(PRTVector disp)
{
	position+=disp;
	p1+=disp;
	p2+=disp;
	p3+=disp;
	convexhull.chbeg+=disp;
	convexhull.chend+=disp;
	convexhull.chcen+=disp;
	//convexhull.SolvePrecision();
}

void PRTTriangle::Scale(PRTVector factor)
{

}*/

void PRTObject::Transform(PRTMatrix mat)
{
	trans=mat;
	invtrans=mat.Invert();
	PRTMatrix aux;
	aux.col[0]=mat.col[0];
	aux.col[1]=mat.col[1];
	aux.col[2]=mat.col[2];
	transrot=aux;
	invtransrot=transrot.Invert();
}
