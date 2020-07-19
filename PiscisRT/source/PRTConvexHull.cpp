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

#include "PRTConvexHull.h"
#include "PRTUtil.h"

bool PRTConvexHull::IntersectWithRay(const PRTRay& r)
{
	//RAY'S CONVEX HULL WITH OBJECT'S CONVEX HULL ----------------------------------------------

	/*PRTVector rini=r.orig;
	PRTVector rfin=r.orig+(r.dir*PRTINFINITE);
	PRTFloat miraquebien;
	if (rini.x>rfin.x)
	{
		miraquebien=rfin.x;
		rfin.x=rini.x;
		rini.x=miraquebien;
	}
	if (rini.y>rfin.y)
	{
		miraquebien=rfin.y;
		rfin.y=rini.y;
		rini.y=miraquebien;
	}
	if (rini.z>rfin.z)
	{
		miraquebien=rfin.z;
		rfin.z=rini.z;
		rini.z=miraquebien;
	}
	if (rini.x>chend.x || rfin.x<chbeg.x || rini.y>chend.y || rfin.y<chbeg.y || rini.z>chend.z || rfin.z<chbeg.z)
		return false;

	// RAY WITH OBJECT'S CONVEX HULL -------------------------------------------------------------

	PRTFloat tnear=-PRTINFINITE;
	PRTFloat tfar=PRTINFINITE;
	PRTFloat penemore;
	PRTFloat t1;
	PRTFloat t2;

	if (r.dir.x==0){if (r.orig.x<chbeg.x || r.orig.x>chend.x) return false;}
	else 
	{
		t1=(chbeg.x-r.orig.x)/r.dir.x;
		t2=(chend.x-r.orig.x)/r.dir.x;
		if (t1>t2)
		{
			penemore=t1;
			t1=t2;
			t2=penemore;
		}
		if (t1>tnear) tnear=t1;
		if (t2<tfar) tfar=t2;
		if (tnear>tfar)	return false;
		if (tfar<0)	return false;
	}
	if (r.dir.y==0){if (r.orig.y<chbeg.y || r.orig.y>chend.y) return false;}
	else
	{
		t1=(chbeg.y-r.orig.y)/r.dir.y;
		t2=(chend.y-r.orig.y)/r.dir.y;
		if (t1>t2)
		{
			penemore=t1;
			t1=t2;
			t2=penemore;
		}
		if (t1>tnear) tnear=t1;
		if (t2<tfar) tfar=t2;
		if (tnear>tfar)	return false;
		if (tfar<0)	return false;
	}
	if (r.dir.z==0){if (r.orig.z<chbeg.z || r.orig.z>chend.z) return false;}
	else
	{
		t1=(chbeg.z-r.orig.z)/r.dir.z;
		t2=(chend.z-r.orig.z)/r.dir.z;
		if (t1>t2)
		{
			penemore=t1;
			t1=t2;
			t2=penemore;
		}
		if (t1>tnear) tnear=t1;
		if (t2<tfar) tfar=t2;
		if (tnear>tfar)	return false;
		if (tfar<0)	return false;
	}*/


	//if (fabs(chbeg.x-chend.x)<0.01) chbeg.x-=10;//*TODO*
	//if (fabs(chbeg.y-chend.y)<0.01) chbeg.y-=10;//*TODO*
	//if (fabs(chbeg.z-chend.z)<0.01) chbeg.z-=10;//*TODO*
	//SolvePrecision();

	//ENHANCED


	vaux1=r.orig;
	vaux2=r.orig+(r.dir*PRTINFINITE);
	if (vaux1.x>vaux2.x)
	{
		faux1=vaux2.x;
		vaux2.x=vaux1.x;
		vaux1.x=faux1;
	}
	if (vaux1.y>vaux2.y)
	{
		faux1=vaux2.y;
		vaux2.y=vaux1.y;
		vaux1.y=faux1;
	}
	if (vaux1.z>vaux2.z)
	{
		faux1=vaux2.z;
		vaux2.z=vaux1.z;
		vaux1.z=faux1;
	}
	if (vaux1.x>chend.x || vaux2.x<chbeg.x || vaux1.y>chend.y || vaux2.y<chbeg.y || vaux1.z>chend.z || vaux2.z<chbeg.z)
		return false;

	// RAY WITH OBJECT'S CONVEX HULL -------------------------------------------------------------

	faux1=-PRTINFINITE;
	faux2=PRTINFINITE;

	if (r.dir.x==0){if (r.orig.x<chbeg.x || r.orig.x>chend.x) return false;}
	else 
	{
		faux4=(chbeg.x-r.orig.x)/r.dir.x;
		faux5=(chend.x-r.orig.x)/r.dir.x;
		if (faux4>faux5)
		{
			faux3=faux4;
			faux4=faux5;
			faux5=faux3;
		}
		if (faux4>faux1) faux1=faux4;
		if (faux5<faux2) faux2=faux5;
		if (faux1>faux2)	return false;
		if (faux2<0)	return false;
	}
	if (r.dir.y==0){if (r.orig.y<chbeg.y || r.orig.y>chend.y) return false;}
	else
	{
		faux4=(chbeg.y-r.orig.y)/r.dir.y;
		faux5=(chend.y-r.orig.y)/r.dir.y;
		if (faux4>faux5)
		{
			faux3=faux4;
			faux4=faux5;
			faux5=faux3;
		}
		if (faux4>faux1) faux1=faux4;
		if (faux5<faux2) faux2=faux5;
		if (faux1>faux2)	return false;
		if (faux2<0)	return false;
	}
	if (r.dir.z==0){if (r.orig.z<chbeg.z || r.orig.z>chend.z) return false;}
	else
	{
		faux4=(chbeg.z-r.orig.z)/r.dir.z;
		faux5=(chend.z-r.orig.z)/r.dir.z;
		if (faux4>faux5)
		{
			faux3=faux4;
			faux4=faux5;
			faux5=faux3;
		}
		if (faux4>faux1) faux1=faux4;
		if (faux5<faux2) faux2=faux5;
		if (faux1>faux2)	return false;
		if (faux2<0)	return false;
	}

	return true;
}

void PRTConvexHull::SolvePrecision(void)
{
	if (fabs(chbeg.x-chend.x)<PRTFloat(0.001))
		chbeg.x-=PRECISION;
	if (fabs(chbeg.y-chend.y)<PRTFloat(0.001))
		chbeg.y-=PRECISION;
	if (fabs(chbeg.z-chend.z)<PRTFloat(0.001))
		chbeg.z-=PRECISION;
	if (chrad<PRTFloat(0.000001))
		chrad+=PRECISION;
}
