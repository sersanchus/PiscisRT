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
#include "PRTTriangleLight.h"
#include "PRTMain.h"
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

PRTTriangleLight::PRTTriangleLight(const PRTVector &auxp1,const PRTVector &auxp2,const PRTVector &auxp3,const PRTVector &auxn1,const PRTVector &auxn2,const PRTVector &auxn3,const PRTVector &c)
	:PRTLight(PRT_TRIANGLELIGHT,c)
{
	p1=auxp1;
	p2=auxp2;
	p3=auxp3;
	n1=auxn1;
	n2=auxn2;
	n3=auxn3;

	area2=((p2-p1)^(p3-p1)).Module();
}

PRTVector PRTTriangleLight::ComputeWhatPointLight(PRTVector &p)
{
  // 1  - it works fine

	//PRTFloat max=(PRTFloat)RAND_MAX;
	PRTFloat a=PRTWRandom();//(PRTFloat)rand()/max;
	PRTFloat b=PRTWRandom();//(PRTFloat)rand()/max;
	if ((a+b)>1)
	{
		a=1-a;
		b=1-b;	
	}
	PRTFloat c=1-a-b;
	return p1*a+p2*b+p3*c;

   // 2  - like in renderbitch

	/*PRTFloat s=PRTSqrt(PRTWRandom());
	PRTFloat t=PRTWRandom();
	return p1*(1.0-s)+p2*(s-t*s)+p3*s*t;
	*/

	//*TODO* ?
	//r1 and r2 in [0-1]
	//return p1+(p3-p1)*r1+(p2-p3)*r2;
}

