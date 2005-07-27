/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGetVar.h"
/*-----------------------------------------------------------------------------------*/
#define AddCharacters 4
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGetVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);

	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if (TypeVar1 == sci_strings)
	{
		char *VarName=NULL;
		char *RetStr=NULL;
		

		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR17);
			return 0;
		}

		RetStr= (char*)Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

		if ( RetStr )
		{
			char *AsciiFromUTF8=NULL;
			char *output=NULL ;

			AsciiFromUTF8=MALLOC(sizeof(char)*(strlen(RetStr)+AddCharacters));

			/* UTF to ANSI */
			Tcl_UtfToExternal(TCLinterp, NULL, RetStr, strlen(RetStr), 0, NULL, AsciiFromUTF8, (int)(strlen(RetStr)+AddCharacters), NULL, NULL,NULL);

			output=(char*)MALLOC((strlen(AsciiFromUTF8)+1)*sizeof(char));
			sprintf(output,"%s",AsciiFromUTF8);
			CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
			
			LhsVar(1) = 1;
			C2F(putlhsvar)();

			if (output) {FREE(output);output=NULL;}
			if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}
	  	}
		else
		{
			Scierror(999,TCL_ERROR18);
			return 0;
		}
		
	}
	else
	{
		 Scierror(999,TCL_ERROR19);
		 return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
