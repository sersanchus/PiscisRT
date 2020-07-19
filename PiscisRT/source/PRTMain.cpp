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
#include "PRTRandom.h"
#include "PRTMain.h"
//#include <math.h>

#include <vector>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

#ifdef __MAKE_WINDOWS_DLL__

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

PRTFloat Module(PRTVector v){ return PRTFSqrt(v.x*v.x + v.y*v.y + v.z*v.z); }//modulo rapido

PRTMain::PRTMain(int type)
{ 
	//BRadiosity=false;
	BTextures=true;
	BReflections=true;
	BRefractions=true;
	BSpecular=false;//¿? va bien o q?
	BAlpha=true;
	BShadows=true;
	BDoubleSided=false;
	BNInter=true;// *TODO* valdria mas hacerla siempre?
	BCInter=true;
	BOctrees=true;
	BConvexHulls=true; //lo hago siempre que toca
	BIndirectLight=false;
	BGiReflections=true;
	BGiRefractions=true;
	BGiAlpha=true;
	BGlossy=true;
	BBump=true;
	BTransformations=true;

	luzambiente=PRTVector(0.0,0.0,0.0);
	shadowpass=1;

	//numrayos=0;
	//numintertest=0;

	//FAST SQRT
	PRTInitFastSqrt();


	//PLUGINS
	LoadPlugins("plugins/");
	
	PRTRandomInit(5);
	PRTWRandomInit(5);

	//OCTREES
	Octrees=NULL;

	switch(type)
	{	
	case PRT_RAYTRACING:
		render=new PRTRayTracing(this);
		break;
	case PRT_PHOTONMAPPING:
		render=new PRTPhotonMapping(this);
		break;
	case PRT_PATCHES:
		render=new PRTPatches(this);
	}

	return; 
}



// INTERFACE




