/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/ 
/* default mode is API */
static scilabMode CurrentScilabMode = SCILAB_API;
/*--------------------------------------------------------------------------*/ 
int getScilabMode(void)
{
	return CurrentScilabMode;
}
/*--------------------------------------------------------------------------*/ 
BOOL setScilabMode(int newmode)
{
	if ( (newmode == SCILAB_API) || (newmode == SCILAB_STD) || (newmode == SCILAB_NW) || (newmode == SCILAB_NWNI) )
	{
		CurrentScilabMode = (scilabMode)newmode;
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
int C2F(getscilabmode)(int *mode)
{
	*mode=getScilabMode();
	return 0;
}
/*--------------------------------------------------------------------------*/ 
