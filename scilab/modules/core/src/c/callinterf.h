/* Copyright INRIA */
#ifndef __CALLINTERF__
#define __CALLINTERF__
#include "machine.h"

/********************************************************
 * Table of hard coded Scilab interface 
 *  to add a new interface add a declaration and a new entry 
 *  at the end of the Interfaces array 
 *  The maximum number of interfaces is bound to DynInterfStart 
 *                                 ( file /core/src/c/addinter.h ) 
 ********************************************************/

/** table of interfaces **/

typedef  struct  {
  void  (*fonc)();
} OpTab ;


void errjump(int n);
void C2F(no_gw_tclsci)(void);
void C2F(NoPvm)(void);
void C2F(Nogw_scicos)(void);
void C2F(Nogw_cscicos)(void);
void C2F(Nogw_slicot)(void);
void C2F(Nogw_fftw)(void);

static void sci_sigint_addinter(int n);


/* 01  C2F(gw_user)(); FREE */
extern void    /* 02  */ C2F(gw_linear_algebra)(void);
/* 03  C2F(gw_user)(); FREE */
/* 04  C2F(gw_user)(); FREE */
extern void    /* 05  */ C2F(gw_io)(void);
extern void    /* 06  */ C2F(gw_elementaries_functions)(void);
extern void    /* 07  */ C2F(gw_graphics)(void);
extern void    /* 08  */ C2F(gw_cacsd0)(void);
extern void    /* 09  */ C2F(gw_cacsd1)(void);
extern void    /* 10  */ C2F(gw_cacsd2)(void);
extern void    /* 11  */ C2F(gw_optim)(void);
extern void    /* 12  */ C2F(gw_differential_equations1)(void);
extern void    /* 13  */ C2F(gw_core)(void);
extern void    /* 14  */ C2F(gw_user)(void);
extern void    /* 15  */ C2F(gw_metanet)(void);
extern void    /* 16  */ C2F(gw_polynomials)(void);
extern void    /* 17  */ C2F(gwdatastructures1)(void);
extern void    /* 18  */ C2F(gw_signal)(void);
extern void    /* 19  */ C2F(gw_interpolation)(void);
extern void    /* 20  */ C2F(gw_cacsd3)(void);
extern void    /* 21  */ C2F(gw_string)(void);
extern void    /* 22  */ C2F(gw_symbolic)(void);
extern void    /* 23  */ C2F(gw_boolean)(void);
extern void    /* 24  */ C2F(gw_user2)(void);
extern void    /* 25  */ C2F(gw_gui)(void);
extern void    /* 26  */ C2F(gw_differential_equations2)(void);
extern void    /* 27  */ C2F(gw_sparse)(void);
extern void    /* 28  */ C2F(gw_slicot)(void);
extern void    /* 29  */ C2F(gw_differential_equations3)(void);
extern void    /* 30  */ C2F(gw_differential_equations4)(void);
extern void    /* 31  */ C2F(gw_differential_equations5)(void);
extern void    /* 32  */ C2F(gw_differential_equations6)(void);
/* 33  */ /* FREE */
extern void    /* 34  */ C2F(gw_fileio)(void);
extern void    /* 35  */ C2F(gw_arnoldi)(void);
extern void    /* 36  */ C2F(gw_special_functions1)(void);
extern void    /* 37  */ C2F(gw_statistics)(void);
extern void    /* 38  */ C2F(gw_randlib)(void);
extern void    /* 39  */ C2F(gw_special_functions2)(void);

#ifdef WITH_TK
extern void    /* 40  */ C2F(gw_tclsci)(void);
#define TCLINTERF C2F(gw_tclsci)
#else 
#define TCLINTERF C2F(no_gw_tclsci)
#endif

extern void    /* 41  */ C2F(gwdatastructures2)(void);

#ifdef WITH_PVM
extern void    /* 42  */ C2F(gw_pvm)(void);
#define PVMINTERF C2F(gw_pvm)
#else 
#define PVMINTERF C2F(NoPvm)
#endif

extern void    /* 43  */ C2F(gw_integer)(void);
extern void    /* 44  */ C2F(gw_linear_algebra2)(void);

