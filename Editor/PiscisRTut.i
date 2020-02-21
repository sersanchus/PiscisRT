%module PiscisRTut

%{
    #include "..\PiscisRTut\source\PRTutMain.h"
%}

extern bool PRTutInicia(void);
extern bool PRTutParsea(char*,PRTMain*,int&,int&,double tiempo=0.0);
extern PRTVector PRTutRenderPixel(int,int,PRTMain*);
extern bool PRTutFinaliza(void);
