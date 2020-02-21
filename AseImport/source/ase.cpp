 // NOTAS: Las cosas animadas NO se exportan

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "..\..\PiscisRT\source\PRTMeshImpExp.h"
//#include "resource.h"
//#include <afxwin.h>

#define MAXCHARLONG 256
#define MAXNUMTEXTURAS 24 // ke no se kede corto
#define LUZ_OMNI	0

#define SGEMAPVER	3
#define TEXLUZ_MARGEN 1

HWND hDialog=NULL;

void PRINTF(char *s,...)
{
	va_list args;
	va_start(args,s);

	char buff[1024]="";
	vsprintf(buff,s,args);

	va_end(s);

	char buff2[1024]="";
	char buffcar[2]="";
	for (unsigned int i=0; i<strlen(buff); i++)
	{
		if (buff[i]=='\n')
			strcat(buff2,(const char*)"\r\n");
		else
		{
			buffcar[0]=buff[i];
			buffcar[1]='\0';
			strcat(buff2,buffcar);
		}
	}	

	//SendMessage(GetDlgItem(hDialog,IDC_EDIT_RESUMEN),EM_REPLACESEL,FALSE,(LPARAM)buff2);
}

/*
#define TEXLUZ_ANCHO	256
#define TEXLUZ_ALTO		256
#define TEXLUZ_TRIANGULO_ANCHO	16
#define TEXLUZ_TRIANGULO_ALTO	16
*/

int TEXLUZ_ANCHO;
int TEXLUZ_ALTO;
int TEXLUZ_TRIANGULO_ANCHO;
int TEXLUZ_TRIANGULO_ALTO;

struct VERTICE
{
	float x,y,z;
	struct VERTICE *sig;
};

struct VERTICE **verts;
struct VERTICE **ncara; // normales de cara
struct VERTICE **nvert; // normales de vertice
struct VERTICE **lm_texverts; // coordenadas de mapeado de cada vertice de cada lightmap

#define POS_NODEFINIDO -1
#define POS_ARRIBA 0
#define POS_ABAJO 1

struct TRILM_POSICION{
	int lm,cuad,pos;
}**trilm_posicion;

struct CARA
{
	int A,B,C;
	int suavizada;
	struct CARA *sig;
};

struct OBJETO
{
	int numv, numc, numtv;
	struct VERTICE *v, *ultv;
	struct CARA *c, *ultc;
	struct VERTICE *tv, *ulttv;
	struct CARA *tc, *ulttc;
	struct VERTICE *nc, *ultnc;
	float *nv;
	struct OBJETO *sig;
	int texid; 
	struct VERTICE color; // solo cuando no haya textura
	char *nombre;
};

struct BTEX
{
	int n;
	char cadcad[6][MAXCHARLONG];
};

struct LISTA
{
	int numo, numv, numc, numtv;
	struct OBJETO *prim, *ult; 
};

struct LUZ
{
	int tipo;
	float pos[3];
	float color[3];
	float intensidad;
	struct LUZ *sig;
};

struct MAPOBJ
{
	char *nombre;
	float pos[3];
	struct MAPOBJ *sig;
};

struct GLOBAL
{
	int numl, numo, numlin, numcam;
	struct LUZ *priml, *ultl;
//	float poscamara[3], miracamara[3], angrotcamara, fov;
//	float posjug1[3], posjug2[3];
	struct MAPOBJ *primo, *ulto;
	struct LINEA *primlin, *ultlin;
	struct CAMARA *primcam, *ultcam;
};

struct POLI
{
	int ipoli;
	struct POLI *sig;
};

struct NODO_OCTREE
{
	int id;
	float izq,der,arr,aba,del,det;
	int *numpolis;
	int numtotalpolis;
	struct POLI **polis; // polis[itex]->[prim]
	struct NODO_OCTREE **hijos;
	int idpadre;
};

struct LINEA
{
	char *nombre;
	int numv;
	struct VERTICE *v;
	struct LINEA *sig;
};

struct CAMARA
{
	char *nombre;
	struct VERTICE pos;
	struct VERTICE mira;
	float angrot;
	float fov;
	struct CAMARA *sig;
};

int octree_numhojas=0;
int octree_numnodos=0;

// Devuelve la posición de una cadena en un fichero de texto,
// a partir de la posicion pos
// si la cadena no existe devuelve -1
int BuscaTexto(FILE *f, char *texto, int pos)
{
	char aux[MAXCHARLONG]="";
	int longitud=0;
	longitud=strlen(texto);
	while(!feof(f))
	{
		fseek(f,pos++,SEEK_SET);
		fgets(aux,longitud+1,f);
		if (strcmp(aux,texto)==0) // si la cadena coincide
			return pos;  // devuelve su pos
	}
	return -1;  // si llega hasta aqui es que no se ha encontrado, devolver -1
}

// igual que la anterior con la diferencia que en vez de pasarle un array de caracteres
// se le pasa un array de cadenas, devuelve el indice de la cadena que primero se ha encontrado
// ocurrencia de una de esas cadenas.
// la pos se actualiza automaticamente.
int BuscaTextoEx(FILE *f, struct BTEX a, int *pos)
{
	char aux[MAXCHARLONG]="";
	int longitud=0;
	int i;
	while(!feof(f))
	{
		for (i=0; i<a.n; i++)
		{
			fseek(f,*pos,SEEK_SET);
			longitud=strlen(a.cadcad[i]);
			fgets(aux,longitud+1,f);
			if (strcmp(aux,a.cadcad[i])==0) // si la cadena coincide
				return i;  // devuelve su pos
		}
		*(pos)=*(pos)+1;
	}
	return -1;
}


void agregar(struct LISTA *l, struct OBJETO *ob)
{
	struct OBJETO *o;
	o=(struct OBJETO*)malloc(sizeof(struct OBJETO));
	*o=*ob;
	o->nombre=(char*)malloc(sizeof(char)*strlen(ob->nombre));
	strcpy(o->nombre,ob->nombre);

	if (l->ult==NULL)
	{
		l->prim=o;
		l->ult=o;
	}
	else
	{
		l->ult->sig=o;
		l->ult=o;
	}
	l->numo++;
	l->numv+=o->numv;
	l->numc+=o->numc;
	l->numtv+=o->numtv;
}

void agregarv(struct OBJETO *o, struct VERTICE *ver)
{
	struct VERTICE *v;
	v=(struct VERTICE*)malloc(sizeof(struct VERTICE));
	*v=*ver;
	if (o->ultv==NULL)
	{
		o->v=v;
		o->ultv=v;
	}
	else
	{
		o->ultv->sig=v;
		o->ultv=v;
	}
}

void agregarc(struct OBJETO *o, struct CARA *car)
{
	struct CARA *c;
	c=(struct CARA*)malloc(sizeof(struct CARA));
	*c=*car;
	if (o->ultc==NULL)
	{
		o->c=c;
		o->ultc=c;
	}
	else
	{
		o->ultc->sig=c;
		o->ultc=c;
	}
}

void agregartv(struct OBJETO *o, struct VERTICE *ver)
{
	struct VERTICE *v;
	v=(struct VERTICE*)malloc(sizeof(struct VERTICE));
	*v=*ver;
	if (o->ulttv==NULL)
	{
		o->tv=v;
		o->ulttv=v;
	}
	else
	{
		o->ulttv->sig=v;
		o->ulttv=v;
	}
}

void agregartc(struct OBJETO *o, struct CARA *car)
{
	struct CARA *c;
	c=(struct CARA*)malloc(sizeof(struct CARA));
	c->A=car->A;
	c->B=car->B;
	c->C=car->C;
	c->sig=NULL;
	if (o->ulttc==NULL)
	{
		o->tc=c;
		o->ulttc=c;
	}
	else
	{
		o->ulttc->sig=c;
		o->ulttc=o->ulttc->sig;
	}
}


void agregarnc(struct OBJETO *o, struct VERTICE *ver)
{
	struct VERTICE *v;
	v=(struct VERTICE*)malloc(sizeof(struct VERTICE));
	*v=*ver;
	if (o->ultnc==NULL)
	{
		o->nc=v;
		o->ultnc=v;
	}
	else
	{
		o->ultnc->sig=v;
		o->ultnc=v;
	}
}

void agregarl(struct GLOBAL *g, struct LUZ *luz)
{
	struct LUZ *lu;
	lu=(struct LUZ*)malloc(sizeof(struct LUZ));
	*lu=*luz;
	if (g->ultl==NULL)
	{
		g->priml=lu;
		g->ultl=lu;
	}
	else
	{
		g->ultl->sig=lu;
		g->ultl=lu;
	}
	g->numl++;
}

void agregarmo(struct GLOBAL *g, struct MAPOBJ *mo)
{
	struct MAPOBJ *mao;
	mao=(struct MAPOBJ*)malloc(sizeof(struct MAPOBJ));
	mao->nombre=(char*)malloc(strlen(mo->nombre));
	strcpy(mao->nombre,mo->nombre);
	*mao=*mo;
	if (g->ulto==NULL)
	{
		g->primo=mao;
		g->ulto=mao;
	}
	else
	{
		g->ulto->sig=mao;
		g->ulto=mao;
	}
	g->numo++;
}

void agregarlin(struct GLOBAL *g, struct LINEA *li)
{
	struct LINEA *lin;
	int i;

	lin=(struct LINEA*)malloc(sizeof(struct LINEA));
	lin->nombre=(char*)malloc(strlen(li->nombre));
	strcpy(lin->nombre,li->nombre);
	lin->numv=li->numv;
	lin->sig=li->sig;
	lin->v=(struct VERTICE*)malloc(sizeof(struct VERTICE)*lin->numv);
	for (i=0; i<lin->numv; i++)
	{
		lin->v[i].x=li->v[i].x;
		lin->v[i].y=li->v[i].y;
		lin->v[i].z=li->v[i].z;
	}

	if (g->ultlin==NULL)
	{
		g->primlin=lin;
		g->ultlin=lin;
	}
	else
	{
		g->ultlin->sig=lin;
		g->ultlin=lin;
	}
	g->numlin++;
}

void agregarcam(struct GLOBAL *g, struct CAMARA *ca)
{
	struct CAMARA *cam;

	cam=(struct CAMARA*)malloc(sizeof(struct CAMARA));
	cam->nombre=(char*)malloc(strlen(ca->nombre));
	strcpy(cam->nombre,ca->nombre);
	cam->angrot=ca->angrot;
	cam->fov=ca->fov;
	cam->pos.x=ca->pos.x;
	cam->pos.y=ca->pos.y;
	cam->pos.z=ca->pos.z;
	cam->mira.x=ca->mira.x;
	cam->mira.y=ca->mira.y;
	cam->mira.z=ca->mira.z;
	cam->sig=ca->sig;

	if (g->ultcam==NULL)
	{
		g->primcam=cam;
		g->ultcam=cam;
	}
	else
	{
		g->ultcam->sig=cam;
		g->ultcam=cam;
	}
	g->numcam++;
}


// devuelve el identificador de la lista ke contiene ese triangulo
// si devuelve -1 es ke esa cara noe sta en ninguna lista
int QueLista(int cara, struct LISTA *l, int nmats)
{
	int i, ncarasantes=0;

	for (i=0; i<nmats; i++)
	{
		if (l[i].numc+ncarasantes > cara)
			return i;
		else
			ncarasantes+=l[i].numc;
	}
	return -1;
}

struct VERTICE ProdVect(struct VERTICE v, struct VERTICE w)
{
	struct VERTICE u;
	u.x = v.y*w.z - v.z*w.y;
	u.y = v.z*w.x - v.x*w.z;
	u.z = v.x*w.y - v.y*w.x;
	return u;
}

// devuelve el i-esimo vertice de la lista l
struct VERTICE *DaVertice(struct LISTA *l, int i)
{	
 	struct VERTICE *vaux;
	struct OBJETO *aux;
	int cont=0;

	aux=l->prim;
	while(aux!=NULL)
	{
		vaux=aux->v;
		while(vaux!=NULL)
		{
			if (cont==i)
				return vaux;
			vaux=vaux->sig;				
			cont++;
		}
		aux=aux->sig;
	}
	return NULL;
}

// devuelve el i-esimo vertice de la lista l
struct VERTICE *DaVerticeMapeado(struct LISTA *l, int i)
{	
 	struct VERTICE *vaux;
	struct OBJETO *aux;
	int cont=0;

	aux=l->prim;
	while(aux!=NULL)
	{
		vaux=aux->tv;
		while(vaux!=NULL)
		{
			if (cont==i)
				return vaux;
			vaux=vaux->sig;				
			cont++;
		}
		aux=aux->sig;
	}
	return NULL;
}

// devuelve la i-esima cara de la lista l
struct CARA *DaCara(struct LISTA *l, int i)
{
	struct CARA *caux;
	struct OBJETO *aux;
	int cont=0;
	aux=l->prim;		
	while(aux!=NULL)
	{
		caux=aux->c;
		while(caux!=NULL)
		{
			if (cont==i)
				return caux;
			caux=caux->sig;
			cont++;
		}
		aux=aux->sig;			
	}
	return NULL;
}

// devuelve la i-esima luz
struct LUZ *DaLuz(struct GLOBAL *g, int i)
{
	int cont=0;
	struct LUZ *aux;
	aux=g->priml;
	while(aux!=NULL)
	{
		if (cont==i)
			return aux;		
		cont++;		
		aux=aux->sig;			
	}
	return NULL;
}

struct CARA *DaCaraMapeado(struct LISTA *l, int i)
{
	struct CARA *caux;
	struct OBJETO *aux;
	int cont=0;
	aux=l->prim;		
	while(aux!=NULL)
	{
		caux=aux->tc;
		while(caux!=NULL)
		{
			if (cont==i)
				return caux;
			caux=caux->sig;
			cont++;
		}
		aux=aux->sig;			
	}
	return NULL;
}

struct VERTICE *DaNormalCara(struct LISTA *l, int i)
{	
 	struct VERTICE *vaux;
	struct OBJETO *aux;
	int cont=0;

	aux=l->prim;
	while(aux!=NULL)
	{
		vaux=aux->nc;
		while(vaux!=NULL)
		{
			if (cont==i)
				return vaux;
			vaux=vaux->sig;				
			cont++;
		}
		aux=aux->sig;
	}
	return NULL;
}


