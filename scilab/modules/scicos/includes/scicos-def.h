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


typedef struct {
  int solver;
} SOLVER_struct;


typedef struct {
  integer kfun;
} CURBLK_struct;


typedef struct {
  double scale;
}  RTFACTOR_struct;



typedef struct {
  integer ptr;
}  SCSPTR_struct;


typedef struct {
	integer idb;
} DBCOS_struct;


typedef struct {
	double atol, rtol, ttol, deltat;
} COSTOL_struct;


typedef struct {
	integer halt;
}  COSHLT_struct;


typedef struct {
	int cosd;
} COSDEBUG_struct;


#endif /*SCICOS_DEF*/
