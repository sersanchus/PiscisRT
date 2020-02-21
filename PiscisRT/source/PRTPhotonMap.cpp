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
#include "PRTPhotonMap.h"
#include <stdlib.h>
#include <assert.h>

PRTPhotonMap::PRTPhotonMap(const int maxPhot) 
{
	storedphotons=0;
	prevscale=1;
	maxphotons=maxPhot;

	//photons=new Photon[maxPhotons+1];
	photons=(PRTPhoton*)malloc(sizeof(PRTPhoton)*(maxphotons+1));
	if (photons==NULL) 
	{
		fprintf(stderr,"Out of memory initializing photon map.\n");
		exit(EXIT_FAILURE);
	}

	bboxMin[0]=bboxMin[1]=bboxMin[2]=1e8f;
	bboxMax[0]=bboxMax[1]=bboxMax[2]=-1e8f;

	for (int i=0;i<256;i++) 
	{
		PRTFloat angle=(PRTFloat)i*(PRTFloat(0.00390625))*PRTPI;
		PRTFloat angle2=PRTFloat(2.0)*angle;
		costheta[i]=(PRTFloat)PRTCos(angle);
		sintheta[i]=(PRTFloat)PRTSin(angle);
		cosphi[i]=(PRTFloat)PRTCos(angle2);
		sinphi[i]=(PRTFloat)PRTSin(angle2);
	}
}

PRTPhotonMap::~PRTPhotonMap() 
{
	delete [] photons;
}

void PRTPhotonMap::Store(const PRTFloat power[3],const PRTFloat pos[3],const PRTFloat dir[3])
{
	if (storedphotons>=maxphotons)
		return;

	storedphotons++;
	PRTPhoton *const node=&photons[storedphotons];

	for (int i=0;i<3;i++)
	{
		node->pos[i]=pos[i];

		if (node->pos[i]<bboxMin[i])
			bboxMin[i]=node->pos[i];
		if (node->pos[i]>bboxMax[i])
			bboxMax[i]=node->pos[i];

		node->power[i]=power[i];
	}

	int theta=(int)(PRTACos(dir[2])*(81.487330863050411913668486846727));
	if (theta>255)
		node->theta=255;
	else
		node->theta=(unsigned char)theta;

	int phi=(int)(atan2(dir[1],dir[0])*(40.743665431525205956834243423364));
	if (phi>255)
		node->phi=255;
	else if (phi<0)
		node->phi=(unsigned char)(phi+256);
	else
		node->phi=(unsigned char)phi;
}

void PRTPhotonMap::ScalePhotonPower(const PRTFloat scale)
{
	for (int i=prevscale;i<=storedphotons;i++)
	{
		photons[i].power[0]*=scale;
		photons[i].power[1]*=scale;
		photons[i].power[2]*=scale;
	}
	prevscale=storedphotons;
}

void PRTPhotonMap::Balance(void)
{
	if (storedphotons>1)
	{
		//Photon **pa1=(Photon **) new (Photon*)[storedphotons+1];
		PRTPhoton **pa1=(PRTPhoton**)malloc(sizeof(PRTPhoton*)*(storedphotons+1));
		//Photon ** pa2=(Photon **) new (Photon*)[storedphotons+1];
		PRTPhoton **pa2=(PRTPhoton**)malloc(sizeof(PRTPhoton*)*(storedphotons+1));
	
		int i;

		for (i=0;i<=storedphotons;i++)
			pa2[i]=&photons[i];

		BalanceSegment(pa1,pa2,1,1,storedphotons);
		delete []pa2;

		int d,j=1,foo=1;
		PRTPhoton fooPhoton=photons[j];

		for (i=1;i<=storedphotons;i++)
		{
			d=pa1[j]-photons;
			pa1[j]=NULL;
			if (d!=foo)
				photons[j]=photons[d];
			else
			{
				photons[j]=fooPhoton;

				if (i<storedphotons) 
				{
					for (;foo<=storedphotons;foo++ )
						if (pa1[foo]!=NULL)
							break;
					fooPhoton=photons[foo];
					j=foo;
				}
				continue;
			}
			j=d;
		}
		delete []pa1;
	}

	halfstoredphotons=storedphotons/2-1;
}

#define swap(ph,a,b){PRTPhoton *ph2=ph[a];ph[a]=ph[b];ph[b]=ph2;}