long int DaIndiceTrianguloMapaLuz(int x, int y)
{
/*	int indice=((TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)*(y/TEXLUZ_TRIANGULO_ALTO))+(x/TEXLUZ_TRIANGULO_ANCHO);
	indice*=2;
	if (x-x/TEXLUZ_TRIANGULO_ANCHO*TEXLUZ_TRIANGULO_ANCHO > y-y/TEXLUZ_TRIANGULO_ALTO*TEXLUZ_TRIANGULO_ALTO)
		indice++;
	return indice;*/
	return 0;
}

// devuelve el nombre del i-esimo objeto de la lista l
char *DaNombre(struct LISTA *l, int i)
{
	struct OBJETO *aux;
	int cont=0;
	aux=l->prim;		
	while(aux!=NULL)
	{
		if (i==cont)
			return aux->nombre;
		cont++;
		aux=aux->sig;			
	}
	return NULL;
}

struct VERTICE Normalizar(struct VERTICE v)
{
	struct VERTICE res;
	float modulo=(float)sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	res.x=v.x/modulo;
	res.y=v.y/modulo;
	res.z=v.z/modulo;
	return res;
}

struct VERTICE *ColisionMapa(struct LISTA *l, int numtex, float desdex, float desdey, float desdez, float ax, float ay, float az, int itexskip, int itriskip)
{
	static struct VERTICE col;
	static struct VERTICE va,vb,vc,v1,v2,v3,w,p,fn,go,colaux;
	float a,b,c,d,t,den,alpha,beta;
	int i,j;
	for (i=0; i<numtex; i++)
	{
		for (j=0; j<l[i].numc; j++)
		{
			if (i==itexskip && j==itriskip)
				continue;

/*			ent=0;						
			aux=l[i].prim;
			cars=aux->numc;
			while(aux!=NULL)
			{
				if (cars>=j)
					break;							
				ent+=aux->numv;
				aux=aux->sig;
				if (aux!=NULL)
					cars+=aux->numc;
			}
*/

//			va=*DaVertice(&l[i],DaCara(&l[i],j)->A+ent);
//			vb=*DaVertice(&l[i],DaCara(&l[i],j)->B+ent);
//			vc=*DaVertice(&l[i],DaCara(&l[i],j)->C+ent);
			va=verts[i][j*3+0];
			vb=verts[i][j*3+1];
			vc=verts[i][j*3+2];
			v1.x = vb.x-va.x;
			v1.y = vb.y-va.y;
			v1.z = vb.z-va.z;
			v2.x = vc.x-va.x;
			v2.y = vc.y-va.y;
			v2.z = vc.z-va.z;
//			p = *DaVertice(&l[i],DaCara(&l[i],j)->A+ent);
			p=verts[i][j*3+0];

			// calculate the plane containing the triangle of the map
			a = v1.y*v2.z-v1.z*v2.y;
			b = v1.z*v2.x-v1.x*v2.z;
			c = v1.x*v2.y-v1.y*v2.x;
			d = v1.x*(v2.z*p.y-v2.y*p.z)+v1.y*(v2.x*p.z-v2.z*p.x)+v1.z*(v2.y*p.x-v2.x*p.y);
//			fn=*DaNormalCara(&l[i],j);
			go.x=ax-desdex;
			go.y=ay-desdey;
			go.z=az-desdez;				

			// is that face oriented to us? = can we see it?
/*			if (cull)
				if (DotProduct(go,fn)>0)
					continue; // we can't see it, it's oriented to the other side. If we can see it, we can't collide with it
*/
//			distgoto=sqrt((tox-fromx)*(tox-fromx) + (toy-fromy)*(toy-fromy) + (toz-fromz)*(toz-fromz));

			if ((a*(ax-desdex)+b*(ay-desdey)+c*(az-desdez))!=0) // if !=0 then exists a solution -> they collided
			{
				t=-(a*desdex+b*desdey+c*desdez+d)/(a*(ax-desdex)+b*(ay-desdey)+c*(az-desdez));
				//if (t>=-0.2f && t<=1.1f) // take only the segment
				if (t>=0.0f && t<=1.0f) // take only the segment
//					if (t>=-0.2f && t<=distgoto+0.1f)
				{
					// calculate the intersection point
					col.x = desdex+t*(ax-desdex);
					col.y = desdey+t*(ay-desdey);
					col.z = desdez+t*(az-desdez);
	
//					colaux=*DaVertice(&l[i],DaCara(&l[i],j)->A+ent);
					colaux=verts[i][j*3];
					col.x-=colaux.x;
					col.y-=colaux.y;
					col.z-=colaux.z;

					// find out wheather the point is inside the triangle						
					v3=ProdVect(v1,v2);
					den=(v1.x*(v2.y*v3.z-v3.y*v2.z) + v2.x*(v3.y*v1.z-v1.y*v3.z) + v3.x*(v1.y*v2.z-v2.y*v1.z));
					alpha = (col.x*(v2.y*v3.z-v3.y*v2.z) + col.y*(v3.x*v2.z-v2.x*v3.z) + col.z*(v2.x*v3.y-v3.x*v2.y))/den;
					beta = -(col.x*(v1.y*v3.z-v3.y*v1.z) + col.y*(v3.x*v1.z-v1.x*v3.z) + col.z*(v1.x*v3.y-v3.x*v1.y))/den;
//						float gamma = (col.x*(v1.y*v2.z-v2.y*v1.z) + col.y*(v2.x*v1.z-v1.x*v2.z) + col.z*(v1.x*v2.y-v2.x*v1.y))/(v1.x*(v2.y*v3.z-v3.y*v2.z) + v2.x*(v3.y*v1.z-v1.y*v3.z) + v3.x*(v1.y*v2.z-v2.y*v1.z));
					col.x = desdex+t*(ax-desdex);
					col.y = desdey+t*(ay-desdey);
					col.z = desdez+t*(az-desdez);
					if (alpha>=0.00001f /*&& alpha<=1.0f*/ && beta>=0.00001f /*&& beta<=1.0f */&& alpha+beta<=0.9999f)
						return &col;
				}
			} // if collided
		} // for faces
	}  // for tex
	return NULL;
}

struct bbox_t
{
	float top, bottom; // 0Y
	float left, right; // 0X
	float front, back; // 0Z
}; 

struct bbox_t **tri_bboxes;


int AABBTriCol(struct bbox_t box, struct VERTICE v0, struct VERTICE v1, struct VERTICE v2)
{	
	int res0,res1,res2,res3;	
	struct VERTICE m0,m1,m2;
	// there is any vertex inside the box?
	if ((v0.x>=box.left && v0.x<=box.right &&
		 v0.y>=box.bottom && v0.y<=box.top &&
		 v0.z>=box.front && v0.z<=box.back) ||
		(v1.x>=box.left && v1.x<=box.right &&
		 v1.y>=box.bottom && v1.y<=box.top &&
		 v1.z>=box.front && v1.z<=box.back) ||
		(v2.x>=box.left && v2.x<=box.right &&
		 v2.y>=box.bottom && v2.y<=box.top &&
		 v2.z>=box.front && v2.z<=box.back))
		return 1; // a vertex is inside the box

	if ((v0.x-v1.x)*(v0.x-v1.x)<=0.1f)
		if ((v1.x-v2.x)*(v1.x-v2.x)<=0.1f)
			if ((v0.y-v1.y)*(v0.y-v1.y)<=0.1f)
				if ((v1.y-v2.y)*(v1.y-v2.y)<=0.1f)
					if ((v0.z-v1.z)*(v0.z-v1.z)<=0.1f)
						if ((v1.z-v2.z)*(v1.z-v2.z)<=0.1f)
							return 0;

	// is the entire triangle on a side of he box?yes:don't collide
	if (v0.y > box.top && v1.y > box.top && v2.y > box.top)
		return 0;
	if (v0.y < box.bottom && v1.y < box.bottom && v2.y < box.bottom)
		return 0;
	if (v0.x > box.right && v1.x > box.right && v2.x > box.right)
		return 0;
	if (v0.x < box.left && v1.x < box.left && v2.x < box.left)
		return 0;
	if (v0.z > box.back && v1.z > box.back && v2.z > box.back)
		return 0;
	if (v0.z < box.front && v1.z < box.front && v2.z < box.front)
		return 0;
	
	m0.x=(v0.x+v1.x)/2.0f;
	m0.y=(v0.y+v1.y)/2.0f;
	m0.z=(v0.z+v1.z)/2.0f;	
	m1.x=(v1.x+v2.x)/2.0f;
	m1.y=(v1.y+v2.y)/2.0f;
	m1.z=(v1.z+v2.z)/2.0f;	
	m2.x=(v2.x+v0.x)/2.0f;
	m2.y=(v2.y+v0.y)/2.0f;
	m2.z=(v2.z+v0.z)/2.0f;	

	res0=AABBTriCol(box,v0,m0,m2);
	res1=AABBTriCol(box,m0,v1,m1);
	res2=AABBTriCol(box,m0,m1,m2);
	res3=AABBTriCol(box,m2,m1,v2);

	if (res0==0 && res1==0 && res2==0 && res3==0)
		return 0;
	
	return 1;
}


void GeneraOctree(struct NODO_OCTREE **o, struct bbox_t box, int maxpolis, int nummats, struct LISTA *l, int idp)
{
	static int id=0;
	int i,j;
	struct bbox_t b;
	struct POLI *nuevo,*ultimo;
	PRINTF("octree id: %d\n",id);
	fflush(stdout);
	octree_numnodos++;

	(*o)=(struct NODO_OCTREE*)malloc(sizeof(struct NODO_OCTREE));

	(*o)->id=id++;
	(*o)->idpadre=idp;

	(*o)->aba=box.bottom;
	(*o)->arr=box.top;
	(*o)->del=box.front;
	(*o)->der=box.right;
	(*o)->det=box.back;
	(*o)->izq=box.left;

	(*o)->hijos=(struct NODO_OCTREE**)malloc(sizeof(struct NODO_OCTREE*)*8);	
	for (i=0; i<8; i++)
		(*o)->hijos[i]=NULL;

	(*o)->polis=(struct POLI**)malloc(sizeof(struct POLI*)*nummats);
	(*o)->numpolis=(int*)malloc(sizeof(int)*nummats);
	for (i=0; i<nummats; i++)
		(*o)->numpolis[i]=0;
	(*o)->numtotalpolis=0;	
	for (i=0; i<nummats; i++)
	{
		(*o)->polis[i]=NULL;		
		ultimo=NULL;
		for (j=0; j<l[i].numc; j++)
		{
			if (AABBTriCol(box,verts[i][j*3+0],verts[i][j*3+1],verts[i][j*3+2]))
			{
				nuevo=(struct POLI*)malloc(sizeof(struct POLI));
				nuevo->ipoli=j;				
				nuevo->sig=NULL;

				if (ultimo==NULL)
				{
					(*o)->polis[i]=nuevo;
					ultimo=nuevo;
				}
				else
				{
					ultimo->sig=nuevo;
					ultimo=ultimo->sig;
				}

				(*o)->numpolis[i]++;
				(*o)->numtotalpolis++;
			}
		}
	}

	if ((*o)->numtotalpolis==0)
	{
		free((*o)->polis);
//		free((*o)->numpolis);
	}

	if ((*o)->numtotalpolis<=maxpolis) // limite de profundidad alcanzado
	{
		octree_numhojas++;
		return;
	}

	(*o)->hijos[0]=NULL;
	b.back=(box.front+box.back)/2;
	b.front=box.front;
	b.left=box.left;
	b.right=(box.left+box.right)/2;
	b.top=box.top;
	b.bottom=(box.bottom+box.top)/2;	
	GeneraOctree(&((*o)->hijos[0]),b,maxpolis,nummats,l,(*o)->id); // 0 // falla al llamar aki
	(*o)->hijos[1]=NULL;
	b.back=(box.front+box.back)/2;
	b.front=box.front;
	b.left=(box.left+box.right)/2;
	b.right=box.right;
	b.top=box.top;
	b.bottom=(box.bottom+box.top)/2;	
	GeneraOctree(&((*o)->hijos[1]),b,maxpolis,nummats,l,(*o)->id); // 1
	(*o)->hijos[2]=NULL;
	b.back=(box.front+box.back)/2;
	b.front=box.front;
	b.left=box.left;
	b.right=(box.left+box.right)/2;
	b.top=(box.top+box.bottom)/2;
	b.bottom=box.bottom;	
	GeneraOctree(&((*o)->hijos[2]),b,maxpolis,nummats,l,(*o)->id); // 2
	(*o)->hijos[3]=NULL;
	b.back=(box.front+box.back)/2;
	b.front=box.front;
	b.left=(box.left+box.right)/2;
	b.right=box.right;
	b.top=(box.top+box.bottom)/2;
	b.bottom=box.bottom;
	GeneraOctree(&((*o)->hijos[3]),b,maxpolis,nummats,l,(*o)->id); // 3
	(*o)->hijos[4]=NULL;
	b.back=box.back;
	b.front=(box.front+box.back)/2;
	b.left=box.left;
	b.right=(box.left+box.right)/2;
	b.top=box.top;
	b.bottom=(box.bottom+box.top)/2;
	GeneraOctree(&((*o)->hijos[4]),b,maxpolis,nummats,l,(*o)->id); // 4
	(*o)->hijos[5]=NULL;
	b.back=box.back;
	b.front=(box.front+box.back)/2;
	b.left=(box.left+box.right)/2;
	b.right=box.right;
	b.top=box.top;
	b.bottom=(box.bottom+box.top)/2;
	GeneraOctree(&((*o)->hijos[5]),b,maxpolis,nummats,l,(*o)->id); // 5
	(*o)->hijos[6]=NULL;
	b.back=box.back;
	b.front=(box.front+box.back)/2;
	b.left=box.left;
	b.right=(box.left+box.right)/2;
	b.top=(box.top+box.bottom)/2;
	b.bottom=box.bottom;
	GeneraOctree(&((*o)->hijos[6]),b,maxpolis,nummats,l,(*o)->id); // 6
	(*o)->hijos[7]=NULL;
	b.back=box.back;
	b.front=(box.front+box.back)/2;
	b.left=(box.left+box.right)/2;
	b.right=box.right;
	b.top=(box.top+box.bottom)/2;
	b.bottom=box.bottom;
	GeneraOctree(&((*o)->hijos[7]),b,maxpolis,nummats,l,(*o)->id); // 7
}
/*
void EscribeOctree(gzFile *fzip, struct NODO_OCTREE *o, int nummats)
{
	int i,j;
	float flo;
	struct POLI *aux;	
	if (o!=NULL)
	{
		PRINTF("Escribiendo nodo: %d.\n",o->id);
		gzwrite(fzip,&(o->id),4);
		gzwrite(fzip,&(o->idpadre),4);

		gzwrite(fzip,&(o->izq),4);
		gzwrite(fzip,&(o->der),4);

		gzwrite(fzip,&(o->del),4);
		gzwrite(fzip,&(o->det),4);
		
		flo=-(o->aba);
		gzwrite(fzip,&flo,4);
		flo=-(o->arr);
		gzwrite(fzip,&flo,4);

			for (i=0; i<nummats; i++)
			{
				//int jur=69;
				int res=gzwrite(fzip,&(o->numpolis[i]),4);
				if (o->numpolis[i]>0)
				{
					aux=o->polis[i];
					for (j=0; j<o->numpolis[i]; j++)
					{
						gzwrite(fzip,&(aux->ipoli),4);
						aux=aux->sig;
					}
				}
			}			
			EscribeOctree(fzip,o->hijos[0],nummats);
			EscribeOctree(fzip,o->hijos[1],nummats);
			EscribeOctree(fzip,o->hijos[2],nummats);
			EscribeOctree(fzip,o->hijos[3],nummats);
			EscribeOctree(fzip,o->hijos[4],nummats);
			EscribeOctree(fzip,o->hijos[5],nummats);
			EscribeOctree(fzip,o->hijos[6],nummats);
			EscribeOctree(fzip,o->hijos[7],nummats);
//		}
	}
}
*/

