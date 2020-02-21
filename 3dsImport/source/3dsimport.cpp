#include <windows.h>
#include "..\..\PiscisRT\source\PRTMeshImpExp.h"
#include "..\extincs/file.h"                        
#include "..\extincs/camera.h"
#include "..\extincs/mesh.h"
#include "..\extincs/node.h"
#include "..\extincs/material.h"
#include "..\extincs/matrix.h"
#include "..\extincs/vector.h"
#include "..\extincs/light.h"

#include <vector>


bool Import3ds(LPCSTR fich,PRTMeshImpExpData *dat);

char * Identification(void)
{
	static char res[]=PRT_PLUGIN_IDENTIFICATION;
	return res;
}

int Type(void)
{
	return PRT_PLUGIN_MESH_IMPORT;
}

char *Extension(void)
{
	static char res[]="3ds";	
	return res;
}

char *Description(void)
{
	static char res[]="Escena 3ds de 3DSMAX";
	return res;
}

bool Import(LPCSTR fich,PRTMeshImpExpData *dat)
{
	return Import3ds(fich,dat);
}

void trans(float* p,Lib3dsMatrix* M)
{
	float x,y,z;
	x=M[0][0][0]*p[0]+M[0][0][1]*p[1]+M[0][0][2]*p[2];
	y=M[0][1][0]*p[0]+M[0][1][1]*p[1]+M[0][1][2]*p[2];
	z=M[0][2][0]*p[0]+M[0][2][1]*p[1]+M[0][2][2]*p[2];

	float nx,ny,nz;
	nx=M[0][0][0]*p[3]+M[0][0][1]*p[4]+M[0][0][2]*p[5];
	ny=M[0][1][0]*p[3]+M[0][1][1]*p[4]+M[0][1][2]*p[5];
	nz=M[0][2][0]*p[3]+M[0][2][1]*p[4]+M[0][2][2]*p[5];

	p[0]=x;
	p[1]=y;
	p[2]=z;
	p[3]=nx;
	p[4]=ny;
	p[5]=nz;
}

