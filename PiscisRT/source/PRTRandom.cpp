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

#include "PRTRandom.h"
#include <stdlib.h>
#include <string.h>

void PRTRandomInit(int seed)
{
	srand(seed);
}

double PRTRandom(void)
{
	return ((double)rand()/(double)RAND_MAX);
}

/************************** MERSENNE.CPP ******************** AgF 2001-10-18 *
*  Random Number generator 'Mersenne Twister'                                *
*                                                                            *
*  This random number generator is described in the article by               *
*  M. Matsumoto & T. Nishimura, in:                                          *
*  ACM Transactions on Modeling and Computer Simulation,                     *
*  vol. 8, no. 1, 1998, pp. 3-30.                                            *
*                                                                            *
*  Experts consider this an excellent random number generator.               *
*                                                                            *
* © 2003 A. Fog. GNU General Public License www.gnu.org/copyleft/gpl.html    *
*****************************************************************************/


void PRTRandomMersenne::RandomInit(long int seed) {
  // re-seed generator
  unsigned long s = (unsigned long)seed;
  for (mti = 0; mti < MERS_N; mti++) {
    s = s * 29943829 - 1;
    mt[mti] = s;}
  // detect computer architecture
  union {double f; unsigned long i[2];} convert;
  convert.f = 1.0;
  if (convert.i[1] == 0x3FF00000) Architecture = LITTLE_ENDIAN2;
  else if (convert.i[0] == 0x3FF00000) Architecture = BIG_ENDIAN2;
  else Architecture = NON_IEEE2;}


unsigned long PRTRandomMersenne::BRandom() {
  // generate 32 random bits
  unsigned long y;

  if (mti >= MERS_N) {
    // generate MERS_N words at one time
    const unsigned long LOWER_MASK = (1LU << MERS_R) - 1; // lower MERS_R bits
    const unsigned long UPPER_MASK = -1L  << MERS_R;      // upper (32 - MERS_R) bits
    int kk, km;
    for (kk=0, km=MERS_M; kk < MERS_N-1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
      mt[kk] = mt[km] ^ (y >> 1) ^ (-(signed long)(y & 1) & MERS_A);
      if (++km >= MERS_N) km = 0;}

    y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ (-(signed long)(y & 1) & MERS_A);
    mti = 0;}

  y = mt[mti++];

  // Tempering (May be omitted):
  y ^=  y >> MERS_U;
  y ^= (y << MERS_S) & MERS_B;
  y ^= (y << MERS_T) & MERS_C;
  y ^=  y >> MERS_L;

  return y;}

  
double PRTRandomMersenne::Random() {
  // output random float number in the interval 0 <= x < 1
  union {double f; unsigned long i[2];} convert;
  unsigned long r = BRandom(); // get 32 random bits
  // The fastest way to convert random bits to floating point is as follows:
  // Set the binary exponent of a floating point number to 1+bias and set
  // the mantissa to random bits. This will give a random number in the 
  // interval [1,2). Then subtract 1.0 to get a random number in the interval
  // [0,1). This procedure requires that we know how floating point numbers
  // are stored. The storing method is tested in function RandomInit and saved 
  // in the variable Architecture. (A PC running Windows or Linux uses 
  // LITTLE_ENDIAN architecture).
  switch (Architecture) {
  case LITTLE_ENDIAN2:
    convert.i[0] =  r << 20;
    convert.i[1] = (r >> 12) | 0x3FF00000;
    return convert.f - 1.0;
  case BIG_ENDIAN2:
    convert.i[1] =  r << 20;
    convert.i[0] = (r >> 12) | 0x3FF00000;
    return convert.f - 1.0;
  case NON_IEEE2: default:
  ;} 
  // This somewhat slower method works for all architectures, including 
  // non-IEEE floating point representation:
  return (double)r * (1./((double)(unsigned long)(-1L)+1.));}

  
long PRTRandomMersenne::IRandom(long min, long max) {
  // output random integer in the interval min <= x <= max
  long r;
  r = long((max - min + 1) * Random()) + min; // multiply interval with random and truncate
  if (r > max) r = max;
  if (max < min) return 0x80000000;
  return r;}