MSG msg;

int CALLBACK dialogfunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:		
		hDialog=hWnd;
		//SetWindowText(GetDlgItem(hWnd,IDC_EDIT_NUMTEX),"0");
		//SetWindowText(GetDlgItem(hWnd,IDC_EDIT_POLIGONOS),"0");
		break;
/*	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hWnd, 1);
			break;
		}
		break;
		default:
			return DefWindowProc(hWnd,msg,wParam,lParam);*/
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}       

bool ImportASE(LPCSTR fichASE, PRTMeshImpExpData *idata)
{
	FILE *fE, *fluz;
	FILE *fASE;
	char ficheroASE[MAXCHARLONG]="", ficheroE[MAXCHARLONG]="", cad[MAXCHARLONG]="", nombremapa[MAXCHARLONG]="";
	int numv, numc, i, j, k, ent, pos=0, numtv, nummats, ilightmap, estait;
	int pidat[1];
	char psdat[MAXCHARLONG]="", mat[MAXNUMTEXTURAS][MAXCHARLONG]; // si "(NINGUNA)" es ke no tiene bitmap
	float pfdat[1];
	struct LISTA l[MAXNUMTEXTURAS];
	struct OBJETO *o, *aux;
	struct VERTICE v, *vaux;
	struct CARA c, *caux, tc, *caux2;
	struct GLOBAL g;
	struct BTEX btex, btex_mat, btex_lines, btex_matref, btex_texcoord, btex_camara;
	struct LUZ luz, *laux;
	struct MAPOBJ mapobj, *maux;
	struct LINEA lin, *linaux;
	struct CAMARA *camaux;
//	int bjug1=0, bjug2=0;
	char caraux[2]="";
	int x,y,itri,lm_numtris,numlm;
	struct VERTICE v1, v2, v3, vd12, vd13, p, pres, vdpluz;
	float xaux,yaux,dist,modpluz,lm_unidad,xaux2,yaux2;
	int *num_lm_this_tex, *max_num_lm_this_tex;
	struct NODO_OCTREE *raiz;
	struct bbox_t bbox;
	int iver1=0,iver2=0,iver3=0;
	unsigned int octree_maxpolis=100;
	int bboxcol=0;
	struct VERTICE colortexel;
	int lm_x, lm_y, ***lm_vert_unido_con, **forma_cuad_con_el_sig, **num_tris_this_lm, **lm_cuadrados;
	int ***orden_vertices;
	unsigned char *buffer_texels;
	FILE *fdebug;
	FILE *ftextura;
	unsigned char *bufftex;

	//CreateDialog(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDD_DIALOG), NULL, dialogfunc);
	//SendMessage(GetDlgItem(hDialog,IDC_EDIT_RESUMEN),EM_LIMITTEXT,-1,0);

	//SetWindowText(GetDlgItem(hDialog,IDC_EDIT_FICHERO),fichASE);

	

	btex.n=6;
	strcpy(btex.cadcad[0], "*GEOMOBJECT");
	strcpy(btex.cadcad[1], "*CAMERAOBJECT");
	strcpy(btex.cadcad[2], "*MATERIAL_LIST");
	strcpy(btex.cadcad[3], "*LIGHTOBJECT");
	strcpy(btex.cadcad[4], "*HELPEROBJECT");
	strcpy(btex.cadcad[5], "*SHAPEOBJECT");

	btex_mat.n=2;
	strcpy(btex_mat.cadcad[0], "}");
	strcpy(btex_mat.cadcad[1], "*BITMAP");

	btex_lines.n=2;
	strcpy(btex_lines.cadcad[0],"*SHAPE_VERTEX_KNOT");
	strcpy(btex_lines.cadcad[1],"*SHAPE_VERTEX_INTERP");

	btex_matref.n=3;
	strcpy(btex_matref.cadcad[0],"*MATERIAL_REF");
	strcpy(btex_matref.cadcad[1],"*WIREFRAME_COLOR");
	strcpy(btex_matref.cadcad[2],"}");

	btex_texcoord.n=2;
	strcpy(btex_texcoord.cadcad[0],"*MESH_NUMTVERTEX");
	strcpy(btex_texcoord.cadcad[1],"*MESH_NORMALS");

	btex_camara.n=2;
	strcpy(btex_camara.cadcad[0],"*NODE_TM");
	strcpy(btex_camara.cadcad[1],"*CAMERA_SETTINGS");

	for (i=0; i<MAXNUMTEXTURAS; i++)
	{
		l[i].numo=0;
		l[i].numv=0;
		l[i].numc=0;
		l[i].numtv=0;
		l[i].prim=NULL;
		l[i].ult=NULL;
	}
	g.priml=NULL;
	g.ultl=NULL;
	g.numl=0;
	g.numo=0;
	g.numcam=0;
	g.primo=NULL;
	g.ulto=NULL;
	g.numlin=0;
	g.primlin=NULL;
	g.ultlin=NULL;
	g.primcam=NULL;
	g.ultcam=NULL;
	
	strcpy(ficheroASE,fichASE);
	
	fASE=fopen(ficheroASE, "r");
	if (fASE==NULL)
	{
		MessageBox(NULL,"ERROR: No se ha encontrado el fichero.\n",ficheroASE,MB_ICONSTOP);
		return false;
	}

	strcpy(ficheroE,ficheroASE);
	ficheroE[strlen(ficheroASE)-3]='\0';
	strcat(ficheroE,"sgemap");
	
	fE=fopen(ficheroE, "wb");
	
	TEXLUZ_ANCHO=128; // TEMPORALISSIIIIIM: HACERLO BIEN
	TEXLUZ_ALTO=TEXLUZ_ANCHO;
	
	TEXLUZ_TRIANGULO_ANCHO=8; // TEMPORALISSIIIIIM: HACERLO BIEN
	TEXLUZ_TRIANGULO_ALTO=TEXLUZ_TRIANGULO_ANCHO;

	octree_maxpolis=100;  // TEMPRALISSIIIIIM: HACERLO BIEN

	pos=0; // tu pa ke crees ke sirve esto?
	numv=0;
	numc=0;

	while(!feof(fASE))
	{		
		ent=BuscaTextoEx(fASE, btex, &pos);
		if (ent==-1)
			break;

		o=(struct OBJETO*)malloc(sizeof(struct OBJETO));	
		o->c=NULL;
		o->v=NULL;
		o->sig=NULL;
		o->ultc=NULL;
		o->ultv=NULL;
		o->tc=NULL;
		o->tv=NULL;
		o->ulttc=NULL;
		o->ulttv=NULL;
		o->nc=NULL;
		o->ultnc=NULL;
		o->nv=NULL;
		o->color.x=1.0f;
		o->color.y=1.0f;
		o->color.z=1.0f;

		v.sig=NULL;
		c.sig=NULL;
		tc.sig=NULL;
		tc.A=tc.B=tc.C=0;

		switch(ent)
		{
		case 0: // *GEOMOBJECT ////////////////////////////////////////////////////////////////
			pos+=11;
			fseek(fASE, SEEK_SET, pos);
			pos=BuscaTexto(fASE, "*NODE_NAME", pos);
			pos+=10;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%s", &nombremapa);
			PRINTF("\n[ %s ]\n", nombremapa);
			if (strcmp(nombremapa,"\"Plano02\"")==0)
				PRINTF("kjhb");

			for (i=1; i<(int)strlen(nombremapa); i++)
				nombremapa[i-1]=nombremapa[i];
			nombremapa[strlen(nombremapa)-2]='|'; // el caracter | marcara el fin de cadena

			o->nombre=(char*)malloc(sizeof(char)*strlen(nombremapa));
			strcpy(o->nombre, nombremapa);
			
			pos=BuscaTexto(fASE, "*MESH_NUMVERTEX", pos);
			pos+=15;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%d", &numv);
			o->numv=numv;

			pos=BuscaTexto(fASE, "*MESH_NUMFACES", pos);
			pos+=14;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%d", &numc);
			o->numc=numc;
			
			PRINTF("Numero de vertices: %d\n", numv);
			PRINTF("Numero de caras: %d\n", numc);

			PRINTF("vertices\n");
			pos=BuscaTexto(fASE, "*MESH_VERTEX_LIST", pos);
			pos+=17;
			fseek(fASE, SEEK_SET, pos);
			for (i=0; i<o->numv; i++)
			{
				pos=BuscaTexto(fASE, "*MESH_VERTEX", pos);
				pos+=12;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &ent);

				fscanf(fASE, "%s", &cad); // X
				*pfdat=(float)atof(cad);
				v.x=*pfdat;

				fscanf(fASE, "%s", &cad); // Y
				*pfdat=(float)atof(cad);
				v.y=*pfdat;

				fscanf(fASE, "%s", &cad); // Z
				*pfdat=(float)atof(cad);
				v.z=*pfdat;
			
				agregarv(o, &v);
//				PRINTF(".");		
			}

			PRINTF("caras\n");
			pos=BuscaTexto(fASE, "*MESH_FACE_LIST", pos);
			pos+=15;
			fseek(fASE, SEEK_SET, pos);
			for (i=0; i<numc; i++)
			{
				pos=BuscaTexto(fASE, "*MESH_FACE", pos);
				pos+=10;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &ent);

				pos=BuscaTexto(fASE, "A:", pos);
				pos+=2;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &pidat);
				c.A=*pidat;

				pos=BuscaTexto(fASE, "B:", pos);
				pos+=2;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &pidat);
				c.B=*pidat;

				pos=BuscaTexto(fASE, "C:", pos);
				pos+=2;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &pidat);
				c.C=*pidat;

				agregarc(o, &c);
//				PRINTF(".");		
			}

			if (BuscaTextoEx(fASE,btex_texcoord,&pos)==0) // si hay informacion sobre los vertices de mapeado
			{
				PRINTF("Cordenadas de mapeado (vertices)\n");
				pos=BuscaTexto(fASE, "*MESH_NUMTVERTEX", pos);
				pos+=16;
				fscanf(fASE,"%d",&numtv);
				*pidat=numtv;
				o->numtv=numtv;
				o->tv=NULL;
				o->tc=NULL;
				
				if (numtv==0)
					goto nomapinfo;

				pos=BuscaTexto(fASE, "*MESH_TVERTLIST", pos);
				pos+=15;
				for (i=0; i<numtv; i++)
				{
					pos=BuscaTexto(fASE, "*MESH_TVERT", pos);
					pos+=11;
					fscanf(fASE,"%d",&ent);

					fscanf(fASE,"%s",&cad);
					*pfdat=(float)atof(cad);
					v.x=*pfdat;

					fscanf(fASE,"%s",&cad);
					*pfdat=(float)atof(cad);
					v.y=*pfdat;

					fscanf(fASE,"%s",&cad);
					*pfdat=(float)atof(cad);
					v.z=*pfdat;

					agregartv(o, &v);
	//				PRINTF(".");
				}

				PRINTF("coordenadas de mapeado (caras)\n");
				pos=BuscaTexto(fASE, "*MESH_TFACELIST", pos);
				pos+=15;
				fseek(fASE, SEEK_SET, pos);
				for (i=0; i<numc; i++)
				{
					pos=BuscaTexto(fASE, "*MESH_TFACE", pos);
					pos+=11;
					fseek(fASE, SEEK_SET, pos);
					fscanf(fASE, "%d", &ent);

					fscanf(fASE, "%d", &pidat);
					c.A=*pidat;

					fscanf(fASE, "%d", &pidat);
					c.B=*pidat;

					fscanf(fASE, "%d", &pidat);
					c.C=*pidat;
					c.sig=0x00000000;

					agregartc(o, &c);
	//				PRINTF(".");		
				}
			}
			else // no hay informacion sobre el mapeado: pos me lo invento
			{
nomapinfo:		o->numtv=0;
				o->tc=NULL;
				o->tv=NULL;
			}

			PRINTF("normales\n");
			o->nv=(float*)malloc(sizeof(float)*(o->numc)*3*3);
			caux=o->c;
			for (i=0; i<o->numc; i++)
			{		
				caux->suavizada=0;
				pos=BuscaTexto(fASE, "*MESH_FACENORMAL", pos);
				pos+=16;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &ent);

				fscanf(fASE, "%s", &cad);
				*pfdat=(float)atof(cad);
				v.x=*pfdat;

				fscanf(fASE, "%s", &cad);
				*pfdat=(float)atof(cad);
				v.y=*pfdat;

				fscanf(fASE, "%s", &cad);
				*pfdat=(float)atof(cad);
				v.z=*pfdat;

				agregarnc(o, &v); // no petar el contenido de v pq se usa bajo para mallaSuavizada
				for (j=0; j<3; j++)
				{
					pos=BuscaTexto(fASE, "*MESH_VERTEXNORMAL", pos);
					pos+=18;
					fseek(fASE, SEEK_SET, pos);
					fscanf(fASE, "%d", &ent);

					fscanf(fASE, "%s", &cad);
					*(o->nv+(i*9)+(j*3)+0)=(float)atof(cad);					
					fscanf(fASE, "%s", &cad);
					*(o->nv+(i*9)+(j*3)+1)=(float)atof(cad);
					fscanf(fASE, "%s", &cad);
					*(o->nv+(i*9)+(j*3)+2)=(float)atof(cad);
					// comparar la normal de cara con las de vertice para
					// ver si la malla esta suavizada o no
					// para usar o normales de cara o de vertice
					if (v.x==0.0f && v.y==0.0f && v.z==0.0f)
					{
						v.x=*(o->nv+(i*9)+(j*3)+0);
						v.y=*(o->nv+(i*9)+(j*3)+1);
						v.z=*(o->nv+(i*9)+(j*3)+2);
					}
					if (v.x != *(o->nv+(i*9)+(j*3)+0) ||
						v.y != *(o->nv+(i*9)+(j*3)+1) ||
						v.z != *(o->nv+(i*9)+(j*3)+2))
						caux->suavizada=1;
				}

