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

#ifndef __PRTOCTREE_H__
#define __PRTOCTREE_H__

#include "PRTUtil.h"
#include "PRTMath.h"
#include "PRTDinamicList.h"
#include "PRTRay.h"
#include "PRTObject.h"

//--------------------------------------------------------------------------------------
//! The PRTOcTreeNode class.
/*!	This class represents a Octree node of the PRTOcTree.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTOcTreeNode
{

public:

	PRTFloat maxx,maxy,maxz,minx,miny,minz;			//!< The definition of a Octree in 3d, with the minimum and maximum point of the box, oriented in axes.
	PRTOcTreeNode *n1,*n2,*n3,*n4,*n5,*n6,*n7,*n8;	//!< The 8 nodes that are inside, the sons.
	PRTDinamicList *objects;						//!< The objects that are inside the boxel.
	PRTOcTreeNode *father;							//!< Pointer to the father.

	bool leaf;										//!< Is this node a leaf?.
		
//--------------------------------------------------------------------------------------

	//!	The constructor of an OcTreeNode. Save the poition and solve the objects that are inside.
	/*!	
		\param mx
		\param my
		\param mz
		\param Mx
		\param My
		\param Mz
		\param leaf
		\param father
	*/
	PRTOcTreeNode(PRTFloat mx,PRTFloat my,PRTFloat mz,PRTFloat Mx,PRTFloat My,PRTFloat Mz,bool leaf,PRTOcTreeNode *father);
	
//--------------------------------------------------------------------------------------

	//!	The destructor of an OcTreeNode. Delete the objects.
	/*!	
	*/
	~PRTOcTreeNode();

//--------------------------------------------------------------------------------------

	//!	Free the objects memory.
	/*!	
	*/
	void FreeObjects();

//--------------------------------------------------------------------------------------

};

//--------------------------------------------------------------------------------------
//! The PRTOcTree class.
/*!	This class represents a OcTree structure.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTOcTree
{

private:

	PRTOcTreeNode *root;		//!< The root of the Tree.
	unsigned int deep;			//!< Maximum deep of Tree.
	unsigned int numonleaf;		//!< Number of minimum objects in each leaf.

public:

//--------------------------------------------------------------------------------------

	//!	The constructor of an OcTree. Need the position of the OcTree, the deep of it, and the Dinamic List to the objects.
	/*!	
		\param mx
		\param my
		\param mz
		\param Mx
		\param My
		\param Mz
		\param deep
		\param numonleaf
		\param objects
	*/
	PRTOcTree(PRTFloat mx,PRTFloat my,PRTFloat mz,PRTFloat Mx,PRTFloat My,PRTFloat Mz,unsigned int deep,unsigned int numonleaf,PRTDinamicList *objects);
	
//--------------------------------------------------------------------------------------

	//!	The destructor of an OcTree.
	/*!	
	*/
	~PRTOcTree();

//--------------------------------------------------------------------------------------

	//!	Return the objects that should intersect the ray.
	/*!	
		\param ray the input ray
		\param object object to discard.
		\return the objects in a dinamic list
	*/
	PRTDinamicList *ReturnObjects(PRTRay ray,PRTObject *object);
	
//--------------------------------------------------------------------------------------
	
};

#endif
