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

#ifndef __PRTRENDER_H__
#define __PRTRENDER_H__

#include "PRTUtil.h"
#include "PRTRay.h"

#define PRT_RAYTRACING					7001
#define PRT_PHOTONMAPPING				7002
#define PRT_PATCHES						7003

//--------------------------------------------------------------------------------------
//! The PRTRender class.
/*!	This class represents an abstract render method of the PiscisRT.*/
//--------------------------------------------------------------------------------------

class PRTMain;

class PISCISRT_API PRTRender
{

public:

	int type;

	PRTMain* main;

	PRTRender(PRTMain*,int);

	virtual PRTVector RayTrace(PRTRay,int)=0;

};

#endif
