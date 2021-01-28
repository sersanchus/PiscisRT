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

#ifndef __PRTMESH_H__
#define __PRTMESH_H__

#include "PRTUtil.h"
#include "PRTObject.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTMaterial.h"
#include "PRTMeshImpExp.h"
#include "PRTOcTree.h"

//--------------------------------------------------------------------------------------
//! The PRTMesh class.
/*!	This class represents an object mesh of triangles.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTMesh : public PRTObject
{

private:

	PRTObject *what;		//!< To know what PRTTriangle is rendering.
	PRTOcTree *octrees;		//!< The PRTMesh octree.
	
public:
	
//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTMesh for an import plugin.
	/*!	
		\param data the mesh data
		\param mat the material of the mesh
	*/
	PRTMesh(PRTMeshImpExpObject* data,PRTMaterial* mat);

//--------------------------------------------------------------------------------------

	//!	A destructor of the PRTMesh.
	/*!	
	*/
	~PRTMesh(void);

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
		\return PRTVector the normal on the PRTMesh
	*/
	PRTVector ComputeNormal(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTMesh
	*/
	PRTVector ComputeTangent(PRTVector point);

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTMesh
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
	
	//!	Computes the intersection of the PRTMesh with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	bool ComputeIntersection(const PRTRay& ray,bool doublesided, PRTIntersectPoint& result);

//--------------------------------------------------------------------------------------

};

#endif