/************************** MOTHER.CPP ****************** AgF 1999-03-03 *
*  'Mother-of-All' random number generator                               *
*                                                                        *
*  This is a multiply-with-carry type of random number generator         *
*  invented by George Marsaglia.  The algorithm is:                      *
*  S = 2111111111*X[n-4] + 1492*X[n-3] + 1776*X[n-2] + 5115*X[n-1] + C   *
*  X[n] = S modulo 2^32                                                  *
*  C = floor(S / 2^32)                                                   *
*                                                                        *
*  IMPORTANT:
*  This implementation uses a long double for C. Note that not all       *
*  computers and compilers support the long double (80-bit) floating     *
*  point format. It is recommended to use a Borland or Gnu compiler on   *
*  a PC. The Microsoft compiler doesn't support the long double format.  *
*  You will get an error message if your system doesn't support this.    *
*                                                                        *
* © 2002 A. Fog. GNU General Public License www.gnu.org/copyleft/gpl.html*
*************************************************************************/


// constructor:
PRTRandomMotherOfAll::PRTRandomMotherOfAll(long int seed) {
  // Check that compiler supports 80-bit long double:
  //assert(sizeof(long double)>9);//*TODO* provoca assertion failed
  // initialize
  RandomInit(seed);}


// returns a random number between 0 and 1:
double PRTRandomMotherOfAll::Random() {
  long double c;
  c = (long double)2111111111.0 * x[3] +
      1492.0 * (x[3] = x[2]) +
      1776.0 * (x[2] = x[1]) +
      5115.0 * (x[1] = x[0]) +
      x[4];
  x[4] = floor(c);
  x[0] = c - x[4];
  x[4] = x[4] * (1./(65536.*65536.));
  return x[0];}


// returns integer random number in desired interval:
int PRTRandomMotherOfAll::IRandom(int min, int max) {
  int iinterval = max - min + 1;
  if (iinterval <= 0) return 0x80000000; // error
  int i = long(iinterval * Random()); // truncate
  if (i >= iinterval) i = iinterval-1;
  return min + i;}


// this function initializes the random number generator:
void PRTRandomMotherOfAll::RandomInit (long int seed) {
  int i;
  unsigned long s = seed;
  // make random numbers and put them into the buffer
  for (i=0; i<5; i++) {
    s = s * 29943829 - 1;
    x[i] = s * (1./(65536.*65536.));}
  // randomize some more
  for (i=0; i<19; i++) Random();}


/************************* RANROTB.CPP ****************** AgF 1999-03-03 *
*  Random Number generator 'RANROT' type B                               *
*                                                                        *
*  This is a lagged-Fibonacci type of random number generator with       *
*  rotation of bits.  The algorithm is:                                  *
*  X[n] = ((X[n-j] rotl r1) + (X[n-k] rotl r2)) modulo 2^b               *
*                                                                        *
*  The last k values of X are stored in a circular buffer named          *
*  randbuffer.                                                           *
*  The code includes a self-test facility which will detect any          *
*  repetition of previous states.                                        *
*                                                                        *
*  The theory of the RANROT type of generators and the reason for the    *
*  self-test are described at www.agner.org/random                       *
*                                                                        *
* © 2002 A. Fog. GNU General Public License www.gnu.org/copyleft/gpl.html*
*************************************************************************/


// constructor:
PRTRanrotBGenerator::PRTRanrotBGenerator(long int seed) {
  RandomInit(seed);
  // detect computer architecture
  union {double f; unsigned long i[2];} convert;
  convert.f = 1.0;
  if (convert.i[1] == 0x3FF00000) Architecture = LITTLE_ENDIAN2;
  else if (convert.i[0] == 0x3FF00000) Architecture = BIG_ENDIAN2;
  else Architecture = NON_IEEE2;}

#ifdef __MAKE_LINUX__
#define _lrotl(x, r)	(x << r) | (x >> (sizeof(x)*8-r))
#endif


