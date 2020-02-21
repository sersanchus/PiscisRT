#include "PRTUtil.h"
#include "PRTPhotonMapping.h"
#include "PRTMain.h"
#include "PRTTexture.h"

PRTPhotonMapping::PRTPhotonMapping(PRTMain* m)
	:PRTRender(m,PRT_PHOTONMAPPING)
{
	numphotones=1000;
	groupphotones=10;
	scalephoton=10;
	
	//KDTREE
	photons=new PRTPhotonMap(0); // esto luego se redimensiona
}

PRTVector PRTPhotonMapping::RayTrace(PRTRay r,int deep)
{
	// RAY TRACING

	PRTVector color(0.0,0.0,0.0);
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
				normal=normal+objeto->material->bumpmap->CalculateBump(mmm,objeto->ComputeTangent(colisionaini.point),objeto->ComputeBinormal(colisionaini.point),colisionaini.point-objeto->position)*objeto->material->bumpfactor;
				normal=normal.Normalize();//*TODO* se normaliza o no?
			}

			if (main->BTransformations)
				normal=objeto->transrot*normal;
			
			//si estamos en double sided y la normal esta al reves, invertirla
			if (main->BDoubleSided && r.dir*normal>0) //la normal esta en la misma direccion que el rayo
				normal=-normal;
					
			PRTVector reflexion(0.0,0.0,0.0);
			PRTVector refraccion(0.0,0.0,0.0);
			PRTVector transparencia(0.0,0.0,0.0);
			
			if (deep>0)
			{
				if (main->BReflections && objeto->material->reflection>0.0) reflexion=main->ComputeReflection(r,colisiona,normal,deep,objeto);
				if (main->BRefractions && objeto->material->refraction>0.0) refraccion=main->ComputeRefraction(r,colisiona,normal,deep,objeto);
				if (main->BAlpha && objeto->material->alpha>0.0) transparencia=main->ComputeTransparence(r,colisiona,normal,deep,objeto);
			}
								
			PRTListMember *qluzesiterator=main->LightsList.first;
			PRTLight* qluzes;
			//PRTPointLight *luz;

			while (qluzesiterator!=NULL)
			{
				qluzes=((PRTLight*)(qluzesiterator->object));

				//luz=new PRTPointLight(qluzes->ComputeWhatPointLight(colisiona.punto),qluzes->color);
				//luz->attenuation0=(qluzes)->attenuation0;
				//luz->attenuation1=(qluzes)->attenuation1;
				//luz->attenuation2=(qluzes)->attenuation2;
				//luz->intensity=(qluzes)->intensity;
				
				PRTVector coloraux(0.0,0.0,0.0);

				/*int itera=1;
				if (qluzes->type==PRT_TRIANGLELIGHT)
				{
					int jatr=0;
					for (int b=0;b<itera;b++)
					{
						delete luz;
						luz=new PRTPointLight(qluzes->ComputeWhatPointLight(colisiona.punto),qluzes->color);
						if (( (colisiona.punto-luz->pos).Normalizado()*(((PRTTriangleLight*)(qluzes))->n1) >=0 )) //*TODO* en la direccion del triangle light
						{
							luz->attenuation0=(qluzes)->attenuation0;
							luz->attenuation1=(qluzes)->attenuation1;
							luz->attenuation2=(qluzes)->attenuation2;
							luz->intensity=(qluzes)->intensity;
							coloraux=coloraux+ComputeLightRay(r,colisiona,normal,objeto,luz);
							jatr++;
						}
					}
					if (jatr)
						coloraux=coloraux/jatr;
				}
				else
				{*/
					coloraux=qluzes->ComputeLightRay(r,colisiona,normal,objeto,main);//potencia que llega desde la luz al punto
				//}

				coloraux=main->ComputeReflectedLight(colisiona.point,
											   (colisiona.point-qluzes->ComputeWhatPointLight(colisiona.point)).Normalize(),
											   (r.orig-colisiona.point).Normalize(),
											   objeto,
											   normal,
											   coloraux);//cantidad de luz q se ve desde la camara en el punto

				color+=coloraux;
			
				//delete luz;
				
				qluzesiterator=qluzesiterator->next;
			}
			
			//color=PRTVector(0,0,0); //¿?

			// PHOTON MAPPING

			if (main->BIndirectLight)
			{
				PRTVector radian=ComputeRadiance(colisiona.point,normal,r.dir,objeto);//se le añade la luz indirecta
				PRTVector diff=objeto->material->color;
				color+=PRTVector(radian.x*diff.x,radian.y*diff.y,radian.z*diff.z);
			}

			//END PHOTON MAPPING

			PRTFloat refl=0.0; if (main->BReflections) refl=objeto->material->reflection;
			PRTFloat refr=0.0; if (main->BRefractions) refr=objeto->material->refraction;
			PRTFloat alph=0.0; if (main->BAlpha) alph=objeto->material->alpha;
			
			if (deep>0) 
				color=((color)*(1-alph)*(1-refl)*(1-refr))+(transparencia*alph)+(reflexion*refl)+(refraccion*refr);
						
			if (main->BTextures && objeto->material->texture!=NULL)
			{
				PRTVector janemore=objeto->material->texture->CalculateColor(objeto->ComputeTexCoord(colisionaini.point),colisiona.point-objeto->position);
				color.x=color.x*janemore.x;	color.y=color.y*janemore.y;	color.z=color.z*janemore.z;
			}

			color+=objeto->material->ambientcolor;
			
			if (color.x>1) color.x=1; if (color.y>1) color.y=1; if (color.z>1) color.z=1;
			if (color.x<0) color.x=0; if (color.y<0) color.y=0; if (color.z<0) color.z=0;
		}
	}
	
	// END RAY TRACING

	//main->numrayos++;
	return color;
}

