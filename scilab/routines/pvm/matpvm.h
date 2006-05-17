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

#include "../machine.h"
#include "../stack-c.h"

typedef int mxArray;
typedef int (*GatefuncS) __PARAMS((char *fname, int l)); 
typedef int Gatefunc __PARAMS((int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]));
typedef int (*FGatefuncH) __PARAMS((int *nlhs,mxArray *plhs[],int *nrhs,mxArray *prhs[]));
typedef int (*GT) ();
typedef int (*GatefuncH) __PARAMS((int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]));
typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));

typedef struct table_struct {
  Myinterfun f;    /** interface **/
  GT F;     /** function **/
  char *name;      /** its name **/
} GenericTable;


#endif /*  __INTMATPVM__ */
/*-----------------------------------------------------------------------------------*/

