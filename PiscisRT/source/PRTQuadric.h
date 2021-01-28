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

#ifndef __PRTQUADRIC_H__
#define __PRTQUADRIC_H__

#include "PRTUtil.h"
#include "PRTObject.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"

#define PRT_QUADRIC_CYLINDER	3001
#define PRT_QUADRIC_CONE		3002
#define PRT_QUADRIC_ELLIPSOID	3003
#define PRT_QUADRIC_PARABOLOID	3004
#define PRT_QUADRIC_HYPERBOLOID1	3005
#define PRT_QUADRIC_HYPERBOLOID2	3006

//--------------------------------------------------------------------------------------
//! The PRTQuadric class.
/*!	This class represents a quadric surface in 3d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTQuadric : public PRTObject
{

private:

	int quadrictype;				//!< The type of the PRTquadric.
	PRTFloat A,B,C,D,E,F,G,H,I,J;	//!< The matrix parameters of the quadric (as in "An introduction to ray tracing").

public:
	
//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTQuadric.
	/*!	
		\param center
		\param a size in x //*TODO*
		\param b size in y //*TODO*
		\param c size in z //*TODO*
		\param type
		\param material
	*/
	PRTQuadric(PRTVector center,PRTFloat a,PRTFloat b,PRTFloat c,int type,PRTMaterial *material); //*TODO* reps and desps

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
		\return PRTVector the normal on the PRTQuadric
	*/
	PRTVector ComputeNormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTQuadric
	*/
	PRTVector ComputeTangent(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTQuadric
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
	
	//!	Computes the intersection of the PRTQuadric with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	bool ComputeIntersection(const PRTRay& ray,bool doublesided, PRTIntersectPoint& result);

//--------------------------------------------------------------------------------------

};

#endif
