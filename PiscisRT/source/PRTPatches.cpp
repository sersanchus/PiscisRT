#include "PRTUtil.h"
#include "PRTPatches.h"
#include "PRTMain.h"
#include "PRTBitmap.h"

PRTPatches::PRTPatches(PRTMain* m)
	:PRTRender(m,PRT_PATCHES)
{
	patchwidth=1;//*TODO*
	patchheight=1;//*TODO*

	lighttam=5; //tamaño del hemicubo
}

PRTVector PRTPatches::RayTrace(PRTRay r,int deep)
{
	PRTVector color(0.0,0.0,0.0);
	PRTIntersectPoint colisiona=main->FindNearestIntersection(r);
	if (colisiona.collision)
	{
		PRTObject* objeto=colisiona.object;
	
		/*if (objeto->material->forced)//?¿
		{
			color=objeto->material->color;
		}
		else*/
		{
			//if (objeto->material->lightmap!=NULL)
			//{
				color=objeto->material->lightmap->CalculateColor(objeto->ComputeTexCoord(colisiona.point));
			//}
		}
		
		if (color.x>1) color.x=1; if (color.y>1) color.y=1; if (color.z>1) color.z=1;
		if (color.x<0) color.x=0; if (color.y<0) color.y=0; if (color.z<0) color.z=0;
	}
	//main->numrayos++;
	return color;
}

unsigned int numlights=0;