PRTVector PRTPhotonMapping::PhotonTrace(PRTRay r,PRTVector fuerza,int deep)
{
	PRTIntersectPoint colisiona=main->FindNearestIntersection(r);

	if (colisiona.collision/* && (PRTWRandom()>0.5)*/) //ha colisionado  !!!RULETA RUSA se absorve
	{
		PRTObject *objeto=colisiona.object;

		//nueva fuerza en el punto de colision
		/*PRTFloat dis=colisiona.distancia;//(colisiona.punto-luz->pos).Module();
		PRTFloat attenuation0=0.4;
		PRTFloat attenuation1=0.02;
		PRTFloat attenuation2=0.002;

		PRTFloat attenuation=1/(attenuation0+attenuation1*dis+attenuation2*dis*dis); //parametros a cambiar
		if (attenuation>1)
			attenuation=1;
		
		fuerza=fuerza*attenuation;*/

		//if (fuerza.x>0.01 || fuerza.y>0.01 || fuerza.z>0.01) // no llega casi luz
		{
			//if (fuerza.x>1 && fuerza.y>1 && fuerza.z>1)
			//	fuerza=PRTVector(1,1,1);
		
			PRTVector normal=objeto->ComputeNormal(colisiona.point);

			//si estamos en double sided y la normal esta al reves, invertirla
			//if (BDoubleSided && r.dir*normal>0) //la normal esta en la misma direccion que el rayo
			//	normal=-normal;
			
			//componentes de reflexion y refracción
			PRTVector reflexion(0.0,0.0,0.0);
			PRTVector refraccion(0.0,0.0,0.0);
			PRTVector transparencia(0.0,0.0,0.0);
			if (deep>0)
			{
				// COMPONENTE REFLEXION
				if (main->BGiReflections && objeto->material->gireflection>0.0)
				{

					//APLICAR RULETA RUSA

					if ((PRTWRandom())<(objeto->material->gireflection))
					{
						//SE REFLEJA
						PRTVector direccion=r.dir.Reflection(normal);
						PRTVector nuevafuerza;
						nuevafuerza.x=objeto->material->color.x*fuerza.x*objeto->material->gireflection;
						nuevafuerza.y=objeto->material->color.y*fuerza.y*objeto->material->gireflection;
						nuevafuerza.z=objeto->material->color.z*fuerza.z*objeto->material->gireflection;
						for (int nm=0;nm<1;nm++) // se esparcen 
						{
							//PRTVector nuevadireccion2=direccion.Perturb((Random()-0.5)*50,(Random()-0.5)*50);
							PRTVector nuevadireccion2=direccion.Perturb(PRTFloat(PRTWRandom()-0.5)*50,PRTFloat(PRTWRandom()-0.5)*50);
							//PRTVector nuevadireccion2=direccion;
							reflexion=PhotonTrace(PRTRay(colisiona.point,nuevadireccion2),nuevafuerza,deep-1);
							//reflexion=PhotonTrace(PRTRay(colisiona.punto,(colisiona.punto-r.orig).Reflejo(normal)),fuerza*objeto->material->reflection,deep-1);
						}
					}
					else
					{
						//SE ABSORVE
					}
				}
				
				// COMPONENTE REFRACCION ¿?
				if (main->BGiRefractions && objeto->material->girefraction>0.0)
				{
					PRTVector normalaux2;
					PRTVector i=(colisiona.point-r.orig).Normalize();
					PRTFloat mu1=0,mu2=0;
					if (deep==2)
					{
						mu1=1.0;//rayo incidente aire
						mu2=2.0;//transmision cristal
						normalaux2=normal;
					}
					if (deep==1)
					{
						mu1=2.0;
						mu2=1.0;
						normalaux2=normal;//PRTVector(-normal.x,-normal.y,-normal.z);
					}
					PRTFloat div=mu1/mu2;
					//if (div!=1)
					{
						PRTFloat cosi=normalaux2*-i;
						PRTFloat discr=1+(div*div)*(cosi*cosi-1);
						PRTVector t=(i*div)+(normalaux2*((div*cosi)-PRTSqrt(discr)));
						//cuando -normalaux2*t no es real -> total internal reflection
						if (discr>=0) 
						{
							PRTVector nuevafuerza;
							nuevafuerza.x=objeto->material->color.x*fuerza.x*objeto->material->girefraction;
							nuevafuerza.y=objeto->material->color.y*fuerza.y*objeto->material->girefraction;
							nuevafuerza.z=objeto->material->color.z*fuerza.z*objeto->material->girefraction;
							//nuevafuerza=nuevafuerza.Perturb((PRTWRandom()-0.5)*50,(PRTWRandom()-0.5)*50);
							nuevafuerza=nuevafuerza.Perturb(PRTFloat(PRTRandom()-0.5)*50,PRTFloat(PRTRandom()-0.5)*50);
							refraccion=PhotonTrace(PRTRay(colisiona.point,t.Normalize()),nuevafuerza,deep-1);
						}
					}
				}
						
			
				// COMPONENTE TRANSPARENCIA ¿?
				if (main->BGiAlpha && objeto->material->gialpha>0.0)
				{
					PRTVector nuevafuerza;
					nuevafuerza.x=objeto->material->color.x*fuerza.x*objeto->material->gialpha;
					nuevafuerza.y=objeto->material->color.y*fuerza.y*objeto->material->gialpha;
					nuevafuerza.z=objeto->material->color.z*fuerza.z*objeto->material->gialpha;
					transparencia=PhotonTrace(PRTRay(colisiona.point,r.dir),nuevafuerza,deep-1);
				}

				// SOMBRA, con los demas que colisione (estaran detras) sombra
				
				//POR DELANTE
		//		PRTListMember *o7=ObjectsList.first;
		//		PRTIntersectPoint aux7; //colision temporal
		//		while (o7!=NULL)
		//		{
		//			aux7=r.Intersect((PRTObject*)(o7->object),!BDoubleSided);
					//numintertest+=r.numrayintertest;
		//			if (aux7.colision && ((PRTObject*)o7)!=objeto)// *TODO* error de precision (objeto delante de otro)
		//			{
						//con lo mio
						/*Photon *aux;
						aux=new Photon;
						aux->punto=aux7.punto;
						aux->fuerza=-PRTVector(0.01,0.01,0.01);
						aux->type=SHADOW;
						//aux->objeto=o7;
						PhotonIllumination.AddAtEnd(aux);	*/

						//con KdTree
						/*Photon *photon=new Photon();
						photon->position=Vec3f(aux7.punto.x,aux7.punto.y,aux7.punto.z);
						photon->power=-Vec3f(0.01f,0.01f,0.01f);
						kdtree->insert(photon);

						numphotones++;
						*/
		//			}
					
					//pos++;
					//o=(PRTObject*)ObjectsList.GetAtPos(pos);
		//			o7=o7->next;
		//		}
				//POR DETRAS
		//		PRTRay raux2(r.orig+(r.dir*PRTINFINITO),-r.dir);
		//		PRTListMember *o72=ObjectsList.first;
		//		PRTIntersectPoint aux72; //colision temporal
		//		while (o72!=NULL)
		//		{
		//			aux72=raux2.Intersect((PRTObject*)(o72->object),!BDoubleSided);
					//numintertest+=r.numrayintertest;
		//			if (aux72.colision && ((PRTObject*)o72)!=objeto)// *TODO* error de precision (objeto delante de otro)
		//			{
						/*Photon *aux;
						aux=new Photon;
						aux->punto=aux72.punto;
						aux->fuerza=-PRTVector(0.01,0.01,0.01);
						aux->type=SHADOW;
						//aux->objeto=o7;
						PhotonIllumination.AddAtEnd(aux);	*/

						//con KdTree
						/*Photon *photon=new Photon();
						photon->position=Vec3f(aux72.punto.x,aux72.punto.y,aux72.punto.z);
						photon->power=-Vec3f(0.01f,0.01f,0.01f);
						kdtree->insert(photon);

						numphotones++;*/
		//			}
					
					//pos++;
					//o=(PRTObject*)ObjectsList.GetAtPos(pos);
		//			o72=o72->next;
		//		}

			}
		
			//ahora se sabe el punto de colision y la distancia recorrida por el photon
			
			if (deep!=2 /*&& !(BGiRefractions && objeto->material->girefraction>0.0 && deep==1)*/)
			{
				
				//con lo mio

				/*Photon *aux;
				aux=new Photon;
				aux->punto=colisiona.punto;
				aux->fuerza=fuerza;
				aux->type=INDIRECT;
				PhotonIllumination.AddAtEnd(aux);	*/

				//con kdtrees
				
				/*Photon *photon=new Photon;
				photon->punto=colisiona.punto;
				photon->fuerza=fuerza;
				photon->in=r.dir;
				kdtree->Add(photon,photon->punto);*/

				PRTFloat fuer[3]={fuerza.x,fuerza.y,fuerza.z};
				PRTFloat col[3]={colisiona.point.x,colisiona.point.y,colisiona.point.z};
				PRTFloat dir[3]={r.dir.x,r.dir.y,r.dir.z};

				photons->Store(fuer,col,dir);

				numphotones++;
			}
			/*if (deep==5)
			{
				Photon *aux;
				aux=new Photon;
				aux->punto=colisiona.punto;
				aux->fuerza=fuerza;
				aux->type=DIRECT;
				PhotonIllumination.AddAtEnd(aux);	
				numphotones++;
			}*/
		}
	}
	
	//main->numintertest+=r.numrayintertest;
	
	return fuerza;
}

