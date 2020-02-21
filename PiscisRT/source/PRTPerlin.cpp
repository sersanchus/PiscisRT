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
#include "PRTPerlin.h"
#include "PRTMath.h"
#include <string.h>
#include <stdlib.h>
#include "PRTRandom.h"

// from Ken Perlin code

#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff
#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )
static int p[B + B + 2];
static double g1[B + B + 2];
static double g2[B + B + 2][2];
static double g3[B + B + 2][3];
//static int start = 1;

#define setup(i,b0,b1,r0,r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;

void normalize2(double v[2])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
}

void normalize3(double v[3])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
   v[2] = v[2] / s;
}

void init(void)
{
   int i, j, k;

   for (i = 0 ; i < B ; i++) 
   {
      p[i] = i;
      g1[i] = (double)((rand() % (B + B)) - B) / B;
      for (j = 0 ; j < 2 ; j++)
         g2[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize2(g2[i]);
      for (j = 0 ; j < 3 ; j++)
         g3[i][j] = (double)((rand() % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = rand() % B];
      p[j] = k;
   }

   for (i = 0 ; i < B + 2 ; i++) {
      p[B + i] = p[i];
      g1[B + i] = g1[i];
      for (j = 0 ; j < 2 ; j++)
         g2[B + i][j] = g2[i][j];
      for (j = 0 ; j < 3 ; j++)
         g3[B + i][j] = g3[i][j];
   }
}

double noise1(double arg)
{
   int bx0, bx1;
   double rx0, rx1, sx, t, u, v, vec[1];

   vec[0] = arg;
   /*if (start) {
      start = 0;
      init();
   }*/

   setup(0,bx0,bx1,rx0,rx1);

   sx = s_curve(rx0);
   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return(lerp(sx, u, v));
}

double PerlinNoise1D(double x,double alpha,double beta,int n,double scale2)
{
   int i;
   double val,sum = 0;
   double p;

   p = x;
   for (i=0;i<n;i++) {
      val = noise1(p);
      sum += val / scale2;
      scale2 *= alpha;
      p *= beta;
   }
   return(sum);
}

double noise2(double vec[2])
{
   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
   int i, j;

   /*if (start) {
      start = 0;
      init();
   }*/

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);
   sy = s_curve(ry0);

   q = g2[ b00 ] ; u = at2(rx0,ry0);
   q = g2[ b10 ] ; v = at2(rx1,ry0);
   a = lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1);
   q = g2[ b11 ] ; v = at2(rx1,ry1);
   b = lerp(sx, u, v);

   return lerp(sy, a, b);
}

double PerlinNoise2D(double x,double y,double alpha,double beta,int n,double scale2)
{
   int i;
   double val,sum = 0;
   double p[2];

   p[0] = x;
   p[1] = y;
   for (i=0;i<n;i++) {
      val = noise2(p);
      sum += val / scale2;
      scale2 *= alpha;
      p[0] *= beta;
      p[1] *= beta;
   }
   return(sum);
}

double noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   /*if (start) {
      start = 0;
      init();
   }*/

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);
   setup(2, bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = lerp(t, u, v);

   c = lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = lerp(t, u, v);

   d = lerp(sy, a, b);

   return lerp(sz, c, d);
}

double PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n,double scale2)
{
   int i;
   double val,sum = 0;
   double p[3];

   p[0] = x;
   p[1] = y;
   p[2] = z;
   for (i=0;i<n;i++) {
      val = noise3(p);
      sum += fabs(val/scale2); // |noise| Turbulence 
	  //sum += val/scale2; // noise
      scale2 *= alpha;
      p[0] *= beta;
      p[1] *= beta;
      p[2] *= beta;
   }
   return(sum);
}

PRTPerlin::PRTPerlin(int t)
	:PRTTexture(PRT_TEXTURE_PERLIN)
{
	//t=NULL;
	//width=height=bytespp=-1;
	//filter=false;
	//PRTRandomInit(3);
	type=t;
	srand(3);
	init();
	scale=50;
	alpha=2;
	beta=2;
	n=5;
}

/*PRTPerlin::PRTPerlin(char *ruta)
	:PRTTexture(PRT_TEXTURE_PERLIN)
{
	t=NULL;
	width=height=bytespp=-1;
	Load(ruta);
	filter=false;
}*/

PRTPerlin::~PRTPerlin(void)
{
	//if (t!=NULL) //*TODO* caution with the objects that have a pointer to the texture
	//	delete[] t;
}

