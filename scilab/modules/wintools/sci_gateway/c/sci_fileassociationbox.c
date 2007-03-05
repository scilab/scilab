/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
/*-----------------------------------------------------------------------------------*/
extern void Callback_FILESASSOCIATIONBOX(void);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_filesassociationbox) _PARAMS((char *fname,unsigned long l))
{
	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	#ifdef _MSC_VER
		Callback_FILESASSOCIATIONBOX();
	#endif
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
