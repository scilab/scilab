/* Copyright INRIA */
#ifndef __CALLINTERF__
#define __CALLINTERF__
/********************************************************
 * Table of hard coded Scilab interface 
 *  to add a new interface add a declaration and a new entry 
 *  at the end of the Interfaces array 
 *  The maximum number of interfaces is bound to DynInterfStart 
 *                                 ( file /core/includes/addinter.h ) 
 ********************************************************/


extern void    /* 01  */ C2F(matlu)(); /* for backward compatibility */
extern void    /* 02  */ C2F(gw_linear_algebra)();
extern void    /* 03  */ C2F(matsvd)();/* for backward compatibility */
extern void    /* 04  */ C2F(matqr)();/* for backward compatibility */
extern void    /* 05  */ C2F(gw_io)();
extern void    /* 06  */ C2F(gw_elementaries_functions)();
extern void    /* 07  */ C2F(gw_graphics)();
extern void    /* 08  */ C2F(gw_cacsd0)();
extern void    /* 09  */ C2F(gw_cacsd1)();
extern void    /* 10  */ C2F(gw_cacsd2)();
extern void    /* 11  */ C2F(gw_optim)();
extern void    /* 12  */ C2F(gw_differential_equations1)();
extern void    /* 13  */ C2F(gw_core)();
extern void    /* 14  */ C2F(gw_user)();
extern void    /* 15  */ C2F(gw_metanet)();
extern void    /* 16  */ C2F(gw_polynomials)();
extern void    /* 17  */ C2F(lstelm)();
extern void    /* 18  */ C2F(gw_signal)();
extern void    /* 19  */ C2F(intinterp)();
extern void    /* 20  */ C2F(gw_cacsd3)();
extern void    /* 21  */ C2F(gw_string)();
extern void    /* 22  */ C2F(gw_symbolic)();
extern void    /* 23  */ C2F(gw_boolean)();
extern void    /* 24  */ C2F(gw_user2)();
extern void    /* 25  */ C2F(gw_gui)();
extern void    /* 26  */ C2F(gw_differential_equations2)();
extern void    /* 27  */ C2F(gw_sparse)();
extern void    /* 28  */ C2F(gw_slicot)();
extern void    /* 29  */ C2F(gw_differential_equations3)();
extern void    /* 30  */ C2F(gw_differential_equations4)();
extern void    /* 31  */ C2F(gw_differential_equations5)();
extern void    /* 32  */ C2F(gw_differential_equations6)();
/* 33  */ /* FREE */
extern void    /* 34  */ C2F(gw_fileio)();
extern void    /* 35  */ C2F(gw_arpack)();
extern void    /* 36  */ C2F(gw_special_functions1)();
extern void    /* 37  */ C2F(gw_statistics)();
extern void    /* 38  */ C2F(gw_randlib)();
extern void    /* 39  */ C2F(gw_special_functions2)();

#ifdef WITH_TK
extern void    /* 40  */ C2F(gw_tclsci)();
#define TCLINTERF C2F(gw_tclsci)
#else 
#define TCLINTERF C2F(no_gw_tclsci)
#endif

extern void    /* 41  */ C2F(lstelmi)();

#ifdef WITH_PVM
extern void    /* 42  */ C2F(gw_pvm)();
#define PVMINTERF C2F(gw_pvm)
#else 
#define PVMINTERF C2F(NoPvm)
#endif

extern void    /* 43  */ C2F(gw_integer)();
extern void    /* 44  */ C2F(gw_linear_algebra2)();

#if WITHOUT_SCICOS
	#define SCICOSINTERF1 C2F(Nogw_scicos)
	#define SCICOSINTERF2 C2F(Nogw_cscicos)
#else
	extern void    /* 45  */ C2F(gw_scicos)();
	#define SCICOSINTERF1 C2F(gw_scicos)
	extern void    /* 46  */ C2F(gw_cscicos)();
	#define SCICOSINTERF2 C2F(gw_cscicos)
#endif

extern void    /* 47  */ C2F(gw_wintools)();
extern void    /* 48  */ C2F(gw_time)();
extern void    /* 49  */ C2F(gw_sound)();

static OpTab Interfaces[] ={
    /* 01  */ {C2F(matlu)},
	/* 02  */ {C2F(gw_linear_algebra)},
	/* 03  */ {C2F(matsvd)},
	/* 04  */ {C2F(matqr)},
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
	/* 17  */ {C2F(lstelm)},
	/* 18  */ {C2F(gw_signal)},
	/* 19  */ {C2F(intinterp)},
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
	/* 46  */ {C2F(gw_arpack)},
	/* 36  */ {C2F(gw_special_functions1)},
	/* 37  */ {C2F(gw_statistics)},
    /* 38  */ {C2F(gw_randlib)},
    /* 39  */ {C2F(gw_special_functions2)},
    /* 40  */ {TCLINTERF},
	/* 41  */ {C2F(lstelmi)},
	/* 42  */ {PVMINTERF},
    /* 43  */ {C2F(gw_integer)},
    /* 44  */ {C2F(gw_linear_algebra2)},
    /* 45  */ {SCICOSINTERF1},  
    /* 46  */ {SCICOSINTERF2},
	/* 47  */ {C2F(gw_wintools)},
	/* 48  */ {C2F(gw_time)},
	/* 49  */ {C2F(gw_sound)}
};

#endif /*__CALLINTERF__*/
