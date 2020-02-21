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
#include "PRTMeshImpExp.h"
#include "PRTPlugin.h"

char *PRTMeshImport::Description(void)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	DESCRIPTIONPROC proc=(DESCRIPTIONPROC)GetProcAddress(hmodule,"Description");
#else
	DESCRIPTIONPROC proc=(DESCRIPTIONPROC)dlsym(hmodule,"Description");
#endif
	return proc();
}

char *PRTMeshImport::Extension(void)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	EXTENSIONPROC proc=(EXTENSIONPROC)GetProcAddress(hmodule,"Extension");
#else
	EXTENSIONPROC proc=(EXTENSIONPROC)dlsym(hmodule,"Extension");
#endif
	return proc();
}

bool PRTMeshImport::Import(LPCSTR file,PRTMeshImpExpData *dat)
{
#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)
	IMPORTPROCMESH proc=(IMPORTPROCMESH)GetProcAddress(hmodule,"Import");
#else
	IMPORTPROCMESH proc=(IMPORTPROCMESH)dlsym(hmodule,"Import");
#endif
	return proc(file,dat);		
}