void PRTPhotonMap::MedianSplit(PRTPhoton **p,const int start,const int end,const int median,const int axis)
{
	int left=start;
	int right=end;

	while (right>left)
	{
		const PRTFloat v=p[right]->pos[axis];
		int i=left-1;
		int j=right;
		for (;;)
		{
			while (p[++i]->pos[axis]<v)
			;
			while (p[--j]->pos[axis]>v && j>left)
			;
			if (i>=j)
				break;
			swap(p,i,j);
		}

		swap(p,i,right);
		if (i>=median)
			right=i-1;
		if (i<=median)
			left=i+1;
	}
}

void PRTPhotonMap::BalanceSegment(PRTPhoton **pbal,PRTPhoton **porg,const int index,const int start,const int end)
{
	int median=1;

	while ((4*median)<=(end-start+1))
		median += median;

	if ((3*median)<=(end-start+1))
	{
		median+=median;
		median+=start-1;
	}
	else
		median=end-median+1;

	int axis=2;
	if ((bboxMax[0]-bboxMin[0])>(bboxMax[1]-bboxMin[1]) && (bboxMax[0]-bboxMin[0])>(bboxMax[2]-bboxMin[2]))
		axis=0;
	else if ((bboxMax[1]-bboxMin[1])>(bboxMax[2]-bboxMin[2]))
		axis = 1;

	MedianSplit(porg,start,end,median,axis);

	pbal[index]=porg[median];
	pbal[index]->plane=axis;

	if (median>start)
	{
		if (start<median-1)
		{
			const PRTFloat tmp=bboxMax[axis];
			bboxMax[axis]=pbal[index]->pos[axis];
			BalanceSegment(pbal,porg,2*index,start,median-1);
			bboxMax[axis]=tmp;
		}
		else
		{
			pbal[2*index]=porg[start];
		}
	}

	if (median<end)
	{
		if (median+1<end)
		{
			const PRTFloat tmp=bboxMin[axis];
			bboxMin[axis]=pbal[index]->pos[axis];
			BalanceSegment(pbal,porg,2*index+1,median+1,end);
			bboxMin[axis]=tmp;
		}
		else
		{
			pbal[2*index+1]=porg[end];
		}
	}
}

//This estimate is the most simplest
void PRTPhotonMap::IrradianceEstimate(PRTFloat irrad[3],const PRTFloat pos[3],const PRTFloat normal[3],const PRTFloat maxDist,const int nPhotons)
{
	irrad[0]=irrad[1]=irrad[2]=0.0;

	PRTNearestPhotons np;
	//np.dist2 = new PRTFloat[nPhotons + 1];
	//np.index = (const Photon **) new (Photon *)[nPhotons + 1];
	np.dist2=(PRTFloat*)malloc(sizeof(PRTFloat)*(nPhotons+1));
	np.index=(const PRTPhoton**)malloc(sizeof(PRTPhoton*)*(nPhotons+1));

	np.pos[0]=pos[0];np.pos[1]=pos[1];np.pos[2]=pos[2];
	np.max=nPhotons;
	np.found=0;
	np.gotHeap=0;
	np.dist2[0]=maxDist*maxDist;

	LocatePhotons(&np,1);

	if(np.found<8)
	{
		delete []np.dist2;
		delete []np.index;
		return;
	}

	PRTFloat pdir[3];

	for (int i=1;i<=np.found;i++)
	{
		const PRTPhoton *p=np.index[i];
		PhotonDir(pdir,p);
		if ((pdir[0]*normal[0]+pdir[1]*normal[1]+pdir[2]*normal[2])<0.0f)
		{
			irrad[0]+=p->power[0];
			irrad[1]+=p->power[1];
			irrad[2]+=p->power[2];
		}
	}

	const PRTFloat tmp=(PRTFloat)((0.31830988618379067153776752674503)/(np.dist2[0]));//estimate of density
	irrad[0]*=tmp;
	irrad[1]*=tmp;
	irrad[2]*=tmp;

	delete []np.dist2;
	delete []np.index;
}

