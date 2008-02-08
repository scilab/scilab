/* Copyright INRIA */
/* Allan CORNET 2006 */

#ifndef SCICOS_DEF 
#define SCICOS_DEF 

#include "machine.h"

#if _LCC_ & FORDLL 
	#define IMPORT __declspec (dllimport)
#else
	#ifdef SCICOS_EXPORTS
		#define IMPORT_SCICOS
	#else
		#ifdef FORDLL 
			#define IMPORT_SCICOS extern  __declspec (dllimport)
		#else 
			#define IMPORT_SCICOS 
		#endif
	#endif
#endif



typedef struct {
  int counter;
} COSDEBUGCOUNTER_struct;

COSDEBUGCOUNTER_struct C2F(cosdebugcounter);

typedef struct {
  int solver;
} SOLVER_struct;

IMPORT_SCICOS  SOLVER_struct C2F(cmsolver);

typedef struct {
  integer kfun;
} CURBLK_struct;

IMPORT_SCICOS CURBLK_struct C2F(curblk);

typedef struct {
  double scale;
}  RTFACTOR_struct;

IMPORT_SCICOS RTFACTOR_struct C2F(rtfactor);


typedef struct {
  integer ptr;
}  SCSPTR_struct;

IMPORT_SCICOS SCSPTR_struct C2F(scsptr);

typedef struct {
	integer idb;
} DBCOS_struct;

IMPORT_SCICOS DBCOS_struct C2F(dbcos);

typedef struct {
	double atol, rtol, ttol, deltat;
} COSTOL_struct;

IMPORT_SCICOS COSTOL_struct C2F(costol);

typedef struct {
	integer halt;
}  COSHLT_struct;

IMPORT_SCICOS COSHLT_struct C2F(coshlt);

typedef struct {
	int cosd;
} COSDEBUG_struct;

IMPORT_SCICOS COSDEBUG_struct C2F(cosdebug);

#endif /*SCICOS_DEF*/
