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
#include "PRTCsgObject.h"

PRTCsgObject::PRTCsgObject(PRTObject* o1aux,PRTObject* o2aux,int typeaux)
	:PRTObject((o1aux->convexhull.chcen+o2aux->convexhull.chcen)/2,PRT_OBJECT_CSG,o1aux->material) //*TODO* material?
{
	o1=o1aux;
	o2=o2aux;
	type=typeaux;

	//CONVEX HULL
	/*PRTFloat inix,finx,iniy,finy,iniz,finz;
	if (o1->convexhull.chbeg.x<o2->convexhull.chend.x) inix=o1->convexhull.chbeg.x; else inix=o2->convexhull.chend.x;
	if (o1->convexhull.chbeg.x>o2->convexhull.chend.x) finx=o1->convexhull.chbeg.x; else finx=o2->convexhull.chend.x;
	if (o1->convexhull.chbeg.y<o2->convexhull.chend.y) iniy=o1->convexhull.chbeg.y; else iniy=o2->convexhull.chend.y;
	if (o1->convexhull.chbeg.y>o2->convexhull.chend.y) finy=o1->convexhull.chbeg.y; else finy=o2->convexhull.chend.y;
	if (o1->convexhull.chbeg.z<o2->convexhull.chend.z) iniz=o1->convexhull.chbeg.z; else iniz=o2->convexhull.chend.z;
	if (o1->convexhull.chbeg.z>o2->convexhull.chend.z) finz=o1->convexhull.chbeg.z; else finz=o2->convexhull.chend.z;		
	convexhull.chbeg=PRTVector(inix,iniy,iniz);
	convexhull.chend=PRTVector(finx,finy,finz);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;*/

	//ENHANCED

	if (o1->convexhull.chbeg.x<o2->convexhull.chbeg.x) faux1=o1->convexhull.chbeg.x; else faux1=o2->convexhull.chbeg.x;
	if (o1->convexhull.chend.x>o2->convexhull.chend.x) faux2=o1->convexhull.chend.x; else faux2=o2->convexhull.chend.x;
	if (o1->convexhull.chbeg.y<o2->convexhull.chbeg.y) faux3=o1->convexhull.chbeg.y; else faux3=o2->convexhull.chbeg.y;
	if (o1->convexhull.chend.y>o2->convexhull.chend.y) faux4=o1->convexhull.chend.y; else faux4=o2->convexhull.chend.y;
	if (o1->convexhull.chbeg.z<o2->convexhull.chbeg.z) faux5=o1->convexhull.chbeg.z; else faux5=o2->convexhull.chbeg.z;
	if (o1->convexhull.chend.z>o2->convexhull.chend.z) faux6=o1->convexhull.chend.z; else faux6=o2->convexhull.chend.z;
	convexhull.chbeg=PRTVector(faux1,faux3,faux5);
	convexhull.chend=PRTVector(faux2,faux4,faux6);
	convexhull.chcen=convexhull.chbeg+((convexhull.chend-convexhull.chbeg)/2);
	convexhull.chrad=((convexhull.chend-convexhull.chbeg).Module())/2;
	
	what=0;
}

PRTCsgObject::~PRTCsgObject(void)
{
	delete o1;
	delete o2;
}

PRTTexCoord PRTCsgObject::ComputeTexCoord(PRTVector p)
{
	if (what==1)
		return o1->ComputeTexCoord(p);
	else
		return o2->ComputeTexCoord(p);
    //return PRTTexCoord();//*TODO*
}

PRTVector PRTCsgObject::ComputePoint(PRTTexCoord tc)
{
	return PRTVector();//*TODO*
}

PRTVector PRTCsgObject::ComputeNormal(PRTVector p)
{
	if (what==1)
	{
		return o1->ComputeNormal(p);
	}
	else
	{
		if (type==PRT_CSG_DIFERENCE)
			return -(o2->ComputeNormal(p));
		else
			return o2->ComputeNormal(p);
	}
	//return PRTVector();//*TODO*
}

PRTVector PRTCsgObject::ComputeTangent(PRTVector p)
{
	if (what==1)
		return o1->ComputeTangent(p);
	else
		return o2->ComputeTangent(p);
	//return PRTVector();//*TODO*
}

PRTVector PRTCsgObject::ComputeBinormal(PRTVector p)
{
	if (what==1)
		return o1->ComputeBinormal(p);
	else
		return o2->ComputeBinormal(p);
	//return PRTVector();//*TODO*
}

PRTVector PRTCsgObject::ComputeColor(PRTVector p)
{
	if (what==1)
		return o1->ComputeColor(p);
	else
		return o2->ComputeColor(p);
//	return PRTVector();//*TODO*
}

