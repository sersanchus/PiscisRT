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

#include "PRTMath.h"

int sqrt_tab[SQRT_TAB_SIZE];

void PRTInitFastSqrt()
{
        int           i;
        double        f;
        unsigned int  *fi = (unsigned int *) &f + MOST_SIG_OFFSET;
        
        for (i = 0; i < (int)(SQRT_TAB_SIZE*0.5); i++)
        {
                f = 0; /* Clears least sig part */
                *fi = (i << MANT_SHIFTS) | (EXP_BIAS << EXP_SHIFTS);
                f = sqrt(f);
                sqrt_tab[i] = *fi & MANT_MASK;

                f = 0; /* Clears least sig part */
                *fi = (i << MANT_SHIFTS) | ((EXP_BIAS + 1) << EXP_SHIFTS);
                f = sqrt(f);
                sqrt_tab[i + (int)(SQRT_TAB_SIZE*0.5)] = *fi & MANT_MASK;
        }
}

PRTFloat fsqrt(PRTFloat f2)
{
		double f=f2;
        unsigned int e;
        unsigned int *fi = (unsigned int *) &f + MOST_SIG_OFFSET;

        if (f == 0.0) return(0.0);
        e = (*fi >> EXP_SHIFTS) - EXP_BIAS;
        *fi &= MANT_MASK;
        if (e & 1)
                *fi |= EXP_LSB;
        e >>= 1;
        *fi = (sqrt_tab[*fi >> MANT_SHIFTS]) |
              ((e + EXP_BIAS) << EXP_SHIFTS);
        return PRTFloat(f);
	//return sqrt(f);
}





PRTFloat PRTGradToRad(PRTFloat x){return x*PRTFloat(0.017453292519943295769236907684888);}
PRTFloat PRTRadToGrad(PRTFloat x){return x*PRTFloat(57.295779513082320876798154814105);}




