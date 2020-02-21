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
#include "PRTMaterial.h"
#include <stdlib.h>
#include <math.h> 

PRTMaterial::PRTMaterial(void)
{
	color=PRTVector(1,1,1);
	ambientcolor=PRTVector(0.0,0.0,0.0);
	shininess=PRTFloat(0.01);
	specular=PRTFloat(0.01);
	reflection=0.0;
	refraction=0.0;
	alpha=0.0;
	gireflection=0.0;
	girefraction=0.0;
	gialpha=0.0;
	glossyper=0.0;
	texture=NULL;
	lightmap=NULL;
	bumpmap=NULL;
	bumpfactor=PRTFloat(0.1);
	
	BRDFtype=BRDF_PHONG;
	BRDFp1=-1;
	BRDFp2=-1;
	BRDFp3=-1;
	BRDFp4=-1;
	BRDFp5=-1;
	BRDFp6=-1;
	BRDFp7=-1;

	forced=false;
}

PRTMaterial::PRTMaterial(const PRTVector &c)
{
	ambientcolor=PRTVector(0.0,0.0,0.0);
	shininess=PRTFloat(0.01);
	specular=PRTFloat(0.01);
	reflection=0.0;
	refraction=0.0;
	alpha=0.0;
	color=c;
	gireflection=0.0;
	girefraction=0.0;
	gialpha=0.0;
	glossyper=0.0;
	texture=NULL;
	lightmap=NULL;
	bumpmap=NULL;

	BRDFtype=BRDF_PHONG;
	BRDFp1=-1;
	BRDFp2=-1;
	BRDFp3=-1;
	BRDFp4=-1;
	BRDFp5=-1;
	BRDFp6=-1;
	BRDFp7=-1;

	forced=false;
}

// functions for the BRDF calculus

PRTFloat CLAMP(PRTFloat x,PRTFloat min,PRTFloat max)
{
	return (x > min) ? (x > max ? max : x) : min;
}

#define EPS PRTFloat(0.0001)

PRTFloat Fresnel_term(PRTFloat c, PRTFloat r, PRTFloat i)
{
	PRTFloat g2r=SQR(r)+SQR(c)-PRTFloat(1.0);
	PRTFloat g2i=SQR(i)+SQR(c)-PRTFloat(1.0);
	PRTFloat gabs=PRTFloat(PRTFloat(hypot(g2r, g2i)));
	PRTFloat a=PRTSqrt((gabs+g2r)/PRTFloat(2.0));
	PRTFloat b=PRTSqrt((gabs-g2r)/PRTFloat(2.0));
	PRTFloat st=(PRTFloat(1.0)-SQR(c))/c;
	return PRTFloat(0.5)*(SQR(a-c)+SQR(b))/(SQR(a+c)+SQR(b))*	(PRTFloat(1.0)+(SQR(a-st)+SQR(b))/(SQR(a+st)+SQR(b)));
}

static inline PRTFloat solve_xexpx2(PRTFloat y)
{
	PRTFloat x=(y>1)?PRTSqrt(PRTLog(y)):y;
	int i;
	if (y > 3.0)
		for (i=0;i<4;i++)
			x=PRTSqrt(PRTLog(y/x));
	else
		for (i=0;i<4;i++)
			x=PRTFloat(0.5)*(x+y*PRTExp(-SQR(x)));
	return x;
}

static inline PRTFloat Htsg_sum(PRTFloat g, PRTFloat T)
{
	if (g < 15) {
		PRTFloat sum=0;
		int m=1;
		PRTFloat term1=PRTExp(-g);
		for (;m<40;m++) {
			register PRTFloat recm=(PRTFloat(1.0)/(PRTFloat)m);
			term1*=g*recm;
			sum += term1*recm*PRTExp(T*recm);
		}
		return sum;
	}
	PRTFloat mx=g;
	int i;
	for (i=0;i<4;i++)
		mx=g*PRTExp(-PRTFloat(1.5)/mx-T/SQR(mx));
	return PRTSqrt(g)*PRTExp(mx*PRTLog(g)-g-(mx+PRTFloat(1.5))*PRTLog(mx)+mx+T/mx);
}

inline void FindCrossProd(PRTFloat a, PRTFloat b, PRTFloat c,
			  PRTFloat d, PRTFloat e, PRTFloat f,
			        PRTFloat &g,       PRTFloat &h,       PRTFloat &i)
{
	g = b*f-c*e;
	h = c*d-a*f;
	i = a*e-b*d;
}

