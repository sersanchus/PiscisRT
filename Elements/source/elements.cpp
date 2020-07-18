///////////////////////////////////////////////

#include <stdio.h>

#ifdef _WIN32
	#include <afxdlgs.h>
	#include "../extincs/glutwindows.h"
#else
	#ifdef __APPLE__
		#include <GLUT/GLUT.h>
	#else
		#include "../extincs/glut.h"
	#endif
	#include <sys/time.h>
#endif

//#include "../extincs/Python/Python.h"

#include "../../PiscisRTut/source/PRTutMain.h" 

#include <map>
#include <list>
#include <stack>
#include <string>
using namespace std;

#include "consola.h"

///////////////////////////////////////////////

unsigned int ancho=0;
unsigned int alto=0;
int ventanaancho=0;
int ventanaalto=0;
PRTMain *trazador;
string scenename;
Consola consola;
GLubyte* Pixels;
int ventana=-1;
float escala=1.0f;
float posx=0.0f;
float posy=0.0f;
int posxantes=0;
int posyantes=0;
unsigned int antialisiang;
PRTutMain utmain;

long GetTickCountMio()
{
#ifdef _WIN32
	return GetTickCount();
#else
	timeval tv;
	#ifdef __APPLE__
	struct timezone tz;
	#else
	__timezone_ptr_t tz;
	#endif
	gettimeofday(&tv,&tz);
	long res=tv.tv_sec*1000L+tv.tv_usec/1000L;
	return res;	
#endif
}

void pinta(void)
{
	glutSetWindow(ventana);
	glPixelZoom(escala,-escala);
	glClear(GL_COLOR_BUFFER_BIT);
	//glLoadIdentity();
	glRasterPos2f((posx)-((float)ancho*escala)/((float)ventanaancho),((float)alto*escala)/((float)ventanaalto)-(posy));
	glDrawPixels(ancho,alto,GL_RGB,GL_UNSIGNED_BYTE,Pixels);
	glutSwapBuffers();
	//glutPostRedisplay();
}

