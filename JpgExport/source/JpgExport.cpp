#include <windows.h>
#include "..\..\PiscisRT\source\PRTBitmapImpExp.h"

#ifdef _WIN32
	//#include "ijl.h" //intel jpeg library
	extern "C" //only for c
	{
		#include "../extincs/jpeglib.h"
	}
#else
	extern "C" //only for c
	{
		#include "../extincs/jpegliblinux.h"
	}
#endif

char * Identification(void)
{
	static char res[]=PRT_PLUGIN_IDENTIFICATION;
	return res;
}

int Type(void)
{
	return PRT_PLUGIN_BITMAP_EXPORT;
}

char *Extension(void)
{
	static char res[]="jpg";	
	return res;
}

char *Description(void)
{
	static char res[]="Mapa de bits jpeg";
	return res;
}

bool Export(LPCSTR fich,PRTBitmapImpExpData *dat)
{
	FILE *f=fopen(fich,"wb");
	if (f==NULL)
	{
		return false;
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, f);

	cinfo.image_height=dat->height;
	cinfo.image_width=dat->width;
	cinfo.input_components=dat->bytespp;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 100, TRUE);
	
	jpeg_start_compress(&cinfo, TRUE);
	
	JSAMPROW row_pointer[1];
    int row_stride;            
    row_stride = cinfo.image_width * cinfo.input_components;
 
	while(cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = & dat->pixels[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	fclose(f);
	return true;
}