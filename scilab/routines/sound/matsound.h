/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATSOUND__
#define __MATSOUND__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

typedef int (*sound_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  sound_interf f;    /** function **/
  char *name;      /** its name **/
} intSoundTable;

#endif /*  __MATSOUND__ */
/*-----------------------------------------------------------------------------------*/
