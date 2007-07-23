/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/ 

#ifndef MAX_PATH_LONG
#define MAX_PATH_LONG 32767
#endif

#ifndef MAX_PATH_SHORT
#define MAX_PATH_SHORT 260
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getlongpathname) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int bOK=FALSE;

	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		char *LongName=NULL;
		char *ShortName=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		ShortName=cstk(l1);

		LongName=(char*)MALLOC(MAX_PATH_LONG*sizeof(char));

		#ifdef _MSC_VER
		  if (GetLongPathName(ShortName,LongName,MAX_PATH_LONG))
		  {
			  bOK=TRUE;
		  }
		  else
		  {
			  strcpy(LongName,ShortName);
			  bOK=FALSE;
		  }
		#else
		  strcpy(LongName,ShortName);
		  bOK=FALSE;
		#endif

		n1=1;
		CreateVarFromPtr(Rhs+ 2, "c",(m1=(int)strlen(LongName), &m1),&n1,&LongName);
		LhsVar(1)=Rhs+2;

		if (LongName) {FREE(LongName);LongName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)MALLOC(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(Rhs+3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=Rhs+3;
			if (bOkOutINT) {FREE(bOkOutINT);bOkOutINT=NULL;}
		}

		C2F(putlhsvar)();
	
	}
	else 
	{
		Scierror(999,"Not a path string");
		return 0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getshortpathname) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int bOK=FALSE;

	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		char *LongName=NULL;
		char *ShortName=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		LongName=cstk(l1);

		ShortName=(char*)MALLOC((strlen(LongName)+1)*sizeof(char));

		#ifdef _MSC_VER
		  if (GetShortPathName(LongName,ShortName,(DWORD)strlen(LongName)+1))
		  {
			  bOK=TRUE;
		  }
		  else
		  {
			  strcpy(ShortName,LongName);
			  bOK=FALSE;
		  }
		#else
		  strcpy(ShortName,LongName);
		  bOK=FALSE;
		#endif

		n1=1;
		CreateVarFromPtr( Rhs+2, "c",(m1=(int)strlen(ShortName), &m1),&n1,&ShortName);
		LhsVar(1)=Rhs+2;

		if (ShortName) {FREE(ShortName);ShortName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)MALLOC(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(Rhs+3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=Rhs+3;
			if (bOkOutINT) {FREE(bOkOutINT);bOkOutINT=NULL;}
		}

		C2F(putlhsvar)();
	
	}
	else 
	{
		Scierror(999,"Not a path string");
		return 0;
	}
	return 0;
	
}
/*-----------------------------------------------------------------------------------*/
