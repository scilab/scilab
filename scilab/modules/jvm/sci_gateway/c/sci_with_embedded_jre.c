/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "JVM_functions.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_with_embedded_jre) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1=0,l1=0;
	n1=1;
	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
	*istk(l1)=(int)withEmbeddedJRE();
	
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
