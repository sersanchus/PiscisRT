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

#include "PRTQuaternion.h"
#include "PRTMatrix.h"

PRTQuaternion PRTQuaternion::operator*(const PRTQuaternion &v)
{
	return PRTQuaternion(x*v.x-y*v.y-z*v.z-w*v.w,
						 x*v.y+y*v.x+z*v.w-w*v.z,
						 x*v.z+z*v.x+w*v.y-y*v.w,
						 x*v.w+w*v.x+y*v.z-z*v.y);
}

PRTQuaternion PRTQuaternion::operator/(const PRTQuaternion &v)
{
	PRTQuaternion aux=v;
	return (*this)=(*this)*(aux.Invert());
}

PRTFloat PRTQuaternion::Norm(void)
{
	return x*x+y*y+z*z+w*w;
}

PRTFloat PRTQuaternion::Module(void)
{
	return PRTSqrt(x*x+y*y+z*z+w*w);
}

PRTQuaternion PRTQuaternion::Negate(void)
{
	return PRTQuaternion(-x,-y,-z,-w);
}

PRTQuaternion PRTQuaternion::Conjugate(void)
{
	return PRTQuaternion(x,-y,-z,-w);
}

PRTQuaternion PRTQuaternion::Sqr(void)
{	
	PRTFloat aux=PRTFloat(2)*x;
	return PRTQuaternion(x*x-(y*y+z*z+w*w),aux*y,aux*z,aux*w); 
}

PRTQuaternion PRTQuaternion::Invert(void)
{
	PRTFloat aux=Norm();
	return PRTQuaternion(x/aux,y/-aux,z/-aux,w/-aux);
}

PRTQuaternion PRTQuaternion::Sqrt(void)
{	
	PRTQuaternion q;
	PRTFloat rho,irho,srho,m,im,a,b;
	rho=PRTSqrt(x*x+y*y+z*z+w*w);
	irho=(rho==0.0)?PRTFloat(1000000.0):PRTFloat(1.0)/rho;
	x*=irho;
	y*=irho;
	z*=irho;
	w*=irho;
	m=PRTSqrt(y*y+z*z+w*w);
	im=(m==0.0)?PRTFloat(1000000.0):PRTFloat(1.0)/m;
	a=PRTSqrt(PRTFloat(0.5)+PRTFloat(0.5)*x);
	b=PRTSqrt(PRTFloat(0.5)-PRTFloat(0.5)*x);
	srho=PRTSqrt(rho);
	x=srho*a;
	y*=srho*b*im;
	z*=srho*b*im;
	w*=srho*b*im;
	return(q); 
}