void render(bool silent)
{
	if (!silent)
	{
		pinta();
		glutPostRedisplay();
	}
	
	long tiempoinicio;

	tiempoinicio=GetTickCountMio();

	/////////////// PARSEA

	//PRTutParsea(Gargv[1],trazador,ancho,alto);
		
	/////////////// RENDERIZA

	int cont=0;
	for (int i=0;i<alto;i++)
	{
		for (int j=0;j<(/*stereo?ancho/2:*/ancho);j++)
		{	
			//if (!stereo)
			{
				PRTVector jar=PRTVector(0,0,0);

				jar=utmain.PRTutRenderPixel(j,i);

				Pixels[((i)*ancho*3)+(j*3)+0]=(GLubyte)(jar.x*255);
				Pixels[((i)*ancho*3)+(j*3)+1]=(GLubyte)(jar.y*255);
				Pixels[((i)*ancho*3)+(j*3)+2]=(GLubyte)(jar.z*255);
			}
			/*else
			{
				// OJO IZQUIERDO - PARA VERLO CON EL DERECHO

				PRTVector jar=PRTVector(0,0,0);
				
				if (camaraup==PRTVector())
					camaraup=PRTVector(0,1,0);

				for(int nb=0;nb<itera;nb++)
				{
					PRTVector N=(camarafin-camarapos).Normalize();	// -z de la camara
					PRTVector U=(N^camaraup).Normalize();				// x de la camara
					PRTVector camarapos2=camarapos+(U*stereowidth); // esto dice si es el izquierdo o el derecho
					N=(camarafin-camarapos2).Normalize();	// -z de la camara
					U=(N^camaraup).Normalize();				// x de la camara
					PRTVector V=(N^U).Normalize();					// -y de la camara
					PRTFloat fovrad=PRTGradToRad(fov);
					PRTVector Xinc=U*((2*tan(fovrad/2))/alto);
					PRTVector Yinc=V*((2*tan(fovrad/2))/alto);
					PRTFloat x=j;
					PRTFloat y=i;
					if (randomsamples)
					{
						x=j+(PRTWRandom()-0.5);
						y=i+(PRTWRandom()-0.5);
					}
					PRTVector aux=(N+(Yinc*(2*y+1-alto)*0.5)+(Xinc*(2*x+1-ancho/2)*0.5));

					jar=jar+trazador->RayTrace(PRTRay(camarapos2,aux.Normalize()),deep);
				}
									
				jar=jar/itera;

				Pixels[((alto-1-i)*ancho*4)+(j*4)+0]=(GLubyte)(jar.x*255);
				Pixels[((alto-1-i)*ancho*4)+(j*4)+1]=(GLubyte)(jar.y*255);
				Pixels[((alto-1-i)*ancho*4)+(j*4)+2]=(GLubyte)(jar.z*255);

				// OJO DERECHO - PARA VERLO CON EL IZQUIERDO

				jar=PRTVector(0,0,0);

				for(nb=0;nb<itera;nb++)
				{
					PRTVector N=(camarafin-camarapos).Normalize();	// -z de la camara
					PRTVector U=(N^camaraup).Normalize();				// x de la camara
					PRTVector camarapos2=camarapos-(U*stereowidth); // esto dice si es el izquierdo o el derecho
					N=(camarafin-camarapos2).Normalize();	// -z de la camara
					U=(N^camaraup).Normalize();				// x de la camara
					PRTVector V=(N^U).Normalize();					// -y de la camara
					PRTFloat fovrad=PRTGradToRad(fov);
					PRTVector Xinc=U*((2*tan(fovrad/2))/alto);
					PRTVector Yinc=V*((2*tan(fovrad/2))/alto);
					PRTFloat x=j;
					PRTFloat y=i;
					if (randomsamples)
					{
						x=j+(PRTWRandom()-0.5);
						y=i+(PRTWRandom()-0.5);
					}
					PRTVector aux=(N+(Yinc*(2*y+1-alto)*0.5)+(Xinc*(2*x+1-ancho/2)*0.5));

					jar=jar+trazador->RayTrace(PRTRay(camarapos2,aux.Normalize()),deep);
				}
									
				jar=jar/itera;

				Pixels[((alto-1-i)*ancho*4)+((j+ancho/2)*4)+0]=(GLubyte)(jar.x*255);
				Pixels[((alto-1-i)*ancho*4)+((j+ancho/2)*4)+1]=(GLubyte)(jar.y*255);
				Pixels[((alto-1-i)*ancho*4)+((j+ancho/2)*4)+2]=(GLubyte)(jar.z*255);
			}*/

		}
		
		if (cont==1)
		{
			consola.tiempo=double(GetTickCountMio()-tiempoinicio);
			consola.porcentaje=(double)i/((double)alto-1.0);
			consola.tiempoaprox=consola.porcentaje==0?999999999999999999999999999.9:double(GetTickCountMio()-tiempoinicio)/consola.porcentaje;
			//if (bconsola) 
				consola.Muestra();
			if (!silent)
			{	
				pinta();
				glutPostRedisplay();
			}
			cont=0;
		}
		else
			cont++;

	}

	if(antialisiang!=PRT_ANTIALIASING_NONE)
	{
	
		//FILTRA ADAPTATIVAMENTE

		cont=0;
		for (int i=1;i<alto-1;i++)
		{
			for (int j=1;j<ancho-1;j++)
			{	
				PRTVector color=PRTVector(Pixels[((i)*ancho*3)+(j*3)+0]*0.0039215686274509,Pixels[((i)*ancho*3)+(j*3)+1]*0.0039215686274509,Pixels[((i)*ancho*3)+(j*3)+2]*0.0039215686274509);
				PRTVector color1=PRTVector(Pixels[((i+1)*ancho*3)+(j*3)+0]*0.0039215686274509,Pixels[((i+1)*ancho*3)+(j*3)+1]*0.0039215686274509,Pixels[((i+1)*ancho*3)+(j*3)+2]*0.0039215686274509);
				PRTVector color2=PRTVector(Pixels[((i-1)*ancho*3)+(j*3)+0]*0.0039215686274509,Pixels[((i-1)*ancho*3)+(j*3)+1]*0.0039215686274509,Pixels[((i-1)*ancho*3)+(j*3)+2]*0.0039215686274509);
				PRTVector color3=PRTVector(Pixels[((i)*ancho*3)+((j+1)*3)+0]*0.0039215686274509,Pixels[((i)*ancho*3)+((j+1)*3)+1]*0.0039215686274509,Pixels[((i)*ancho*3)+((j+1)*3)+2]*0.0039215686274509);
				PRTVector color4=PRTVector(Pixels[((i)*ancho*3)+((j-1)*3)+0]*0.0039215686274509,Pixels[((i)*ancho*3)+((j-1)*3)+1]*0.0039215686274509,Pixels[((i)*ancho*3)+((j-1)*3)+2]*0.0039215686274509);
				PRTVector coloraverage=(color1+color2+color3+color4)*PRTFloat(0.25);

				PRTFloat threshold=PRTFloat(0.1);

				if(PRTAbs(coloraverage.x-color.x)>threshold || PRTAbs(coloraverage.y-color.y)>threshold || PRTAbs(coloraverage.z-color.z)>threshold)
				{				
					PRTVector jar=PRTVector(0,0,0);

					jar=utmain.PRTutFilterPixel(j,i);

					Pixels[((i)*ancho*3)+(j*3)+0]=(GLubyte)(jar.x*255);
					Pixels[((i)*ancho*3)+(j*3)+1]=(GLubyte)(jar.y*255);
					Pixels[((i)*ancho*3)+(j*3)+2]=(GLubyte)(jar.z*255);
				}
			}	

			if (cont==1)
			{
				consola.tiempo=double(GetTickCountMio()-tiempoinicio);
				consola.porcentaje=(double)i/((double)alto-1.0);
				consola.tiempoaprox=consola.porcentaje==0?999999999999999999999999999.9:double(GetTickCountMio()-tiempoinicio)/consola.porcentaje;
				//if (bconsola) 
					consola.Muestra();
				if (!silent)
				{	
					pinta();
					glutPostRedisplay();
				}
				cont=0;
			}
			else
				cont++;

		}
	}

	consola.tiempo=double(GetTickCountMio()-tiempoinicio);
	consola.porcentaje=1;
	consola.tiempoaprox=consola.porcentaje==0?999999999999999999999999999.9:double(GetTickCountMio()-tiempoinicio)/consola.porcentaje;
	//if (bconsola) 
		consola.Muestra();
	if (!silent)
	{
		pinta();
		glutPostRedisplay();
	}
}