PRTIntersectPoint PRTCsgObject::ComputeIntersection(PRTRay r,bool testcull)
{
	// NOT ENHANCED *TODO*

	PRTIntersectPoint aux;

	switch (type)
	{
	case PRT_CSG_UNION:
		{
			//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

			if (!o2->convexhull.IntersectWithRay(r) && !o1->convexhull.IntersectWithRay(r))
				return aux;

			// OBJECT'S INTERSECTION -------------------------------------------------------------------
			
			PRTIntersectPoint aux1=o1->ComputeIntersection(r,0);//testcull);
			PRTIntersectPoint aux2=o2->ComputeIntersection(r,0);//testcull);
			if(aux1.distance<aux2.distance)
			{
				aux=aux1;
				material=o1->material;//*TODO* material
				what=1;
			}
			else
			{
				aux=aux2;
				material=o2->material;
				what=2;
			}
		}
		break;
	case PRT_CSG_INTERSECTION:
		{
			//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

			if (!o2->convexhull.IntersectWithRay(r) || !o1->convexhull.IntersectWithRay(r))
				return aux;

			// OBJECT'S INTERSECTION -------------------------------------------------------------------

			PRTIntersectPoint aux1=o1->ComputeIntersection(r,0);//testcull);
			PRTIntersectPoint aux2=o2->ComputeIntersection(r,0);//testcull);
			PRTIntersectPoint aux3;
			PRTIntersectPoint aux4;
			
			bool no=false;

			while(!no)
			{
				if(aux1.collision && aux2.collision)
				{
					aux3=o1->ComputeIntersection(PRTRay(aux1.point+(r.dir*PRTFloat(0.001)),r.dir),0);//testcull);1?*TODO*
					if (aux3.collision)	aux3.distance+=aux1.distance+PRTFloat(0.001);
					aux4=o2->ComputeIntersection(PRTRay(aux2.point+(r.dir*PRTFloat(0.001)),r.dir),0);//testcull);
					if (aux4.collision)	aux4.distance+=aux2.distance+PRTFloat(0.001);
					
					if ((aux1.distance<aux2.distance && aux2.distance<aux3.distance))
					{
						aux=aux2;
						material=o2->material;//*TODO* material
						no=true;
						what=2;
					}
					else if ((aux2.distance<aux1.distance && aux1.distance<aux4.distance))
					{
						aux=aux1;
						material=o1->material;//*TODO* material
						no=true;
						what=1;
					}
					else
					{
						aux1=aux3;//o1->ComputeIntersection(PRTRay(aux3.point+(r.dir*0.0001),r.dir),0);//testcull);
						aux2=aux4;//o2->ComputeIntersection(PRTRay(aux4.point+(r.dir*0.0001),r.dir),0);//testcull);
					}
				}
				else
					no=true;
			}
		}
		break;
	case PRT_CSG_DIFERENCE:
		{
			//RAY'S WITH OBJECT'S CONVEX HULL ----------------------------------------------

			if (!o1->convexhull.IntersectWithRay(r)) // because the order is o1-o2
				return aux;

			// OBJECT'S INTERSECTION -------------------------------------------------------------------

			PRTIntersectPoint aux1=o1->ComputeIntersection(r,0);//testcull);
			PRTIntersectPoint aux2=o2->ComputeIntersection(r,0);//testcull);
			PRTIntersectPoint aux3;
			PRTIntersectPoint aux4;
			if (aux1.collision && aux2.collision)
			{
				if(aux1.distance<aux2.distance)
				{
					aux=aux1;
					material=o1->material;//*TODO* material
					what=1;
				}
				else
				{
					aux3=o1->ComputeIntersection(PRTRay(aux1.point+(r.dir*PRTFloat(0.001)),r.dir),0);//testcull);
					if (aux3.collision)	aux3.distance+=aux1.distance+PRTFloat(0.001);
					aux4=o2->ComputeIntersection(PRTRay(aux2.point+(r.dir*PRTFloat(0.001)),r.dir),0);//testcull);
					if (aux4.collision)	aux4.distance+=aux2.distance+PRTFloat(0.001);

					if (aux3.collision && aux4.collision)
					{
						if (aux4.distance<aux3.distance)
						{
							aux=aux4;
							material=o2->material;//*TODO* material
							what=2;
						}
						/*else
						{
							return aux;
						}*/
					}
					else
					{
						if (!aux4.collision)
						{
							aux=aux1;
							material=o1->material;
							what=1;
						}
					}
				}
			}
			else
			{
				if (aux1.collision)
				{
					aux=aux1;
					material=o1->material;
					what=1;
				}
			}
		}
		break;
	}

	return aux;
}
