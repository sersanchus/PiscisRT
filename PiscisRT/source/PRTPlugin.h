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

#ifndef __PRTPLUGIN_H__
#define __PRTPLUGIN_H__

#include "PRTUtil.h"

#define PRT_PLUGIN_IDENTIFICATION	"PiscisRTPlugin"
#define PRT_PLUGIN_UNKNOW			9001
#define PRT_PLUGIN_MESH_IMPORT		9002
#define PRT_PLUGIN_BITMAP_IMPORT	9003
#define PRT_PLUGIN_BITMAP_EXPORT	9004

typedef int (* TYPEPROC) (void);
typedef char* (* IDENTIFICATIONPROC) (void);

//--------------------------------------------------------------------------------------
//! The PRTPlugin class.
/*!	This class represents a plugin for the PiscisRT engine.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTPlugin
{

protected:

	unsigned int type;		//!< The type of plugin.
	HMODULE hmodule;		//!< The handle for the plugin.
	
public:
	
	LPCSTR path;			//!< The path to the plugin.
	
//--------------------------------------------------------------------------------------

	//!	A constructor of the PRTPlugin.
	/*!	
		\param dllfile the path to the plugin
	*/
	PRTPlugin(LPCSTR dllfile);

//--------------------------------------------------------------------------------------

	//!	The empty destructor for the PRTPlugin.
	/*!	
	*/
	~PRTPlugin(void);

//--------------------------------------------------------------------------------------

	//!	Returns the type of this PRTPlugin.
	/*!	
	*/
	unsigned int Type(void){ return type; }

//--------------------------------------------------------------------------------------
	
};

bool PRTIsPlugin(LPCSTR dll);
unsigned int PRTTypePlugin(LPCSTR dll);

#endif
