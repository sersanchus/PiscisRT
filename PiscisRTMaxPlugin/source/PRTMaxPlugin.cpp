#include "PRTMaxPlugin.h"
#include "../../PiscisRTMaxMaterialFinal/source/PRTMaxMaterial.h"
#include "maxincl.h"
#include <stdlib.h>

class PRTMaxPluginClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new PRTMaxPlugin; }
	const TCHAR *	ClassName() { return GetString(IDS_LIBDESCRIPTION); }
	SClass_ID		SuperClassID() { return RENDERER_CLASS_ID; }
	Class_ID		ClassID() { return PRTMAXPLUGIN_CLASS_ID; }
	const TCHAR* 	Category() { return _T(""); }
	//void			ResetClassParams(BOOL fileReset) {}

	const TCHAR*	InternalName() { return _T("PRTMaxPlugin"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

	//TCHAR* GetRsrcString(INT_PTR id){ return (TCHAR*)id;} // PQ??

};

static PRTMaxPluginClassDesc PRTMaxPluginDesc;
ClassDesc* GetPRTMaxPluginDesc() { return &PRTMaxPluginDesc; }

/*enum { prtmaxplugin_params };

//TODO: Add enums for various parameters
enum { 
	p_btextures,
	p_bdoublesided,
	p_bshadows,
	p_bspecular,
	p_breflections,
	p_brefractions,
	p_balpha,
	p_bglossy,
	p_bninter,
	p_bcinter,
	p_boctrees,
	p_bconvexhulls,
	p_bmejorapordistancia,
	p_octreesdeep,
	p_bindirectlight,
	p_bgireflections,
	p_bgirefractions,
	p_bgialpha,
	p_numfotones,
	p_grupofotones,
	p_escalafotones,
};

static ParamBlockDesc2 prtmaxplugin_param_blk ( prtmaxplugin_params, _T("params"), 0, &PRTMaxPluginDesc, 
	(P_AUTO_CONSTRUCT + P_AUTO_UI), PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	p_btextures,			_T("btextures"),			TYPE_BOOL,		0,				IDS_BTEXTURES,				p_default,		TRUE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BTextures,				end,
	p_bdoublesided,			_T("bdoublesided"),			TYPE_BOOL,		0,				IDS_BDOUBLESIDED,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BDoubleSided,			end,
	p_bshadows,				_T("bshadows"),				TYPE_BOOL,		0,				IDS_BSHADOWS,				p_default,		TRUE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BShadows,				end,
	p_bspecular,			_T("bspecular"),			TYPE_BOOL,		0,				IDS_BSPECULAR,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BSpecular,				end,
	p_breflections,			_T("breflections"),			TYPE_BOOL,		0,				IDS_BREFLECTIONS,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BReflections,			end,
	p_brefractions,			_T("brefractions"),			TYPE_BOOL,		0,				IDS_BREFRACTIONS,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BRefractions,			end,
	p_balpha,				_T("balpha"),				TYPE_BOOL,		0,				IDS_BALPHA,					p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BAlpha,					end,
	p_bglossy,				_T("bglossy"),				TYPE_BOOL,		0,				IDS_BGLOSSY,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BGlossy,				end,
	p_bninter,				_T("bninter"),				TYPE_BOOL,		0,				IDS_BNINTER,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BNInter,				end,
	p_bcinter,				_T("bcinter"),				TYPE_BOOL,		0,				IDS_BCINTER,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BCInter,				end,
	p_boctrees,				_T("boctrees"),				TYPE_BOOL,		0,				IDS_BOCTREES,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BOctrees,				end,
	p_bconvexhulls,			_T("bconvexhulls"),			TYPE_BOOL,		0,				IDS_BCONVEXHULLS,			p_default,		TRUE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BConvexHulls,			end,
	p_bmejorapordistancia,	_T("bmejorapordistancia"),	TYPE_BOOL,		0,				IDS_BMEJORAPORDISTANCIA,	p_default,		TRUE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BMejoraPorDistancia,	end,
	p_octreesdeep,			_T("octreesdeep"),			TYPE_INT,		0,				IDS_OCTREESDEEP,			p_default,		0,						p_range,		0,					100,			p_ui,					TYPE_SPINNER,	EDITTYPE_INT,		IDC_OctreesDeep,		IDC_OctreesDeepSpinner,		1,		end,
	p_bindirectlight,		_T("bindirectlight"),		TYPE_BOOL,		0,				IDS_BINDIRECTLIGHT,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BIndirectLight,			end,
	p_bgireflections,		_T("bgireflections"),		TYPE_BOOL,		0,				IDS_BGIREFLECTIONS,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BGiReflections,			end,
	p_bgirefractions,		_T("bgirefractions"),		TYPE_BOOL,		0,				IDS_BGIREFRACTIONS,			p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BGiRefractions,			end,
	p_bgialpha,				_T("bgialpha"),				TYPE_BOOL,		0,				IDS_BGIALPHA,				p_default,		FALSE,	 				p_ui,			TYPE_SINGLECHEKBOX,		IDC_BGiAlpha,				end,
	p_numfotones,			_T("numfotones"),			TYPE_INT,		0,				IDS_NUMFOTONES,				p_default,		100,					p_range,		0,					100000,			p_ui,					TYPE_SPINNER,	EDITTYPE_INT,		IDC_NumFotones,		IDC_NumFotonesSpinner,		100,		end,
	p_grupofotones,			_T("grupofotones"),			TYPE_INT,		0,				IDS_GRUPOFOTONES,			p_default,		10,						p_range,		0,					10000,			p_ui,					TYPE_SPINNER,	EDITTYPE_INT,		IDC_GroupPhotones,	IDC_GroupPhotonesSpinner,	10,			end,
	p_escalafotones,		_T("escalafotones"),		TYPE_FLOAT,		0,				IDS_ESCALAFOTONES,			p_default,		0.01f,					p_range,		0.000001f,			1000.0f,		p_ui,					TYPE_SPINNER,	EDITTYPE_FLOAT,		IDC_ScalePhoton,	IDC_ScalePhotonSpinner,		0.001f,		end,
	end
	);
*/
void PRTMaxPlugin::ResetParams()
{
	//PRTMaxPluginDesc.MakeAutoParamBlocks(this);
}

// constructor

PRTMaxPlugin::PRTMaxPlugin()
{
	rendParams.renderer = this;
	BTextures=false;
	BReflections=false;
	BRefractions=false;
	BAlpha=false;
	BSpecular=false;
	BShadows=false;
	BDoubleSided=false;
	BNInter=false;
	BCInter=false;
	BOctrees=false;
	BConvexHulls=true;

	OctreesDeep=0;

	BIndirectLight=false;
	BGiReflections=false;
	BGiRefractions=false;
	BGiAlpha=false;
	BGlossy=false;
	BMejoraPorDistancia=true;

	NumPhotones=100;
	ScalePhoton=0.001f;
	GroupPhotones=10;

	//PRTMaxPluginDesc.MakeAutoParamBlocks(this);
}



//------------------------------------
//			RENDER PARAMS
//------------------------------------

PRTRenderParams::PRTRenderParams()
{
/*	nMaxDepth = 0;
	nAntiAliasLevel = AA_NONE;
	bReflectEnv = FALSE;
*/
	envMap = NULL;
	atmos = NULL;
	rendType = RENDTYPE_NORMAL;
	nMinx = 0;
	nMiny = 0;
	nMaxx = 0;
	nMaxy = 0;
	nNumDefLights = 0;
	nRegxmin = 0;
	nRegxmax = 0;
	nRegymin = 0;
	nRegymax = 0;
	scrDUV = Point2(0.0f, 0.0f);
	pDefaultLights = NULL;
	pFrp = NULL;
	bVideoColorCheck = 0;
	bForce2Sided = FALSE;
	bRenderHidden = FALSE;
	bSuperBlack = FALSE;
	bRenderFields = FALSE;
	bNetRender = FALSE;

	renderer = NULL;
	projType = PROJ_PERSPECTIVE;
	devWidth = 0;
	devHeight = 0;
	xscale = 0;
	yscale = 0;
	xc = 0;
	yc = 0;
	antialias = FALSE;
	nearRange = 0;
	farRange = 0;
	devAspect = 0;
	frameDur = 0;
	time = 0;
	wireMode = FALSE;
	inMtlEdit = FALSE;
	fieldRender = FALSE;
	first_field = FALSE;
	field_order = FALSE;
	objMotBlur = FALSE;
	nBlurFrames = 0;
}

#define VIEW_DEFAULT_WIDTH ((float)400.0)

void PRTRenderParams::ComputeViewParams(const ViewParams&vp)
{
	worldToCam = vp.affineTM;
	camToWorld = Inverse(worldToCam);

	xc = devWidth / 2.0f;
	yc = devHeight / 2.0f;

	scrDUV.x = 1.0f/(float)devWidth;
	scrDUV.y = 1.0f/(float)devHeight;

	projType = vp.projType;

	if (projType == PROJ_PERSPECTIVE) {
		float fac =  -(float)(1.0 / tan(0.5*(double)vp.fov));
		xscale =  fac*xc;
		yscale = -devAspect*xscale;
	}
	else {
		xscale = (float)devWidth/(VIEW_DEFAULT_WIDTH*vp.zoom);
		yscale = -devAspect*xscale;
	}

}

Point3 PRTRenderParams::RayDirection(float sx, float sy)
{
	Point3 p(0,0,-1);
	if (projType==PROJ_PERSPECTIVE)
	{
		p.x = -(sx-xc)/xscale; 
		p.y = -(sy-yc)/yscale; 
		p.z = -1.0f;
	}
	return Normalize(p);
}

Point3 PRTRenderParams::RayPosition(float sx, float sy)
{
	Point3 p(0,0,0);
	if (projType!=PROJ_PERSPECTIVE)
	{
		p.x = (sx-xc)/xscale; 
		p.y = (sy-yc)/yscale; 
		p.z = 5000;//¿?TODO
		//p=Normalize(p);
	}
	//return Normalize(p);
	return p;
}

/*int CJRenderParams::NumRenderInstances()
{
	return ((CJRenderer*)renderer)->instTab.Count();
}

RenderInstance* CJRenderParams::GetRenderInstance(int i)
{
	if (i<NumRenderInstances()) {
		return ((CJRenderer*)renderer)->instTab[i];
	}

	return NULL;
}
*/











//------------------------------------
//         MATERIALKEEPER
//------------------------------------


/*void PreProcess(INode* node, int nodeCount, MtlKeeper mtlList)
{
	nodeCount++;
	
	// Add the nodes material to out material list
	// Null entries are ignored when added...
	mtlList.AddMtl(node->GetMtl());

	// For each child of this node, we recurse into ourselves 
	// and increment the counter until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		PreProcess(node->GetChildNode(c), nodeCount, mtlList);
	}
}

BOOL MtlKeeper::AddMtl(Mtl* mtl)
{
	if (!mtl) {
		return FALSE;
	}

	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return FALSE;
		}
	}
	mtlTab.Append(1, &mtl, 25);

	return TRUE;
}

int MtlKeeper::GetMtlID(Mtl* mtl)
{
	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return i;
		}
	}
	return -1;
}

int MtlKeeper::Count()
{
	return mtlTab.Count();
}

Mtl* MtlKeeper::GetMtl(int id)
{
	return mtlTab[id];
}


*/









//------------------
//   MY VIEW
//------------------

Point2 MyView::ViewToScreen(Point3 p)
{
	return pRendParams->MapToScreen(p);
}









//-----------------------------
//		PRTMaxPlugin
//-----------------------------

#define BIGFLOAT (float(1.0e30))

void RemoveScaling(Matrix3 &m)
{
	for (int i=0; i<3; i++) 
		m.SetRow(i,Normalize(m.GetRow(i)));
}

void PRTMaxPlugin::GetViewParams(INode* vnode, ViewParams& vp, TimeValue t)
{
	Interval iv;
	const ObjectState& os = vnode->EvalWorldState(t);
	switch (os.obj->SuperClassID()) {
		case CAMERA_CLASS_ID: {
			// compute camera transform
			CameraState cs;
			CameraObject *cam = (CameraObject *)os.obj;
			iv.SetInfinite();

			// Grab the Camera transform from the node.
			Matrix3 camtm = vnode->GetObjTMAfterWSM(t,&iv);

			//camarapos=PRTVector(camtm.GetRow(3).x,camtm.GetRow(3).z,camtm.GetRow(3).z);//¿?

			RemoveScaling(camtm);
			vp.affineTM = Inverse(camtm);
			cam->EvalCameraState(t,iv,&cs);
			if (cs.manualClip) {
				vp.hither = cs.hither;
				vp.yon = cs.yon;
			}
			else {
			    vp.hither	= 0.1f;
		    	vp.yon	  	= -BIGFLOAT;
			}
			vp.projType = PROJ_PERSPECTIVE;
			vp.fov = cs.fov;
			rendParams.nearRange = cs.nearRange;
			rendParams.farRange = cs.farRange;
			}
			break;
		case LIGHT_CLASS_ID: {

			iv.SetInfinite();
			Matrix3 ltm = vnode->GetObjTMAfterWSM(t,&iv);
			vp.affineTM = Inverse(ltm);
			
			LightState ls;
			LightObject *ltob = (LightObject *)os.obj;
			ltob->EvalLightState(t,iv,&ls);

			float aspect = ls.shape?1.0f:ls.aspect;
			switch(ls.type) {
				case SPOT_LGT:			
					vp.projType = PROJ_PERSPECTIVE;      
					vp.fov = DegToRad(ls.fallsize);  
					vp.fov = 2.0f* (float)atan(tan(vp.fov*0.5f)*sqrt(aspect));
					rendParams.devAspect = (float(rendParams.devHeight)/float(rendParams.devWidth))*aspect;
					break;
				case DIRECT_LGT:
					vp.projType = PROJ_PARALLEL; 
					rendParams.devAspect = (float(rendParams.devHeight)/float(rendParams.devWidth))*aspect;
					break;
			}
		    vp.hither	= 0.1f;
	    	vp.yon	  	= -BIGFLOAT;  // so  it doesn't get used

			rendParams.nearRange = 0.0f;
			rendParams.farRange = 500.0f;
			}
			break;
		default:
			rendParams.nearRange = 0.0f;
			rendParams.farRange = 500.0f;
			break;
	}	
}


//       OBJETOS GEOMETRICOS


TSTR GetIndent(int indentLevel)
{
	TSTR indentString = "";
	for (int i=0; i<indentLevel; i++) {
		indentString += "\t";
	}
	
	return indentString;
}

//void ExportNodeHeader(INode* node, TCHAR* type, int indentLevel)
//{
/*	TSTR indent = GetIndent(indentLevel);
	
	// Output node header and object type 
	fprintf(pStream,"%s%s {\n", indent.data(), type);
	
	// Node name
	fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_NODE_NAME, FixupName(node->GetName()));
	
	//  If the node is linked, export parent node name
	INode* parent = node->GetParentNode();
	if (parent && !parent->IsRootNode()) {
		fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_NODE_PARENT, FixupName(parent->GetName()));
	}*/
//}

//void ExportNodeTM(INode* node, int indentLevel)
//{
/*	Matrix3 pivot = node->GetNodeTM(GetStaticFrame());
	TSTR indent = GetIndent(indentLevel);
	
	fprintf(pStream,"%s\t%s {\n", indent.data(), ID_NODE_TM);
	
	// Node name
	// We export the node name together with the nodeTM, because some objects
	// (like a camera or a spotlight) has an additional node (the target).
	// In that case the nodeTM and the targetTM is exported after eachother
	// and the nodeName is how you can tell them apart.
	fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_NODE_NAME, FixupName(node->GetName()));

	// Export TM inheritance flags
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();
	fprintf(pStream,"%s\t\t%s %d %d %d\n", indent.data(), ID_INHERIT_POS,
		INHERIT_POS_X & iFlags ? 1 : 0,
		INHERIT_POS_Y & iFlags ? 1 : 0,
		INHERIT_POS_Z & iFlags ? 1 : 0);

	fprintf(pStream,"%s\t\t%s %d %d %d\n", indent.data(), ID_INHERIT_ROT,
		INHERIT_ROT_X & iFlags ? 1 : 0,
		INHERIT_ROT_Y & iFlags ? 1 : 0,
		INHERIT_ROT_Z & iFlags ? 1 : 0);

	fprintf(pStream,"%s\t\t%s %d %d %d\n", indent.data(), ID_INHERIT_SCL,
		INHERIT_SCL_X & iFlags ? 1 : 0,
		INHERIT_SCL_Y & iFlags ? 1 : 0,
		INHERIT_SCL_Z & iFlags ? 1 : 0);

	// Dump the full matrix
	DumpMatrix3(&pivot, indentLevel+2);
	
	fprintf(pStream,"%s\t}\n", indent.data());*/
//}

BOOL TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

TriObject* GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

static Point3 basic_tva[3] = { 
	Point3(0.0,0.0,0.0),Point3(1.0,0.0,0.0),Point3(1.0,1.0,0.0)
};
static Point3 basic_tvb[3] = { 
	Point3(1.0,1.0,0.0),Point3(0.0,1.0,0.0),Point3(0.0,0.0,0.0)
};
static int nextpt[3] = {1,2,0};
static int prevpt[3] = {2,0,1};

void make_face_uv(Face *f, Point3 *tv)
{
	int na,nhid,i;
	Point3 *basetv;
	/* make the invisible edge be 2->0 */
	nhid = 2;
	if (!(f->flags&EDGE_A))  nhid=0;
	else if (!(f->flags&EDGE_B)) nhid = 1;
	else if (!(f->flags&EDGE_C)) nhid = 2;
	na = 2-nhid;
	basetv = (f->v[prevpt[nhid]]<f->v[nhid]) ? basic_tva : basic_tvb; 
	for (i=0; i<3; i++) {  
		tv[i] = basetv[na];
		na = nextpt[na];
	}
}

BOOL CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh, int indentLevel)
{
	if (!mtl || !mesh) {
		return FALSE;
	}
	
	ULONG matreq = mtl->Requirements(-1);
	
	// Are we using face mapping?
	if (!(matreq & MTLREQ_FACEMAP)) {
		return FALSE;
	}
	
	TSTR indent = GetIndent(indentLevel+1);
	
	// OK, we have a FaceMap situation here...
	
//	fprintf(pStream, "%s%s {\n", indent.data(), ID_MESH_FACEMAPLIST);
	for (int i=0; i<mesh->getNumFaces(); i++) {
		Point3 tv[3];
		Face* f = &mesh->faces[i];
		make_face_uv(f, tv);
//		fprintf(pStream, "%s\t%s %d {\n", indent.data(), ID_MESH_FACEMAP, i);
//		fprintf(pStream, "%s\t\t%s\t%d\t%d\t%d\n", indent.data(), ID_MESH_FACEVERT, (int)tv[0].x, (int)tv[0].y, (int)tv[0].z);
//		fprintf(pStream, "%s\t\t%s\t%d\t%d\t%d\n", indent.data(), ID_MESH_FACEVERT, (int)tv[1].x, (int)tv[1].y, (int)tv[1].z);
//		fprintf(pStream, "%s\t\t%s\t%d\t%d\t%d\n", indent.data(), ID_MESH_FACEVERT, (int)tv[2].x, (int)tv[2].y, (int)tv[2].z);
//		fprintf(pStream, "%s\t}\n", indent.data());
	}
//	fprintf(pStream, "%s}\n", indent.data());
	
	return TRUE;
}

