/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "ifexpr.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(ifexpr)(void)
{
	#define blank  40
	int p = 0;

	if (C2F(errgst).err1 != 0) return FALSE_;
	p = C2F(recu).pt;
	do 
	{
		--p;
		/* the test rstk(p).eq.307.and.ids(1,p-1).ne.blank) checks if we are */
		/* evaluating the argument of a function. ids(1,p-1).ne.blank */
		/* excludes the case of a parenthesized if expression */
		if (p == 0 || (C2F(recu).rstk[p - 1] == 307 && C2F(recu).ids[(p - 1) * 6 - 6] != blank)) return FALSE_;
	} while(C2F(recu).rstk[p - 1] != 803);
	return TRUE_;
}
/*-----------------------------------------------------------------------------------*/ 


