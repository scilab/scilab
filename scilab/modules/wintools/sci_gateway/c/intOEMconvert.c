/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  extern BOOL IsWindowInterface(void);
#endif
/*-----------------------------------------------------------------------------------*/

int C2F(sci_oemtochar) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)	
	{
		char *OEMstring=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		OEMstring=cstk(l1);
		
		Output=(char*)MALLOC((strlen(OEMstring)+1)*sizeof(char));
	#ifdef _MSC_VER
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
	CreateVarFromPtr(Rhs+ 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(sci_chartooem) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)	
	{
		char *Charstring=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		Charstring=cstk(l1);

		Output=(char*)MALLOC((strlen(Charstring)+1)*sizeof(char));
	#ifdef _MSC_VER
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
	CreateVarFromPtr( Rhs+1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