PRTVector PRTPerlin::CalculateColor(PRTTexCoord aux, PRTVector p)
{
	/*PRTVector color1(0.7294,0.7091,0.5882);
	PRTVector color2(0.4901,0.3568,0.2235);*/

	PRTFloat tu,tv;
	tu=aux.u-(PRTFloat)(int)aux.u;
	tv=aux.v-(PRTFloat)(int)aux.v;

	if (tu<0) tu+=1;
	if (tv<0) tv+=1;

	unsigned int coordx=(unsigned int)(tu*(PRTFloat)(width));
	unsigned int coordy=(unsigned int)(((PRTFloat)1-tv)*(PRTFloat)(height));

	if (coordx==width) coordx=0;
	if (coordy==height) coordy=0;
	
	PRTVector ret;
	if (!filter || (width<=1 || height<=1))
	{
		// without filter
		//double jur=PerlinNoise2D(tu,tv,2,2,10);
		/*double jur=PerlinNoise2D(tu,tv,2,2,10)*20;
		jur=jur-(double)int(jur);*/
		//jur=(jur+1.0)/2.0;
		
		PRTFloat jur;

		switch(type)
		{
			case PRT_TEXTURE_PERLIN_CLOUD:
				jur=(PRTFloat)PerlinNoise3D(p.x/scale,p.y/scale,p.z/scale,alpha,beta,n,1/*scale*/);
				break;
			case PRT_TEXTURE_PERLIN_MARBLE:
				jur=(PRTFloat)PerlinNoise3D(p.x/scale,p.y/scale,p.z/scale,alpha,beta,n,1)*10;
				jur=jur-(PRTFloat)int(jur);
				break;
			case PRT_TEXTURE_PERLIN_WOOD:	
				//			sin	( period*x		   + distortion*noise )
				jur=/*PRTAbs*/(PRTSin(( PRTFloat(0.4)*(p.x/scale) + PRTFloat(3.0)*(PRTFloat)PerlinNoise3D(p.x/scale,p.y/scale,p.z/scale,alpha,beta,n,PRTFloat(1.0)/*scale*/) )*PRTPI));
				jur=(jur+PRTFloat(1.0))*PRTFloat(0.5);
				break;				
		}
		
		ret=color1*(PRTFloat(1.0)-jur)+color2*(jur);
	}
	else
	{
		// with filter
		PRTVector aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8,aux9;

		PRTFloat peichx=PRTFloat(1.0)/(PRTFloat)(width);//*/0.005; *TODO*
		PRTFloat peichy=PRTFloat(1.0)/(PRTFloat)(height);//*/0.005; *TODO*
		
		filter=false;
		aux1=CalculateColor(aux+PRTTexCoord(-peichx,peichy));
		aux2=CalculateColor(aux+PRTTexCoord(0,peichy));
		aux3=CalculateColor(aux+PRTTexCoord(peichx,peichy));
		aux4=CalculateColor(aux+PRTTexCoord(-peichx,0));
		aux5=CalculateColor(aux+PRTTexCoord(0,0));
		aux6=CalculateColor(aux+PRTTexCoord(peichx,0));
		aux7=CalculateColor(aux+PRTTexCoord(-peichx,-peichy));
		aux8=CalculateColor(aux+PRTTexCoord(0,-peichy));
		aux9=CalculateColor(aux+PRTTexCoord(peichx,-peichy));
		ret=(aux1+aux2+aux3+aux4+aux5+aux6+aux7+aux8+aux9)/9;
		filter=true;
	}

	return ret;
}

PRTVector PRTPerlin::CalculateBump(PRTTexCoord aux,PRTVector normaltou,PRTVector normaltov,PRTVector p)
{
/*	PRTFloat tu,tv;
	tu=aux.u-(PRTFloat)(int)aux.u;
	tv=aux.v-(PRTFloat)(int)aux.v;

	if (tu<0) tu+=1;
	if (tv<0) tv+=1;

	int coordx=(int)(tu*(PRTFloat)(width));
	int coordy=(int)(((PRTFloat)1-tv)*(PRTFloat)(height));

	if (coordx==width) coordx=0;
	if (coordy==height) coordy=0;
*/
	PRTFloat peichx=PRTFloat(1.0)/(PRTFloat)(width);//*/0.005; *TODO*
	PRTFloat peichy=PRTFloat(1.0)/(PRTFloat)(height);//*/0.005; *TODO*

	PRTVector ret;
	if (!filter)
	{
		// without filter

		PRTFloat inf=0.0001f;

		//PRTFloat mul=0.3f;
			
		PRTFloat p0=CalculateColor(aux,p).x/*/255*/;
		PRTFloat pu=CalculateColor(aux+PRTTexCoord(peichx,0),p+normaltou*inf).x/*/255*/;
		PRTFloat pv=CalculateColor(aux+PRTTexCoord(0,peichy),p+normaltov*inf).x/*/255*/;
		PRTFloat pu2=CalculateColor(aux+PRTTexCoord(peichx,0),p-normaltou*inf).x/*/255*/;
		PRTFloat pv2=CalculateColor(aux+PRTTexCoord(0,peichy),p-normaltov*inf).x/*/255*/;
		//PRTFloat p11=CalculateColor(aux+PRTTexCoord(peichx,peichy),p+normaltou*0.01+normaltov*0.01).x/255;

		//ret=(normaltou*((pu-p0)/inf)+normaltov*((pv-p0)/inf))*100000;
		PRTFloat enu=-PRTSin((pu-pu2)/(2.0f*inf));
		PRTFloat env=-PRTSin((pv-pv2)/(2.0f*inf));
		//ret=(normaltou*(mul*enu)+normaltov*(mul*env));
		ret=(normaltou*enu+normaltov*env);

		//ret=p0*8000;
	}
	else
	{
		// with filter
		PRTVector aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8,aux9;
		
		filter=false;

		aux1=CalculateBump(aux+PRTTexCoord(-peichx,peichy),normaltou,normaltov,p);
		aux2=CalculateBump(aux+PRTTexCoord(0,peichy),normaltou,normaltov,p);
		aux3=CalculateBump(aux+PRTTexCoord(peichx,peichy),normaltou,normaltov,p);
		aux4=CalculateBump(aux+PRTTexCoord(-peichx,0),normaltou,normaltov,p);
		aux5=CalculateBump(aux+PRTTexCoord(0,0),normaltou,normaltov,p);
		aux6=CalculateBump(aux+PRTTexCoord(peichx,0),normaltou,normaltov,p);
		aux7=CalculateBump(aux+PRTTexCoord(-peichx,-peichy),normaltou,normaltov,p);
		aux8=CalculateBump(aux+PRTTexCoord(0,-peichy),normaltou,normaltov,p);
		aux9=CalculateBump(aux+PRTTexCoord(peichx,-peichy),normaltou,normaltov,p);
		ret=(aux1+aux2+aux3+aux4+aux5+aux6+aux7+aux8+aux9)/9;
		filter=true;
	}

	return ret;
}
