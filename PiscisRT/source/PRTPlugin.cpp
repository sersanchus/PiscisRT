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
#include "PRTPlugin.h"
#ifdef __MAKE_LINUX__
	#include <dlfcn.h>
#endif

#ifdef __APPLE__
#include <string.h>
#endif

PRTPlugin::PRTPlugin(LPCSTR dllfile)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	hmodule=LoadLibrary(dllfile);
	TYPEPROC proc=(TYPEPROC)GetProcAddress(hmodule,"Type");
#else
	hmodule=dlopen(dllfile,RTLD_NOW);
	TYPEPROC proc=(TYPEPROC)dlsym(hmodule,"Type");
#endif
	type=proc();	
}

PRTPlugin::~PRTPlugin(void)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	FreeLibrary(hmodule);
#else
	dlclose(hmodule);	
#endif
}

bool PRTIsPlugin(LPCSTR dll)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	HMODULE hmodule=LoadLibrary(dll);
#else
	HMODULE hmodule=dlopen(dll,RTLD_NOW);
#endif
	if (!hmodule)
		return false;
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	IDENTIFICATIONPROC proc=(IDENTIFICATIONPROC)GetProcAddress(hmodule,"Identification");
#else
	IDENTIFICATIONPROC proc=(IDENTIFICATIONPROC)dlsym(hmodule,"Identification");
#endif
	if (!proc)
		return false;
	
	/*char* jar1;
	jar1=proc();
	char* jar2;
	jar2=PRT_PLUGIN_IDENTIFICATION;

	bool res=false;
	int pos=0;
	while (1)
	{
		if (jar1[pos]!=jar2[pos])
		{
			res=false;
			break;
		}
		pos++;
		if (jar1[pos]=='\0')
		{
			res=true;
			break;
		}
	}*/


	bool res=strcmp(proc(),PRT_PLUGIN_IDENTIFICATION)==0;

	//bool res=((char*)jar==(char*)PLUGIN_IDENTIFICACION);
	//bool res=strcmp(jar1,jar2);
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	FreeLibrary(hmodule);
#else
	dlclose(hmodule);
#endif	
	return res;
}

unsigned int PRTTypePlugin(LPCSTR dll)
{
	unsigned int res;
	if (PRTIsPlugin(dll))
	{
		#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
			HMODULE hmodule=LoadLibrary(dll);
		#else
			HMODULE hmodule=dlopen(dll,RTLD_NOW);
		#endif
		if (!hmodule)
			return PRT_PLUGIN_UNKNOW;
		#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
			TYPEPROC proc=(TYPEPROC)GetProcAddress(hmodule,"Type");
		#else
			TYPEPROC proc=(TYPEPROC)dlsym(hmodule,"Type");
		#endif		
		if (!proc)
			return PRT_PLUGIN_UNKNOW;
		res=proc();
		#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
			FreeLibrary(hmodule);
		#else
			dlclose(hmodule);
		#endif
	}
	else
		res=PRT_PLUGIN_UNKNOW;
	return res;
}