void PRTPatches::CalculatePatches(void)
{
	numlights=main->LightsList.Lenght();
	unsigned int i;

	for (i=0;i<numlights;i++)
	{
		PRTLight *l=(PRTLight*)main->LightsList.GetAtPos(i);
		
		PRTMaterial *mat=new PRTMaterial();
		mat->color=l->color;
		
		switch(l->type)
		{
			case PRT_TRIANGLELIGHT:
				main->AddTriangle(((PRTTriangleLight*)l)->p1,((PRTTriangleLight*)l)->p2,((PRTTriangleLight*)l)->p3,
								  ((PRTTriangleLight*)l)->n1,((PRTTriangleLight*)l)->n2,((PRTTriangleLight*)l)->n3,
									mat);
				break;
		}
	}
	
	patches=new PRTObjectPatches[main->ObjectsList.Lenght()/*+main->LightsList.Lenght()*/];

	int pos=0;

	int num=5;//tamaño del lightmap y patches

	for (i=0;i<main->ObjectsList.Lenght();i++)
	{
		
		PRTObject *o=(PRTObject*)main->ObjectsList.GetAtPos(i);
		o->material->lightmap=new PRTBitmap();
		o->material->lightmap->width=num;
		o->material->lightmap->height=num;
		o->material->lightmap->bytespp=3;
		((PRTBitmap*)(o->material->lightmap))->t=new unsigned char[o->material->lightmap->width
												  *o->material->lightmap->height
												  *o->material->lightmap->bytespp];

		patches[pos].width=num;
		patches[pos].height=num;
		patches[pos].p=new PRTPatch[num*num];


		for (int j=0;j<patches[pos].height;j++)
		{
			for (int k=0;k<patches[pos].width;k++)
			{
				if (main->ObjectsList.Lenght()-i>numlights)
					patches[pos].p[j*patches[pos].width+k].emmision=PRTVector(0,0,0);
				else
					patches[pos].p[j*patches[pos].width+k].emmision=o->material->color;
				patches[pos].p[j*patches[pos].width+k].incident=PRTVector(0,0,0);
				((PRTBitmap*)(o->material->lightmap))->t[j*o->material->lightmap->width*o->material->lightmap->bytespp+k*o->material->lightmap->bytespp+0]=(unsigned char)(patches[pos].p[j*patches[pos].width+k].emmision.x*255.0f);
				((PRTBitmap*)(o->material->lightmap))->t[j*o->material->lightmap->width*o->material->lightmap->bytespp+k*o->material->lightmap->bytespp+1]=(unsigned char)(patches[pos].p[j*patches[pos].width+k].emmision.y*255.0f);
				((PRTBitmap*)(o->material->lightmap))->t[j*o->material->lightmap->width*o->material->lightmap->bytespp+k*o->material->lightmap->bytespp+2]=(unsigned char)(patches[pos].p[j*patches[pos].width+k].emmision.z*255.0f);
				//patches[pos].p[j*patches[pos].width+k].excident=0;
			}
		}

		pos++;

		/*for (int j=0;j<o->material->lightmap->width;j++)
		{
			for (int k=0;k<o->material->lightmap->height;k++)
			{
				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)+(k*o->material->lightmap->bytespp)+0]=(255.0f*PRTRandom());
				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)+(k*o->material->lightmap->bytespp)+1]=(255.0f*PRTRandom());
				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)+(k*o->material->lightmap->bytespp)+2]=(255.0f*PRTRandom());
			}
		}*/

	}
	/*for (i=0;i<main->LightsList.Lenght();i++)
	{
		PRTLight *l=(PRTLight*)main->LightsList.GetAtPos(i);
		
		patches[pos].width=num;
		patches[pos].height=num;
		patches[pos].p=new Patch[num*num];

		for (int j=0;j<patches[pos].width;j++)
		{
			for (int k=0;k<patches[pos].height;k++)
			{
				patches[pos].p[j*patches[pos].width+k].emmision=l->color;
				//patches[pos].p[j*patches[pos].width+k].excident=l->intensity;
			}
		}

		pos++;
	}*/


	//creo el multiplier map
	
	PRTVector lightcamera,lightmiddle,lightmiddleall,lightcameraall,desp;
	unsigned int lighti,lightj;

	lightcamera=lightcameraall=PRTVector(((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2);
	lightmiddle=lightmiddleall=PRTVector(0,0,-((PRTFloat)lighttam-1)/2);
	desp=PRTVector(0,0,0);
	lighti=lighttam;
	lightj=lighttam;
	Mfront=new PRTFloat[lightj*lighti];
	for (i=0;i<lighti;i++)
	{
		for (unsigned int j=0;j<lightj;j++)
		{
			PRTFloat light=lightmiddle%(PRTVector((PRTFloat)j,(PRTFloat)i,0)-lightcamera);
			light*=lightmiddleall%(PRTVector((PRTFloat)j,(PRTFloat)i,0)+desp-lightcameraall);
			Mfront[i*lightj+j]=light;
		}
	}
	
	lightcamera=PRTVector(((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2);
	lightmiddle=PRTVector(0,0,-((PRTFloat)lighttam-1)/2);
	desp=PRTVector(0,(PRTFloat)(lighttam-1),0);
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	Mdown=new PRTFloat[lightj*lighti];
	for (i=0;i<lighti;i++)
	{
		for (unsigned int j=0;j<lightj;j++)
		{
			PRTFloat light=lightmiddle%(PRTVector((PRTFloat)j,(PRTFloat)i,0)-lightcamera);
			light*=lightmiddleall%(PRTVector((PRTFloat)j,(PRTFloat)i,0)+PRTVector(desp.x,desp.y,(PRTFloat)i)-lightcameraall);
			Mfront[i*lightj+j]=light;
		}
	}

	lightcamera=PRTVector(((PRTFloat)lighttam-1)/2,0,((PRTFloat)lighttam-1)/2);
	lightmiddle=PRTVector(0,0,-((PRTFloat)lighttam-1)/2);
	desp=PRTVector(0,(PRTFloat)-(lighttam-1)/2,0);
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	Mup=new PRTFloat[lightj*lighti];
	for (i=0;i<lighti;i++)
	{
		for (unsigned int j=0;j<lightj;j++)
		{
			PRTFloat light=lightmiddle%(PRTVector((PRTFloat)j,(PRTFloat)i,0)-lightcamera);
			light*=lightmiddleall%(PRTVector((PRTFloat)j,(PRTFloat)i,0)+PRTVector(desp.x,desp.y,(PRTFloat)(lighti-i))-lightcameraall);
			Mup[i*lightj+j]=light;
		}
	}

	lightcamera=PRTVector(0,((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2);
	lightmiddle=PRTVector(0,0,-((PRTFloat)lighttam-1)/2);
	desp=PRTVector((PRTFloat)-(lighttam-1)/2,0,0);
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	Mleft=new PRTFloat[lightj*lighti];
	for (i=0;i<lighti;i++)
	{
		for (unsigned int j=0;j<lightj;j++)
		{
			PRTFloat light=lightmiddle%(PRTVector((PRTFloat)j,(PRTFloat)i,0)-lightcamera);
			light*=lightmiddleall%(PRTVector((PRTFloat)j,(PRTFloat)i,0)+PRTVector(desp.x,desp.y,(PRTFloat)(lightj-j))-lightcameraall);
			Mleft[i*lightj+j]=light;
		}
	}

	lightcamera=PRTVector(((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2,((PRTFloat)lighttam-1)/2);
	lightmiddle=PRTVector(0,0,-((PRTFloat)lighttam-1)/2);
	desp=PRTVector((PRTFloat)(lighttam-1),0,0);
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	Mright=new PRTFloat[lightj*lighti];
	for (i=0;i<lighti;i++)
	{
		for (unsigned int j=0;j<lightj;j++)
		{
			PRTFloat light=lightmiddle%(PRTVector((PRTFloat)j,(PRTFloat)i,0)-lightcamera);
			light*=lightmiddleall%(PRTVector((PRTFloat)j,(PRTFloat)i,0)+PRTVector(desp.x,desp.y,(PRTFloat)j)-lightcameraall);
			Mright[i*lightj+j]=light;
		}
	}


	//lo normalizo

	PRTFloat div=0.0f;

	lighti=lighttam;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			div+=Mfront[i*lightj+j];
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			div+=Mdown[i*lightj+j];
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			div+=Mup[i*lightj+j];
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			div+=Mleft[i*lightj+j];
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			div+=Mright[i*lightj+j];

	lighti=lighttam;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			Mfront[i*lightj+j]/=div;
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			Mdown[i*lightj+j]/=div;
	lighti=(lighttam-1)/2;
	lightj=lighttam;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			Mup[i*lightj+j]/=div;
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			Mleft[i*lightj+j]/=div;
	lighti=lighttam;
	lightj=(lighttam-1)/2;
	for (i=0;i<lighti;i++)
		for (unsigned int j=0;j<lightj;j++)
			Mright[i*lightj+j]/=div;
	

}


PRTVector Dir(PRTVector camarapos,PRTVector camarafin,PRTVector camaraup,PRTFloat fov,int i,int j,int ancho,int alto)
{
	if (camaraup==PRTVector())
		camaraup=PRTVector(0,1,0);
	
	PRTVector N=(camarafin-camarapos).Normalize();	// -z de la camara
	PRTVector U=(N^camaraup).Normalize();				// x de la camara
	PRTVector V=(N^U).Normalize();					// -y de la camara
	PRTFloat fovrad=PRTGradToRad(fov);
	PRTVector Xinc=U*((2*PRTTan(fovrad/2))/alto);
	PRTVector Yinc=V*((2*PRTTan(fovrad/2))/alto);
	PRTFloat x=(PRTFloat)j;
	PRTFloat y=(PRTFloat)i;
	
	PRTVector aux=(N+(Yinc*(2*y+1-alto)*0.5)+(Xinc*(2*x+1-ancho)*0.5));

	return aux.Normalize();
}


PRTVector PRTPatches::CalculateIncidentLight(PRTVector point,PRTVector normal,PRTVector tangent)
{
	PRTVector TotalLight(0,0,0);

	int lighti,lightj,i,j;
	PRTFloat div=0.0f;
	PRTVector center;

	//front

	lighti=lighttam;
	lightj=lighttam;

	PRTVector *Hfront=new PRTVector[lighti*lightj];
	PRTVector *Rfront=new PRTVector[lighti*lightj];

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Hfront[i*lightj+j]=Rfront[i*lightj+j]=PRTVector(0,0,0);
			div+=Mfront[i*lightj+j];
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			PRTVector dir=Dir(point,point+normal,-(normal^tangent).Normalize(),45,j,i,lightj,lighti);
			Hfront[i*lightj+j]=RayTrace(PRTRay(point+normal*0.1f,dir),1);
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Rfront[i*lightj+j]=Hfront[i*lightj+j]*Mfront[i*lightj+j];
			TotalLight+=Rfront[i*lightj+j];
		}
	}

	delete Hfront;
	delete Rfront;

	//down

	lighti=(lighttam-1)/2;
	lightj=lighttam;

	PRTVector *Hdown=new PRTVector[lighti*lightj];
	PRTVector *Rdown=new PRTVector[lighti*lightj];

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Hdown[i*lightj+j]=Rdown[i*lightj+j]=PRTVector(0,0,0);
			div+=Mdown[i*lightj+j];
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			PRTVector dir=Dir(point,point+(normal^tangent).Normalize(),normal,45,j,i,lightj,lighti);
			Hdown[i*lightj+j]=RayTrace(PRTRay(point+normal*0.1f,dir),1);
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Rdown[i*lightj+j]=Hdown[i*lightj+j]*Mdown[i*lightj+j];
			TotalLight+=Rdown[i*lightj+j];
		}
	}

	delete Hdown;
	delete Rdown;

	//up

	lighti=(lighttam-1)/2;
	lightj=lighttam;

	PRTVector *Hup=new PRTVector[lighti*lightj];
	PRTVector *Rup=new PRTVector[lighti*lightj];

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Hup[i*lightj+j]=Rup[i*lightj+j]=PRTVector(0,0,0);
			div+=Mup[i*lightj+j];
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			PRTVector dir=Dir(point,point-(normal^tangent).Normalize(),normal,45,j,i,lightj,lighti);
			Hup[i*lightj+j]=RayTrace(PRTRay(point+normal*0.1f,dir),1);
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Rup[i*lightj+j]=Hup[i*lightj+j]*Mup[i*lightj+j];
			TotalLight+=Rup[i*lightj+j];
		}
	}

	delete Hup;
	delete Rup;

	//left

	lighti=lighttam;
	lightj=(lighttam-1)/2;

	PRTVector *Hleft=new PRTVector[lighti*lightj];
	PRTVector *Rleft=new PRTVector[lighti*lightj];

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Hleft[i*lightj+j]=Rleft[i*lightj+j]=PRTVector(0,0,0);
			div+=Mleft[i*lightj+j];
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			PRTVector dir=Dir(point,point-tangent,normal,45,j,i,lightj,lighti);
			Hleft[i*lightj+j]=RayTrace(PRTRay(point+normal*0.1f,dir),1);
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Rleft[i*lightj+j]=Hleft[i*lightj+j]*Mleft[i*lightj+j];
			TotalLight+=Rleft[i*lightj+j];
		}
	}

	delete Hleft;
	delete Rleft;

	//right

	lighti=lighttam;
	lightj=(lighttam-1)/2;

	PRTVector *Hright=new PRTVector[lighti*lightj];
	PRTVector *Rright=new PRTVector[lighti*lightj];

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Hright[i*lightj+j]=Rright[i*lightj+j]=PRTVector(0,0,0);
			div+=Mright[i*lightj+j];
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			PRTVector dir=Dir(point,point+tangent,normal,45,j,i,lightj,lighti);
			Hright[i*lightj+j]=RayTrace(PRTRay(point+normal*0.1f,dir),1);
		}
	}

	for (i=0;i<lighti;i++)
	{
		for (j=0;j<lightj;j++)
		{
			Rright[i*lightj+j]=Hright[i*lightj+j]*Mright[i*lightj+j];
			TotalLight+=Rright[i*lightj+j];
		}
	}

	delete Hright;
	delete Rright;
	
	
	/*PRTVector down=RayTrace(PRTRay(point+normal*0.1f,(normal^tangent).Normalize()),1);
	PRTVector up=RayTrace(PRTRay(point+normal*0.1f,-(normal^tangent).Normalize()),1);
	PRTVector left=RayTrace(PRTRay(point+normal*0.1f,tangent),1);
	PRTVector right=RayTrace(PRTRay(point+normal*0.1f,-tangent),1);*/

	return TotalLight/div;
}

