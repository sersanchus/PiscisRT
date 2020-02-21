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

#ifndef __PRTBITMAPIMPEXP_H__
#define __PRTBITMAPIMPEXP_H__

#include "PRTUtil.h"
#include "PRTPlugin.h"

class PRTBitmapImpExpData
{
public:
	unsigned int bytespp;
	unsigned int width;
	unsigned int height;
	unsigned char *pixels;
	PRTBitmapImpExpData(){pixels=NULL;};
	~PRTBitmapImpExpData(){/*delete pixels;*/} //*TODO*
};

typedef char* (* EXTENSIONPROC) (void);
typedef char* (* DESCRIPTIONPROC) (void);
typedef bool (* IMPORTPROCBITMAP) (LPCSTR,PRTBitmapImpExpData*);

class PISCISRT_API PRTBitmapImport : public PRTPlugin
{
public:
	PRTBitmapImport(LPCSTR dllfile):PRTPlugin(dllfile){}
	char *Description(void); 
	char *Extension(void);
	bool Import(LPCSTR,PRTBitmapImpExpData*);
};

class PISCISRT_API PRTBitmapExport : public PRTPlugin
{
public:
	PRTBitmapExport(LPCSTR dllfile):PRTPlugin(dllfile){}
	char *Description(void); 
	char *Extension(void);
	bool Export(LPCSTR,PRTBitmapImpExpData*);
};

#endif