Point3 GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}

	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}

void PRTMaxPlugin::ExportMesh(INode* node, int indentLevel,TimeValue t)
{
	int i;
	Mtl* nodeMtl = node->GetMtl();
	Matrix3 tm = node->GetObjTMAfterWSM(t);
	BOOL negScale = TMNegParity(tm);
	int vx1, vx2, vx3;
	TSTR indent;
	
	indent = GetIndent(indentLevel+1);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return; // Safety net. This shouldn't happen.
	}
	
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}
	
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return;
	}
	
	Mesh* mesh = &tri->GetMesh();
	
	mesh->buildNormals();
	//mesh->UnifyNormals(false);
	
/*	fprintf(pStream, "%s%s {\n",indent.data(),  ID_MESH);
	fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_TIMEVALUE, t);
	fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMVERTEX, mesh->getNumVerts());
    fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMFACES, mesh->getNumFaces());
*/	
	// Export the vertices
//	fprintf(pStream,"%s\t%s {\n",indent.data(), ID_MESH_VERTEX_LIST);
//	for (i=0; i<mesh->getNumVerts(); i++) {
//		Point3 v = tm * mesh->verts[i];
//		fprintf(pStream, "%s\t\t%s %4d\t%s\n",indent.data(), ID_MESH_VERTEX, i, Format(v));
//	}
//	fprintf(pStream,"%s\t}\n",indent.data()); // End vertex list
	
	// To determine visibility of a face, get the vertices in clockwise order.
	// If the objects has a negative scaling, we must compensate for that by
	// taking the vertices counter clockwise
