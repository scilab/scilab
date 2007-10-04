/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "system_getproperty.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_system_getproperty) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs = Max(Rhs,0);
	CheckRhs(1,1);
	CheckLhs(0,1);

	if ( GetType(1) == sci_strings )
	{
		static int m1,n1=0,l1=0;
		char *propertyName = NULL;
		char *propertyValue = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		propertyName=cstk(l1);
		propertyValue=system_getproperty(propertyName,"unknown");

		n1=1;
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(propertyValue), &m1),&n1,&propertyValue);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		if (propertyValue) {FREE(propertyValue);propertyValue=NULL;}
	}
	else
	{
		Scierror(999,"invalid parameter.\n");
		return 0;
	}
	return 0;
}

/*-----------------------------------------------------------------------------------*/

