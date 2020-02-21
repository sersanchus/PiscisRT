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

#ifndef __PRTTHREAD_H__
#define __PRTTHREAD_H__

#include "PRTUtil.h"

#ifdef _AIX //problems with aix
	#include "../extincs/sched.h"
	#include "../extincs/pthread.h"
	typedef void * (* FUNCTION) (void*); 
#endif

#ifdef __MAKE_LINUX__
	#include <pthread.h>
	typedef void * (* FUNCTION) (void*);
#endif

#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
	typedef void (* FUNCTION) (void);
#endif

//--------------------------------------------------------------------------------------
//! The PRTThread class.
/*!	This class represents a thread depending on operative system.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTThread
{

public:

	DWORD id;				//!< The identificator of the thread.

	#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
	HANDLE handle;			//!< the handle of the thread.
	#else
	pthread_t handle;		//!< the handle of the thread.
	#endif

	void *param;			//!< the parameters of the thread.
	FUNCTION func;			//!< the function to execute on the thread.

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTThread.
	/*!	
		\param a the function to execute
		\param p the parameters of the function
	*/
	PRTThread(FUNCTION a,void *p=NULL)
	{	
		func=a;
		param=p;
		#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
			handle=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)a, p, CREATE_SUSPENDED, &id);
		#else
			pthread_create(&handle,NULL,a,p);
		#endif
	}

//--------------------------------------------------------------------------------------

	//!	The destructor of the PRTThread.
	/*!	
	*/
	~PRTThread()
	{
		#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
			CloseHandle(handle);
		#else
			pthread_cancel(handle);
		#endif
	}

//--------------------------------------------------------------------------------------

	//!	A function to resume paused thread.
	/*!	
	*/
	void Resume(void)
	{
		#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
			ResumeThread(handle);
		#else
		
		#endif
	}

//--------------------------------------------------------------------------------------

	//!	A function to suspend thread.
	/*!	
	*/
	void Suspend(void)
	{
		#if defined(__MAKE_WINDOWS__) || defined (__MAKE_CYGWIN__)
			SuspendThread(handle);
		#else

		#endif
	}

//--------------------------------------------------------------------------------------
	
};

#endif
