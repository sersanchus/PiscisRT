#ifdef _WIN32
#include <windows.h>
#else
#endif

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
	
#ifdef _WIN32
	HANDLE scr;
	CONSOLE_SCREEN_BUFFER_INFO scrbi;
	COORD orig;
#endif
	
	Consola(void);
	void Muestra(void);
};
