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

#ifndef __PRTTEXCOORD_H__
#define __PRTTEXCOORD_H__

#include "PRTUtil.h"
#include "PRTMath.h"

//--------------------------------------------------------------------------------------
//! The PRTTexCoord class.
/*!	This class represents a texture coordinate in 2d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTTexCoord
{

public:

	PRTFloat u,v;		//!< The u,v coordinates.
	
//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTTexCoord.
	/*!	
	*/
	inline PRTTexCoord(void){u=0;v=0;}

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTTexCoord.
	/*!	
		\param uaux the u coordinate
		\param vaux the v coordinate
	*/
	inline PRTTexCoord(const PRTFloat &uaux,const PRTFloat &vaux){u=uaux;v=vaux;}

//--------------------------------------------------------------------------------------

	//!	The PRTTexCoord+PRTTexCoord operator.
	/*!	
		\param aux the PRTTexCoord to add
		\return the PRTTexCoord solution
	*/
	inline PRTTexCoord operator+(const PRTTexCoord &aux){ return PRTTexCoord(u+aux.u,v+aux.v); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTTexCoord=PRTTexCoord operator.
	/*!	
		\param t the PRTTexCoord to assign 
		\return the PRTTexCoord solution
	*/
	inline PRTTexCoord operator=(const PRTTexCoord &t){ u=t.u; v=t.v; return t; }

//--------------------------------------------------------------------------------------

	//!	The PRTTexCoord[] operator.
	/*!	
		\param i the index 
		\return the PRTFloat in PRTTexCoord[i]
	*/	
	inline PRTFloat &operator[](const unsigned int i){ return (&u)[i]; }

//--------------------------------------------------------------------------------------

};

#endif
