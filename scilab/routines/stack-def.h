/* Copyright INRIA */

#ifndef STACK_DEF 
#define STACK_DEF 

/* XXXXX pour gcc -3.1 il faut un extern devant le 
 * __declspec(dllimport) 
 */ 

#ifdef FORDLL 
#define IMPORT extern  __declspec (dllimport)
#else 
#define IMPORT extern
#endif

/* typedef int integer ; */

#define csiz 63  
#define bsiz 4096  
#define isizt 2048
#define psiz 256  
#define nsiz 6  
#define lsiz 16384 
#define nlgh nsiz*4  
#define vsiz 2 
#define intersiz 60

IMPORT struct {
    double Stk[vsiz];
} C2F(stack);


IMPORT struct {
    int bot, top, idstk[nsiz*isizt], Lstk[isizt], leps, 
	    bbot, bot0, Infstk[isizt], gbot, gtop, isiz;
} C2F(vstk);


IMPORT struct {
    int ids[nsiz*psiz], Pstk[psiz], rstk[psiz], pt, niv, 
	    macr, paus, icall, krec;
} C2F(recu);

IMPORT struct {
    int ddt, err, lct[8], lin[lsiz], lpt[6], hio, rio, wio, rte, wte;
} C2F(iop);

IMPORT struct {
    int err1, err2, errct, toperr, errpt, ieee;
} C2F(errgst); 

IMPORT struct {
    int sym, syn[nsiz], char1, fin, fun, lhs, rhs, ran[2], comp[3];
} C2F(com);


IMPORT struct {
    int lbot, ie, is, ipal, nbarg, ladr[intersiz];
} C2F(adre);


IMPORT struct {
  int nbvars, iwhere[intersiz], nbrows[intersiz], nbcols[intersiz], itflag[intersiz], ntypes[intersiz], lad[intersiz],ladc[intersiz],lhsvar[intersiz];
} C2F(intersci);


IMPORT struct {
  char alfa[csiz], alfb[csiz], buf[bsiz];
} C2F(cha1);


#endif /** STACK_DEF  **/

