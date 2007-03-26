/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTMATPVM__
#define __INTMATPVM__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

int C2F(gw_pvm)(void);
/*
typedef int mxArray;
typedef int (*GatefuncS) __PARAMS((char *fname, int l)); 
typedef int Gatefunc __PARAMS((int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]));
typedef int (*FGatefuncH) __PARAMS((int *nlhs,mxArray *plhs[],int *nrhs,mxArray *prhs[]));
typedef int (*GT) ();
typedef int (*GatefuncH) __PARAMS((int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]));
typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));
*/
typedef int (*PVM_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  PVM_Interf f;     /** function **/
  char *name;      /** its name **/
} PVMTable;


int intspvm_addhosts _PARAMS((char *fname,unsigned long fname_len));
int intspvm_barrier _PARAMS((char *fname,unsigned long fname_len));
int intspvm_bcast _PARAMS((char *fname,unsigned long fname_len));
int intspvm_bufinfo _PARAMS((char *fname,unsigned long fname_len));
int intspvm_config _PARAMS((char *fname,unsigned long fname_len));
int intspvm_delhosts _PARAMS((char *fname,unsigned long fname_len));
int intspvm_error _PARAMS((char *fname,unsigned long fname_len));
int intspvm_error_mode _PARAMS((char *fname,unsigned long fname_len));
int intspvm_exit _PARAMS((char *fname,unsigned long fname_len));
int intspvm_f772sci _PARAMS((char *fname,unsigned long fname_len));
int intspvm_getinst _PARAMS((char *fname,unsigned long fname_len));
int intspvm_gettid _PARAMS((char *fname,unsigned long fname_len));
int intspvm_get_timer _PARAMS((char *fname,unsigned long fname_len));
int intspvm_gsize _PARAMS((char *fname,unsigned long fname_len));
int intspvm_halt _PARAMS((char *fname,unsigned long fname_len));
int intspvm_joingroup _PARAMS((char *fname,unsigned long fname_len));
int intspvm_kill _PARAMS((char *fname,unsigned long fname_len));
int intspvm_lvgroup _PARAMS((char *fname,unsigned long fname_len));
int intspvm_mytid _PARAMS((char *fname,unsigned long fname_len));
int intspvm_parent _PARAMS((char *fname,unsigned long fname_len));
int intspvm_probe _PARAMS((char *fname,unsigned long fname_len));
int intspvm_recv _PARAMS((char *fname,unsigned long fname_len));
int intspvm_recv_var _PARAMS((char *fname,unsigned long fname_len));
int intspvm_reduce _PARAMS((char *fname,unsigned long fname_len));
int intspvm_sci2f77 _PARAMS((char *fname,unsigned long fname_len));
int intspvm_send _PARAMS((char *fname,unsigned long fname_len));
int intspvm_send_var _PARAMS((char *fname,unsigned long fname_len));
int intspvm_set_timer _PARAMS((char *fname,unsigned long fname_len));
int intspvm_spawn _PARAMS((char *fname,unsigned long fname_len));
int intspvm_spawn_independent _PARAMS((char *fname,unsigned long fname_len));
int intspvm_start _PARAMS((char *fname,unsigned long fname_len));
int intspvm_tasks _PARAMS((char *fname,unsigned long fname_len));
int intspvm_tidtohost _PARAMS((char *fname,unsigned long fname_len));


#endif /*  __INTMATPVM__ */
/*-----------------------------------------------------------------------------------*/