//	fprintf(pStream, "%s\t%s {\n",indent.data(), ID_MESH_FACE_LIST);




	TCHAR *jare;
	PRTTexture *tex;
	// buscar textura bitmap
	if (nodeMtl && nodeMtl->GetActiveTexmap() && nodeMtl->GetActiveTexmap()->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)) 
	{
		//TSTR className;
		//nodeMtl->GetClassName(className);
		jare=((BitmapTex *)(nodeMtl->GetActiveTexmap()))->GetMapName();
	}
	else
	{
		jare="";
	}

	tex=new PRTTexture((char*)jare); // se crea una nueva textura cada vez *TODO*

	for (i=0; i<mesh->getNumFaces(); i++) {
//		fprintf(pStream,"%s\t\t%s %4d:    A: %4d B: %4d C: %4d AB: %4d BC: %4d CA: %4d",
//			indent.data(),
//			ID_MESH_FACE, i,
//			mesh->faces[i].v[vx1],
//			mesh->faces[i].v[vx2],
//			mesh->faces[i].v[vx3],
//			mesh->faces[i].getEdgeVis(vx1) ? 1 : 0,
//			mesh->faces[i].getEdgeVis(vx2) ? 1 : 0,
//			mesh->faces[i].getEdgeVis(vx3) ? 1 : 0);
//		fprintf(pStream,"\t %s ", ID_MESH_SMOOTHING);
//		for (int j=0; j<32; j++) {
//			if (mesh->faces[i].smGroup & (1<<j)) {
//				if (mesh->faces[i].smGroup>>(j+1)) {
//					fprintf(pStream,"%d,",j+1); // Add extra comma
//				} else {
//					fprintf(pStream,"%d ",j+1);
//				}
//			}


			/*if (nodeMtl) 
			{
				int mtlID = nodeMtl.GetMtlID(mtl);
				//if (mtlID >= 0) 
					//fprintf(pStream,"%s\t%s %d\n", indent.data(), ID_MATERIAL_REF, mtlID);
				((BitmapTex *)(nodeMtl->GetActiveTexmap()))->GetMapName();
			}*/

			Color nodecol;
			PRTMaterial *mat;
		
			if (nodeMtl)
			{
				TSTR matclassname;
				nodeMtl->GetClassName(matclassname);
				if (strcmp(matclassname,"PRTMaxMaterial")==0)
				{
					/*mat.reflection=((PRTMaxMaterial*)(nodeMtl))->reflection;
					mat.refraction=((PRTMaxMaterial*)(nodeMtl))->refraction;
					mat.alpha=((PRTMaxMaterial*)(nodeMtl))->alpha;
					mat.gireflection=((PRTMaxMaterial*)(nodeMtl))->gireflection;
					mat.girefraction=((PRTMaxMaterial*)(nodeMtl))->girefraction;
					mat.gialpha=((PRTMaxMaterial*)(nodeMtl))->gialpha;*/
					mat=((PRTMaxMaterial*)(nodeMtl))->mat;
				}
				else
				{
					nodecol=nodeMtl->GetDiffuse();
					mat=new PRTMaterial(); // se crea un nuevo material cada vez *TODO*
					mat->color=PRTVector(nodecol.r,nodecol.g,nodecol.b);

					//mat.gireflection=1;
					if (nodeMtl->GetActiveTexmap())
					{
						mat->texture=tex;
						mat->color=PRTVector(1,1,1); //*TODO*
						//mat.gireflection=1;
					}
				}
			}
			else
			{
				mat=new PRTMaterial(); // se crea un nuevo material cada vez *TODO*
				DWORD c = node->GetWireColor();
				nodecol=Color(GetRValue(c)/255.0f, GetGValue(c)/255.0f, GetBValue(c)/255.0f);
				mat->color=PRTVector(nodecol.r,nodecol.g,nodecol.b);
				//if (nodeMtl->GetActiveTexmap() && nodeMtl->GetActiveTexmap()->GetName())
				//	mat.texture=(char*)((BitmapTex *)(nodeMtl->GetActiveTexmap()))->GetMapName();
				//mat->gireflection=1;
			}

			mat->BRDFtype=BRDF_PHONG; // por defecto de 3dsmax?
			
			///////////////
			// VERTICES
			///////////////

			Point3 vertice1aux=tm * mesh->verts[mesh->faces[i].v[vx1]];
			Point3 vertice2aux=tm * mesh->verts[mesh->faces[i].v[vx2]];
			Point3 vertice3aux=tm * mesh->verts[mesh->faces[i].v[vx3]];
			PRTVector vertice1(vertice1aux.x,vertice1aux.y,vertice1aux.z);
			PRTVector vertice2(vertice2aux.x,vertice2aux.y,vertice2aux.z);
			PRTVector vertice3(vertice3aux.x,vertice3aux.y,vertice3aux.z);
			
			///////////////
			// NORMALES
			///////////////

			/*Matrix3 lposmat=node->GetNodeTM(t);
			Point3 normal1p=lposmat.VectorTransform(GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx1))));
			Point3 normal2p=lposmat.VectorTransform(GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx2))));
			Point3 normal3p=lposmat.VectorTransform(GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx3))));
			PRTVector normal1,normal2,normal3;
			normal1=PRTVector(normal1p.x,normal1p.y,normal1p.z);
			normal2=PRTVector(normal2p.x,normal2p.y,normal2p.z);
			normal3=PRTVector(normal3p.x,normal3p.y,normal3p.z);*/

			PRTVector normaldecara=-((vertice1-vertice2).Normalize()^(vertice3-vertice2).Normalize()).Normalize(); //faceNormal?
			//me la calculaba yo
			// ahora lo haré bien
			
			PRTVector normal1,normal2,normal3;
			normal1=normal2=normal3=normaldecara;
			Point3 normal1aux,normal2aux,normal3aux;
			if (mesh->getSmoothFlags())
			{
				normal1aux=mesh->getNormal(mesh->faces[i].v[vx1]);
				normal2aux=mesh->getNormal(mesh->faces[i].v[vx2]);
				normal3aux=mesh->getNormal(mesh->faces[i].v[vx3]);
				normal1=PRTVector(normal1aux.x,normal1aux.y,normal1aux.z);
				normal2=PRTVector(normal2aux.x,normal2aux.y,normal2aux.z);
				normal3=PRTVector(normal3aux.x,normal3aux.y,normal3aux.z);
			}

			// smooth groups

			Face* f = &mesh->faces[i];
			DWORD smGroup = f->smGroup;
			int numNormals;
			Point3 vertexNormal;
			RVertex *rv;
			Matrix3 lposmat=node->GetNodeTM(t);
			
			rv=mesh->getRVertPtr(mesh->faces[i].getVert(vx1));
			if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
				if (numNormals == 1) {	normal1aux = lposmat.VectorTransform(rv->rn.getNormal());normal1=PRTVector(normal1aux.x,normal1aux.y,normal1aux.z);}
				else {for (int i = 0; i < numNormals; i++){
				if (rv->ern[i].getSmGroup() & smGroup) {normal1aux = lposmat.VectorTransform(rv->ern[i].getNormal());normal1=PRTVector(normal1aux.x,normal1aux.y,normal1aux.z);
			}}}}

			rv=mesh->getRVertPtr(mesh->faces[i].getVert(vx2));
			if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
				if (numNormals == 1) {	normal2aux = lposmat.VectorTransform(rv->rn.getNormal());normal2=PRTVector(normal2aux.x,normal2aux.y,normal2aux.z);}
				else {for (int i = 0; i < numNormals; i++){
				if (rv->ern[i].getSmGroup() & smGroup) {normal2aux = lposmat.VectorTransform(rv->ern[i].getNormal());normal2=PRTVector(normal2aux.x,normal2aux.y,normal2aux.z);
			}}}}

			rv=mesh->getRVertPtr(mesh->faces[i].getVert(vx3));
			if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
				if (numNormals == 1) {	normal3aux = lposmat.VectorTransform(rv->rn.getNormal());normal3=PRTVector(normal3aux.x,normal3aux.y,normal3aux.z);}
				else {for (int i = 0; i < numNormals; i++){
				if (rv->ern[i].getSmGroup() & smGroup) {normal3aux = lposmat.VectorTransform(rv->ern[i].getNormal());normal3=PRTVector(normal3aux.x,normal3aux.y,normal3aux.z);
			}}}}

			

			///////////////
			// COORDENADAS DE TEXTURA
			///////////////
							
			PRTTexCoord cordtex1;
			PRTTexCoord cordtex2;
			PRTTexCoord cordtex3;
			
			int numTVx = mesh->getNumTVerts();
			if (numTVx)
			{
				cordtex1=PRTTexCoord(mesh->tVerts[mesh->faces[i].v[vx1]].x,mesh->tVerts[mesh->faces[i].v[vx1]].y);
			//if (mesh->tVerts[mesh->faces[i].v[vx2]])
				cordtex2=PRTTexCoord(mesh->tVerts[mesh->faces[i].v[vx2]].x,mesh->tVerts[mesh->faces[i].v[vx2]].y);
			//if (mesh->tVerts[mesh->faces[i].v[vx3]])
				cordtex3=PRTTexCoord(mesh->tVerts[mesh->faces[i].v[vx3]].x,mesh->tVerts[mesh->faces[i].v[vx3]].y);
			}


			trazador.AddTriangle(vertice1,vertice2,vertice3,
								  /*PRTVector(
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx1))).x,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx1))).y,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx1))).z
												//mesh->FaceNormal(i,TRUE).x,
												//mesh->FaceNormal(i,TRUE).y,
												//mesh->FaceNormal(i,TRUE).z
											)*//*normaldecara*/normal1,
								  /*PRTVector(
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx2))).x,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx2))).y,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx2))).z
												//mesh->FaceNormal(i,TRUE).x,
												//mesh->FaceNormal(i,TRUE).y,
												//mesh->FaceNormal(i,TRUE).z
											)*//*normaldecara*/normal2,
								  /*PRTVector(
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx3))).x,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx3))).y,
												//GetVertexNormal(mesh,i,mesh->getRVertPtr(mesh->faces[i].getVert(vx3))).z
												//mesh->FaceNormal(i,TRUE).x,
												//mesh->FaceNormal(i,TRUE).y,
												//mesh->FaceNormal(i,TRUE).z
											)*//*normaldecara*/normal3,
											cordtex1,
											cordtex2,
											cordtex3,
								  //PRTVector((mesh->vertCol[mesh->faces[i].v[vx1]]).x,(mesh->vertCol[mesh->faces[i].v[vx1]]).y,(mesh->vertCol[mesh->faces[i].v[vx1]]).z),
								  //PRTVector((mesh->vertCol[mesh->faces[i].v[vx2]]).x,(mesh->vertCol[mesh->faces[i].v[vx2]]).y,(mesh->vertCol[mesh->faces[i].v[vx2]]).z),
								  //PRTVector((mesh->vertCol[mesh->faces[i].v[vx3]]).x,(mesh->vertCol[mesh->faces[i].v[vx3]]).y,(mesh->vertCol[mesh->faces[i].v[vx3]]).z),
								  mat
								  );
			
		}
		
		// This is the material ID for the face.
		// Note: If you use this you should make sure that the material ID
		// is not larger than the number of sub materials in the material.
		// The standard approach is to use a modulus function to bring down
		// the material ID.
//		fprintf(pStream,"\t%s %d", ID_MESH_MTLID, mesh->faces[i].getMatID());
		
//		fprintf(pStream,"\n");
	//}
