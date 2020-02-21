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
#include "PRTSphereLight.h"
#include "PRTMain.h"

PRTSphereLight::PRTSphereLight(const PRTVector &center,const PRTFloat radius,const PRTVector &color)
	:PRTLight(PRT_SPHERELIGHT,color)
{
	p=center;
	r=radius;
}

PRTVector PRTSphereLight::ComputeWhatPointLight(PRTVector& point)
{
	PRTFloat a,b;
	PRTVector ret;
	//PRTMaterial mataux;
	//PRTSphere sphere(p,r,&mataux);
	//do
	//{
		a=PRTFloat((PRTWRandom()*PRTPI)-PRTFloat(1.5707963267948966192313216916398));
		b=PRTFloat(PRTWRandom()*PRTFloat(6.283185307179586476925286766559));	
		ret.x=p.x+(r*PRTCos(a)*PRTCos(b));
		ret.y=p.y+(r*PRTCos(a)*PRTSin(b));
		ret.z=p.z+(r*PRTSin(a));
	//}while((sphere.ComputeNormal(ret)*((point-ret).Normalize()))<=0);//*TODO*
	return ret;
}

PRTVector PRTSphereLight::ComputeLightRay(PRTRay &r,PRTIntersectPoint &collision,PRTVector &normal, const PRTObject* object, PRTMain* main)
{
	PRTVector coloraux(0,0,0);
	PRTVector shadow(1,1,1);
	PRTVector totalshadow(0,0,0);
	PRTVector lightpos=(ComputeWhatPointLight(collision.point));
	PRTFloat collisiontolight=(collision.point-lightpos).Module();
	
	if (/*BDoubleSided ||*/ /*(luz->pos!=collision.point) &&*/ (collision.point-lightpos).Normalize()*normal/*object->n1*/<=0) //if it is oriented to the light
	{								
		if (main->BShadows)
		{
			for(int i=0;i<main->shadowpass;i++)
			{
				lightpos=(ComputeWhatPointLight(collision.point));
				collisiontolight=(collision.point-lightpos).Module();
				
				//is ther any object between?
					
				PRTRay rayodeshadow(lightpos,(((collision.point)-lightpos)).Normalize());
				PRTListMember *o2=main->ObjectsList.first;
				PRTFloat LAMBDA=PRTFloat(0.000001);
				bool issurethatyes=false;

				while (!issurethatyes && o2!=NULL)
				{
					PRTIntersectPoint shadowinter;
					if (object!=((PRTObject*)(o2->object)))
					{
						shadowinter=rayodeshadow.Intersect((PRTObject*)o2->object,!main->BDoubleSided,false/*main->BTransformations*/);
						//main->numintertest+=rayodeshadow.numrayintertest;
					
						PRTFloat disshadow=(shadowinter.point-lightpos).Module();
						if (shadowinter.collision && disshadow<(collisiontolight-LAMBDA)) //*TODO*
						{
							issurethatyes=true;
							shadow=PRTVector(0,0,0);
						}
					}
					o2=o2->next;
				}
				
				//*TODO* don't use points that are in the hidden side of the sphere

				PRTMaterial mataux;

				PRTObject* sphereaux=new PRTSphere(p,this->r,&mataux);

				if (!issurethatyes)
				{
					PRTIntersectPoint shadowinter;
					shadowinter=rayodeshadow.Intersect(sphereaux,!main->BDoubleSided,main->BTransformations);
					//main->numintertest+=rayodeshadow.numrayintertest;
				
					PRTFloat disshadow=(shadowinter.point-lightpos).Module();
					if (shadowinter.collision && disshadow<(collisiontolight-LAMBDA)) //*TODO*
					{
						//issurethatyes=true; //*TODO*
						shadow=PRTVector(0,0,0);
					}
				}

				delete sphereaux;

				totalshadow+=shadow;
			}
			shadow=totalshadow/PRTFloat(main->shadowpass);
		}
		/*if (shadow.x>1) shadow.x=1;
		if (shadow.y>1) shadow.y=1;
		if (shadow.z>1) shadow.z=1;
		*/
		
		// LIGHT ATTENUATION COMPONENT
		PRTFloat dis=collisiontolight;//*TODO*
		PRTFloat attenuation=1/(attenuation0+attenuation1*dis+attenuation2*dis*dis);
		if (attenuation>1)
			attenuation=1;
		
		coloraux=PRTVector(attenuation*shadow.x*color.x*intensity,
						   attenuation*shadow.y*color.y*intensity,
						   attenuation*shadow.z*color.z*intensity);
	}
	return coloraux;
}
