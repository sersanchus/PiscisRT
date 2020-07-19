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

#ifndef __PRTTRIANGLE_H__
#define __PRTTRIANGLE_H__

#include "PRTUtil.h"
#include "PRTObject.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"

//--------------------------------------------------------------------------------------
//! The PRTTriangle class.
/*!	This class represents a closed Triangle surface in 3d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTTriangle : public PRTObject
{

public:
//private:

	PRTVector p1;			//!< The first point on the triangle.
	PRTVector p2;			//!< The second point on the triangle.
	PRTVector p3;			//!< The third point on the triangle.
	PRTVector n1;			//!< The normal of the first point on the triangle.
	PRTVector n2;			//!< The normal of the second point on the triangle.
	PRTVector n3;			//!< The normal of the third point on the triangle.
	PRTVector c1;			//!< The color of the first point on the triangle.
	PRTVector c2;			//!< The color of the second point on the triangle.
	PRTVector c3;			//!< The color of the third point on the triangle.
	PRTTexCoord t1;			//!< The texture coordinates of the first point on the triangle.
	PRTTexCoord t2;			//!< The texture coordinates of the second point on the triangle.
	PRTTexCoord t3;			//!< The texture coordinates of the third point on the triangle.
	PRTFloat areaper2;		//!< For fast calculations, area*2.
	PRTFloat t2ulesst1u;	//!< For fast calculations, t2.u-t1.u .
	PRTFloat t2vlesst1v;	//!< For fast calculations, t2.v-t1.v .
	PRTFloat t3ulesst1u;	//!< For fast calculations, t3.u-t1.u .
	PRTFloat t3vlesst1v;	//!< For fast calculations, t3.v-t1.v .
	PRTVector n2lessn1;		//!< For fast calculations, n2-n1 .
	PRTVector n3lessn1;		//!< For fast calculations, n3-n1 .
	PRTVector c2lessc1;		//!< For fast calculations, c2-c1 .
	PRTVector c3lessc1;		//!< For fast calculations, c3-c1 .
	PRTVector facenormal;	//!< The face normal.

//public:
	
//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTTriangle.
	/*!	
		\param p1
		\param p2
		\param p3
		\param n1
		\param n2
		\param n3
		\param material of the PRTTriangle
	*/
	PRTTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTMaterial *material);

//--------------------------------------------------------------------------------------

	//!	Another constructor of the PRTTriangle.
	/*!	
		\param p1
		\param p2
		\param p3
		\param n1
		\param n2
		\param n3
		\param c1
		\param c2
		\param c3
		\param material of the PRTTriangle
	*/
	PRTTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTVector c1,PRTVector c2,PRTVector c3,PRTMaterial *material);

//--------------------------------------------------------------------------------------

	//!	Another constructor of the PRTTriangle.
	/*!	
		\param p1
		\param p2
		\param p3
		\param n1
		\param n2
		\param n3
		\param t1
		\param t2
		\param t3
		\param material of the PRTTriangle
	*/
	PRTTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTTexCoord t1,PRTTexCoord t2,PRTTexCoord t3,PRTMaterial *material);
	
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
		\return PRTVector the normal on the PRTTriangle
	*/
	PRTVector ComputeNormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTTriangle
	*/
	PRTVector ComputeTangent(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTTriangle
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
	
	//!	Computes the intersection of the PRTTriangle with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	bool ComputeIntersection(const PRTRay& ray,bool doublesided, PRTIntersectPoint& result);

//--------------------------------------------------------------------------------------

};

#endif
