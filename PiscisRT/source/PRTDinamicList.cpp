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
#include "PRTDinamicList.h"

PRTListMember::PRTListMember(void)
{
	next=NULL;
	object=NULL;
}

PRTListMember::~PRTListMember()
{
}

PRTDinamicList::PRTDinamicList(void)
{
	first=NULL;
	last=NULL;
	num=0;
}

void PRTDinamicList::AddAtEnd(void* o)
{
	if (first==NULL)
	{
		first=new PRTListMember;
		first->object=o;
		last=first;
	}
	else
	{
		// very less improved!! :(
		/*PRTListMember *aux1;
		PRTListMember *aux2;
		aux1=first;
		aux2=first->next;
		while (aux2!=NULL)
		{
			aux1=aux2;
			aux2=aux2->next;
		}
		aux2=new PRTListMember;
		aux2->object=o;
		aux1->next=aux2;
		last=aux2;*/

		// very much improved!! :)
		PRTListMember *aux;
		aux=new PRTListMember;
		aux->object=o;
		last->next=aux;
		last=aux;
	}
	num++;
}

void *PRTDinamicList::GetAtPos(unsigned int pos)
{
	/*int count=0;
	PRTListMember *aux;
	aux=first;
	while (aux!=NULL && count!=pos)
	{
		aux=aux->next;
		count++;
	}
	if (aux==NULL)
		return NULL;
	else
		return aux->object;
	*/

	//ENHANCED

	uiaux1=0;
	PRTListMember *aux;
	aux=first;
	while (aux!=NULL && uiaux1!=pos)
	{
		aux=aux->next;
		uiaux1++;
	}
	if (aux==NULL)
		return NULL;
	else
		return aux->object;
}

void PRTDinamicList::Free(void)
{
	PRTListMember *aux;
	aux=first;
	while (aux!=NULL)
	{
		PRTListMember *aux2=aux;
		aux=aux->next;
		//delete aux2->object; // the object still alives *TODO*
		delete aux2;
	}
	first=NULL;
	num=0;
}

PRTDinamicList::~PRTDinamicList(void)
{
	PRTListMember *aux;
	aux=first;
	while (aux!=NULL)
	{
		PRTListMember *aux2=aux;
		aux=aux->next;
		//delete aux2->object; // // the object still alives *TODO*, in the octress is asumed so
		delete aux2;
	}
}

unsigned int PRTDinamicList::Lenght(void)
{
	return num;
}
