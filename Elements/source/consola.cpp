#include "consola.h"
#include <stdio.h>

Consola::Consola(void)
{
	paso=NULL;
	framestotales=0;
	frame=1;
	tiempototal=0;
	tiempo=0;
	tiempoacum=0;
	t=0;
	tiempoaprox=0;
	porcentaje=0;
	
#ifdef _WIN32
	scr=GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(scr,&scrbi);
	orig=scrbi.dwCursorPosition;
#endif
	
	printf("Status         = ");
	//printf("%f %%",porcentaje*100);
	printf("\n");

	printf("Total time     = ");
	//muestratiempo(tiempoaprox);
	printf("\n");

	printf("Remaining time = ");
	//muestratiempo(tiempoaprox-tiempo);
	printf("\n");

	//printf("------------------------------------\n");

	/*orig.X=0;
	orig.Y=0;*/
}

void muestratiempo(double tiempoaux)
{
	int aux;
	aux=(int)tiempoaux/3600000;
	tiempoaux-=aux*3600000;
	if (aux!=0) printf("%d h : ",aux);
	aux=(int)tiempoaux/60000;
	tiempoaux-=aux*60000;
	if (aux!=0) printf("%d m : ",aux);
	aux=(int)tiempoaux/1000;
	tiempoaux-=aux*1000;
	printf("%d s : ",aux);
	printf("%d ms",(int)tiempoaux);
}

void Consola::Muestra(void)
{
	//Borra();
	
	/*printf("-- GENERAL --\n\n");

	printf("frame          = ");
	printf("%d",frame);
	printf("\n");

	printf("framestotales  = ");
	printf("%d",framestotales);
	printf("\n");

	printf("t              = ");
	printf("%f",t);
	printf("\n");

	printf("\n-- FRAME ACTUAL --\n\n");

	printf("paso           = ");
	printf("%s",paso);
	printf("\n");
	*/

#ifdef _WIN32
	GetConsoleScreenBufferInfo(scr,&scrbi);
	COORD coordini={scrbi.dwCursorPosition.X,scrbi.dwCursorPosition.Y};

	DWORD aux;
	COORD coord=orig;
	coord.X=17;
	FillConsoleOutputCharacter(scr,(TCHAR)' ',30,coord,&aux);
	SetConsoleCursorPosition(scr,coord);
	
	//printf("Status         = ");
	printf("%f %%",porcentaje*100);
	//printf("\n");

	coord.Y+=1;
	FillConsoleOutputCharacter(scr,(TCHAR)' ',30,coord,&aux);
	SetConsoleCursorPosition(scr,coord);

	//printf("Total time     = ");
	muestratiempo(tiempoaprox);
	//printf("\n");

	coord.Y+=1;
	FillConsoleOutputCharacter(scr,(TCHAR)' ',30,coord,&aux);
	SetConsoleCursorPosition(scr,coord);

	//printf("Remaining time = ");
	muestratiempo(tiempoaprox-tiempo);
	//printf("\n");

	SetConsoleCursorPosition(scr,coordini);
#endif
	
	/*printf("\n-- ANIMACIÓN --\n\n");

	printf("tiempo         = ");
	muestratiempo(tiempo);
	printf("\n");

	printf("tiempoacum     = ");
	muestratiempo(tiempoacum);
	printf("\n");

	printf("tiempototal    = ");
	muestratiempo(tiempototal);
	printf("\n");

	printf("tiemporestante = ");
	muestratiempo(tiempototal-tiempoacum);
	printf("\n");*/
}