//				PRINTF(".");		
				caux=caux->sig;
			}	

			pos=BuscaTexto(fASE, "*PROP_MOTIONBLUR", pos);
			pos+=16;
			fseek(fASE, SEEK_SET, pos);		
			switch(BuscaTextoEx(fASE, btex_matref, &pos)) // que textura usa
			{
			case 0: pos+=13; 
					fseek(fASE, SEEK_SET, pos);
					fscanf(fASE, "%d", &(o->texid));
					break;
			case 1:	pos+=16;
					fseek(fASE, SEEK_SET, pos);
					fscanf(fASE, "%s", &cad);
					o->color.x=(float)atof(cad);
					fscanf(fASE, "%s", &cad);
					o->color.y=(float)atof(cad);
					fscanf(fASE, "%s", &cad);
					o->color.z=(float)atof(cad);
					o->texid=nummats-1;
					break;
			case 2: pos+=1; 
					o->texid=nummats-1; 
					break;
			}

//			if (o->numtv==0)
//				o->texid=nummats-1;
			char bufbuf[16];
			//GetWindowText(GetDlgItem(hDialog,IDC_EDIT_POLIGONOS),bufbuf,20);
			//SetWindowText(GetDlgItem(hDialog,IDC_EDIT_POLIGONOS),itoa(atoi(bufbuf)+o->numc,bufbuf,10));
			UpdateWindow(hDialog);
			
			agregar(&(l[o->texid]), o);
//			free(o->nombre);
			o->nombre=NULL;
			PRINTF("Okis");
			break;


		case 1: // *CAMERAOBJECT //////////////////////////////////////////////////////////////
			pos+=13;
			fseek(fASE, SEEK_SET, pos);
//			pos=BuscaTexto(fASE, "*NODE_NAME", pos);
//			pos+=10;
//			gzseek(fASE, SEEK_SET, pos);
//			gzread(fASE, "%s", cad);
//			for (i=0; i<(int)strlen(cad); i++) // kitar las comillas
//				cad[i]=cad[i+1];
//			
//			cad[i-1]='\0';
//			switch(atoi(cad))
//			{
//			case 0: // Camara principal ///////////////

				camaux=(struct CAMARA*)malloc(sizeof(struct CAMARA));
				PRINTF("\n[ Camara ]\n");
				pos=BuscaTexto(fASE, "*NODE_NAME", pos);
				pos+=10;
				camaux->nombre=(char*)malloc(128);
				fscanf(fASE, "%s", camaux->nombre);				
				pos=BuscaTexto(fASE, "*TM_POS", pos);
				pos+=7;
				fseek(fASE, SEEK_SET, pos);

				fscanf(fASE, "%s", cad);
				camaux->pos.x=(float)atof(cad);				
				fscanf(fASE, "%s", cad);
				camaux->pos.y=(float)atof(cad);				
				fscanf(fASE, "%s", cad);
				camaux->pos.z=(float)atof(cad);				


				pos=BuscaTexto(fASE, "*TM_ROTANGLE", pos);
				pos+=12;
				fseek(fASE, SEEK_SET, pos);

				fscanf(fASE, "%s", cad);
				camaux->angrot=(float)atof(cad);				

				if (BuscaTextoEx(fASE,btex_camara,&pos)==0)
				{
					pos=BuscaTexto(fASE, "*TM_POS", pos); // a donde mira el jugador
					pos+=7;
					fseek(fASE, SEEK_SET, pos);
					fscanf(fASE, "%s", cad);
					camaux->mira.x=(float)atof(cad);
					fscanf(fASE, "%s", cad);
					camaux->mira.y=(float)atof(cad);
					fscanf(fASE, "%s", cad);
					camaux->mira.z=(float)atof(cad);
				}

				pos=BuscaTexto(fASE, "*CAMERA_FOV", pos); 
				pos+=11;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%s", cad);
				camaux->fov=(float)atof(cad);
				camaux->sig=NULL;
				agregarcam(&g,camaux);
				
				PRINTF("Okis");
//				break;
/*			case 1: // MOSTRO //////////////////
				PRINTF("\n[ Mostro ]\n");
				p.id=1;
				pos=BuscaTexto(fASE, "*TM_POS", pos);
				pos+=7;
				gzseek(fASE, SEEK_SET, pos);

				gzread(fASE, "%s", cad);
				p.pos.x =(float)atof(cad);				
				gzread(fASE, "%s", cad);
				p.pos.y =(float)atof(cad);				
				gzread(fASE, "%s", cad);
				p.pos.z =(float)atof(cad);				

				pos=BuscaTexto(fASE, "*TM_POS", pos); // a donde mira el jugador
				pos+=7;
				gzseek(fASE, SEEK_SET, pos);

				gzread(fASE, "%s", cad);
				p.mira.x=(float)atof(cad);
				gzread(fASE, "%s", cad);
				p.mira.y=(float)atof(cad);
				gzread(fASE, "%s", cad);
				p.mira.z=(float)atof(cad);

				p.sig=NULL;

				agregarp(&(l[0]), &p);
				PRINTF("Okis"); 
				break; */
//			}
			break;
		case 2: // *MATERIAL_LIST /////////////////////////////////////////////////////////////
			PRINTF("\n[ Materiales ]\n");
			pos+=14;
			fseek(fASE, SEEK_SET, pos);
			pos=BuscaTexto(fASE, "*MATERIAL_COUNT", pos);
			pos+=15;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%d", &nummats);
			for (i=0; i<nummats; i++)
			{
				pos=BuscaTexto(fASE, "*MATERIAL ", pos);
				pos+=10;
				fseek(fASE, SEEK_SET, pos);
				fscanf(fASE, "%d", &j);
				ent=BuscaTextoEx(fASE, btex_mat, &pos);
				switch(ent)
				{
				case 0: // }
					strcpy(mat[j],"(NINGUNA)");
					pos+=1;
					break;
				case 1: // *BITMAP
					pos+=7;
					fseek(fASE, SEEK_SET, pos);
					caraux[0]=fgetc(fASE);
					caraux[0]=fgetc(fASE);
	//				gzread(fASE, "\"%s\"", cad);
					strcpy(cad,"");
					while((caraux[0]=fgetc(fASE))!='\"')
					{
						strcat(cad, caraux);
					}
					for (k=MAXCHARLONG-1; k>=0; k--)
						if (cad[k]=='\\') // '\\' simboliza el caracter '\'
							break;
					for (ent=0; ent<(int)strlen(cad); ent++)
						cad[ent]=cad[ent+k+1];
					cad[strlen(cad)]='\0';
					strcpy(mat[j], cad);
					break;
				case -1: // no se ha encontrado ninguna					
					break;
				}
			}
			
			//SetWindowText(GetDlgItem(hDialog,IDC_EDIT_NUMTEX),itoa(nummats,bufbuf,10));
			UpdateWindow(hDialog);

			nummats++; // REVISAR ESTOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!!!!!!!!!!!!1 para los ke no tienen bitmap
//			mats_real[nummats-mats_aux-1]=j+1;
			strcpy(mat[nummats-1],"(NINGUNA)");

			// arreglar el array de materiales
/*			for (j=0; j<nummats; j++)
			{
				if (mats_real[j]>0 && mats_real[j]<nummats)
				{
					strcpy(cad,mat[mats_real[j]]);
					strcpy(mat[j],cad);
				}
			}

			nummats-=mats_aux;
*/
			PRINTF("Okis");
			break;
		case 3: // *LIGHTOBJECT /////////////////////////////////////////////////////////////
			PRINTF("\n[ Luz ]\n");
			pos+=12;
			fseek(fASE, SEEK_SET, pos);
//			pos=BuscaTexto(fASE, "*LIGHT_TYPE", pos);
//			pos+=11;
//			gzseek(fASE, SEEK_SET, pos);
//			gzread(fASE, "%d", &nummats);
			// por ahora el tipo de luz será siempre OMNI
			luz.tipo=LUZ_OMNI;
			pos=BuscaTexto(fASE, "*TM_POS", pos);
			pos+=7;
			fseek(fASE, SEEK_SET, pos);
			for (i=0; i<3; i++)
			{
				fscanf(fASE, "%s", cad);
				luz.pos[i]=(float)atof(cad);				
			}
			pos=BuscaTexto(fASE, "*LIGHT_COLOR", pos);
			pos+=12;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%s", cad);  // R
			luz.color[0]=(float)atof(cad);
			fscanf(fASE, "%s", cad);  // G
			luz.color[1]=(float)atof(cad);
			fscanf(fASE, "%s", cad);  // B
			luz.color[2]=(float)atof(cad);

			pos=BuscaTexto(fASE, "*LIGHT_INTENS", pos);
			pos+=13;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%s", cad);
			luz.intensidad=(float)atof(cad);
			luz.sig=NULL;
			
			agregarl(&g, &luz);
			PRINTF("Okis");
			break;
		
		case 4: // *HELPEROBJECT ////////////////////////
			PRINTF("\n[ Objeto ]\n");
			pos+=13;
			fseek(fASE, SEEK_SET, pos);
			pos=BuscaTexto(fASE, "*NODE_NAME", pos);
			pos+=10;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%s", cad);

			mapobj.nombre=(char*)malloc(strlen(cad)-1); // no hay ke poner las comillas			
			for (i=1; i<strlen(cad)-1; i++)
				mapobj.nombre[i-1]=cad[i];
			mapobj.nombre[i-1]='\0';


/*			if (strcmp(cad,"\"1\"")!=0 && strcmp(cad,"\"2\"")!=0)
			{
				PRINTF("!!! ERROR ¡¡¡: El Nombre del helperobject (ayudante) debe ser 1 o 2\n");
				//getch();
				exit(0);
			}
			if (strcmp(cad,"\"1\"")==0)
			{
				PRINTF("1 ]\n");
				pos=BuscaTexto(fASE, "*TM_POS", pos);
				pos+=7;
				fseek(fASE, SEEK_SET, pos);
				for (i=0; i<3; i++)
				{
					fscanf(fASE, "%s", cad);
					g.posjug1[i]=(float)atof(cad);
				}
				PRINTF("Okis");
				//bjug1=1;
			}
			if (strcmp(cad,"\"2\"")==0)
			{
				PRINTF("2 ]\n");
				pos=BuscaTexto(fASE, "*TM_POS", pos);
				pos+=7;
				fseek(fASE, SEEK_SET, pos);
				for (i=0; i<3; i++)
				{
					fscanf(fASE, "%s", cad);
					g.posjug2[i]=(float)atof(cad);
				}
				PRINTF("Okis");
				//bjug2=1;
			}*/

			pos=BuscaTexto(fASE, "*TM_POS", pos);
			pos+=7;
			fseek(fASE, SEEK_SET, pos);
			for (i=0; i<3; i++)
			{
				fscanf(fASE, "%s", cad);
				mapobj.pos[i]=(float)atof(cad);
			}

			mapobj.sig=NULL;
			agregarmo(&g,&mapobj);
			PRINTF("Okis");

			break;

		case 5: // *SHAPEOBJECT ////////////////////////
			pos+=12;
			pos=BuscaTexto(fASE, "*NODE_NAME", pos);
			pos+=10;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%s", cad);
			PRINTF("\n[ Linea (%s) ]\n",cad);

			lin.nombre=(char*)malloc(strlen(cad)-1); // no hay ke poner las comillas			
			for (i=1; i<strlen(cad)-1; i++)
				lin.nombre[i-1]=cad[i];
			lin.nombre[i-1]='\0';