void PRTPhotonMap::LocatePhotons(PRTNearestPhotons *const np,const int index)
{
	const PRTPhoton *p=&photons[index];
	PRTFloat dist1;

	if (index<halfstoredphotons)
	{
		dist1=np->pos[p->plane]-p->pos[p->plane];

		if (dist1>0.0)
		{
			LocatePhotons(np,2*index+1);
			if (dist1*dist1<np->dist2[0])
				LocatePhotons(np,2*index);
		}
		else
		{
			LocatePhotons(np,2*index);
			if (dist1*dist1<np->dist2[0])
				LocatePhotons(np,2*index+1);
		}
	}

	// Compute squared distance between current photon and np->pos

	dist1=p->pos[0]-np->pos[0];
	PRTFloat dist2=dist1*dist1;
	dist1=p->pos[1]-np->pos[1];
	dist2+=dist1*dist1;
	dist1=p->pos[2]-np->pos[2];
	dist2+=dist1*dist1;

	if (dist2<np->dist2[0])
	{
		// Found a photon.  Insert in the candidate list.

		if (np->found<np->max)
		{
			// Heap is not full.  Use array.
			np->found++;
			np->dist2[np->found]=dist2;
			np->index[np->found]=p;
		}
		else
		{
			int j,parent;
			if (np->gotHeap==0)
			{
				// Build heap
				PRTFloat dst2;
				const PRTPhoton*phot;
				int halfFound=np->found>>1;
				for (int k=halfFound;k>=1;k--)
				{
					parent=k;
					phot=np->index[k];
					dst2=np->dist2[k];
					while (parent<=halfFound)
					{
						j=parent+parent;
						if (j<np->found && np->dist2[j]<np->dist2[j+1])
							j++;
						if (dst2>=np->dist2[j])
							break;
						np->dist2[parent]=np->dist2[j];
						np->index[parent]=np->index[j];
						parent=j;
					}
					np->dist2[parent]=dst2;
					np->index[parent]=phot;
				}
				np->gotHeap=1;
			}

			// Insert new photon into max heap
			// Delete largest element, insert new, and reorder the heap

			parent=1;
			j=2;
			while (j<=np->found)
			{
				if (j<np->found && np->dist2[j]<np->dist2[j+1])
					j++;
				if (dist2>np->dist2[j])
					break;
				np->dist2[parent]=np->dist2[j];
				np->index[parent]=np->index[j];
				parent=j;
				j+=j;
			}
			np->index[parent]=p;
			np->dist2[parent]=dist2;

			np->dist2[0]=np->dist2[1];
		}
	}
}

void PRTPhotonMap::PhotonDir(PRTFloat *dir,const PRTPhoton *p)
{
	dir[0]=sintheta[p->theta]*cosphi[p->phi];
	dir[1]=sintheta[p->theta]*sinphi[p->phi];
	dir[2]=costheta[p->theta];
}

void PRTPhotonMap::WriteToDisk(const char *fileName) 
{
	FILE *file=fopen(fileName,"wb");

	fwrite(photons,sizeof(PRTPhoton),storedphotons,file);
	fclose(file);

	printf("Wrote %d photons to file %s.\n",storedphotons,fileName);
}

void PRTPhotonMap::ReadFromDisk(const char *fileName,int numPhotons)
{
	FILE *file=fopen(fileName,"rb");

	if (file==NULL) 
		fprintf(stderr,"Failed to open photon map file %s.\n",fileName);

	assert((int)(photons,sizeof(PRTPhoton),numPhotons,file)==numPhotons);
	fclose(file);

	storedphotons=numPhotons;
	halfstoredphotons=storedphotons/2-1;

	printf("Read %d photons from file %s.\n",numPhotons,fileName);
}

bool PRTPhotonMap::Empty(void)
{
	return storedphotons==0;
}

void PRTPhotonMap::Free(void)
{
	delete []photons;

	storedphotons=0;
	prevscale=1;

	//photons=new Photon[maxphotons+1];
	photons=(PRTPhoton*)malloc(sizeof(PRTPhoton)*(maxphotons+1));
	if (photons==NULL) 
	{
		fprintf(stderr,"Out of memory initializing photon map.\n");
		exit(EXIT_FAILURE);
	}

	bboxMin[0]=bboxMin[1]=bboxMin[2]=1e8f;
	bboxMax[0]=bboxMax[1]=bboxMax[2]=-1e8f;

	for (int i=0;i<256;i++)
	{
		PRTFloat angle=(PRTFloat)i*PRTFloat(0.012271846303085129837744700715936);
		costheta[i]=(PRTFloat)PRTCos(angle);
		sintheta[i]=(PRTFloat)PRTSin(angle);
		cosphi[i]=(PRTFloat)PRTCos(PRTFloat(2.0)*angle);
		sinphi[i]=(PRTFloat)PRTSin(PRTFloat(2.0)*angle);
	}
}

int PRTPhotonMap::StoredPhotons(void)
{
	return storedphotons;
}