// returns a random number between 0 and 1:
double PRTRanrotBGenerator::Random() {
  unsigned long x;
  // generate next random number
  x = randbuffer[p1] = _lrotl(randbuffer[p2], R1) + _lrotl(randbuffer[p1], R2);
  // rotate list pointers
  if (--p1 < 0) p1 = KK - 1;
  if (--p2 < 0) p2 = KK - 1;
  // perform self-test
  if (randbuffer[p1] == randbufcopy[0] &&
    memcmp(randbuffer, randbufcopy+KK-p1, KK*sizeof(long)) == 0) {
      // self-test failed
      if ((p2 + KK - p1) % KK != JJ) {
        // note: the way of printing error messages depends on system
        // In Windows you may use FatalAppExit
        printf("Random number generator not initialized");}
      else {
        printf("Random number generator returned to initial state");}
      exit(1);}
  // conversion to float:
  union {double f; unsigned long i[2];} convert;
  switch (Architecture) {
  case LITTLE_ENDIAN2:
    convert.i[0] =  x << 20;
    convert.i[1] = (x >> 12) | 0x3FF00000;
    return convert.f - 1.0;
  case BIG_ENDIAN2:
    convert.i[1] =  x << 20;
    convert.i[0] = (x >> 12) | 0x3FF00000;
    return convert.f - 1.0;
  case NON_IEEE2: default:
  ;} 
  // This somewhat slower method works for all architectures, including 
  // non-IEEE floating point representation:
  return (double)x * (1./((double)(unsigned long)(-1L)+1.));}


// returns integer random number in desired interval:
int PRTRanrotBGenerator::IRandom(int min, int max) {
  int iinterval = max - min + 1;
  if (iinterval <= 0) return 0x80000000; // error
  int i = (int)( (double)iinterval * Random()); // truncate
  if (i >= iinterval) i = iinterval-1;
  return min + i;}
  

void PRTRanrotBGenerator::RandomInit (long int seed) {
  // this function initializes the random number generator.
  int i;
  long int s = seed;

  // make random numbers and put them into the buffer
  for (i=0; i<KK; i++) {
    s = s * 2891336453 + 1;
    randbuffer[i] = (unsigned long) s;}

  // check that the right data formats are used by compiler:
  union {
    double randp1;
    unsigned long randbits[2];};
  randp1 = 1.5;
  assert(randbits[1]==0x3FF80000); // check that IEEE double precision float format used

  // initialize pointers to circular buffer
  p1 = 0;  p2 = JJ;
  // store state for self-test
  memcpy (randbufcopy, randbuffer, KK*sizeof(long));
  memcpy (randbufcopy+KK, randbuffer, KK*sizeof(long));
  // randomize some more
  for (i=0; i<9; i++) Random();
}


/************************* RANROTW.CPP ****************** AgF 1999-03-03 *
*  Random Number generator 'RANROT' type W                               *
*  This version is used when a resolution higher that 32 bits is desired.*
*                                                                        *
*  This is a lagged-Fibonacci type of random number generator with       *
*  rotation of bits.  The algorithm is:                                  *
*  Z[n] = (Y[n-j] + (Y[n-k] rotl r1)) modulo 2^(b/2)                     *
*  Y[n] = (Z[n-j] + (Z[n-k] rotl r2)) modulo 2^(b/2)                     *
*  X[n] = Y[n] + Z[n]*2^(b/2)                                            *
*                                                                        *
*  The last k values of Y and Z are stored in a circular buffer named    *
*  randbuffer.                                                           *
*  The code includes a self-test facility which will detect any          *
*  repetition of previous states.                                        *
*  The function uses a fast method for conversion to floating point.     *
*  This method relies on floating point numbers being stored in the      *
*  standard 64-bit IEEE format or the 80-bit long double format.         *
*                                                                        *
*  The theory of the RANROT type of generators and the reason for the    *
*  self-test are described at www.agner.org/random/theory                *
*                                                                        *
* ©2002 A. Fog. GNU General Public License www.gnu.org/copyleft/gpl.html *
*************************************************************************/

// constructor:
PRTRanrotWGenerator::PRTRanrotWGenerator(long int seed) {
  RandomInit(seed);
  // detect computer architecture
  randbits[2] = 0; randp1 = 1.0;
  if (randbits[2] == 0x3FFF) Architecture = EXTENDED_PRECISION_LITTLE_ENDIAN2;
  else if (randbits[1] == 0x3FF00000) Architecture = LITTLE_ENDIAN2;
  else if (randbits[0] == 0x3FF00000) Architecture = BIG_ENDIAN2;
  else Architecture = NON_IEEE2;}


