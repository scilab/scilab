/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SOUND__
#define __GW_SOUND__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

int C2F(gw_sound)(void);

int sci_Beep __PARAMS((char *fname,unsigned long fname_len));
int sci_Playsound __PARAMS((char *fname,unsigned long fname_len));
int sci_savewave __PARAMS((char *fname,unsigned long fname_len));
int sci_loadwave __PARAMS((char *fname,unsigned long fname_len));

typedef int (*sound_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  sound_interf f;    /** function **/
  char *name;      /** its name **/
} intSoundTable;

#endif /*  __GW_SOUND__ */
/*-----------------------------------------------------------------------------------*/
