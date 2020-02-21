/**********************************************************************
 *<
	FILE: PRTMaxMaterial.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "PRTMaxMaterial.h"

class PRTMaxMaterialClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading) { return new PRTMaxMaterial(loading); }

	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return MATERIAL_CLASS_ID; }
	Class_ID		ClassID() { return PRTMAXMATERIAL_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("PRTMaxMaterial"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

	//TCHAR* GetRsrcString(INT_PTR id){ return id != 0 ? (TCHAR*)id : NULL; } // PQ??

};



static PRTMaxMaterialClassDesc PRTMaxMaterialDesc;
ClassDesc2* GetPRTMaxMaterialDesc() { return &PRTMaxMaterialDesc; }


enum { prtmaxmaterial_params };


//TODO: Add enums for various parameters
enum { 
	mtl_mat1,
	mtl_mat1_on,
	p_color,
	p_ambiente,
	p_especular,
	p_shininess,
	p_xparency,
	p_shinstr,
	p_wiresize,
	p_illumfloat,
	p_illumbool,
	p_illumcolor,
	p_spin,
	p_gireflection,
	p_girefraction,
	p_gialpha,
	p_reflection,
	p_refraction,
	p_alpha,
};



static ParamBlockDesc2 prtmaxmaterial_param_blk ( prtmaxmaterial_params, _T("params"), 0, &PRTMaxMaterialDesc, 
	(P_AUTO_CONSTRUCT + P_AUTO_UI), PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	mtl_mat1,		_T("mtl_mat1"),		TYPE_MTL,		P_OWNERS_REF,	IDS_MTL1,			p_refno,		0,						p_submtlno,		0,						p_ui,			TYPE_MTLBUTTON,			IDC_MTL1,		end,
	mtl_mat1_on,	_T("mtl_mat1_on"),	TYPE_BOOL,		0,				IDS_MTL1ON,			p_default,		TRUE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_MTLON1,		end,
	
	p_color,		_T("color"),		TYPE_RGBA,		0,				IDS_COLOR,			p_default,		Point3(1.0f,1.0f,1.0f),	p_ms_default,	Point3(1.0f,1.0f,1.0f),	p_ui,			TYPE_COLORSWATCH,		IDC_Color,		end,
	p_ambiente,		_T("ambiente"),		TYPE_RGBA,		0,				IDS_AMBIENTE,		p_default,		Point3(1.0f,1.0f,1.0f),	p_ms_default,	Point3(1.0f,1.0f,1.0f),	p_ui,			TYPE_COLORSWATCH,		IDC_Ambiente,	end,
	p_especular,	_T("especular"),	TYPE_RGBA,		0,				IDS_ESPECULAR,		p_default,		Point3(1.0f,1.0f,1.0f),	p_ms_default,	Point3(1.0f,1.0f,1.0f),	p_ui,			TYPE_COLORSWATCH,		IDC_Especular,	end,
	p_shininess,	_T("shininess"),	TYPE_FLOAT,		0,				IDS_SHININESS,		p_default,		0.1f,					p_range,		0.0f,					100.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_SHININESS,		IDC_SHININESSSPIN,		0.01f,		end,
	p_xparency,		_T("xparency"),		TYPE_FLOAT,		0,				IDS_XPARENCY,		p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_XPARENCY,		IDC_XPARENCYSPIN,		0.01f,		end,
	p_shinstr,		_T("shinstr"),		TYPE_FLOAT,		0,				IDS_SHINSTR,		p_default,		0.1f,					p_range,		0.0f,					100.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_SHINSTR,		IDC_SHINSTRSPIN,		0.01f,		end,
	p_wiresize,		_T("wiresize"),		TYPE_FLOAT,		0,				IDS_WIRESIZE,		p_default,		0.0f,					p_range,		0.0f,					100.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_WIRESIZE,		IDC_WIRESIZESPIN,		0.01f,		end,
	p_illumfloat,	_T("illumfloat"),	TYPE_FLOAT,		0,				IDS_ILLUMFLOAT,		p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_ILLUMFLOAT,		IDC_ILLUMFLOATSPIN,		0.01f,		end,
	p_illumbool,	_T("illumbool"),	TYPE_BOOL,		0,				IDS_ILLUMBOOL,		p_default,		FALSE,					p_ui,			TYPE_SINGLECHEKBOX,		IDC_ILLUMBOOL,	end,
	p_illumcolor,	_T("illumcolor"),	TYPE_RGBA,		0,				IDS_ILLUMCOLOR,		p_default,		Point3(1.0f,1.0f,1.0f),	p_ms_default,	Point3(1.0f,1.0f,1.0f),	p_ui,			TYPE_COLORSWATCH,		IDC_ILLUMCOLOR,	end,
		
	p_spin,			_T("spin"),			TYPE_FLOAT,		P_ANIMATABLE,	IDS_SPIN,			p_default,		0.1f,					p_range,		0.0f,					1000.0f,		p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_EDIT,			IDC_SPIN,				0.01f,		end,
	
	p_gireflection,	_T("gireflection"),	TYPE_FLOAT,		0,				IDS_GIREFLECTION,	p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_GIREFLECTION,	IDC_GIREFLECTIONSPIN,	0.01f,		end,
	p_girefraction,	_T("girefraction"),	TYPE_FLOAT,		0,				IDS_GIREFRACTION,	p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_GIREFRACTION,	IDC_GIREFRACTIONSPIN,	0.01f,		end,
	p_gialpha,		_T("gialpha"),		TYPE_FLOAT,		0,				IDS_GIALPHA,		p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_GIALPHA,		IDC_GIALPHASPIN,		0.01f,		end,
	p_reflection,	_T("reflection"),	TYPE_FLOAT,		0,				IDS_REFLECTION,		p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_REFLECTION,		IDC_REFLECTIONSPIN,		0.01f,		end,
	p_refraction,	_T("refraction"),	TYPE_FLOAT,		0,				IDS_REFRACTION,		p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_REFRACTION,		IDC_REFRACTIONSPIN,		0.01f,		end,
	p_alpha,		_T("alpha"),		TYPE_FLOAT,		0,				IDS_ALPHA,			p_default,		0.0f,					p_range,		0.0f,					1.0f,			p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_ALPHA,			IDC_ALPHASPIN,			0.01f,		end,
	
	end
	);




PRTMaxMaterial::PRTMaxMaterial(BOOL loading) 
{
	for (int i=0; i<NSUBMTL; i++) submtl[i] = NULL;
	pblock = NULL;

	mat=new PRTMaterial();

	if (!loading) 
		Reset();
}


void PRTMaxMaterial::Reset() 
{
	ivalid.SetEmpty();
	for (int i=0; i<NSUBMTL; i++) {
		if( submtl[i] ){ 
			DeleteReference(i);
			submtl[i] = NULL;
		}

	}

	PRTMaxMaterialDesc.MakeAutoParamBlocks(this);
}



ParamDlg* PRTMaxMaterial::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = PRTMaxMaterialDesc.CreateParamDlgs(hwMtlEdit, imp, this);
	
	// TODO: Set param block user dialog if necessary
	return masterDlg;
}

BOOL PRTMaxMaterial::SetDlgThing(ParamDlg* dlg)
{
	return FALSE;
}

Interval PRTMaxMaterial::Validity(TimeValue t)
{
	Interval valid = FOREVER;		

	for (int i=0; i<NSUBMTL; i++) 
	{
		if (submtl[i]) 
			valid &= submtl[i]->Validity(t);
	}
	
	float u;
	pblock->GetValue(p_spin,t,u,valid);
	Color c;
	pblock->GetValue(p_color,t,c,valid);
	pblock->GetValue(p_ambiente,t,c,valid);
	pblock->GetValue(p_especular,t,c,valid);
	float f;
	pblock->GetValue(p_shininess,t,f,valid);
	pblock->GetValue(p_xparency,t,f,valid);
	pblock->GetValue(p_shinstr,t,f,valid);
	pblock->GetValue(p_wiresize,t,f,valid);
	pblock->GetValue(p_illumfloat,t,f,valid);
	pblock->GetValue(p_gireflection,t,f,valid);
	pblock->GetValue(p_girefraction,t,f,valid);
	pblock->GetValue(p_gialpha,t,f,valid);
	pblock->GetValue(p_reflection,t,f,valid);
	pblock->GetValue(p_refraction,t,f,valid);
	pblock->GetValue(p_alpha,t,f,valid);
	BOOL b;
	pblock->GetValue(p_illumbool,t,b,valid);
	pblock->GetValue(p_illumcolor,t,c,valid);
	return valid;
}

/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

RefTargetHandle PRTMaxMaterial::GetReference(int i) 
{
	if (i < NSUBMTL )
		return submtl[i];
	else return pblock;
}

void PRTMaxMaterial::SetReference(int i, RefTargetHandle rtarg) 
{
	if (i < NSUBMTL)
		submtl[i] = (Mtl *)rtarg; 
	else pblock = (IParamBlock2 *)rtarg; 
}

TSTR PRTMaxMaterial::SubAnimName(int i) 
{
	if (i < NSUBMTL)
		return GetSubMtlTVName(i);
	else return TSTR(_T(""));
}

Animatable* PRTMaxMaterial::SubAnim(int i) {
	if (i < NSUBMTL)
		return submtl[i]; 
	else return pblock;
	}

RefResult PRTMaxMaterial::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message ) 
{
	switch (message) {
		case REFMSG_CHANGE:
			ivalid.SetEmpty();
			if (hTarget == pblock)
				{
				ParamID changing_param = pblock->LastNotifyParamID();
				prtmaxmaterial_param_blk.InvalidateUI(changing_param);
				}
			break;

		}
	return REF_SUCCEED;
}

/*===========================================================================*\
 |	SubMtl get and set
\*===========================================================================*/

