/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_TIME__
#define __GW_TIME__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

typedef int (*time_interf) __PARAMS((char *fname,unsigned long fname_len));

/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getdate) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_calendar) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_timer) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sleep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xpause) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_realtimeinit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_realtime) _PARAMS((char *fname,unsigned long fname_len));

int C2F(gw_time)(void);

typedef struct table_struct {
  time_interf f;    /** function **/
  char *name;      /** its name **/
} intTimeTable;

#endif /*  __GW_TIME__ */
/*-----------------------------------------------------------------------------------*/
