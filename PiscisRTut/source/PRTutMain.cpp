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

#include "PRTutMain.h"

#ifdef _WIN32
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

PRTutMain::PRTutMain(void)
{
	subwidth=PRTTexCoord(0,PRTINFINITE);
	subheight=PRTTexCoord(0,PRTINFINITE);
	//SCENE
	scenename="";
	ya=false;
	ahora=false;
	antialiasing=PRT_ANTIALIASING_NONE;
	deep=0;
	itera=1;
	octreesdeep=1;
	octreesnumonleaf=50;
	stereo=false;
	stereowidth=1;
	//CSG
	incsg=0;
	csgo1=NULL;
	csgo2=NULL;
	//ROTATE,TRANSLATE,SCALE
	inrts=0;
	//PHOTONMAP
	photonmap=false;
	numphotones=1000;
	groupphotones=10;
	scalephoton=1;
	//PATCHES
	patches=false;
	//ESTADISTICAS
	t=0;
	tinicio=0;
	tfinal=1;
	tpaso=1;
	//THREADS
	hx=0;
	hy=0;
	hilos=2;
	esperar=true;
	//DOF
	dofpoint=PRTVector(0,0,0);
	dofaperture=PRTFloat(0.04);
	dof=false;

	PRTWRandomInit(3);
}

bool PRTutMain::PRTutParse(string fichero,string escena,PRTMain* prtmain,unsigned int &ancho2,unsigned int &alto2,unsigned int &antialiasing2,double tiempo)
{
#ifdef __WIN32
	Py_Initialize();
#endif

	trazador=prtmain;
	t=tiempo;
	if (escena!="")
		scenename=escena;
		
	textures.clear();
	materials.clear();
	variables.clear();
	trazador->Reset();

	char buf[BUFSIZ];
	FILE* fich;
	fich=fopen((char*)fichero.c_str(),"r");
	int done;
	int depth = 0;
	
	//XML_SetUserData(parser, &depth);
	//XML_SetElementHandler(parser, startElement, endElement);
	do 
	{
		size_t len = fread(buf, 1, sizeof(buf), fich);
		done = len < sizeof(buf);
		
		if (!XML_Parse(buf, len, done)) 
		{
			fprintf(stderr,"%s at line %d\n",XML_ErrorString(XML_GetErrorCode()),XML_GetCurrentLineNumber());
			exit(1);
		}
	} while (!done);
	//XML_ParserFree(parser);

	// si ya=false es que no se ha encontrado la escena requerida

	ancho2=ancho;
	alto2=alto;
	antialiasing2=antialiasing;

	if (photonmap)
	{
		//delete trazador->render; //no hace falta liberar su memoria
		trazador->render=new PRTPhotonMapping(trazador);
	}
	else if (patches)
	{
		//delete trazador->render; //no hace falta liberar su memoria
		trazador->render=new PRTPatches(trazador);
	}
	
	if (photonmap && trazador->render->type==PRT_PHOTONMAPPING)
	{
		trazador->BIndirectLight=true;
		((PRTPhotonMapping*)trazador->render)->numphotones=numphotones;
		((PRTPhotonMapping*)trazador->render)->groupphotones=groupphotones;
		((PRTPhotonMapping*)trazador->render)->scalephoton=scalephoton;
		bool siono=((PRTPhotonMapping*)trazador->render)->BuildPhotonIllumination();
		if (!siono)
			fprintf(stderr,"The photon map couldn't be builded\n");
	}
		
	if (patches && trazador->render->type==PRT_PATCHES)
	{
		trazador->BIndirectLight=true;
		((PRTPatches*)trazador->render)->CalculatePatches();
		((PRTPatches*)trazador->render)->CalculateLighting();
	}
		
	//OCTREES
	trazador->BuildOctrees(octreesdeep,octreesnumonleaf);

	//
	if (camaraup==PRTVector())
			camaraup=PRTVector(0,1,0);

	N=(camarafin-camarapos).Normalize();	// -z de la camara
	U=(N^camaraup).Normalize();				// x de la camara
	V=(N^U).Normalize();					// -y de la camara
	fovrad=PRTGradToRad(fov);
	Xinc=U*((2.0*tan(fovrad/2.0))/alto);
	Yinc=V*((2.0*tan(fovrad/2.0))/alto);

	//parseexpr("import dumpmods\ndumpmods.analyze_modules(\"D:\\PiscisRT\\Elements\\out\")");
	
#ifdef __WIN32
	Py_Finalize();
#endif

	return true;
}

PRTVector PRTutMain::colorpixel(PRTVector aux)
{
	if (dof)
	{
		PRTVector hacia=(camarafin-camarapos).Normalize();
		PRTVector right=(hacia^camaraup).Normalize();
		PRTVector pos=camarapos;
			
		PRTFloat r1=PRTWRandom()*dofaperture*PRTFloat(0.5);
		PRTFloat r2=PRTWRandom();

		PRTFloat ang=2*PRTPI*r2;
		PRTFloat r=PRTSqrt(r1);

		PRTVector punto=pos+right*PRTCos(ang)*r+camaraup*PRTSin(ang)*r; // punto de salida
		PRTFloat tang=PRTTan(PRTACos(hacia%aux));

		PRTFloat aux1=(camarafin.x-camarapos.x);
		PRTFloat aux2=(camarafin.y-camarapos.y);
		PRTFloat distancefptocamray=(((camarapos.y-camarafin.y)*dofpoint.x)+(aux1*dofpoint.y)+(camarapos.x*camarafin.y-camarafin.x*camarapos.y))/PRTSqrt(aux1*aux1+aux2*aux2);
		PRTFloat aux3=(dofpoint-camarapos).Module();
		PRTVector dofdistance=PRTSqrt(aux3*aux3-distancefptocamray*distancefptocamray); // focal lenght, distancia de camara a focal plane
		PRTVector focalpoint2=pos+aux*(PRTSqrt(dofdistance*dofdistance+tang*tang));

		return trazador->RayTrace(PRTRay(punto,(focalpoint2-punto).Normalize()),deep);
	}
	else
	{
		return trazador->RayTrace(PRTRay(camarapos,aux.Normalize()),deep);	
	}
}

