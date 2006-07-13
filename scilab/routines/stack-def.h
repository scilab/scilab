/* Copyright INRIA */

#ifndef STACK_DEF 
#define STACK_DEF 


#if _LCC_ & FORDLL 
  #define IMPORT __declspec (dllimport)
#else
 #ifdef FORDLL 
   #define IMPORT extern  __declspec (dllimport)
 #else 
   #define IMPORT extern
 #endif
#endif

#define csiz 63  
#define bsiz 4096  
#define isizt 10000
#define psiz 4096  
#define nsiz 6  
#define lsiz 65536
#define nlgh nsiz*4  
#define vsiz 2 
#define intersiz 1024
#define maxdb 100
#define maxbpt 1000

typedef struct 
{
 double Stk[vsiz];
} STACK_struct;

IMPORT STACK_struct C2F(stack);

typedef struct 
{
 int bot, top, idstk[nsiz*isizt], lstk[isizt], leps,bbot, bot0, infstk[isizt], gbot, gtop, isiz;
} VSTK_struct;

IMPORT VSTK_struct C2F(vstk);

typedef struct 
{
 int ids[nsiz*psiz], pstk[psiz], rstk[psiz], pt, niv, macr, paus, icall, krec;
} RECU_struct;

IMPORT RECU_struct C2F(recu);

typedef struct 
{
 int ddt, err, lct[8], lin[lsiz], lpt[6], hio, rio, wio, rte, wte;
} IOP_struct;

IMPORT IOP_struct C2F(iop);

typedef struct 
{
 int err1, err2, errct, toperr, errpt, ieee ,errcatch;
} ERRGST_struct; 

IMPORT ERRGST_struct C2F(errgst);

typedef struct 
{
 int sym, syn[nsiz], char1, fin, fun, lhs, rhs, ran[2], comp[3];
} COM_struct;

IMPORT COM_struct C2F(com);

typedef struct 
{
 int lbot, ie, is, ipal, nbarg, ladr[intersiz];
} ADRE_struct;

IMPORT ADRE_struct C2F(adre);

typedef struct 
{
  int nbvars, iwhere[intersiz], nbrows[intersiz], nbcols[intersiz], itflag[intersiz], ntypes[intersiz], lad[intersiz],ladc[intersiz],lhsvar[intersiz];
} INTERSCI_struct;

IMPORT INTERSCI_struct C2F(intersci);

typedef struct
{
  char alfa[csiz], alfb[csiz], buf[bsiz];
} CHA1_struct;

IMPORT CHA1_struct C2F(cha1);

typedef struct 
{
  int wmac, lcntr, nmacs, macnms[nsiz*maxdb], lgptrs[maxdb+1], bptlg[maxbpt];
} DBG_struct;

IMPORT DBG_struct C2F(dbg);

#endif /** STACK_DEF  **/

