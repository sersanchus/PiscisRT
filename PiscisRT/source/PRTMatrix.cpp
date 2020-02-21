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

#include "PRTMatrix.h"

PRTMatrix::PRTMatrix(void)
{
	col[0]=PRTVector4(1,0,0,0);
	col[1]=PRTVector4(0,1,0,0);
	col[2]=PRTVector4(0,0,1,0);
	col[3]=PRTVector4(0,0,0,1);
}

PRTVector4 PRTMatrix::operator*(PRTVector4 /*&*/v)
{ 
	return PRTVector4(
	col[0].x*v.x + col[1].x*v.y + col[2].x*v.z + col[3].x*v.w,
	col[0].y*v.x + col[1].y*v.y + col[2].y*v.z + col[3].y*v.w,
	col[0].z*v.x + col[1].z*v.y + col[2].z*v.z + col[3].z*v.w,
	col[0].w*v.x + col[1].w*v.y + col[2].w*v.z + col[3].w*v.w
	);
}

PRTMatrix PRTMatrix::operator*(PRTFloat /*&*/s)
{
	PRTMatrix res;		
	res.col[0]=col[0]*s;
	res.col[1]=col[1]*s;
	res.col[2]=col[2]*s;
	res.col[3]=col[3]*s;
	return res;
}

PRTMatrix PRTMatrix::operator*(PRTMatrix /*&*/tm)
{
	PRTMatrix res;		
	for (unsigned int c=0; c<4; c++)
		for (unsigned int f=0; f<4; f++)
		{
			PRTFloat ff=0.0;
			ff+=col[0][f]*tm[c][0];
			ff+=col[1][f]*tm[c][1];
			ff+=col[2][f]*tm[c][2];
			ff+=col[3][f]*tm[c][3];
			res[c][f]=ff;
		}
	return res;
}

PRTVector4 &PRTMatrix::operator[](unsigned int column)
{
	return ((PRTVector4&)(col[column]));
}

PRTMatrix PRTMatrix::operator=(PRTMatrix /*&*/tm)
{
	col[0]=tm[0];
	col[1]=tm[1];
	col[2]=tm[2];
	col[3]=tm[3];
	return (*this);
}

bool PRTMatrix::operator!=(PRTMatrix /*&*/tm)
{
	if (col[0]!=tm[0]) return false;
	if (col[1]!=tm[1]) return false;
	if (col[2]!=tm[2]) return false;
	if (col[3]!=tm[3]) return false;
	return true;
}

void PRTMatrix::LoadIdentity(void)
{
	col[0][0]=1.0; col[1][0]=0.0; col[2][0]=0.0; col[3][0]=0.0;
	col[0][1]=0.0; col[1][1]=1.0; col[2][1]=0.0; col[3][1]=0.0;
	col[0][2]=0.0; col[1][2]=0.0; col[2][2]=1.0; col[3][2]=0.0;
	col[0][3]=0.0; col[1][3]=0.0; col[2][3]=0.0; col[3][3]=1.0;
}

void PRTMatrix::LoadRotationXMatrix(PRTFloat angle)
{
	PRTFloat cosa=PRTCos(PRTGradToRad(angle));
	PRTFloat sina=PRTSin(PRTGradToRad(angle));
	col[0][0]=1.0; col[1][0]=0.0; col[2][0]=0.0; col[3][0]=0.0;
	col[0][1]=0.0; col[1][1]=cosa; col[2][1]=-sina;col[3][1]=0.0;
	col[0][2]=0.0; col[1][2]=sina; col[2][2]=cosa; col[3][2]=0.0;
	col[0][3]=0.0; col[1][3]=0.0; col[2][3]=0.0; col[3][3]=1.0;
}

void PRTMatrix::LoadRotationYMatrix(PRTFloat angle)
{
	PRTFloat cosa=PRTCos(PRTGradToRad(angle));
	PRTFloat sina=PRTSin(PRTGradToRad(angle));
	col[0][0]=cosa; col[1][0]=0.0; col[2][0]=sina; col[3][0]=0.0;
	col[0][1]=0.0; col[1][1]=1.0; col[2][1]=0.0; col[3][1]=0.0;
	col[0][2]=-sina;col[1][2]=0.0; col[2][2]=cosa; col[3][2]=0.0;
	col[0][3]=0.0; col[1][3]=0.0; col[2][3]=0.0; col[3][3]=1.0;
}

void PRTMatrix::LoadRotationZMatrix(PRTFloat angle)
{
	PRTFloat cosa=PRTCos(PRTGradToRad(angle));
	PRTFloat sina=PRTSin(PRTGradToRad(angle));
	col[0][0]=cosa; col[1][0]=-sina;col[2][0]=0.0; col[3][0]=0.0;
	col[0][1]=sina; col[1][1]=cosa; col[2][1]=0.0; col[3][1]=0.0;
	col[0][2]=0.0; col[1][2]=0.0; col[2][2]=1.0; col[3][2]=0.0;
	col[0][3]=0.0; col[1][3]=0.0; col[2][3]=0.0; col[3][3]=1.0;
}

void PRTMatrix::RotateX(PRTFloat angle, int mode)
{
	PRTMatrix tr;
	tr.LoadRotationXMatrix(angle); // *TODO*!!!!! -angle?!?!
	if (mode==PRTMATRIX_OBJECT_SPACE)
		operator=( (*this)*tr );
	else
		operator=( tr*(*this) );
}

