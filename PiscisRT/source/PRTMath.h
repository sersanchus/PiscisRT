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

#ifndef __PRTMATH_H__
#define __PRTMATH_H__

#include <math.h>
#include <stdio.h>

//--------------------------------------------------------------------------------------
// defines for PRTFloat (float or double)

#define PRTFloat float
#define PRTSqrt(a) sqrtf(a)
#define PRTFSqrt(a) fsqrt(a)
#define PRTACos(a) acosf(a)
#define PRTASin(a) asinf(a)
#define PRTCos(a) cosf(a)
#define PRTSin(a) sinf(a)
#define PRTLog(a) logf(a)
#define PRTExp(a) expf(a)
#define PRTTan(a) tanf(a)
#define PRTPow(a,b) powf(a,b)
#define PRTAbs(a) fabsf(a)
#define PRTPI PRTFloat(3.1415926535897932384626433832795)
#define PRTINFINITE PRTFloat(99999999999.9)

#include "PRTUtil.h"

//--------------------------------------------------------------------------------------
// fast square root

	/* MOST_SIG_OFFSET gives the (int *) offset from the address of the double
	 * to the part of the number containing the sign and exponent.
	 * You will need to find the relevant offset for your architecture.
	 */

#define MOST_SIG_OFFSET 1

	/* SQRT_TAB_SIZE - the size of the lookup table - must be a power of four.
	 */

#define SQRT_TAB_SIZE 256

	/* MANT_SHIFTS is the number of shifts to move mantissa into position.
	 * If you quadruple the table size subtract two from this constant,
	 * if you quarter the table size then add two.
	 * Valid values are: (16384, 7) (4096, 9) (1024, 11) (256, 13)
	 */

#define MANT_SHIFTS   13

#define EXP_BIAS   1023       /* Exponents are always positive     */
#define EXP_SHIFTS 20         /* Shifs exponent to least sig. bits */
#define EXP_LSB    0x00100000 /* 1 << EXP_SHIFTS                   */
#define MANT_MASK  0x000FFFFF /* Mask to extract mantissa          */

extern int sqrt_tab[SQRT_TAB_SIZE];

void PISCISRT_API PRTInitFastSqrt();
PRTFloat PISCISRT_API fsqrt(PRTFloat);

//--------------------------------------------------------------------------------------
// degrees and radians

PRTFloat PISCISRT_API PRTGradToRad(PRTFloat);
PRTFloat PISCISRT_API PRTRadToGrad(PRTFloat);

//--------------------------------------------------------------------------------------
// intersections

#include "PRTVector.h"

bool PISCISRT_API Segment2dSegment2dInter(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector p4);
PRTVector PISCISRT_API Segment2dSegment2dInterPoint(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector p4);
bool PISCISRT_API SameSide(PRTVector p1,PRTVector p2,PRTVector a,PRTVector b);
char PISCISRT_API HitBoundingBox(PRTVector minB,PRTVector maxB,PRTVector origin,PRTVector dir,PRTVector coord);

//--------------------------------------------------------------------------------------
// general math aplications

PRTFloat PISCISRT_API ArcCos(PRTFloat x);

#ifndef MAX
#	define MAX(a,b)			(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#	define MIN(a,b)			(((a) < (b)) ? (a) : (b))
#endif

#ifndef SQR
#	define SQR(a)			a*a
#endif

#ifndef CUBE
#	define CUBE(x)			x*x*x
#endif

#if defined __MAKE_CYGWIN__ && !defined(_lrotl)
#	define _lrotl(x, r) (x << r) | (x >> (sizeof(x)*8-r))
#endif

//--------------------------------------------------------------------------------------
// quadrics,quartics and qubics

int PISCISRT_API SolveQuadric(PRTFloat c[ 3 ],PRTFloat s[ 2 ]);
int PISCISRT_API SolveCubic(PRTFloat c[4], PRTFloat s[3]);
int PISCISRT_API SolveQuartic(PRTFloat c[5], PRTFloat s[4]);

#endif
