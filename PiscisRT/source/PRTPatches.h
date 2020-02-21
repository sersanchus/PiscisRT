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

#ifndef __PRTPATCHES_H__
#define __PRTPATCHES_H__

#include "PRTUtil.h"
#include "PRTRender.h"

//--------------------------------------------------------------------------------------
//! The PRTPatches class.
/*!	This class represents the patches radiosity method.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPatch
{
public:
		PRTVector emmision;
		PRTVector excident;
		PRTVector incident;
		PRTVector reflectance;
};

class PISCISRT_API PRTObjectPatches
{
public:
		int width,height;
		PRTPatch *p;
};

class PISCISRT_API PRTPatches : public PRTRender
{

public:
	PRTPatches(PRTMain*);

	PRTVector RayTrace(PRTRay,int);
	PRTVector RayTraceToPatch(PRTRay,int);
	
	PRTFloat patchwidth;
	PRTFloat patchheight;

	void CalculatePatches(void);
	void CalculateLighting(void);

	PRTObjectPatches* patches;

	PRTVector CalculateIncidentLight(PRTVector,PRTVector,PRTVector);

	PRTFloat *Mfront;
	PRTFloat *Mup;
	PRTFloat *Mdown;
	PRTFloat *Mleft;
	PRTFloat *Mright;
	int lighttam;

};

#endif
