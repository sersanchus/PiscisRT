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

#ifndef __PRTBITMAP_H__
#define __PRTBITMAP_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTTexture.h"
#include "PRTBitmapImpExp.h"

//--------------------------------------------------------------------------------------
//! The PRTBitmap class.
/*!	This class represents a bitmap texture in 2d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTBitmap : public PRTTexture
{

public:

	unsigned char *t;				//!< Array with the texels of the texture.
		
//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTBitmap.
	/*!	
	*/
	PRTBitmap(void);

//--------------------------------------------------------------------------------------

	//!	The plugin constructor of the PRTBitmap.
	/*!	
		\param data the bitmap data
	*/
	PRTBitmap(PRTBitmapImpExpData* data);

//--------------------------------------------------------------------------------------

	//!	The destructor of the PRTBitmap.
	/*!	
	*/
	~PRTBitmap(void);

//--------------------------------------------------------------------------------------

	//!	The asigment operator of the PRTBitmap.
	/*!	
		\param tex the PRTBitmap origin
		\return the PRTBitmap
	*/
	inline PRTBitmap operator=(PRTBitmap tex) {t=tex.t;width=tex.width;height=tex.height;bytespp=tex.bytespp;return tex;}

//--------------------------------------------------------------------------------------

	//!	The function to calculate the color that a texture coordinate have.
	/*!	
		\param texcoord the texture coordinate 
		\param point the point for a procedural texture 
		\return the color
	*/
	PRTVector CalculateColor(PRTTexCoord texcoord,PRTVector point=PRTVector(0,0,0));

//--------------------------------------------------------------------------------------

	//!	The function to calculate the bump vector that a texture coordinate have.
	/*!	
		\param texcoord the texture coordinate 
		\param u the u direction
		\param v the v direction
		\param point the point in 3d for a procedural texture
		\return the bump vector
	*/
	PRTVector CalculateBump(PRTTexCoord texcoord,PRTVector u,PRTVector v,PRTVector point=PRTVector(0,0,0));

//--------------------------------------------------------------------------------------

};

#endif
