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

#ifndef __PRTPERLIN_H__
#define __PRTPERLIN_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTTexCoord.h"
#include "PRTTexture.h"

#define PRT_TEXTURE_PERLIN_CLOUD		10001
#define PRT_TEXTURE_PERLIN_WOOD			10002
#define PRT_TEXTURE_PERLIN_MARBLE		10003

//--------------------------------------------------------------------------------------
//! The PRTPerlin class.
/*!	This class represents a Perlin noise in 2d/3d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPerlin : public PRTTexture
{

public:

	int type;							//!< The type of the Perlin noise.
	PRTFloat scale;						//!< The scale of the Perlin noise.
	PRTFloat alpha;						//!< The alpha factor.
	PRTFloat beta;						//!< The beta factor.
	int n;								//!< Number of harmonics in the noise.
	PRTVector color1;					//!< The init color.  0.0
	PRTVector color2;					//!< The final color. 1.0

//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTPerlin.
	/*!	
		\param type the type of the Perlin noise
	*/
	PRTPerlin(int type);

//--------------------------------------------------------------------------------------

	//!	The destructor of the PRTPerlin.
	/*!	
	*/
	~PRTPerlin(void);

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
