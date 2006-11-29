/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdio.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "version.h" 
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
static char Version[]=SCI_VERSION; 
extern int C2F(withtk)(int *rep);
extern int C2F(withpvm)(int *rep);
extern int C2F(withgtk)(int *rep);
extern int C2F(withmodelicac)(int *rep);
extern int C2F(withjavasci)(int *rep);
extern int C2F(getcomp)(char *buf,int *nbuf,long int lbuf);
/*-----------------------------------------------------------------------------------*/
int C2F(intversion) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getversion) _PARAMS((char *fname,unsigned long fname_len))
{
 	C2F(intversion)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intversion)(char * fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	char *v = Version ;
	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(1,2);

	if (Rhs==0)
	{
		n1=1;
		CreateVarFromPtr( 1, "c",(m1=strlen(Version), &m1),&n1,&v);
		LhsVar(1) = 1;
		if (Lhs==2) {
			static char *Str[12];
			char tk[]="tk";
			char pvm[]="pvm";
			char modelicac[]="modelicac";
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
			C2F(withpvm)(&irep);
			if (irep) {Str[n1]=pvm;n1++;}
			C2F(withtk)(&irep);
			if (irep) {Str[n1]=tk;n1++;}
			C2F(withmodelicac)(&irep);
			if (irep) {Str[n1]=modelicac;n1++;}
			C2F(withjavasci)(&irep);
			if (irep) {Str[n1]=javasci;n1++;}
#ifdef WITH_ATLAS
			{Str[n1]=atlas;n1++;}
#endif

#ifdef _MSC_VER
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

	}
	else
	{
		/* Rhs == 1 */
		if (GetType(1) == sci_strings)
		{
			char *Param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			Param=cstk(l1);

			if (strcmp(Param,"scilab")==0)
			{
				int *VERSIONMATRIX=NULL;
				char *versionstring=NULL ;

				VERSIONMATRIX=(int *)MALLOC( (4)*sizeof(int) );

				VERSIONMATRIX[0]=(int)SCI_VERSION_MAJOR;
				VERSIONMATRIX[1]=(int)SCI_VERSION_MINOR;
				VERSIONMATRIX[2]=(int)SCI_VERSION_MAINTENANCE;
				VERSIONMATRIX[3]=(int)SCI_VERSION_REVISION; 

				m1=1;
				n1=4;
				CreateVarFromPtr(1, "i", &m1, &n1 ,&VERSIONMATRIX);
				LhsVar(1)=1;

				if (Lhs==2) 
				{
					n1=1;
					versionstring=(char*)MALLOC((strlen(SCI_VERSION_STRING)+1)*sizeof(char));
					sprintf(versionstring,"%s",SCI_VERSION_STRING);

					CreateVarFromPtr( 2, "c",(m1=strlen(versionstring), &m1),&n1,&versionstring);
					LhsVar(2) =2;
				}
				
				C2F(putlhsvar)();

				if (versionstring)
				{
					FREE(versionstring);
					versionstring=NULL;
				}
				if (VERSIONMATRIX)
				{
					FREE(VERSIONMATRIX);
					VERSIONMATRIX=NULL;
				}
			}
			else
			{
				Scierror(999,"invalid name.\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"must be a string.\n");
			return 0;
		}
	}

	return(0);
}
/*-----------------------------------------------------------------------------------*/
