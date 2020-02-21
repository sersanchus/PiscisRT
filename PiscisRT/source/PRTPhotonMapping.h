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

#ifndef __PRTPHOTONMAPPING_H__
#define __PRTPHOTONMAPPING_H__

#include "PRTUtil.h"
#include "PRTRender.h"
#include "PRTPhotonMap.h"

//--------------------------------------------------------------------------------------
//! The PRTPhotonMapping class.
/*!	This class represents the photon mapping method.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPhotonMapping : public PRTRender
{

public:
	PRTPhotonMapping(PRTMain*);

	PRTVector RayTrace(PRTRay,int);

	PRTVector PhotonTrace(PRTRay,PRTVector,int);
	bool BuildPhotonIllumination(void);
	PRTDinamicList PhotonIllumination;
	PRTVector ComputeRadiance(PRTVector,PRTVector,PRTVector,PRTObject*);
	int numphotones;
	int groupphotones;
	PRTFloat scalephoton;

	PRTPhotonMap *photons;

};

#endif