double erfc(double Y)
{
	int  ISW,I;
	double P[4],Q[3],P1[6],Q1[5],P2[4],Q2[3];
	double XMIN,XLARGE,SQRPI;
	double X,RES,XSQ,XNUM,XDEN,XI,XBIG,ERFCret;
	P[1]=0.3166529;
	P[2]=1.722276;
	P[3]=21.38533;
	Q[1]=7.843746;
	Q[2]=18.95226;
	P1[1]=0.5631696;
	P1[2]=3.031799;
	P1[3]=6.865018;
	P1[4]=7.373888;
	P1[5]=4.318779e-5;
	Q1[1]=5.354217;
	Q1[2]=12.79553;
	Q1[3]=15.18491;
	Q1[4]=7.373961;
	P2[1]=5.168823e-2;
	P2[2]=0.1960690;
	P2[3]=4.257996e-2;
	Q2[1]=0.9214524;
	Q2[2]=0.1509421;
	XMIN=1.0E-5;
	XLARGE=4.1875E0;
	XBIG=9.0;
	SQRPI=0.5641896;
	X=Y;
	ISW=1;
	if (X<0) 
	{
		ISW=-1;
		X=-X;
	}
	if (X<0.477) 
	{
		if (X>=XMIN) 
		{
			XSQ=X*X;
			XNUM=(P[1]*XSQ+P[2])*XSQ+P[3];
			XDEN=(XSQ+Q[1])*XSQ+Q[2];
			RES=X*XNUM/XDEN;
		}
		else RES=X*P[3]/Q[2];
		if (ISW==-1) RES=-RES;
			RES=1.0-RES;
		goto slut;
	}
	if (X>4.0) 
	{
		if (ISW>0) goto ulf;
		if (X<XLARGE) goto eva;
		RES=2.0;
		goto slut;
	}
	XSQ=X*X;
	XNUM=P1[5]*X+P1[1];
	XDEN=X+Q1[1];
	for(I=2;I<=4;I++) 
	{
		XNUM=XNUM*X+P1[I];
		XDEN=XDEN*X+Q1[I];
	}
	RES=XNUM/XDEN;
	goto elin;
	ulf:    if (X>XBIG) goto fred;
	eva:    XSQ=X*X;
	XI=1.0/XSQ;
	XNUM=(P2[1]*XI+P2[2])*XI+P2[3];
	XDEN=XI+Q2[1]*XI+Q2[2];
	RES=(SQRPI+XI*XNUM/XDEN)/X;
	elin:   RES=RES*exp(-XSQ);
	if (ISW==-1) RES=2.0-RES;
	goto slut;
	fred:   RES=0.0;
	slut:   ERFCret=RES;
	return  ERFCret;
}

