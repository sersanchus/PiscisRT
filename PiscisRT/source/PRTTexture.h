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

#ifndef __PRTTEXTURE_H__
#define __PRTTEXTURE_H__

#include "PRTUtil.h"

/*#ifdef __MAKE_WINDOWS__
	//#include "ijl.h" //intel jpeg library
	extern "C" //only for c
	{
		#include "../extincs/jpeglib.h"
	}
#endif

#ifdef __MAKE_LINUX__
	extern "C" //only for c
	{
		#include "../extincs/jpegliblinux.h"
	}
#endif*/

#include "PRTVector.h"
#include "PRTTexCoord.h"

#define PRT_TEXTURE_BITMAP		8001 //A bitmap texture from a file
#define PRT_TEXTURE_PERLIN		8002 //Perlin noise

//--------------------------------------------------------------------------------------
//! The PRTTexture class.
/*!	This class represents a texture in 2d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTTexture
{

public:

	unsigned int width;						//!< The width of the texture.
	unsigned int height;						//!< The height of the texture.
	unsigned int bytespp;					//!< The bytes per pixel of the texture.
	
	bool filter;					//!< Want you to filter the image?.

	int type;						//!< Type of the texture.

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTTexture.
	/*!	
	*/
	PRTTexture(int);

//--------------------------------------------------------------------------------------

	//!	The destructor of the PRTTexture.
	/*!	
	*/
	~PRTTexture(void);

//--------------------------------------------------------------------------------------

	//!	The function to calculate the color that a texture coordinate have.
	/*!	
		\param texcoord the texture coordinate 
		\param point the point for a procedural texture 
		\return the color
	*/
	virtual PRTVector CalculateColor(PRTTexCoord texcoord,PRTVector point=PRTVector(0,0,0))=0;

//--------------------------------------------------------------------------------------

	//!	The function to calculate the bump vector that a texture coordinate have.
	/*!	
		\param texcoord the texture coordinate 
		\param u the u direction
		\param v the v direction
		\param point the point in 3d for a procedural texture
		\return the bump vector
	*/
	virtual PRTVector CalculateBump(PRTTexCoord texcoord,PRTVector u,PRTVector v,PRTVector point=PRTVector(0,0,0))=0;

//--------------------------------------------------------------------------------------

};

#endif