bool renderya=false;

void reshape(int x, int y)
{
	ventanaancho=x;
	ventanaalto=y;
	
	glViewport(0,0,ventanaancho,ventanaalto);

	if (renderya)
	{
		render(false);
		renderya=false;
	}

	pinta();
}

void parseafichero(string ruta)
{
	if (ruta=="")
	{
		return;
	}
	
	/*FILE* fich;
	fich=fopen(ruta,"r");*/

	/*if (Gargv[2]!=NULL)
	{
		if (scenename!=NULL)
			delete scenename;
		scenename=new char;
		strcpy(scenename,Gargv[2]);
	}
	else*/
	
	trazador->Reset();
	
	
	if (scenename=="")
		fprintf(stderr,"No scene selected, first of all will parsed\n");

	utmain.PRTutParse(ruta,scenename,trazador,ancho,alto,antialisiang);
	
	if (Pixels!=NULL)
		delete Pixels;

	Pixels=new GLubyte[ancho*alto*3];
	for (int i2=0;i2<alto*ancho*3;i2++)
		Pixels[i2]=128;
}

void guardarimagen(string salida)
{
	PRTBitmapImpExpData* fdat=new PRTBitmapImpExpData;
	fdat->pixels=Pixels;
	fdat->width=ancho;
	fdat->height=alto;
	fdat->bytespp=3;

	string ext=salida.substr(salida.find(".")+1,salida.length()-salida.find(".")+1);
	PRTListMember* i2;
	i2=trazador->Plugins.first;
	bool res2=false;
	while(i2!=NULL)
	{
		if (((PRTPlugin*)i2->object)->Type()==PRT_PLUGIN_BITMAP_EXPORT)
		{
			string ext2=((PRTBitmapExport *)i2->object)->Extension();
			
			bool res=!ext2.compare(ext);
			
			if (res)
			{
				res2=((PRTBitmapExport *)i2->object)->Export(salida.c_str(),fdat);
				break;
			}
		}
		
		i2=i2->next;
	}
}

