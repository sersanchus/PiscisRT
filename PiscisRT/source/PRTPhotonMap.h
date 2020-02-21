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

#ifndef __PRTPHOTONMAP_H__
#define __PRTPHOTONMAP_H__

#include "PRTUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include "PRTMath.h"

//--------------------------------------------------------------------------------------
//! The PRTPhoton class.
/*!	This class represents a lighting photon. For used within a PRTPhotonMap. As in "Realistic Image Synthesis Using Photon Mapping" by Henrik Wann Jensen.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPhoton 
{

public:

	PRTFloat pos[3];				//!< The position of a photon in 3d.
	short plane;					//!< Axis of balancement.
	unsigned char theta, phi;		//!< Angles of incidence orientation.
	PRTFloat power[3];				//!< The power of the photon.

};

//--------------------------------------------------------------------------------------
//! The PRTNearestPhotons class.
/*!	This class represents the nearest photons to a location. For used within a PRTPhotonMap. As in "Realistic Image Synthesis Using Photon Mapping" by Henrik Wann Jensen.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTNearestPhotons 
{

public:

	int max;						//!< The maximum number of photons to search.
	int found;						//!< The number of founded photons.
	int gotHeap;					//!< The heap is full.
	PRTFloat pos[3];				//!< The position to evaluate the nearest photons.
	PRTFloat *dist2;				//!< The distances of the photons to searched position.
	const PRTPhoton **index;		//!< The indexes of the nearest photons.

};

//--------------------------------------------------------------------------------------
//! The PRTPhotonMap class.
/*!	This class represents a KdTree where photons are stored. As in "Realistic Image Synthesis Using Photon Mapping" by Henrik Wann Jensen.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPhotonMap 
{

private:

	PRTPhoton *photons;				//!< The array of photons stored in the KdTree.
	int storedphotons;				//!< The number of stored photons.
	int halfstoredphotons;			//!< The half of storedphotons.
	int maxphotons;					//!< The maximum number of photons to store.
	int prevscale;					//!< The previous scale.
	PRTFloat costheta[256];			//!< An array to store fast cosinus solutions of theta.
	PRTFloat sintheta[256];			//!< An array to store fast sinus solutions of theta.
	PRTFloat cosphi[256];			//!< An array to store fast cosinus solutions of phi.
	PRTFloat sinphi[256];			//!< An array to store fast sinus solutions of phi.
	PRTFloat bboxMin[3];			//!< The minimum point of a bounding box.
	PRTFloat bboxMax[3];			//!< The maximum point of a bounding box.

public:

//--------------------------------------------------------------------------------------

	//!	The constructor of a PRTPhotonMap.
	/*!	
		\param maxphot the maximum photons to store
	*/
	PRTPhotonMap(int maxphot);

//--------------------------------------------------------------------------------------

	//!	The destructor of a PRTPhotonMap.
	/*!	
	*/
	~PRTPhotonMap();

//--------------------------------------------------------------------------------------

	//!	Store a photon on the PhotonMap.
	/*!	
		\param power the power of the photon
		\param pos the position of the photon
		\param dir the incidence direction
	*/
	void Store(const PRTFloat power[3],const PRTFloat pos[3],const PRTFloat dir[3]);

//--------------------------------------------------------------------------------------

	//!	Scale the photon power by a factor scale.
	/*!	
		\param scale the scale factor
	*/
	void ScalePhotonPower(const PRTFloat scale);
	
//--------------------------------------------------------------------------------------

	//!	Balances the PRTPhotonMap for an enhanced search.
	/*!	
	*/
	void Balance(void);

//--------------------------------------------------------------------------------------

	//!	This function splits correctly the photon map to balance it.
	/*!	
		\param p the photon map to balance
		\param start 
		\param end
		\param median
		\param axis
	*/
	void MedianSplit(PRTPhoton **p,const int start,const int end,const int median,const int axis);

//--------------------------------------------------------------------------------------

	//!	This function solves the balance of a segment on the photon map.
	/*!	
		\param pbal the photon map balanced
		\param porg the photon map to balance
		\param index
		\param start
		\param end
	*/
	void BalanceSegment(PRTPhoton **pbal,PRTPhoton **porg,const int index,const int start,const int end);

//--------------------------------------------------------------------------------------

	//!	This function estimates the irradiance of a point, the most important function. Find the nearest photons and estimate the irradiance that produce on a location in 3d.
	/*!	
		\param irrad the output irradiance
		\param pos the position to estimate
		\param normal the normal in that point
		\param maxdist the maximum distance for the calculation
		\param nphotons the number of photons to search
	*/
	void IrradianceEstimate(PRTFloat irrad[3],const PRTFloat pos[3],const PRTFloat normal[3],const PRTFloat maxdist,const int nphotons);

//--------------------------------------------------------------------------------------

	//!	This function search the nearest photons to a photon.
	/*!	
		\param np the PRTNearestPhotons output class
		\param index the index of that photon
	*/
	void LocatePhotons(PRTNearestPhotons *const np,const int index);
	
//--------------------------------------------------------------------------------------

	//!	This function calculates the incidence direction of a photon.
	/*!	
		\param dir the output incidence direction
		\param p the photon
	*/
	void PhotonDir(PRTFloat *dir,const PRTPhoton *p);
	
//--------------------------------------------------------------------------------------

	//!	Write the photons to a file.
	/*!	
		\param filename the file to write in
	*/
	void WriteToDisk(const char *filename);

//--------------------------------------------------------------------------------------

	//!	Read the photons on a file.
	/*!	
		\param filename the file to read from
		\param numphotons the number of photons to read
	*/
	void ReadFromDisk(const char *filename, int numphotons);

//--------------------------------------------------------------------------------------

	//!	Is the Photon map empty?.
	/*!	
		\return is empty or not
	*/
	bool Empty(void);

//--------------------------------------------------------------------------------------

	//!	Function to deallocate the memory used by the Photon map.
	/*!	
	*/
	void Free(void);
	
//--------------------------------------------------------------------------------------

	//!	Function to solve the number of stored photons.
	/*!	
		\return the number of stored photons
	*/
	int StoredPhotons(void);

//--------------------------------------------------------------------------------------

};

#endif
