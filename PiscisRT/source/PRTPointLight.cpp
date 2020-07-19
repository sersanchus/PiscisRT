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

#include "PRTUtil.h"
#include "PRTPointLight.h"
#include "PRTMain.h"

PRTPointLight::PRTPointLight(const PRTVector &p,const PRTVector &c)
	:PRTLight(PRT_POINTLIGHT,c)
{
	pos=p;
}

PRTVector PRTPointLight::ComputeWhatPointLight(PRTVector &p)
{
	return pos;
}

PRTVector PRTPointLight::ComputeLightRay(PRTRay &r,PRTIntersectPoint &collision, PRTVector &normal, const PRTObject* object, PRTMain* main)
{
	PRTVector coloraux(0,0,0);

	PRTVector lightpos=(ComputeWhatPointLight(collision.point));
	
	PRTFloat collisiontolight=(collision.point-lightpos).Module();
	
	if (/*BDoubleSided ||*/ /*(luz->pos!=collision.punto) &&*/ (collision.point-lightpos).Normalize()*normal<=0) //the normal pointing to the light
	{								
		// shadow
		PRTVector shadow(1.0,1.0,1.0); // ther isn't shadow, multiplier

		if (main->BShadows)
		{
			//ther is any object between?
							
			PRTRay rayodesombra(lightpos,(((collision.point)-lightpos)).Normalize());
//			PRTListMember *o2=main->ObjectsList.first;
			PRTFloat LAMBDA=PRTFloat(0.000001);
			bool seguroqsi=false;

//			while (!seguroqsi && o2!=NULL)
			for (unsigned int i=0; i<!seguroqsi && main->ObjectsList.Length(); i++)
			{
				PRTObject* obj = main->ObjectsList.GetAtPos(i);
				PRTIntersectPoint sombrainter;//auxiliar collision
				if (object!=obj)
				{
					sombrainter=rayodesombra.Intersect(obj,!main->BDoubleSided,main->BTransformations);
					//main->numintertest+=rayodesombra.numrayintertest;
				
					PRTFloat dissombra=(sombrainter.point-lightpos).Module();
					if (sombrainter.collision && dissombra<(collisiontolight-LAMBDA)) //*TODO*
					{
						seguroqsi=true;
						shadow=PRTVector(0,0,0);
					}
				}
//				o2=o2->next;
			}
		}
		/*
		if (shadow.x>1) shadow.x=1;
		if (shadow.y>1) shadow.y=1;
		if (shadow.z>1) shadow.z=1;
		*/
		
		// ATTENUATION OF LIGHT
		PRTFloat dis=collisiontolight;
		PRTFloat attenuation=1/(attenuation0+attenuation1*dis+attenuation2*dis*dis);
		if (attenuation>1)
			attenuation=1;
		
		coloraux=PRTVector(attenuation*shadow.x*color.x*intensity,
						   attenuation*shadow.y*color.y*intensity,
						   attenuation*shadow.z*color.z*intensity);

		
	}
	return coloraux;
}
