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


/***************************** RANDOMC.H *********************** 2001-10-24 AF *
*
* This file contains class declarations for the C++ library of uniform
* random number generators.
*
* Overview of classes:
* ====================
*
* class TRanrotBGenerator:
* Random number generator of type RANROT-B.
* Source file ranrotb.cpp
*
* class TRanrotWGenerator:
* Random number generator of type RANROT-W.
* Source file ranrotw.cpp
*
* class TRandomMotherOfAll:
* Random number generator of type Mother-of-All (Multiply with carry).
* Source file mother.cpp
*
* class TRandomMersenne:
* Random number generator of type Mersenne twister.
* Source file mersenne.cpp
*
* class TRandomMotRot:
* Combination of Mother-of-All and RANROT-W generators.
* Source file ranmoro.cpp and motrot.asm.
* Coded in assembly language for improved speed.
* Must link in RANDOMAO.LIB or RANDOMAC.LIB.
*
*
* Member functions (methods):
* ===========================
*
* All these classes have identical member functions:
*
* Constructor(long int seed):
* The seed can be any integer. Usually the time is used as seed.
* Executing a program twice with the same seed will give the same sequence of
* random numbers. A different seed will give a different sequence.
*
* double Random();
* Gives a floating point random number in the interval 0 <= x < 1.
* The resolution is 32 bits in TRanrotBGenerator, TRandomMotherOfAll and
* TRandomMersenne. 52 or 63 bits in TRanrotWGenerator. 63 bits in 
* TRandomMotRot.
*
* int IRandom(int min, int max);
* Gives an integer random number in the interval min <= x <= max. (max-min < MAXINT).
* The resolution is the same as for Random(). 
*
* unsigned long BRandom();
* Gives 32 random bits. 
* Only available in the classes TRanrotWGenerator and TRandomMersenne.
*
*
* Example:
* ========
* The file EX-RAN.CPP contains an example of how to generate random numbers.
*
*
* Further documentation:
* ======================
* The file randomc.htm contains further documentation on these random number
* generators.
*
* © 2002 Agner Fog. GNU General Public License www.gnu.org/copyleft/gpl.html
*******************************************************************************/

#ifndef __PRTRANDOM_H__
#define __PRTRANDOM_H__

#include "PRTUtil.h"

#include <math.h>
#include <assert.h>

//--------------------------------------------------------------------------------------
//! The PRTRandom generic class.
/*!	This class represents a generic random number generator.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTRandom
{

public:

//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTRandom.
	/*!	
	*/
	PRTRandom(){}

//--------------------------------------------------------------------------------------

};

class PISCISRT_API PRTRanrotBGenerator : public PRTRandom {              // encapsulate random number generator
  enum constants {                     // define parameters
    KK = 17, JJ = 10, R1 = 13, R2 =  9};
  public:
  void RandomInit(long int seed);      // initialization
  int IRandom(int min, int max);       // get integer random number in desired interval
  double Random();                     // get floating point random number
  PRTRanrotBGenerator(long int seed);    // constructor
  protected:
  int p1, p2;                          // indexes into buffer
  unsigned long randbuffer[KK];        // history buffer
  unsigned long randbufcopy[KK*2];     // used for self-test
  enum TArch {LITTLE_ENDIAN2, BIG_ENDIAN2, NON_IEEE2};
  TArch Architecture;                  // conversion to float depends on computer architecture
};


class PISCISRT_API PRTRanrotWGenerator : public PRTRandom {              // encapsulate random number generator
  enum constants {                     // define parameters
    KK = 17, JJ = 10, R1 = 19, R2 =  27};
  public:
  void RandomInit(long int seed);      // initialization
  int IRandom(int min, int max);       // get integer random number in desired interval
  long double Random();                // get floating point random number
  unsigned long BRandom();             // output random bits  
  PRTRanrotWGenerator(long int seed);    // constructor
  protected:
  int p1, p2;                          // indexes into buffer
  union {                              // used for conversion to float
    long double randp1;
    unsigned long randbits[3];};  
  unsigned long randbuffer[KK][2];     // history buffer
  unsigned long randbufcopy[KK*2][2];  // used for self-test
  enum TArch {LITTLE_ENDIAN2, BIG_ENDIAN2, NON_IEEE2, EXTENDED_PRECISION_LITTLE_ENDIAN2};
  TArch Architecture;                  // conversion to float depends on computer architecture
};

class PISCISRT_API PRTRandomMotherOfAll : public PRTRandom {             // encapsulate random number generator
  public:
  void RandomInit(long int seed);      // initialization
  int IRandom(int min, int max);       // get integer random number in desired interval
  double Random();                     // get floating point random number
  PRTRandomMotherOfAll(long int seed);   // constructor
  protected:
  double x[5];                         // history buffer
  };

class PISCISRT_API PRTRandomMersenne : public PRTRandom {                // encapsulate random number generator
  #if 1
    // define constants for MT11213A:
    // (long constants cannot be defined as enum in 16-bit compilers)
    #define MERS_N   351
    #define MERS_M   175
    #define MERS_R   19
    #define MERS_U   11
    #define MERS_S   7
    #define MERS_T   15
    #define MERS_L   17
    #define MERS_A   0xE4BD75F5
    #define MERS_B   0x655E5280
    #define MERS_C   0xFFD58000
  #else    
    // or constants for MT19937:
    #define MERS_N   624
    #define MERS_M   397
    #define MERS_R   31
    #define MERS_U   11
    #define MERS_S   7
    #define MERS_T   15
    #define MERS_L   18
    #define MERS_A   0x9908B0DF
    #define MERS_B   0x9D2C5680
    #define MERS_C   0xEFC60000
  #endif
  public:
  PRTRandomMersenne(long int seed) {     // constructor
    RandomInit(seed);}
  void RandomInit(long int seed);      // re-seed
  long IRandom(long min, long max);    // output random integer
  double Random();                     // output random float
  unsigned long BRandom();             // output random bits
  private:
  unsigned long mt[MERS_N];            // state vector
  int mti;                             // index into mt
  enum TArch {LITTLE_ENDIAN2, BIG_ENDIAN2, NON_IEEE2};
  TArch Architecture;                  // conversion to float depends on computer architecture
  };    

// RANROT type W generator                          
                                             
void		PISCISRT_API PRTWRandomInit (int seed);
double		PISCISRT_API PRTWRandom  (void);
int			PISCISRT_API PRTWIRandom (int min, int max);
unsigned	PISCISRT_API PRTWBRandom (void);

// Mother-of-all generator

void		PISCISRT_API PRTMRandomInit (int seed);
double		PISCISRT_API PRTMRandom (void);
int			PISCISRT_API PRTMIRandom (int min, int max);
unsigned	PISCISRT_API PRTMBRandom (void);

// Mersenne Twister

void		PISCISRT_API PRTTRandomInit (int seed);
double		PISCISRT_API PRTTRandom (void);
int			PISCISRT_API PRTTIRandom (int min, int max);
unsigned	PISCISRT_API PRTTBRandom ();

// Windows Random

void		PISCISRT_API PRTRandomInit(int seed);
double		PISCISRT_API PRTRandom(void);

#endif 