//	fprintf(pStream,"%s\t}\n", indent.data()); // End face list
	
	// Export face map texcoords if we have them...
	if (!CheckForAndExportFaceMap(nodeMtl, mesh, indentLevel+1)) {
		// If not, export standard tverts
		int numTVx = mesh->getNumTVerts();

//		fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMTVERTEX, numTVx);

		if (numTVx) {
//			fprintf(pStream,"%s\t%s {\n",indent.data(), ID_MESH_TVERTLIST);
			for (i=0; i<numTVx; i++) {
				UVVert tv = mesh->tVerts[i];
//				fprintf(pStream, "%s\t\t%s %d\t%s\n",indent.data(), ID_MESH_TVERT, i, Format(tv));
			}
//			fprintf(pStream,"%s\t}\n",indent.data());
			
//			fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMTVFACES, mesh->getNumFaces());

//			fprintf(pStream, "%s\t%s {\n",indent.data(), ID_MESH_TFACELIST);
			for (i=0; i<mesh->getNumFaces(); i++) {
//				fprintf(pStream,"%s\t\t%s %d\t%d\t%d\t%d\n",
//					indent.data(),
//					ID_MESH_TFACE, i,
//					mesh->tvFace[i].t[vx1],
//					mesh->tvFace[i].t[vx2],
//					mesh->tvFace[i].t[vx3]);
			}
//			fprintf(pStream, "%s\t}\n",indent.data());
		}

		// CCJ 3/9/99
		// New for R3 - Additional mapping channels
		for (int mp = 2; mp < MAX_MESHMAPS-1; mp++) {
			if (mesh->mapSupport(mp)) {

//				fprintf(pStream, "%s\t%s %d {\n",indent.data(), ID_MESH_MAPPINGCHANNEL, mp);


				int numTVx = mesh->getNumMapVerts(mp);
//				fprintf(pStream, "%s\t\t%s %d\n",indent.data(), ID_MESH_NUMTVERTEX, numTVx);

				if (numTVx) {
//					fprintf(pStream,"%s\t\t%s {\n",indent.data(), ID_MESH_TVERTLIST);
					for (i=0; i<numTVx; i++) {
						UVVert tv = mesh->mapVerts(mp)[i];
//						fprintf(pStream, "%s\t\t\t%s %d\t%s\n",indent.data(), ID_MESH_TVERT, i, Format(tv));
					}
//					fprintf(pStream,"%s\t\t}\n",indent.data());
					
//					fprintf(pStream, "%s\t\t%s %d\n",indent.data(), ID_MESH_NUMTVFACES, mesh->getNumFaces());

//					fprintf(pStream, "%s\t\t%s {\n",indent.data(), ID_MESH_TFACELIST);
					for (i=0; i<mesh->getNumFaces(); i++) {
//						fprintf(pStream,"%s\t\t\t%s %d\t%d\t%d\t%d\n",
//							indent.data(),
//							ID_MESH_TFACE, i,
//							mesh->mapFaces(mp)[i].t[vx1],
//							mesh->mapFaces(mp)[i].t[vx2],
//							mesh->mapFaces(mp)[i].t[vx3]);
					}
//					fprintf(pStream, "%s\t\t}\n",indent.data());
				}
//				fprintf(pStream, "%s\t}\n",indent.data());
			}
		}
	}

	// Export color per vertex info
	if (/*GetIncludeVertexColors()*/1) {
		int numCVx = mesh->numCVerts;

//		fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMCVERTEX, numCVx);
		if (numCVx) {
//			fprintf(pStream,"%s\t%s {\n",indent.data(), ID_MESH_CVERTLIST);
			for (i=0; i<numCVx; i++) {
				Point3 vc = mesh->vertCol[i];
//				fprintf(pStream, "%s\t\t%s %d\t%s\n",indent.data(), ID_MESH_VERTCOL, i, Format(vc));
			}
//			fprintf(pStream,"%s\t}\n",indent.data());
			
//			fprintf(pStream, "%s\t%s %d\n",indent.data(), ID_MESH_NUMCVFACES, mesh->getNumFaces());

//			fprintf(pStream, "%s\t%s {\n",indent.data(), ID_MESH_CFACELIST);
			for (i=0; i<mesh->getNumFaces(); i++) {
//				fprintf(pStream,"%s\t\t%s %d\t%d\t%d\t%d\n",
//					indent.data(),
//					ID_MESH_CFACE, i,
//					mesh->vcFace[i].t[vx1],
//					mesh->vcFace[i].t[vx2],
//					mesh->vcFace[i].t[vx3]);
			}
//			fprintf(pStream, "%s\t}\n",indent.data());
		}
	}
	
	if (/*GetIncludeNormals()*/1) {
		// Export mesh (face + vertex) normals
//		fprintf(pStream, "%s\t%s {\n",indent.data(), ID_MESH_NORMALS);
		
		Point3 fn;  // Face normal
		Point3 vn;  // Vertex normal
		int  vert;
		Face* f;
		
		// Face and vertex normals.
		// In MAX a vertex can have more than one normal (but doesn't always have it).
		// This is depending on the face you are accessing the vertex through.
		// To get all information we need to export all three vertex normals
		// for every face.
		for (i=0; i<mesh->getNumFaces(); i++) {
			f = &mesh->faces[i];
			fn = mesh->getFaceNormal(i);
//			fprintf(pStream,"%s\t\t%s %d\t%s\n", indent.data(), ID_MESH_FACENORMAL, i, Format(fn));
			
			vert = f->getVert(vx1);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
//			fprintf(pStream,"%s\t\t\t%s %d\t%s\n",indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
			
			vert = f->getVert(vx2);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
//			fprintf(pStream,"%s\t\t\t%s %d\t%s\n",indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
			
			vert = f->getVert(vx3);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
//			fprintf(pStream,"%s\t\t\t%s %d\t%s\n",indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
		}
		
//		fprintf(pStream, "%s\t}\n",indent.data());
	}
	
//	fprintf(pStream, "%s}\n",indent.data());
	
	if (needDel) {
		delete tri;
	}
}

/*

  void AsciiExp::ExportMaterial(INode* node, int indentLevel)
{
	Mtl* mtl = node->GetMtl();
	
	TSTR indent = GetIndent(indentLevel);
	
	// If the node does not have a material, export the wireframe color
	if (mtl) {
		int mtlID = mtlList.GetMtlID(mtl);
		if (mtlID >= 0) {
			fprintf(pStream,"%s\t%s %d\n", indent.data(), ID_MATERIAL_REF, mtlID);
		}
	}
	else {
		DWORD c = node->GetWireColor();
		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_WIRECOLOR,
			Format(Color(GetRValue(c)/255.0f, GetGValue(c)/255.0f, GetBValue(c)/255.0f)));
	}
}

void AsciiExp::DumpMaterial(Mtl* mtl, int mtlID, int subNo, int indentLevel)
{
	int i;
	TimeValue t = GetStaticFrame();
	
	if (!mtl) return;
	
	TSTR indent = GetIndent(indentLevel+1);
	
	TSTR className;
	mtl->GetClassName(className);
	
	
	if (subNo == -1) {
		// Top level material
		fprintf(pStream,"%s%s %d {\n",indent.data(), ID_MATERIAL, mtlID);
	}
	else {
		fprintf(pStream,"%s%s %d {\n",indent.data(), ID_SUBMATERIAL, subNo);
	}
	fprintf(pStream,"%s\t%s \"%s\"\n",indent.data(), ID_MATNAME, FixupName(mtl->GetName()));
	fprintf(pStream,"%s\t%s \"%s\"\n",indent.data(), ID_MATCLASS, FixupName(className));
	
	// We know the Standard material, so we can get some extra info
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
		StdMat* std = (StdMat*)mtl;

		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_AMBIENT, Format(std->GetAmbient(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_DIFFUSE, Format(std->GetDiffuse(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SPECULAR, Format(std->GetSpecular(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SHINE, Format(std->GetShininess(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SHINE_STRENGTH, Format(std->GetShinStr(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_TRANSPARENCY, Format(std->GetXParency(t)));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_WIRESIZE, Format(std->WireSize(t)));

		fprintf(pStream,"%s\t%s ", indent.data(), ID_SHADING);
		switch(std->GetShading()) {
		case SHADE_CONST: fprintf(pStream,"%s\n", ID_MAT_SHADE_CONST); break;
		case SHADE_PHONG: fprintf(pStream,"%s\n", ID_MAT_SHADE_PHONG); break;
		case SHADE_METAL: fprintf(pStream,"%s\n", ID_MAT_SHADE_METAL); break;
		case SHADE_BLINN: fprintf(pStream,"%s\n", ID_MAT_SHADE_BLINN); break;
		default: fprintf(pStream,"%s\n", ID_MAT_SHADE_OTHER); break;
		}
		
		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_XP_FALLOFF, Format(std->GetOpacFalloff(t)));
		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_SELFILLUM, Format(std->GetSelfIllum(t)));
		
		if (std->GetTwoSided()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_TWOSIDED);
		}
		
		if (std->GetWire()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_WIRE);
		}
		
		if (std->GetWireUnits()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_WIREUNITS);
		}
		
		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_FALLOFF, std->GetFalloffOut() ? ID_FALLOFF_OUT : ID_FALLOFF_IN);
		
		if (std->GetFaceMap()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_FACEMAP);
		}
		
		if (std->GetSoften()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_SOFTEN);
		}
		
		fprintf(pStream,"%s\t%s ", indent.data(), ID_XP_TYPE);
		switch (std->GetTransparencyType()) {
		case TRANSP_FILTER: fprintf(pStream,"%s\n", ID_MAP_XPTYPE_FLT); break;
		case TRANSP_SUBTRACTIVE: fprintf(pStream,"%s\n", ID_MAP_XPTYPE_SUB); break;
		case TRANSP_ADDITIVE: fprintf(pStream,"%s\n", ID_MAP_XPTYPE_ADD); break;
		default: fprintf(pStream,"%s\n", ID_MAP_XPTYPE_OTH); break;
		}
	}
	else {
		// Note about material colors:
		// This is only the color used by the interactive renderer in MAX.
		// To get the color used by the scanline renderer, we need to evaluate
		// the material using the mtl->Shade() method.
		// Since the materials are procedural there is no real "diffuse" color for a MAX material
		// but we can at least take the interactive color.
		
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_AMBIENT, Format(mtl->GetAmbient()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_DIFFUSE, Format(mtl->GetDiffuse()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SPECULAR, Format(mtl->GetSpecular()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SHINE, Format(mtl->GetShininess()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_SHINE_STRENGTH, Format(mtl->GetShinStr()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_TRANSPARENCY, Format(mtl->GetXParency()));
		fprintf(pStream,"%s\t%s %s\n",indent.data(), ID_WIRESIZE, Format(mtl->WireSize()));
	}

	for (i=0; i<mtl->NumSubTexmaps(); i++) {
		Texmap* subTex = mtl->GetSubTexmap(i);
		float amt = 1.0f;
		if (subTex) {
			// If it is a standard material we can see if the map is enabled.
			if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
				if (!((StdMat*)mtl)->MapEnabled(i))
					continue;
				amt = ((StdMat*)mtl)->GetTexmapAmt(i, 0);
				
			}
			DumpTexture(subTex, mtl->ClassID(), i, amt, indentLevel+1);
		}
	}
	
	if (mtl->NumSubMtls() > 0)  {
		fprintf(pStream,"%s\t%s %d\n",indent.data(), ID_NUMSUBMTLS, mtl->NumSubMtls());
		
		for (i=0; i<mtl->NumSubMtls(); i++) {
			Mtl* subMtl = mtl->GetSubMtl(i);
			if (subMtl) {
				DumpMaterial(subMtl, 0, i, indentLevel+1);
			}
		}
	}
	fprintf(pStream,"%s}\n", indent.data());
}


// For a standard material, this will give us the meaning of a map
// givien its submap id.
TCHAR* AsciiExp::GetMapID(Class_ID cid, int subNo)
{
	static TCHAR buf[50];
	
	if (cid == Class_ID(0,0)) {
		strcpy(buf, ID_ENVMAP);
	}
	else if (cid == Class_ID(DMTL_CLASS_ID, 0)) {
		switch (subNo) {
		case ID_AM: strcpy(buf, ID_MAP_AMBIENT); break;
		case ID_DI: strcpy(buf, ID_MAP_DIFFUSE); break;
		case ID_SP: strcpy(buf, ID_MAP_SPECULAR); break;
		case ID_SH: strcpy(buf, ID_MAP_SHINE); break;
		case ID_SS: strcpy(buf, ID_MAP_SHINESTRENGTH); break;
		case ID_SI: strcpy(buf, ID_MAP_SELFILLUM); break;
		case ID_OP: strcpy(buf, ID_MAP_OPACITY); break;
		case ID_FI: strcpy(buf, ID_MAP_FILTERCOLOR); break;
		case ID_BU: strcpy(buf, ID_MAP_BUMP); break;
		case ID_RL: strcpy(buf, ID_MAP_REFLECT); break;
		case ID_RR: strcpy(buf, ID_MAP_REFRACT); break;
		}
	}
	else {
		strcpy(buf, ID_MAP_GENERIC);
	}
	
	return buf;
}

void AsciiExp::DumpTexture(Texmap* tex, Class_ID cid, int subNo, float amt, int indentLevel)
{
	if (!tex) return;
	
	TSTR indent = GetIndent(indentLevel+1);
	
	TSTR className;
	tex->GetClassName(className);
	
	fprintf(pStream,"%s%s {\n", indent.data(), GetMapID(cid, subNo));
	
	fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_TEXNAME, FixupName(tex->GetName()));
	fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_TEXCLASS, FixupName(className));
	
	// If we include the subtexture ID, a parser could be smart enough to get
	// the class name of the parent texture/material and make it mean something.
	fprintf(pStream,"%s\t%s %d\n", indent.data(), ID_TEXSUBNO, subNo);
	
	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_TEXAMOUNT, Format(amt));
	
	// Is this a bitmap texture?
	// We know some extra bits 'n pieces about the bitmap texture
	if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)) {
		TSTR mapName = ((BitmapTex *)tex)->GetMapName();
		fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_BITMAP, FixupName(mapName));
		
		StdUVGen* uvGen = ((BitmapTex *)tex)->GetUVGen();
		if (uvGen) {
			DumpUVGen(uvGen, indentLevel+1);
		}
		
		TextureOutput* texout = ((BitmapTex*)tex)->GetTexout();
		if (texout->GetInvert()) {
			fprintf(pStream,"%s\t%s\n", indent.data(), ID_TEX_INVERT);
		}
		
		fprintf(pStream,"%s\t%s ", indent.data(), ID_BMP_FILTER);
		switch(((BitmapTex*)tex)->GetFilterType()) {
		case FILTER_PYR:  fprintf(pStream,"%s\n", ID_BMP_FILT_PYR); break;
		case FILTER_SAT: fprintf(pStream,"%s\n", ID_BMP_FILT_SAT); break;
		default: fprintf(pStream,"%s\n", ID_BMP_FILT_NONE); break;
		}
	}
	
	for (int i=0; i<tex->NumSubTexmaps(); i++) {
		DumpTexture(tex->GetSubTexmap(i), tex->ClassID(), i, 1.0f, indentLevel+1);
	}
	
	fprintf(pStream, "%s}\n", indent.data());
}

void AsciiExp::DumpUVGen(StdUVGen* uvGen, int indentLevel)
{
	int mapType = uvGen->GetCoordMapping(0);
	TimeValue t = GetStaticFrame();
	TSTR indent = GetIndent(indentLevel+1);
	
	fprintf(pStream,"%s%s ", indent.data(), ID_MAPTYPE);
	
	switch (mapType) {
	case UVMAP_EXPLICIT: fprintf(pStream,"%s\n", ID_MAPTYPE_EXP); break;
	case UVMAP_SPHERE_ENV: fprintf(pStream,"%s\n", ID_MAPTYPE_SPH); break;
	case UVMAP_CYL_ENV:  fprintf(pStream,"%s\n", ID_MAPTYPE_CYL); break;
	case UVMAP_SHRINK_ENV: fprintf(pStream,"%s\n", ID_MAPTYPE_SHR); break;
	case UVMAP_SCREEN_ENV: fprintf(pStream,"%s\n", ID_MAPTYPE_SCR); break;
	}
	
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_U_OFFSET, Format(uvGen->GetUOffs(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_V_OFFSET, Format(uvGen->GetVOffs(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_U_TILING, Format(uvGen->GetUScl(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_V_TILING, Format(uvGen->GetVScl(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_ANGLE, Format(uvGen->GetAng(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_BLUR, Format(uvGen->GetBlur(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_BLUR_OFFSET, Format(uvGen->GetBlurOffs(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_NOISE_AMT, Format(uvGen->GetNoiseAmt(t)));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_NOISE_SIZE, Format(uvGen->GetNoiseSize(t)));
	fprintf(pStream,"%s%s %d\n", indent.data(), ID_NOISE_LEVEL, uvGen->GetNoiseLev(t));
	fprintf(pStream,"%s%s %s\n", indent.data(), ID_NOISE_PHASE, Format(uvGen->GetNoisePhs(t)));
}

  */

