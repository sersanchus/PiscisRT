#include <windows.h>
#include "..\..\PiscisRT\source\PRTBitmapImpExp.h"

#include "../extincs/png.h"

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
	static char res[]="png";	
	return res;
}

char *Description(void)
{
	static char res[]="Mapa de bits png";
	return res;
}

bool Import(LPCSTR fich,PRTBitmapImpExpData *dat)
{
	unsigned char header[8];

	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 8;
	png_uint_32 width, height;
	//int bit_depth, color_type;
	int number_of_passes;
	png_bytep * row_pointers;
	
	FILE *f=fopen(fich,"rb");
	if (f==NULL)
	{
		return false;
	}

	fread(header, 1, 8, f);
	if (png_sig_cmp(header, 0, 8))
	{
		fclose(f);
		return false;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(f);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(f);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(f);
		return false;
	}

	png_init_io(png_ptr, f);

	png_set_sig_bytes(png_ptr, sig_read);

	png_read_info(png_ptr, info_ptr);

	width = info_ptr->width;
	height = info_ptr->height;
	//color_type = info_ptr->color_type;
	//bit_depth = info_ptr->bit_depth;

	dat->width=width;
	dat->height=height;
	dat->bytespp=info_ptr->pixel_depth/8;

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(f);
		return false;
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (unsigned int y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

	png_read_image(png_ptr, row_pointers);

	dat->pixels=new unsigned char[width*height*dat->bytespp];

	for (y=0; y<height; y++)
	{
		for (unsigned int x=0; x<width; x++)
		{
			dat->pixels[y*width*dat->bytespp+x*dat->bytespp]=row_pointers[y][x*dat->bytespp];
			dat->pixels[y*width*dat->bytespp+x*dat->bytespp+1]=row_pointers[y][x*dat->bytespp+1];
			dat->pixels[y*width*dat->bytespp+x*dat->bytespp+2]=row_pointers[y][x*dat->bytespp+2];
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	delete[] row_pointers;

	fclose(f);
   	return true;
}