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

#ifndef __PRTMATRIX_H__
#define __PRTMATRIX_H__

#include "PRTUtil.h"
#include "PRTVector.h"

#define PRTMATRIX_WORLD_SPACE	0
#define PRTMATRIX_OBJECT_SPACE	1

//--------------------------------------------------------------------------------------
//! The PRTMatrix class.
/*!	This class represents a 4x4 homogeneus matrix.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTMatrix
{

public:

	PRTVector4 col[4];			//!< The array of columns of the matrix.
	
//--------------------------------------------------------------------------------------

	//!	The constructor of an identity matrix.
	/*!	
	*/
	PRTMatrix(void);

//--------------------------------------------------------------------------------------

	//!	The multiply operator between a matrix and a vector.
	/*!	
		\param vector the right vector
		\return PRTVector
	*/
	PRTVector4 operator*(PRTVector4 /*&*/vector);

//--------------------------------------------------------------------------------------

	//!	The multiply operator between a matrix and a scalar.
	/*!	
		\param scalar the scalar
		\return PRTVector
	*/
	PRTMatrix operator*(PRTFloat /*&*/scalar);

//--------------------------------------------------------------------------------------

	//!	The multiply operator between a matrix and another one.
	/*!	
		\param matrix the right vector
		\return PRTMatrix
	*/
	PRTMatrix operator*(PRTMatrix /*&*/matrix);
		

//--------------------------------------------------------------------------------------

	//!	A function to return a column in the matrix.
	/*!	
		\param column
		\return PRTVector
	*/
	PRTVector4 &operator[](unsigned int column);

//--------------------------------------------------------------------------------------

	//!	The assignation operator between matrix.
	/*!	
		\param matrix
		\return PRTMatrix
	*/
	PRTMatrix operator=(PRTMatrix /*&*/matrix);

//--------------------------------------------------------------------------------------

	//!	The comparison operator between matrix, "different than".
	/*!	
		\param matrix
		\return PRTMatrix
	*/
	bool operator!=(PRTMatrix /*&*/matrix);

//--------------------------------------------------------------------------------------

	//!	Makes the matrix an identity matrix.
	/*!	
	*/
	void LoadIdentity(void);

//--------------------------------------------------------------------------------------

	//!	Sets the matrix to a specified rotation matrix in x.
	/*!	
		\param angle angle in x axis
	*/
	void LoadRotationXMatrix(PRTFloat angle);

//--------------------------------------------------------------------------------------

	//!	Sets the matrix to a specified rotation matrix in y.
	/*!	
		\param angle angle in y axis
	*/
	void LoadRotationYMatrix(PRTFloat angle);

//--------------------------------------------------------------------------------------

	//!	Sets the matrix to a specified rotation matrix in z.
	/*!	
		\param angle angle in z axis
	*/
	void LoadRotationZMatrix(PRTFloat angle);

//--------------------------------------------------------------------------------------

	//!	Rotate the matrix around x axis.
	/*!	
		\param angle angle in x axis
		\param mode in object space or in world space
	*/
	void RotateX(PRTFloat angle, int mode=PRTMATRIX_OBJECT_SPACE);

//--------------------------------------------------------------------------------------

	//!	Rotate the matrix around y axis.
	/*!	
		\param angle angle in y axis
		\param mode in object space or in world space
	*/
	void RotateY(PRTFloat angle, int mode=PRTMATRIX_OBJECT_SPACE);

//--------------------------------------------------------------------------------------

	//!	Rotate the matrix around z axis.
	/*!	
		\param angle angle in z axis
		\param mode in object space or in world space
	*/
	void RotateZ(PRTFloat angle, int mode=PRTMATRIX_OBJECT_SPACE);

//--------------------------------------------------------------------------------------

	//!	Translate the matrix.
	/*!	
		\param vector displacement vector
		\param mode in object space or in world space
	*/
	void Translate(PRTVector /*&*/vector, int mode=PRTMATRIX_OBJECT_SPACE);

//--------------------------------------------------------------------------------------

	//!	Calculate the determinant of the matrix.
	/*!	
	*/
	PRTFloat Determinant();

//--------------------------------------------------------------------------------------

	//!	Calculate the adjoint the matrix.
	/*!	
	*/
	PRTMatrix Adjoint();

//--------------------------------------------------------------------------------------

	//!	Invert the matrix.
	/*!	
	*/
	PRTMatrix Invert();

//--------------------------------------------------------------------------------------

};

#endif