PRTVector PRTutMain::jitter(int x, int y,unsigned int itera)
{
	itera=itera%16;
	switch (itera)
	{
		case 1:	x=x-PRTFloat(0.125);y=y-PRTFloat(0.125);break;
		case 2:	x=x+PRTFloat(0.375);y=y-PRTFloat(0.375);break;
		case 3:	x=x+PRTFloat(0.125);y=y+PRTFloat(0.375);break;
		case 4:	x=x-PRTFloat(0.375);y=y+PRTFloat(0.125);break;
		case 5:	x=x+PRTFloat(0.125);y=y-PRTFloat(0.375);break;
		case 6:	x=x-PRTFloat(0.375);y=y-PRTFloat(0.375);break;
		case 7:	x=x-PRTFloat(0.375);y=y+PRTFloat(0.375);break;
		case 8:	x=x+PRTFloat(0.125);y=y-PRTFloat(0.125);break;
		case 9:	x=x+PRTFloat(0.375);y=y+PRTFloat(0.125);break;
		case 10:x=x-PRTFloat(0.125);y=y-PRTFloat(0.375);break;
		case 11:x=x-PRTFloat(0.125);y=y+PRTFloat(0.375);break;
		case 12:x=x+PRTFloat(0.375);y=y-PRTFloat(0.125);break;
		case 13:x=x+PRTFloat(0.125);y=y+PRTFloat(0.125);break;
		case 14:x=x+PRTFloat(0.375);y=y+PRTFloat(0.375);break;
		case 15:x=x-PRTFloat(0.125);y=y+PRTFloat(0.125);break;
		case 16:x=x-PRTFloat(0.375);y=y-PRTFloat(0.125);break;
	}

	x=x+(PRTWRandom()*PRTFloat(0.25)-PRTFloat(0.125));
	y=y+(PRTWRandom()*PRTFloat(0.25)-PRTFloat(0.125));
	
	return (N+(Yinc*(PRTFloat(2*y+1-alto)*PRTFloat(0.5)))+(Xinc*(PRTFloat(2*x+1-ancho)*PRTFloat(0.5))));
}

PRTVector PRTutMain::PRTutRenderPixel(int x, int y)
{
	if (PRTFloat(x)<subwidth.u || PRTFloat(x)>subwidth.v || PRTFloat(y)<subheight.u || PRTFloat(y)>subheight.v)
		return PRTVector(1,1,1);

	/////////////// RENDERIZA

	PRTVector jar(0,0,0);

	for(unsigned int nb=1;nb<=itera;nb++)
	{
		PRTVector aux=(N+(Yinc*(PRTFloat(2*y+1-alto)*PRTFloat(0.5)))+(Xinc*(PRTFloat(2*x+1-ancho)*PRTFloat(0.5))));
		jar=jar+colorpixel(aux);
	}
							
	jar=jar/PRTFloat(itera);

	return jar;
}

