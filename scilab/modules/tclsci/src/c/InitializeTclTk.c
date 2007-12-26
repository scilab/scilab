/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "InitializeTclTk.h"
#include "InitTclTk.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/ 
/*--------------------------------------------------------------------------*/ 
BOOL InitializeTclTk(void)
{
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if ( OpenTCLsci()==0 )
		{
			setTkStarted(TRUE);
			return TRUE;
		}
		else
		{
			setTkStarted(FALSE);
		}
	}
	else
	{
		setTkStarted(FALSE);
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