//			fseek(fASE, SEEK_SET, pos);
			pos=BuscaTexto(fASE,"*SHAPE_LINECOUNT", pos);
			pos+=16;
			fseek(fASE,SEEK_SET,pos);
			fscanf(fASE, "%d", &ent);
			if (ent==0)
				break;
			pos=BuscaTexto(fASE, "*SHAPE_VERTEXCOUNT", pos);
			pos+=18;
			fseek(fASE, SEEK_SET, pos);
			fscanf(fASE, "%d", &lin.numv);
			lin.v=(struct VERTICE*)malloc(sizeof(struct VERTICE)*lin.numv);

			for (i=0; i<lin.numv; i++)
			{
				switch(BuscaTextoEx(fASE, btex_lines, &pos))
				{
				case 0: // *SHAPE_VERTEX_KNOT
					pos+=18;
					break;
				case 1: // *SHAPE_VERTEX_INTERP
					pos+=20;
					break;
				}

				fseek(fASE, SEEK_SET, pos);

				fscanf(fASE, "%d", &ent);
				if (i!=ent)
				{
					PRINTF("\n\n\n¡¡ Error leyendo linea, posible fichero corrupto. (i!=ent) !! \nPulsa cualquier tecla para continuar...\n\n");
			//		getch();
					return false;
				}
				
				fscanf(fASE, "%s", cad);
				lin.v[i].x=(float)atof(cad);
				fscanf(fASE, "%s", cad);
				lin.v[i].y=(float)atof(cad);
				fscanf(fASE, "%s", cad);
				lin.v[i].z=(float)atof(cad);

			}

			lin.sig=NULL;
			agregarlin(&g,&lin);
			PRINTF("Okis");

			break;

		}
	}


	////////// escribir todos los datos leidos en el la estructura de datos ///////////////
	printf("\n\n [ Calculando lista de vertices ]");
	verts=(struct VERTICE **)malloc(sizeof(struct VERTICE *)*nummats);
	for (j=0; j<nummats; j++)
	{
		verts[j]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[j].numc*3);
		k=0;
		ent=0;
		aux=l[j].prim;

		while(aux!=NULL)
		{
			for (i=ent; i<aux->numc+ent; i++)
			{
				caux=DaCara((&l[j]), i);
				verts[j][i*3+0].x=DaVertice((&l[j]), caux->A + k)->x;
				verts[j][i*3+0].y=DaVertice((&l[j]), caux->A + k)->y;
				verts[j][i*3+0].z=DaVertice((&l[j]), caux->A + k)->z;
				verts[j][i*3+1].x=DaVertice((&l[j]), caux->B + k)->x;
				verts[j][i*3+1].y=DaVertice((&l[j]), caux->B + k)->y;
				verts[j][i*3+1].z=DaVertice((&l[j]), caux->B + k)->z;
				verts[j][i*3+2].x=DaVertice((&l[j]), caux->C + k)->x;
				verts[j][i*3+2].y=DaVertice((&l[j]), caux->C + k)->y;
				verts[j][i*3+2].z=DaVertice((&l[j]), caux->C + k)->z;
			}
			k+=aux->numv;
			ent+=aux->numc;
			aux=aux->sig;
		}
	}


	PRINTF("\n [ Calculando normales de cara ]");
	ncara=(struct VERTICE**)malloc(nummats*sizeof(struct VERTICE*));

	// calcular todas las normales de cara
	for (j=0; j<nummats; j++)
	{
		ncara[j]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[j].numc);
		aux=l[j].prim;
		ent=0;
		while(aux!=NULL)
		{			
			for (k=ent; k<aux->numc+ent;  k++)
			{
				v1.x=verts[j][k*3+1].x-verts[j][k*3+0].x;
				v1.y=verts[j][k*3+1].y-verts[j][k*3+0].y;				
				v1.z=verts[j][k*3+1].z-verts[j][k*3+0].z;
				v2.x=verts[j][k*3+2].x-verts[j][k*3+0].x;
				v2.y=verts[j][k*3+2].y-verts[j][k*3+0].y;
				v2.z=verts[j][k*3+2].z-verts[j][k*3+0].z;
				v1=Normalizar(v1);
				v2=Normalizar(v2);
				v3=ProdVect(v1,v2);
				ncara[j][k]=Normalizar(v3); // v3 = normal de cara
			}
			ent+=aux->numc;
			aux=aux->sig;			
		}
	}

	PRINTF("\n [ Calculando normales de vertice ]");
	// calcular las normales de cada vertice
	nvert=(struct VERTICE**)malloc(sizeof(struct VERTICE*)*nummats);
	for (j=0; j<nummats; j++)
	{
		nvert[j]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[j].numc*3);
		aux=l[j].prim;
		ent=0;
		while(aux!=NULL)
		{
			caux=aux->c;
			for (k=ent; k<aux->numc+ent;  k++)
			{
				if (caux->suavizada)
				{
					// encontrar todas las caras que comparten este vértice
					// SOLO BUSCARA CARAS DE ESTE MISMO OBJETO					
					nvert[j][k*3+0].x=0.0f;
					nvert[j][k*3+0].y=0.0f;
					nvert[j][k*3+0].z=0.0f;
					nvert[j][k*3+1].x=0.0f;
					nvert[j][k*3+1].y=0.0f;
					nvert[j][k*3+1].z=0.0f;
					nvert[j][k*3+2].x=0.0f;
					nvert[j][k*3+2].y=0.0f;
					nvert[j][k*3+2].z=0.0f;

					v1.x=0.0f; // v1 guardara el numero de normales en la suma para la media
					v1.y=0.0f;
					v1.z=0.0f;
					iver1=caux->A; // iver1 es el vertice cuya normal kiero calcular
					iver2=caux->B;
					iver3=caux->C;

					caux2=aux->c;
					for (i=ent; i<aux->numc+ent; i++) // comparar con las otras caras del objeto
					{							
						if (caux2->A==iver1 || caux2->B==iver1 || caux2->C==iver1)
						{
							nvert[j][k*3+0].x+=ncara[j][i].x;
							nvert[j][k*3+0].y+=ncara[j][i].y;
							nvert[j][k*3+0].z+=ncara[j][i].z;
							v1.x++;
						}
						if (caux2->A==iver2 || caux2->B==iver2 || caux2->C==iver2)
						{
							nvert[j][k*3+1].x+=ncara[j][i].x;
							nvert[j][k*3+1].y+=ncara[j][i].y;
							nvert[j][k*3+1].z+=ncara[j][i].z;
							v1.y++;
						}
						if (caux2->A==iver3 || caux2->B==iver3 || caux2->C==iver3)
						{
							nvert[j][k*3+2].x+=ncara[j][i].x;
							nvert[j][k*3+2].y+=ncara[j][i].y;
							nvert[j][k*3+2].z+=ncara[j][i].z;
							v1.z++;
						}
						caux2=caux2->sig;
					}
					nvert[j][k*3+0].x/=v1.x;
					nvert[j][k*3+0].y/=v1.x;
					nvert[j][k*3+0].z/=v1.x;
					nvert[j][k*3+1].x/=v1.y;
					nvert[j][k*3+1].y/=v1.y;
					nvert[j][k*3+1].z/=v1.y;
					nvert[j][k*3+2].x/=v1.z;
					nvert[j][k*3+2].y/=v1.z;
					nvert[j][k*3+2].z/=v1.z;
					nvert[j][k*3+0]=Normalizar(nvert[j][k*3+0]);
					nvert[j][k*3+1]=Normalizar(nvert[j][k*3+1]);
					nvert[j][k*3+2]=Normalizar(nvert[j][k*3+2]);
					caux=caux->sig;
				}
				else
				{
					nvert[j][k*3+0]=ncara[j][k];
					nvert[j][k*3+1]=ncara[j][k];
					nvert[j][k*3+2]=ncara[j][k];
					caux=caux->sig;
				}
			}
			ent+=aux->numc;
			aux=aux->sig;			
		}
	}


	// ATENCION!!! l[j].numv parace estar corrupto: creo ke no muestra el numero de vertices
	PRINTF("\n\nExportando...");
	idata->nummesh=nummats;
	idata->geomdat=new PRTMeshImpExpObject[nummats];
	PRINTF("\n - Coordenadas de vertice");
	for (j=0; j<nummats; j++)
	{
		idata->geomdat[j].numv = l[j].numc*3;
		idata->geomdat[j].vertexes = new PRTMeshImpExpVertex[l[j].numc*3];

		for (i=0; i<l[j].numc*3; i++)
		{
			idata->geomdat[j].vertexes[i].x=verts[j][i].x;
			idata->geomdat[j].vertexes[i].y=verts[j][i].y;
			idata->geomdat[j].vertexes[i].z=verts[j][i].z;
		}
	}

	// Escribir los vertices de mapeado
	// primero los ke tienen textura
	PRINTF("\n - Coordenadas de textura");
	for (j=0; j<nummats; j++)
	{
		if (l[j].numtv==0)
		{
			aux=l[j].prim;
			while(aux!=NULL)
			{
				for (i=0; i<aux->numc; i++)
				{					
					idata->geomdat[j].vertexes[i*3+0].u=0.0f;
					idata->geomdat[j].vertexes[i*3+0].v=0.0f;
					idata->geomdat[j].vertexes[i*3+1].u=0.0f;
					idata->geomdat[j].vertexes[i*3+1].v=0.0f;
					idata->geomdat[j].vertexes[i*3+2].u=0.0f;
					idata->geomdat[j].vertexes[i*3+2].v=0.0f;
				}
				aux=aux->sig;
			}
			continue;
		}

		k=0;
		ent=0;
		aux=l[j].prim;
		while(aux!=NULL)
		{
			for (i=ent; i<aux->numc+ent; i++)
			{
				caux=DaCaraMapeado(&l[j], i);
				vaux=DaVerticeMapeado(&l[j], caux->A + k);
				idata->geomdat[j].vertexes[i*3+0].u=vaux->x;				
				idata->geomdat[j].vertexes[i*3+0].v=vaux->y;
				vaux=DaVerticeMapeado(&l[j], caux->B + k);
				idata->geomdat[j].vertexes[i*3+1].u=vaux->x;				
				idata->geomdat[j].vertexes[i*3+1].v=vaux->y;
				vaux=DaVerticeMapeado(&l[j], caux->C + k);
				idata->geomdat[j].vertexes[i*3+2].u=vaux->x;				
				idata->geomdat[j].vertexes[i*3+2].v=vaux->y;			
			}

			k+=aux->numtv;
			ent+=aux->numc;
			aux=aux->sig;
		}
	}


	// escribir las normales de cada vertice	
	PRINTF("\n - Normales de vertice");
	for (i=0; i<nummats; i++)
	{
		for (j=0; j<l[i].numc*3; j++)
		{
			idata->geomdat[i].vertexes[j].nx=nvert[i][j].x;
			idata->geomdat[i].vertexes[j].ny=nvert[i][j].z;
			idata->geomdat[i].vertexes[j].nz=-nvert[i][j].y;
		}
	}

	// Liberar toda la memoria intermedia usada
	PRINTF("\n\n\n Liberando memoria intermedia...\n");

	for (i=0; i<nummats; i++)
	{
		aux=l[i].prim;
		// liberar todos los objetos de las listas
		while(aux)
		{
			// liberar todas las caras del objeto			
			caux=aux->c;			
			while(caux)
			{
				CARA *cauxsig=caux->sig;
				free(caux);		
				caux=cauxsig;
			}			

			// liberar todas las caras de mapeado del objeto			
			caux=aux->tc;			
			while(caux)
			{
				CARA *cauxsig=caux->sig;
				free(caux);		
				caux=cauxsig;
			}			

			// liberar todas las normales de cara del objeto			
			vaux=aux->nc;			
			while(vaux)
			{
				VERTICE *vauxsig=vaux->sig;
				free(vaux);		
				vaux=vauxsig;
			}

			// liberar todos los vertices del objeto
			vaux=aux->v;			
			while(vaux)
			{
				VERTICE *vauxsig=vaux->sig;
				free(vaux);		
				vaux=vauxsig;
			}
						
			// liberar todos los vertices de mapeado del objeto			
			vaux=aux->tv;			
			while(vaux)
			{
				VERTICE *vauxsig=vaux->sig;
				free(vaux);		
				vaux=vauxsig;
			}
			
			// liberar todas las normales de vertice del objeto			
			free(aux->nv);
			
			// liberar el objeto
			OBJETO *auxsig=aux->sig;
			free(aux);
			aux=auxsig;
		}
	}

	// si se ha creado el dialogo, se cierra
	if (hDialog)
		EndDialog(hDialog,0);
	
	return true;
}
	// FIN DE LA FUNCION LO SIGUIENTE ES LO QUE SOBRABA

	///////////////////////////////////////////////////////////////////////////////////
	// NO SE VA A ESCRIBIR NADA EN NINGUN FICHERO EN ESTE PLUGIN
	// TODOS LOS DATOS SE PASARAN HACIA EL EXTERIOR A TRAVES DE LA ESTRUCTURA DADA COMO PARAMETRO
	// ¡¡¡ LA PARTE DEL PLUGIN QUE ESCRIBE A UN FICHEROE STARA TODA COMENTADA !!!
	////////////////////////////////////////////////////////////////////////////////////

	// ESCRIBIR VERSION