PRTVector PRTutMain::PRTutFilterPixel(int x,int y)
{
	if (PRTFloat(x)<subwidth.u || PRTFloat(x)>subwidth.v || PRTFloat(y)<subheight.u || PRTFloat(y)>subheight.v)
		return PRTVector(1,1,1);

	PRTVector jar(0,0,0);

	for(unsigned int nb=1;nb<=itera;nb++)
	{
		if (antialiasing==PRT_ANTIALIASING_JITTER)
		{
			jar=jar+colorpixel(jitter(x,y,itera));
		}
		else if(antialiasing==PRT_ANTIALIASING_5)
		{
			//-------------------------------------------------------------------
			//
			//						1/5
			// antialiasing		1/5 1/5 1/5
			//						1/5
			
			PRTVector auxx,auxy;
			PRTVector color,colorx05,colorxm05,colory05,colorym05;
			auxy=Yinc*(PRTFloat(2*y+1-alto)*PRTFloat(0.5));
			auxx=Xinc*(PRTFloat(2*x+1-ancho)*PRTFloat(0.5));
			
			color=colorpixel(N+auxy+auxx);
			colorx05=colorpixel(N+auxy+(Xinc*(PRTFloat(2*(x+PRTFloat(0.5))+1-ancho)*PRTFloat(0.5))));
			colorxm05=colorpixel(N+auxy+(Xinc*(PRTFloat(2*(x-PRTFloat(0.5))+1-ancho)*PRTFloat(0.5))));
			colory05=colorpixel(N+(Yinc*(PRTFloat(2*(y+PRTFloat(0.5))+1-alto)*PRTFloat(0.5)))+auxx);
			colorym05=colorpixel(N+(Yinc*(PRTFloat(2*(y-PRTFloat(0.5))+1-alto)*PRTFloat(0.5)))+auxx);
			
			jar+=(color+colorx05+colorxm05+colory05+colorym05)*PRTFloat(0.2);
			
			//-------------------------------------------------------------------
		}
		else if(antialiasing==PRT_ANTIALIASING_9)
		{
			//-------------------------------------------------------------------
			//
			//					1/9 1/9 1/9
			// antialiasing		1/9 1/9 1/9
			//					1/9 1/9 1/9
			
			PRTVector auxx,auxy;
			PRTVector color,colorx05,colorxm05,colory05,colorym05,colorx05y05,colorx05ym05,colorxm05y05,colorxm05ym05;
			auxy=Yinc*(PRTFloat(2*y+1-alto)*PRTFloat(0.5));
			auxx=Xinc*(PRTFloat(2*x+1-ancho)*PRTFloat(0.5));
			
			color=colorpixel(N+auxy+auxx);
			colorx05=colorpixel(N+(Xinc*(PRTFloat(2*(x+PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+auxy);
			colorxm05=colorpixel(N+(Xinc*(PRTFloat(2*(x-PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+auxy);
			colory05=colorpixel(N+auxx+(Yinc*(PRTFloat(2*(y+PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			colorym05=colorpixel(N+auxx+(Yinc*(PRTFloat(2*(y-PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			
			colorx05y05=colorpixel(N+(Xinc*(PRTFloat(2*(x+PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+(Yinc*(PRTFloat(2*(y+PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			colorx05ym05=colorpixel(N+(Xinc*(PRTFloat(2*(x+PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+(Yinc*(PRTFloat(2*(y-PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			colorxm05y05=colorpixel(N+(Xinc*(PRTFloat(2*(x-PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+(Yinc*(PRTFloat(2*(y+PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			colorxm05ym05=colorpixel(N+(Xinc*(PRTFloat(2*(x-PRTFloat(0.5))+1-ancho)*PRTFloat(0.5)))+(Yinc*(PRTFloat(2*(y-PRTFloat(0.5))+1-alto)*PRTFloat(0.5))));
			
			jar+=(color+colorx05+colorxm05+colory05+colorym05+colorx05y05+colorx05ym05+colorxm05y05+colorxm05ym05)*PRTFloat(0.1111111111111111111111);

			//-------------------------------------------------------------------
		}
	}
							
	jar=jar/PRTFloat(itera);

	return jar;
}

PRTutMain::~PRTutMain(void)
{
}

double PRTutMain::parseexpr(const char* c) // con variable global t
{
#ifdef _WIN32
	char aux[255];
	//Py_Initialize();
	PyObject* globals = PyModule_GetDict(PyImport_ImportModule("__main__"));
	PyObject* locals = PyDict_New();
	sprintf(aux,"import math\n");
	sprintf(aux,"%st=%f\n",aux,t);
	
	typedef pair <string,string> auxpair;
	list<auxpair>::const_iterator it=variables.begin();
	while (it!=variables.end())
	{
		sprintf(aux,"%s%s=%s\n",aux,((string)(*it).first).c_str(),((string)(*it).second).c_str());
		it++;
	}
	
	PyRun_SimpleString(aux);
	sprintf(aux,"%s",c);

	PyObject *objeto=PyRun_String(aux,Py_eval_input,globals,locals);
	//Py_Finalize();
	return PyFloat_AsDouble(objeto);
#else
	// unsuported python 
	return atof(c);
#endif
}

int PRTutMain::parseint(const char* c)
{
	if (!strcmp(c,"PRT_ANTIALIASING_NONE"))			return 101;
	if (!strcmp(c,"PRT_ANTIALIASING_JITTER"))		return 102;
	if (!strcmp(c,"PRT_ANTIALIASING_5"))			return 103;
	if (!strcmp(c,"PRT_ANTIALIASING_9"))			return 104;
	
	if (!strcmp(c,"PRT_OBJECT_TRIANGLE"))		return 1001;
	if (!strcmp(c,"PRT_OBJECT_SPHERE"))			return 1002;
	if (!strcmp(c,"PRT_OBJECT_QUADRIC"))		return 1003;
	if (!strcmp(c,"PRT_OBJECT_CYLINDER"))		return 1004;
	if (!strcmp(c,"PRT_OBJECT_CIRCLE"))			return 1005;
	if (!strcmp(c,"PRT_OBJECT_PLANE"))			return 1006;
	if (!strcmp(c,"PRT_OBJECT_QUARTIC"))		return 1007;
	if (!strcmp(c,"PRT_OBJECT_CONE"))			return 1008;
	if (!strcmp(c,"PRT_OBJECT_LINE"))			return 1009;
	if (!strcmp(c,"PRT_OBJECT_CSG"))			return 1010;
	if (!strcmp(c,"PRT_OBJECT_QUATJULIA"))		return 1011;
	if (!strcmp(c,"PRT_OBJECT_MESH"))			return 1012;
	if (!strcmp(c,"PRT_OBJECT_POLYGON"))		return 1013;
		
	if (!strcmp(c,"PRT_POINTLIGHT"))			return 2001;
	if (!strcmp(c,"PRT_TRIANGLELIGHT"))			return 2002;
	if (!strcmp(c,"PRT_SPHERELIGHT"))			return 2003;
	if (!strcmp(c,"PRT_DIRECTIONALLIGHT"))		return 2004;

	if (!strcmp(c,"PRT_QUADRIC_CYLINDER"))		return 3001;
	if (!strcmp(c,"PRT_QUADRIC_CONE"))			return 3002;
	if (!strcmp(c,"PRT_QUADRIC_ELLIPSOID"))		return 3003;
	if (!strcmp(c,"PRT_QUADRIC_PARABOLOID"))	return 3004;
	if (!strcmp(c,"PRT_QUADRIC_HYPERBOLOID1"))	return 3005;
	if (!strcmp(c,"PRT_QUADRIC_HYPERBOLOID2"))	return 3006;

	if (!strcmp(c,"PRT_QUARTIC_TORUS"))			return 4001;

	if (!strcmp(c,"BRDF_PHONG"))				return 5001;
	if (!strcmp(c,"BRDF_COSINE_LOBE"))			return 5002;
	if (!strcmp(c,"BRDF_DIFFUSE"))				return 5003;
	if (!strcmp(c,"BRDF_TS"))					return 5004;
	if (!strcmp(c,"BRDF_BM"))					return 5005;
	if (!strcmp(c,"BRDF_ON"))					return 5006;
	if (!strcmp(c,"BRDF_HTSG"))					return 5007;

	if (!strcmp(c,"PRT_CSG_UNION"))				return 6001;
	if (!strcmp(c,"PRT_CSG_DIFERENCE"))			return 6002;
	if (!strcmp(c,"PRT_CSG_INTERSECTION"))		return 6003;

	if (!strcmp(c,"PRT_RAYTRACING"))			return 7001;
	if (!strcmp(c,"PRT_PHOTONMAPPING"))			return 7002;
	if (!strcmp(c,"PRT_PATCHES"))				return 7003;

	if (!strcmp(c,"PRT_TEXTURE_BITMAP"))		return 8001;
	if (!strcmp(c,"PRT_TEXTURE_PERLIN"))		return 8002;

	if (!strcmp(c,"PRT_TEXTURE_PERLIN_CLOUD"))	return 10001;
	if (!strcmp(c,"PRT_TEXTURE_PERLIN_WOOD"))	return 10002;
	if (!strcmp(c,"PRT_TEXTURE_PERLIN_MARBLE"))	return 10003;
	
	return (int)parseexpr(c);//atoi(c);
}

PRTVector PRTutMain::parsevector(char* c)
{
	PRTFloat f1=parseexpr(strtok(c,","));//strtod(strtok(c,","),NULL);
	PRTFloat f2=parseexpr(strtok(NULL,","));//strtod(strtok(NULL,","),NULL);
	PRTFloat f3=parseexpr(strtok(NULL,"\0"));//strtod(strtok(NULL,"\0"),NULL);
	return PRTVector(f1,f2,f3);
}

PRTQuaternion PRTutMain::parsequat(char* c)
{
	PRTFloat f1=parseexpr(strtok(c,","));//strtod(strtok(c,","),NULL);
	PRTFloat f2=parseexpr(strtok(NULL,","));//strtod(strtok(NULL,","),NULL);
	PRTFloat f3=parseexpr(strtok(NULL,","));//strtod(strtok(NULL,","),NULL);
	PRTFloat f4=parseexpr(strtok(NULL,"\0"));//strtod(strtok(NULL,"\0"),NULL);
	return PRTQuaternion(f1,f2,f3,f4);
}

PRTFloat PRTutMain::parseprtfloat(const char* c)
{	
	return parseexpr(c);//strtod(c,NULL);
}

PRTTexCoord PRTutMain::parsecoord(char* c)
{
	PRTFloat f1=parseexpr(strtok(c,","));//strtod(strtok(c,","),NULL);
	PRTFloat f2=parseexpr(strtok(NULL,"\0"));//strtod(strtok(NULL,"\0"),NULL);
	return PRTTexCoord(f1,f2);
}

bool PRTutMain::parsebool(const char* c)
{
	return (strcmp(c,"true")==0);
}

void PRTutMain::startElement(const XML_Char* name, const XML_Char** atts)
{
	int i;

	//
	// ESCENA
	//

	if (!ya && !strcmp(name,"scene"))
	{
		i=0;
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"name"))
			{
				if (scenename=="")
				{
					ahora=true;
				}
				else if (atts[i+1]==scenename)
					ahora=true;
			}
			else if (!strcmp(atts[i],"width"))			ancho=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"height"))			alto=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"subwidth"))			subwidth=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"subheight"))			subheight=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"camerapos"))		camarapos=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"cameraend"))		camarafin=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"cameraup"))		camaraup=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"camerafov"))		fov=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"deep"))			deep=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"photonmap"))		photonmap=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"numphotones"))	numphotones=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"groupphotones"))	groupphotones=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"scalephoton"))	scalephoton=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"BShadows"))		trazador->BShadows=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"shadowpass"))		trazador->shadowpass=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"antialiasing"))		antialiasing=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"rendersamples"))		itera=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"BOctrees"))		trazador->BOctrees=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"octreesdeep"))		octreesdeep=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"octreesnumonleaf"))	octreesnumonleaf=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"BBump"))		trazador->BBump=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BDoubleSided"))		trazador->BDoubleSided=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BTextures"))		trazador->BTextures=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"stereo"))		{ stereo=parsebool(atts[i+1]); if (stereo) ancho*=2; }
			else if (!strcmp(atts[i],"stereowidth"))		stereowidth=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"patches"))		patches=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BSpecular"))		trazador->BSpecular=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BReflections"))		trazador->BReflections=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BRefractions"))		trazador->BRefractions=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BAlpha"))		trazador->BAlpha=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"tbegin"))		tinicio=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"tend"))		tfinal=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"tbreak"))		tpaso=parseprtfloat(atts[i+1]);
			//else if (!strcmp(atts[i],"avi"))		hazavi=parsebool(atts[i+1]);
			//else if (!strcmp(atts[i],"consola"))		bconsola=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"BTransformations"))		trazador->BTransformations=parsebool(atts[i+1]);
			else if (!strcmp(atts[i],"dofpoint"))		dofpoint=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"dofaperture"))		dofaperture=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"dof"))		dof=parsebool(atts[i+1]);
			i+=2;
		}
	}

	//
	// VARIABLES
	//

	else if (!ya && ahora && !strcmp(name,"variables"))
	{
		i=0;
		
		while (atts[i]!=NULL)
		{
			typedef pair <string,string> auxpair;
			variables.push_back(auxpair(atts[i],atts[i+1]));
			i+=2;
		}
	}

	//
	// MATERIAL
	//

	else if (!ya && ahora && !strcmp(name,"material"))
	{
		i=0;
		string materialname;

		PRTMaterial *aux=new PRTMaterial();

		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"name"))			{materialname=atts[i+1];}
			else if (!strcmp(atts[i],"color"))			aux->color=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"ambientcolor"))	aux->ambientcolor=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"shininess"))		aux->shininess=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"specular"))		aux->specular=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"texture"))		//	aux->texture=new PRTPerlin();//aux->texture=new PRTBitmap((char*)atts[i+1]);// *TODO* vaya ful, para cada material una textura nueva
														{map <string,PRTTexture*>::const_iterator itera;itera=textures.find(atts[i+1]);aux->texture=(PRTTexture*)(*itera).second;}
			else if (!strcmp(atts[i],"bumpmap"))		//	aux->bumpmap=new PRTBitmap((char*)atts[i+1]);// *TODO* vaya ful, para cada bumpmap una textura nueva
														{map <string,PRTTexture*>::const_iterator itera;itera=textures.find(atts[i+1]);aux->bumpmap=(PRTTexture*)(*itera).second;}
			else if (!strcmp(atts[i],"bumpfactor"))		aux->bumpfactor=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"reflection"))		aux->reflection=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"refraction"))		aux->refraction=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"alpha"))			aux->alpha=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"gireflection"))	aux->gireflection=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"girefraction"))	aux->girefraction=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"gialpha"))			aux->gialpha=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"BRDFtype"))		aux->BRDFtype=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"glossyper"))		aux->glossyper=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"forced"))			aux->forced=parsebool(atts[i+1]);
			i+=2;
		}
		if (materialname!="")
		{
			typedef pair <string,PRTMaterial*> auxpair;

			materials.insert(auxpair(materialname,aux));
			
			//delete materialname;
			//materialname=NULL;
		}
	}

	//
	// TEXTURE
	//

	else if (!ya && ahora && !strcmp(name,"texture"))
	{
		i=0;
		string texturename;
		string bitmappath;
		int tipo,n=5;
		PRTFloat scale=20,alpha=2,beta=2;
		PRTVector color1,color2;

		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"name"))			{texturename=atts[i+1];}
			else if (!strcmp(atts[i],"type"))		tipo=parseint(atts[i+1]);
			
			// BITMAP
			else if (!strcmp(atts[i],"path"))		{bitmappath=atts[i+1];}
			
			// PERLIN
			else if (!strcmp(atts[i],"scale"))		scale=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"alpha"))		alpha=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"beta"))		beta=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"n"))			n=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"color1"))			color1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"color2"))			color2=parsevector((char*)atts[i+1]);

			
			
			i+=2;
		}
		if (texturename!="")
		{
			PRTTexture *aux;

			if (tipo==8001)
			{
				PRTBitmapImpExpData* fdat=new PRTBitmapImpExpData;

				std::string e(bitmappath);
				e=e.substr(e.find(".")+1,e.length()-e.find(".")+1);
				PRTListMember* i;
				i=trazador->Plugins.first;
				bool res2=false;
				while(i!=NULL)
				{
					if (((PRTPlugin*)i->object)->Type()==PRT_PLUGIN_BITMAP_IMPORT)
					{
						std::string ext=((PRTBitmapImport *)i->object)->Extension();
						
						bool res=!ext.compare(e);
						
						if (res)
						{
							res2=((PRTBitmapImport *)i->object)->Import(bitmappath.c_str(),fdat);
							break;
						}
					}
					
					i=i->next;
				}
				
				if (res2)
				{
					//PRTBitmap *aux;
					aux=new PRTBitmap(fdat);
					delete fdat->pixels;
					delete fdat;
				}
				//else
				//	MessageBox(NULL,(string("No hay plugin para leer la extension de bitmap '") + e + string("'")).c_str(),"Error",0);
			}
			else if (tipo==10001 || tipo==10002 || tipo==10003)
			{
				aux=new PRTPerlin(tipo);
				((PRTPerlin*)aux)->scale=scale;
				((PRTPerlin*)aux)->alpha=alpha;
				((PRTPerlin*)aux)->beta=beta;
				((PRTPerlin*)aux)->n=n;
				((PRTPerlin*)aux)->color1=color1;
				((PRTPerlin*)aux)->color2=color2;
			}

			typedef pair <string,PRTTexture*> auxpair;

			textures.insert(auxpair(texturename,aux));

			//delete texturename;
			//texturename=NULL;
		}
	}

	//
	// OBJETOS
	//

	else if (!ya && ahora && !strcmp(name,"triangle"))
	{
		i=0;

		PRTVector trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3;
		PRTTexCoord trianauxt1,trianauxt2,trianauxt3;
		PRTMaterial* trianauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"p1"))			trianauxp1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p2"))			trianauxp2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p3"))			trianauxp3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n1"))			trianauxn1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n2"))			trianauxn2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n3"))			trianauxn3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t1"))			trianauxt1=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t2"))			trianauxt2=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t3"))			trianauxt3=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);trianauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}

		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial);
			else if (!csgo2)
				csgo2=new PRTTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial);
		}

	}

	else if (!ya && ahora && !strcmp(name,"rectangle"))
	{
		i=0;

		PRTVector trianauxp1,trianauxp2,trianauxp3,trianauxp4,trianauxn1,trianauxn2,trianauxn3,trianauxn4;
		PRTTexCoord trianauxt1,trianauxt2,trianauxt3,trianauxt4;
		PRTMaterial* trianauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"p1"))			trianauxp1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p2"))			trianauxp2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p3"))			trianauxp3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p4"))			trianauxp4=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n1"))			trianauxn1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n2"))			trianauxn2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n3"))			trianauxn3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n4"))			trianauxn4=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t1"))			trianauxt1=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t2"))			trianauxt2=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t3"))			trianauxt3=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t4"))			trianauxt4=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);trianauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}

		if (!incsg)
		{
			//int pos=trazador->ObjectsList.Lenght();
			
			PRTObject *aux;
			aux=new PRTPolygon(new PRTTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial));
			((PRTPolygon*)aux)->AddVertex(trianauxp4,trianauxn4,trianauxt4);
			trazador->ObjectsList.AddAtEnd(aux);
			if (inrts) aux->Transform(rts);
		}
		else
		{
			if (!csgo1)
			{
				csgo1=new PRTPolygon(new PRTTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial));
				((PRTPolygon*)csgo1)->AddVertex(trianauxp4,trianauxn4,trianauxt4);
			}
			else if (!csgo2)
			{
				csgo2=new PRTPolygon(new PRTTriangle(trianauxp1,trianauxp2,trianauxp3,trianauxn1,trianauxn2,trianauxn3,trianauxt1,trianauxt2,trianauxt3,trianauxmaterial));
				((PRTPolygon*)csgo2)->AddVertex(trianauxp4,trianauxn4,trianauxt4);
			}
		}

	}

	else if (!ya && ahora && !strcmp(name,"line"))
	{
		i=0;

		PRTVector lineauxp1,lineauxp2,trianauxn1,trianauxn2;
		PRTTexCoord lineauxt1,lineauxt2;
		PRTMaterial* lineauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"p1"))				lineauxp1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p2"))			lineauxp2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n1"))			trianauxn1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n2"))			trianauxn2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t1"))			lineauxt1=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"t2"))			lineauxt2=parsecoord((char*)atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);lineauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddLine(lineauxp1,lineauxp2/*,trianauxn1,trianauxn2,lineauxt1,lineauxt2*/,lineauxmaterial);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}

		else
		{
			if (!csgo1)
				csgo1=new PRTLine(lineauxp1,lineauxp2,PRTVector(),PRTVector()/*,trianauxn1,trianauxn2,lineauxt1,lineauxt2*/,lineauxmaterial);
			else if (!csgo2)
				csgo2=new PRTLine(lineauxp1,lineauxp2,PRTVector(),PRTVector()/*,trianauxn1,trianauxn2,lineauxt1,lineauxt2*/,lineauxmaterial);
		}
	}

	
	else if (!ya && ahora && !strcmp(name,"sphere"))
	{
		i=0;

		PRTVector sphereauxcentro; 
		PRTFloat sphereauxradio,sphereauxrepu=1.0f,sphereauxrepv=1.0f;
		PRTMaterial* sphereauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))		sphereauxcentro=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"radius"))		sphereauxradio=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"repu"))		sphereauxrepu=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"repv"))		sphereauxrepv=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);sphereauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddSphere(sphereauxcentro,sphereauxradio,sphereauxrepu,sphereauxrepv,sphereauxmaterial);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTSphere(sphereauxcentro,sphereauxradio,sphereauxrepu,sphereauxrepv,sphereauxmaterial);
			else if (!csgo2)
				csgo2=new PRTSphere(sphereauxcentro,sphereauxradio,sphereauxrepu,sphereauxrepv,sphereauxmaterial);
		}
	}

	else if (!ya && ahora && !strcmp(name,"quatjulia"))
	{
		i=0;
		PRTVector centro; 
		PRTQuaternion c;
		int iterac;
		PRTMaterial* mat;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))		centro=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"c"))		c=parsequat((char*)atts[i+1]);
			else if (!strcmp(atts[i],"itera"))		iterac=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);mat=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddQuatJulia(centro,c,iterac,mat);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTQuatJulia(centro,c,iterac,mat);
			else if (!csgo2)
				csgo2=new PRTQuatJulia(centro,c,iterac,mat);
		}
	}

	else if (!ya && ahora && !strcmp(name,"quartic"))
	{
		i=0;

		PRTVector quarticauxcentro; 
		PRTFloat quarticauxa,quarticauxb;
		int quarticauxtype;
		PRTMaterial* quarticauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))		quarticauxcentro=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"a"))		quarticauxa=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"b"))		quarticauxb=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"type"))		quarticauxtype=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);quarticauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddQuartic(quarticauxcentro,quarticauxa,quarticauxb,quarticauxtype,quarticauxmaterial);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTQuartic(quarticauxcentro,quarticauxa,quarticauxb,quarticauxtype,quarticauxmaterial);
			else if (!csgo2)
				csgo2=new PRTQuartic(quarticauxcentro,quarticauxa,quarticauxb,quarticauxtype,quarticauxmaterial);
		}
	}

	else if (!ya && ahora && !strcmp(name,"quadric"))
	{
		i=0;

		PRTVector quadricauxcentro; 
		PRTFloat quadricauxa,quadricauxb,quadricauxc;
		int quadricauxtype;
		PRTMaterial* quadricauxmaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))		quadricauxcentro=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"a"))		quadricauxa=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"b"))		quadricauxb=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"c"))		quadricauxc=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"type"))		quadricauxtype=parseint(atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);quadricauxmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddQuadric(quadricauxcentro,quadricauxa,quadricauxb,quadricauxc,quadricauxtype,quadricauxmaterial);
			if (inrts) trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTQuadric(quadricauxcentro,quadricauxa,quadricauxb,quadricauxc,quadricauxtype,quadricauxmaterial);
			else if (!csgo2)
				csgo2=new PRTQuadric(quadricauxcentro,quadricauxa,quadricauxb,quadricauxc,quadricauxtype,quadricauxmaterial);
		}
	}

	else if (!ya && ahora && !strcmp(name,"cylinder"))
	{
		i=0;

		PRTVector cylindercentro; 
		PRTFloat cylinderradio,cylinderaltura,cylinderrepu,cylinderrepv;
		PRTMaterial* cylindermaterial;
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))		cylindercentro=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"radius"))		cylinderradio=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"lenght"))		cylinderaltura=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"repu"))		cylinderrepu=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"repv"))		cylinderrepv=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);cylindermaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}
		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddCylinder(cylindercentro,cylinderradio,cylinderaltura,cylinderrepu,cylinderrepv,cylindermaterial);
			if (inrts)
			{
				trazador->GetObjectAtPos(pos)->Transform(rts);
				trazador->GetObjectAtPos(pos+1)->Transform(rts);
				trazador->GetObjectAtPos(pos+2)->Transform(rts);
			}
		}
		else
		{
			if (!csgo1)
				csgo1=new PRTCylinder(cylindercentro,cylinderradio,cylinderaltura,cylinderrepu,cylinderrepv,cylindermaterial);
			else if (!csgo2)
				csgo2=new PRTCylinder(cylindercentro,cylinderradio,cylinderaltura,cylinderrepu,cylinderrepv,cylindermaterial);
		}
	}

	else if (!ya && ahora && !strcmp(name,"mesh"))
	{
		i=0;
		
		char* fich="\0";
		PRTMaterial* pluginmaterial;

		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"path"))			fich=((char*)atts[i+1]);
			else if (!strcmp(atts[i],"material"))	{map <string,PRTMaterial*>::const_iterator itera;itera=materials.find(atts[i+1]);pluginmaterial=(PRTMaterial*)(*itera).second;}
			i+=2;
		}

		if (!incsg)
		{
			int pos=trazador->ObjectsList.Length();
			trazador->AddMesh(fich,pluginmaterial);
			trazador->GetObjectAtPos(pos)->Transform(rts);
		}
		else
		{
			// *TODO* quitar esto de aquí

			PRTMeshImpExpData* fdat=new PRTMeshImpExpData;

			std::string e(fich);
			e=e.substr(e.find(".")+1,e.length()-e.find(".")+1);
			PRTListMember* i;
			i=trazador->Plugins.first;
			bool res2=false;
			while(i!=NULL)
			{
				if (((PRTPlugin*)i->object)->Type()==PRT_PLUGIN_MESH_IMPORT)
				{
					std::string ext=((PRTMeshImport *)i->object)->Extension();
					
					bool res=!ext.compare(e);
					
					if (res)
					{
						res2=((PRTMeshImport *)i->object)->Import(fich,fdat);
						break;
					}
				}
				
				i=i->next;
			}
	
			if (res2)
			{
				// *TODO* only first mesh
				if (!csgo1)
					csgo1=new PRTMesh(fdat->geomdat,pluginmaterial);
				else if (!csgo2)
					csgo2=new PRTMesh(fdat->geomdat,pluginmaterial);
				for (unsigned int i=0;i<fdat->nummesh;i++)
					delete fdat->geomdat[i].vertexes;
				delete fdat;
			}
			//else
			//	MessageBox(NULL,(string("No hay plugin para leer la extension de mesh '") + e + string("'")).c_str(),"Error",0);
		}
	}

	//
	// LUCES
	//

	else if (!ya && ahora && !strcmp(name,"pointlight"))
	{
		i=0;

		int auxpl=trazador->LightsList.Lenght();
		PRTPointLight plaux(PRTVector(0,0,0),PRTVector(1,1,1));
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"pos"))				plaux.pos=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"color"))			plaux.color=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"attenuation0"))	plaux.attenuation0=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation1"))	plaux.attenuation1=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation2"))	plaux.attenuation2=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"intensity"))		plaux.intensity=parseprtfloat(atts[i+1]);
			i+=2;
		}
		trazador->AddPointLight(plaux.pos,plaux.color);
		trazador->GetLightAtPos(auxpl)->attenuation0=plaux.attenuation0;
		trazador->GetLightAtPos(auxpl)->attenuation1=plaux.attenuation1;
		trazador->GetLightAtPos(auxpl)->attenuation2=plaux.attenuation2;
		trazador->GetLightAtPos(auxpl)->intensity=plaux.intensity;
	}

	else if (!ya && ahora && !strcmp(name,"directionallight"))
	{
		i=0;

		int auxdl=trazador->LightsList.Lenght();
		PRTDirectionalLight dlaux(PRTVector(1,0,0),PRTVector(1,1,1));
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"dir"))				dlaux.dir=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"color"))			dlaux.color=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"attenuation0"))	dlaux.attenuation0=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation1"))	dlaux.attenuation1=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation2"))	dlaux.attenuation2=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"intensity"))		dlaux.intensity=parseprtfloat(atts[i+1]);
			i+=2;
		}
		trazador->AddDirectionalLight(dlaux.dir,dlaux.color);
		trazador->GetLightAtPos(auxdl)->attenuation0=dlaux.attenuation0;
		trazador->GetLightAtPos(auxdl)->attenuation1=dlaux.attenuation1;
		trazador->GetLightAtPos(auxdl)->attenuation2=dlaux.attenuation2;
		trazador->GetLightAtPos(auxdl)->intensity=dlaux.intensity;
	}

	else if (!ya && ahora && !strcmp(name,"trianglelight"))
	{
		i=0;

		int auxtrianl=trazador->LightsList.Lenght();
		PRTTriangleLight trianlaux(PRTVector(0,0,0),PRTVector(0,0,0),PRTVector(0,0,0),PRTVector(0,0,0),PRTVector(0,0,0),PRTVector(0,0,0),PRTVector(1,1,1));
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"p1"))				trianlaux.p1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p2"))				trianlaux.p2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"p3"))				trianlaux.p3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n1"))				trianlaux.n1=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n2"))				trianlaux.n2=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"n3"))				trianlaux.n3=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"color"))			trianlaux.color=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"attenuation0"))	trianlaux.attenuation0=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation1"))	trianlaux.attenuation1=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation2"))	trianlaux.attenuation2=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"intensity"))		trianlaux.intensity=parseprtfloat(atts[i+1]);
			i+=2;
		}
		if (trianlaux.n1!=PRTVector(0,0,0) || trianlaux.n2!=PRTVector(0,0,0) || trianlaux.n3!=PRTVector(0,0,0))
			trazador->AddTriangleLight(trianlaux.p1,trianlaux.p2,trianlaux.p3,trianlaux.n1,trianlaux.n2,trianlaux.n3,trianlaux.color);
		else
			trazador->AddTriangleLight(trianlaux.p1,trianlaux.p2,trianlaux.p3,trianlaux.color);
		trazador->GetLightAtPos(auxtrianl)->attenuation0=trianlaux.attenuation0;
		trazador->GetLightAtPos(auxtrianl)->attenuation1=trianlaux.attenuation1;
		trazador->GetLightAtPos(auxtrianl)->attenuation2=trianlaux.attenuation2;
		trazador->GetLightAtPos(auxtrianl)->intensity=trianlaux.intensity;
	}

	else if (!ya && ahora && !strcmp(name,"spherelight"))
	{
		i=0;

		int auxspherel=trazador->LightsList.Lenght();
		PRTSphereLight spherelaux(PRTVector(0,0,0),1,PRTVector(1,1,1));//centro radio color
		
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"center"))				spherelaux.p=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"radius"))			spherelaux.r=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"color"))			spherelaux.color=parsevector((char*)atts[i+1]);
			else if (!strcmp(atts[i],"attenuation0"))	spherelaux.attenuation0=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation1"))	spherelaux.attenuation1=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"attenuation2"))	spherelaux.attenuation2=parseprtfloat(atts[i+1]);
			else if (!strcmp(atts[i],"intensity"))		spherelaux.intensity=parseprtfloat(atts[i+1]);
			i+=2;
		}
		trazador->AddSphereLight(spherelaux.p,spherelaux.r,spherelaux.color);
		trazador->GetLightAtPos(auxspherel)->attenuation0=spherelaux.attenuation0;
		trazador->GetLightAtPos(auxspherel)->attenuation1=spherelaux.attenuation1;
		trazador->GetLightAtPos(auxspherel)->attenuation2=spherelaux.attenuation2;
		trazador->GetLightAtPos(auxspherel)->intensity=spherelaux.intensity;
	}

	//
	// CSG Y RTS
	//

	else if (!ya && ahora && !strcmp(name,"union"))
	{
		incsg++;
		i=0;
	}

	else if (!ya && ahora && !strcmp(name,"intersection"))
	{
		incsg++;
		i=0;
	}

	else if (!ya && ahora && !strcmp(name,"diference"))
	{
		incsg++;
		i=0;
	}

	else if (!ya && ahora && !strcmp(name,"rotate"))
	{
		i=0;
		inrts++;
		PRTVector rotateeje,rotatecentro;
		PRTFloat rotateangulo;
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"axis"))				rotateeje=parsevector((char*)atts[i+1]);
			else if(!strcmp(atts[i],"angle"))		rotateangulo=parseprtfloat((char*)atts[i+1]);
			//else if(!strcmp(atts[i],"centro"))		rotatecentro=parsevector((char*)atts[i+1]);
			i+=2;
		}
		rtsantes.push(rts);
		
		/*PRTMatrix PosRotZ;
		PosRotZ.RotateZ(rotateeje.z*rotateangulo);//in degrees
		PRTMatrix PosRotY;
		PosRotY.RotateY(rotateeje.y*rotateangulo);
		PRTMatrix PosRotX;
		PosRotX.RotateX(rotateeje.x*rotateangulo);

		rts=rts*PosRotZ*PosRotY*PosRotX;*/

		PRTFloat c=PRTCos(PRTGradToRad(rotateangulo));
		PRTFloat s=PRTSin(PRTGradToRad(rotateangulo));
		PRTFloat t=1-c;
		PRTMatrix PosRot;
		PosRot.col[0]=PRTVector4(t*rotateeje.x*rotateeje.x+c,t*rotateeje.x*rotateeje.y+rotateeje.z*s,t*rotateeje.x*rotateeje.z-rotateeje.y*s,0);
		PosRot.col[1]=PRTVector4(t*rotateeje.x*rotateeje.y-rotateeje.z*s,t*rotateeje.y*rotateeje.y+c,t*rotateeje.y*rotateeje.z+rotateeje.x*s,0);
		PosRot.col[2]=PRTVector4(t*rotateeje.x*rotateeje.z+rotateeje.y*s,t*rotateeje.y*rotateeje.z-rotateeje.x*s,t*rotateeje.z*rotateeje.z+c,0);
		rts=rts*PosRot;
	}

	else if (!ya && ahora && !strcmp(name,"translate"))
	{
		i=0;
		inrts++;
		PRTVector translatevector;
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"vector"))		translatevector=parsevector((char*)atts[i+1]);
			i+=2;
		}
		rtsantes.push(rts);

		PRTMatrix aux;
		aux.col[3]=translatevector;
		rts=rts*aux;
		//rts.col[3]+=translatevector;
	}

	else if (!ya && ahora && !strcmp(name,"scale"))
	{
		i=0;
		inrts++;
		PRTVector scalefactor,scalecentro;
		while (atts[i]!=NULL)
		{
			if (!strcmp(atts[i],"factor"))		scalefactor=parsevector((char*)atts[i+1]);
			//if (!strcmp(atts[i],"centro"))		scalecentro=parsevector((char*)atts[i+1]);
			i+=2;
		}
		rtsantes.push(rts);
		
		PRTMatrix auxscal;
		auxscal.col[0]=PRTVector4(scalefactor.x,0,0,0);
		auxscal.col[1]=PRTVector4(0,scalefactor.y,0,0);
		auxscal.col[2]=PRTVector4(0,0,scalefactor.z,0);
		/*PRTMatrix auxtrans;
		auxtrans.col[3]=-scalecentro;
		rts=rts*auxtrans*auxscal;*/
		rts=rts*auxscal;
	}

}

