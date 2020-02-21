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

#ifndef __PRTVECTOR_H__
#define __PRTVECTOR_H__

#include "PRTMath.h"

//--------------------------------------------------------------------------------------
//! The PRTVector class.
/*!	This class represents a vector in 3d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTVector
{

public:

	PRTFloat x,y,z;		//!< The x,y,z coordinates.

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTVector.
	/*!	
		\param vx the x coordinate
		\param vy the y coordinate
		\param vz the z coordinate
	*/
	inline PRTVector(const PRTFloat vx=1.0,const PRTFloat vy=1.0,const PRTFloat vz=1.0){ x=vx; y=vy; z=vz; }	

//--------------------------------------------------------------------------------------

	//!	The PRTVector+PRTVector operator.
	/*!	
		\param v the PRTVector to add
		\return the PRTVector solution
	*/
	inline PRTVector operator+(const PRTVector &v){ return PRTVector(x+v.x,y+v.y,z+v.z); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector-PRTVector operator.
	/*!	
		\param v the PRTVector to substract
		\return the PRTVector solution
	*/
	inline PRTVector operator-(const PRTVector &v){ return PRTVector(x-v.x,y-v.y,z-v.z); }

//--------------------------------------------------------------------------------------

	//!	The negate operator.
	/*!	
		\return the -PRTVector
	*/	
	inline PRTVector operator-(void){ return PRTVector(-x,-y,-z); }

//--------------------------------------------------------------------------------------

	//!	The += operator.
	/*!	
		\param v the PRTVector to +=
	*/
	inline void operator+=(const PRTVector &v){ x+=v.x; y+=v.y; z+=v.z; }
	
//--------------------------------------------------------------------------------------

	//!	The -= operator.
	/*!	
		\param v the PRTVector to -=
	*/
	inline void operator-=(const PRTVector &v){ x-=v.x; y-=v.y; z-=v.z; }

//--------------------------------------------------------------------------------------

	//!	The PRTVector*PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector solution
	*/
	inline PRTVector operator*(const PRTFloat f){ return PRTVector(x*f,y*f,z*f); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector*PRTVector operator, dot product. The dot product of a PRTVector \f$(x_1,y_1,z_1)\f$ and a PRTVector \f$(x_2,y_2,z_2)\f$ is \f$x_1*x_2+y_1*y_2+z_1*z_2\f$.
	/*!	
		\param v the PRTVector to operate
		\return the PRTFloat solution
	*/	
	inline PRTFloat operator*(const PRTVector &v){ return x*v.x+y*v.y+z*v.z; }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector^PRTVector operator, vectorial product. The vectorial product of a PRTVector \f$(x_1,y_1,z_1)\f$ and a PRTVector \f$(x_2,y_2,z_2)\f$ is \f$PRTVector(y_1*z_2-z_1*y_2,z_1*x_2-x_1*z_2,x_1*y_2-y_1*x_2)\f$.
	/*!	
		\param v the PRTVector to operate
		\return the PRTVector solution
	*/	
	inline PRTVector operator^(const PRTVector &v){ return PRTVector(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector/PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector solution
	*/	
	inline PRTVector operator/(PRTFloat f){ return (*this)*(1/f); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector/=PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector solution
	*/	
	inline PRTVector operator/=(PRTFloat f){ x/=f; y/=f; z/=f; return (*this); }

//--------------------------------------------------------------------------------------

	//!	The cosinus between two PRTVectors. Operator %.
	/*!	
		\param v the other PRTVector
		\return the PRTFloat cosinus
	*/	
	inline PRTFloat operator%(PRTVector v){ return (*this)*v/(Module()*v.Module()); }
	
//--------------------------------------------------------------------------------------

	//!	The compare operator ==.
	/*!	
		\param v the PRTVector to compare with
		\return are equal?
	*/	
	inline bool operator==(const PRTVector &v){ return (x==v.x && y==v.y && z==v.z); }
	
//--------------------------------------------------------------------------------------

	//!	The compare operator !=.
	/*!	
		\param v the PRTVector to compare with
		\return are different?
	*/		
	inline bool operator!=(const PRTVector &v){ return (x!=v.x || y!=v.y || z!=v.z); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector=PRTVector operator.
	/*!	
		\param v the PRTVector to assign 
		\return the PRTVector solution
	*/	
	inline PRTVector operator=(const PRTVector &v){ x=v.x; y=v.y; z=v.z; return v; }//*TODO* use w?
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector[] operator.
	/*!	
		\param i the index 
		\return the PRTFloat in PRTVector[i]
	*/	
	inline PRTFloat &operator[](const unsigned int i){ return (&x)[i]; }

//--------------------------------------------------------------------------------------

	//!	The PRTVector>PRTVector operator.
	/*!	
		\param v the PRTVector to compare
		\return the bool solution
	*/	
	inline bool operator>(const PRTVector &v){ return (x>v.x && y>v.y && z>v.z);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector<PRTVector operator.
	/*!	
		\param v the PRTVector to compare
		\return the bool solution
	*/		
	inline bool operator<(const PRTVector &v){ return (x<v.x && y<v.y && z<v.z);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector>=PRTVector operator.
	/*!	
		\param v the PRTVector to compare
		\return the bool solution
	*/		
	inline bool operator>=(const PRTVector &v){ return (x>=v.x && y>=v.y && z>=v.z);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector<=PRTVector operator.
	/*!	
		\param v the PRTVector to compare
		\return the bool solution
	*/		
	inline bool operator<=(const PRTVector &v){ return (x<=v.x && y<=v.y && z<=v.z);}

//--------------------------------------------------------------------------------------

	//!	The Module of a PRTVector. The module of a 3d vector \f$(x,y,z)\f$ is \f$\sqrt{x^2+y^2+z^2}\f$.
	/*!	
		\return the Module of the PRTVector
	*/	
	inline PRTFloat Module(void){ return PRTSqrt(x*x + y*y + z*z); }
	
//--------------------------------------------------------------------------------------

	//!	The function to normalize a PRTVector.
	/*!	
		\return the PRTVector normalized
	*/	
	inline PRTVector Normalize(void){ return (*this)/Module(); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector reflection based on a PRTVector normal.
	/*!	
		\param normal the PRTVector normal
		\return the PRTVector reflection
	*/	
	inline PRTVector Reflection(PRTVector normal){ return ((this->Normalize())-((normal*2.0)*(normal*(this->Normalize())))); }

//--------------------------------------------------------------------------------------

	//!	The perturb function.
	/*!	
		\param p1 perturb factor on one axis
		\param p2 perturb factor on another axis
		\return the PRTVector solution
	*/	
	PRTVector Perturb(const PRTFloat p1,const PRTFloat p2);

//--------------------------------------------------------------------------------------

	//!	The Unit function. See code.
	/*!	
		\return the PRTVector solution
	*/	
	inline PRTVector Unit(void){ PRTFloat d=(*this)*(*this);return d>0.0?(*this)*PRTSqrt(PRTFloat(1.0)/d):PRTVector(0,0,0); }
	
//--------------------------------------------------------------------------------------

	//!	The Unitize function. See code.
	/*!	
		\return the PRTFloat solution
	*/	
	inline PRTFloat Unitize(void){ PRTFloat l=(*this)*(*this);if(l!=1.0&&l>0.0){l=PRTSqrt(l);/*a/=l;*/}return l; }
	
//--------------------------------------------------------------------------------------

};

//--------------------------------------------------------------------------------------
//! The PRTVector4 class.
/*!	This class represents a vector in 4d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTVector4
{

public:

	PRTFloat x,y,z,w;		//!< The x,y,z,w coordinates.

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTVector4.
	/*!	
		\param vx the x coordinate
		\param vy the y coordinate
		\param vz the z coordinate
		\param vw the w coordinate
	*/
	inline PRTVector4(const PRTFloat vx=1.0,const PRTFloat vy=1.0,const PRTFloat vz=1.0,const PRTFloat vw=1.0){ x=vx; y=vy; z=vz; w=vw; }	

//--------------------------------------------------------------------------------------

	//!	The PRTVector4 conversion operator.
	/*!	
		\param v the PRTVector to convert
	*/		
	inline PRTVector4(const PRTVector &v){ x=v.x;y=v.y;z=v.z;w=1; } // *TODO* w=1 or w=0

//--------------------------------------------------------------------------------------

	//!	The PRTVector conversion operator.
	/*!	
	*/		
	inline operator PRTVector(void){ return PRTVector(x,y,z); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4+PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to add
		\return the PRTVector4 solution
	*/
	inline PRTVector4 operator+(const PRTVector4 &v){ return PRTVector4(x+v.x,y+v.y,z+v.z,w+v.w); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4-PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to substract
		\return the PRTVector4 solution
	*/
	inline PRTVector4 operator-(const PRTVector4 &v){ return PRTVector4(x-v.x,y-v.y,z-v.z,w-v.w); }

//--------------------------------------------------------------------------------------

	//!	The negate operator.
	/*!	
		\return the -PRTVector4
	*/	
	inline PRTVector4 operator-(void){ return PRTVector4(-x,-y,-z,-w); }

//--------------------------------------------------------------------------------------

	//!	The += operator.
	/*!	
		\param v the PRTVector4 to +=
	*/
	inline void operator+=(const PRTVector4 &v){ x+=v.x; y+=v.y; z+=v.z; w+=v.w; }
	
//--------------------------------------------------------------------------------------

	//!	The -= operator.
	/*!	
		\param v the PRTVector4 to -=
	*/
	inline void operator-=(const PRTVector4 &v){ x-=v.x; y-=v.y; z-=v.z; w-=v.w; }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4*PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector4 solution
	*/
	inline PRTVector4 operator*(const PRTFloat f){ return PRTVector4(x*f,y*f,z*f,w*f); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4*PRTVector4 operator, dot product. The dot product of a PRTVector4 \f$(x_1,y_1,z_1,w_1)\f$ and a PRTVector4 \f$(x_2,y_2,z_2,w_2)\f$ is \f$x_1*x_2+y_1*y_2+z_1*z_2+w_1*w_2\f$.
	/*!	
		\param v the PRTVector4 to operate
		\return the PRTFloat solution
	*/	
	inline PRTFloat operator*(const PRTVector4 &v){ return x*v.x+y*v.y+z*v.z+w*v.w; }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4^PRTVector4 operator, vectorial product. The vectorial product of a PRTVector4 \f$(x_1,y_1,z_1)\f$ and a PRTVector \f$(x_2,y_2,z_2)\f$ is \f$PRTVector(y_1*z_2-z_1*y_2,z_1*x_2-x_1*z_2,x_1*y_2-y_1*x_2)\f$.
	/*!	
		\param v the PRTVector4 to operate
		\return the PRTVector4 solution
	*/	
	inline PRTVector4 operator^(const PRTVector4 &v){ return PRTVector4(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4/PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector4 solution
	*/	
	inline PRTVector4 operator/(PRTFloat f){ return (*this)*(1/f); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4/=PRTFloat operator.
	/*!	
		\param f the PRTFloat to operate
		\return the PRTVector4 solution
	*/	
	inline PRTVector4 operator/=(PRTFloat f){ x/=f; y/=f; z/=f; w/=f; return (*this); }

//--------------------------------------------------------------------------------------

	//!	The cosinus between two PRTVectors4. Operator %.
	/*!	
		\param v the other PRTVector4
		\return the PRTFloat cosinus
	*/	
	inline PRTFloat operator%(PRTVector4 v){ return (*this)*v/(Module()*v.Module()); }
	
//--------------------------------------------------------------------------------------

	//!	The compare operator ==.
	/*!	
		\param v the PRTVector4 to compare with
		\return are equal?
	*/	
	inline bool operator==(const PRTVector4 &v){ return (x==v.x && y==v.y && z==v.z && w==v.w); }
	
//--------------------------------------------------------------------------------------

	//!	The compare operator !=.
	/*!	
		\param v the PRTVector4 to compare with
		\return are different?
	*/		
	inline bool operator!=(const PRTVector4 &v){ return (x!=v.x || y!=v.y || z!=v.z || w!=v.w); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4=PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to assign 
		\return the PRTVector4 solution
	*/	
	inline PRTVector4 operator=(const PRTVector4 &v){ x=v.x; y=v.y; z=v.z; w=v.w; return v; }//*TODO* use w?
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4[] operator.
	/*!	
		\param i the index 
		\return the PRTFloat in PRTVector4[i]
	*/	
	inline PRTFloat &operator[](const unsigned int i){ return (&x)[i]; }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4>PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to compare
		\return the bool solution
	*/	
	inline bool operator>(const PRTVector4 &v){ return (x>v.x && y>v.y && z>v.z && w>v.w);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4<PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to compare
		\return the bool solution
	*/		
	inline bool operator<(const PRTVector4 &v){ return (x<v.x && y<v.y && z<v.z && w<v.w);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4>=PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to compare
		\return the bool solution
	*/		
	inline bool operator>=(const PRTVector4 &v){ return (x>=v.x && y>=v.y && z>=v.z && w>=v.w);}
	
//--------------------------------------------------------------------------------------

	//!	The PRTVector4<=PRTVector4 operator.
	/*!	
		\param v the PRTVector4 to compare
		\return the bool solution
	*/		
	inline bool operator<=(const PRTVector4 &v){ return (x<=v.x && y<=v.y && z<=v.z && w<=v.w);}

//--------------------------------------------------------------------------------------

	//!	The Module of a PRTVector4. The module of a 4d vector \f$(x,y,z,w)\f$ is \f$\sqrt{x^2+y^2+z^2+w^2}\f$.
	/*!	
		\return the Module of the PRTVector4
	*/	
	inline PRTFloat Module(void){ return PRTSqrt(x*x + y*y + z*z + w*w); }
	
//--------------------------------------------------------------------------------------

	//!	The function to normalize a PRTVector4.
	/*!	
		\return the PRTVector4 normalized
	*/	
	inline PRTVector4 Normalize(void){ return (*this)/Module(); }

//--------------------------------------------------------------------------------------

	//!	The PRTVector4 reflection based on a PRTVector4 normal.
	/*!	
		\param normal the PRTVector4 normal
		\return the PRTVector4 reflection
	*/	
	inline PRTVector4 Reflection(PRTVector4 normal){ return ((this->Normalize())-((normal*2.0)*(normal*(this->Normalize())))); }

//--------------------------------------------------------------------------------------

	//!	The perturb function.
	/*!	
		\param p1 perturb factor on one axis
		\param p2 perturb factor on another axis
		\return the PRTVector4 solution
	*/	
	PRTVector4 Perturb(const PRTFloat p1,const PRTFloat p2);

//--------------------------------------------------------------------------------------

	//!	The Unit function. See code.
	/*!	
		\return the PRTVector solution
	*/	
	inline PRTVector4 Unit(void){ PRTFloat d=(*this)*(*this);return d>0.0?(*this)*PRTSqrt(PRTFloat(1.0)/d):PRTVector4(0,0,0,0); }
	
//--------------------------------------------------------------------------------------

	//!	The Unitize function. See code.
	/*!	
		\return the PRTFloat solution
	*/	
	inline PRTFloat Unitize(void){ PRTFloat l=(*this)*(*this);if(l!=1.0&&l>0.0){l=PRTSqrt(l);/*a/=l;*/}return l; }
	
//--------------------------------------------------------------------------------------

};

#endif