#ifdef WITHOUT_SCICOS
	#define SCICOSINTERF1 C2F(Nogw_scicos)
	#define SCICOSINTERF2 C2F(Nogw_cscicos)
#else
	extern void    /* 45  */ C2F(gw_scicos)(void);
	#define SCICOSINTERF1 C2F(gw_scicos)
	extern void    /* 46  */ C2F(gw_cscicos)(void);
	#define SCICOSINTERF2 C2F(gw_cscicos)
#endif

extern void    /* 47  */ C2F(gw_wintools)(void);
extern void    /* 48  */ C2F(gw_time)(void);
extern void    /* 49  */ C2F(gw_sound)(void);
extern void    /* 50  */ C2F(gw_localization)(void);

#if defined(WITH_FFTW) || defined(_MSC_VER)
extern void    /* 51  */ C2F(gw_fftw)(void);
#define FFTWINTERF C2F(gw_fftw)
#else 
#define FFTWINTERF C2F(Nogw_fftw)
#endif

static OpTab Interfaces[] = {
    /* 01  */ {C2F(gw_user)}, /* free position may be used */
	/* 02  */ {C2F(gw_linear_algebra)},
	/* 03  */ {C2F(gw_user)}, /* free position may be used */
	/* 04  */ {C2F(gw_user)}, /* free position may be used */
	/* 05  */ {C2F(gw_io)},
	/* 06  */ {C2F(gw_elementaries_functions)},
	/* 07  */ {C2F(gw_graphics)},
	/* 08  */ {C2F(gw_cacsd0)},
	/* 09  */ {C2F(gw_cacsd1)},
	/* 10  */ {C2F(gw_cacsd2)},
	/* 11  */ {C2F(gw_optim)},
	/* 12  */ {C2F(gw_differential_equations1)},
	/* 13  */ {C2F(gw_core)},
	/* 14  */ {C2F(gw_user)},
	/* 15  */ {C2F(gw_metanet)},
	/* 16  */ {C2F(gw_polynomials)},
	/* 17  */ {C2F(gwdatastructures1)},
	/* 18  */ {C2F(gw_signal)},
	/* 19  */ {C2F(gw_interpolation)},
	/* 20  */ {C2F(gw_cacsd3)},
	/* 21  */ {C2F(gw_string)},
	/* 22  */ {C2F(gw_symbolic)},
	/* 23  */ {C2F(gw_boolean)},
	/* 24  */ {C2F(gw_user2)},
	/* 25  */ {C2F(gw_gui)},
	/* 26  */ {C2F(gw_differential_equations2)},
	/* 27  */ {C2F(gw_sparse)},
	/* 28  */ {C2F(gw_slicot)},
	/* 29  */ {C2F(gw_differential_equations3)},
	/* 30  */ {C2F(gw_differential_equations4)},
	/* 31  */ {C2F(gw_differential_equations5)},
	/* 32  */ {C2F(gw_differential_equations6)},
    /* 33  */ {C2F(gw_user2)}, /* free position may be used */
	/* 34  */ {C2F(gw_fileio)},
	/* 46  */ {C2F(gw_arnoldi)},
	/* 36  */ {C2F(gw_special_functions1)},
	/* 37  */ {C2F(gw_statistics)},
    /* 38  */ {C2F(gw_randlib)},
    /* 39  */ {C2F(gw_special_functions2)},
    /* 40  */ {TCLINTERF},
	/* 41  */ {C2F(gwdatastructures2)},
	/* 42  */ {PVMINTERF},
    /* 43  */ {C2F(gw_integer)},
    /* 44  */ {C2F(gw_linear_algebra2)},
    /* 45  */ {SCICOSINTERF1},  
    /* 46  */ {SCICOSINTERF2},
	/* 47  */ {C2F(gw_wintools)},
	/* 48  */ {C2F(gw_time)},
	/* 49  */ {C2F(gw_sound)},
	/* 50  */ {C2F(gw_localization)},
	/* 51  */ {FFTWINTERF}
};


/**
 * call the apropriate interface according to the value of k 
 * iflagint is no more used here ....
 * @param k the number of the interface
 * @return 
 */
int C2F(callinterf) (int *k);

#endif /*__CALLINTERF__*/
