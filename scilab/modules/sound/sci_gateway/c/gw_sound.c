/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_sound.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
typedef int (*sound_interf) __PARAMS((char *fname,unsigned long fname_len));
typedef struct table_struct {
	sound_interf f;    /** function **/
	char *name;      /** its name **/
} intSoundTable;
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
