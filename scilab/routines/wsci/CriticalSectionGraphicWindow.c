/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CriticalSectionGraphicWindow.h"
/*-----------------------------------------------------------------------------------*/
static CRITICAL_SECTION Sync_scig_buzy;
/*-----------------------------------------------------------------------------------*/
static int init_C_S_Sync_scig_buzy=0;
/*-----------------------------------------------------------------------------------*/
void Setscig_buzyState(BOOL state)
{
	if (init_C_S_Sync_scig_buzy==0)
	{
		InitializeCriticalSection(&Sync_scig_buzy);
		init_C_S_Sync_scig_buzy++;
	}

	if (state == TRUE)
	{
		EnterCriticalSection(&Sync_scig_buzy);
	}
	else
	{
		LeaveCriticalSection(&Sync_scig_buzy);
	}
}
/*-----------------------------------------------------------------------------------*/
