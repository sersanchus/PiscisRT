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
#include "PRTMesh.h"
#include "PRTTriangle.h"

PRTMesh::PRTMesh(PRTMeshImpExpObject* data,PRTMaterial* mat)
	:PRTObject(PRTVector(0,0,0),PRT_OBJECT_MESH,mat) //*TODO* position?
{
	PRTVector pos;
	int numtris=0;
	PRTFloat minx,miny,minz;
	PRTFloat maxx,maxy,maxz;
	minx=miny=minz=PRTINFINITE;
	maxx=maxy=maxz=-PRTINFINITE;
	PRTDinamicList* tris=new PRTDinamicList();

	// *TODO* only loads the first mesh in the data

	for (unsigned int i=0;i<data->numv;i+=3)
	{
		PRTTriangle* aux=new PRTTriangle(
			PRTVector(data->vertexes[i].x,data->vertexes[i].y,data->vertexes[i].z),
			PRTVector(data->vertexes[i+1].x,data->vertexes[i+1].y,data->vertexes[i+1].z),
			PRTVector(data->vertexes[i+2].x,data->vertexes[i+2].y,data->vertexes[i+2].z),
			PRTVector(data->vertexes[i].nx,data->vertexes[i].ny,data->vertexes[i].nz),
			PRTVector(data->vertexes[i+1].nx,data->vertexes[i+1].ny,data->vertexes[i+1].nz),
			PRTVector(data->vertexes[i+2].nx,data->vertexes[i+2].ny,data->vertexes[i+2].nz),
			PRTTexCoord(data->vertexes[i].u,data->vertexes[i].v),
			PRTTexCoord(data->vertexes[i+1].u,data->vertexes[i+1].v),
			PRTTexCoord(data->vertexes[i+2].u,data->vertexes[i+2].v),
			mat
			);
		tris->AddAtEnd(aux);
		numtris++;
		if (aux->p1.x<minx)	minx=aux->p1.x;
		if (aux->p2.x<minx)	minx=aux->p2.x;
		if (aux->p3.x<minx)	minx=aux->p3.x;
		if (aux->p1.y<miny)	miny=aux->p1.y;
		if (aux->p2.y<miny)	miny=aux->p2.y;
		if (aux->p3.y<miny)	miny=aux->p3.y;
		if (aux->p1.z<minz)	minz=aux->p1.z;
		if (aux->p2.z<minz)	minz=aux->p2.z;
		if (aux->p3.z<minz)	minz=aux->p3.z;
		if (aux->p1.x>maxx)	maxx=aux->p1.x;
		if (aux->p2.x>maxx)	maxx=aux->p2.x;
		if (aux->p3.x>maxx)	maxx=aux->p3.x;
		if (aux->p1.y>maxy)	maxy=aux->p1.y;
		if (aux->p2.y>maxy)	maxy=aux->p2.y;
		if (aux->p3.y>maxy)	maxy=aux->p3.y;
		if (aux->p1.z>maxz)	maxz=aux->p1.z;
		if (aux->p2.z>maxz)	maxz=aux->p2.z;
		if (aux->p3.z>maxz)	maxz=aux->p3.z;
		pos+=aux->position;
	}
	pos=pos/(PRTFloat)numtris;
	position=pos;
	material=mat;
	despu=0;
	despv=0;
	repu=1;
	repv=1;
	convexhull.chbeg=PRTVector(minx,miny,minz);
	convexhull.chend=PRTVector(maxx,maxy,maxz);
	convexhull.chcen=position;
	convexhull.chrad=(convexhull.chbeg-convexhull.chcen).Module();
	convexhull.SolvePrecision();

	what=0;

	//if (BOctrees)
	{
		PRTArray<PRTObject*> trislist;
		PRTListMember* member = tris->first;
		while(member != NULL)
		{
			trislist.AddAtEnd((PRTObject*)member->object);
			member = member->next;
		}
		
		octrees=new PRTOcTree(minx,miny,minz,maxx,maxy,maxz,/*octreesdeep*/5,100,trislist);
	}

	delete tris;
}

PRTMesh::~PRTMesh(void)
{
	//tris.Free();
	delete octrees;
}

PRTTexCoord PRTMesh::ComputeTexCoord(PRTVector p)
{
	return what->ComputeTexCoord(p);
}

PRTVector PRTMesh::ComputePoint(PRTTexCoord tc)
{
	return what->ComputePoint(tc);
}

PRTVector PRTMesh::ComputeNormal(PRTVector p)
{
	return what->ComputeNormal(p);
}

PRTVector PRTMesh::ComputeTangent(PRTVector p)
{
	return what->ComputeTangent(p);
}

PRTVector PRTMesh::ComputeBinormal(PRTVector p)
{
	return what->ComputeBinormal(p);
}

PRTVector PRTMesh::ComputeColor(PRTVector p)
{
	return what->ComputeColor(p);
}

PRTIntersectPoint PRTMesh::ComputeIntersection(PRTRay r,bool testcull)
{
	PRTIntersectPoint aux;

	PRTArray<PRTObject*> list;
//	bool borra=false;
	//if (!BOctrees || Octrees==NULL) // si lo del octree esrootsolo se tendria que dejar que se pusiera octreesdeep==0
	//	ejem=&ObjectsList;
	//else // tengo en cuenta los octrees
	{
//		borra=true;
		octrees->ReturnObjects(r, NULL, list);
	}

	for (unsigned int i=0;i<list.Length();i++)
	{
		PRTIntersectPoint aux2;
		aux2=list.GetAtPos(i)->ComputeIntersection(r,testcull);
		if (aux2.collision && aux2.distance<aux.distance)
		{
			aux=aux2;
			what=list.GetAtPos(i);
		}
	}

//	if (borra)
//		delete ejem;
		
	return aux;
}
