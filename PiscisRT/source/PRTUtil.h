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

#ifndef __PRTUTIL_H__
#define __PRTUTIL_H__

#pragma warning(disable:4786)

//--------------------------------------------------------------------------------------
// choose a platform

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
	#define __MAKE_WINDOWS__		//windows lib *TODO* don't work
	#define __MAKE_WINDOWS_DLL__	//windows dll use with __MAKE_WINDOWS__
#elif defined(CYGWIN) || defined(_CYGWIN) || defined(__CYGWIN) || defined(__CYGWIN__)
	#define __MAKE_CYGWIN__
#else
	#define __MAKE_LINUX__			//linux lib libPiscisRT.a
#endif


//--------------------------------------------------------------------------------------
// on windows

#if defined(__MAKE_WINDOWS__) || defined(__MAKE_CYGWIN__)

#if _MSC_VER > 1000
#pragma once
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#ifndef __MAKE_WINDOWS_DLL__
	#define PISCISRT_API
#else
	#ifdef WINDOWSVC6_EXPORTS
		#define PISCISRT_API __declspec(dllexport)
	#else
		#define PISCISRT_API __declspec(dllimport)
	#endif
#endif

#endif 

//--------------------------------------------------------------------------------------
// on linux

#ifdef __MAKE_LINUX__
	#define PISCISRT_API

#ifndef NULL
	#define NULL 0x00000000
#endif

#ifndef LPCSTR
	typedef const char* LPCSTR; 
#endif

#ifndef HMODULE
	typedef void* HMODULE;
#endif

#ifndef DWORD
	typedef unsigned long DWORD;
#endif

#ifndef HANDLE
	typedef void* HANDLE;
#endif

#ifndef LPSTR
	typedef char* LPSTR;
#endif

#endif
	

//--------------------------------------------------------------------------------------
// on mac
#ifdef __APPLE__
#include <dlfcn.h>
#endif


//--------------------------------------------------------------------------------------
// variables of generic use

#include "PRTMath.h"

class PISCISRT_API PRTVector;

extern PRTFloat faux1,faux2,faux3,faux4,faux5,faux6;
extern unsigned int uiaux1,uiaux2,uiaux3;
extern PRTVector vaux1,vaux2,vaux3;

#endif