void PRTMatrix::RotateY(PRTFloat angle, int mode)
{
	PRTMatrix tr;
	tr.LoadRotationYMatrix(angle);
	if (mode==PRTMATRIX_OBJECT_SPACE)
		operator=( (*this)*tr );
	else
		operator=( tr*(*this) );
}

void PRTMatrix::RotateZ(PRTFloat angle, int mode)
{
	PRTMatrix tr;
	tr.LoadRotationZMatrix(angle);
	if (mode==PRTMATRIX_OBJECT_SPACE)
		operator=( (*this)*tr );
	else
		operator=( tr*(*this) );
}

void PRTMatrix::Translate(PRTVector /*&*/tv, int mode)
{
	PRTMatrix tr;
	tr.col[3]=tv;
	if (mode==PRTMATRIX_OBJECT_SPACE)
		operator=( (*this)*tr );
	else
		operator=( tr*(*this) );
}

PRTFloat Determinant2x2(PRTFloat a,PRTFloat b,PRTFloat c,PRTFloat d)
{
	return a*d-b*c;
}

PRTFloat Determinant3x3(PRTFloat a1,PRTFloat a2,PRTFloat a3,PRTFloat b1,PRTFloat b2,PRTFloat b3,PRTFloat c1,PRTFloat c2,PRTFloat c3)
{
	return a1*Determinant2x2(b2,b3,c2,c3)-b1*Determinant2x2(a2,a3,c2,c3)+c1*Determinant2x2(a2,a3,b2,b3);
}

PRTFloat PRTMatrix::Determinant()
{
    PRTFloat ans = 
		col[0].x * Determinant3x3( col[1].y, col[1].z, col[1].w, col[2].y, col[2].z, col[2].w, col[3].y, col[3].z, col[3].w)
        - col[1].x * Determinant3x3( col[0].y, col[0].z, col[0].w, col[2].y, col[2].z, col[2].w, col[3].y, col[3].z, col[3].w)
        + col[2].x * Determinant3x3( col[0].y, col[0].z, col[0].w, col[1].y, col[1].z, col[1].w, col[3].y, col[3].z, col[3].w)
        - col[3].x * Determinant3x3( col[0].y, col[0].z, col[0].w, col[1].y, col[1].z, col[1].w, col[2].y, col[2].z, col[2].w);
    return ans;
}

PRTMatrix PRTMatrix::Adjoint()
{
	PRTMatrix ret;
    ret.col[0].x=   Determinant3x3( col[1].y, col[1].z, col[1].w, col[2].y, col[2].z, col[2].w, col[3].y, col[3].z, col[3].w);
    ret.col[0].y= - Determinant3x3( col[0].y, col[0].z, col[0].w, col[2].y, col[2].z, col[2].w, col[3].y, col[3].z, col[3].w);
    ret.col[0].z=   Determinant3x3( col[0].y, col[0].z, col[0].w, col[1].y, col[1].z, col[1].w, col[3].y, col[3].z, col[3].w);
    ret.col[0].w= - Determinant3x3( col[0].y, col[0].z, col[0].w, col[1].y, col[1].z, col[1].w, col[2].y, col[2].z, col[2].w);
    ret.col[1].x= - Determinant3x3( col[1].x, col[1].z, col[1].w, col[2].x, col[2].z, col[2].w, col[3].x, col[3].z, col[3].w);
    ret.col[1].y=   Determinant3x3( col[0].x, col[0].z, col[0].w, col[2].x, col[2].z, col[2].w, col[3].x, col[3].z, col[3].w);
    ret.col[1].z= - Determinant3x3( col[0].x, col[0].z, col[0].w, col[1].x, col[1].z, col[1].w, col[3].x, col[3].z, col[3].w);
    ret.col[1].w=   Determinant3x3( col[0].x, col[0].z, col[0].w, col[1].x, col[1].z, col[1].w, col[2].x, col[2].z, col[2].w);
    ret.col[2].x=   Determinant3x3( col[1].x, col[1].y, col[1].w, col[2].x, col[2].y, col[2].w, col[3].x, col[3].y, col[3].w);
    ret.col[2].y= - Determinant3x3( col[0].x, col[0].y, col[0].w, col[2].x, col[2].y, col[2].w, col[3].x, col[3].y, col[3].w);
    ret.col[2].z=   Determinant3x3( col[0].x, col[0].y, col[0].w, col[1].x, col[1].y, col[1].w, col[3].x, col[3].y, col[3].w);
    ret.col[2].w= - Determinant3x3( col[0].x, col[0].y, col[0].w, col[1].x, col[1].y, col[1].w, col[2].x, col[2].y, col[2].w);
    ret.col[3].x= - Determinant3x3( col[1].x, col[1].y, col[1].z, col[2].x, col[2].y, col[2].z, col[3].x, col[3].y, col[3].z);
    ret.col[3].y=   Determinant3x3( col[0].x, col[0].y, col[0].z, col[2].x, col[2].y, col[2].z, col[3].x, col[3].y, col[3].z);
    ret.col[3].z= - Determinant3x3( col[0].x, col[0].y, col[0].z, col[1].x, col[1].y, col[1].z, col[3].x, col[3].y, col[3].z);
    ret.col[3].w=   Determinant3x3( col[0].x, col[0].y, col[0].z, col[1].x, col[1].y, col[1].z, col[2].x, col[2].y, col[2].z);
	return ret;
}

PRTMatrix PRTMatrix::Invert()
{
	PRTFloat det=Determinant();
	PRTMatrix ret=Adjoint();
	return ret*(1/det);
}