bool PRTPhotonMapping::BuildPhotonIllumination(void)
{
	if (photons!=NULL)
	{
		photons->Free();
		delete photons;
		photons=new PRTPhotonMap(numphotones);
	}
	if (main->BIndirectLight) 
	{
		//la intensidad de todas las luces (suma)
		PRTFloat probtodasluces=0;
		PRTListMember *o=main->LightsList.first;
		while (o!=NULL)
		{
			probtodasluces+=((PRTPointLight*)(o->object))->color*PRTVector(((PRTPointLight*)(o->object))->intensity,((PRTPointLight*)(o->object))->intensity,((PRTPointLight*)(o->object))->intensity);
			o=o->next;
		}
		o=main->LightsList.first;

		int total=numphotones;//mayor q 0 , numero de fotones a lanzar
		numphotones=0;
        while (o!=NULL)
		{

			//LAS CONSIDERO TODAS PUNTUALES DE MOMENTO

			PRTPointLight *luz;
			PRTLight* qluzes;
			qluzes=((PRTLight*)(o->object));
			PRTVector puntocero(0,0,0);
			luz=new PRTPointLight(qluzes->ComputeWhatPointLight(puntocero),qluzes->color); //*TODO*
			luz->attenuation0=(qluzes)->attenuation0;
			luz->attenuation1=(qluzes)->attenuation1;
			luz->attenuation2=(qluzes)->attenuation2;
			luz->intensity=(qluzes)->intensity;

            //probabilidad de la luz o
			PRTFloat probluz=(luz->color*PRTVector(luz->intensity,luz->intensity,luz->intensity))/probtodasluces;
			int totalestaluz=(int)(total*probluz);
			int fallo=0; //para controlar situaciones donde no se pueden guardar los fotones
			int numantes;
			while(numphotones<totalestaluz)
			{
				if (fallo>5000)
					return false;// no se ha podido completar
				int itera=1; //*TODO*
				PRTVector normal(0,0,0);
				if (qluzes->type==PRT_TRIANGLELIGHT)
				{
					for (int b=0;b<itera;b++)
					{
						delete luz;
						luz=new PRTPointLight(qluzes->ComputeWhatPointLight(puntocero),qluzes->color); //*TODO*
						luz->attenuation0=(qluzes)->attenuation0;
						luz->attenuation1=(qluzes)->attenuation1;
						luz->attenuation2=(qluzes)->attenuation2;
						luz->intensity=(qluzes)->intensity;
						normal=((PRTTriangleLight*)(qluzes))->n1; //*TODO*
					}
				}
				
				PRTFloat r1,r2,r3;
				//PRTFloat max=(PRTFloat)RAND_MAX;
				do{
					r1=PRTFloat(PRTWRandom());//(PRTFloat)rand()/max;
					r2=PRTFloat(PRTWRandom());//(PRTFloat)rand()/max;
				
				/*PRTFloat sinr1=PRTSin(6.283185307179586476925286766559*r1);
				PRTFloat sinr2=PRTSin(6.283185307179586476925286766559*r2);
				PRTFloat cosr1=PRTCos(r1*6.283185307179586476925286766559);
				PRTFloat cosr2=PRTCos(r2*6.283185307179586476925286766559);
				PRTVector nuevadireccion(cosr1*sinr2,cosr1*cosr2,sinr1);*/
				
				//nuevadireccion=nuevadireccion.Perturbar(((((PRTFloat)rand())/((PRTFloat)RAND_MAX))-0.5)*1,((((PRTFloat)rand())/((PRTFloat)RAND_MAX))-0.5)*1);

					r3=PRTFloat(PRTWRandom());//(PRTFloat)rand()/max;
				}while((r1*r1+r2*r2+r3*r3)>1 || ((qluzes->type==PRT_TRIANGLELIGHT) && ((PRTVector(PRTFloat(r1*2.0-1.0),PRTFloat(r2*2.0-1.0),PRTFloat(r3*2.0-1.0)).Normalize()*normal)<=0)));//*TODO*
				PRTVector nuevadireccion(PRTFloat(r1*2.0-1.0),PRTFloat(r2*2.0-1.0),PRTFloat(r3*2.0-1.0));
				nuevadireccion=nuevadireccion.Normalize();
				
				PRTVector nuevaintensidad=(luz->color*luz->intensity); //ARGG!!! *TODO*
				//nuevaintensidad=nuevaintensidad*4000;
				numantes=numphotones;
				PRTVector penemore=PhotonTrace(PRTRay(luz->pos,nuevadireccion.Normalize()),nuevaintensidad,2);
				if (numphotones==numantes)
					fallo++;
				else
					fallo=0;
			}
			//kdtree->ScalePhotonPower((PRTFloat)((PRTFloat)1/(PRTFloat)totalestaluz)); //¿?
			photons->ScalePhotonPower(scalephoton/*/(PRTFloat)numphotones*/);//?¿?¿
			o=o->next;
			numphotones=0;
			delete luz;
		}
		//kdtree->Balance();
		photons->Balance();
		numphotones=photons->StoredPhotons();
	}
	return true; // se lanzaron todos los fotones
}