/*procedure Calc_Incident_Light(point: P, vector: N)  
    light TotalLight
    hemicube H, R, M
    H = empty
    M = Multiplier Hemicube
    R = empty
    div = sum of pixels in M
    camera C
    C.lens = P
    C.direction = N
    H.front = RenderView(C, N, Full_View)
    C.direction = N rotated 90° down
    H.down = RenderView(C, N, Top_Half)
    C.direction = N rotated 90° up
    H.up = RenderView(C, N, Bottom_Half)
    C.direction = N rotated 90° left
    H.left = RenderView(C, N, Right_Half)
    C.direction = N rotated 90° right
    H.right = RenderView(C, N, Left_Half)
    multiply all pixels in H by corresponding
    pixels in M, storing the results in R
    TotalLight = black
    loop p through each pixel in R
      add p to TotalLight 
    end loop
    
    divide TotalLight by div
    return TotalLight
  end procedure*/

void PRTPatches::CalculateLighting(void)
{

	// loop of passes

	int loop=0;
	int loopmax=5;
	unsigned int i;

	while(loop<loopmax)
	{
		int pos=0;

		for (i=0;i<main->ObjectsList.Lenght();i++)
		{
			PRTObject *o=(PRTObject*)main->ObjectsList.GetAtPos(i);
			
			/*for (int j=0;j<patches[pos].width;j++)
			{
				for (int k=0;k<patches[pos].height;k++)
				{
					patches[pos].p[j*patches[pos].width+k].incident=1;
				}
			}*/
			for (unsigned int j=0;j<o->material->lightmap->height;j++)
			{
				for (unsigned int k=0;k<o->material->lightmap->width;k++)
				{
					PRTVector emmision=patches[pos].p[j*patches[pos].width+k].emmision;
					//PRTVector excident=patches[pos].p[j*patches[pos].width+k].excident;
					PRTVector incident=patches[pos].p[j*patches[pos].width+k].incident;
					PRTVector reflectance=PRTVector(4,4,4);//patches[pos].p[k*patches[pos].width+j].reflectance;

					((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
											+(k*o->material->lightmap->bytespp)
											+0]=(unsigned char)(((incident.x*reflectance.x)+emmision.x)*255.0f);
					((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
											+(k*o->material->lightmap->bytespp)
											+1]=(unsigned char)(((incident.y*reflectance.y)+emmision.y)*255.0f);
					((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
											+(k*o->material->lightmap->bytespp)
											+2]=(unsigned char)(((incident.z*reflectance.z)+emmision.z)*255.0f);
				}
			}
			pos++;
		}	

		pos=0;

		for (i=0;i<main->ObjectsList.Lenght();i++)
		{
			PRTObject *o=(PRTObject*)main->ObjectsList.GetAtPos(i);
			
			for (int j=0;j<patches[pos].height;j++)
			{
				for (int k=0;k<patches[pos].width;k++)
				{
					PRTTexCoord tccenter;//what texture coordinate have the patch
					tccenter.u=(PRTFloat(k)/(o->material->lightmap->width))*o->repu+o->despu+1.0f/(o->material->lightmap->width*2.0f);
					tccenter.v=(PRTFloat(j)/(o->material->lightmap->height))*o->repv+o->despv+1.0f/(o->material->lightmap->height*2.0f);
					PRTVector center=o->ComputePoint(tccenter);//center of the patch in 3d
					PRTVector normal=o->ComputeNormal(center);//normal of the patch in 3d
					PRTVector tangent=o->ComputeTangent(center).Normalize();//tangent vector

					if (main->ObjectsList.Lenght()-i>numlights)
						patches[pos].p[j*patches[pos].width+k].incident=CalculateIncidentLight(center,normal,tangent);
					//patches[pos].p[j*patches[pos].width+k].emmision+=patches[pos].p[j*patches[pos].width+k].incident;
				}
			}
			pos++;
		}	
		/*for (i=0;i<main->LightsList.Lenght();i++)
		{
			PRTLight *l=(PRTLight*)main->LightsList.GetAtPos(i);
			
			for (int j=0;j<patches[pos].width;j++)
			{
				for (int k=0;k<patches[pos].height;k++)
				{
					//PRTTexCoord tccenter;//what texture coordinate have the patch
					//tccenter.u=(k/(o->material->lightmap->width-1))*o->repu+o->despu;
					//tccenter.v=(j/(o->material->lightmap->height-1))*o->repv+o->despv;
					//PRTVector center=o->ComputePoint(tccenter);//center of the patch in 3d
					//PRTVector normal=o->ComputeNormal(center);//normal of the patch in 3d

					patches[pos].p[j*patches[pos].width+k].incident=PRTVector(0,0,0);//CalculateIncidentLight(center,normal);
				}
			}
			pos++;
		}*/

		loop++;
	}

		
	// calculate excident light - final

	int pos=0;
	for (i=0;i<main->ObjectsList.Lenght();i++)
	{
		PRTObject *o=(PRTObject*)main->ObjectsList.GetAtPos(i);
		
		/*for (int j=0;j<patches[pos].width;j++)
		{
			for (int k=0;k<patches[pos].height;k++)
			{
				patches[pos].p[j*patches[pos].width+k].incident=1;
			}
		}*/
		for (unsigned int j=0;j<o->material->lightmap->height;j++)
		{
			for (unsigned int k=0;k<o->material->lightmap->width;k++)
			{
				PRTVector emmision=patches[pos].p[j*patches[pos].width+k].emmision;
				//PRTVector excident=patches[pos].p[j*patches[pos].width+k].excident;
				PRTVector incident=patches[pos].p[j*patches[pos].width+k].incident;
				PRTVector reflectance=PRTVector(4,4,4);//patches[pos].p[k*patches[pos].width+j].reflectance;

				((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+0]=(unsigned char)(((incident.x*reflectance.x)+emmision.x)*255.0f);
				((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+1]=(unsigned char)(((incident.y*reflectance.y)+emmision.y)*255.0f);
				((PRTBitmap*)(o->material->lightmap))->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+2]=(unsigned char)(((incident.z*reflectance.z)+emmision.z)*255.0f);
			}
		}
		pos++;
	}	
	/*for (i=0;i<main->LightsList.Lenght();i++)
	{
		PRTLight *l=(PRTLight*)main->LightsList.GetAtPos(i);
		
		for (int j=0;j<patches[pos].width;j++)
		{
			for (int k=0;k<patches[pos].height;k++)
			{
				//patches[pos].p[j*patches[pos].width+k].incident=1;
			}
		}
		pos++;
	}*/
		
	/*for (int j=0;j<o->material->lightmap->width;j++)
		{
			for (int k=0;k<o->material->lightmap->height;k++)
			{
				PRTVector emmision;
				PRTVector excident;
				PRTVector incident;
				PRTVector reflectance;

				PRTTexCoord tccenter;//what texture coordinate have the patch
				tccenter.u=(k/(o->material->lightmap->width-1))*o->repu+o->despu;
				tccenter.v=(j/(o->material->lightmap->height-1))*o->repv+o->despv;
				PRTVector center=o->ComputePoint(tccenter);//center of the patch in 3d
				PRTVector normal=o->ComputeNormal(center);//normal of the patch in 3d


				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+0]=(incident.x*reflectance.x)+emmision.x;
				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+1]=(incident.y*reflectance.y)+emmision.y;
				o->material->lightmap->t[(j*o->material->lightmap->width*o->material->lightmap->bytespp)
										+(k*o->material->lightmap->bytespp)
										+2]=(incident.z*reflectance.z)+emmision.z;
			}
		}

	}*/	
}