void SelectFromMenu(int idCommand)
{
#ifdef _WIN32
	if (idCommand==0)
	{
		string path;
	
		CFileDialog *fd=new CFileDialog(false,NULL,NULL,NULL,"Jpg image (*.jpg)|*.jpg|Png image (*.png)|*.png");
		if (fd->DoModal()==IDOK)
			path=fd->GetPathName();
		delete fd;
	
		if (path=="")
			return;
		
		guardarimagen(path);
	}
	else if(idCommand==1)
	{
		string path;
		CFileDialog *fd=new CFileDialog(true,NULL,NULL,NULL,"Prt file (*.prt)|*.prt");
		if (fd->DoModal()==IDOK)
			path=fd->GetPathName();
		delete fd;

		if (path=="")
			return;

		/*if (Pixels!=NULL)
				delete Pixels;*/

		scenename="";

		//if (scenename=="")
		//	fprintf(stderr,"No scene selected, first of all will parsed\n");

		parseafichero(path);

		/*Pixels=new GLubyte[ancho*alto*3];
		for (int i2=0;i2<alto*ancho*3;i2++)
			Pixels[i2]=128;*/

		render(false);
	}
#endif
}

static void keyboard(unsigned char key,int x,int y)
{
	switch (key)
	{
		case '+':
			escala+=0.1f;
			pinta();
			glutPostRedisplay();
			break;
		case '-':
			escala-=0.1f;
			pinta();
			glutPostRedisplay();
			break;
	}
}

static void mouse(int button, int state, int x, int y)
{
	posxantes=x;
	posyantes=y;
}

static void mousemotion(int x, int y)
{
	posx+=2.0f*(x-posxantes)/((float)ventanaancho);
	posy+=2.0f*(y-posyantes)/((float)ventanaalto);
	posxantes=x;
	posyantes=y;
	pinta();
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	trazador=new PRTMain();
		
	bool silent=false;
	bool siscene=false;
	string salida;
	int numarg=1;
	while (argv[numarg]!=NULL)
	{
		if (numarg==1)
		{
			renderya=true;
		}
		else
		{
			if (string(argv[numarg]).find("-silent:")==0)
			{
				salida=string(argv[numarg]).substr(8,string(argv[numarg]).length()-8);
				silent=true;
			}
			else if (string(argv[numarg]).find("-scene:")==0)
			{
				scenename=string(argv[numarg]).substr(7,string(argv[numarg]).length()-7);
				siscene=true;
			}
		}
		numarg++;
	}

	ancho=400;
	alto=400;

	if (renderya)
		parseafichero(argv[1]);

	ventanaancho=ancho;
	ventanaalto=alto;

	Pixels=new GLubyte[ancho*alto*3];
	for (int i2=0;i2<alto*ancho*3;i2++)
		Pixels[i2]=128;

	if (!silent)
	{
		glutInit(&argc,argv);
		glutInitWindowPosition(0,0);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

		glutInitWindowSize(ventanaancho,ventanaalto);
		ventana=glutCreateWindow(string("Elements").c_str());
		glutSetWindow(ventana);
		glutDisplayFunc(pinta);

		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutMotionFunc(mousemotion);
		glutMouseFunc(mouse);
		int menu;
		menu = glutCreateMenu (SelectFromMenu);
		glutAddMenuEntry ("Render file...", 1);
		glutAddMenuEntry ("Save image...", 0);
		glutAttachMenu (GLUT_RIGHT_BUTTON);
		//glutShowWindow();
		//glutReshapeWindow(ancho,alto);
		//glutPostRedisplay();
		glPixelZoom(escala,-escala);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glClearColor(0.5,0.5,0.5,1);
		glutMainLoop();
		glutDestroyWindow(ventana);
	}
	else
	{
		render(true);
		if (salida!="")
		{
			guardarimagen(salida);
		}
	}
	
	if (Pixels!=NULL)
		delete Pixels;
			
	delete trazador;

	return 0;
}