PRTVector PRTPhotonMapping::ComputeRadiance(PRTVector punto,PRTVector n,PRTVector dir, PRTObject* o)
{
/*	if (!kdtree->Empty())
	{
		PRTFloat radio=1000.0;//radio maximo
		PRTDinamicList *dl;
		
		
		dl=kdtree->FindNearest(p,groupphotones,&radio);
		
		Photon *qPhoton;
		PRTVector power(0.0,0.0,0.0);

		bool si=false;
		PRTListMember* lm;
		lm=dl->first;

		//para flitro
		PRTFloat k=1;

		while (lm!=NULL) 
		
		{ //sin filtro
			qPhoton=((Photon*)(lm->object));
			
			PRTFloat BRDF=o->material->ComputeBRDF(p,
								qPhoton->in,
								-dir,
								o->material->BRDFp1,o->material->BRDFp2,o->material->BRDFp3,o->material->BRDFp4,o->material->BRDFp5,o->material->BRDFp6,o->material->BRDFp7);
			//PRTFloat BRDF=1;
			//PRTFloat divisor=(radio*radio*3.1415926535897932384626f);
			//PRTFloat division=BRDF/divisor;
			//power=power+(qPhoton->fuerza*division);
			power=power+(qPhoton->fuerza*BRDF);
		
			if (((((Photon*)(lm->object))->punto)-p).Module()<5)
			{
				power=PRTVector(1,0,0);
				break;
			}

			lm=lm->next;
		}

		//radio=10;

		if (power!=PRTVector(1,0,0))
		{
			PRTFloat divisor=(radio*radio*3.1415926535897932384626f);
			power=power/divisor;
		}

//		{ //con filtro
//			qPhoton=((Photon*)(lm->object));
//			
//			PRTFloat BRDF=o->material->ComputeBRDF(p,
//								qPhoton->in,
//								-dir,
//								o->material->BRDFp1,o->material->BRDFp2,o->material->BRDFp3,o->material->BRDFp4,o->material->BRDFp5,o->material->BRDFp6,o->material->BRDFp7);
//			//BRDF=1;
//			//PRTFloat divisor=(radio*radio*3.1415926535897932384626f);
//			//PRTFloat division=BRDF/divisor;
//			PRTFloat wp=1-((qPhoton->punto-p).Module()/(k*radio));
//			if (wp<0) wp=0;
//			power=power+(qPhoton->fuerza*BRDF*wp);
//			lm=lm->next;
//		}
//		power=power/((1-(2/(3*k)))*3.1415926535897932384626f*radio*radio);
//		//fin filtro


		//dl->Free();

//		PRTListMember *aux;
//		aux=dl->first;
//		while (aux!=NULL)
//		{
//			PRTListMember *aux2=aux;
//			aux=aux->next;
//			delete ((Photon*)aux2->object);
//			//delete aux2;
//		}
//		dl->first=NULL;
//		dl->last=NULL;
//		dl->num=0;
	
		delete dl;

		return PRTVector(power[0],power[1],power[2]);
	}
	return PRTVector(0.0,0.0,0.0);
*/

	if (!photons->Empty())
	{

		PRTFloat irrad[3];
		PRTFloat pos[3];pos[0]=punto.x;pos[1]=punto.y;pos[2]=punto.z;
		PRTFloat normal[3];normal[0]=n.x;normal[1]=n.y;normal[2]=n.z;
		PRTFloat maxDist=1000;
		int nPhotons=groupphotones;
   
		irrad[0]=irrad[1]=irrad[2]=0.0;

		PRTNearestPhotons np;
		//np.dist2=new PRTFloat[nPhotons+1];
		//np.index=(const Photon**)(new(Photon*))[nPhotons + 1];
		np.dist2=(PRTFloat*)malloc(sizeof(PRTFloat)*(nPhotons+1));
		np.index=(const PRTPhoton**)malloc(sizeof(PRTPhoton*)*(nPhotons+1));

		np.pos[0]=pos[0];np.pos[1]=pos[1];np.pos[2]=pos[2];
		np.max=nPhotons;
		np.found=0;
		np.gotHeap=0;
		np.dist2[0]=maxDist*maxDist;

		photons->LocatePhotons(&np,1);

		if (np.found<8)
		{
			delete []np.dist2;
			delete []np.index;
			return PRTVector(0,0,0);
		}

		PRTFloat pdir[3];

		for (int i=1;i<=np.found;i++)
		{
			const PRTPhoton *p=np.index[i];
			photons->PhotonDir(pdir,p);
			//if ((pdir[0]*normal[0]+pdir[1]*normal[1]+pdir[2]*normal[2])<0.0f)
			{
				PRTFloat BRDF;
				BRDF=1;
				/*
				//entrada,salida,normal
				BRDF=o->material->ComputeBRDF(PRTVector(pdir[0],pdir[1],pdir[2]),
								-dir,
								-PRTVector(pdir[0],pdir[1],pdir[2]),
								o->material->BRDFp1,o->material->BRDFp2,o->material->BRDFp3,o->material->BRDFp4,o->material->BRDFp5,o->material->BRDFp6,o->material->BRDFp7);
				*/

				irrad[0]+=p->power[0]*BRDF;
				irrad[1]+=p->power[1]*BRDF;
				irrad[2]+=p->power[2]*BRDF;
			}
		}

		/*for (int n=0;n<np.found;n++)
			if (np.dist2[n]<20)
			{
				delete [] np.dist2;
				delete [] np.index;
				return PRTVector(1,0,0);
			}
		*/
		const PRTFloat tmp=(PRTFloat)((0.31830988618379067153776752674503)/(np.dist2[0])); // estimate of density
		irrad[0]*=tmp;
		irrad[1]*=tmp;
		irrad[2]*=tmp;

		delete []np.dist2;
		delete []np.index;

		return PRTVector(irrad[0],irrad[1],irrad[2]);
	}
	return PRTVector(0,0,0);
}
