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

#include "PRTUtil.h"
#include "PRTBitmap.h"
#include "PRTMath.h"
#include <string.h>

PRTBitmap::PRTBitmap(void)
	:PRTTexture(PRT_TEXTURE_BITMAP)
{
	t=NULL;
}

PRTBitmap::PRTBitmap(PRTBitmapImpExpData* data)
	:PRTTexture(PRT_TEXTURE_BITMAP)
{
	width=data->width;
	height=data->height;
	bytespp=data->bytespp;
	t=new unsigned char[width*height*bytespp];
	for (unsigned int i=0;i<width*height*bytespp;i++)
		t[i]=data->pixels[i];
}

/*PRTBitmap::PRTBitmap(char *ruta)
	:PRTTexture(PRT_TEXTURE_BITMAP)
{
	t=NULL;
	Load(ruta);
}*/

PRTBitmap::~PRTBitmap(void)
{
	//if (t!=NULL) //*TODO* caution with the objects that have a pointer to the texture
	//	delete[] t;
}
/*
bool PRTBitmap::Load(char *fich)
{
	char *aux=fich;
	aux+=strlen(fich)-3;
	bool res;

	if (strcmp(aux,"tga")==0 || strcmp(aux,"TGA")==0)	
		res=LoadTGA(fich);
	if (strcmp(aux,"jpg")==0 || strcmp(aux,"JPG")==0)
		res=LoadJPG(fich);
	if (strcmp(aux,"tga")!=0 && strcmp(aux,"TGA")!=0 && strcmp(aux,"jpg")!=0 && strcmp(aux,"JPG")!=0)
	{
		return false;
	}

	if (!res) 
	{
		t=NULL;
		width=height=bytespp=-1;
		return false;
	}
	
	return true;
}

bool PRTBitmap::LoadTGA(char *file)
{
	FILE *f=fopen(file,"rb");
	if (f==NULL)
	{
		return false;
	}

	// reads the width and the height of the image
	int aux,aux2,j;
	unsigned char c;
	for (j=1; j<=18; j++)
	{
		fscanf(f,"%c",&c);
		if (j==13)
			aux=(int)c;
		if (j==14)
		{
			aux2=(int)c;
			width = ( aux2 << 8 ) | aux;
		}
		if (j==15)
			aux=(int)c;
		if (j==16)
		{
			aux2=(int)c;
			height = ( aux2 << 8 ) | aux;
		}
		if (j==17)
			bytespp=((int)c)/8;
	}

	if (t!=NULL)
		delete t;
	t=new unsigned char[width*height*bytespp];
	if (!t)
	{
		return false;
	}

	unsigned char r,g,b,a;
	for (j=height-1; j>=0; j--)
		for (int i=0; i<width; i++)
		{
			if (bytespp==3)
				fscanf(f,"%c%c%c", &b, &g, &r);
			if (bytespp==4)
				fscanf(f,"%c%c%c%c", &b, &g, &r, &a);
			*(t+(((width*j)+i)*bytespp)+0)=r;
			*(t+(((width*j)+i)*bytespp)+1)=g;
			*(t+(((width*j)+i)*bytespp)+2)=b;
			if (bytespp==4)
				*(t+(((width*j)+i)*bytespp)+3)=a;
		}

	fclose(f);
	return true;
}*/

