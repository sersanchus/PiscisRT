#include <windows.h>

class Consola
{
public:
	
	char *paso;
	int framestotales;
	int frame;
	double tiempototal;
	double tiempo;
	double tiempoacum;
	double t;
	double tiempoaprox;
	double porcentaje;
	
	HANDLE scr;
	CONSOLE_SCREEN_BUFFER_INFO scrbi;
	COORD orig;
	
	Consola(void);
	void Muestra(void);
};