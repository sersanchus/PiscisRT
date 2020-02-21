#include "PRTMaxPlugin.h"
#include "resource.h"
#include "maxincl.h"

class PRTMaxPluginDlg : public RendParamDlg 
{
public:

	PRTMaxPlugin *rend;
	IRendParams *ir;
	HWND hPanel;
	BOOL prog;
	HFONT hFont;

	ISpinnerControl* OctreesDeepSpinner;
	ISpinnerControl* NumFotonesSpinner;
	ISpinnerControl* ScalePhotonSpinner;
	ISpinnerControl* GroupPhotonesSpinner;

	//TSTR workFileName;
	PRTMaxPluginDlg(PRTMaxPlugin *r,IRendParams *i,BOOL prog);
	~PRTMaxPluginDlg();
	
	//VIRTUAL
	void AcceptParams();
	void RejectParams();

	void DeleteThis() {delete this;}
	
	void InitParamDialog(HWND hWnd);
	void InitProgDialog(HWND hWnd);
//	void ReleaseControls() {delete this;}
//	BOOL FileBrowse();
	BOOL WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
};
