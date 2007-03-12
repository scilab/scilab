#include "gw_sound.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "sciprint.h"
#include "gw_sound.h"

/*-----------------------------------------------------------------------------------*/ 
static intSoundTable Tab[]={
	{ sci_savewave, "savewave"},
	{ sci_loadwave, "loadwave"},
	{ sci_Playsound,"PlaySound"},
	{ sci_Beep,"beep"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_sound)()
{  
	Rhs = Max(0, Rhs);
	#ifdef _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
