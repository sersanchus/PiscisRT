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

#ifndef __PRTQUATJULIA_H__
#define __PRTQUATJULIA_H__

#include "PRTUtil.h"
#include "PRTObject.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"
#include "PRTQuaternion.h"

//--------------------------------------------------------------------------------------
//! The PRTQuatJulia class.
/*!	This class represents a closed quaternion Julia surface in 3d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTQuatJulia : public PRTObject
{

private:

	PRTVector center;			//!< The center of the quaternion Julia in 3d.
	PRTVector normal;			//!< Normal of the slice plane.
	//PRTFloat radius;			//!< The radius of the quaternion Julia.
	PRTQuaternion c;			//!< The c parameter of the quaternion Julia, q'=f(q)+c.
	int iterations;				//!< The number of iterations to see if a point is on the fractal.

	PRTFloat *Sx;				//!< To store de interation sequence.
	PRTFloat *Sy;				//!< To store de interation sequence.
	PRTFloat *Sz;				//!< To store de interation sequence.
	PRTFloat *Sw;				//!< To store de interation sequence.

	PRTFloat R;					//!< The limit to solve if the fractal diverge.
	int count;	

public:

//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTQuatJulia.
	/*!	
		\param center
		\param c
		\param iterations
		\param material of the PRTQuatJulia
	*/
	PRTQuatJulia(PRTVector center/*,PRTFloat radius*/,PRTQuaternion c,int iterations, PRTMaterial *material);

//--------------------------------------------------------------------------------------

	//!	The destructor.
	/*!			
	*/
	~PRTQuatJulia(void);

//--------------------------------------------------------------------------------------

	//!	The iteration function of a quaternion.
	/*!	
		\param p
		\return the PRTQuaternion resulting of the iteration.
	*/
	PRTQuaternion P(PRTQuaternion p);

//--------------------------------------------------------------------------------------

	//!	Function that answer if a point is in the fractal.
	/*!	
		\param p
		\return length of vector to see if its infinite, >4.
	*/
	PRTFloat PointInFractal(PRTVector4 p);

//--------------------------------------------------------------------------------------

	//!	Function that returns the lower bound of the distance from a point to the fractal.
	/*!	
		\param p
		\param dist 
		\return if there is a collision.
	*/
	bool d(PRTVector p,PRTFloat* dist);

//--------------------------------------------------------------------------------------
	
	//!	Computes a texture coordinates for a point on 3d.
	/*!	
		\param point on 3d
		\return the texture coordinates
	*/
	PRTTexCoord ComputeTexCoord(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a point in 3d for a texture coordinates.
	/*!	
		\param texcoord texture coordinates
		\return PRTVector the point in 3d
	*/
	PRTVector ComputePoint(PRTTexCoord texcoord);
	
//--------------------------------------------------------------------------------------
	
	//!	Computes a normal for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the normal on the PRTQuatJulia
	*/
	PRTVector ComputeNormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTQuatJulia
	*/
	PRTVector ComputeTangent(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTQuatJulia
	*/
	PRTVector ComputeBinormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a color for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the color for the point
	*/
	PRTVector ComputeColor(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes the intersection of the PRTQuatJulia with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	bool ComputeIntersection(const PRTRay& ray,bool doublesided,PRTIntersectPoint& result);

//--------------------------------------------------------------------------------------

};

#endif
