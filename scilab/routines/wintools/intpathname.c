/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intpathname.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intgetlongpathname) _PARAMS((char *fname))
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

		LongName=(char*)malloc(MAX_PATH_LONG*sizeof(char));

		#ifdef WIN32
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
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(LongName), &m1),&n1,&LongName);
		LhsVar(1)=2;

		if (LongName) {free(LongName);LongName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)malloc(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=3;
			if (bOkOutINT) {free(bOkOutINT);bOkOutINT=NULL;}
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
int C2F(intgetshortpathname) _PARAMS((char *fname))
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

		ShortName=(char*)malloc((strlen(LongName)+1)*sizeof(char));

		#ifdef WIN32
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
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(ShortName), &m1),&n1,&ShortName);
		LhsVar(1)=2;

		if (ShortName) {free(ShortName);ShortName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)malloc(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=3;
			if (bOkOutINT) {free(bOkOutINT);bOkOutINT=NULL;}
		}

		C2F(putlhsvar)();
	
	}
	else 
	{
		Scierror(999,"Not a path string");
		return 0;
	}
	return 0;
	
}/*-----------------------------------------------------------------------------------*/
