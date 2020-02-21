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

#ifndef __PRTDINAMICLIST_H__
#define __PRTDINAMICLIST_H__

#include "PRTUtil.h"

//--------------------------------------------------------------------------------------
//! The PRTListMember class.
/*!	This class represents a member of a PRTDinamicList.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTListMember
{

public:

	PRTListMember *next;			//!< Pointer to the next element if any.
	void *object;					//!< Pointer to the object stored in the PRTListMember, accesed by a void pointer.

//--------------------------------------------------------------------------------------

	//!	The constructor of a list member.
	/*!	
	*/
	PRTListMember();

//--------------------------------------------------------------------------------------

	//!	The destroyer of a list member.
	/*!	
	*/
	~PRTListMember();

//--------------------------------------------------------------------------------------

};

//--------------------------------------------------------------------------------------
//! The PRTDinamicList class.
/*!	This class represents a dinamic pointer list named PRTDinamicList.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTDinamicList
{

public:

	unsigned int num;						//!< Number of stored list members.
	PRTListMember *first;			//!< Pointer to the first list member.
	PRTListMember *last;			//!< Pointer to the last list member.

//--------------------------------------------------------------------------------------

	//!	The constructor of the dinamic list.
	/*!	
	*/
	PRTDinamicList();

//--------------------------------------------------------------------------------------

	//!	A function to add a new member at end of the list.
	/*!	
		\param member the object to add as a void *
	*/
	void AddAtEnd(void* member);

//--------------------------------------------------------------------------------------

	//!	A function to get a member by its ordered location on the list.
	/*!	
		\param pos the ordered position as int
		\return void * the object in that position
	*/
	void *GetAtPos(unsigned int pos);

//--------------------------------------------------------------------------------------

	//!	A function to free the list used memory.
	/*!	
	*/
	void Free(void);

//--------------------------------------------------------------------------------------

	//!	A function to return the lenght of the list.
	/*!	
		\return the number of stored members
	*/
	unsigned int Lenght(void);

//--------------------------------------------------------------------------------------

	//!	The destroyer of the dinamic list.
	/*!	
	*/
	~PRTDinamicList();

//--------------------------------------------------------------------------------------
	
};

#endif
