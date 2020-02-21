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

#ifndef __PRTQUATERNION_H__
#define __PRTQUATERNION_H__

#include "PRTMath.h"

//--------------------------------------------------------------------------------------
//! The PRTQuaternion class.
/*!	This class represents a quaternion in 3d/4d.*/
//--------------------------------------------------------------------------------------

class PISCISRT_API PRTQuaternion
{

public:

	PRTFloat x,y,z,w;		//!< The x,y,z,w coordinates.

//--------------------------------------------------------------------------------------

	//!	The constructor of the PRTQuaternion.
	/*!	
		\param vx the x coordinate
		\param vy the y coordinate
		\param vz the z coordinate
		\param vw the w coordinate
	*/
	inline PRTQuaternion(PRTFloat vx=0.0, PRTFloat vy=0.0, PRTFloat vz=0.0, PRTFloat vw=0.0){ x=vx; y=vy; z=vz; w=vw;}	
	
//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion+PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to add
		\return the PRTQuaternion solution
	*/
	inline PRTQuaternion operator+(const PRTQuaternion &v){ return PRTQuaternion(x+v.x,y+v.y,z+v.z,w+v.w); }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion-PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to add
		\return the PRTQuaternion solution
	*/
	inline PRTQuaternion operator-(const PRTQuaternion &v){ return PRTQuaternion(x-v.x,y-v.y,z-v.z,w-v.w); }
	
//--------------------------------------------------------------------------------------

	//!	The += operator.
	/*!	
		\param v the PRTQuaternion to +=
	*/
	inline void operator+=(const PRTQuaternion &v){ x+=v.x; y+=v.y; z+=v.z; w+=v.w; }

//--------------------------------------------------------------------------------------

	//!	The -= operator.
	/*!	
		\param v the PRTQuaternion to -=
	*/
	inline void operator-=(const PRTQuaternion &v){ x-=v.x; y-=v.y; z-=v.z; w-=v.w; }

//--------------------------------------------------------------------------------------

	//!	The compare operator ==.
	/*!	
		\param v the PRTQuaternion to compare with
		\return are equal?
	*/	
	inline bool operator==(const PRTQuaternion &v){ return (x==v.x && y==v.y && z==v.z && w==v.w); }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion=PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to assign 
		\return the PRTQuaternion solution
	*/		
	inline PRTQuaternion operator=(const PRTQuaternion &v){ x=v.x; y=v.y; z=v.z; w=v.w; return v; }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion*PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to operate
		\return the PRTQuaternion solution
	*/
	PRTQuaternion operator*(const PRTQuaternion &v);

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion*=PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to operate
	*/
	inline void operator*=(const PRTQuaternion &v){ (*this)=(*this)*v; }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion*float operator.
	/*!	
		\param v the float to operate
		\return the PRTQuaternion solution
	*/
	inline PRTQuaternion operator*(const float &v) { return PRTQuaternion(x*v,y*v,z*v,w*v); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion*=float operator.
	/*!	
		\param v the float to operate
	*/
	inline void operator*=(const float &v) { x*=v;y*=v;z*=v;w*=v; }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion/PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to operate
		\return the PRTQuaternion solution
	*/
	PRTQuaternion operator/(const PRTQuaternion &v);

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion/=PRTQuaternion operator.
	/*!	
		\param v the PRTQuaternion to operate
	*/
	inline void operator/=(const PRTQuaternion &v){ (*this)=(*this)/v; }

//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion/float operator.
	/*!	
		\param v the float to operate
		\return the PRTQuaternion solution
	*/
	inline PRTQuaternion operator/(const float &v) { return PRTQuaternion(x/v,y/v,z/v,w/v); }
	
//--------------------------------------------------------------------------------------

	//!	The PRTQuaternion/=float operator.
	/*!	
		\param v the float to operate
	*/
	inline void operator/=(const float &v) { x/=v;y/=v;z/=v;w/=v; }
	
//--------------------------------------------------------------------------------------
	
	//!	The Norm function.
	/*!	
		\return the Norm
	*/
	PRTFloat Norm(void);

//--------------------------------------------------------------------------------------

	//!	The Module of a PRTQuaternion.
	/*!	
		\return the Module of the PRTVector
	*/
	PRTFloat Module(void);

//--------------------------------------------------------------------------------------
	
	//!	The Negate function.
	/*!	
		\return the PRTQuaternion solution
	*/	
	PRTQuaternion Negate(void);

//--------------------------------------------------------------------------------------
	
	//!	The Conjugate function.
	/*!	
		\return the PRTQuaternion solution
	*/	
	PRTQuaternion Conjugate(void);

//--------------------------------------------------------------------------------------
	
	//!	The Sqr (square) function.
	/*!	
		\return the PRTQuaternion solution
	*/	
	PRTQuaternion Sqr(void);

//--------------------------------------------------------------------------------------
	
	//!	The Invert function.
	/*!	
		\return the PRTQuaternion solution
	*/	
	PRTQuaternion Invert(void);

//--------------------------------------------------------------------------------------

	//!	The Sqrt (square root) function.
	/*!	
		\return the PRTQuaternion solution
	*/	
	PRTQuaternion Sqrt(void);

//--------------------------------------------------------------------------------------

	//*TODO*?
	/*inline PRTQuaternion operator*(PRTFloat f){ return PRTQuaternion(x*f,y*f,z*f,w*f); }
	inline bool operator!=(const PRTQuaternion &v){ return (x!=v.x || y!=v.y || z!=v.z || w!=v.w); }
	inline PRTQuaternion operator-(void){ return PRTQuaternion(-x,-y,-z,-w); }
	inline PRTQuaternion operator/(PRTFloat f){ return (*this)*(1/f); }
	inline PRTQuaternion operator/=(PRTFloat f){ x/=f; y/=f; z/=f; w/=f; return (*this); }
	inline PRTQuaternion operator=(const PRTQuaternion &v){ x=v.x; y=v.y; z=v.z; w=v.w; return PRTQuaternion(x,y,z,w); }
		
	inline PRTQuaternion operator^(const PRTQuaternion &v){ return PRTQuaternion(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x); }

	inline PRTFloat &operator[](unsigned int i){ return (&x)[i]; }

	inline PRTFloat operator%(PRTQuaternion v){ return (*this)*v/(Module()*v.Module()); }
	inline PRTQuaternion Normalize(void){ return (*this)/Module(); }

	PRTQuaternion Reflejo(PRTQuaternion);

	PRTQuaternion Perturbar(PRTFloat p1,PRTFloat p2);

	// para iluminacion del trianglelight ¿?
	PRTQuaternion Unit(void){PRTFloat d=(*this)*(*this);return d>0.0?(*this)*PRTSqrt(1.0/d):PRTQuaternion(0,0,0);}
	PRTFloat Unitize(void){PRTFloat l=(*this)*(*this);if(l!=1.0&&l>0.0){l=PRTSqrt(l);}return l;}
	
	inline bool operator>(PRTQuaternion &v){ return (x>v.x && y>v.y && z>v.z && w>v.w);}
	inline bool operator<(PRTQuaternion &v){ return (x<v.x && y<v.y && z<v.z && w<v.w);}
	inline bool operator>=(PRTQuaternion &v){ return (x>=v.x && y>=v.y && z>=v.z && w>=v.w);}
	inline bool operator<=(PRTQuaternion &v){ return (x<=v.x && y<=v.y && z<=v.z && w<=v.w);}*/

};

#endif
