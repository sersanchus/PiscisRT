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

#ifndef __PRTLIGHT_H__
#define __PRTLIGHT_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTRay.h"

#define PRT_POINTLIGHT			2001
#define PRT_TRIANGLELIGHT		2002
#define PRT_SPHERELIGHT			2003
#define PRT_DIRECTIONALLIGHT	2004

class PRTMain;

//--------------------------------------------------------------------------------------
//! The PRTLight class.
/*!	This class represents an abstract light in the PiscisRT engine.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTLight
{

public:

	int type;						//!< The type of the PRTLight.
	PRTVector color;				//!< Color of the light. (Diffuse color).
	PRTFloat attenuation0;			//!< Attenuation factor 0.
	PRTFloat attenuation1;			//!< Attenuation factor 1.
	PRTFloat attenuation2;			//!< Attenuation factor 2.
	PRTFloat intensity;				//!< Intensity/power of the light.
	
//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTLight.
	/*!	
		\param type
		\param color
	*/
	PRTLight(const int type,const PRTVector &color);

//--------------------------------------------------------------------------------------

	//!	Empty constructor for PRTLight.
	/*!	
	*/
	PRTLight(){};

//--------------------------------------------------------------------------------------

	//!	Computes what point light represents the PRTLight at a given position.
	/*!	
		\param point point on 3d
		\return the position of the point light
	*/
	virtual PRTVector ComputeWhatPointLight(PRTVector &point)=0;

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
	virtual PRTVector ComputeLightRay(PRTRay &ray,PRTIntersectPoint &ipoint, PRTVector &normal, const PRTObject *object, PRTMain* main)=0;

//--------------------------------------------------------------------------------------

};

#endif
