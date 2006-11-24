/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Signal_Interf) __PARAMS((char *fname,unsigned long l));
int C2F(gw_signal)(void);
typedef struct table_struct 
{
  Signal_Interf f;    /** function **/
  char *name;      /** its name **/
} SignalTable;
/*-----------------------------------------------------------------------------------*/
int C2F(sci_ffir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fft) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_corr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fiir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rpem) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_amell) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_delip) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_remez) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_syredi) _PARAMS((char *fname,unsigned long fname_len));
#endif /*  __GW_SIGNAL__ */
/*-----------------------------------------------------------------------------------*/

