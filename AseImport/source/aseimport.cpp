// aseimport.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "..\..\PiscisRT\source\PRTMeshImpExp.h"

extern bool ImportASE(LPCSTR,PRTMeshImpExpData*);

// soy un plugin de PISCISRT?
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
	static char res[]="ase";	
	return res;
}

char *Description(void)
{
	static char res[]="Escena ASCII de 3DSMAX";
	return res;
}

bool Import(LPCSTR fich, PRTMeshImpExpData *dat)
{
	return ImportASE(fich,dat);
}