unsigned long PRTRanrotWGenerator::BRandom() {
  // generate next random number
  unsigned long y, z;
  // generate next number
  z = _lrotl(randbuffer[p1][0], R1) + randbuffer[p2][0];
  y = _lrotl(randbuffer[p1][1], R2) + randbuffer[p2][1];
  randbuffer[p1][0] = y; randbuffer[p1][1] = z;
  // rotate list pointers
  if (--p1 < 0) p1 = KK - 1;
  if (--p2 < 0) p2 = KK - 1;
  // perform self-test
  if (randbuffer[p1][0] == randbufcopy[0][0] &&
    memcmp(randbuffer, randbufcopy[KK-p1], 2*KK*sizeof(long)) == 0) {
      // self-test failed
      if ((p2 + KK - p1) % KK != JJ) {
        // note: the way of printing error messages depends on system
        // In Windows you may use FatalAppExit
        printf("Random number generator not initialized");}
      else {
        printf("Random number generator returned to initial state");}
      exit(1);}
  randbits[0] = y;
  randbits[1] = z;
  return y;}


long double PRTRanrotWGenerator::Random() {
  // returns a random number between 0 and 1.
  unsigned long z = BRandom();  // generate 64 random bits
  switch (Architecture) {
  case EXTENDED_PRECISION_LITTLE_ENDIAN2:
    // 80 bits floats = 63 bits resolution
    randbits[1] = z | 0x80000000;  break;
  case LITTLE_ENDIAN2:
    // 64 bits floats = 52 bits resolution
    randbits[1] = (z & 0x000FFFFF) | 0x3FF00000;  break;
  case BIG_ENDIAN2:
    // 64 bits floats = 52 bits resolution
    randbits[0] = (randbits[0] & 0x000FFFFF) | 0x3FF00000;  break;
  case NON_IEEE2: default:
    // not a recognized floating point format. 32 bits resolution
    return (double)z * (1./((double)(unsigned long)(-1L)+1.));}
  return randp1 - 1.0;}


int PRTRanrotWGenerator::IRandom(int min, int max) {
  // get integer random number in desired interval
  int iinterval = max - min + 1;
  if (iinterval <= 0) return 0x80000000;  // error
  int i = int(iinterval * Random());      // truncate
  if (i >= iinterval) i = iinterval-1;
  return min + i;}


void PRTRanrotWGenerator::RandomInit (long int seed) {
  // this function initializes the random number generator.
  int i, j;

  // make random numbers and put them into the buffer
  for (i=0; i<KK; i++) {
    for (j=0; j<2; j++) {
      seed = seed * 2891336453UL + 1;
      randbuffer[i][j] = seed;}}
  // set exponent of randp1
  randbits[2] = 0; randp1 = 1.0;
  // initialize pointers to circular buffer
  p1 = 0;  p2 = JJ;
  // store state for self-test
  memcpy (randbufcopy, randbuffer, 2*KK*sizeof(long));
  memcpy (randbufcopy[KK], randbuffer, 2*KK*sizeof(long));
  // randomize some more
  for (i=0; i<31; i++) BRandom();
}

PRTRanrotWGenerator trwg(0);

void		PRTWRandomInit (int seed){trwg.RandomInit(seed);};
double		PRTWRandom  (void){return trwg.Random();};
int			PRTWIRandom (int min, int max){return trwg.IRandom(min,max);};
unsigned	PRTWBRandom (void){return trwg.BRandom();};

PRTRandomMotherOfAll trmoa(0);

void		PRTMRandomInit (int seed){trmoa.RandomInit(seed);};
double		PRTMRandom (void){return trmoa.Random();};
int			PRTMIRandom (int min, int max){return trmoa.IRandom(min,max);};
unsigned	PRTMBRandom (void){return (unsigned long)trmoa.Random();}; //*TODO* no hay BRandom

PRTRandomMersenne trm(0);

void		PRTTRandomInit (int seed){trm.RandomInit(seed);};
double		PRTTRandom (void){return trm.Random();};
int			PRTTIRandom (int min, int max){return trm.IRandom(min,max);};
unsigned	PRTTBRandom (){return trm.BRandom();};
