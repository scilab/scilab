/* Copyright INRIA */

/********************************************************
 * Table of hard coded Scilab interface 
 *  to add a new interface add a declaration and a new entry 
 *  at the end of the Interfaces array 
 *  The maximum number of interfaces is bound to DynInterfStart 
 *                                 ( file ../sun/addinter.h ) 
 ********************************************************/


extern void    /* 01  */ C2F(matlu)();
extern void    /* 02  */ C2F(MatdsRC)();
extern void    /* 03  */ C2F(matsvd)();
extern void    /* 04  */ C2F(matqr)();
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
extern void    /* 15  */ C2F(metane)();
extern void    /* 16  */ C2F(polelm)();
extern void    /* 17  */ C2F(lstelm)();
extern void    /* 18  */ C2F(sigelm)();
extern void    /* 19  */ C2F(datatf)();
extern void    /* 20  */ C2F(polaut)();
extern void    /* 21  */ C2F(strelm)();
extern void    /* 22  */ C2F(fmlelm)();
extern void    /* 23  */ C2F(logelm)();
extern void    /* 24  */ C2F(matus2)();
extern void    /* 25  */ C2F(xawelm)();
extern void    /* 26  */ C2F(matimp)();
extern void    /* 27  */ C2F(spelm)();
extern void    /* 28  */ C2F(intscicos)();
extern void    /* 29  */ C2F(matodc)();
extern void    /* 30  */ C2F(defint)();
extern void    /* 31  */ C2F(feval)();
extern void    /* 32  */ C2F(bva)();
extern void    /* 33  */ C2F(comm)();
extern void    /* 34  */ C2F(soundi)();
extern void    /* 35  */ C2F(coselm)();
extern void    /* 36  */ C2F(specfun)();
extern void    /* 37  */ C2F(dcd)();
extern void    /* 38  */ C2F(randlib)();

#ifdef WITH_TK
extern void    /* 40  */ C2F(tksciInterf)();
#define TKINTERF C2F(tksciInterf)
#else 
#define TKINTERF C2F(NoTksci)
#endif
extern void    /* 41  */ C2F(lstelmi)();
#ifdef WITH_PVM
extern void    /* 42  */ C2F(intpvm)();
#define PVMINTERF C2F(intpvm)
#else 
#define PVMINTERF C2F(NoPvm)
#endif
extern void    /* 43  */ C2F(intelm)();

static OpTab Interfaces[] ={
            /* 01  */ {C2F(matlu)},
	    /* 02  */ {C2F(MatdsRC)},
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
	    /* 15  */ {C2F(metane)},
	    /* 16  */ {C2F(polelm)},
	    /* 17  */ {C2F(lstelm)},
	    /* 18  */ {C2F(sigelm)},
	    /* 19  */ {C2F(datatf)},
	    /* 20  */ {C2F(polaut)},
	    /* 21  */ {C2F(strelm)},
	    /* 22  */ {C2F(fmlelm)},
	    /* 23  */ {C2F(logelm)},
	    /* 24  */ {C2F(matus2)},
	    /* 25  */ {C2F(xawelm)},
	    /* 26  */ {C2F(matimp)},
	    /* 27  */ {C2F(spelm)},
	    /* 28  */ {C2F(intscicos)},
	    /* 29  */ {C2F(matodc)},
	    /* 30  */ {C2F(defint)},
	    /* 31  */ {C2F(feval)},
	    /* 32  */ {C2F(bva)},
	    /* 33  */ {C2F(comm)},
	    /* 34  */ {C2F(soundi)},
	    /* 35  */ {C2F(coselm)},
	    /* 36  */ {C2F(specfun)},
	    /* 37  */ {C2F(dcd)},
     	    /* 38  */ {C2F(randlib)},
     	    /* 39  */ {C2F(matus2)}, /* free position may be used */
     	    /* 40  */ {TKINTERF},
	    /* 41  */ {C2F(lstelmi)},
	    /* 42  */ {PVMINTERF},
            /* 43  */ {C2F(intelm)},
};
