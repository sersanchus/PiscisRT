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
	return PRT_PLUGIN_BITMAP_IMPORT;
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

bool Import(LPCSTR fich,PRTBitmapImpExpData *dat)
{
	FILE *f=fopen(fich,"rb");
	if (f==NULL)
	{
		return false;
	}

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
	
	//with JPEGLIB

	JSAMPARRAY buffer;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, TRUE);

    dat->width=cinfo.image_width;
	dat->height=cinfo.image_height;
	dat->bytespp=cinfo.num_components;

	buffer = (*cinfo.mem->alloc_sarray)	((j_common_ptr) &cinfo, JPOOL_IMAGE, dat->width*dat->bytespp, 1);

	dat->pixels=new unsigned char[dat->width*dat->height*dat->bytespp];

	jpeg_start_decompress(&cinfo);

	for (unsigned int j=0;j<dat->height;j++)
	{
		int jarenauer=jpeg_read_scanlines(&cinfo, buffer, 1);
		for (unsigned int i=0;i<dat->width*dat->bytespp;i++)
			dat->pixels[(j*dat->width*dat->bytespp)+i]=(*buffer)[i];
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//#endif
	
	fclose(f);
	return true;
}