/*bool PRTBitmap::LoadJPG(char *file)
{

	FILE *f=fopen(file,"rb");
	if (f==NULL)
	{
		return false;
	}*/

	//use with IJL

	/*#ifdef __MAKE_WINDOWS__

	JPEG_CORE_PROPERTIES jcprops;

	if (ijlInit(&jcprops)!=IJL_OK)
		return false;

	jcprops.JPGFile=file;

	if (ijlRead(&jcprops,IJL_JFILE_READPARAMS)!=IJL_OK)
		return false;

	width=jcprops.JPGWidth;
	height=jcprops.JPGHeight;
	bytespp=jcprops.JPGChannels;

	if (t!=NULL)
		delete t;
	t=new unsigned char[width*height*bytespp];

	jcprops.DIBWidth=width;
	jcprops.DIBHeight=height;
	jcprops.DIBChannels=bytespp;
	jcprops.DIBColor=bytespp==3?IJL_RGB:IJL_RGBA_FPX;

	jcprops.DIBPadBytes=0;
	jcprops.DIBBytes=t;

	switch(jcprops.JPGChannels)
	{
		case 1:
		{
			jcprops.JPGColor = IJL_G;
			break;
		}
		case 3:
		{
			jcprops.JPGColor = IJL_YCBCR;
			break;
		}
		default:
		{
			jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
			jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
		break;
		}
	}

	if (ijlRead(&jcprops,IJL_JFILE_READWHOLEIMAGE)!=IJL_OK)
		return false;

	ijlFree(&jcprops);

	#endif

	#ifdef __MAKE_LINUX__
	*/
/*	
	//with JPEGLIB

	JSAMPARRAY buffer;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, TRUE);

    width=cinfo.image_width;
	height=cinfo.image_height;
	bytespp=cinfo.num_components;

	buffer = (*cinfo.mem->alloc_sarray)	((j_common_ptr) &cinfo, JPOOL_IMAGE, width*bytespp, 1);

	if (t!=NULL)
		delete t;
	t=new unsigned char[width*height*bytespp];

	jpeg_start_decompress(&cinfo);

	for (int j=0;j<height;j++)
	{
		int jarenauer=jpeg_read_scanlines(&cinfo, buffer, 1);
		for (int i=0;i<width*bytespp;i++)
			t[(j*width*bytespp)+i]=(*buffer)[i];
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//#endif
	
	fclose(f);
	return true;
}*/

PRTVector PRTBitmap::CalculateColor(PRTTexCoord aux, PRTVector p) // p is not used
{
	PRTFloat tu,tv;
	tu=aux.u-(PRTFloat)(int)aux.u;
	tv=aux.v-(PRTFloat)(int)aux.v;

	if (tu<0) tu+=1;
	if (tv<0) tv+=1;

	unsigned int coordx=(unsigned int)(tu*(PRTFloat)(width));
	unsigned int coordy=(unsigned int)(((PRTFloat)1-tv)*(PRTFloat)(height));

	if (coordx==width) coordx=0;
	if (coordy==height) coordy=0;
	
	PRTVector ret;
	if (!filter || (width<=1 || height<=1))
	{
		// without filter
		ret=PRTVector((PRTFloat)t[(coordy*width*bytespp)+(coordx*bytespp)+0]/PRTFloat(255.0),
					  (PRTFloat)t[(coordy*width*bytespp)+(coordx*bytespp)+1]/PRTFloat(255.0),
					  (PRTFloat)t[(coordy*width*bytespp)+(coordx*bytespp)+2]/PRTFloat(255.0));	
	}
	else
	{
		// with filter
		PRTVector aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8,aux9;

		PRTFloat peichx=PRTFloat(1.0)/(PRTFloat)(width);//*/0.005; *TODO*
		PRTFloat peichy=PRTFloat(1.0)/(PRTFloat)(height);//*/0.005; *TODO*
		
		filter=false;
		aux1=CalculateColor(aux+PRTTexCoord(-peichx,peichy),p);
		aux2=CalculateColor(aux+PRTTexCoord(0,peichy),p);
		aux3=CalculateColor(aux+PRTTexCoord(peichx,peichy),p);
		aux4=CalculateColor(aux+PRTTexCoord(-peichx,0),p);
		aux5=CalculateColor(aux+PRTTexCoord(0,0),p);
		aux6=CalculateColor(aux+PRTTexCoord(peichx,0),p);
		aux7=CalculateColor(aux+PRTTexCoord(-peichx,-peichy),p);
		aux8=CalculateColor(aux+PRTTexCoord(0,-peichy),p);
		aux9=CalculateColor(aux+PRTTexCoord(peichx,-peichy),p);
		ret=(aux1+aux2+aux3+aux4+aux5+aux6+aux7+aux8+aux9)/9;
		filter=true;
	}

	return ret;
}