void PRTMaxPlugin::ExportGeomObject(INode* node, int indentLevel,TimeValue t)
{
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj)
		return;
	
	// Targets are actually geomobjects, but we will export them
	// from the camera and light objects, so we skip them here.
	if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		return;
	
	
	TSTR indent = GetIndent(indentLevel);
	
	//ExportNodeHeader(node, ID_GEOMETRY, indentLevel);
	
	//ExportNodeTM(node, indentLevel);
	
	ExportMesh(node, indentLevel,t);
	
	// Node properties (only for geomobjects)
/*	fprintf(pStream, "%s\t%s %d\n", indent.data(), ID_PROP_MOTIONBLUR, node->MotBlur());
	fprintf(pStream, "%s\t%s %d\n", indent.data(), ID_PROP_CASTSHADOW, node->CastShadows());
	fprintf(pStream, "%s\t%s %d\n", indent.data(), ID_PROP_RECVSHADOW, node->RcvShadows());

	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
	}

	// Export the visibility track
	Control* visCont = node->GetVisController();
	if (visCont) {
		fprintf(pStream, "%s\t%s {\n", indent.data(), ID_VISIBILITY_TRACK);
		DumpFloatKeys(visCont, indentLevel);
		fprintf(pStream, "\t}\n");
	}

	if (GetIncludeMtl()) {
		ExportMaterial(node, indentLevel);
	}

	if (GetIncludeMeshAnim()) {
		ExportAnimMesh(node, indentLevel);
	}
	
	if (GetIncludeIKJoints()) {
		ExportIKJoints(node, indentLevel);
	}
	
	fprintf(pStream,"%s}\n", indent.data());
*/
}



//      EXPORT CAMERA OBJECT


void ExportCameraSettings(CameraState* cs, CameraObject* cam, TimeValue t, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);

//	fprintf(pStream,"%s\t%s {\n", indent.data(), ID_CAMERA_SETTINGS);

	// Frame #
//	fprintf(pStream, "%s\t\t%s %d\n", indent.data(), ID_TIMEVALUE, t);

	if (cs->manualClip) {
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_HITHER, Format(cs->hither));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_YON, Format(cs->yon));
	}

//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_NEAR, Format(cs->nearRange));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_FAR, Format(cs->farRange));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_FOV, Format(cs->fov));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_CAMERA_TDIST, Format(cam->GetTDist(t)));

//	fprintf(pStream,"%s\t}\n",indent.data());
}

BOOL IsKnownController(Control* cont)
{
	ulong partA, partB;

	if (!cont)
		return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)
		return FALSE;

	switch (partA) {
		case TCBINTERP_POSITION_CLASS_ID:
		case TCBINTERP_ROTATION_CLASS_ID:
		case TCBINTERP_SCALE_CLASS_ID:
		case HYBRIDINTERP_POSITION_CLASS_ID:
		case HYBRIDINTERP_ROTATION_CLASS_ID:
		case HYBRIDINTERP_SCALE_CLASS_ID:
		case LININTERP_POSITION_CLASS_ID:
		case LININTERP_ROTATION_CLASS_ID:
		case LININTERP_SCALE_CLASS_ID:
			return TRUE;
	}

	return FALSE;
}

void DumpPosKeys(Control* cont, int indentLevel) 
{
	if (!cont)
		return;
	
	int i;
	TSTR indent = GetIndent(indentLevel);
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	// TCB position
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_TCB); 
			for (i=0; i<numKeys; i++) {
				ITCBPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s",
//					indent.data(),
//					ID_TCB_POS_KEY,
//					key.time,
//					Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	// Bezier position
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0))
	{
		int numKeys;
		if(numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_BEZIER); 
			for (i=0; i<numKeys; i++) {
				IBezPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s",
//					indent.data(), 
//					ID_BEZIER_POS_KEY,
//					key.time,
//					Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	// Linear position
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0)) {
		int numKeys;
		if(numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_LINEAR); 
			for (i=0; i<numKeys; i++) {
				ILinPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n",
//					indent.data(),
//					ID_POS_KEY,
//					key.time,
//					Format(key.val));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}

void DumpRotKeys(Control* cont, int indentLevel) 
{
	if (!cont)
		return;
	
	int i;
	TSTR indent = GetIndent(indentLevel);
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_TCB); 
			for (i=0; i<numKeys; i++) {
				ITCBRotKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s", 
	//				indent.data(),
	//				ID_TCB_ROT_KEY,
	//				key.time,
	//				Format(key.val));
	//			fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_BEZIER); 
			for (i=0; i<numKeys; i++) {
				IBezQuatKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s\n", 
	//				indent.data(),
					 // Quaternions are converted to AngAxis when written to file
	//				ID_ROT_KEY,
	//				key.time,
	//				Format(key.val));
				// There is no intan/outtan for Quat Rotations
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_LINEAR); 
			for (i=0; i<numKeys; i++) {
				ILinRotKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s\n",
	//				indent.data(), 
					 // Quaternions are converted to AngAxis when written to file
	//				ID_ROT_KEY,
	//				key.time,
	//				Format(key.val));
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}

void DumpScaleKeys(Control* cont, int indentLevel) 
{
	if (!cont)
		return;
	
	int i;
	TSTR indent = GetIndent(indentLevel);
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_TCB); 
			for (i=0; i<numKeys; i++) {
				ITCBScaleKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s",
	//				indent.data(),
	//				ID_TCB_SCALE_KEY,
	//				key.time,
	//				Format(key.val));
	//			fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_BEZIER); 
			for (i=0; i<numKeys; i++) {
				IBezScaleKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s",
	//				indent.data(),
	//				ID_BEZIER_SCALE_KEY,
	//				key.time,
	//				Format(key.val));
	//			fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_SCALE_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
	//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_LINEAR); 
			for (i=0; i<numKeys; i++) {
				ILinScaleKey key;
				ikc->GetKey(i, &key);
	//			fprintf(pStream, "%s\t\t\t%s %d\t%s\n",
	//				indent.data(),
	//				ID_SCALE_KEY,
	//				key.time,
	//				Format(key.val));
			}
	//		fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}

#define ALMOST_ZERO 1.0e-3f

BOOL EqualPoint3(Point3 p1, Point3 p2)
{
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.z - p2.z) > ALMOST_ZERO)
		return FALSE;

	return TRUE;
}

BOOL CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale, Interface *ip)
{
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3 firstPos;
	float rotAngle, firstRotAngle;
	Point3 rotAxis, firstRotAxis;
	Point3 firstScaleFactor;

	bPos = bRot = bScale = FALSE;

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		AngAxisFromQ(ap.q, &rotAngle, rotAxis);

		if (t != start) {
			if (!bPos) {
				if (!EqualPoint3(ap.t, firstPos)) {
					bPos = TRUE;
					}
				}
			// MAX 2.x:
			// We examine the rotation angle to see if the rotation component
			// has changed.
			// Although not entierly true, it should work.
			// It is rare that the rotation axis is animated without
			// the rotation angle being somewhat affected.
			// MAX 3.x:
			// The above did not work, I have a repro scene that doesn't export a rotation track
			// because of this. I fixed it to also compare the axis.
			if (!bRot) {
				if (fabs(rotAngle - firstRotAngle) > ALMOST_ZERO) {
					bRot = TRUE;
					}
				else if (!EqualPoint3(rotAxis, firstRotAxis)) {
					bRot = TRUE;
					}
				}

			if (!bScale) {
				if (!EqualPoint3(ap.k, firstScaleFactor)) {
					bScale = TRUE;
					}
				}
			}
		else {
			firstPos = ap.t;
			firstRotAngle = rotAngle;
			firstRotAxis = rotAxis;
			firstScaleFactor = ap.k;
			}

		// No need to continue looping if all components are animated
		if (bPos && bRot && bScale)
			break;
		}

	return bPos || bRot || bScale;
}

void DumpPosSample(INode* node, int indentLevel, Interface *ip) 
{	
	TSTR indent = GetIndent(indentLevel);
	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_POS_TRACK);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame() * /*GetKeyFrameStep()*/5;
	Matrix3 tm;
	AffineParts ap;
	Point3	prevPos;

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		Point3 pos = ap.t;

		if (t!= start && EqualPoint3(pos, prevPos)) {
			// Skip identical keys 
			continue;
		}

		prevPos = pos;

		// Output the sample
//		fprintf(pStream, "%s\t\t\t%s %d\t%s\n",
//			indent.data(),
//			ID_POS_SAMPLE,
//			t,
//			Format(pos));
	}

