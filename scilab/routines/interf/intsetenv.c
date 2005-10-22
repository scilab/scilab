/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
/* setenv interface */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h> 
#include "../stack-c.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/*-----------------------------------------------------------------------------------*/
extern int setenvc(char *string,char * value);
static int ReturnValueSetenv(int value);
/*-----------------------------------------------------------------------------------*/
int	C2F(intsetenv) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	Rhs=Max(0,Rhs);
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
	{
		char *param1=NULL,*param2=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);
			
		GetRhsVar(2,"c",&m1,&n1,&l1);
		param2=cstk(l1);

		ReturnValueSetenv(setenvc(param1,param2));
	}
	else
	{
		Scierror(999,"Invalid type : string recquired");
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int ReturnValueSetenv(int value)
{
	#define TRUE  1
	#define FALSE 0
	static int n1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	if (value == TRUE) *paramoutINT=(int)(TRUE);
	else  *paramoutINT=(int)(FALSE);

	n1=1;
	CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
    LhsVar(1)=1;
	C2F(putlhsvar)();
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