void PRTutMain::endElement(const XML_Char* name)
{

	if (ahora && !strcmp(name,"scene"))
	{
		ahora=false;
		ya=true;
	}

	if (incsg && ahora && !strcmp(name,"union"))
	{
		incsg--;
		if (incsg)
			csgo1=new PRTCsgObject(csgo1,csgo2,PRT_CSG_UNION);
		else
		{
			trazador->AddCsgObject(csgo1,csgo2,PRT_CSG_UNION);
			csgo1=NULL;
		}
		csgo2=NULL;
	}

	if (incsg && ahora && !strcmp(name,"intersection"))
	{
		incsg--;
		if (incsg)
			csgo1=new PRTCsgObject(csgo1,csgo2,PRT_CSG_INTERSECTION);
		else
		{
			trazador->AddCsgObject(csgo1,csgo2,PRT_CSG_INTERSECTION);
			csgo1=NULL;
		}
		csgo2=NULL;
	}

	if (incsg && ahora && !strcmp(name,"diference"))
	{
		incsg--;
		if (incsg)
			csgo1=new PRTCsgObject(csgo1,csgo2,PRT_CSG_DIFERENCE);
		else
		{
			trazador->AddCsgObject(csgo1,csgo2,PRT_CSG_DIFERENCE);
			csgo1=NULL;
		}
		csgo2=NULL;
	}

	if (inrts && ahora && !strcmp(name,"rotate"))
	{
		rts=rtsantes.top();
		rtsantes.pop();
		inrts--;
	}

	if (inrts && ahora && !strcmp(name,"translate"))
	{
		rts=rtsantes.top();
		rtsantes.pop();
		inrts--;
	}

	if (inrts && ahora && !strcmp(name,"scale"))
	{
		rts=rtsantes.top();
		rtsantes.pop();
		inrts--;
	}
}