//	fprintf(pStream,"%s\t\t}\n", indent.data());
}

void DumpRotSample(INode* node, int indentLevel, Interface *ip) 
{	
	TSTR indent = GetIndent(indentLevel);
	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_ROT_TRACK);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame() * /*GetKeyFrameStep()*/5;
	Matrix3 tm;
	AffineParts ap;
	Quat prevQ;

	prevQ.Identity();

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		// Rotation keys should be relative, so we need to convert these
		// absolute samples to relative values.

		Quat q = ap.q / prevQ;
		prevQ = ap.q;

		if (q.IsIdentity()) {
			// No point in exporting null keys...
			continue;
		}

		// Output the sample
//		fprintf(pStream, "%s\t\t\t%s %d\t%s\n",
//			indent.data(),
//			ID_ROT_SAMPLE,
//			t,
//			Format(q));
	}

//	fprintf(pStream,"%s\t\t}\n", indent.data());
}

void DumpScaleSample(INode* node, int indentLevel, Interface *ip) 
{	
	TSTR indent = GetIndent(indentLevel);
	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_SCALE_TRACK);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame() * /*GetKeyFrameStep()*/5;
	Matrix3 tm;
	AffineParts ap;
	Point3	prevFac;

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		if (t!= start && EqualPoint3(ap.k, prevFac)) {
			// Skip identical keys 
			continue;
		}

		prevFac = ap.k;

		// Output the sample
//		fprintf(pStream, "%s\t\t\t%s %d\t%s %s\n",
//			indent.data(),
//			ID_SCALE_SAMPLE,
//			t,
//			Format(ap.k),
//			Format(ap.u));
	}

//	fprintf(pStream,"%s\t\t}\n", indent.data());
}

void ExportAnimKeys(INode* node, int indentLevel, Interface *ip) 
{
	TSTR indent = GetIndent(indentLevel);
	BOOL bPosAnim;
	BOOL bRotAnim;
	BOOL bScaleAnim;
	BOOL bDoKeys = FALSE;

	// We can only export keys if all TM controllers are "known" to us.
	// The reason for that is that some controllers control more than what
	// they should. Consider a path position controller, if you turn on
	// follow and banking, this position controller will also control
	// rotation. If a node that had a path position controller also had a
	// TCB rotation controller, the TCB keys would not describe the whole
	// rotation of the node.
	// For that reason we will only export keys if all controllers
	// position, rotation and scale are linear, hybrid (bezier) or tcb.

	if (!/*GetAlwaysSample()*/1) {
		Control* pC = node->GetTMController()->GetPositionController();
		Control* rC = node->GetTMController()->GetRotationController();
		Control* sC = node->GetTMController()->GetScaleController();

		if (IsKnownController(pC) && IsKnownController(rC) && IsKnownController(sC)) {
			bDoKeys = TRUE;
		}
	}

	if (bDoKeys) {
		// Only dump the track header if any of the controllers have keys
		if (node->GetTMController()->GetPositionController()->NumKeys() ||
			node->GetTMController()->GetRotationController()->NumKeys() ||
			node->GetTMController()->GetScaleController()->NumKeys()) {

	//		fprintf(pStream,"%s\t%s {\n", indent.data(), ID_TM_ANIMATION); 
	//		fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_NODE_NAME,
	//			FixupName(node->GetName()));

			DumpPosKeys(node->GetTMController()->GetPositionController(), indentLevel);
			DumpRotKeys(node->GetTMController()->GetRotationController(), indentLevel);
			DumpScaleKeys(node->GetTMController()->GetScaleController(), indentLevel);

	//		fprintf(pStream,"%s\t}\n", indent.data());
		}
	}
	else if (CheckForAnimation(node, bPosAnim, bRotAnim, bScaleAnim,ip)) {
	//	fprintf(pStream,"%s\t%s {\n", indent.data(), ID_TM_ANIMATION); 
	//	fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_NODE_NAME,
	//		FixupName(node->GetName()));

		if (bPosAnim) {
			DumpPosSample(node, indentLevel, ip);
		}
		if (bRotAnim) {
			DumpRotSample(node, indentLevel, ip);
		}
		if (bScaleAnim) {
			DumpScaleSample(node, indentLevel, ip);
		}

	//	fprintf(pStream,"%s\t}\n", indent.data());
	}
}




void ExportCameraObject(INode* node, int indentLevel,Interface *ip)
{
	TSTR indent = GetIndent(indentLevel);

	//ExportNodeHeader(node, ID_CAMERA, indentLevel);

	INode* target = node->GetTarget();
	if (target) {
	//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_CAMERA_TYPE, ID_CAMERATYPE_TARGET);
	}
	else {
	//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_CAMERA_TYPE, ID_CAMERATYPE_FREE);
	}


	//ExportNodeTM(node, indentLevel);
	// If we have a target object, export animation keys for the target too.
	/*if (target) {
		ExportNodeTM(target, indentLevel);
	}*/
	
	CameraState cs;
	TimeValue t = 0;
	Interval valid = FOREVER;
	// Get animation range
	Interval animRange = ip->GetAnimRange();
	
	ObjectState os = node->EvalWorldState(t);
	CameraObject *cam = (CameraObject *)os.obj;
	
	cam->EvalCameraState(t,valid,&cs);
	
	ExportCameraSettings(&cs, cam, t, indentLevel);

	// Export animated camera settings
	if (!valid.InInterval(animRange) && /*GetIncludeCamLightAnim()*/1) {

	//	fprintf(pStream,"%s\t%s {\n", indent.data(), ID_CAMERA_ANIMATION);

		TimeValue t = animRange.Start();
		
		while (1) {
			valid = FOREVER; // Extend the validity interval so the camera can shrink it.
			cam->EvalCameraState(t,valid,&cs);

			t = valid.Start() < animRange.Start() ? animRange.Start() : valid.Start();
			
			// Export the camera settings at this frame
			ExportCameraSettings(&cs, cam, t, indentLevel+1);
			
			if (valid.End() >= animRange.End()) {
				break;
			}
			else {
				t = (valid.End()/GetTicksPerFrame()+/*GetMeshFrameStep()*/5) * GetTicksPerFrame();
			}
		}

	//	fprintf(pStream,"%s\t}\n", indent.data());
	}
	
	// Export animation keys for the light node
	if (/*GetIncludeAnim()*/1) {
		ExportAnimKeys(node, indentLevel, ip);
		
		// If we have a target, export animation keys for the target too.
		if (target) {
			ExportAnimKeys(target, indentLevel, ip);
		}
	}
	
	//fprintf(pStream,"%s}\n", indent.data());
}



//        EXPORT LIGHT OBJECT



void ExportLightSettings(LightState* ls, GenLight* light, TimeValue t, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);

//	fprintf(pStream,"%s\t%s {\n", indent.data(), ID_LIGHT_SETTINGS);

	// Frame #
//	fprintf(pStream, "%s\t\t%s %d\n",indent.data(), ID_TIMEVALUE, t);

//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_COLOR, Format(ls->color));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_INTENS, Format(ls->intens));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_ASPECT, Format(ls->aspect));
	
//	if (ls->type != OMNI_LIGHT) {
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_HOTSPOT, Format(ls->hotsize));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_FALLOFF, Format(ls->fallsize));
//	}
//	if (ls->type != DIR_LIGHT && ls->useAtten) {
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_ATTNSTART, Format(ls->attenStart));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_ATTNEND,	Format(ls->attenEnd));
//	}

//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_TDIST, Format(light->GetTDist(t, FOREVER)));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_MAPBIAS, Format(light->GetMapBias(t, FOREVER)));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_MAPRANGE, Format(light->GetMapRange(t, FOREVER)));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_MAPSIZE, Format(light->GetMapSize(t, FOREVER)));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_RAYBIAS, Format(light->GetRayBias(t, FOREVER)));

//	fprintf(pStream,"%s\t}\n", indent.data());
}

void PRTMaxPlugin::ExportLightObject(INode* node, int indentLevel, TimeValue t)
{
	//TimeValue t = /*GetStaticFrame()*/t;
	TSTR indent = GetIndent(indentLevel);

	//ExportNodeHeader(node, ID_LIGHT, indentLevel);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj) {
//		fprintf(pStream, "%s}\n", indent.data());
		return;
	}
	
	GenLight* light = (GenLight*)os.obj;
	struct LightState ls;
	Interval valid = FOREVER;
	Interval animRange = ip->GetAnimRange();

	light->EvalLightState(t, valid, &ls);

	// This is part os the lightState, but it doesn't
	// make sense to output as an animated setting so
	// we dump it outside of ExportLightSettings()

//	fprintf(pStream, "%s\t%s ", indent.data(), ID_LIGHT_TYPE);
	switch(ls.type) {
	case OMNI_LIGHT:  
		{
			//fprintf(pStream, "%s\n", ID_LIGHT_TYPE_OMNI); 
			
			Matrix3 lposmat=node->GetNodeTM(t);
			Point3 lpos=lposmat.PointTransform(Point3(0,0,0));
			
			int lposnum=trazador.LightsList.Lenght();
			trazador.AddPointLight(PRTVector(lpos.x,lpos.y,lpos.z),PRTVector(ls.color.r,ls.color.g,ls.color.b));
			trazador.GetLightAtPos(lposnum)->intensity=ls.intens;
			//por la naturaleza de la omni light del 3dsmax ¿?
			//trazador.GetLightAtPos(lposnum)->attenuation2=0;
			//trazador.GetLightAtPos(lposnum)->attenuation1=0;
			//trazador.GetLightAtPos(lposnum)->attenuation0=1;
						
			break;
		}
//	case TSPOT_LIGHT: fprintf(pStream, "%s\n", ID_LIGHT_TYPE_TARG);  break;
//	case DIR_LIGHT:   fprintf(pStream, "%s\n", ID_LIGHT_TYPE_DIR); break;
//	case FSPOT_LIGHT: fprintf(pStream, "%s\n", ID_LIGHT_TYPE_FREE); break;
	}

	/*ExportNodeTM(node, indentLevel);
	// If we have a target object, export Node TM for the target too.
	*/INode* target = node->GetTarget();
	/*if (target) {
		ExportNodeTM(target, indentLevel);
	}*/

	int shadowMethod = light->GetShadowMethod();
