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

#ifndef __PRTREFOBJECT_H__
#define __PRTREFOBJECT_H__

#include "PRTUtil.h"
#include "PRTObject.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"

//--------------------------------------------------------------------------------------
//! The PRTRefObject class.
/*!	This class represents a referenced object.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTRefObject : public PRTObject
{

private:

	PRTObject* reference;		//!< The referenced object.
	
public:

//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTRefObject.
	/*!	
		\param object
	*/
	PRTRefObject(PRTObject* object){reference=object;}

//--------------------------------------------------------------------------------------
	
	//!	Computes a texture coordinates for a point on 3d.
	/*!	
		\param point on 3d
		\return the texture coordinates
	*/
	PRTTexCoord ComputeTexCoord(PRTVector point){return reference->ComputeTexCoord(point);}

//--------------------------------------------------------------------------------------
	
	//!	Computes a point in 3d for a texture coordinates.
	/*!	
		\param texcoord texture coordinates
		\return PRTVector the point in 3d
	*/
	PRTVector ComputePoint(PRTTexCoord texcoord){return ComputePoint(texcoord);}
	
//--------------------------------------------------------------------------------------

	
	//!	Computes a normal for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the normal on the PRTRefObject
	*/
	PRTVector ComputeNormal(PRTVector point){return ComputeNormal(point);}

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTRefObject
	*/
	PRTVector ComputeTangent(PRTVector point){return ComputeTangent(point);}

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTRefObject
	*/
	PRTVector ComputeBinormal(PRTVector point){return ComputeBinormal(point);}

//--------------------------------------------------------------------------------------
	
	//!	Computes a color for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the color for the point
	*/
	PRTVector ComputeColor(PRTVector point){return ComputeColor(point);}

//--------------------------------------------------------------------------------------
	
	//!	Computes the intersection of the PRTRefObject with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	PRTIntersectPoint ComputeIntersection(PRTRay ray,bool doublesided){return ComputeIntersection(ray,doublesided);}

//--------------------------------------------------------------------------------------

};

#endif
