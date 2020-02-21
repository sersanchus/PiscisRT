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

#ifndef __PRTCONVEXHULL_H__
#define __PRTCONVEXHULL_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTRay.h"

#define PRECISION PRTFloat(1)

//--------------------------------------------------------------------------------------
//! The PRTConvexHull class.
/*!	This class represents a closed convex hull surface in 3d. This convex hull is used to envolve an object more complicate than it.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTConvexHull
{

public:
	
	PRTVector chbeg;			//!< The point on the three lesser coordinates of a box that includes the object.
	PRTVector chend;			//!< The point on the three greater coordinates of a box that includes the object.
	
	PRTVector chcen;			//!< The point on the center of a sphere that includes the object.
	PRTFloat chrad;				//!< The radius of a sphere that includes the object.

//--------------------------------------------------------------------------------------

	//!	A function to solve an intersection of a ray with the convex hull.
	/*!	
		\param ray the PRTRay
		\return bool there is an intersection, first convex hull of ray with convex hull of the object, then if true ray with convex hull of the object, in all cases the convex box is used and not the convex sphere
	*/
	bool IntersectWithRay(PRTRay ray);

//--------------------------------------------------------------------------------------

	//!	A function to solve precision problems.
	/*!	
	*/
	void SolvePrecision(void);

//--------------------------------------------------------------------------------------

};

#endif