/*	ent=SGEMAPVER;
	gzwrite(fE,&ent,4);


	PRINTF("\n\nEscribiendo datos...");
	gzwrite(fE, &nummats, 4); // num de texturas

//	*pidat=0;
//	for (i=0; i<nummats; i++)
//		*pidat+=l[i].numc*3; // numero de vertices ke forman la secuencia de vertices de la malla
//	gzwrite(pidat, 4, 1, fE); // num vert

	PRINTF("\n - Numero de vertices");
	for (i=0; i<nummats; i++)
	{
		*pidat=l[i].numc*3;
		gzwrite(fE, pidat, 4);
	}


	// Escribir los vertices
	PRINTF("\n - Vertices");
	for (j=0; j<nummats; j++)
	{
		k=0;
		ent=0;
		aux=l[j].prim;
		while(aux!=NULL)
		{
			for (i=ent; i<aux->numc+ent; i++)
			{
				caux=DaCara((&l[j]), i);
				vaux=DaVertice((&l[j]), caux->A + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);
				*pfdat=-vaux->y;
				gzwrite(fE, pfdat, 4);

				vaux=DaVertice((&l[j]), caux->B + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);
				*pfdat=-vaux->y;
				gzwrite(fE, pfdat, 4);

				vaux=DaVertice((&l[j]), caux->C + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);
				*pfdat=-vaux->y;
				gzwrite(fE, pfdat, 4);
//				PRINTF(".");
			}
			k+=aux->numv;
			ent+=aux->numc;
			aux=aux->sig;
		}
	}


	// Escribir los vertices de mapeado
	// primero los ke tienen textura
	PRINTF("\n - Coordenadas de textura");
	for (j=0; j<nummats; j++)
	{
		if (l[j].numtv==0)
		{
			aux=l[j].prim;
			while(aux!=NULL)
			{
				for (i=0; i<aux->numc; i++)
				{
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);

					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);

					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
					*pfdat=0.0f;
					gzwrite(fE, pfdat, 4);
				}
				aux=aux->sig;
			}
			continue;
		}

		k=0;
		ent=0;
		aux=l[j].prim;
		while(aux!=NULL)
		{
			for (i=ent; i<aux->numc+ent; i++)
			{
				caux=DaCaraMapeado(&l[j], i);
				vaux=DaVerticeMapeado(&l[j], caux->A + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->y;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);

				vaux=DaVerticeMapeado(&l[j], caux->B + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->y;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);

				vaux=DaVerticeMapeado(&l[j], caux->C + k);
				*pfdat=vaux->x;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->y;
				gzwrite(fE, pfdat, 4);
				*pfdat=vaux->z;
				gzwrite(fE, pfdat, 4);
//				PRINTF(".");
			}

			k+=aux->numtv;
			ent+=aux->numc;
			aux=aux->sig;
		}
	}


	PRINTF("\n [ Calculando normales de cara ]");
	ncara=(struct VERTICE**)malloc(nummats*sizeof(struct VERTICE*));

	// calcular todas las normales de cara
	for (j=0; j<nummats; j++)
	{
		ncara[j]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[j].numc);
		aux=l[j].prim;
		ent=0;
		while(aux!=NULL)
		{			
			for (k=ent; k<aux->numc+ent;  k++)
			{
				v1.x=verts[j][k*3+1].x-verts[j][k*3+0].x;
				v1.y=verts[j][k*3+1].y-verts[j][k*3+0].y;				
				v1.z=verts[j][k*3+1].z-verts[j][k*3+0].z;
				v2.x=verts[j][k*3+2].x-verts[j][k*3+0].x;
				v2.y=verts[j][k*3+2].y-verts[j][k*3+0].y;
				v2.z=verts[j][k*3+2].z-verts[j][k*3+0].z;
				v1=Normalizar(v1);
				v2=Normalizar(v2);
				v3=ProdVect(v1,v2);
				ncara[j][k]=Normalizar(v3); // v3 = normal de cara
			}
			ent+=aux->numc;
			aux=aux->sig;			
		}
	}

	PRINTF("\n [ Calculando normales de vertice ]");
	// calcular las normales de cada vertice
	nvert=(struct VERTICE**)malloc(sizeof(struct VERTICE*)*nummats);
	for (j=0; j<nummats; j++)
	{
		nvert[j]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[j].numc*3);
		aux=l[j].prim;
		ent=0;
		while(aux!=NULL)
		{
			caux=aux->c;
			for (k=ent; k<aux->numc+ent;  k++)
			{
				if (caux->suavizada)
				{
					// encontrar todas las caras que comparten este vértice
					// SOLO BUSCARA CARAS DE ESTE MISMO OBJETO					
					nvert[j][k*3+0].x=0.0f;
					nvert[j][k*3+0].y=0.0f;
					nvert[j][k*3+0].z=0.0f;
					nvert[j][k*3+1].x=0.0f;
					nvert[j][k*3+1].y=0.0f;
					nvert[j][k*3+1].z=0.0f;
					nvert[j][k*3+2].x=0.0f;
					nvert[j][k*3+2].y=0.0f;
					nvert[j][k*3+2].z=0.0f;

					v1.x=0.0f; // v1 guardara el numero de normales en la suma para la media
					v1.y=0.0f;
					v1.z=0.0f;
					iver1=caux->A; // iver1 es el vertice cuya normal kiero calcular
					iver2=caux->B;
					iver3=caux->C;

					caux2=aux->c;
					for (i=ent; i<aux->numc+ent; i++) // comparar con las otras caras del objeto
					{							
						if (caux2->A==iver1 || caux2->B==iver1 || caux2->C==iver1)
						{
							nvert[j][k*3+0].x+=ncara[j][i].x;
							nvert[j][k*3+0].y+=ncara[j][i].y;
							nvert[j][k*3+0].z+=ncara[j][i].z;
							v1.x++;
						}
						if (caux2->A==iver2 || caux2->B==iver2 || caux2->C==iver2)
						{
							nvert[j][k*3+1].x+=ncara[j][i].x;
							nvert[j][k*3+1].y+=ncara[j][i].y;
							nvert[j][k*3+1].z+=ncara[j][i].z;
							v1.y++;
						}
						if (caux2->A==iver3 || caux2->B==iver3 || caux2->C==iver3)
						{
							nvert[j][k*3+2].x+=ncara[j][i].x;
							nvert[j][k*3+2].y+=ncara[j][i].y;
							nvert[j][k*3+2].z+=ncara[j][i].z;
							v1.z++;
						}
						caux2=caux2->sig;
					}
					nvert[j][k*3+0].x/=v1.x;
					nvert[j][k*3+0].y/=v1.x;
					nvert[j][k*3+0].z/=v1.x;
					nvert[j][k*3+1].x/=v1.y;
					nvert[j][k*3+1].y/=v1.y;
					nvert[j][k*3+1].z/=v1.y;
					nvert[j][k*3+2].x/=v1.z;
					nvert[j][k*3+2].y/=v1.z;
					nvert[j][k*3+2].z/=v1.z;
					nvert[j][k*3+0]=Normalizar(nvert[j][k*3+0]);
					nvert[j][k*3+1]=Normalizar(nvert[j][k*3+1]);
					nvert[j][k*3+2]=Normalizar(nvert[j][k*3+2]);
					caux=caux->sig;
				}
				else
				{
					nvert[j][k*3+0]=ncara[j][k];
					nvert[j][k*3+1]=ncara[j][k];
					nvert[j][k*3+2]=ncara[j][k];
					caux=caux->sig;
				}
			}
			ent+=aux->numc;
			aux=aux->sig;			
		}
	}

	// escribir las normales de cada vertice
	PRINTF("\nEscribiendo...");
	PRINTF("\n - Normales de vertice");
	for (i=0; i<nummats; i++)
	{
		for (j=0; j<l[i].numc*3; j++)
		{
			gzwrite(fE,&(nvert[i][j].x),4);
			gzwrite(fE,&(nvert[i][j].z),4);
			*pfdat=-nvert[i][j].y;
			gzwrite(fE,pfdat,4);
		}
	}

	// ahora los colores de los vertices ke no tienen textura
	aux=l[nummats-1].prim;
	while(aux!=NULL)
	{
		for (i=0; i<aux->numc; i++)
		{
			*pfdat=aux->color.x;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.y;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.z;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.x;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.y;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.z;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.x;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.y;
			gzwrite(fE,pfdat,4);
			*pfdat=aux->color.z;
			gzwrite(fE,pfdat,4);
		}
		aux=aux->sig;
	}

	PRINTF("\n - Nombres de las texturas");
	for (i=0; i<nummats; i++)
	{
		strcpy(cad, mat[i]);
		if (strcmp(cad,"(NINGUNA)")!=0)
		{		
			*pidat=strlen(cad);
			*pidat=*pidat+1;
			gzwrite(fE, pidat, 4);
			gzwrite(fE, cad, *pidat);
		}
		else
		{
			*pidat=0;
			gzwrite(fE, pidat, 4);
		}
	}

	PRINTF("\n - Lineas");
	gzwrite(fE, &g.numlin, 4);	
	linaux=g.primlin;
	while(linaux!=NULL)
	{
		*pidat=strlen(linaux->nombre);
		gzwrite(fE, pidat, 4);
		gzwrite(fE, linaux->nombre, *pidat);
		gzwrite(fE, &linaux->numv, 4);
		for (i=0; i<linaux->numv; i++)
		{
			gzwrite(fE, &linaux->v[i].x, 4);
			gzwrite(fE, &linaux->v[i].z, 4);
			*pfdat=-linaux->v[i].y;
			gzwrite(fE, pfdat, 4);
		}
		linaux=linaux->sig;
	}

	PRINTF("\n - Camara");
	gzwrite(fE, &g.numcam, 4);
	camaux=g.primcam;
	while(camaux!=NULL)
	{
		*pidat=strlen(camaux->nombre)+1;
		gzwrite(fE, pidat, 4);
		gzwrite(fE, camaux->nombre, *pidat);
		gzwrite(fE, &camaux->pos.x, 4);
		gzwrite(fE, &camaux->pos.z, 4);
		*pfdat=-camaux->pos.y;
		gzwrite(fE, pfdat, 4);
		gzwrite(fE, &camaux->mira.x, 4);
		gzwrite(fE, &camaux->mira.z, 4);
		*pfdat=-camaux->mira.y;
		gzwrite(fE, pfdat, 4);
		gzwrite(fE, &camaux->angrot, 4);
		gzwrite(fE, &camaux->fov,4);
		camaux=camaux->sig;
	}
	
	PRINTF("\n - Luces");
	gzwrite(fE, &g.numl, 4);
	laux=g.priml;
	while(laux!=NULL)
	{
		gzwrite(fE, &laux->pos[0], 4);
		gzwrite(fE, &laux->pos[2], 4);
		*pfdat=-laux->pos[1];
		gzwrite(fE, pfdat, 4);
		gzwrite(fE, laux->color, 12);
		laux=laux->sig;
	}
	
	PRINTF("\n - Objetos");
	gzwrite(fE, &g.numo,4); // num creatures TEMPORAL!!! HACER KE SE PUEDAN EXPORTAR MUCHAS
	aux=0;
//	gzwrite(fE, &i,4); // id
	maux=g.primo;
	while(maux!=NULL)
	{
		*pidat=strlen(maux->nombre);
		gzwrite(fE, pidat, 4);
		gzwrite(fE, maux->nombre, *pidat);
		gzwrite(fE, &maux->pos[0], 4);
		gzwrite(fE, &maux->pos[2], 4);
		*pfdat=-maux->pos[1];
		gzwrite(fE, pfdat, 4);
		maux=maux->sig;
	}
//	gzwrite(fE, &i,4); // id
//	gzwrite(fE, &g.posjug2[0], 4);
//	gzwrite(fE, &g.posjug2[2], 4);
//	g.posjug2[1]=-g.posjug2[1];
//	gzwrite(fE, &g.posjug2[1], 4);


	// calcular los mapas de luz
	PRINTF("\n[ Calcular lightmaps ]\n");
	lm_numtris=(TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)*(TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO);	
	num_lm_this_tex=(int*)malloc(sizeof(int)*nummats);
	max_num_lm_this_tex=(int*)malloc(sizeof(int)*nummats);

	for (i=0; i<nummats; i++)
		for (max_num_lm_this_tex[i]=1; 1; max_num_lm_this_tex[i]++)
			if (lm_numtris*max_num_lm_this_tex[i]>=l[i].numc)
				break;

	fluz=fE;
	pidat[0]=TEXLUZ_ANCHO; gzwrite(fluz, pidat,4);
	pidat[0]=TEXLUZ_ALTO; gzwrite(fluz, pidat,4);
	pidat[0]=TEXLUZ_TRIANGULO_ANCHO; gzwrite(fluz, pidat,4);
	pidat[0]=TEXLUZ_TRIANGULO_ALTO; gzwrite(fluz, pidat,4);

	lm_texverts=(struct VERTICE**)malloc(sizeof(struct VERTICE*)*nummats);

	// calcular posiciones de cada triangulo dentro de cada lightmap, numero de triangulos por lightmap
	// y numero de lightmaps por textura
	trilm_posicion=(struct TRILM_POSICION**)malloc(sizeof(struct TRILM_POSICION*)*nummats);	
	lm_vert_unido_con=(int***)malloc(sizeof(int**)*nummats);
	forma_cuad_con_el_sig=(int**)malloc(sizeof(int*)*nummats);
	num_tris_this_lm=(int**)malloc(sizeof(int*)*nummats);	
	lm_cuadrados=(int**)malloc(sizeof(int*)*nummats);
	orden_vertices=(int***)malloc(sizeof(int**)*nummats);
	for (i=0; i<nummats; i++)
	{
		trilm_posicion[i]=(struct TRILM_POSICION*)malloc(sizeof(struct TRILM_POSICION)*l[i].numc);
		lm_vert_unido_con[i]=(int**)malloc(sizeof(int*)*l[i].numc);
		forma_cuad_con_el_sig[i]=(int*)malloc(sizeof(int)*l[i].numc);
		num_tris_this_lm[i]=(int*)malloc(sizeof(int)*max_num_lm_this_tex[i]);
		lm_cuadrados[i]=(int*)malloc(sizeof(int)*max_num_lm_this_tex[i]);
		orden_vertices[i]=(int**)malloc(sizeof(int*)*l[i].numc);
		ilightmap=0;
		num_lm_this_tex[i]=1;
		for (k=0; k<max_num_lm_this_tex[i]; k++)
		{
			num_tris_this_lm[i][k]=0;
			lm_cuadrados[i][k]=0; // cuenta los cuadrados ocupados por triangulos
		}
		for (j=0; j<l[i].numc; j++)
		{
			lm_vert_unido_con[i][j]=(int*)malloc(sizeof(int)*3);
			orden_vertices[i][j]=(int*)malloc(sizeof(int)*3);
			orden_vertices[i][j][0]=0;
			orden_vertices[i][j][1]=1;
			orden_vertices[i][j][2]=2;
			trilm_posicion[i][j].pos=POS_NODEFINIDO;

			lm_vert_unido_con[i][j][0]=-1;
			lm_vert_unido_con[i][j][1]=-1;
			lm_vert_unido_con[i][j][2]=-1;
			forma_cuad_con_el_sig[i][j]=0;
			trilm_posicion[i][j].pos=POS_ARRIBA;						

			ent=0;
			if (j==0)
				ent=1;
			else
				if (forma_cuad_con_el_sig[i][j-1]==0)
					ent=1;

			if (ent==1) // triangulos de arriba
			{
				num_tris_this_lm[i][ilightmap]++;
				lm_cuadrados[i][ilightmap]++;
				// mirar si tienen la misma normal
				if (j+1<l[i].numc && 
					ncara[i][j].x == ncara[i][j+1].x && 
					ncara[i][j].y == ncara[i][j+1].y &&
					ncara[i][j].z == ncara[i][j+1].z)
				{
					// mirar ke vertices son los ke estan unidos
					for (k=0; k<3; k++)
						for (ent=0; ent<3; ent++)
							if (verts[i][j*3+k].x==verts[i][j*3+3+ent].x &&
								verts[i][j*3+k].y==verts[i][j*3+3+ent].y &&
								verts[i][j*3+k].z==verts[i][j*3+3+ent].z)
								lm_vert_unido_con[i][j][k]=ent;
					if ((lm_vert_unido_con[i][j][0]>=0 && lm_vert_unido_con[i][j][1]>=0) ||
						(lm_vert_unido_con[i][j][0]>=0 && lm_vert_unido_con[i][j][2]>=0) ||
						(lm_vert_unido_con[i][j][1]>=0 && lm_vert_unido_con[i][j][2]>=0))
						forma_cuad_con_el_sig[i][j]=1;
					else
						forma_cuad_con_el_sig[i][j]=0;
				} 
			}
			else // triangulos de abajo
			{
				num_tris_this_lm[i][ilightmap]++;
				trilm_posicion[i][j].pos=POS_ABAJO;
			}

//			else
//			{	
//				if (forma_cuad_con_el_sig[i][j]==0)
//				{
//					lm_cuadrados[i][ilightmap]++;
//					trilm_posicion[i][j].pos=POS_ARRIBA;
//				}
//				else
//					trilm_posicion[i][j].pos=POS_ABAJO;
//				num_tris_this_lm[i][ilightmap]++;
//				forma_cuad_con_el_sig[i][j]=0;
//			}
//			num_tris_this_lm[i][ilightmap]++;
			if (lm_cuadrados[i][ilightmap]>=(TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)*(TEXLUZ_ALTO/TEXLUZ_TRIANGULO_ALTO))
			{
				if (!forma_cuad_con_el_sig[i][j])
				{
					ilightmap++;
					num_lm_this_tex[i]++;
				}
			}
		} // j=0 -> l[i].numc
	}

	numlm=0;
	for (i=0; i<nummats; i++)
		numlm+=num_lm_this_tex[i];

	for (i=0; i<nummats; i++)
		gzwrite(fluz, &(num_lm_this_tex[i]),4);

	for (i=0; i<nummats; i++)
		for (j=0; j<num_lm_this_tex[i]; j++)
			gzwrite(fluz,&(num_tris_this_lm[i][j]),4);


	buffer_texels=(unsigned char *)malloc(sizeof(unsigned char)*TEXLUZ_ALTO*TEXLUZ_ANCHO*3);
	for (i=0; i<TEXLUZ_ALTO*TEXLUZ_ANCHO; i++)
	{
		buffer_texels[i*3+0]=0;
		buffer_texels[i*3+1]=255;
		buffer_texels[i*3+2]=0;		
	}

	ilightmap=0;
	PRINTF("Num grupos tex: %d\n",nummats);
	for (i=0; i<nummats; i++)
	{		
		lm_texverts[i]=(struct VERTICE*)malloc(sizeof(struct VERTICE)*l[i].numc*3);
		for (k=0; k<num_lm_this_tex[i]; k++)
		{
			PRINTF("(%d/%d) - [",ilightmap+1,numlm);
			fflush(stdout);
			estait=0;			

			for (j=0; j<TEXLUZ_ALTO*TEXLUZ_ANCHO; j++)
			{
				buffer_texels[i*3+0]=0;
				buffer_texels[i*3+1]=255;
				buffer_texels[i*3+2]=0;		
			}

			for (x=0; x<TEXLUZ_ANCHO; x++)
			{
				for (y=0; y<TEXLUZ_ALTO; y++)
				{	
					if (estait++>=TEXLUZ_ANCHO*TEXLUZ_ALTO/50)
					{
						estait=0;
						PRINTF(".");
						fflush(stdout);
					}

//					itri=DaIndiceTrianguloMapaLuz(x,y);

//					itri=((TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)*(y/TEXLUZ_TRIANGULO_ALTO))+(x/TEXLUZ_TRIANGULO_ANCHO);
//					for (j=0; j<(TEXLUZ_ALTO/TEXLUZ_TRIANGULO_ALTO); j++)
//						for (ent=0; ent<(TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO); ent++)
//							if 
					ent=0;
					for (j=0; j<k; j++)
						ent+=num_tris_this_lm[i][j];

					itri=0;
					*pidat=0; // cuadrados
					for (j=0; j<(TEXLUZ_ALTO/TEXLUZ_TRIANGULO_ALTO)*(TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO); j++)
					{
						if ((*pidat)==((TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)*(y/TEXLUZ_TRIANGULO_ALTO))+(x/TEXLUZ_TRIANGULO_ANCHO))
						{
							// si xaux-TEXLUZ_TRIANGULO_ALTO+yaux>=0 esta en la parte de abajo
							if ((x-((x/(int)TEXLUZ_TRIANGULO_ANCHO)*TEXLUZ_TRIANGULO_ANCHO))-TEXLUZ_TRIANGULO_ALTO+(y-((y/(int)TEXLUZ_TRIANGULO_ALTO)*TEXLUZ_TRIANGULO_ALTO))>=0)
								itri++;
							break;						
						}
						if (itri+ent<l[i].numc)
						{
							if (forma_cuad_con_el_sig[i][itri+ent])
								itri+=2;
							else
								itri++;
						}
						else
							itri++;
						(*pidat)++;
					}

					itri+=ent;

					if (itri<l[i].numc)
					{					
//						ent=0;						
//						aux=l[i].prim;
//						j=aux->numc;
//						while(aux!=NULL)
//						{
//							if (j>=itri)
//								break;							
//							ent+=aux->numv;
//							aux=aux->sig;
//							if (aux!=NULL)
//								j+=aux->numc;
//						}
//
//						v1=*DaVertice(&(l[i]),DaCara(&(l[i]),itri)->A+ent);
//						v2=*DaVertice(&(l[i]),DaCara(&(l[i]),itri)->B+ent);
//						v3=*DaVertice(&(l[i]),DaCara(&(l[i]),itri)->C+ent);

						// comprobar si el triangulo siguiente forma un cuadrado con el actual,
						// de ser asi podemos agruparlos en un mismo cuadrado del lightmap
						// en vez de en 2 trianguos separados

						if (trilm_posicion[i][itri].pos==POS_ARRIBA)
						{
							if (forma_cuad_con_el_sig[i][itri])
							{
								if (lm_vert_unido_con[i][itri][0]>=0 && lm_vert_unido_con[i][itri][1]>=0)
								{
									v1=verts[i][itri*3+2];
									v2=verts[i][itri*3+0];
									v3=verts[i][itri*3+1];
									orden_vertices[i][itri][0]=2;
									orden_vertices[i][itri][1]=0;
									orden_vertices[i][itri][2]=1;
								}
								if (lm_vert_unido_con[i][itri][0]>=0 && lm_vert_unido_con[i][itri][2]>=0)
								{
									v1=verts[i][itri*3+1];
									v2=verts[i][itri*3+2];
									v3=verts[i][itri*3+0];
									orden_vertices[i][itri][0]=1;
									orden_vertices[i][itri][1]=2;
									orden_vertices[i][itri][2]=0;
								}
								if (lm_vert_unido_con[i][itri][1]>=0 && lm_vert_unido_con[i][itri][2]>=0)
								{
									v1=verts[i][itri*3+0];
									v2=verts[i][itri*3+1];
									v3=verts[i][itri*3+2];
									orden_vertices[i][itri][0]=0;
									orden_vertices[i][itri][1]=1;
									orden_vertices[i][itri][2]=2;
								}
							}
							else
							{
								v1=verts[i][itri*3];
								v2=verts[i][itri*3+1];
								v3=verts[i][itri*3+2];
								orden_vertices[i][itri][0]=0;
								orden_vertices[i][itri][1]=1;
								orden_vertices[i][itri][2]=2;

							}
						}
						else // si el triangulo va abajo
						{							
							if (lm_vert_unido_con[i][itri-1][0]>=0 && lm_vert_unido_con[i][itri-1][1]>=0)
							{
								v1=verts[i][itri*3+2];
								v2=verts[i][itri*3+0];
								v3=verts[i][itri*3+1];
								orden_vertices[i][itri][0]=2;
								orden_vertices[i][itri][1]=0;
								orden_vertices[i][itri][2]=1;
							}
							if (lm_vert_unido_con[i][itri-1][0]>=0 && lm_vert_unido_con[i][itri-1][2]>=0)
							{
								v1=verts[i][itri*3+1];
								v2=verts[i][itri*3+2];
								v3=verts[i][itri*3+0];
								orden_vertices[i][itri][0]=1;
								orden_vertices[i][itri][1]=2;
								orden_vertices[i][itri][2]=0;
							}
							if (lm_vert_unido_con[i][itri-1][1]>=0 && lm_vert_unido_con[i][itri-1][2]>=0)
							{
								v1=verts[i][itri*3+0];
								v2=verts[i][itri*3+1];
								v3=verts[i][itri*3+2];
								orden_vertices[i][itri][0]=0;
								orden_vertices[i][itri][1]=1;
								orden_vertices[i][itri][2]=2;
							}
//							v1=verts[i][itri*3];
//							v2=verts[i][itri*3+1];
//							v3=verts[i][itri*3+2];
						}

						vd12.x=v2.x-v1.x;
						vd12.y=v2.y-v1.y;
						vd12.z=v2.z-v1.z;
						vd13.x=v3.x-v1.x;
						vd13.y=v3.y-v1.y;
						vd13.z=v3.z-v1.z;

						xaux=x-((x/(int)TEXLUZ_TRIANGULO_ANCHO)*TEXLUZ_TRIANGULO_ANCHO);
						yaux=y-((y/(int)TEXLUZ_TRIANGULO_ALTO)*TEXLUZ_TRIANGULO_ALTO);

//						p.x = (yaux*vy.x - xaux*vy.y) / (vx.y*vy.x - vx.x*vy.y);
//						p.y = (xaux - p.x*vx.x) / vy.x;

						if (xaux-TEXLUZ_TRIANGULO_ALTO+yaux==0 && trilm_posicion[i][itri].pos==POS_ARRIBA)
						{
							if (xaux>=TEXLUZ_TRIANGULO_ALTO*0.5f)
							{
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+0]=buffer_texels[((y*TEXLUZ_ANCHO)+x-1)*3+0];
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+1]=buffer_texels[((y*TEXLUZ_ANCHO)+x-1)*3+1];
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+2]=buffer_texels[((y*TEXLUZ_ANCHO)+x-1)*3+2];
							}
							else
							{
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+0]=buffer_texels[(((y-1)*TEXLUZ_ANCHO)+x)*3+0];
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+1]=buffer_texels[(((y-1)*TEXLUZ_ANCHO)+x)*3+1];
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+2]=buffer_texels[(((y-1)*TEXLUZ_ANCHO)+x)*3+2];
							}
							continue;
						}



						if (xaux-TEXLUZ_TRIANGULO_ALTO+yaux>0 && trilm_posicion[i][itri].pos==POS_ARRIBA)
						{
//							if (forma_cuad_con_el_sig[i][itri]==0)
//							{
//								oscuridad=255; gzwrite(fluz,&oscuridad,1);
//								oscuridad=0;   gzwrite(fluz,&oscuridad,1);
//								oscuridad=0;   gzwrite(fluz,&oscuridad,1);								
//								buffer_texels[y][x][0]=255;
//								buffer_texels[y][x][1]=0;
//								buffer_texels[y][x][2]=0;
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+0]=255;
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+1]=0;
								buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+2]=0;
								continue;
//							}							
						}

						if (xaux-TEXLUZ_TRIANGULO_ALTO+yaux<0)
						{
//							p.x = (xaux-1)*((float)(TEXLUZ_TRIANGULO_ANCHO)/(float)(TEXLUZ_TRIANGULO_ANCHO-2.0f)) / (float)(TEXLUZ_TRIANGULO_ANCHO-2.0f);
//							p.y = (yaux-1)*((float)(TEXLUZ_TRIANGULO_ALTO)/(float)(TEXLUZ_TRIANGULO_ALTO-2.0f)) / (float)(TEXLUZ_TRIANGULO_ALTO-2.0f);

							xaux2=xaux-1;
							if (xaux<TEXLUZ_MARGEN)
								xaux2=0.0f;
							if (xaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
								xaux2=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN-1;
							yaux2=yaux-1;
							if (yaux<TEXLUZ_MARGEN)
								yaux2=0.0f;
							if (yaux>=TEXLUZ_TRIANGULO_ALTO-TEXLUZ_MARGEN)
								yaux2=TEXLUZ_TRIANGULO_ALTO-TEXLUZ_MARGEN-1;

							p.x = xaux2 / (float)(TEXLUZ_TRIANGULO_ANCHO-2.0f);
							p.y = yaux2 / (float)(TEXLUZ_TRIANGULO_ALTO-2.0f);

							if (xaux<TEXLUZ_MARGEN)
								p.x=0.0f;

							if (yaux<TEXLUZ_MARGEN)
								p.y=0.0f;

							if (xaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
								p.x=(float)(TEXLUZ_TRIANGULO_ANCHO-1)/(float)(TEXLUZ_TRIANGULO_ANCHO);

							if (yaux>=TEXLUZ_TRIANGULO_ALTO-TEXLUZ_MARGEN)
								p.y=(float)(TEXLUZ_TRIANGULO_ALTO-1)/(float)(TEXLUZ_TRIANGULO_ALTO);

//							if (xaux<TEXLUZ_MARGEN)
//								p.x=(float)TEXLUZ_MARGEN / (float)TEXLUZ_TRIANGULO_ANCHO;
//
//							if (yaux<TEXLUZ_MARGEN)
//								p.y=(float)TEXLUZ_MARGEN / (float)TEXLUZ_TRIANGULO_ALTO;
//
//							// ESTO HAY QUE REVISARLO PARA UN MARGEN MAYOR KE 1:
//							if (forma_cuad_con_el_sig[i][itri]==0 && xaux-TEXLUZ_TRIANGULO_ALTO+yaux==-1)
//							{
//								if (xaux>=TEXLUZ_TRIANGULO_ANCHO/2)
//									p.x-=0.5f / (float)(TEXLUZ_TRIANGULO_ANCHO-2);
//								else
//									p.y-=0.5f / (float)(TEXLUZ_TRIANGULO_ALTO-2);
//							}
							p.x +=0.5f / (float)(TEXLUZ_TRIANGULO_ANCHO-2);
							p.y +=0.5f / (float)(TEXLUZ_TRIANGULO_ALTO-2);
						}
						else // para el triangulo de abajo
						{
//							p.x = 1.0f - ((xaux-1.0f) / (float)(TEXLUZ_TRIANGULO_ANCHO-1.0f));
//							p.y = 1.0f - ((yaux-1.0f) / (float)(TEXLUZ_TRIANGULO_ALTO-1.0f));
//							p.x = (TEXLUZ_TRIANGULO_ANCHO-1-xaux-1)*((float)(TEXLUZ_TRIANGULO_ANCHO)/(float)(TEXLUZ_TRIANGULO_ANCHO-1.0f)) / (float)(TEXLUZ_TRIANGULO_ANCHO-2.0f);
//							p.y = (TEXLUZ_TRIANGULO_ALTO-1-yaux-1)*((float)(TEXLUZ_TRIANGULO_ALTO)/(float)(TEXLUZ_TRIANGULO_ALTO-1.0f)) / (float)(TEXLUZ_TRIANGULO_ALTO-2.0f);

							xaux2=TEXLUZ_TRIANGULO_ANCHO-xaux-1;
							if (xaux<TEXLUZ_MARGEN)
								xaux2=1.0f;
							if (xaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
								xaux2=0.0f;
							yaux2=TEXLUZ_TRIANGULO_ALTO-yaux-1;
							if (yaux<TEXLUZ_MARGEN)
								yaux2=1.0f;
							if (yaux>=TEXLUZ_TRIANGULO_ALTO-TEXLUZ_MARGEN)
								yaux2=0.0f;

							p.x = xaux2 / (float)(TEXLUZ_TRIANGULO_ANCHO-2.0f);
							p.y = yaux2 / (float)(TEXLUZ_TRIANGULO_ALTO-2.0f);


							if (xaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
								p.x=0.0f+1.0f/(float)(TEXLUZ_TRIANGULO_ANCHO-2.0f);

							if (yaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
								p.y=0.0f+1.0f/(float)(TEXLUZ_TRIANGULO_ALTO-2.0f);

							if (xaux<TEXLUZ_MARGEN)
								p.x=1.0f;

							if (yaux<TEXLUZ_MARGEN)
								p.y=1.0f;

//							if (xaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
//								p.x=(float)TEXLUZ_MARGEN / (float)TEXLUZ_TRIANGULO_ANCHO;
//
//							if (yaux>=TEXLUZ_TRIANGULO_ANCHO-TEXLUZ_MARGEN)
//								p.y=(float)TEXLUZ_MARGEN / (float)TEXLUZ_TRIANGULO_ALTO;
//
							p.x -=0.5f / (float)(TEXLUZ_TRIANGULO_ANCHO-2);
							p.y -=0.5f / (float)(TEXLUZ_TRIANGULO_ALTO-2);
						}


//						p.x=(p.x*(float)(TEXLUZ_TRIANGULO_ANCHO)/(float)(TEXLUZ_TRIANGULO_ANCHO-1.0f));
//						p.y=(p.y*(float)(TEXLUZ_TRIANGULO_ALTO)/(float)(TEXLUZ_TRIANGULO_ALTO-1.0f));

						// para ke se lance el rayo a la mitad del texel y no al principio
//						p.x +=0.5f / (float)(TEXLUZ_TRIANGULO_ANCHO);
//						p.y +=0.5f / (float)(TEXLUZ_TRIANGULO_ALTO);
						
						pres.x = p.x*vd12.x + p.y*vd13.x;
						pres.y = p.x*vd12.y + p.y*vd13.y;
						pres.z = p.x*vd12.z + p.y*vd13.z;

						pres.x+=v1.x;
						pres.y+=v1.y;
						pres.z+=v1.z;

//						ilum=0.1f;
//						if (xaux-TEXLUZ_TRIANGULO_ALTO+yaux>=0)
							colortexel.x=0.1f;
//						else
//							if (xaux-TEXLUZ_TRIANGULO_ALTO+yaux==-1)
//								colortexel.x=1.0f;
//							else
//								colortexel.x=0.1f;
						colortexel.y=0.1f;
						colortexel.z=0.1f;

						for (j=0; j<g.numl; j++)
						{
							laux=DaLuz(&g,j);

							vdpluz.x=laux->pos[0]-pres.x;
							vdpluz.y=laux->pos[1]-pres.y;
							vdpluz.z=laux->pos[2]-pres.z;							
							
							modpluz=sqrt(vdpluz.x*vdpluz.x + vdpluz.y*vdpluz.y + vdpluz.z*vdpluz.z);
//							vdpluz.x=0-pres.x;
//							vdpluz.y=0-pres.y;
//							vdpluz.z=0-pres.z;
							if (modpluz<255*laux->intensidad) // distdelaluz*laux->intensidad
								if (!ColisionMapa(l,nummats,laux->pos[0],laux->pos[1],laux->pos[2],pres.x,pres.y,pres.z,i,itri))
								{
									dist=modpluz;
//									dist*=2.0f;
									dist/=laux->intensidad;
									if (dist>255)
										dist=255.0f;
									*pfdat=((ncara[i][itri].x*vdpluz.x)+
											(ncara[i][itri].y*vdpluz.y)+
											(ncara[i][itri].z*vdpluz.z)) /
											(sqrt(ncara[i][itri].x*ncara[i][itri].x + ncara[i][itri].y*ncara[i][itri].y + ncara[i][itri].z*ncara[i][itri].z) *
											 sqrt(vdpluz.x*vdpluz.x + vdpluz.y*vdpluz.y + vdpluz.z*vdpluz.z));

									if (*pfdat>=0.0f)
									{
										colortexel.x+=(1.0f-dist/255.0f)*laux->color[0]*(*pfdat);
										colortexel.y+=(1.0f-dist/255.0f)*laux->color[1]*(*pfdat);
										colortexel.z+=(1.0f-dist/255.0f)*laux->color[2]*(*pfdat);
									}
								}							
						}

//						dist=sqrt((DaLuz(&g,0)->pos[0]-pres.x)*(DaLuz(&g,0)->pos[0]-pres.x)+(DaLuz(&g,0)->pos[1]-pres.y)*(DaLuz(&g,0)->pos[1]-pres.y)+(DaLuz(&g,0)->pos[2]-pres.z)*(DaLuz(&g,0)->pos[2]-pres.z));
//						dist*=12; // arbitrario
						
//						vdpluz.x=DaLuz(&g,0)->pos[0]-pres.x;
//						vdpluz.y=DaLuz(&g,0)->pos[1]-pres.y;
//						vdpluz.z=DaLuz(&g,0)->pos[2]-pres.z;
//						vdpluz=Normalizar(vdpluz);
//						vcara=DaNormalCara(&l[i],itri);
						
//						if (dist>255)
//							dist=255.0f;

						if (colortexel.x>1.0f)
							colortexel.x=1.0f;
						if (colortexel.y>1.0f)
							colortexel.y=1.0f;
						if (colortexel.z>1.0f)
							colortexel.z=1.0f;

//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+0]=0;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+1]=0;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+2]=0;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+3]=(1.0f-ilum)*255.0f;
//						
						//oscuridad=(unsigned char)((1.0f-ilum)*255.0f);
//						oscuridad=(unsigned char)dist;
//						gzwrite(&oscuridad,1,1,fluz);
					}
					else
					{

						colortexel.x=0;
						colortexel.y=0;
						colortexel.z=1;

						//oscuridad=(unsigned char)255;
//						gzwrite(&oscuridad,1,1,fluz);
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+0]=255;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+1]=0;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+2]=0;
//						lm_buffer[((y*TEXLUZ_ANCHO)+x)*4+3]=255;
//				oscuridad=0;   gzwrite(fluz,&oscuridad,1);
//				oscuridad=0;   gzwrite(fluz,&oscuridad,1);
//				oscuridad=255; gzwrite(fluz,&oscuridad,1);							
//				continue;
//
					}

//					oscuridad=(unsigned char)(colortexel.x*255); gzwrite(fluz, &oscuridad,1);
//					oscuridad=(unsigned char)(colortexel.y*255); gzwrite(fluz, &oscuridad,1);
//					oscuridad=(unsigned char)(colortexel.z*255); gzwrite(fluz, &oscuridad,1);
//					buffer_texels[y][x][0]=(unsigned char)(colortexel.x*255);
//					buffer_texels[y][x][1]=(unsigned char)(colortexel.y*255);
//					buffer_texels[y][x][2]=(unsigned char)(colortexel.z*255);
					buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+0]=(unsigned char)(colortexel.x*255);
					buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+1]=(unsigned char)(colortexel.y*255);
					buffer_texels[((y*TEXLUZ_ANCHO)+x)*3+2]=(unsigned char)(colortexel.z*255);

//					PRINTF(".");
				}
			}
			PRINTF("]\n");
			ilightmap++;
			// escribir el lightmap
			gzwrite(fluz,buffer_texels,TEXLUZ_ANCHO*TEXLUZ_ALTO*3);
			

		} // next k

		lm_x=0;
		lm_y=0;
		lm_unidad=1.0f/(float)TEXLUZ_ANCHO;
		for (j=0; j<l[i].numc; j++)
		{	
			if (trilm_posicion[i][j].pos==POS_ARRIBA) // es el triangulo va arriba
			{
				lm_texverts[i][j*3+orden_vertices[i][j][0]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO+lm_unidad; // si no se ponen estos (float) al denominador se operan como enteros y se trunca y como siempre es menor ke uno siempre dara 0
				lm_texverts[i][j*3+orden_vertices[i][j][0]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO+lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][0]].z = 0.0f;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO+TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO-lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO+lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].z = 0.0f;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO+lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO+TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO-lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].z = 0.0f;
			}
			else // si no: va abajo
			{
				// el tri de abajo no tiene margen en la diagonal pq si esta abajo es ke forma un cuadrado con el de arriba
				lm_x--;
				lm_texverts[i][j*3+orden_vertices[i][j][0]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO+TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO-lm_unidad; // si no se ponen estos (float) al denominador se operan como enteros y se trunca y como siempre es menor ke uno siempre dara 0
				lm_texverts[i][j*3+orden_vertices[i][j][0]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO+TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO-lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][0]].z = 0.0f;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO+lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO+TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO-lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][1]].z = 0.0f;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].x = (lm_x*TEXLUZ_TRIANGULO_ANCHO+TEXLUZ_TRIANGULO_ANCHO)/(float)TEXLUZ_ANCHO-lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].y = (lm_y*TEXLUZ_TRIANGULO_ALTO)/(float)TEXLUZ_ALTO+lm_unidad;
				lm_texverts[i][j*3+orden_vertices[i][j][2]].z = 0.0f;

	//			tlm[j*9+0]=(x*lmap_size)/(float)lmap_totalsize+unidad+unidad2;
	//			tlm[j*9+1]=(y*lmap_size)/(float)lmap_totalsize+unidad;
	//			tlm[j*9+2]=0;
	//			tlm[j*9+3]=(x*lmap_size+lmap_size)/(float)lmap_totalsize-unidad;
	//			tlm[j*9+4]=(y*lmap_size+lmap_size)/(float)lmap_totalsize-unidad-unidad2;
	//			tlm[j*9+5]=0;
	//			tlm[j*9+6]=(x*lmap_size+lmap_size)/(float)lmap_totalsize-unidad;
	//			tlm[j*9+7]=(y*lmap_size)/(float)lmap_totalsize+unidad;
	//			tlm[j*9+8]=0;
	//			x++;
			}
			lm_x++;
			if (lm_x>=TEXLUZ_ANCHO/TEXLUZ_TRIANGULO_ANCHO)
			{
				if (forma_cuad_con_el_sig[i][j]==0) // esto se hace pq cuando se llegue al maximo y falta por poner el triangulo de abajo no se pase a la siguiente fila
				{
					lm_x=0;
					lm_y++;
					if (lm_y>=TEXLUZ_ALTO/TEXLUZ_TRIANGULO_ALTO)
						lm_y=0;
				}
			}			
		}
	} // next i -> nummats
	

	// coordenadas de textura de los vertices para los lightmap
	PRINTF("\n [ Escribir Coordenadas de Textura de los vertices para los lightmap... ]\n");
	for (i=0; i<nummats; i++)
	{
		for (j=0; j<l[i].numc*3; j++)
		{
			gzwrite(fluz,&lm_texverts[i][j].x,4);
			gzwrite(fluz,&lm_texverts[i][j].y,4);
			gzwrite(fluz,&lm_texverts[i][j].z,4);
		}
	}


	// Generar octree
	PRINTF("\nGenerando Octree...\n");
//		raiz=(struct NODO_OCTREE *)malloc(sizeof(NODO_OCTREE
	bbox.left=bbox.right=verts[0][0].x;
	bbox.top=bbox.bottom=verts[0][0].y;
	bbox.front=bbox.back=verts[0][0].z;		
	// calcular nodo pricipal
	for (i=0; i<nummats; i++)
	{
		for (j=0; j<l[i].numc*3; j++)
		{
			if (verts[i][j].x<bbox.left) bbox.left=verts[i][j].x;
			if (verts[i][j].x>bbox.right) bbox.right=verts[i][j].x;
			if (verts[i][j].y<bbox.bottom) bbox.bottom=verts[i][j].y;
			if (verts[i][j].y>bbox.top) bbox.top=verts[i][j].y;
			if (verts[i][j].z<bbox.front) bbox.front=verts[i][j].z;
			if (verts[i][j].z>bbox.back) bbox.back=verts[i][j].z;
		}
	}


	GeneraOctree(&raiz,bbox,octree_maxpolis,nummats,l,-1);
	PRINTF(">> Octree terminado con %d hojas.\n",octree_numhojas);		
	PRINTF("Escribiendo Octrees...");
	fflush(stdout);
	gzwrite(fluz,&octree_numnodos,4);
	EscribeOctree(fluz,raiz,nummats);
	PRINTF("OK!\n");
	
//  PRINTF("\nEmpaquetando Texturas...\n");
//	bufftex=NULL;
//	for (i=0; i<nummats; i++)
//	{
//		ftextura=fopen(mat[i],"rb");
//		if (!ftextura)
//		{
//			PRINTF("ERROR: no se encontró la textura '%s'!!\n",mat[i]);
//			getch();
//		}
//		if (bufftex)
//			free(bufftex);
//		bufftex=(unsigned char*)malloc(
//
//	}

	PRINTF("Okis");	
	gzclose(fE);
	fclose(fASE);

*/