void PRTMain::AddTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTMaterial* material)
{
	PRTObject *aux;
	aux=new PRTTriangle(p1,p2,p3,n1,n2,n3,material);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTMaterial* material)
{
	PRTVector normal=(p2-p1)^(p3-p1);
	PRTObject *aux;
	aux=new PRTTriangle(p1,p2,p3,normal,normal,normal,material);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTVector c1,PRTVector c2,PRTVector c3,PRTMaterial* material)
{
	PRTObject *aux;
	aux=new PRTTriangle(p1,p2,p3,n1,n2,n3,c1,c2,c3,material);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddTriangle(PRTVector p1,PRTVector p2,PRTVector p3,PRTVector n1,PRTVector n2,PRTVector n3,PRTTexCoord t1,PRTTexCoord t2,PRTTexCoord t3,PRTMaterial* material)
{
	PRTObject *aux;
	aux=new PRTTriangle(p1,p2,p3,n1,n2,n3,t1,t2,t3,material);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddSphere(PRTVector cen,PRTFloat rad,PRTMaterial* mat)
{
	PRTObject *aux;
	aux=new PRTSphere(cen,rad,mat);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddSphere(PRTVector cen,PRTFloat rad,PRTFloat ru,PRTFloat rv,PRTMaterial* mat)
{
	PRTObject *aux;
	aux=new PRTSphere(cen,rad,ru,rv,mat);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddQuadric(PRTVector v,PRTFloat a,PRTFloat b,PRTFloat c,int t,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTQuadric(v,a,b,c,t,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddCylinder(PRTVector c,PRTFloat r,PRTFloat a,PRTFloat ru,PRTFloat rv,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTCylinder(c,r,a,ru,rv,m);
	ObjectsList.AddAtEnd(aux);
	aux=new PRTCircle(c+PRTVector(0,0,a/2.0),r,PRTVector(0,0,1),ru,rv,m);
	ObjectsList.AddAtEnd(aux);
	aux=new PRTCircle(c+PRTVector(0,0,-a/2.0),r,PRTVector(0,0,-1),ru,rv,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddCircle(PRTVector c,PRTVector n,PRTFloat r,PRTFloat ru,PRTFloat rv,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTCircle(c,r,n,ru,rv,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddPlane(PRTVector c,PRTVector n,PRTFloat ru,PRTFloat rv,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTPlane(c,n,ru,rv,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddQuartic(PRTVector p,PRTFloat a,PRTFloat b,int t,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTQuartic(p,a,b,t,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddCone(PRTVector c,PRTFloat r,PRTFloat a,PRTFloat ru,PRTFloat rv,PRTMaterial* m)
{
	PRTObject *aux;
	aux=new PRTCone(c,r,a,ru,rv,m);
	ObjectsList.AddAtEnd(aux);
	aux=new PRTCircle(c+PRTVector(0,0,-a/2.0),r,PRTVector(0,0,-1),ru,rv,m);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddLine(PRTVector p1,PRTVector p2,PRTMaterial* material)
{
	PRTVector normal;
	PRTObject *aux;
	aux=new PRTLine(p1,p2,normal,normal,material);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddCsgObject(PRTObject* o1,PRTObject* o2,int type)
{
	PRTCsgObject *aux;
	aux=new PRTCsgObject(o1,o2,type);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddQuatJulia(PRTVector cen/*,PRTFloat rad*/,PRTQuaternion caux,int itera, PRTMaterial *mat)
{
	PRTQuatJulia *aux;
	aux=new PRTQuatJulia(cen,caux,itera,mat);
	ObjectsList.AddAtEnd(aux);
}

void PRTMain::AddMesh(char* fich, PRTMaterial* mat)
{
	PRTMeshImpExpData* fdat=new PRTMeshImpExpData;

//#ifdef __MAKE_WINDOWS__

	string e(fich);
	e=e.substr(e.find(".")+1,e.length()-e.find(".")+1);
	PRTListMember* i;
	i=this->Plugins.first;
	bool res2=false;
	while(i!=NULL)
	{
		if (((PRTPlugin*)i->object)->Type()==PRT_PLUGIN_MESH_IMPORT)
		{
			string ext=((PRTMeshImport *)i->object)->Extension();
			
			bool res=!ext.compare(e);
			
			if (res)
			{
				res2=((PRTMeshImport *)i->object)->Import(fich,fdat);
				break;
			}
		}
		
		i=i->next;
	}
	
//#endif

	if (res2)
	{
		PRTMesh *aux;
		for (unsigned int i=0;i<fdat->nummesh;i++)
		{
			aux=new PRTMesh(&fdat->geomdat[i],mat);
			ObjectsList.AddAtEnd(aux);
		}
		for (unsigned int i=0;i<fdat->nummesh;i++)
			delete fdat->geomdat[i].vertexes;
		delete fdat;
	}
	//else
	//	MessageBox(NULL,(string("No hay plugin para leer la extension de mesh '") + e + string("'")).c_str(),"Error",0);
}

PRTObject *PRTMain::GetObjectAtPos(int i)
{
	return (PRTObject*) ObjectsList.GetAtPos(i);
}

void PRTMain::AddPointLight(PRTVector p,PRTVector c)
{
	PRTLight *aux;
	aux=new PRTPointLight(p,c);
	LightsList.AddAtEnd(aux);
}

void PRTMain::AddTriangleLight(PRTVector auxp1,PRTVector auxp2,PRTVector auxp3,PRTVector auxn1,PRTVector auxn2,PRTVector auxn3,PRTVector c)
{
	PRTLight *aux;
	aux=new PRTTriangleLight(auxp1,auxp2,auxp3,auxn1,auxn2,auxn3,c);
	LightsList.AddAtEnd(aux);
}

void PRTMain::AddTriangleLight(PRTVector auxp1,PRTVector auxp2,PRTVector auxp3,PRTVector c)
{
	PRTLight *aux;
	aux=new PRTTriangleLight(auxp1,auxp2,auxp3,(auxp1-auxp3).Normalize(),(auxp2-(auxp1+((auxp3-auxp1)/2))).Normalize(),(auxp3-auxp1).Normalize(),c);
	LightsList.AddAtEnd(aux);
}

void PRTMain::AddSphereLight(PRTVector punto,PRTFloat radio,PRTVector color)
{
	PRTLight *aux;
	aux=new PRTSphereLight(punto,radio,color);
	LightsList.AddAtEnd(aux);
}

void PRTMain::AddDirectionalLight(PRTVector d,PRTVector c)
{
	PRTLight *aux;
	aux=new PRTDirectionalLight(d,c);
	LightsList.AddAtEnd(aux);
}

PRTLight *PRTMain::GetLightAtPos(int i)
{
	return (PRTLight*) LightsList.GetAtPos(i);
}






// PLUGINS






void PRTMain::LoadPlugins(LPSTR path)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)

	WIN32_FIND_DATA fdata;	
	string j1=path;j1+="*.dll";LPSTR todo=(LPSTR)j1.c_str();
	HANDLE hsearch=FindFirstFile(todo,&fdata);
	if(hsearch!=(HANDLE)0xffffffff)
	{		
		string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
		if (PRTIsPlugin(todo))
		{
			string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
			unsigned int tipoplugin=PRTTypePlugin(todo);
			if (tipoplugin==PRT_PLUGIN_MESH_IMPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTMeshImport *plugin=new PRTMeshImport(todo);
				//ImportPlugins.AddAtEnd(plugin);
				Plugins.AddAtEnd(plugin);
			}
			else if (tipoplugin==PRT_PLUGIN_BITMAP_IMPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTBitmapImport *plugin=new PRTBitmapImport(todo);
				//ImportPlugins.AddAtEnd(plugin);
				Plugins.AddAtEnd(plugin);
			}
			else if (tipoplugin==PRT_PLUGIN_BITMAP_EXPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTBitmapExport *plugin=new PRTBitmapExport(todo);
				//ImportPlugins.AddAtEnd(plugin);
				Plugins.AddAtEnd(plugin);
			}
			/*if (tipoplugin==PRT_PLUGIN_UNKNOW)
			{
				//AfxMessageBox(((CString)"Tipo de plugin desconocido")+path+fdata.cFileName,MB_ICONEXCLAMATION,0);				
			}*/
		}

		while(1)
		{
			if (!FindNextFile(hsearch,&fdata))
				break;
			string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
			if (!PRTIsPlugin(todo))
			{
				continue;
			}
			j1=path;j1+=fdata.cFileName;todo=(LPSTR)j1.c_str();
			unsigned int tipoplugin=PRTTypePlugin(todo);
			if (tipoplugin==PRT_PLUGIN_MESH_IMPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTMeshImport *plugin=new PRTMeshImport(todo);
				//ImportPlugins.AddAtEnd(plugin);
                Plugins.AddAtEnd(plugin);
			}
			else if (tipoplugin==PRT_PLUGIN_BITMAP_IMPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTBitmapImport *plugin=new PRTBitmapImport(todo);
				//ImportPlugins.AddAtEnd(plugin);
				Plugins.AddAtEnd(plugin);
			}
			else if (tipoplugin==PRT_PLUGIN_BITMAP_EXPORT)
			{
				string j1=path;j1+=fdata.cFileName;LPSTR todo=(LPSTR)j1.c_str();
				PRTBitmapExport *plugin=new PRTBitmapExport(todo);
				//ImportPlugins.AddAtEnd(plugin);
				Plugins.AddAtEnd(plugin);
			}
			/*if (tipoplugin==PRT_PLUGIN_UNKNOW)
			{
				//AfxMessageBox(((CString)"Tipo de plugin desconocido")+path+fdata.cFileName,MB_ICONEXCLAMATION,0);				
			}*/
		}
	}
#endif

}

/*IMPORT_EXPORT_DATA PRTMain::ImportPlugin(LPCSTR e,LPCSTR arc)
{
	
	IMPORT_EXPORT_DATA fdat;

#ifdef __MAKE_WINDOWS__

	PRTListMember* i;
	i=this->ImportPlugins.first;
	while(i!=NULL)
	{
		LPCSTR ext=((PRTPluginImport *)i->object)->Extension();
		
		//bool res=false;
		//int pos=0;
		//while (1)
		//{
		//	if (ext[pos]!=e[pos])
		//	{
		//		res=false;
		//		break;
		//	}
		//	pos++;
		//	if (ext[pos]=='\0')
		//	{
		//		res=true;
		//		break;
		//	}
		//}

		bool res=strcmp(ext,e)==0;
		
		if (res)
		{
			bool res2=((PRTPluginImport *)i->object)->Import(arc,&fdat);
		}

		i=i->next;
	}
	
#endif

	return fdat;
}*/






// RENDER







PRTVector PRTMain::RayTrace(PRTRay r,int deep)
{
	return render->RayTrace(r,deep);
}

PRTIntersectPoint PRTMain::FindNearestIntersection(PRTRay r,PRTObject *d)
{
	PRTFloat DELTA=PRTFloat(0.000001);
	PRTIntersectPoint aux; //colision temporal
	PRTIntersectPoint colisiona; //colision optima, final
	PRTObject* opt=NULL; //objeto optimo
	PRTFloat valoropt=PRTINFINITE; //distancia muy grande, para ir guardando la distancia mas corta
	PRTVector pointopt;
	
	PRTArray<PRTObject*> octreeList;
	PRTArray<PRTObject*>* list;
//	bool borra=false;
	if (!BOctrees || Octrees==NULL) // si lo del octree esrootsolo se tendria que dejar que se pusiera octreesdeep==0
		list = &ObjectsList;
	else // tengo en cuenta los octrees
	{
//		borra=true;
//		ejem = Octrees->ReturnObjects(r,d);
		Octrees->ReturnObjects(r, d, octreeList);
		list = &octreeList;
	}
//	PRTListMember *o=ejem->first;
	bool ya=false;
//	while (o!=NULL)
	for (unsigned int i=0; i<list->Length(); i++)
	{
		PRTObject* obj = list->GetAtPos(i);
		//	...				...				distancia al punto mas cecano del convex hull del objeto
		if ((obj!=d) && ( !ya || ((Module( obj->convexhull.chcen-r.orig )-(obj->convexhull.chrad))<valoropt) ) )
		{
			aux=r.Intersect(obj,!BDoubleSided,BTransformations);
			//numintertest+=r.numrayintertest;
			if (aux.collision && aux.distance-DELTA<valoropt)// *TODO* error de precision (objeto delante de otro)
			{
				valoropt=aux.distance;
				pointopt=aux.point;
				opt=obj;
				ya=true;
			}
		}
//		o=o->next;
	}
	if (opt!=NULL)
	{
		colisiona.collision=true;
		colisiona.distance=valoropt;
		colisiona.point=pointopt;
		colisiona.object=opt;
	}
//	if (borra)
//		delete ejem;
	return colisiona;
}

PRTVector PRTMain::ComputeReflection(PRTRay r,PRTIntersectPoint colisiona, PRTVector normal,int deep,PRTObject *o)
{
	PRTFloat DELTA=PRTFloat(0.0001);
	PRTVector p=r.dir.Reflection(normal); /////?=?=?=?=??=?¿?¿?¿?¿
	if (BGlossy && o->material->glossyper>0.0)
	{
		//p=p.Perturb((Random()-0.5)*o->material->glossyper,(Random()-0.5)*o->material->glossyper);
		p=p.Perturb((PRTWRandom()-0.5)*o->material->glossyper,(PRTWRandom()-0.5)*o->material->glossyper);
	}
	return RayTrace(PRTRay(colisiona.point+(p*DELTA),p),deep-1);
}

PRTVector PRTMain::ComputeRefraction(PRTRay r,PRTIntersectPoint colisiona, PRTVector normal,int deep,PRTObject *o)
{
	PRTFloat DELTA=PRTFloat(0.0001);
	PRTVector ret(0,0,0);
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
		PRTVector t=(i*div)+(normalaux2*((div*cosi)-PRTFSqrt(discr)));
		//cuando -normalaux2*t no es real -> total internal reflection
		if (discr>=0)
			return RayTrace(PRTRay(colisiona.point+(t.Normalize()*DELTA),t.Normalize()),deep-1);
	}
	return PRTVector(0.0,0.0,0.0);
}

PRTVector PRTMain::ComputeTransparence(PRTRay r,PRTIntersectPoint colisiona, PRTVector normal,int deep, PRTObject*o)
{
	PRTFloat DELTA=PRTFloat(0.0001);
	return RayTrace(PRTRay(colisiona.point+(r.dir*DELTA),r.dir),deep-1);
}

PRTVector PRTMain::ComputeReflectedLight(PRTVector p,PRTVector in,PRTVector out,PRTObject* objeto,PRTVector normal,PRTVector power)
{
	PRTVector coloraux(0,0,0);

	coloraux=PRTVector(luzambiente.x*objeto->material->color.x,luzambiente.y*objeto->material->color.y,luzambiente.z*objeto->material->color.z);

	// COMPONENTE DIRECTA
	PRTFloat BRDF=objeto->material->ComputeBRDF(in,out,normal,objeto->material->BRDFp1,objeto->material->BRDFp2,objeto->material->BRDFp3,objeto->material->BRDFp4,objeto->material->BRDFp5,objeto->material->BRDFp6,objeto->material->BRDFp7);
	PRTFloat coseno;//=(normal)*(-in); //parte difusa
	//coseno=coseno*BRDF; //*TODO*
	coseno=BRDF;
		
	// CALCULO DE COLOR DIRECTO
	PRTVector colormat;
	if (!BCInter || !(objeto->type==PRT_OBJECT_TRIANGLE))
		colormat=objeto->material->color;
	else
		colormat=objeto->material->color;
		//colormat=objeto->ComputeColor(p);
			
	if (BSpecular)
	{
		// COMPONENTE ESPECULAR ¿?
		PRTFloat specular=(-in)*((-out).Reflection(normal/*objeto->n1*/));//.Normalizado();
		if(specular>0) specular=pow(specular,objeto->material->shininess)*objeto->material->specular;
		else specular=0;

		coloraux.x+=((power.x*coseno/*+luzambiente.x*/+specular)*colormat.x);
		coloraux.y+=((power.y*coseno/*+luzambiente.y*/+specular)*colormat.y);
		coloraux.z+=((power.z*coseno/*+luzambiente.z*/+specular)*colormat.z);
	}
	else
	{
		coloraux.x+=((power.x*coseno/*+luzambiente.x*/)*colormat.x);
		coloraux.y+=((power.y*coseno/*+luzambiente.y*/)*colormat.y);
		coloraux.z+=((power.z*coseno/*+luzambiente.z*/)*colormat.z);
	}

	return coloraux;
}





// OCTREES






bool PRTMain::BuildOctrees(unsigned int deep, unsigned int num)
{
	if (Octrees!=NULL)
		delete Octrees;

	if (BOctrees)
	{
		PRTFloat maxx=-PRTINFINITE;
		PRTFloat maxy=-PRTINFINITE;
		PRTFloat maxz=-PRTINFINITE;
		PRTFloat minx=PRTINFINITE;
		PRTFloat miny=PRTINFINITE;
		PRTFloat minz=PRTINFINITE;
//		PRTListMember *o=ObjectsList.first;
//		while (o!=NULL)
		for (unsigned int i=0; i<ObjectsList.Length(); i++)
		{
			PRTObject* obj = ObjectsList.GetAtPos(i);
			if (obj->convexhull.chbeg.x<minx) minx=obj->convexhull.chbeg.x;
			if (obj->convexhull.chbeg.y<miny) miny=obj->convexhull.chbeg.y;
			if (obj->convexhull.chbeg.z<minz) minz=obj->convexhull.chbeg.z;
			if (obj->convexhull.chend.x>maxx) maxx=obj->convexhull.chend.x;
			if (obj->convexhull.chend.y>maxy) maxy=obj->convexhull.chend.y;
			if (obj->convexhull.chend.z>maxz) maxz=obj->convexhull.chend.z;
//			o=o->next;
		}
		//o=ObjectsList.first;
		// ya tengo los limites del octree padre en (minx,miny,minz) (maxx,maxy,maxz)
		
		Octrees=new PRTOcTree(minx,miny,minz,maxx,maxy,maxz,deep,num,ObjectsList);
	}

	return true;
}

void PRTMain::Reset()
{
	if (Octrees)
		delete Octrees;
	ObjectsList.Clear();
	LightsList.Free();
		
	//BRadiosity=false;
	BTextures=true;
	BReflections=true;
	BRefractions=true;
	BSpecular=false;//¿? va bien o q?
	BAlpha=true;
	BShadows=true;
	BDoubleSided=false;
	BNInter=true;// *TODO* valdria mas hacerla siempre?
	BCInter=true;
	BOctrees=true;
	BConvexHulls=true; //lo hago siempre que toca
	BIndirectLight=false;
	BGiReflections=true;
	BGiRefractions=true;
	BGiAlpha=true;
	BGlossy=true;
	BBump=true;

	luzambiente=PRTVector(0.0,0.0,0.0);
	shadowpass=1;

	//numrayos=0;
	//numintertest=0;

	//OCTREES
	Octrees=NULL;
}