PRTVector PRTBitmap::CalculateBump(PRTTexCoord aux,PRTVector normaltou,PRTVector normaltov,PRTVector p) // p is not used
{
	PRTFloat tu,tv;
	tu=aux.u-(PRTFloat)(int)aux.u;
	tv=aux.v-(PRTFloat)(int)aux.v;

	if (tu<0) tu+=1;
	if (tv<0) tv+=1;

	unsigned int coordx=(unsigned int)(tu*(PRTFloat)(width));
	unsigned int coordy=(unsigned int)(((PRTFloat)1-tv)*(PRTFloat)(height));

	if (coordx==width) coordx=0;
	if (coordy==height) coordy=0;

	PRTFloat peichx=PRTFloat(1.0)/(PRTFloat)(width);//*/0.005; *TODO*
	PRTFloat peichy=PRTFloat(1.0)/(PRTFloat)(height);//*/0.005; *TODO*
	
	PRTVector ret;
	if (!filter)
	{
		// without filter

		PRTFloat p0=CalculateColor(aux).x/255;
		PRTFloat pu=CalculateColor(aux+PRTTexCoord(peichx,0)).x/255;
		PRTFloat pv=CalculateColor(aux+PRTTexCoord(0,peichy)).x/255;

		ret=normaltou*(p0-pu)+normaltov*(p0-pv);
	}
	else
	{
		// with filter
		PRTVector aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8,aux9;
		
		filter=false;

		aux1=CalculateBump(aux+PRTTexCoord(-peichx,peichy),normaltou,normaltov);
		aux2=CalculateBump(aux+PRTTexCoord(0,peichy),normaltou,normaltov);
		aux3=CalculateBump(aux+PRTTexCoord(peichx,peichy),normaltou,normaltov);
		aux4=CalculateBump(aux+PRTTexCoord(-peichx,0),normaltou,normaltov);
		aux5=CalculateBump(aux+PRTTexCoord(0,0),normaltou,normaltov);
		aux6=CalculateBump(aux+PRTTexCoord(peichx,0),normaltou,normaltov);
		aux7=CalculateBump(aux+PRTTexCoord(-peichx,-peichy),normaltou,normaltov);
		aux8=CalculateBump(aux+PRTTexCoord(0,-peichy),normaltou,normaltov);
		aux9=CalculateBump(aux+PRTTexCoord(peichx,-peichy),normaltou,normaltov);
		ret=(aux1+aux2+aux3+aux4+aux5+aux6+aux7+aux8+aux9)/9;
		filter=true;
	}

	return ret;
}


/*void PRTBitmap::Save(char* fich)
{
	char *aux=fich;
	aux+=strlen(fich)-3;
	
	//if (strcmp(aux,"tga")==0 || strcmp(aux,"TGA")==0)	*TODO*
	//	SaveTGA(fich);
	if (strcmp(aux,"jpg")==0 || strcmp(aux,"JPG")==0)
		SaveJPG(fich);
	if (strcmp(aux,"tga")!=0 && strcmp(aux,"TGA")!=0 && strcmp(aux,"jpg")!=0 && strcmp(aux,"JPG")!=0)
	{
		//return false;
	}
}

void PRTBitmap::SaveJPG(char *nombre)
{
	FILE *f=fopen(nombre,"wb");
	if (f==NULL)
	{
		//return false;
	}

//extern "C" //only for c
//{

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, f);

	cinfo.image_height=height;
	cinfo.image_width=width;
	cinfo.input_components=bytespp;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 100, TRUE);
	
	jpeg_start_compress(&cinfo, TRUE);
	
	JSAMPROW row_pointer[1];
    int row_stride;            
    row_stride = cinfo.image_width * cinfo.input_components;
 
	while(cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = & t[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
//}

	fclose(f);
}*/
