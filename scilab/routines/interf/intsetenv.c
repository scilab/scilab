/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
/* setenv interface */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h> 
#include "../stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int setenvc(char *envstring);
static int ReturnValueSetenv(int value);
/*-----------------------------------------------------------------------------------*/
int	C2F(intsetenv) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	
	Rhs=Max(0,Rhs);
	CheckRhs(1,2);
	CheckLhs(0,1);

	if (Rhs == 2)
	{
		int TypeVar1=GetType(1);
		int TypeVar2=GetType(2);

		if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
		{
			char *param1=NULL,*param2=NULL;
			char *env=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param1=cstk(l1);
			
			GetRhsVar(2,"c",&m1,&n1,&l1);
			param2=cstk(l1);

			env=(char*)malloc((strlen(param1)+strlen(param2)+2)*sizeof(char));
			sprintf(env,"%s=%s",param1,param2);

			ReturnValueSetenv(setenvc(env));

			if (env) free(env);
		}
		else
		{
			Scierror(999,"Invalid type : string recquired");
		}
	}
	else
	{
		/* Rhs == 1 */
		int TypeVar1=GetType(1);
		if (TypeVar1 == sci_strings)
		{
			char *param1=NULL;
		
			GetRhsVar(1,"c",&m1,&n1,&l1);
			param1=cstk(l1);

			ReturnValueSetenv(setenvc(param1));
		}
		else
		{
			Scierror(999,"Invalid type : string recquired");
		}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int ReturnValueSetenv(int value)
{
	#define TRUE  1
	#define FALSE 0
	static int n1;
	int *paramoutINT=(int*)malloc(sizeof(int));

	if (value == TRUE) *paramoutINT=(int)(TRUE);
	else  *paramoutINT=(int)(FALSE);

	n1=1;
	CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
	LhsVar(1)=1;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
