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

#ifndef __PRTTRIANGLELIGHT_H__
#define __PRTTRIANGLELIGHT_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTLight.h"
#include "PRTTriangle.h"
#include "PRTRay.h"

//--------------------------------------------------------------------------------------
//! The PRTTriangleLight class.
/*!	This class represents a triangle light.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTTriangleLight : public PRTLight
{

public:

	PRTVector p1;  			//!< The first point of the triangle.
	PRTVector p2;			//!< The second point of the triangle.
	PRTVector p3;			//!< The third point of the triangle.
	PRTVector n1;			//!< The normal or the first point of the triangle.
	PRTVector n2;			//!< The normal or the second point of the triangle.
	PRTVector n3;			//!< The normal or the third point of the triangle.
	PRTFloat area2;			//!< The area of triangle multiplied by 2.

//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTTriangleLight.
	/*!	
		\param p1 p1 point
		\param p2 p2 point
		\param p3 p3 point
		\param n1 normal for p1
		\param n2 normal for p2
		\param n3 normal for p3
		\param color color
	*/
	PRTTriangleLight(const PRTVector &p1,const PRTVector &p2,const PRTVector &p3,const PRTVector &n1,const PRTVector &n2,const PRTVector &n3,const PRTVector &color);

//--------------------------------------------------------------------------------------

	//!	Computes what point light represents the PRTTriangleLight at a given position.
	/*!	
		\param point point on 3d
		\return the position of the point light
	*/
	PRTVector ComputeWhatPointLight(PRTVector &point);
	
//--------------------------------------------------------------------------------------

	//!	Computes the light ray. What light intensity is seen by the ray.
	/*!	
		\param ray the ray
		\param ipoint intersected point
		\param normal normal at that point
		\param object object to discard
		\param main pointer to the global PRTMain
		\return the intensity
	*/	
	PRTVector ComputeLightRay(PRTRay &ray,PRTIntersectPoint &ipoint,PRTVector &normal, const PRTObject *object, PRTMain *main);

//--------------------------------------------------------------------------------------

};

#endif
