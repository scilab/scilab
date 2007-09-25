/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "error_scilab.h"
#include "MALLOC.h"
#include "md5.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define stricmp _stricmp
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getmd5) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *output=NULL ;

	Rhs=Max(Rhs,0);
	CheckRhs(1,2) ;
	CheckLhs(1,1) ;

	if (Rhs==1)
	{
		if (GetType(1) == sci_strings)
		{
			char *Param1=NULL;
			FILE *fp=NULL;
			char *MD5=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			Param1=cstk(l1);

			fp = fopen(Param1, "rb");
			if (fp)
			{
				MD5=md5_file(fp);
				fclose(fp);

				output=(char*)MALLOC((strlen(MD5)+1)*sizeof(char));
				sprintf(output,"%s",MD5);
				if (MD5) {FREE(MD5);MD5=NULL;}

				n1=1;
				CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
				if (output) {FREE(output);output=NULL;}

				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
			}
			else
			{
				error_scilab(999,"file %s doesn't exist.",Param1);
			}
		}
		else
		{
			error_scilab(999,"Invalid type parameter. it must be a string (a filename).");
		}
	}
	else /* Rhs == 2 */
	{
		if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
		{
			char *Param1=NULL;
			char *Param2=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			Param1=cstk(l1);

			GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
			Param2=cstk(l1);

#ifdef _MSC_VER
			if ( stricmp(Param2,"string")==0 )
#else
			if ( strcmp(Param2,"string")==0 )
#endif
			{
				char *MD5=NULL;
				MD5=md5_str(Param1);

				output=(char*)MALLOC((strlen(MD5)+1)*sizeof(char));
				sprintf(output,"%s",MD5);
				if (MD5) {FREE(MD5);MD5=NULL;}

				n1=1;
				CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
				if (output) {FREE(output);output=NULL;}

				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
			}
			else
			{
				error_scilab(999,"Invalid second parameter. It must be "string".");
			}
		}
		else
		{
			error_scilab(999,"Invalid type parameter. parameters must be strings.");
		}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
