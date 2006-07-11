/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intOEMconvert.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
#if defined (linux)
#include <string.h>
#endif

/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  extern BOOL IsWindowInterface(void);
#endif
/*-----------------------------------------------------------------------------------*/

int C2F(intoemtochar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int bOK=FALSE;
	char *Output=NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)	
	{
		char *OEMstring=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		OEMstring=cstk(l1);
		
		Output=(char*)MALLOC((strlen(OEMstring)+1)*sizeof(char));
	#if WIN32
		if (IsWindowInterface())
		{
				OemToChar(OEMstring,Output);
		}
		else
		{
			wsprintf(Output,"%s",OEMstring);
		}
	#else
		sprintf(Output,"%s",OEMstring);
	#endif
	}
	else
	{
		Scierror(999,"parameter must be a string.");
		return 0;
	}
	
	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = 1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intchartooem) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int bOK=FALSE;
	char *Output=NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)	
	{
		char *Charstring=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		Charstring=cstk(l1);

		Output=(char*)MALLOC((strlen(Charstring)+1)*sizeof(char));
	#if WIN32
		if (IsWindowInterface())
		{
			CharToOem(Charstring,Output);
		}
		else
		{
			wsprintf(Output,"%s",Charstring);
		}
	#else
			sprintf(Output,"%s",Charstring);
	#endif
	}
	else
	{
		Scierror(999,"parameter must be a string.");
		return 0;
	}
	
	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = 1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