bool Segment2dSegment2dInter(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector p4)
{
    PRTFloat denom=((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
    if (fabs(denom)<0.0000001)
		return false;//not intersected lines
    PRTFloat alpha1=((p4.x-p3.x)*(p1.y-p3.y)-(p4.y-p3.y)*(p1.x-p3.x))/denom;
    PRTFloat alpha2=((p2.x-p1.x)*(p1.y-p3.y)-(p2.y-p1.y)*(p1.x-p3.x))/denom;
    return ((alpha1>=0 && alpha1<=1) && (alpha2>=0 && alpha2<=1));
}

PRTVector Segment2dSegment2dInterPoint(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector p4)
{
    PRTFloat denom=((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
    PRTFloat alpha1=((p4.x-p3.x)*(p1.y-p3.y)-(p4.y-p3.y)*(p1.x-p3.x))/denom;
    PRTVector aux=(p1+((p2-p1)*alpha1));
    return aux;
}

bool SameSide(PRTVector p1,PRTVector p2,PRTVector a,PRTVector b)
{
        PRTVector cp1=(b-a)^(p1-a);
        PRTVector cp2=(b-a)^(p2-a);
        if (cp1*cp2>=0)
                return true;
        else
                return false;
}

char HitBoundingBox(PRTVector minB,PRTVector maxB,PRTVector origin,PRTVector dir,PRTVector coord)
{
	char inside = true;
	char quadrant[3];
	register int i;
	int whichPlane;
	double maxT[3];
	double candidatePlane[3];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<3; i++)
		if(origin[i] < minB[i]) {
			quadrant[i] = 1;
			candidatePlane[i] = minB[i];
			inside = false;
		}else if (origin[i] > maxB[i]) {
			quadrant[i] = 0;
			candidatePlane[i] = maxB[i];
			inside = false;
		}else	{
			quadrant[i] = 2;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		coord = origin;
		return (true);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < 3; i++)
		if (quadrant[i] != 2 && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (false);
	for (i = 0; i < 3; i++)
		if (whichPlane != i) {
			coord[i] = PRTFloat(origin[i] + maxT[whichPlane] *dir[i]);
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (false);
		} else {
			coord[i] = PRTFloat(candidatePlane[i]);
		}
	return (true);				/* ray hits box */
}





PRTFloat ArcCos(PRTFloat x)
{
	PRTFloat y;

	if (-1.0 <= x && x <= 1.0)
		y = PRTACos( x );
	else if (x >  1.0)
		y = 0.0;
	else if (x < -1.0)
		y = PRTPI;

	return y;
}




#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#define     cbrt(x)     ((x) > 0.0 ? pow((PRTFloat)(x), 1.0/3.0) : ((x) < 0.0 ? -pow((PRTFloat)-(x), 1.0/3.0) : 0.0))

int SolveQuadric(PRTFloat c[ 3 ],PRTFloat s[ 2 ])
{
    PRTFloat p, q, D;

    /* normal form: x^2 + px + q = 0 */

    p = c[ 1 ] / (2 * c[ 2 ]);
    q = c[ 0 ] / c[ 2 ];

    D = p * p - q;

    if (IsZero(D))
    {
		s[ 0 ] = - p;
		return 1;
    }
    else 
	{
		if (D < 0)
		{
			return 0;
		}
		else 
		{
			if (D > 0)
			{
				PRTFloat sqrt_D = PRTSqrt(D);
	
				s[ 0 ] =   sqrt_D - p;
				s[ 1 ] = - sqrt_D - p;
				return 2;
			}
			return 0;
		}
	}
}

int SolveCubic(PRTFloat c[4], PRTFloat s[3])
{
    int     i, num;
    PRTFloat  sub;
    PRTFloat  A, B, C;
    PRTFloat  sq_A, p, q;
    PRTFloat  cb_p, D;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */

    A = c[ 2 ] / c[ 3 ];
    B = c[ 1 ] / c[ 3 ];
    C = c[ 0 ] / c[ 3 ];

    /*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */

    sq_A = A * A;
    p = PRTFloat( 1.0/3 * (-0.333333333333333333333333333333333 /*1.0/3*/ * sq_A + B));
    q = PRTFloat( 1.0/2 * (0.074074074074074074074074074074074 /*2.0/27*/ * A * sq_A - 0.333333333333333333333333333333333/*1.0/3*/ * A * B + C) );

    /* use Cardano's formula */

    cb_p = p * p * p;
    D = q * q + cb_p;

    if (IsZero(D))
    {
	if (IsZero(q)) /* one triple solution */
	{
	    s[ 0 ] = 0;
	    num = 1;
	}
	else /* one single and one PRTFloat solution */
	{
	    PRTFloat u = PRTFloat(cbrt(-q));
	    s[ 0 ] = 2 * u;
	    s[ 1 ] = - u;
	    num = 2;
	}
    }
    else if (D < 0) /* Casus irreducibilis: three real solutions */
    {
	PRTFloat phi = PRTFloat(0.333333333333333333333333333333333/*1.0/3*/ * PRTACos(-q / PRTSqrt(-cb_p)));
	PRTFloat t = 2 * PRTSqrt(-p);

	s[ 0 ] =   t * PRTCos(phi);
	s[ 1 ] = - t * PRTCos(phi + PRTPI * PRTFloat(0.333333333333333333333333333333333)/*/ 3*/);
	s[ 2 ] = - t * PRTCos(phi - PRTPI * PRTFloat(0.333333333333333333333333333333333)/*/ 3*/);
	num = 3;
    }
    else /* one real solution */
    {
	PRTFloat sqrt_D = PRTSqrt(D);
	PRTFloat u = PRTFloat(cbrt(sqrt_D - q));
	PRTFloat v = -PRTFloat(cbrt(sqrt_D + q));

	s[ 0 ] = u + v;
	num = 1;
    }

    /* resubstitute */

    sub = PRTFloat(0.333333333333333333333333333333333 )/*1.0/3*/ * A;

    for (i = 0; i < num; ++i)
	s[ i ] -= sub;

    return num;
}

int SolveQuartic(PRTFloat c[5], PRTFloat s[4])
{
	PRTFloat  coeffs[ 4 ];
	PRTFloat  z, u, v, sub;
	PRTFloat  A, B, C, D;
	PRTFloat  sq_A, p, q, r;
	int     i, num;

	/* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

	A = c[ 3 ] / c[ 4 ];
	B = c[ 2 ] / c[ 4 ];
	C = c[ 1 ] / c[ 4 ];
	D = c[ 0 ] / c[ 4 ];

	/*  substitute x = y - A/4 to eliminate cubic term:
	x^4 + px^2 + qx + r = 0 */

	sq_A = A * A;
	p = - PRTFloat(0.375 )/*/3.0/8*/ * sq_A + B;
	q = PRTFloat(0.125 /*1.0/8*/ * sq_A * A - 0.5/*1.0/2*/ * A * B + C);
	r = PRTFloat( - 0.01171875 /*3.0/256*/*sq_A*sq_A + 0.0625/*1.0/16*/*sq_A*B - 0.25/*1.0/4*/*A*C + D );

	if (IsZero(r))
	{
	/* no absolute term: y(y^3 + py + q) = 0 */

	coeffs[ 0 ] = q;
	coeffs[ 1 ] = p;
	coeffs[ 2 ] = 0;
	coeffs[ 3 ] = 1;

	num = SolveCubic(coeffs, s);

	s[ num++ ] = 0;
	}
	else
	{
	/* solve the resolvent cubic ... */

	coeffs[ 0 ] = PRTFloat( 0.5/*1.0/2*/ * r * p - 0.125/*1.0/8*/ * q * q );
	coeffs[ 1 ] = - r;
	coeffs[ 2 ] = - PRTFloat(0.5)/*1.0/2*/ * p;
	coeffs[ 3 ] = 1;

	(void) SolveCubic(coeffs, s);

	/* ... and take the one real solution ... */

	z = s[ 0 ];

	/* ... to build two quadric equations */

	u = z * z - r;
	v = 2 * z - p;

	if (IsZero(u))
		u = 0;
	else if (u > 0)
		u = PRTSqrt(u);
	else
		return 0;

	if (IsZero(v))
		v = 0;
	else if (v > 0)
		v = PRTSqrt(v);
	else
		return 0;

	coeffs[ 0 ] = z - u;
	coeffs[ 1 ] = q < 0 ? -v : v;
	coeffs[ 2 ] = 1;

	num = SolveQuadric(coeffs, s);

	coeffs[ 0 ]= z + u;
	coeffs[ 1 ] = q < 0 ? v : -v;
	coeffs[ 2 ] = 1;

	num += SolveQuadric(coeffs, s + num);
	}

	/* resubstitute */

	sub = PRTFloat(0.25)/*1.0/4*/ * A;

	for (i = 0; i < num; ++i)
	s[ i ] -= sub;

	return num;
}
