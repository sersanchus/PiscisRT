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

#include "PRTArray.h"
#include "PRTVector.h"
#include "PRTTriangle.h"
#include "PRTSphere.h"
#include "PRTQuadric.h"
#include "PRTCylinder.h"
#include "PRTCircle.h"
#include "PRTPlane.h"
#include "PRTQuartic.h"
#include "PRTCone.h"
#include "PRTLight.h"
#include "PRTPointLight.h"
#include "PRTTriangleLight.h"
#include "PRTSphereLight.h"
#include "PRTDirectionalLight.h"
#include "PRTRay.h"
#include "PRTPhotonMap.h"
#include "PRTPlugin.h"
#include "PRTMeshImpExp.h"
#include "PRTUtil.h"
#include "PRTThread.h"
#include "PRTRandom.h"
#include "PRTOcTree.h"
#include "PRTLine.h"
#include "PRTCsgObject.h"
#include "PRTObject.h"
#include "PRTQuatJulia.h"
#include "PRTRender.h"
#include "PRTRayTracing.h"
#include "PRTPhotonMapping.h"
#include "PRTPatches.h"
#include "PRTBitmap.h"
#include "PRTPerlin.h"
#include "PRTMesh.h"
#include "PRTPolygon.h"

//--------------------------------------------------------------------------------------
//! The PRTMain class.
/*!	This class represents the interface to the PiscisRT.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTMain
{

public:
	
	//PRTMAIN
	PRTMain(int type=PRT_RAYTRACING);
	bool BTextures;
	bool BReflections;
	bool BRefractions;
	bool BSpecular; //brillo especular
	bool BAlpha;
	bool BShadows;
	bool BDoubleSided;
	bool BNInter; //normal interpolation
	bool BCInter; //color interpolation
	bool BOctrees;
	bool BConvexHulls; //siempre se hacen
	bool BIndirectLight; //activa la radiosidad
	bool BGiReflections;
	bool BGiRefractions;
	bool BGiAlpha;
	bool BGlossy;
	bool BBump;
	bool BTransformations;
	PRTVector luzambiente;
	int shadowpass;
	PRTRender* render;
	//int numrayos;
	//unsigned long int numintertest;
	PRTVector RayTrace(PRTRay,int);
	PRTIntersectPoint FindNearestIntersection(PRTRay,PRTObject *d=NULL);
	PRTVector ComputeReflection(PRTRay,PRTIntersectPoint,PRTVector,int,PRTObject*);
	PRTVector ComputeRefraction(PRTRay,PRTIntersectPoint,PRTVector,int,PRTObject*);
	PRTVector ComputeTransparence(PRTRay,PRTIntersectPoint,PRTVector,int,PRTObject*);
	PRTVector ComputeReflectedLight(PRTVector,PRTVector,PRTVector,PRTObject*,PRTVector,PRTVector);
	void Reset(void);
	
	//OBJECTS
	PRTArray<PRTObject*> ObjectsList;
	void AddTriangle(PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTMaterial*);
	void AddTriangle(PRTVector,PRTVector,PRTVector,PRTMaterial*);
	void AddTriangle(PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTMaterial*);
	void AddTriangle(PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTTexCoord,PRTTexCoord,PRTTexCoord,PRTMaterial*);
	void AddSphere(PRTVector,PRTFloat,PRTMaterial*);
	void AddSphere(PRTVector,PRTFloat,PRTFloat,PRTFloat,PRTMaterial*);
	void AddQuadric(PRTVector,PRTFloat,PRTFloat,PRTFloat,int,PRTMaterial*);
	void AddCylinder(PRTVector,PRTFloat,PRTFloat,PRTFloat,PRTFloat,PRTMaterial*);
	void AddCircle(PRTVector,PRTVector,PRTFloat,PRTFloat,PRTFloat,PRTMaterial*);
	void AddPlane(PRTVector,PRTVector,PRTFloat,PRTFloat,PRTMaterial*);
	void AddQuartic(PRTVector,PRTFloat,PRTFloat,int,PRTMaterial*);
	void AddCone(PRTVector,PRTFloat,PRTFloat,PRTFloat,PRTFloat,PRTMaterial*);
	void AddLine(PRTVector,PRTVector,PRTMaterial*);
	void AddCsgObject(PRTObject*,PRTObject*,int);
	void AddQuatJulia(PRTVector/*,PRTFloat rad*/,PRTQuaternion,int,PRTMaterial*);
	void AddMesh(char*,PRTMaterial*);
	PRTObject *GetObjectAtPos(int);
	
	//LIGHTS
	PRTDinamicList LightsList;
	void AddPointLight(PRTVector,PRTVector);
	void AddTriangleLight(PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector,PRTVector);
	void AddTriangleLight(PRTVector,PRTVector,PRTVector,PRTVector);
	void AddSphereLight(PRTVector,PRTFloat,PRTVector);
	void AddDirectionalLight(PRTVector,PRTVector);
	PRTLight *GetLightAtPos(int);

	//OCTREES
	bool BuildOctrees(unsigned int,unsigned int);
	PRTOcTree *Octrees;

	//PLUGINS
	void LoadPlugins(LPSTR);
	//PRTDinamicList ImportPlugins;
	PRTDinamicList Plugins;
	//IMPORT_EXPORT_DATA ImportPlugin(LPCSTR,LPCSTR);
	
};