PRTVector PRTTriangleLight::ComputeLightRay(PRTRay &r,PRTIntersectPoint &collision, PRTVector &normal, const PRTObject* object, PRTMain* main)
{
	PRTVector coloraux(0,0,0);
	PRTVector shadow(1,1,1);
	PRTVector totalshadow(0,0,0);
	PRTVector lightpos=(ComputeWhatPointLight(collision.point));
	PRTFloat collisiontolight=(collision.point-lightpos).Module();


	if (/*BDoubleSided ||*/ /*(luz->pos!=collision.punto) &&*/ (lightpos-collision.point).Normalize()*normal>=0) // if it is oriented to the light
	{								
		if (main->BShadows)
		{
			for(int i=0;i<main->shadowpass;i++)
			{
				lightpos=(ComputeWhatPointLight(collision.point));
				collisiontolight=(collision.point-lightpos).Module();
				
				//the is any object between?
					
				PRTRay shadowray(lightpos,(((collision.point)-lightpos)).Normalize());
				PRTListMember *o2=main->ObjectsList.first;
				PRTFloat LAMBDA=PRTFloat(0.000001);
				bool surethatyes=false;

				while (!surethatyes && o2!=NULL)
				{
					PRTIntersectPoint shadowinter;
					if (object!=((PRTObject*)(o2->object)))
					{
						shadowinter=shadowray.Intersect((PRTObject*)o2->object,!main->BDoubleSided,main->BTransformations);
						//main->numintertest+=shadowray.numrayintertest;
					
						PRTFloat disshadow=(shadowinter.point-lightpos).Module();
						if (shadowinter.collision && disshadow<(collisiontolight-LAMBDA)) //*TODO*
						{
							surethatyes=true;
							shadow=PRTVector(0,0,0);
						}
					}
					o2=o2->next;
				}
				
				totalshadow+=shadow;
			}
			shadow=totalshadow/(main->shadowpass);
		}
		/*if (shadow.x>1) shadow.x=1;
		if (shadow.y>1) shadow.y=1;
		if (shadow.z>1) shadow.z=1;
		*/


		// LIKE IN RENDER BITCH POLYGONAL LIGHT

		PRTVector point=collision.point;
		bool above = true; // if finally it's true, all vertexes take place
		bool below = true; // if finally it's true, all vertexes don't take place

		int num_above = 0; // number of vertexes in the contribution

		if ((normal*(p1-point))<0.0){above=false;}else{num_above++;below=false;}
		if ((normal*(p2-point))<0.0){above=false;}else{num_above++;below=false;}
		if ((normal*(p3-point))<0.0){above=false;}else{num_above++;below=false;}

		if (above)
		{
			PRTVector Phi(0,0,0);

			PRTVector Vi;PRTVector Vi_plus=(p1-point).Unit();
			Vi=Vi_plus;Vi_plus=(p2-point).Unit();Phi+=((Vi^Vi_plus).Unit())*(ArcCos(Vi*Vi_plus));
			Vi=Vi_plus;Vi_plus=(p3-point).Unit();Phi+=((Vi^Vi_plus).Unit())*(ArcCos(Vi*Vi_plus));
			Vi=Vi_plus;Vi_plus=(p1-point).Unit();Phi+=((Vi^Vi_plus).Unit())*(ArcCos(Vi*Vi_plus));
			
			PRTVector vaux=(PRTVector(color.x*intensity,color.y*intensity,color.z*intensity)/PRTPI);
			PRTFloat faux=MAX(0,(-Phi*normal));
			coloraux=vaux*faux*0.5;
		}
		else if (!below)
		{
			PRTVector I(0,0,0);
			std::vector<PRTVector> verts = std::vector<PRTVector>(3,PRTVector(0,0,0));
			verts[0] = p1;
			verts[1] = p2;
			verts[2] = p3;

			std::vector<int> bads;
			std::vector<int> goods;

			for (int j = 0; j < 3; j++)
			{
				if ((normal*(verts[j]-point)) < 0.0)
					bads.push_back(j);
				else
					goods.push_back(j);
			}

			if (bads.size() == 1)
			{
				int g1, g2;
				g1 = goods[0];
				g2 = goods[1];

				PRTVector p = verts[bads[0]] - point;
				PRTVector d1 = verts[g1] - verts[bads[0]];
				PRTVector d2 = verts[g2] - verts[bads[0]];

				PRTFloat dist1 = d1.Unitize();
				PRTFloat dist2 = d2.Unitize();

				PRTFloat t1 = -(normal*p)/(normal*d1);
				PRTFloat t2 = -(normal*p)/(normal*d2);

				PRTFloat fract1 = t1/dist1;
				PRTFloat fract2 = t2/dist2;

				PRTVector v1 = (verts[bads[0]]*(1.0-fract1) + verts[g1]*fract1 - point).Unit();
				PRTVector v2 = (verts[g1] - point).Unit();
				PRTVector v3 = (verts[g2] - point).Unit();
				PRTVector v4 = (verts[bads[0]]*(1.0-fract2) + verts[g2]*fract2 - point).Unit();

				PRTVector Phi =	((v1^v2)).Unit() * ArcCos((v1*v2)) +
								((v2^v3)).Unit() * ArcCos((v2*v3)) +
								((v3^v4)).Unit() * ArcCos((v3*v4)) +
								((v4^v1)).Unit() * ArcCos((v4*v1));
			
				PRTVector vaux=(PRTVector(color.x*intensity,color.y*intensity,color.z*intensity)/PRTPI);
				PRTFloat faux=fabs((Phi*normal));
				I += vaux*0.5*faux;
			}
			else if (bads.size() == 2)
			{
				int g1 = goods[0];

				PRTVector p = verts[g1] - point;
				PRTVector d1 = verts[bads[0]] - verts[g1];
				PRTVector d2 = verts[bads[1]] - verts[g1];

				PRTFloat dist1 = d1.Unitize();
				PRTFloat dist2 = d2.Unitize();

				PRTFloat t1 = -(normal*p)/(normal*d1);
				PRTFloat t2 = -(normal*p)/(normal*d2);

				PRTFloat fract1 = t1/dist1;
				PRTFloat fract2 = t2/dist2;

				PRTVector v1 = (verts[g1]*(1.0-fract1) + verts[bads[0]]*fract1 - point).Unit();
				PRTVector v2 = (verts[g1] - point).Unit();
				PRTVector v3 = (verts[g1]*(1.0-fract2) + verts[bads[1]]*fract2 - point).Unit();

				PRTVector Phi =	((v1^v2)).Unit() * ArcCos((v1*v2)) +
								((v2^v3)).Unit() * ArcCos((v2*v3)) +
								((v3^v1)).Unit() * ArcCos((v3*v1));
			
				PRTVector vaux=(PRTVector(color.x*intensity,color.y*intensity,color.z*intensity)/PRTPI);
				PRTFloat faux=fabs((Phi*normal));
				I += vaux*0.5*faux;
			}
			else if (bads.size() == 0)
			{
				PRTVector v1 = (verts[0] - point).Unit();
				PRTVector v2 = (verts[1] - point).Unit();
				PRTVector v3 = (verts[2] - point).Unit();

				PRTVector Phi =	((v1^v2)).Unit() * ArcCos((v1*v2)) +
								((v2^v3)).Unit() * ArcCos((v2*v3)) +
								((v3^v1)).Unit() * ArcCos((v3*v1));
			
				PRTVector vaux=(PRTVector(color.x*intensity,color.y*intensity,color.z*intensity)/PRTPI);
				PRTFloat faux=fabs((Phi*normal));
				I += vaux*0.5*faux;
			}
			coloraux=I;
		}

		coloraux=PRTVector(/*attenuation**/shadow.x*coloraux.x*0.002/**color.x*intensity*/, // *TODO*!!!!
						   /*attenuation**/shadow.y*coloraux.y*0.002/**color.y*intensity*/,
						   /*attenuation**/shadow.z*coloraux.z*0.002/**color.z*intensity*/);

	}

	return coloraux;
}