//										 input			output			 normal
PRTFloat PRTMaterial::ComputeBRDF(PRTVector &u,const PRTVector &v,const PRTVector &n,PRTFloat p1,PRTFloat p2,PRTFloat p3,PRTFloat p4,PRTFloat p5,PRTFloat p6,PRTFloat p7)
{
	switch (BRDFtype)
	{
	case BRDF_PHONG:
		{
			return (-u)%(n);
			break;
		}
	case BRDF_COSINE_LOBE:
		{	//p1 -> especular?

			if (p1==-1) p1=1;

			return PRTPow((u.Reflection(n)*v),p1)*((p1+2)/PRTFloat(6.283185307179586476925286766559));
			break;
		}
	case BRDF_DIFFUSE: 
		{	//p1 -> Total reflectivity -> [1.0]?

			if (p1==-1) p1=1;

			return (PRTFloat(1.0)/PRTPI)*p1;
			break;
		}
	case BRDF_TS:
		{	//[0.08]    Surface roughness, 
			//[1.6]		Index of refraction (real part), 
			//[-0.2]	(imaginary part), 
			//[0.6]		Specular reflectivity, 
			//[0.4]		Diffuse reflectivity

			if (p1==-1) p1=PRTFloat(0.08);
			if (p2==-1) p2=PRTFloat(1.6);
			if (p3==-1) p3=-PRTFloat(0.2);
			if (p4==-1) p4=PRTFloat(0.6);
			if (p5==-1) p5=PRTFloat(0.4);


			PRTFloat theta_in=PRTFloat(hypot(u.x,u.y));	if (theta_in >= PRTFloat(1.0)) theta_in=(PRTPI/PRTFloat(2.0));	theta_in=PRTASin(theta_in);
			PRTFloat phi_in=PRTFloat(atan2(u.y,u.x));
			PRTFloat theta_out=PRTFloat(hypot(v.x,v.y));	if (theta_out >= PRTFloat(1.0)) theta_out=(PRTPI/PRTFloat(2.0)); theta_out=PRTASin(theta_out);
			PRTFloat phi_out=PRTFloat(atan2(v.y,v.x));

			// Beta is the angle between the normal and the halfway vector 
			PRTFloat ix=PRTSin(theta_in)*PRTCos(phi_in);
			PRTFloat iy=PRTSin(theta_in)*PRTSin(phi_in);
			PRTFloat iz=-PRTCos(theta_in);
			PRTFloat ox=PRTSin(theta_out)*PRTCos(phi_out);
			PRTFloat oy=PRTSin(theta_out)*PRTSin(phi_out);
			PRTFloat oz=PRTCos(theta_out);
			PRTFloat lh=PRTSqrt(SQR(ix+ox)+SQR(iy+oy)+SQR(iz+oz));
			PRTFloat hx=(ix+ox)/lh;
			PRTFloat hy=(iy+oy)/lh;
			PRTFloat h2=(iz+oz)/lh;
			PRTFloat cosbeta=CLAMP(h2,EPS,PRTFloat(1.0)-EPS);
			PRTFloat sinbeta=PRTSqrt(PRTFloat(1.0)-SQR(cosbeta));
			PRTFloat tanbeta=sinbeta/cosbeta;
			PRTFloat c=CLAMP(ix*hx+iy*hy+iz*h2,EPS,PRTFloat(1.0));

			PRTFloat D=PRTExp(-SQR(tanbeta/p1))/SQR(PRTFloat(2.0)*p1*SQR(cosbeta));
			PRTFloat G=MIN(PRTFloat(1.0),MIN(PRTFloat(2.0)*cosbeta*iz/c,PRTFloat(2.0)*cosbeta*oz/c));
			PRTFloat F=Fresnel_term(c,p2,p3);

			return (p5+p4*F*D*G/iz/oz)/PRTPI;
			break;
		}
	case BRDF_BM: //¿?¿?¿ todo
		{
			//[0.4]		Surface shadowing parameter, 
			//[0.15]	Surface shadowing parameter,
            //[3.5]		Index of refraction (real part)
            //[-0.2]	(imaginary part)
            //[0.8]		First surface reflectivity
            //[0.1]		Diffuse reflectivity
            //[0.1]		Volumetric reflectivity

			if (p1==-1) p1=PRTFloat(0.4);
			if (p2==-1) p2=PRTFloat(0.15);
			if (p3==-1) p3=PRTFloat(3.5);
			if (p4==-1) p4=-PRTFloat(0.2);
			if (p5==-1) p5=PRTFloat(0.8);
			if (p6==-1) p6=PRTFloat(0.1);
			if (p7==-1) p7=PRTFloat(0.1);

			PRTFloat theta_in=PRTFloat(hypot(u.x,u.y));	if (theta_in >= PRTFloat(1.0)) theta_in=(PRTPI/PRTFloat(2.0));	theta_in=PRTASin(theta_in);
			PRTFloat phi_in=PRTFloat(atan2(u.y,u.x));
			PRTFloat theta_out=PRTFloat(hypot(v.x,v.y));	if (theta_out >= PRTFloat(1.0)) theta_out=(PRTPI/PRTFloat(2.0)); theta_out=PRTASin(theta_out);
			PRTFloat phi_out=PRTFloat(atan2(v.y,v.x));

			// Beta is the angle between the normal and the halfway vector 
			PRTFloat ix=PRTSin(theta_in)*PRTCos(phi_in);
			PRTFloat iy=PRTSin(theta_in)*PRTSin(phi_in);
			PRTFloat iz=PRTCos(theta_in);
			PRTFloat ox=PRTSin(theta_out)*PRTCos(phi_out);
			PRTFloat oy=PRTSin(theta_out)*PRTSin(phi_out);
			PRTFloat oz=PRTCos(theta_out);
			PRTFloat lh=PRTSqrt(SQR(ix+ox)+SQR(iy+oy)+SQR(iz+oz));
			PRTFloat hx=(ix+ox)/lh;
			PRTFloat hy=(iy+oy)/lh;
			PRTFloat h2=(iz+oz)/lh;
			PRTFloat cosbeta=CLAMP(h2,EPS,PRTFloat(1.0)-EPS);
			PRTFloat beta=PRTACos(cosbeta);
			PRTFloat c=CLAMP(ix*hx+iy*hy+iz*h2,EPS,PRTFloat(1.0));

			PRTFloat SO=(PRTFloat(1.0)+beta/p1*PRTExp(-PRTFloat(2.0)*PRTACos(c)/p2))/(PRTFloat(1.0)+beta/p1);
			PRTFloat F=Fresnel_term(c,p3,p4);

			return (p6+PRTFloat(2.0)*p7/MAX(iz+oz,EPS)+p5*SQR(cosbeta)*SO*F/iz/oz)/PRTPI;

			break;
		}
	case BRDF_ON:
		{
			//[0.3]     Surface roughness
            //[1]       Reflectivity

			if (p1==-1) p1=PRTFloat(0.3);
			if (p2==-1) p2=1;
			
			PRTFloat theta_in=PRTFloat(hypot(u.x,u.y));	if (theta_in >= PRTFloat(1.0)) theta_in=(PRTPI/PRTFloat(2.0));	theta_in=PRTASin(theta_in);
			PRTFloat phi_in=PRTFloat(atan2(u.y,u.x));
			PRTFloat theta_out=PRTFloat(hypot(v.x,v.y));	if (theta_out >= PRTFloat(1.0)) theta_out=(PRTPI/PRTFloat(2.0)); theta_out=PRTASin(theta_out);
			PRTFloat phi_out=PRTFloat(atan2(v.y,v.x));


			PRTFloat alpha=MAX(theta_in,theta_out);
			PRTFloat beta=MIN(theta_in,theta_out);
			PRTFloat c=PRTCos(phi_out-phi_in);
			PRTFloat C1=PRTFloat(1.0)-PRTFloat(0.5)*SQR(p1)/(SQR(p1)+PRTFloat(0.33));
			PRTFloat C2=(c > 0) ?
				PRTFloat(0.45)*SQR(p1)/(SQR(p1)+PRTFloat(0.09))*PRTSin(alpha) :
				PRTFloat(0.45)*SQR(p1)/(SQR(p1)+PRTFloat(0.09))*(PRTSin(alpha)-CUBE(PRTFloat(2.0)*beta/PRTPI));
			PRTFloat C3=PRTFloat(0.125)*SQR(p1)/(SQR(p1)+PRTFloat(0.09))*SQR(PRTFloat(4.0)*alpha*beta/SQR(PRTPI));
			PRTFloat L1=p2/PRTPI*(
				C1 +
				c*C2*PRTTan(beta) +
				(PRTFloat(1.0)-PRTAbs(c))*C3*PRTTan((alpha+beta)/PRTFloat(2.0)) );
			PRTFloat L2=PRTFloat(0.17)*SQR(p2)/PRTPI*SQR(p1)/(SQR(p1)+PRTFloat(0.13))*(PRTFloat(1.0)-c*SQR(PRTFloat(2.0)*beta/PRTPI));
			return L1+L2;

			break;
		}
	case BRDF_HTSG: //¿?
		{
			//[20]      tau/lambda in He's model
            //[1]       sigma/lambda in He's model
            //[1.6]		Index of refraction (real part)\n"
            //[-0.2]    (imaginary part)\n"
            //[0.8]		Specular reflectivity\n"
            //[0.2]		Diffuse reflectivity\n"

			if (p1==-1) p1=20;
			if (p2==-1) p2=1;
			if (p3==-1) p3=PRTFloat(1.6);
			if (p4==-1) p4=-PRTFloat(0.2);
			if (p5==-1) p5=PRTFloat(0.8);
			if (p6==-1) p6=PRTFloat(0.2);
			
			PRTFloat theta_in=PRTFloat(hypot(u.x,u.y));	if (theta_in >= PRTFloat(1.0)) theta_in=(PRTPI/PRTFloat(2.0));	theta_in=PRTASin(theta_in);
			PRTFloat phi_in=PRTFloat(atan2(u.y,u.x));
			PRTFloat theta_out=PRTFloat(hypot(v.x,v.y));	if (theta_out >= PRTFloat(1.0)) theta_out=(PRTPI/PRTFloat(2.0)); theta_out=PRTASin(theta_out);
			PRTFloat phi_out=PRTFloat(atan2(v.y,v.x));
	
			if (theta_in < EPS) {
				theta_in = EPS;
				phi_in=0;
			}
			if (theta_out < EPS) {
				theta_out = EPS;
				phi_out=PRTPI/PRTFloat(2.0);
			}

			PRTFloat ix=PRTSin(theta_in)*PRTCos(phi_in);
			PRTFloat iy=PRTSin(theta_in)*PRTSin(phi_in);
			PRTFloat iz=PRTCos(theta_in);
			PRTFloat ox=PRTSin(theta_out)*PRTCos(phi_out);
			PRTFloat oy=PRTSin(theta_out)*PRTSin(phi_out);
			PRTFloat oz=PRTCos(theta_out);
			PRTFloat lh=PRTSqrt(SQR(ix+ox)+SQR(iy+oy)+SQR(iz+oz));
			PRTFloat hx=(ix+ox)/lh;
			PRTFloat hy=(iy+oy)/lh;
			PRTFloat h2=(iz+oz)/lh;

			PRTFloat c=CLAMP(ix*hx+iy*hy+iz*h2,EPS,1.0);

			PRTFloat vx=ox+ix, vy=oy+iy, vz=oz+iz;
			PRTFloat vxy2=SQR(vx)+SQR(vy);
			PRTFloat six= -iy, siy=ix, siz=0;
			PRTFloat lsi=PRTSqrt(SQR(six)+SQR(siy)+SQR(siz));
			six/=lsi; siy/=lsi; siz/=lsi;
			PRTFloat sox=oy, soy= -ox, soz=0;
			PRTFloat lso=PRTSqrt(SQR(sox)+SQR(soy)+SQR(soz));
			sox/=lso; soy/=lso; soz/=lso;
			PRTFloat pix,piy,piz, pox,poy,poz;
			FindCrossProd(six,siy,siz,-ix,-iy,-iz,pix,piy,piz);
			FindCrossProd(sox,soy,soz,ox,oy,oz,pox,poy,poz);

			PRTFloat Ci=(p1/p2/PRTFloat(2.0)/PRTTan(theta_in));
			PRTFloat Co=(p1/p2/PRTFloat(2.0)/PRTTan(theta_out));
			PRTFloat Ei=PRTFloat(erfc(Ci));
			PRTFloat Eo=PRTFloat(erfc(Co));
			PRTFloat Ki=PRTTan(theta_in)*Ei;
			PRTFloat Ko=PRTTan(theta_out)*Eo;

			PRTFloat zo_over_so_over_sqrt2=solve_xexpx2((Ki+Ko)/PRTSqrt(PRTPI)/PRTFloat(4.0));
			PRTFloat sigma=p2/PRTSqrt(PRTFloat(1.0)+PRTFloat(2.0)*SQR(zo_over_so_over_sqrt2));
			PRTFloat g=SQR(PRTFloat(2.0)*PRTPI*sigma*(iz+oz));
			PRTFloat T = -vxy2*SQR(PRTPI*p1);
			PRTFloat D=SQR(PRTPI*p1/PRTFloat(2.0))*Htsg_sum(g,T);

			PRTFloat S=(PRTFloat(1.0)-PRTFloat(0.5)*Ei)/(PRTFloat(1.0)+PRTFloat(0.5)*(PRTFloat(1.0)/PRTSqrt(PRTPI)/Ci-Ei))*
					(PRTFloat(1.0)-PRTFloat(0.5)*Eo)/(PRTFloat(1.0)+PRTFloat(0.5)*(PRTFloat(1.0)/PRTSqrt(PRTPI)/Co-Eo));

			PRTFloat kkx, kky, kkz;
			FindCrossProd(ox,oy,oz,-ix,-iy,-iz,kkx,kky,kkz);
			PRTFloat kk4=SQR(SQR(kkx)+SQR(kky)+SQR(kkz));
			PRTFloat G=SQR((SQR(vx)+SQR(vy)+SQR(vz))/vz)/kk4*
				(SQR(sox*ix+soy*iy+soz*iz)+SQR(pox*ix+poy*iy+poz*iz))*
				(SQR(six*ox+siy*oy+siz*oz)+SQR(pix*ox+piy*oy+piz*oz));

			PRTFloat F=Fresnel_term(c,p3,p4);

			return (p6+F*G*S*D*p5/iz/oz)/PRTPI;
			break;
		}
	}
	return 0;
}