Mtl* PRTMaxMaterial::GetSubMtl(int i)
{
	if (i < NSUBMTL )
		return submtl[i];
	return NULL;
}

void PRTMaxMaterial::SetSubMtl(int i, Mtl *m)
{
	ReplaceReference(i,m);
	// TODO: Set the material and update the UI	
}

TSTR PRTMaxMaterial::GetSubMtlSlotName(int i)
{
	// Return i'th sub-material name 
	return _T(""); 
}

TSTR PRTMaxMaterial::GetSubMtlTVName(int i)
{
	return GetSubMtlSlotName(i);
}

/*===========================================================================*\
 |	Texmap get and set
 |  By default, we support none
\*===========================================================================*/

Texmap* PRTMaxMaterial::GetSubTexmap(int i)
{
	return NULL;
}

void PRTMaxMaterial::SetSubTexmap(int i, Texmap *m)
{
}

TSTR PRTMaxMaterial::GetSubTexmapSlotName(int i)
{
	return _T("");
}

TSTR PRTMaxMaterial::GetSubTexmapTVName(int i)
{
	// Return i'th sub-texture name 
	return GetSubTexmapSlotName(i);
}



/*===========================================================================*\
 |	Standard IO
\*===========================================================================*/

#define MTL_HDR_CHUNK 0x4000

