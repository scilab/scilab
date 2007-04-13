/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "sciprint.h"
#include "cluni0.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  extern void SetLanguageMenu(char *Language); /* see "wsci/wmenu.c" */
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_setlanguagemenu) _PARAMS((char *fname,unsigned long l))
{
	#define LANGUAGE_MAX 1024 
	
	char Language[LANGUAGE_MAX];
	int m1,n1,l1;
	int out_n;
	long int lout;

	CheckRhs(1,1);
  
	/*  checking variable file */
	GetRhsVar(1,"c",&m1,&n1,&l1);
	/*** first call to get the size **/
	lout=LANGUAGE_MAX;
	C2F(cluni0)(cstk(l1), Language, &out_n,m1*n1,lout);

	if ( (strcmp(Language,"en")!=0) && (strcmp(Language,"fr")!=0) )
	{
		sciprint("Unsupported language\n");
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	}

	#ifdef _MSC_VER
		SetLanguageMenu(Language);
	#else

	#endif

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
