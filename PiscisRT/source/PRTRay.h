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

#ifndef __PRTRAY_H__
#define __PRTRAY_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTDinamicList.h"

class PISCISRT_API PRTObject;

//--------------------------------------------------------------------------------------
//! The PRTIntersectPoint class.
/*!	This class represents an intersection point.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTIntersectPoint
{

public:
	
	bool collision;				//!< There is a collision?.
	PRTVector point;			//!< Point of collision.
	PRTFloat distance;			//!< Distance from the origin of the ray to the point of collision.
	PRTObject* object;			//!< Object in collision.

//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTIntersectPoint.
	/*!	
	*/
	PRTIntersectPoint(void){collision=false;distance=PRTINFINITE;/**TODO*?*/}

//--------------------------------------------------------------------------------------

};

//--------------------------------------------------------------------------------------
//! The PRTRay class.
/*!	This class represents a ray.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTRay
{

public:

	PRTVector orig;						//!< Origin of the ray.
	PRTVector dir;						//!< Direction of the ray.
	//unsigned long int numrayintertest;	//!< Number of solved intersections.

//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTRay.
	/*!	
	*/
	PRTRay(){orig=PRTVector(0,0,0);dir=PRTVector(0,0,-1);/*numrayintertest=0;*/}

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTRay.
	/*!	
		\param o origin o the ray
		\param d direction of the ray
	*/
	PRTRay(PRTVector o,PRTVector d){orig=o;dir=d;/*numrayintertest=0;*/}

//--------------------------------------------------------------------------------------

	//!	The function to solve if there is any intersection with the ray.
	/*!	
		\param o object to compute on intersections
		\param tc test cull actived?
		\param trans transformations actived or not?
	*/
	PRTIntersectPoint Intersect(PRTObject *o,bool tc,bool trans=false) const;
	
//--------------------------------------------------------------------------------------

};

#endif
