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

#ifndef __PRTMATERIAL_H__
#define __PRTMATERIAL_H__

#include "PRTUtil.h"
#include "PRTVector.h"
#include "PRTTexture.h"

#define BRDF_DIFFUSE		5003 //Lambertian diffuse
#define BRDF_TS				5004 //The Cook-Torrance-Sparrow model, with Beckmann distribution function, Blinn geometric attenuation, and Fresnel reflection term
#define BRDF_BM				5005 //The Beard-Maxwell model
#define BRDF_ON				5006 //The rough diffuse surface model by Oren and Nayar
#define BRDF_HTSG			5007 //The He-Torrance-Sillon-Greenberg BRDF

//*TODO*
/*	B(HTSG),
	B(Ward),
	B(Cyl),
	B(LS),
	B(HLS),
	B(Minn),
	B(Phong),
	B(Schlick),
	B(Cos),
	B(Strauss),
	B(Rotate),
	B(Sum),
	B(SqDiff),
	B(AbsDiff),
	B(Rotate),*/

#define BRDF_PHONG			5001
#define BRDF_COSINE_LOBE	5002

//--------------------------------------------------------------------------------------
//! The PRTMaterial class.
/*!	This class represents a material for an object in the PiscisRT engine.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTMaterial
{

public:

	PRTVector color;					//!< The color of the material (diffuse).
	PRTVector ambientcolor;				//!< The ambient color of the material.
	PRTFloat shininess;					//!< The shininess factor.
	PRTFloat specular;					//!< The specular factor.
	PRTTexture *texture;				//!< The PRTTexture of the material.
	PRTTexture *lightmap;				//!< The lightmap of the material.
	PRTTexture *bumpmap;				//!< The Bump Map of the material.
	PRTFloat bumpfactor;				//!< The Bump Map multiplier.
	PRTFloat reflection;				//!< The reflection factor.
	PRTFloat refraction;				//!< The refraction factor.
	PRTFloat alpha;						//!< The alpha factor. 1 is transparent.
	PRTFloat gireflection;				//!< The global illumination reflection factor.
	PRTFloat girefraction;				//!< The global illumination refraction factor.
	PRTFloat gialpha;					//!< The global illumination alpha factor.

	int BRDFtype;						//!< The type of the BRDF of the material.
	PRTFloat BRDFp1;					//!< The first parameter of the BRDF function. See code.
	PRTFloat BRDFp2;					//!< The second parameter of the BRDF function. See code.
	PRTFloat BRDFp3;					//!< The third parameter of the BRDF function. See code.
	PRTFloat BRDFp4;					//!< The fourth parameter of the BRDF function. See code.
	PRTFloat BRDFp5;					//!< The fifth parameter of the BRDF function. See code.
	PRTFloat BRDFp6;					//!< The sixth parameter of the BRDF function. See code.
	PRTFloat BRDFp7;					//!< The seventh parameter of the BRDF function. See code.

	PRTFloat glossyper;					//!< The glossy perturbation factor.

	bool forced;						//!< This material is only a color?, without attenuation and BRDF

//--------------------------------------------------------------------------------------

	//!	The empty constructor of the PRTMaterial.
	/*!	
	*/
	PRTMaterial(void);
	
//--------------------------------------------------------------------------------------

	//!	Another constructor of the PRTMaterial.
	/*!	
		\param color the diffuse color of the material
	*/
	PRTMaterial(const PRTVector &color);

//--------------------------------------------------------------------------------------

	//!	The function to compute the BRDF of this material with the BRDF type and parameters.
	/*!	
		\param in the input vector of the light
		\param out the output vector to the watcher
		\param normal the normal in the point of incidence
		\param p1 the p1 BRDF parameter
		\param p2 the p2 BRDF parameter
		\param p3 the p3 BRDF parameter
		\param p4 the p4 BRDF parameter
		\param p5 the p5 BRDF parameter
		\param p6 the p6 BRDF parameter
		\param p7 the p7 BRDF parameter
	*/
	PRTFloat ComputeBRDF(PRTVector &in,const PRTVector &out,const PRTVector &normal,PRTFloat p1=-1.0,PRTFloat p2=-1.0,PRTFloat p3=-1.0,PRTFloat p4=-1.0,PRTFloat p5=-1.0,PRTFloat p6=-1,PRTFloat p7=-1);

//--------------------------------------------------------------------------------------

};

#endif