//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_SHADOWS,
//			shadowMethod == LIGHTSHADOW_NONE ? ID_LIGHT_SHAD_OFF :
//			shadowMethod == LIGHTSHADOW_MAPPED ? ID_LIGHT_SHAD_MAP :
//			ID_LIGHT_SHAD_RAY);

	
//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_USELIGHT, Format(light->GetUseLight()));
	
//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_SPOTSHAPE, 
//		light->GetSpotShape() == RECT_LIGHT ? ID_LIGHT_SHAPE_RECT : ID_LIGHT_SHAPE_CIRC);

//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_USEGLOBAL, Format(light->GetUseGlobal()));
//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_ABSMAPBIAS, Format(light->GetAbsMapBias()));
//	fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_LIGHT_OVERSHOOT, Format(light->GetOvershoot()));

	ExclList* el = light->GetExclList();  // DS 8/31/00 . switched to NodeIDTab from NameTab
	if (el->Count()) {
//		fprintf(pStream,"%s\t%s {\n", indent.data(), ID_LIGHT_EXCLUSIONLIST);
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_NUMEXCLUDED, Format(el->Count()));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_EXCLINCLUDE, Format(el->TestFlag(NT_INCLUDE)));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_EXCL_AFFECT_ILLUM, Format(el->TestFlag(NT_AFFECT_ILLUM)));
//		fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_EXCL_AFFECT_SHAD, Format(el->TestFlag(NT_AFFECT_SHADOWCAST)));
		for (int nameid = 0; nameid < el->Count(); nameid++) {
			INode *n = (*el)[nameid];	// DS 8/31/00
//			if (n)
//				fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_LIGHT_EXCLUDED, n->GetName());
			}
//		fprintf(pStream,"%s\t}\n", indent.data());
	}

	// Export light settings for frame 0
	ExportLightSettings(&ls, light, t, indentLevel);

	// Export animated light settings
	if (!valid.InInterval(animRange) && /*GetIncludeCamLightAnim()*/1) {
//		fprintf(pStream,"%s\t%s {\n", indent.data(), ID_LIGHT_ANIMATION);

		TimeValue t = animRange.Start();
		
		while (1) {
			valid = FOREVER; // Extend the validity interval so the camera can shrink it.
			light->EvalLightState(t, valid, &ls);

			t = valid.Start() < animRange.Start() ? animRange.Start() : valid.Start();
			
			// Export the light settings at this frame
			ExportLightSettings(&ls, light, t, indentLevel+1);
			
			if (valid.End() >= animRange.End()) {
				break;
			}
			else {
				t = (valid.End()/GetTicksPerFrame()+/*GetMeshFrameStep()*/5) * GetTicksPerFrame();
			}
		}

//		fprintf(pStream,"%s\t}\n", indent.data());
	}

	// Export animation keys for the light node
	if (/*GetIncludeAnim()*/1) {
		ExportAnimKeys(node, indentLevel, ip);
		
		// If we have a target, export animation keys for the target too.
		if (target) {
			ExportAnimKeys(target, indentLevel, ip);
		}
	}
	
//	fprintf(pStream,"%s}\n", indent.data());
}



//			   NODEENUM



BOOL PRTMaxPlugin::nodeEnum(INode* node, int indentLevel, TimeValue t) 
{
	//if(node->Selected() == FALSE)
	//	return TREE_CONTINUE;

	//if (ip->GetCancel())
	//	return FALSE;
	
	TSTR indent = GetIndent(indentLevel);

	if (node->IsGroupHead()) 
	{
		indentLevel++;
	}

	ObjectState os = node->EvalWorldState(t); 

	if (os.obj) 
	{

			// We look at the super class ID to determine the type of the object.
			switch(os.obj->SuperClassID()) {
			case GEOMOBJECT_CLASS_ID: 
				ExportGeomObject(node, indentLevel,t); 
				break;
			case CAMERA_CLASS_ID:
				ExportCameraObject(node, indentLevel, ip); 
				break;
			case LIGHT_CLASS_ID:
				ExportLightObject(node, indentLevel,t); 
				break;
			/*case SHAPE_CLASS_ID:
				ExportShapeObject(node, indentLevel); 
				break;
			case HELPER_CLASS_ID:
				ExportHelperObject(node, indentLevel); 
				break;*/
			}
		
	}

	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnum(node->GetChildNode(c), indentLevel,t))
			return FALSE;
	}

	if (node->IsGroupHead()) {
//		fprintf(pStream,"%s}\n", indent.data());
		indentLevel--;
	}

	return TRUE;
}



//              OPEN



int PRTMaxPlugin::Open(
		INode *Scene,     	// root node of scene to render
		INode *vnode,     	// view node (camera or light), or NULL
		ViewParams *viewPar,// view params for rendering ortho or user viewport
		RendParams &rpar,  	// common renderer parameters
		HWND hwnd, 				// owner window, for messages
		DefaultLight* defaultLights, // Array of default lights if none in scene
		int numDefLights	// number of lights in defaultLights array
)
{

	/*for(int i=0;i<numDefLights;i++)
	{
		//if ((defaultLights+i)->ls.type==OMNI_LIGHT) //es una luz direccional!!! arggg
		{
			Matrix3 lposmat=(defaultLights+i)->tm;
			Point3 lpos=lposmat.PointTransform(Point3(0,0,0));
			int lposnum=trazador.LightsList.Lenght();
			trazador.AddPointLight(PRTVector(lpos.x,lpos.y,lpos.z),PRTVector((defaultLights+i)->ls.color.r,(defaultLights+i)->ls.color.g,(defaultLights+i)->ls.color.b));
			trazador.GetLightAtPos(lposnum)->intensity=(defaultLights+i)->ls.intens;
		}
	}*/


//	RP=rp;
	scene=Scene;

	rendParams.bVideoColorCheck = rpar.colorCheck;
	rendParams.bForce2Sided = rpar.force2Side;
	rendParams.bRenderHidden = rpar.rendHidden;
	rendParams.bSuperBlack = rpar.superBlack;
	rendParams.bRenderFields = rpar.fieldRender;
	rendParams.bNetRender = rpar.isNetRender;
	rendParams.rendType = rpar.rendType;
	rendParams.nNumDefLights = numDefLights;
	rendParams.pDefaultLights = defaultLights;
	rendParams.effect = rpar.effect;

	pViewNode = vnode;
	//theView.pRendParams = &rendParams;
	if (viewPar) 
		view = *viewPar;

//	PreProcess(scene,0,mtlList);
//	int numMtls = mtlList.Count();

	//ip = GetCOREInterface();
	//int numChildren = ip->GetRootNode()->NumberOfChildren();
//	int numChildren = scene->NumberOfChildren();
	
//	for (int idx=0; idx<numChildren; idx++) {
		//if (ip->GetCancel())
		//	break;
		//nodeEnum(ip->GetRootNode()->GetChildNode(idx), 0);
//		nodeEnum(scene->GetChildNode(idx), 0,0);
//	}

	/*	if (lightTab.Count() == 0 && defaultLights) {
		for (idx = 0; idx < numDefLights; idx++) {
			RenderLight* rl = new RenderLight(&defaultLights[idx]);
			lightTab.Append(1, &rl);
		}
		DebugPrint("\tUsing %d default lights\n", lightTab.Count());
	}
*/

	rendParams.atmos = rpar.atmos;
	rendParams.envMap = rpar.envMap;


/*	if (rendParams.atmos)

		EnumRefs(rendParams.atmos,getmaps);

	if (rendParams.envMap)
		EnumRefs(rendParams.envMap,getmaps);

  BeginThings();
*/
	bOpen=TRUE;

	trazador.BTextures=BTextures;
	trazador.BReflections=BReflections;
	trazador.BRefractions=BRefractions;
	trazador.BAlpha=BAlpha;
	trazador.BSpecular=BSpecular;
	trazador.BShadows=BShadows;
	trazador.BDoubleSided=BDoubleSided;
	trazador.BNInter=BNInter;
	trazador.BCInter=BCInter;
	trazador.BOctrees=BOctrees;
	trazador.BConvexHulls=BConvexHulls;
	trazador.BIndirectLight=BIndirectLight;
	trazador.BGiReflections=BGiReflections;
	trazador.BGiRefractions=BGiRefractions;
	trazador.BGiAlpha=BGiAlpha;
	trazador.BGlossy=BGlossy;
	trazador.MejoraPorDistancia=BMejoraPorDistancia;
	((PRTPhotonMapping*)trazador.render)->numphotones=NumPhotones;
	((PRTPhotonMapping*)trazador.render)->scalephoton=ScalePhoton;
	((PRTPhotonMapping*)trazador.render)->groupphotones=GroupPhotones;
	trazador.octreesdeep=OctreesDeep;


	return 1;
}



//              RENDER


BMM_Color_64 colTo64(Color c)
{
	BMM_Color_64 bc;

	// Clamp the colors
	c.ClampMinMax();

	bc.r = (WORD)(c.r * 65535.0);
	bc.g = (WORD)(c.g * 65535.0);
	bc.b = (WORD)(c.b * 65535.0);

	return bc;
}

