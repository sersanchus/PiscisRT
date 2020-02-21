#include "PRTUtil.h"
#include "PRTRayTracing.h"
#include "PRTMain.h"
#include "PRTTexture.h"

PRTRayTracing::PRTRayTracing(PRTMain* m)
	:PRTRender(m,PRT_RAYTRACING)
{

}

PRTVector PRTRayTracing::RayTrace(PRTRay r,int deep)
{
	PRTVector color(0,0,0);
	
	PRTIntersectPoint colisiona=main->FindNearestIntersection(r);
		
	if (colisiona.collision)
	{
		PRTObject* objeto=colisiona.object;

		PRTIntersectPoint colisionaini=colisiona;
		if (main->BTransformations)
			colisionaini.point=objeto->invtrans*colisiona.point;

		if (objeto->material->forced)
		{
			color=objeto->material->color;
		}
		else
		{

			PRTVector normal=objeto->ComputeNormal(colisionaini.point);

			// si hay bumpmap distorsionamos la normal
			if (main->BBump && objeto->material->bumpmap)
			{
				PRTTexCoord mmm=objeto->ComputeTexCoord(colisionaini.point);
				normal=normal+objeto->material->bumpmap->CalculateBump(mmm,objeto->ComputeTangent(colisionaini.point).Normalize(),objeto->ComputeBinormal(colisionaini.point).Normalize(),colisionaini.point-objeto->position)*objeto->material->bumpfactor;
				normal=normal.Normalize();//*TODO* se normaliza o no?
			}

			if (main->BTransformations)
				normal=objeto->transrot*normal;

			//si estamos en double sided y la normal esta al reves, invertirla
			if (main->BDoubleSided && r.dir*normal>0) //la normal esta en la misma direccion que el rayo
				normal=-normal;
					
			PRTVector reflexion(0,0,0);
			PRTVector refraccion(0,0,0);
			PRTVector transparencia(0,0,0);
			
			if (deep>0)
			{
				if (main->BReflections && objeto->material->reflection>0.0) reflexion=main->ComputeReflection(r,colisiona,normal,deep,objeto);
				if (main->BRefractions && objeto->material->refraction>0.0) refraccion=main->ComputeRefraction(r,colisiona,normal,deep,objeto);
				if (main->BAlpha && objeto->material->alpha>0.0) transparencia=main->ComputeTransparence(r,colisiona,normal,deep,objeto);
			}
								
			PRTListMember *qluzesiterator=main->LightsList.first;
			PRTLight* qluzes;
			while (qluzesiterator!=NULL)
			{
				qluzes=((PRTLight*)(qluzesiterator->object));
		
				PRTVector coloraux(0,0,0);

				coloraux=qluzes->ComputeLightRay(r,colisiona,normal,objeto,main);//potencia que llega desde la luz al punto
				
				coloraux=main->ComputeReflectedLight(colisiona.point,
											   (colisiona.point-qluzes->ComputeWhatPointLight(colisiona.point)).Normalize(),
											   (r.orig-colisiona.point).Normalize(),
											   objeto,
											   normal,
											   coloraux);//cantidad de luz q se ve desde la camara en el punto
				
				color+=coloraux;
			
				qluzesiterator=qluzesiterator->next;
			}
			
			if (deep>0) 
			{
				PRTFloat refl=PRTFloat(0.0); if (main->BReflections) refl=objeto->material->reflection;
				PRTFloat refr=PRTFloat(0.0); if (main->BRefractions) refr=objeto->material->refraction;
				PRTFloat alph=PRTFloat(0.0); if (main->BAlpha) alph=objeto->material->alpha;
				color=((color)*(1-alph)*(1-refl)*(1-refr))+(transparencia*alph)+(reflexion*refl)+(refraccion*refr);
			}
						
			if (main->BTextures && objeto->material->texture!=NULL)
			{
				PRTVector janemore=objeto->material->texture->CalculateColor(objeto->ComputeTexCoord(colisionaini.point),colisionaini.point-objeto->position);
				color.x=color.x*janemore.x;	color.y=color.y*janemore.y;	color.z=color.z*janemore.z;
			}

			color+=objeto->material->ambientcolor;
			
			if (color.x>PRTFloat(1.0)) color.x=PRTFloat(1.0); if (color.y>PRTFloat(1.0)) color.y=PRTFloat(1.0); if (color.z>PRTFloat(1.0)) color.z=PRTFloat(1.0);
			if (color.x<PRTFloat(0.0)) color.x=PRTFloat(0.0); if (color.y<PRTFloat(0.0)) color.y=PRTFloat(0.0); if (color.z<PRTFloat(0.0)) color.z=PRTFloat(0.0);
		}
	}
	//main->numrayos++;
	return color;
}
