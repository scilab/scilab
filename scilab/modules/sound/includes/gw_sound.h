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

typedef int (*sound_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  sound_interf f;    /** function **/
  char *name;      /** its name **/
} intSoundTable;

#endif /*  __GW_SOUND__ */
/*-----------------------------------------------------------------------------------*/
