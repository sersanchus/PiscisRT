//---------------------------------------------------------------------------//
//                                                                           //
// PiscisRT (Piscis Ray Tracer)                                              //
//                                                                           //
// Copyright (C) 2020  Jesus Gumbau Portales                                 //
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
// Contact with jesusgumbau@gmail.com                                        //
//                                                                           //
// Almazora-Castellón-Spain                                                  //
//                                                                           //
//---------------------------------------------------------------------------//

#ifndef __PRTARRAY_H__
#define __PRTARRAY_H__

#include "PRTUtil.h"

#include <string.h>

#define DEFAULT_CAPACITY 16

//--------------------------------------------------------------------------------------
//! The PRTArray class.
/*!	This class represents a resizeable contiguous memory container.*/
//--------------------------------------------------------------------------------------

template <class T>
class PISCISRT_API PRTArray
{

public:

//--------------------------------------------------------------------------------------

	//!	The constructor of the array
	/*!	
	*/
	PRTArray():buffer(NULL), num(0), capacity(0)
	{
		Reserve(DEFAULT_CAPACITY);
	}


//--------------------------------------------------------------------------------------

	//!	A function to add a new member at end of the list.
	/*!	
		\param member the object to add
	*/
	void AddAtEnd(const T& elem)
	{
		if (num == capacity)
		{
			// Need to resize the array
			Reserve(capacity * 2);
		}
		buffer[num] = elem;
		num++;
	}

//--------------------------------------------------------------------------------------

	//!	A function to get a member by its ordered location on the list.
	/*!	
		\param pos the ordered position as int
		\return void * the object in that position
	*/
	const T& GetAtPos(unsigned int pos) const
	{
		return buffer[pos];
	}

//--------------------------------------------------------------------------------------

	//!	A function to clear the container
	/*!	
	*/
	void Clear(void)
	{
		num=0;
	}

//--------------------------------------------------------------------------------------

	//!	A function to return the lenght of the list.
	/*!	
		\return the number of stored members
	*/
	unsigned int Length(void) const { return num; }
	
	
	void Reserve(unsigned int totalCount)
	{
		T* newBuffer = new T[totalCount];
			
		if (buffer != NULL)
		{
			memcpy(newBuffer, buffer, num * sizeof(T));
			delete[] buffer;
		}
		
		buffer = newBuffer;
		capacity = totalCount;
	}

//--------------------------------------------------------------------------------------

	//!	The destructor of the array.
	/*!	
	*/
	~PRTArray()
	{
		Clear();
		delete[] buffer;
		capacity = 0;
		buffer = NULL;
	}

//--------------------------------------------------------------------------------------
	
private:
	T* buffer;
	unsigned int num;
	unsigned int capacity;

};

#endif
