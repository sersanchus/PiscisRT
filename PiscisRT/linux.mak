OUTDIR=./lib
INTDIR=./lib
EXTLIB=./extlibs
SOURCE=./source

CPP=gcc
CPP_OBJ1=-c
CPP_OBJ2=-o

PiscisRT:	all
		#$(CPP) -shared -o $(OUTDIR)/libPiscisRT.a $(INTDIR)/*.o -lm -L$(EXTLIB) -ljpeg -lstdc++
		ar -rvs $(OUTDIR)/libPiscisRT.a $(INTDIR)/*.o

all:	$(SOURCE)/*.cpp $(SOURCE)/*.h
	for fich in `ls $(SOURCE) -h|grep ".cpp"|sed -e 's/.cpp//'` ; do $(CPP) $(CPP_OBJ1) $(SOURCE)/$$fich.cpp $(CPP_OBJ2) $(OUTDIR)/$$fich.o ; echo $$fich ; done
