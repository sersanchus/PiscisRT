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
#include "PRTOcTree.h"
#include "PRTVector.h"
#include "PRTObject.h"

PRTDinamicList *objini;

PRTOcTreeNode::PRTOcTreeNode(PRTFloat mx,PRTFloat my,PRTFloat mz,PRTFloat Mx,PRTFloat My,PRTFloat Mz,bool leaf2,PRTOcTreeNode* wasfather)
{
	minx=mx;miny=my;minz=mz;maxx=Mx;maxy=My;maxz=Mz;
	objects=NULL;
	n1=n2=n3=n4=n5=n6=n7=n8=NULL;
	leaf=leaf2;
	father=wasfather;

	//saves all the objects that are inside the leaf if this node is a leaf

	if (father==NULL) //father node
	{
		objects=new PRTDinamicList();
		PRTListMember *o=objini->first;
		while (o!=NULL)
		{		
			// if the object is inside the node save it
			
			if ((PRTVector(minx,miny,minz)<=((PRTObject*)(o->object))->convexhull.chend) && (PRTVector(maxx,maxy,maxz)>=((PRTObject*)(o->object))->convexhull.chbeg))
			{
				objects->AddAtEnd(o->object);
			}
			o=o->next;	
		}
	}
	else
	{
		objects=new PRTDinamicList();
		PRTListMember *o=father->objects->first;
		while (o!=NULL)
		{		
			// if the object is inside the node save it
			
			if ((PRTVector(minx,miny,minz)<=((PRTObject*)(o->object))->convexhull.chend) && (PRTVector(maxx,maxy,maxz)>=((PRTObject*)(o->object))->convexhull.chbeg))
			{
				objects->AddAtEnd(o->object);
			}
			o=o->next;	
		}
	}
}

PRTOcTreeNode::~PRTOcTreeNode()
{
	if (objects)
	{
		//for (unsigned int i=0;i<objects->Lenght();i++)
		//	delete ((PRTObject*)(objects->GetAtPos(i)));
		delete objects; // it don't delete the objects
	}
}

void PRTOcTreeNode::FreeObjects()
{
	if (objects)
	{
		//for (unsigned int i=0;i<objects->Lenght();i++)
		//	delete ((PRTObject*)(objects->GetAtPos(i)));
		delete objects;
		objects=NULL;
	}
}

PRTOcTreeNode* recursive(PRTFloat mx,PRTFloat my,PRTFloat mz,PRTFloat Mx,PRTFloat My,PRTFloat Mz,unsigned int deep,unsigned int numonleaf,PRTOcTreeNode* wasfather)
{
	PRTOcTreeNode* aux;
	if(deep>1) // the branch
	{
		aux=new PRTOcTreeNode(mx,my,mz,Mx,My,Mz,false,wasfather);
		if (aux->objects->num>numonleaf)
		{
			aux->n1=recursive(	mx,			my,			mz,			(Mx+mx)/2,	(My+my)/2,	(Mz+mz)/2,	deep-1, numonleaf,	aux);
			aux->n2=recursive(	(Mx+mx)/2,	my,			mz,			Mx,			(My+my)/2,	(Mz+mz)/2,	deep-1, numonleaf,	aux);
			aux->n3=recursive(	mx,			(My+my)/2,	mz,			(Mx+mx)/2,	My,			(Mz+mz)/2,	deep-1, numonleaf,	aux);
			aux->n4=recursive(	(Mx+mx)/2,	(My+my)/2,	mz,			Mx,			My,			(Mz+mz)/2,	deep-1, numonleaf,	aux);
			aux->n5=recursive(	mx,			my,			(Mz+mz)/2,	(Mx+mx)/2,	(My+my)/2,	Mz,			deep-1, numonleaf,	aux);
			aux->n6=recursive(	(Mx+mx)/2,	my,			(Mz+mz)/2,	Mx,			(My+my)/2,	Mz,			deep-1, numonleaf,	aux);
			aux->n7=recursive(	mx,			(My+my)/2,	(Mz+mz)/2,	(Mx+mx)/2,	My,			Mz,			deep-1, numonleaf,	aux);
			aux->n8=recursive(	(Mx+mx)/2,	(My+my)/2,	(Mz+mz)/2,	Mx,			My,			Mz,			deep-1, numonleaf,	aux);
			aux->FreeObjects();
		}
		else
		{
			aux->leaf=true;			
			/*aux->FreeObjects();
			delete aux;
			aux=NULL;*/
		}
	}
	else if (deep==1) // the leafs
	{
		aux=new PRTOcTreeNode(mx,my,mz,Mx,My,Mz,false,wasfather);
		if (aux->objects->num>0)
		{
			aux->n1=new PRTOcTreeNode(	mx,			my,			mz,			(Mx+mx)/2,	(My+my)/2,	(Mz+mz)/2,	true,aux);
			if (aux->n1->objects->num==0){ delete aux->n1; aux->n1=NULL;}
			aux->n2=new PRTOcTreeNode(	(Mx+mx)/2,	my,			mz,			Mx,		 	(My+my)/2,	(Mz+mz)/2,	true,aux);
			if (aux->n2->objects->num==0){ delete aux->n2; aux->n2=NULL;}
			aux->n3=new PRTOcTreeNode(	mx,			(My+my)/2,	mz,			(Mx+mx)/2,	My,			(Mz+mz)/2,	true,aux);
			if (aux->n3->objects->num==0){ delete aux->n3; aux->n3=NULL;}
			aux->n4=new PRTOcTreeNode(	(Mx+mx)/2,	(My+my)/2,	mz,			Mx,			My,			(Mz+mz)/2,	true,aux);
			if (aux->n4->objects->num==0){ delete aux->n4; aux->n4=NULL;}
			aux->n5=new PRTOcTreeNode(	mx,			my,			(Mz+mz)/2,	(Mx+mx)/2,	(My+my)/2,	Mz,			true,aux);
			if (aux->n5->objects->num==0){ delete aux->n5; aux->n5=NULL;}
			aux->n6=new PRTOcTreeNode(	(Mx+mx)/2,	my,			(Mz+mz)/2,	Mx,			(My+my)/2,	Mz,			true,aux);
			if (aux->n6->objects->num==0){ delete aux->n6; aux->n6=NULL;}
			aux->n7=new PRTOcTreeNode(	mx,			(My+my)/2,	(Mz+mz)/2,	(Mx+mx)/2,	My,			Mz,			true,aux);
			if (aux->n7->objects->num==0){ delete aux->n7; aux->n7=NULL;}
			aux->n8=new PRTOcTreeNode(	(Mx+mx)/2,	(My+my)/2,	(Mz+mz)/2,	Mx,			My,			Mz,			true,aux);
			if (aux->n8->objects->num==0){ delete aux->n8; aux->n8=NULL;}
		}
		aux->FreeObjects();
	}
	return aux;
}

