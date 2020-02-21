#include "PRTMaxPluginDlg.h"
#include "PRTMaxPlugin.h"
#include "maxincl.h"

RendParamDlg *PRTMaxPlugin::CreateParamDialog(IRendParams *ir,BOOL prog)
{
	//IAutoMParamDlg* masterDlg = PRTMaxPluginDesc.CreateParamDlgs(hwMtlEdit, imp, this);

	RendParamDlg* jar=new PRTMaxPluginDlg(this,ir,prog);
	//if (!prog) 
	//	dlg=((PRTMaxPluginDlg*)(jar))->hPanel;
	/*if (dlg==NULL)
	{
		RendParamDlg* jar2=new PRTMaxPluginDlg(this,ir,0);
		dlg=((PRTMaxPluginDlg*)(jar2))->hPanel;
	}*/

	return jar;
	//return NULL;
};

void PRTMaxPluginDlg::InitProgDialog(HWND hWnd) //cuando se renderiza, estadísticas
{
	//SetDlgItemText(hWnd,IDC_RENDVUE_FILENAME, rend->vueFileName.data());
}

void PRTMaxPluginDlg::InitParamDialog(HWND hWnd) //cuando se inicia
{
	//workFileName = rend->vueFileName;
	//SetDlgItemText(hWnd,IDC_RENDVUE_FILENAME, workFileName);

	CheckDlgButton(hWnd,IDC_BTextures,rend->BTextures);
	CheckDlgButton(hWnd,IDC_BReflections,rend->BReflections);
	CheckDlgButton(hWnd,IDC_BRefractions,rend->BAlpha);
	CheckDlgButton(hWnd,IDC_BAlpha,rend->BAlpha);
	CheckDlgButton(hWnd,IDC_BSpecular,rend->BSpecular);
	CheckDlgButton(hWnd,IDC_BShadows,rend->BShadows);
	CheckDlgButton(hWnd,IDC_BDoubleSided,rend->BDoubleSided);
	CheckDlgButton(hWnd,IDC_BNInter,rend->BNInter);
	CheckDlgButton(hWnd,IDC_BCInter,rend->BCInter);
	CheckDlgButton(hWnd,IDC_BOctrees,rend->BOctrees);
	CheckDlgButton(hWnd,IDC_BConvexHulls,rend->BConvexHulls);
	
	OctreesDeepSpinner = GetISpinner(GetDlgItem(hWnd, IDC_OctreesDeepSpinner)); 
	OctreesDeepSpinner->LinkToEdit(GetDlgItem(hWnd,IDC_OctreesDeep), EDITTYPE_INT ); 
	OctreesDeepSpinner->SetLimits(0, 100, TRUE); 
	OctreesDeepSpinner->SetValue(rend->OctreesDeep ,FALSE);
	OctreesDeepSpinner->SetScale(1); 
	
	CheckDlgButton(hWnd,IDC_BIndirectLight,rend->BIndirectLight);
	CheckDlgButton(hWnd,IDC_BGiReflections,rend->BGiReflections);
	CheckDlgButton(hWnd,IDC_BGiRefractions,rend->BGiRefractions);
	CheckDlgButton(hWnd,IDC_BGiAlpha,rend->BGiAlpha);
	CheckDlgButton(hWnd,IDC_BGlossy,rend->BGlossy);
	CheckDlgButton(hWnd,IDC_BMejoraPorDistancia,rend->BMejoraPorDistancia);
	
	NumFotonesSpinner = GetISpinner(GetDlgItem(hWnd, IDC_NumFotonesSpinner)); 
	NumFotonesSpinner->LinkToEdit(GetDlgItem(hWnd,IDC_NumFotones), EDITTYPE_INT ); 
	NumFotonesSpinner->SetLimits(0, 100000, TRUE); 
	NumFotonesSpinner->SetValue(rend->NumPhotones ,FALSE);
	NumFotonesSpinner->SetScale(100); 

	ScalePhotonSpinner = GetISpinner(GetDlgItem(hWnd, IDC_ScalePhotonSpinner)); 
	ScalePhotonSpinner->LinkToEdit(GetDlgItem(hWnd,IDC_ScalePhoton), EDITTYPE_FLOAT ); 
	ScalePhotonSpinner->SetLimits(0, 100, TRUE); 
	ScalePhotonSpinner->SetValue(rend->ScalePhoton ,FALSE);
	ScalePhotonSpinner->SetScale(0.0001f); 
	
	GroupPhotonesSpinner = GetISpinner(GetDlgItem(hWnd, IDC_GroupPhotonesSpinner)); 
	GroupPhotonesSpinner->LinkToEdit(GetDlgItem(hWnd,IDC_GroupPhotones), EDITTYPE_INT ); 
	GroupPhotonesSpinner->SetLimits(0, 10000, TRUE); 
	GroupPhotonesSpinner->SetValue(rend->GroupPhotones ,FALSE);
	GroupPhotonesSpinner->SetScale(10); 

}

//***************************************************************************
// Accept parameters.
// This is called if the user clicks "Ok" or "Close"
//***************************************************************************

