/* Copyright INRIA */
/* Allan CORNET 2006 */

#ifndef SCICOS_DEF 
#define SCICOS_DEF 

#include "machine.h"

#if _LCC_ & FORDLL 
  #define IMPORT __declspec (dllimport)
#else
 #ifdef FORDLL 
   #define IMPORT extern  __declspec (dllimport)
 #else 
   #define IMPORT 
 #endif
#endif

typedef struct {
  int cosd;
} COSDEBUG_struct;

IMPORT COSDEBUG_struct C2F(cosdebug);


typedef struct {
  int counter;
} COSDEBUGCOUNTER_struct;

COSDEBUGCOUNTER_struct C2F(cosdebugcounter);

typedef struct {
  int solver;
} SOLVER_struct;

IMPORT  SOLVER_struct C2F(cmsolver);

typedef struct {
  integer iero;
} IERODE_struct;

IMPORT IERODE_struct C2F(ierode);

typedef struct {
  integer kfun;
} CURBLK_struct;

IMPORT CURBLK_struct C2F(curblk);

typedef struct {
  double scale;
}  RTFACTOR_struct;

IMPORT RTFACTOR_struct C2F(rtfactor);

typedef struct {
  integer halt;
}  COSHLT_struct;

IMPORT COSHLT_struct C2F(coshlt);

typedef struct {
  integer ptr;
}  SCSPTR_struct;

IMPORT SCSPTR_struct C2F(scsptr);



#endif /*SCICOS_DEF*/
