/* Copyright INRIA */
#ifndef __CALLINTERF__
#define __CALLINTERF__
/********************************************************
 * Table of hard coded Scilab interface 
 *  to add a new interface add a declaration and a new entry 
 *  at the end of the Interfaces array 
 *  The maximum number of interfaces is bound to DynInterfStart 
 *                                 ( file ../sun/addinter.h ) 
 ********************************************************/


extern void    /* 01  */ C2F(matlu)(); /* for backward compatibility */
extern void    /* 02  */ C2F(matdsr)();
extern void    /* 03  */ C2F(matsvd)();/* for backward compatibility */
extern void    /* 04  */ C2F(matqr)();/* for backward compatibility */
extern void    /* 05  */ C2F(matio)();
extern void    /* 06  */ C2F(matelm)();
extern void    /* 07  */ C2F(matdes)();
extern void    /* 08  */ C2F(matqz)();
extern void    /* 09  */ C2F(matric)();
extern void    /* 10  */ C2F(matnew)();
extern void    /* 11  */ C2F(matopt)();
extern void    /* 12  */ C2F(matode)();
extern void    /* 13  */ C2F(matsys)();
extern void    /* 14  */ C2F(matusr)();
extern void    /* 15  */ C2F(gw_metanet)();
extern void    /* 16  */ C2F(polelm)();
extern void    /* 17  */ C2F(lstelm)();
extern void    /* 18  */ C2F(sigelm)();
extern void    /* 19  */ C2F(intinterp)();
extern void    /* 20  */ C2F(polaut)();
extern void    /* 21  */ C2F(strelm)();
extern void    /* 22  */ C2F(fmlelm)();
extern void    /* 23  */ C2F(logelm)();
extern void    /* 24  */ C2F(matus2)();
extern void    /* 25  */ C2F(xawelm)();
extern void    /* 26  */ C2F(matimp)();
extern void    /* 27  */ C2F(spelm)();
extern void    /* 28  */ C2F(intslicot)();
extern void    /* 29  */ C2F(matodc)();
extern void    /* 30  */ C2F(defint)();
extern void    /* 31  */ C2F(feval)();
extern void    /* 32  */ C2F(bva)();
/* 33  */ /* FREE */
extern void    /* 34  */ C2F(matfileio)();
extern void    /* 35  */ C2F(intarpack)();
extern void    /* 36  */ C2F(specfun)();
extern void    /* 37  */ C2F(dcd)();
extern void    /* 38  */ C2F(randlib)();
extern void    /* 39  */ C2F(otherspfunlib)();

#ifdef WITH_TK
extern void    /* 40  */ C2F(gw_tclsci)();
#define TCLINTERF C2F(gw_tclsci)
#else 
#define TCLINTERF C2F(no_gw_tclsci)
#endif

extern void    /* 41  */ C2F(lstelmi)();

#ifdef WITH_PVM
extern void    /* 42  */ C2F(intpvm)();
#define PVMINTERF C2F(intpvm)
#else 
#define PVMINTERF C2F(NoPvm)
#endif

extern void    /* 43  */ C2F(intelm)();
extern void    /* 44  */ C2F(intlapack)();

#if WITHOUT_SCICOS
	#define SCICOSINTERF1 C2F(Nointscicos)
	#define SCICOSINTERF2 C2F(Nointcscicos)
#else
	extern void    /* 45  */ C2F(intscicos)();
	#define SCICOSINTERF1 C2F(intscicos)
	extern void    /* 46  */ C2F(intcscicos)();
	#define SCICOSINTERF2 C2F(intcscicos)
#endif

extern void    /* 47  */ C2F(intwintools)();
extern void    /* 48  */ C2F(mattime)();
extern void    /* 49  */ C2F(gw_sound)();

static OpTab Interfaces[] ={
  /* 01  */ {C2F(matlu)},
	/* 02  */ {C2F(matdsr)},
	/* 03  */ {C2F(matsvd)},
	/* 04  */ {C2F(matqr)},
	/* 05  */ {C2F(matio)},
	/* 06  */ {C2F(matelm)},
	/* 07  */ {C2F(matdes)},
	/* 08  */ {C2F(matqz)},
	/* 09  */ {C2F(matric)},
	/* 10  */ {C2F(matnew)},
	/* 11  */ {C2F(matopt)},
	/* 12  */ {C2F(matode)},
	/* 13  */ {C2F(matsys)},
	/* 14  */ {C2F(matusr)},
	/* 15  */ {C2F(gw_metanet)},
	/* 16  */ {C2F(polelm)},
	/* 17  */ {C2F(lstelm)},
	/* 18  */ {C2F(sigelm)},
	/* 19  */ {C2F(intinterp)},
	/* 20  */ {C2F(polaut)},
	/* 21  */ {C2F(strelm)},
	/* 22  */ {C2F(fmlelm)},
	/* 23  */ {C2F(logelm)},
	/* 24  */ {C2F(matus2)},
	/* 25  */ {C2F(xawelm)},
	/* 26  */ {C2F(matimp)},
	/* 27  */ {C2F(spelm)},
	/* 28  */ {C2F(intslicot)},
	/* 29  */ {C2F(matodc)},
	/* 30  */ {C2F(defint)},
	/* 31  */ {C2F(feval)},
	/* 32  */ {C2F(bva)},
  /* 33  */ {C2F(matus2)}, /* free position may be used */
	/* 34  */ {C2F(matfileio)},
	/* 46  */ {C2F(intarpack)},
	/* 36  */ {C2F(specfun)},
	/* 37  */ {C2F(dcd)},
  /* 38  */ {C2F(randlib)},
  /* 39  */ {C2F(otherspfunlib)},
  /* 40  */ {TCLINTERF},
	/* 41  */ {C2F(lstelmi)},
	/* 42  */ {PVMINTERF},
  /* 43  */ {C2F(intelm)},
  /* 44  */ {C2F(intlapack)},
  /* 45  */ {SCICOSINTERF1},  
  /* 46  */ {SCICOSINTERF2},
	/* 47  */ {C2F(intwintools)},
	/* 48  */ {C2F(mattime)},
	/* 49  */	{C2F(gw_sound)}
};

#endif /*__CALLINTERF__*/
