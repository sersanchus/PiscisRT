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

#ifndef __PRTPOLYGON_H__
#define __PRTPOLYGON_H__

#include "PRTUtil.h"
#include "PRTTriangle.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"

//--------------------------------------------------------------------------------------
//! The PRTPolygon class.
/*!	This class represents a polygon of undefined number of vertexes.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPolygon : public PRTObject
{

private:

	PRTTriangle *what;		//!< To know what PRTTriangle is rendering.
	PRTDinamicList tris;	//!< The coded PRTTriangles.
		
public:
	
//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTPolygon for an import plugin.
	/*!	
		\param t the frist triangle
	*/
	PRTPolygon(PRTTriangle *t);

//--------------------------------------------------------------------------------------

	//!	A function to add another vertex.
	/*!	
		\param p point
		\param n normal
		\param t texture coordinate
	*/
	void AddVertex(PRTVector p,PRTVector n,PRTTexCoord t);

//--------------------------------------------------------------------------------------

	//!	A destructor of the PRTPolygon.
	/*!	
	*/
	~PRTPolygon(void);

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
		\return PRTVector the normal on the PRTPolygon
	*/
	PRTVector ComputeNormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTPolygon
	*/
	PRTVector ComputeTangent(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTPolygon
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
	
	//!	Computes the intersection of the PRTPolygon with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	PRTIntersectPoint ComputeIntersection(PRTRay ray,bool doublesided);

//--------------------------------------------------------------------------------------

};

#endif