PRTOcTree::PRTOcTree(PRTFloat mx,PRTFloat my,PRTFloat mz,PRTFloat Mx,PRTFloat My,PRTFloat Mz,unsigned int deep2,unsigned int numonleaf2,PRTDinamicList *oini)
{
	root=NULL;
	objini=oini;
	numonleaf=numonleaf2;
	deep=deep2;
	if (deep>0)
		root=recursive(mx,my,mz,Mx,My,Mz,deep,numonleaf,NULL); //it makes all the work
}

void findrecursive(PRTRay* r,PRTObject* discarded,PRTOcTreeNode* aux,PRTDinamicList* exit)
{
	if (aux)
	{

		PRTFloat tnear=-PRTINFINITE;
		PRTFloat tfar=PRTINFINITE;

		if (r->dir.x==0){if (r->orig.x<aux->minx || r->orig.x>aux->maxx) return;}
		else 
		{
			PRTFloat t1=(aux->minx-r->orig.x)/r->dir.x;
			PRTFloat t2=(aux->maxx-r->orig.x)/r->dir.x;
			if (t1>t2)
			{
				PRTFloat penemore=t1;
				t1=t2;
				t2=penemore;
			}
			if (t1>tnear) tnear=t1;
			if (t2<tfar) tfar=t2;
			if (tnear>tfar)	return;
			if (tfar<0)	return;
		}
		if (r->dir.y==0){if (r->orig.y<aux->miny || r->orig.y>aux->maxy) return;}
		else
		{
			PRTFloat t1=(aux->miny-r->orig.y)/r->dir.y;
			PRTFloat t2=(aux->maxy-r->orig.y)/r->dir.y;
			if (t1>t2)
			{
				PRTFloat penemore=t1;
				t1=t2;
				t2=penemore;
			}
			if (t1>tnear) tnear=t1;
			if (t2<tfar) tfar=t2;
			if (tnear>tfar)	return;
			if (tfar<0)	return;
		}
		if (r->dir.z==0){if (r->orig.z<aux->minz || r->orig.z>aux->maxz) return;}
		else
		{
			PRTFloat t1=(aux->minz-r->orig.z)/r->dir.z;
			PRTFloat t2=(aux->maxz-r->orig.z)/r->dir.z;
			if (t1>t2)
			{
				PRTFloat penemore=t1;
				t1=t2;
				t2=penemore;
			}
			if (t1>tnear) tnear=t1;
			if (t2<tfar) tfar=t2;
			if (tnear>tfar)	return;
			if (tfar<0)	return;
		}

		if (aux->leaf)
		{
			PRTListMember *o=aux->objects->first;
			while (o!=NULL)
			{		
				if (((PRTObject*)(o->object))!=discarded)
					exit->AddAtEnd(o->object);
				o=o->next;	
			}
		}
		else
		{
			findrecursive(r,discarded,aux->n1,exit);
			findrecursive(r,discarded,aux->n2,exit);
			findrecursive(r,discarded,aux->n3,exit);
			findrecursive(r,discarded,aux->n4,exit);
			findrecursive(r,discarded,aux->n5,exit);
			findrecursive(r,discarded,aux->n6,exit);
			findrecursive(r,discarded,aux->n7,exit);
			findrecursive(r,discarded,aux->n8,exit);
		}
	}
}

PRTDinamicList* PRTOcTree::ReturnObjects(PRTRay ray,PRTObject* d)
{
	PRTDinamicList* aux=new PRTDinamicList();
	findrecursive(&ray,d,root,aux);
	return aux;
}

void deleterecursive(PRTOcTreeNode* aux)
{
	if (aux)
	{
		deleterecursive(aux->n1);
		deleterecursive(aux->n2);
		deleterecursive(aux->n3);
		deleterecursive(aux->n4);
		deleterecursive(aux->n5);
		deleterecursive(aux->n6);
		deleterecursive(aux->n7);
		deleterecursive(aux->n8);
		delete aux;
	}
}

PRTOcTree::~PRTOcTree()
{
	deleterecursive(root);
}