IOResult PRTMaxMaterial::Save(ISave *isave) { 
	IOResult res;
	isave->BeginChunk(MTL_HDR_CHUNK);
	res = MtlBase::Save(isave);
	if (res!=IO_OK) return res;
	isave->EndChunk();

	return IO_OK;
	}	

IOResult PRTMaxMaterial::Load(ILoad *iload) { 
	IOResult res;
	int id;
	while (IO_OK==(res=iload->OpenChunk())) {
		switch(id = iload->CurChunkID())  {
			case MTL_HDR_CHUNK:
				res = MtlBase::Load(iload);
				break;
			}
		iload->CloseChunk();
		if (res!=IO_OK) 
			return res;
		}

	return IO_OK;
	}


/*===========================================================================*\
 |	Updating and cloning
\*===========================================================================*/

RefTargetHandle PRTMaxMaterial::Clone(RemapDir &remap) {
	PRTMaxMaterial *mnew = new PRTMaxMaterial(FALSE);
	*((MtlBase*)mnew) = *((MtlBase*)this); 
	mnew->ReplaceReference(NSUBMTL,remap.CloneRef(pblock));

	mnew->ivalid.SetEmpty();	
	for (int i = 0; i<NSUBMTL; i++) {
		mnew->submtl[i] = NULL;
		if (submtl[i])
			mnew->ReplaceReference(i,remap.CloneRef(submtl[i]));
		mnew->mapOn[i] = mapOn[i];
		}
	BaseClone(this, mnew, remap);
	return (RefTargetHandle)mnew;
	}

