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

//#include "../extincs/expat.h"
#include "../extincs/Python/Python.h"
#include "../extincs/expatpp.h"
#include "../../PiscisRT/source/PRTMain.h" //trazador
//#include "../extincs/aviFree/AviFree.h"
#include <map>
#include <list>
#include <stack>
#include <string>
using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
	#define __MAKE_WINDOWS__		//windows lib *TODO* don't work
	#define __MAKE_WINDOWS_DLL__	//windows dll use with __MAKE_WINDOWS__
#endif

#if defined(__MAKE_WINDOWS__)

#if _MSC_VER > 1000
#pragma once
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#ifndef __MAKE_WINDOWS_DLL__
	#define PISCISRTUT_API
#else
	//#ifdef WINDOWSVC6_EXPORTS
		#define PISCISRTUT_API __declspec(dllexport)
	//#else
	//	#define PISCISRTUT_API __declspec(dllimport)
	//#endif
#endif
#endif

#define PRT_ANTIALIASING_NONE			101
#define PRT_ANTIALIASING_JITTER			102
#define PRT_ANTIALIASING_5				103
#define PRT_ANTIALIASING_9				104

class PISCISRTUT_API PRTutMain : public expatpp
{

private:

	//SCENE
	string scenename;
	bool ya;
	bool ahora;
	int ancho;
	int alto;
	PRTTexCoord subwidth;
	PRTTexCoord subheight;
	PRTVector camarapos;
	PRTVector camarafin;
	PRTVector camaraup;
	PRTFloat fov;
	int deep;
	map<string,PRTMaterial*> materials;
	map<string,PRTTexture*> textures;
	typedef pair <string,string> auxpair;
	list<auxpair> variables;
	unsigned int antialiasing;
	int itera;
	unsigned int octreesdeep;
	unsigned int octreesnumonleaf;
	bool stereo;
	PRTFloat stereowidth;
	//CSG
	int incsg;
	PRTObject* csgo1;
	PRTObject* csgo2;
	//ROTATE,TRANSLATE,SCALE
	int inrts;
	stack<PRTMatrix> rtsantes;
	PRTMatrix rts;
	//PHOTONMAP
	bool photonmap;
	int numphotones;
	int groupphotones;
	PRTFloat scalephoton;
	//DOF
	PRTVector dofpoint;
	PRTFloat dofaperture;
	bool dof;
	//PATCHES
	bool patches;
	//TRAZADOR
	PRTMain *trazador;
	//ESTADISTICAS
	int tiempoinicio;
	int tiempo;
	double t;
	double tinicio;
	double tfinal;
	double tpaso;
	//THREADS
	int hx;
	int hy;
	int hilos;
	bool esperar;
	//
	PRTVector N;
	PRTVector U;
	PRTVector V;
	PRTFloat fovrad;
	PRTVector Xinc;
	PRTVector Yinc;

	PRTVector colorpixel(PRTVector aux);
	PRTVector jitter(int x, int y,unsigned int itera);
	double parseexpr(const char* c);
	int parseint(const char* c);
	PRTVector parsevector(char* c);
	PRTQuaternion parsequat(char* c);
	PRTFloat parseprtfloat(const char* c);
	PRTTexCoord parsecoord(char* c);
	bool parsebool(const char* c);
	void startElement(const XML_Char* name, const XML_Char** atts);
	void endElement(const XML_Char*);

public:

	PRTutMain(void);
	bool PRTutParse(string,string,PRTMain*,unsigned int&,unsigned int&,unsigned int&,double tiempo=0.0);
	PRTVector PRTutRenderPixel(int,int);
	PRTVector PRTutFilterPixel(int,int);
	~PRTutMain(void);

};