/**********************************************************************
 *<
	FILE: PRTMaxMaterial.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef __PRTMAXMATERIAL__H
#define __PRTMAXMATERIAL__H

#include "../../PiscisRTFinal/source/PRTMaterial.h"
//#include "Max.h"
#include "resource.h"
//#include "istdplug.h"
//#include "iparamb2.h"
#include "Max.h"
#include "iparamm2.h"


#define PRTMAXMATERIAL_CLASS_ID	Class_ID(0x5ccbd670, 0x459b3054)

#define NSUBMTL		5 // TODO: number of sub-materials supported by this plugin 

#define PBLOCK_REF	NSUBMTL

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;


class PRTMaxMaterial : public Mtl {
	public:


		// Parameter block
		IParamBlock2	*pblock;	//ref 0

		Mtl				*submtl[NSUBMTL];  //array of sub-materials
		BOOL			mapOn[NSUBMTL];
		
		//Interactive Renderer
		Color			color;
		Color			ambiente;
		Color			especular;
		float			shininess;
		float			xparency;
		float			shinstr;
		float			wiresize;
		float			spin;
		float			illumfloat;
		BOOL			illumbool;
		Color			illumcolor;

		//Render
		float			reflection;
		float			refraction;
		float			alpha;
		float			gireflection;
		float			girefraction;
		float			gialpha;

		//PRTMaterial
		PRTMaterial* mat;
		
		
		Interval		ivalid;
		
		ParamDlg *CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		void Update(TimeValue t, Interval& valid);
		Interval Validity(TimeValue t);
		void Reset();

		void NotifyChanged();

		// From MtlBase and Mtl
		void SetAmbient(Color c, TimeValue t);		
		void SetDiffuse(Color c, TimeValue t);		
		void SetSpecular(Color c, TimeValue t);
		void SetShininess(float v, TimeValue t);
		Color GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
	    Color GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
		Color GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
		float GetXParency(int mtlNum=0, BOOL backFace=FALSE);
		float GetShininess(int mtlNum=0, BOOL backFace=FALSE);		
		float GetShinStr(int mtlNum=0, BOOL backFace=FALSE);
		float WireSize(int mtlNum=0, BOOL backFace=FALSE);
		float GetSelfIllum(int mtlNum=0, BOOL backFace=FALSE);
		BOOL GetSelIllumColorOn(int mtlNum=0, BOOL backFace=FALSE);
		Color GetSelIllumColor(int mtlNum=0, BOOL backFace=FALSE);
				

		// Shade and displacement calculation
		void Shade(ShadeContext& sc);
		float EvalDisplacement(ShadeContext& sc); 
		Interval DisplacementValidity(TimeValue t); 	

		// SubMaterial access methods
		int NumSubMtls() {return NSUBMTL;}
		Mtl* GetSubMtl(int i);
		void SetSubMtl(int i, Mtl *m);
		TSTR GetSubMtlSlotName(int i);
		TSTR GetSubMtlTVName(int i);

		// SubTexmap access methods
		int NumSubTexmaps() {return 0;}
		Texmap* GetSubTexmap(int i);
		void SetSubTexmap(int i, Texmap *m);
		TSTR GetSubTexmapSlotName(int i);
		TSTR GetSubTexmapTVName(int i);
		
		BOOL SetDlgThing(ParamDlg* dlg);
		PRTMaxMaterial(BOOL loading);

		
		// Loading/Saving
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);

		//From Animatable
		Class_ID ClassID() {return PRTMAXMATERIAL_CLASS_ID;}		
		SClass_ID SuperClassID() { return MATERIAL_CLASS_ID; }
		void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

		RefTargetHandle Clone( RemapDir &remap );
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
			PartID& partID,  RefMessage message);


		int NumSubs() { return 1+NSUBMTL; }
		Animatable* SubAnim(int i); 
		TSTR SubAnimName(int i);

		// TODO: Maintain the number or references here 
		int NumRefs() { return 1+NSUBMTL; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);



		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

		void DeleteThis() { delete this; }		
		//Constructor/Destructor


};

#endif // __PRTMAXMATERIAL__H