void PRTMaxPluginDlg::AcceptParams()
{
	/*rend->rendParams.nMaxDepth = depthSpinner->GetIVal();

	rend->rendParams.nAntiAliasLevel = IsDlgButtonChecked(hPanel, IDC_AA_NONE) ? AA_NONE :
		IsDlgButtonChecked(hPanel, IDC_AA_MEDIUM) ? AA_MEDIUM : 
		IsDlgButtonChecked(hPanel, IDC_AA_HIGH) ? AA_MEDIUM : 0;
	rend->rendParams.bReflectEnv = IsDlgButtonChecked(hPanel, IDC_REFLENV);*/

	rend->BTextures=IsDlgButtonChecked(hPanel,IDC_BTextures)==1?1:0;
	rend->BReflections=IsDlgButtonChecked(hPanel,IDC_BReflections)==1?1:0;
	rend->BRefractions=IsDlgButtonChecked(hPanel,IDC_BRefractions)==1?1:0;
	rend->BAlpha=IsDlgButtonChecked(hPanel,IDC_BAlpha)==1?1:0;
	rend->BSpecular=IsDlgButtonChecked(hPanel,IDC_BSpecular)==1?1:0;
	rend->BShadows=IsDlgButtonChecked(hPanel,IDC_BShadows)==1?1:0;
	rend->BDoubleSided=IsDlgButtonChecked(hPanel,IDC_BDoubleSided)==1?1:0;
	rend->BNInter=IsDlgButtonChecked(hPanel,IDC_BNInter)==1?1:0;
	rend->BCInter=IsDlgButtonChecked(hPanel,IDC_BCInter)==1?1:0;
	rend->BOctrees=IsDlgButtonChecked(hPanel,IDC_BOctrees)==1?1:0;
	rend->BConvexHulls=(hPanel,IDC_BConvexHulls)==1?1:0;
	
	int tbufl=(GetISpinner(GetDlgItem(hPanel, IDC_OctreesDeepSpinner)))->GetIVal();
	rend->OctreesDeep=tbufl;
	
	rend->BIndirectLight=IsDlgButtonChecked(hPanel,IDC_BIndirectLight)==1?1:0;
	rend->BGiReflections=IsDlgButtonChecked(hPanel,IDC_BGiReflections)==1?1:0;
	rend->BGiRefractions=IsDlgButtonChecked(hPanel,IDC_BGiRefractions)==1?1:0;
	rend->BGiAlpha=IsDlgButtonChecked(hPanel,IDC_BGiAlpha)==1?1:0;
	rend->BGlossy=IsDlgButtonChecked(hPanel,IDC_BGlossy)==1?1:0;
	rend->BMejoraPorDistancia=IsDlgButtonChecked(hPanel,IDC_BMejoraPorDistancia)==1?1:0;

	tbufl=(GetISpinner(GetDlgItem(hPanel, IDC_NumFotonesSpinner)))->GetIVal();
	rend->NumPhotones=tbufl;

	float tbuf2=(GetISpinner(GetDlgItem(hPanel, IDC_ScalePhotonSpinner)))->GetFVal();
	rend->ScalePhoton=tbuf2;

	tbufl=(GetISpinner(GetDlgItem(hPanel, IDC_GroupPhotonesSpinner)))->GetIVal();
	rend->GroupPhotones=tbufl;
}


//***************************************************************************
// Called if the user cancels the render param dialog.
// Reset any options you have changed here.
// Since we don't update the parameters until AcceptParams() is called,
// we don't need to do anything here.
//***************************************************************************

void PRTMaxPluginDlg::RejectParams()
{
}


BOOL PRTMaxPluginDlg::WndProc(
		HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			if (prog) InitProgDialog(hWnd); //si esta renderizando
			else InitParamDialog(hWnd);
			break;
		
		/*case WM_DESTROY:
			if (!prog) ReleaseControls();
			break;*/
		default:
			return FALSE;
		}
	return TRUE;
}

static INT_PTR CALLBACK PRTMaxPluginDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) //PQ???
{
	PRTMaxPluginDlg *dlg = (PRTMaxPluginDlg*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	switch (msg) {
		case WM_INITDIALOG:
			dlg = (PRTMaxPluginDlg*)lParam;
			SetWindowLongPtr(hWnd,GWLP_USERDATA,lParam);
			/*if (dlg) {
				if (dlg->prog)
					dlg->InitProgDialog(hWnd);
				else
					dlg->InitParamDialog(hWnd);
			}*/
			break;
		case WM_DESTROY:
			if (!dlg->prog) {
				ReleaseISpinner(dlg->NumFotonesSpinner);
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
			dlg->ir->RollupMouseMessage(hWnd,msg,wParam,lParam);
			break;
		//case WM_COMMAND:
			/*case IDC_RENDVUE_FILE:
				if (dlg->FileBrowse()) {
					SetDlgItemText(hWnd,IDC_RENDVUE_FILENAME, dlg->workFileName.data());
					}*/
		//		break;
		}	
	if (dlg) return dlg->WndProc(hWnd,msg,wParam,lParam);
	else return FALSE;
}


PRTMaxPluginDlg::PRTMaxPluginDlg(PRTMaxPlugin *r,IRendParams *i,BOOL prog)
{
	hFont      = hFont = CreateFont(14,0,0,0,FW_BOLD,0,0,0,0,0,0,0, VARIABLE_PITCH | FF_SWISS, _T(""));
	rend       = r;
	ir         = i;
	this->prog = prog;
	if (prog) {		
		hPanel = ir->AddRollupPage(
			hInstance, 
			MAKEINTRESOURCE(IDD_PANEL_PROG),
			PRTMaxPluginDlgProc,
			GetString(IDS_PISCISRT),
			(LPARAM)this);
	} else {
		hPanel = ir->AddRollupPage(
			hInstance, 
			MAKEINTRESOURCE(IDD_PANEL),
			PRTMaxPluginDlgProc,
			GetString(IDS_PISCISRT),
			(LPARAM)this);
	}
	//dlg=hPanel;
}

PRTMaxPluginDlg::~PRTMaxPluginDlg()
{
	DeleteObject(hFont);
	ir->DeleteRollupPage(hPanel);
}