bool Import3ds(LPCSTR fich,PRTMeshImpExpData *dat)
{
	// cargo el fichero

	Lib3dsFile *file=0;
	file=lib3ds_file_load(fich);
	if (!file)
		return 0;
	
	// evaluo el frame 0

	lib3ds_file_eval(file,0); // 0 es el número de frame
	
	// recorro los meshes y almaceno la información
	
	std::vector<float**> matrixes;
	typedef std::vector<float*> infmesh;
	std::vector<infmesh> inf;
	
	Lib3dsMesh *mesh=file->meshes;
	while (mesh!=NULL)
	{

		//nueva malla
		infmesh meshaux;

		unsigned p;
		Lib3dsVector *normalL=new Lib3dsVector[3*mesh->faces];
		
		//matriz de transformación
		
		Lib3dsMatrix M;
		lib3ds_matrix_copy(M, mesh->matrix);
		lib3ds_matrix_inv(M);

		matrixes.push_back((float**)M);
									
		lib3ds_mesh_calculate_normals(mesh, normalL);

		for (p=0; p<mesh->faces; ++p) 
		{
			Lib3dsFace *f=&mesh->faceL[p];		
		  
			for (int i=0;i<3;i++)
			{
				float *jar=new float[8];
				jar[0]=mesh->pointL[f->points[i]].pos[0];
				jar[1]=mesh->pointL[f->points[i]].pos[1];
				jar[2]=mesh->pointL[f->points[i]].pos[2];
				jar[3]=normalL[3*p+i][0];
				jar[4]=normalL[3*p+i][1];
				jar[5]=normalL[3*p+i][2];
				jar[6]=0;//mesh->texelL[f->points[i]][0];
				jar[7]=0;//mesh->texelL[f->points[i]][1];
					
				meshaux.push_back(jar);
			}
					  		
		}

		delete[] normalL;
		inf.push_back(meshaux);

		mesh=mesh->next;
	}

	/*Lib3dsNode *p1,*p2;
	for (p1=file->nodes; p1!=0; p1=p1->next) 
	{

		if (p1->type==LIB3DS_OBJECT_NODE) 
		{
			if (strcmp(p1->name,"$$$DUMMY")==0) 
				break;
			
			Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, p1->name);
			if (!mesh)
				break;
		
			//nueva malla
			infmesh meshaux;

			unsigned p;
			Lib3dsVector *normalL=new Lib3dsVector[3*mesh->faces];
			
			//matriz de transformación
			
			Lib3dsMatrix M;
			lib3ds_matrix_copy(M, mesh->matrix);
			lib3ds_matrix_inv(M);

			matrixes.push_back((float**)M);
										
			lib3ds_mesh_calculate_normals(mesh, normalL);

			for (p=0; p<mesh->faces; ++p) 
			{
				Lib3dsFace *f=&mesh->faceL[p];		
			  
				for (int i=0;i<3;i++)
				{
					float *jar=new float[8];
					jar[0]=mesh->pointL[f->points[i]].pos[0];
					jar[1]=mesh->pointL[f->points[i]].pos[1];
					jar[2]=mesh->pointL[f->points[i]].pos[2];
					jar[3]=normalL[3*p+i][0];
					jar[4]=normalL[3*p+i][1];
					jar[5]=normalL[3*p+i][2];
					jar[6]=0;//mesh->texelL[f->points[i]][0];
					jar[7]=0;//mesh->texelL[f->points[i]][1];
						
					meshaux.push_back(jar);
				}
					  			
			}

			delete[] normalL;
			inf.push_back(meshaux);
		}

		for (p2=p1->childs; p2!=0; p2=p2->next) 
		{
			if (p2->type==LIB3DS_OBJECT_NODE) 
			{
				if (strcmp(p2->name,"$$$DUMMY")==0) 
					break;
				
				Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, p2->name);
				if (!mesh)
					break;
			
				//nueva malla
				infmesh meshaux;

				unsigned p;
				Lib3dsVector *normalL=new Lib3dsVector[3*mesh->faces];
				
				//matriz de transformación
				
				Lib3dsMatrix M;
				lib3ds_matrix_copy(M, mesh->matrix);
				lib3ds_matrix_inv(M);

				matrixes.push_back((float**)M);
			
				lib3ds_mesh_calculate_normals(mesh, normalL);

				for (p=0; p<mesh->faces; ++p) 
				{
					Lib3dsFace *f=&mesh->faceL[p];
				  
					for (int i=0;i<3;i++)
					{
						float *jar=new float[8];
						jar[0]=mesh->pointL[f->points[i]].pos[0];
						jar[1]=mesh->pointL[f->points[i]].pos[1];
						jar[2]=mesh->pointL[f->points[i]].pos[2];
						jar[3]=normalL[3*p+i][0];
						jar[4]=normalL[3*p+i][1];
						jar[5]=normalL[3*p+i][2];
						jar[6]=mesh->texelL[f->points[i]][0];
						jar[7]=mesh->texelL[f->points[i]][1];
							
						meshaux.push_back(jar);
					}
				  
				}

				delete[] normalL;
				inf.push_back(meshaux);
			}
		}
    }*/

	// volcar la información en PRTMeshImpExpData
	//
	// inf es un vector a mallas
	// cada malla contiene un vector de vertices
	// cada vertice es x,y,z,nx,ny,nz,u,v
	//

	dat->nummesh=inf.size();
	dat->geomdat=new PRTMeshImpExpObject[dat->nummesh];
	for (int i=0;i<dat->nummesh;i++)
	{
		dat->geomdat[i].numv=inf[i].size();
		dat->geomdat[i].vertexes=new PRTMeshImpExpVertex[dat->geomdat[i].numv];
		for (int j=0;j<dat->geomdat[i].numv;j++)
		{
			trans(inf[i][j],(Lib3dsMatrix*)matrixes[i]);
			dat->geomdat[i].vertexes[j].x=inf[i][j][0];
			dat->geomdat[i].vertexes[j].y=inf[i][j][1];
			dat->geomdat[i].vertexes[j].z=inf[i][j][2];
			dat->geomdat[i].vertexes[j].nx=inf[i][j][3];
			dat->geomdat[i].vertexes[j].ny=inf[i][j][4];
			dat->geomdat[i].vertexes[j].nz=inf[i][j][5];
			dat->geomdat[i].vertexes[j].u=inf[i][j][6];
			dat->geomdat[i].vertexes[j].v=inf[i][j][7];
		}
		inf[i].clear();
	}

	inf.clear();
	matrixes.clear();

	return 1;
}