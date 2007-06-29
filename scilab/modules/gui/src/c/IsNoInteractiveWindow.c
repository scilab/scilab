/*-----------------------------------------------------------------------------------*/
/* INRIA 2006/2007 */
/* @author Vincent Couvert */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 

#include "IsNoInteractiveWindow.h"
#include "scilabmode.h"

int IsNoInteractiveWindow(void)
{
	return (getScilabMode() == SCILAB_NWNI);
}
