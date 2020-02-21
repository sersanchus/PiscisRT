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

#ifndef __PRTSPHERELIGHT_H__
#define __PRTSPHERELIGHT_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTLight.h"
#include "PRTSphere.h"
#include "PRTRay.h"

//--------------------------------------------------------------------------------------
//! The PRTSphereLight class.
/*!	This class represents a sphere light.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTSphereLight : public PRTLight
{

public:

	PRTVector p;  		//!< The center of the light.
	PRTFloat r; 		//!< The radius of the light.

//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTSphereLight.
	/*!	
		\param center position of the light's center
		\param radius radius of the sphere light
		\param color color
	*/
	PRTSphereLight(const PRTVector &center,const PRTFloat radius,const PRTVector &color);

//--------------------------------------------------------------------------------------

	//!	Computes what point light represents the PRTSphereLight at a given position.
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