int PRTMaxPlugin::Render(
		TimeValue t,   			// frame to render.
   		Bitmap *tobm, 			// optional target bitmap
		FrameRendParams &frp,	// Time dependent parameters
		HWND hwnd, 				// owner window
		RendProgressCallback *prog,
		ViewParams *viewPar  // override viewPar given on Open.
)
{
	
	//----------------
	//	  INICIO
	//----------------

	if (prog)
		prog->SetTitle("Preparing to render...");

	//----------------
	//  INTERFACE
	//----------------
	

	ip = GetCOREInterface();


	//----------------
	//     ESCENA
	//----------------
	
//	scene=ip->GetRootNode();
	

	//----------------
	//		VISTA
	//----------------

//	if (viewPar) 
//		view = *viewPar;


	//-----------------
	//	 RENDPARAMS
	//-----------------

	rendParams.progCallback = prog;	
	rendParams.devWidth = tobm->Width();
	rendParams.devHeight = tobm->Height();
	rendParams.devAspect = tobm->Aspect();
	rendParams.nRegxmin = frp.regxmin;
	rendParams.nRegymin = frp.regymin;
	rendParams.nRegxmax = frp.regxmax;
	rendParams.nRegymax = frp.regymax;
	rendParams.time = t;
	rendParams.pFrp = &frp;
	if (viewPar) 
		view = *viewPar;
	if (pViewNode)
		GetViewParams(pViewNode, view, t);
	rendParams.ComputeViewParams(view);

	if (rendParams.rendType == RENDTYPE_REGION) {
		if (rendParams.nRegxmin<0) rendParams.nRegxmin = 0;
		if (rendParams.nRegymin<0) rendParams.nRegymin = 0;
		if (rendParams.nRegxmax>rendParams.devWidth)
			rendParams.nRegxmax = rendParams.devWidth;
		if (rendParams.nRegymax>rendParams.devHeight)
			rendParams.nRegymax = rendParams.devHeight;
		rendParams.nMinx = rendParams.nRegxmin;
		rendParams.nMiny = rendParams.nRegymin;
		rendParams.nMaxx = rendParams.nRegxmax;
		rendParams.nMaxy = rendParams.nRegymax;
	}
	else {
		rendParams.nMinx = 0;
		rendParams.nMiny = 0;
		rendParams.nMaxx = rendParams.devWidth;
		rendParams.nMaxy = rendParams.devHeight;
	}

	//TODO
//	rendParams.atmos = frp.atmos;
//	rendParams.envMap = frp.envMap;

	//update materiales ,mallas y luces




	//----------------
	//  GLOBAL INFO
	//----------------


	//envirorment map
/*	Texmap* env = ip->GetEnvironmentMap();
	Control* ambLightCont;
	Control* bgColCont;
	Color bgCol;
	if (env) 
	{
		//envirorment map en env
	}
	else 
	{
		bgColCont = ip->GetBackGroundController();
		if (bgColCont && bgColCont->IsAnimated()) 
		{
			//background color animado en bgColCont
		}
		else 
		{
			bgCol = ip->GetBackGround(0, FOREVER);
			//background color en bgCol
		}
	}
	
	// ambient light
	ambLightCont = ip->GetAmbientController();
	Color ambLight;
	if (ambLightCont && ambLightCont->IsAnimated()) 
	{
		//ambient light animada en ambLightCont
	}
	else 
	{
		ambLight = ip->GetAmbient(0, FOREVER);
		//ambient light en ambLight
	}
*/
		
	//----------------
	//  MATERIALES
	//----------------

//	PreProcess(scene,0,mtlList);
//	int numMtls = mtlList.Count();
	//materiales en mtlList y número de ellos en numMtls

/*	for (int ihy=0;ihy<numMtls;ihy++)
	{
		Mtl *aux1=mtlList.GetMtl(0);
		MtlBase *aux2=aux1->GetActiveTexmap();
		PRTMaterial *aux3;
		aux3=new PRTMaterial();
		
		TSTR tex=aux2->GetName();
		if (tex) aux3->texture=PRTTexture(tex);
		aux3->shininess=aux1->GetShininess();
		//aux3->specular=aux1->GetSpecular();
		aux3->color=PRTVector(aux1->GetDiffuse().r,aux1->GetDiffuse().g,aux1->GetDiffuse().b);
		Materiales.AddAtEnd(aux3);
	}
*/

	//----------------
	//    OBJETOS
	//----------------

	if (trazador.ObjectsList.first!=NULL)
		trazador.ObjectsList.Free();
	if (trazador.LightsList.first!=NULL)
		trazador.LightsList.Free();


	//int numChildren = ip->GetRootNode()->NumberOfChildren();
	int numChildren = scene->NumberOfChildren();
	
	for (int idx=0; idx<numChildren; idx++) {
		//if (ip->GetCancel())
		//	break;
		//nodeEnum(ip->GetRootNode()->GetChildNode(idx), 0);
		nodeEnum(scene->GetChildNode(idx), 0, t);//t¿?
	}


	//----------------
	//  LOS MAPAS DE FOTONES
	//----------------

	if (trazador.BIndirectLight==true)
	{

		if (prog) 
			prog->SetTitle("Creando mapa de fotones...");
			
		((PRTPhotonMapping*)trazador.render)->BuildPhotonIllumination();
	}

	if (trazador.BOctrees==true)
	{
		if (prog) 
			prog->SetTitle("Creando octrees...");
			
		//trazador.octreesdeep=OctreesDeep;
		trazador.BuildOctrees();
	}

	//----------------
	//  RENDERIZAR
	//----------------

	
	//BroadcastNotification(NOTIFY_PRE_RENDERFRAME, (void*)(RenderGlobalContext*)&rendParams);

//	ip->ProgressStart(/*GetString(IDS_PROGRESS_MSG)*/"Renderizando...", TRUE, fn, NULL);


	if (prog) {
		prog->SetTitle("Rendering...");
		prog->SetSceneStats(trazador.LightsList.Lenght(), 0, 0, trazador.ObjectsList.Lenght(), trazador.ObjectsList.Lenght());
	}





	Point3 trans=rendParams.camToWorld.PointTransform(Point3(0,0,0));
	PRTVector camaraposini=PRTVector(trans.x,trans.y,trans.z);
	//PRTVector camarapos2;
	trans=rendParams.camToWorld.PointTransform(Point3(0,0,-1));
	PRTVector camaradirini=PRTVector(trans.x,trans.y,trans.z);
	camaradirini=(camaradirini-camaraposini).Normalize();
	fov=view.fov;
	


	//int nExitStatus = RenderImage(rendParams, t, tobm, prog);

	//trazador.AddPointLight(PRTVector(0,0,0),PRTVector(1,1,1));


	BroadcastNotification(NOTIFY_PRE_RENDERFRAME, (void*)(RenderGlobalContext*)&rendParams);

	ancho=rendParams.nMaxx-rendParams.nMinx;
	alto=rendParams.nMaxy-rendParams.nMiny;

	int nExitStatus=1;
	for (int j=rendParams.nMiny;j<rendParams.nMaxy;j++)
	{
		for (int i=rendParams.nMinx;i<rendParams.nMaxx;i++)
		{
			Point3 pnorm;
			pnorm=rendParams.RayDirection(i,j);
			Point3 pnorm2;
			pnorm2=rendParams.RayPosition(i,j);
	//		camaradir=PRTVector(pnorm.x,pnorm.z,-pnorm.y);

			//if (view.projType==PROJ_PERSPECTIVE) //proyeccion perspectiva
			//	camarapos2=camarapos;
			//else
			//	camarapos2=camarapos+PRTVector(i,j,0);
			
			/*PRTVector camarafin(0,0,-5);
			ancho=320;
			alto=240;
			fov=PRTRadToGrad((PRT_FLOAT)view.fov);
			PRTVector aux;
			PRTVector auxu;
			PRTVector auxv;
			PRTVector cfcp=camarafin-camarapos;
			PRTVector cpcf=camarapos-camarafin;
			PRT_FLOAT dist=(cfcp).Module();
			PRT_FLOAT enj= ( ((PRT_FLOAT)j*(PRT_FLOAT)(2.0/(PRT_FLOAT)ancho))-1.0) *fov*0.017453292520882224401732378141093;
			PRT_FLOAT eni= ( ((PRT_FLOAT)i*(PRT_FLOAT)(2.0/(PRT_FLOAT)alto))-1.0) *fov*0.017453292520882224401732378141093;
			//PRT_FLOAT enz= ( ((PRT_FLOAT)i*(PRT_FLOAT)(2.0/(PRT_FLOAT)alto))-1.0) *fov/57.29577951;
			//aux=PRTVector(camarafin.x-camarapos.x+dist*sin(enj),camarafin.y-camarapos.y-dist*sin(eni),camarafin.z-camarapos.z-(dist*sin(enz))*sin(enz)); //*TODO*
		
			auxu=PRTVector(0,1,0)^(cpcf).Normalizado();
			auxv=(cpcf).Normalizado()^auxu;
			aux=(cfcp)+auxu*enj*(dist*sin(fov/57.29577951))+auxv*-eni*(dist*sin(fov/57.29577951));

			//aux=PRTVector(aux.x,aux.z,-aux.y);

			camaradir=aux.Normalizado();
			//camarapos=PRTVector(i,j,5);
			*/

	/*		PRTVector aux;
			PRTVector auxu;
			PRTVector auxv;
			//PRTVector cfcp=camarafin-camarapos;
			//PRTVector cpcf=camarapos-camarafin;
			//PRT_FLOAT dist=(cfcp).Module();
			PRT_FLOAT enj= ( ((PRT_FLOAT)j*(PRT_FLOAT)(2.0/(PRT_FLOAT)ancho))-1.0) *view.fov*0.017453292520882224401732378141093;
			PRT_FLOAT eni= ( ((PRT_FLOAT)i*(PRT_FLOAT)(2.0/(PRT_FLOAT)alto))-1.0) *view.fov*0.017453292520882224401732378141093;
			//PRT_FLOAT enz= ( ((PRT_FLOAT)i*(PRT_FLOAT)(2.0/(PRT_FLOAT)alto))-1.0) *fov/57.29577951;
			//aux=PRTVector(camarafin.x-camarapos.x+dist*sin(enj),camarafin.y-camarapos.y-dist*sin(eni),camarafin.z-camarapos.z-(dist*sin(enz))*sin(enz)); //*TODO*
			
			auxu=PRTVector(0,1,0)^-camaradir;
			auxv=camaradir^auxu;
			aux=(camaradir)+auxu*enj*(sin(fov/57.29577951))+auxv*-eni*(sin(fov/57.29577951));
*/

			pnorm=rendParams.camToWorld.PointTransform(pnorm);
			pnorm2=rendParams.camToWorld.PointTransform(pnorm2);
			camaradir=PRTVector(pnorm.x,pnorm.y,pnorm.z);
			camarapos=PRTVector(pnorm2.x,pnorm2.y,pnorm2.z);
			camaradir=(camaradir-camaraposini).Normalize();
			

			PRTVector jar=trazador.RayTrace(PRTRay(camarapos,camaradir),1);
			Color color(jar.x,jar.y,jar.z);
			BMM_Color_64 col64;
			col64=colTo64(color);
			tobm->PutPixels(i, j, 1, &col64);

		}

		if (j!=0)
		{
			Rect r;
			r.top = j-1;
			r.bottom = j;
			r.left = 0;
			r.right = tobm->Width();
			tobm->ShowProgressLine(j-1); //linea blanca
			tobm->RefreshWindow(&r);
		}
		if (prog && (prog->Progress(j, rendParams.devHeight-1) == RENDPROG_ABORT)) 
		{
			tobm->ShowProgressLine(-1);	// Clear progress line
			return 0;
			break;
		}
	}
	tobm->ShowProgressLine(-1);
	//prog->SetSceneStats(trazador.LightsList.Lenght(), trazador.numrayos, trazador.numintertest, trazador.ObjectsList.Lenght(), trazador.ObjectsList.Lenght());


// y para region?



	BroadcastNotification(NOTIFY_POST_RENDERFRAME, (void*)(RenderGlobalContext*)&rendParams);





	//BroadcastNotification(NOTIFY_POST_RENDERFRAME, (void*)(RenderGlobalContext*)&rendParams);

	Rect r;
	r.top = 0;
	r.bottom = tobm->Height();
	r.left = 0;
	r.right = tobm->Width();
	tobm->RefreshWindow(&r);

	// Write RendInfo to output bitmap
	/*RenderInfo* ri = tobm->AllocRenderInfo();
	if (ri) {
		ri->projType = rendParams.projType?ProjParallel:ProjPerspective;
		ri->kx = rendParams.xscale;
		ri->ky = rendParams.yscale;
		ri->xc = (float)rendParams.xc;
		ri->yc = (float)rendParams.xc;
		ri->fieldRender = FALSE;	// We don't support field rendering...
		ri->fieldOdd = FALSE;		// We don't support field rendering...
		ri->renderTime[0] = rendParams.time;
		ri->worldToCam[0] = rendParams.worldToCam;
		ri->camToWorld[0] = rendParams.camToWorld;
	}*/

	/*if (nExitStatus && rendParams.effect&&rendParams.effect->Active(t))  {
		MyCallback cb(this);
		rendParams.effect->Apply(t, tobm, &rendParams, &cb);
		tobm->RefreshWindow(NULL);
		if (tobm->GetWindow())
			UpdateWindow(tobm->GetWindow());
		needFinalRefresh = FALSE;
		}*/

	if (prog) {
		prog->SetTitle("Done.");
		prog->SetSceneStats(trazador.LightsList.Lenght(), trazador.numrayos, trazador.numintertest, trazador.ObjectsList.Lenght(), trazador.ObjectsList.Lenght());
	}

	return nExitStatus;
}	