void PRTMaxMaterial::NotifyChanged() 
{
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

void PRTMaxMaterial::Update(TimeValue t, Interval& valid) 
{	
	if (!ivalid.InInterval(t)) {

		ivalid.SetInfinite();
		pblock->GetValue( mtl_mat1_on, t, mapOn[0], ivalid);
		pblock->GetValue( p_spin, t, spin, ivalid);
		pblock->GetValue( p_color, t, color, ivalid);
		pblock->GetValue( p_ambiente, t, ambiente, ivalid);
		pblock->GetValue( p_especular, t, especular, ivalid);
		pblock->GetValue( p_shininess, t, shininess, ivalid);
		pblock->GetValue( p_xparency, t, xparency, ivalid);
		pblock->GetValue( p_shinstr, t, shinstr, ivalid);
		pblock->GetValue( p_wiresize, t, wiresize, ivalid);
		pblock->GetValue( p_illumfloat, t, illumfloat, ivalid);
		pblock->GetValue( p_illumbool, t, illumbool, ivalid);
		pblock->GetValue( p_illumcolor, t, illumcolor, ivalid);
		pblock->GetValue( p_gireflection, t, gireflection, ivalid);
		pblock->GetValue( p_girefraction, t, girefraction, ivalid);
		pblock->GetValue( p_gialpha, t, gialpha, ivalid);
		pblock->GetValue( p_reflection, t, reflection, ivalid);
		pblock->GetValue( p_refraction, t, refraction, ivalid);
		pblock->GetValue( p_alpha, t, alpha, ivalid);


		for (int i=0; i<NSUBMTL; i++) {
			if (submtl[i]) 
				submtl[i]->Update(t,ivalid);
			}
		}

		/*	Color			color;
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
		float			gialpha;*/

		mat->color=PRTVector(color[0],color[1],color[2]);
		mat->reflection=reflection;
		mat->refraction=refraction;
		mat->alpha=alpha;
		mat->gireflection=gireflection;
		mat->girefraction=girefraction;
		mat->gialpha=gialpha;


	valid &= ivalid;
}

/*===========================================================================*\
 |	Determine the characteristics of the material
\*===========================================================================*/

// para la previsualización

void PRTMaxMaterial::SetAmbient(Color c, TimeValue t) {ambiente=c;}		
void PRTMaxMaterial::SetDiffuse(Color c, TimeValue t) {color=c;}		
void PRTMaxMaterial::SetSpecular(Color c, TimeValue t) {especular=c;}
void PRTMaxMaterial::SetShininess(float v, TimeValue t) {shininess=v;}
				
Color PRTMaxMaterial::GetAmbient(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetAmbient(mtlNum,backFace):*/ambiente;
}

Color PRTMaxMaterial::GetDiffuse(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetDiffuse(mtlNum,backFace):*/color;
}

Color PRTMaxMaterial::GetSpecular(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetSpecular(mtlNum,backFace):*/especular;
}

float PRTMaxMaterial::GetXParency(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetXParency(mtlNum,backFace):*/xparency;
}

float PRTMaxMaterial::GetShininess(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetShininess(mtlNum,backFace):*/shininess;
}

float PRTMaxMaterial::GetShinStr(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->GetShinStr(mtlNum,backFace):*/shinstr;
}

float PRTMaxMaterial::WireSize(int mtlNum, BOOL backFace)
{
	return /*submtl[0]?submtl[0]->WireSize(mtlNum,backFace):*/wiresize;
}

float PRTMaxMaterial::GetSelfIllum(int mtlNum, BOOL backFace)
{
	return illumfloat;
}
	
BOOL PRTMaxMaterial::GetSelIllumColorOn(int mtlNum, BOOL backFace)
{
	return illumbool;
}

Color PRTMaxMaterial::GetSelIllumColor(int mtlNum, BOOL backFace)
{
	return illumcolor;
}
		
/*===========================================================================*\
 |	Actual shading takes place
\*===========================================================================*/

//shading, render

struct SIllumParams {
	ULONG flags;
	float sh_str, ph_exp, shine, softThresh;
	Color amb,diff,spec;
	Point3 N,V;
	Color diffIllum,specIllum,ambIllum;
	};

// StdMtl flags values
#define STDMTL_ADD_TRANSP   (1<<0)
#define STDMTL_FALLOFF_OUT  (1<<1)
#define STDMTL_WIRE		  	(1<<2)
#define STDMTL_2SIDE		(1<<3)
#define STDMTL_SOFTEN       (1<<4)
#define STDMTL_FILT_TRANSP 	(1<<5)
#define STDMTL_WIRE_UNITS	(1<<6)
#define STDMTL_LOCK_AD      (1<<8)
#define STDMTL_LOCK_DS      (1<<9)
#define STDMTL_UNUSED1		(1<<10)
#define STDMTL_LOCK_ADTEX   (1<<11)
#define STDMTL_FACEMAP		(1<<12)
#define STDMTL_OLDSPEC      (1<<13)
#define STDMTL_SSAMP		(1<<14)

#define STDMTL_ROLLUP1_OPEN  (1<<28)
#define STDMTL_ROLLUP2_OPEN  (1<<29)
#define STDMTL_ROLLUP3_OPEN  (1<<30)
#define STDMTL_ROLLUP4_OPEN  (1<<27)

#define STDMTL_ROLLUP_FLAGS (STDMTL_ROLLUP1_OPEN|STDMTL_ROLLUP2_OPEN|STDMTL_ROLLUP3_OPEN|STDMTL_ROLLUP4_OPEN)


void PRTMaxMaterial::Shade(ShadeContext& sc) 
{
	//::Shade(sc);

	
//	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;
//	if (gbufID) sc.SetGBufferID(gbufID);

//	if(sm1) sm1->Shade(sc);
//	else
	//{


	//	sc.out.c=color;
	//	sc.out.t=xparency;

		// TODO: compute the color and transparency output returned in sc.out.

	/*	SIllumParams ip;
		Color lightCol,rescol, diffIllum0;
		float opac;
		RGBA mval;
		Point3 N0,P;
		BOOL bumped = FALSE;
		if (gbufID) sc.SetGBufferID(gbufID);
*/
//		ip.flags = /*flags*/STDMTL_FILT_TRANSP|STDMTL_LOCK_ADTEX|STDMTL_SOFTEN;
//		ip.softThresh = 0.1f/*softThresh*/;
/*		ip.diffIllum = Color(0.0f,0.0f,0.0f);
		ip.specIllum = Color(0.0f,0.0f,0.0f);
		ip.ambIllum  = Color(0.0f,0.0f,0.0f);
		ip.amb = ambiente;
		ip.diff = color;
		ip.spec = especular;

		opac =  xparency;

		if (sc.mode==SCMODE_SHADOW) 
		{
			// Opacity mapping;
*///			if (mapOn[/*ID_OP*/0]) 
//				opac = submtl[/*ID_OP*/0]->LerpEvalMono(sc,opac);

			// "Shadow mode": This just computes the transparency, which is all 
			// you need for shadowing.
/*			if (opac!=1.0f||opfall!=0.0f) 
			{
				if (opfall!=0.0f) 
				{	
					ip.N = (shading==SHADE_CONST)?sc.GNormal():sc.Normal();
					ip.V = sc.V();  // get unit view vector
					float d = (float)fabs(DotProd(ip.N,ip.V));
					if (flags&STDMTL_FALLOFF_OUT) d = 1.0f-d;
					opac *= (1.0f-opfall*d);
				}

				// Transparency may use diffuse color mapping
				if ((flags&STDMTL_ADD_TRANSP|STDMTL_FILT_TRANSP)==0) 
				{
					if (DOMAP(ID_DI)) 
					{
						mval = (*maps)[ID_DI].Eval(sc);
						AlphaCompCol(ip.diff,mval); 
					}
				}
				// Compute the transpareny color
				sc.out.t = TranspColor(sc, opac, ip.diff);
			}
			else 
				sc.out.t = blackCol;
			return;
			}
		}*//*
		N0 = ip.N = sc.Normal();

		if (shading==SHADE_CONST) {
			ip.N = sc.GNormal();
			bumped = TRUE;
			sc.SetNormal(ip.N);
			}

		P = sc.P();
		ip.V = sc.V();  // get unit view vector

		// Do texture mapping

		// Bump mapping: Do this FIRST so other maps can use the perturbed normal
		if (DOMAP(ID_BU)) {
			Point3 dn = (*maps)[ID_BU].EvalNormalPerturb(sc);
			bumped = TRUE;
			ip.N = FNormalize(ip.N + (sc.backFace?-dn:dn));
			sc.SetNormal(ip.N);
			}

		// Diffuse color mapping
		if (DOMAP(ID_DI)) {
			mval = (*maps)[ID_DI].Eval(sc);
			AlphaCompCol(ip.diff,mval); 
			if (flags&STDMTL_LOCK_ADTEX) 
				AlphaCompCol(ip.amb,mval); 
			}

		// Ambient color mapping
		if (!(flags&STDMTL_LOCK_ADTEX)) {
			if (DOMAP(ID_AM)) {
				AlphaCompCol(ip.amb, (*maps)[ID_AM].Eval(sc)); 
				}	
			}

		// Specular color mapping
		if (DOMAP(ID_SP)) 
			AlphaCompCol(ip.spec,(*maps)[ID_SP].Eval(sc)); 

		// Opacity mapping;
		if (DOMAP(ID_OP)) {
			opac = (*maps)[ID_OP].LerpEvalMono(sc,opac);
			}

		// Shininess mapping
		if (DOMAP(ID_SH)) {
			//ip.shine = shininess*(*maps)[ID_SH].EvalMono(sc); 
			ip.shine = (*maps)[ID_SH].LerpEvalMono(sc,shininess); 
			ip.ph_exp = (float)pow(2.0,ip.shine*10.0); // expensive.!!	TBD
			}
		else {
			ip.shine = shininess;
			ip.ph_exp = phongexp;
			}

		// Shininess strength mapping
		ip.sh_str = (DOMAP(ID_SS))? (*maps)[ID_SS].LerpEvalMono(sc,shine_str): shine_str; 

		// Self illumination mapping
		float self_i = (DOMAP(ID_SI))? (*maps)[ID_SI].LerpEvalMono(sc,self_illum) : self_illum;

		curShader->Illum(sc,ip);

		diffIllum0 = ip.diffIllum; // save this for reflection dimming

		// Apply self illumination
		if (self_i>0.0f) {
			ip.diffIllum = (self_i>=1.0f) ?  whiteCol: ip.diffIllum*(1.0f-self_i) + self_i;
			}

		ip.diffIllum = ip.amb*(sc.ambientLight+ip.ambIllum) +  ip.diff*ip.diffIllum;		

		if (DOMAP(ID_RR)) {
			// Set up for opacity for Refraction map.
			opac *= 1.0f-(*maps)[ID_RR].amount;   

			// Make more opaque where specular hilite occurs:
			float a_s = MaxRGB(ip.specIllum);
			if (a_s>1.0f) a_s = 1.0f; 
			sc.out.c = opac*ip.diffIllum + ip.specIllum;
			opac = opac + a_s - opac*a_s;

			// Evaluate refraction map, filtered by filter color.
			Texmap *refrmap = (*maps)[ID_RR].map;
			AColor rcol;
			sc.SetIOR(ioRefract);
			if (refrmap->HandleOwnViewPerturb()) 
				rcol = refrmap->EvalColor(sc);
			else  
				rcol = sc.EvalEnvironMap(refrmap, sc.RefractVector(ioRefract));
			sc.out.c += Color(rcol.r,rcol.g,rcol.b)*TranspColor(sc, opac, ip.diff);
			sc.out.t.Black();  // no transparency when doing refraction
			}
		else {
			if (opac!=1.0f||opfall!=0.0f) {
				if (opfall!=0.0f) {	
					float d = (float)fabs(DotProd(ip.N,ip.V));
					if (flags&STDMTL_FALLOFF_OUT) d = 1.0f-d;
					opac *= (1.0f-opfall*d);
					}
				
				// Make more opaque where specular hilite occurs, so you
				// can still see the hilite:
				float a_s = MaxRGB(ip.specIllum);
				if (a_s>1.0f) a_s = 1.0f; 
				sc.out.c = opac*ip.diffIllum + ip.specIllum;
				opac = opac + a_s - opac*a_s;

				// Compute the color of the transparent filter color
				sc.out.t = TranspColor(sc, opac, ip.diff);
				}
			else {
				sc.out.t = blackCol;
				sc.out.c = ip.diffIllum + ip.specIllum;
				}
			}

		// Evaluate reflection map.
		if (DOMAP(ID_RL)) {
			AColor rcol;
			Texmap *reflmap = (*maps)[ID_RL].map;
			if (reflmap->HandleOwnViewPerturb()) {
				sc.TossCache(reflmap);
				rcol = reflmap->EvalColor(sc);
				}
			else 
				rcol = sc.EvalEnvironMap(reflmap, sc.ReflectVector());
			Color rc(rcol.r,rcol.g,rcol.b);
			curShader->AffectReflMap(sc,ip,rc);
			float r = rcol.a*(*maps)[ID_RL].amount;
			if (dimReflect) {
				float dimfact = ((1.0f-dimIntens)*Intens(diffIllum0)*dimMult + dimIntens);
				r *= dimfact;
				}
			sc.out.c += (rc*r);
			}
		if (bumped) sc.SetNormal(N0);
		*/
//	}

}

float PRTMaxMaterial::EvalDisplacement(ShadeContext& sc)
{
	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;
	return (sm1)?sm1->EvalDisplacement(sc):0.0f;
}

Interval PRTMaxMaterial::DisplacementValidity(TimeValue t)
{
	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;

	Interval iv; iv.SetInfinite();
	if(sm1) iv &= sm1->DisplacementValidity(t);

	return iv;	
}
	

