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

#ifndef __PRTOBJECT_H__
#define __PRTOBJECT_H__

#include "PRTUtil.h"

#include "PRTMaterial.h"
#include "PRTDinamicList.h"
#include "PRTRay.h"
#include "PRTMatrix.h"
#include "PRTConvexHull.h"

#define PRT_OBJECT_TRIANGLE		1001
#define PRT_OBJECT_SPHERE		1002
#define PRT_OBJECT_QUADRIC		1003
#define PRT_OBJECT_CYLINDER		1004
#define PRT_OBJECT_CIRCLE		1005
#define PRT_OBJECT_PLANE		1006
#define PRT_OBJECT_QUARTIC		1007
#define PRT_OBJECT_CONE			1008
#define PRT_OBJECT_LINE			1009
#define PRT_OBJECT_CSG			1010
#define PRT_OBJECT_QUATJULIA	1011
#define PRT_OBJECT_MESH			1012
#define PRT_OBJECT_POLYGON		1013

class PISCISRT_API PRTRay;

//--------------------------------------------------------------------------------------
//! The PRTObject class.
/*!	This class represents an abstract 3d object in the PiscisRT engine.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTObject
{

public:
	
	PRTVector position;			//!< Position of the PRTObject, center of it.
	int type;					//!< The type of the PRTObject.
	PRTMaterial *material;		//!< The PRTMaterial assigned to the PRTObject.
	PRTFloat despu,despv;		//!< Displacement of the texture in u and v coordinates.
	PRTFloat repu,repv;			//!< Repetition of the texture in u and v coordinates.
	PRTConvexHull convexhull;	//!< Convex hull of the PRTObject.
	PRTMatrix trans;			//!< The rotate, translate and scale transformations.
	PRTMatrix transrot;			//!< The rotate transformation.
	PRTMatrix invtrans;			//!< Inverse of the rotate, translate and scale transformations.
	PRTMatrix invtransrot;		//!< Inverse of the rotate transformation.

//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTObject.
	/*!	
		\param position
		\param type
		\param material
	*/
	PRTObject(PRTVector position,int type,PRTMaterial *material); //*TODO* desp and rep?

//--------------------------------------------------------------------------------------

	//!	Empty constructor for PRTObject.
	/*!	
	*/
	PRTObject(){};

//--------------------------------------------------------------------------------------

	//!	Computes a texture coordinates for a point on 3d.
	/*!	
		\param point on 3d
		\return the texture coordinates
	*/
	virtual PRTTexCoord ComputeTexCoord(PRTVector point)=0;
	
//--------------------------------------------------------------------------------------
	
	//!	Computes a point in 3d for a texture coordinates.
	/*!	
		\param texcoord texture coordinates
		\return PRTVector the point in 3d
	*/
	virtual PRTVector ComputePoint(PRTTexCoord texcoord)=0;
	
//--------------------------------------------------------------------------------------

	//!	Computes a normal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the normal on the PRTObject
	*/
	virtual PRTVector ComputeNormal(PRTVector point)=0;

//--------------------------------------------------------------------------------------
	
	//!	Computes a tangent vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the tangent on the PRTObject
	*/
	virtual PRTVector ComputeTangent(PRTVector point)=0;

//--------------------------------------------------------------------------------------
	
	//!	Computes a binormal vector for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the binormal on the PRTObject
	*/
	virtual PRTVector ComputeBinormal(PRTVector point)=0;

//--------------------------------------------------------------------------------------
	
	//!	Computes a color for a point on 3d.
	/*!	
		\param point on 3d
		\return PRTVector the color for the point
	*/
	virtual PRTVector ComputeColor(PRTVector point)=0;

//--------------------------------------------------------------------------------------
	
	//!	Computes the intersection of the PRTObject with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	virtual bool ComputeIntersection(const PRTRay& ray, bool doublesided, PRTIntersectPoint& result)=0;

//--------------------------------------------------------------------------------------
	
	//!	Computes the intersection of the transformated PRTObject with a PRTRay.
	/*!	
		\param ray for intersection
		\param doublesided forced or not
		\return PRTIntersectPoint the point of intersection if any
	*/
	bool ComputeTransformatedIntersection(const PRTRay& ray,bool doublesided, PRTIntersectPoint& result);
	
//--------------------------------------------------------------------------------------
	
	//!	Rotate the PRTObject in 3d.
	/*!	
		\param axis of the rotation
		\param angle of rotation
		\param point of reference
	*/
	void Rotate(PRTVector axis,PRTFloat angle,PRTVector point=PRTVector(0,0,0));
	
//--------------------------------------------------------------------------------------
	
	//!	Translate the PRTObject in 3d.
	/*!	
		\param vector displacement
	*/
	void Translate(PRTVector vector);

//--------------------------------------------------------------------------------------

	//!	Scale the PRTObject in 3d.
	/*!	
		\param factor of the scale
		\param point of reference
	*/
	void Scale(PRTVector factor,PRTVector point=PRTVector(0,0,0));

//--------------------------------------------------------------------------------------

	//!	Transform the PRTObject in 3d.
	/*!	
		\param mat transformation matrix
	*/
	void Transform(PRTMatrix mat);

//--------------------------------------------------------------------------------------

};

#endif
