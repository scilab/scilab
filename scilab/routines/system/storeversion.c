#include <string.h>
#include "../stack-c.h"
#include "../version.h" 

static char Version[]=VERSION; 
extern int C2F(withtk)(int *rep);
extern int C2F(withpvm)(int *rep);
extern int C2F(withgtk)(int *rep);
extern int C2F(withocaml)(int *rep);
extern int C2F(withjavasci)(int *rep);
extern int C2F(getcomp)(char *buf,int *nbuf,long int lbuf);

int C2F(intversion)(char * fname,unsigned long fname_len)
{
  static int n1,m1;
  char *v = Version ;
  Rhs=Max(0,Rhs);
  CheckRhs(0,0);
  CheckLhs(1,2);
  n1=1;
  CreateVarFromPtr( 1, "c",(m1=strlen(Version), &m1),&n1,&v);
  LhsVar(1) = 1;
  if (Lhs==2) {
    static char *Str[8];
    char tk[]="tk";
    char pvm[]="pvm";
    char gtk[]="gtk";
    char ocaml[]="ocaml";
	char javasci[]="javasci";
	char atlas[]="atlas";
	char debugmode[]="debug";
	char releasemode[]="release";

	#ifdef __TIME__
	char TimeBuild[]=__TIME__;
	#endif

	#ifdef __DATE__
	char DateBuild[]=__DATE__;
	#endif

    static int irep,nbuf;

    n1=0;
    C2F(getcomp)( C2F(cha1).buf,&nbuf,128);
    Str[n1]=C2F(cha1).buf;n1++;
    C2F(withgtk)(&irep);
    if (irep) {Str[n1]=gtk;n1++;}
    C2F(withpvm)(&irep);
    if (irep) {Str[n1]=pvm;n1++;}
    C2F(withtk)(&irep);
    if (irep) {Str[n1]=tk;n1++;}
    C2F(withocaml)(&irep);
    if (irep) {Str[n1]=ocaml;n1++;}
	C2F(withjavasci)(&irep);
    if (irep) {Str[n1]=javasci;n1++;}

	#ifdef WITH_ATLAS
		{Str[n1]=atlas;n1++;}
	#endif

	#ifdef WIN32
		#if _DEBUG
			{Str[n1]=debugmode;n1++;}
		#else
			{Str[n1]=releasemode;n1++;}
		#endif
	#else
		#ifdef NDEBUG
			{Str[n1]=releasemode;n1++;}
		#else
			{Str[n1]=debugmode;n1++;}
		#endif
	#endif

	#ifdef __TIME__
		{Str[n1]=DateBuild;n1++;}
	#endif

	#ifdef __DATE__
		{Str[n1]=TimeBuild;n1++;}
	#endif

    m1=1;
    CreateVarFromPtr( 2, "S", &m1, &n1, Str);
    LhsVar(2) = 2;
  }
  C2F(putlhsvar)();
  return(0);
}
