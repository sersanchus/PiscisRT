#ifndef __PRTMAXPLUGIN__H
#define __PRTMAXPLUGIN__H

#include "maxincl.h"
#include "resource.h"
#include "../../PiscisRTFinal/source/PRTMain.h"

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;
#define PRTMAXPLUGIN_CLASS_ID	Class_ID(0x3465c2b, 0x4649bf1b)
#define PBLOCK_REF	0


//-------------------
// RENDER PARAMS
//-------------------

// Antialias levels
/*#define  AA_NONE	0x00
#define  AA_MEDIUM	0x01
#define  AA_HIGH	0x02*/

class PRTRenderParams : public RenderGlobalContext {
public:
	RendType	rendType;				// View, blowup, region etc.
	int			nMinx;
	int			nMiny;
	int			nMaxx;
	int			nMaxy;
	int			nNumDefLights;			// The default lights passed into the renderer
	int			nRegxmin;				// Coords for render blowup etc.
	int			nRegxmax;				// Coords for render blowup etc.
	int			nRegymin;				// Coords for render blowup etc.
	int			nRegymax;				// Coords for render blowup etc.
	Point2		scrDUV;
/*	BitArray	gbufChan;				// The G buffer channels (bitflags)
*/	DefaultLight*	pDefaultLights;
	FrameRendParams*	pFrp;			// Frame specific members
	RendProgressCallback*	progCallback;	// Render progress callback
/*
	GBufReader*	gbufReader;
	GBufWriter*	gbufWriter;
*/
	// Custom options
	// These options are specific to the sample renderer
/*	int			nMaxDepth;
	int			nAntiAliasLevel;
	BOOL		bReflectEnv;
*/
	// Standard options
	// These options are configurable for all plugin renderers
	BOOL		bVideoColorCheck;
	BOOL		bForce2Sided;
	BOOL		bRenderHidden;
	BOOL		bSuperBlack;
	BOOL		bRenderFields;
	BOOL		bNetRender;

	// Render effects
	Effect*		effect;

	PRTRenderParams();
	void		ComputeViewParams(const ViewParams&vp);
	Point3		RayDirection(float sx, float sy);
	Point3		RayPosition(float sx, float sy);

	/*int				NumRenderInstances();
	RenderInstance*	GetRenderInstance(int i);*/
};

//MATERIALKEEPER

/*class MtlKeeper {
public:
	BOOL	AddMtl(Mtl* mtl);
	int		GetMtlID(Mtl* mtl);
	int		Count();
	Mtl*	GetMtl(int id);

	Tab<Mtl*> mtlTab;
};*/


//MY VIEW

class MyView : public View {
public:
	PRTRenderParams*	pRendParams;
	Point2		ViewToScreen(Point3 p);
};


//RENDERER

class PRTMaxPlugin : public Renderer {
	public:

		BOOL		bOpen;					// Indicate that Open() has been called

		//escena
		INode *scene;
		INode* pViewNode;
//		MyView theView;
		ViewParams view;
		void GetViewParams(INode* vnode, ViewParams& vp, TimeValue t);


		//interface
		Interface * ip;

		//trazador
		PRTMain trazador;
		PRTVector camarapos;
		PRTVector camaradir;
		PRTFloat ancho;
		PRTFloat alto;
		PRTFloat fov;
		PRTDinamicList Materiales;
	

		//materiales
//		MtlKeeper mtlList;

		//render params
		PRTRenderParams rendParams;
		
		


		//exportar
		BOOL nodeEnum(INode*, int, TimeValue); 

		void ExportMesh(INode*, int, TimeValue);
		void ExportGeomObject(INode*, int, TimeValue);

		void ExportLightObject(INode* , int, TimeValue);


		//dialogo
		//HWND dlg;




		//Render
//		RendParams RP;


		// Parameter block
		//IParamBlock2	*pblock;	//ref 0
		
		// Loading/Saving
		IOResult Load(ILoad *iload){return IO_OK;};
		IOResult Save(ISave *isave){return IO_OK;};

		//From Animatable
		Class_ID ClassID() {return PRTMAXPLUGIN_CLASS_ID;}		
		//SClass_ID SuperClassID() { return RENDERER_CLASS_ID; }
		void GetClassName(TSTR& s) {s = GetString(IDS_LIBDESCRIPTION);}

		RefTargetHandle Clone( RemapDir &remap )
		{
			PRTMaxPlugin *newRend = new PRTMaxPlugin;
			//newRend->pblock=pblock;
			BaseClone(this, newRend, remap);
			return newRend;	
		};
		//RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message)
		//{
		//	return REF_SUCCEED;
		//};

	//	int NumSubs() { return 1; }
	//	TSTR SubAnimName(int i) { return GetString(IDS_PARAMS); }				
	//	Animatable* SubAnim(int i) { return pblock; }

		// TODO: Maintain the number or references here
	//	int NumRefs() { return 1; }
	//	RefTargetHandle GetReference(int i) { return pblock; }
	//	void SetReference(int i, RefTargetHandle rtarg) { pblock=(IParamBlock2*)rtarg; }

		/*int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock
		*/
		void DeleteThis() 
		{ 
			delete this; 
		}		
		//Constructor/Destructor

		PRTMaxPlugin();//{rendParams.renderer = this;};
		/*~PRTMaxPlugin()
		{
			//delete this;
		};*/		


		//VIRTUALES

		void ResetParams();

		RendParamDlg *CreateParamDialog(IRendParams *ir,BOOL prog);
		
		void Close(	HWND hwnd ){;};
		
		int Open(
		INode *scene,     	// root node of scene to render
		INode *vnode,     	// view node (camera or light), or NULL
		ViewParams *viewPar,// view params for rendering ortho or user viewport
		RendParams &rp,  	// common renderer parameters
		HWND hwnd, 				// owner window, for messages
		DefaultLight* defaultLights=NULL, // Array of default lights if none in scene
		int numDefLights=0	// number of lights in defaultLights array
		);
		
		int Render(
		TimeValue t,   			// frame to render.
   		Bitmap *tobm, 			// optional target bitmap
		FrameRendParams &frp,	// Time dependent parameters
		HWND hwnd, 				// owner window
		RendProgressCallback *prog=NULL,
		ViewParams *viewPar=NULL  // override viewPar given on Open.
		);



		//información de los dialogos

		//dialogo de opciones

		bool BTextures;
		bool BReflections;
		bool BRefractions;
		bool BAlpha;
		bool BSpecular;
		bool BShadows;
		bool BDoubleSided;
		bool BNInter;
		bool BCInter;
		bool BOctrees;
		bool BConvexHulls;

		int OctreesDeep;
		
		bool BIndirectLight;
		bool BGiReflections;
		bool BGiRefractions;
		bool BGiAlpha;
		bool BGlossy;
		bool BMejoraPorDistancia;

		int NumPhotones;
		int GroupPhotones;

		float ScalePhoton;
};


#endif // __PRTMAXPLUGIN__H