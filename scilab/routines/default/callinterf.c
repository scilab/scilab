/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include "../machine.h"
#include "../sun/addinter.h" /* for DynInterfStart */

static  jmp_buf jmp_env; 
extern int  C2F(matdsc) __PARAMS((void));
extern int  C2F(matdsr) __PARAMS((void));
extern int  C2F(userlk) __PARAMS((int *));
extern int  C2F(error) __PARAMS((int *));
extern void sciprint __PARAMS((char* ,...));

extern void  errjump(int n);
extern void  sci_sig_tstp(int n);
/***********************************************************
 * interface function 
 ***********************************************************/

static int Iflag=0; /* special flag for matdsr matdsc */

void C2F(MatdsRC)()
{
  if (Iflag == 1) 
    C2F(matdsc)();
  else
    C2F(matdsr)();
}

static int c_local = 9999;

void C2F(NoTksci)()
{
  sciprint("tksci interface not loaded \n");
  C2F(error)(&c_local);
  return;
}

void C2F(NoPvm)()
{
  sciprint("pvm interface not loaded \n");
  C2F(error)(&c_local);
  return;
}



/** table of interfaces **/

typedef  struct  {
  void  (*fonc)();} OpTab ;


#include "callinterf.h"


/***********************************************************
 * call the apropriate interface according to the value of k 
 * iflagint is only used inside MatdsRC to switch between 
 * matdsc or matdsr 
 ***********************************************************/

int C2F(callinterf)(k,iflagint)
      int *k,*iflagint;
{
  int returned_from_longjump ;
  static int count = 0;
  Iflag=*iflagint;
  if ( count == 0) 
    {
      /* fprintf(stderr," je fixe le SIGTSTP a errjump "); */
      /*XX signal (SIGTSTP ,errjump); */
      if (( returned_from_longjump = setjmp(jmp_env)) != 0 )
	{
	  /*XX signal (SIGTSTP ,sci_sig_tstp) ; */
	  Scierror(999,"SIGSTP: aborting current computation\r\n");
	  count = 0;
	  return 0;
	}
    }
  count++;
  if (*k > DynInterfStart) 
    C2F(userlk)(k);
  else
    (*(Interfaces[*k-1].fonc))();
  count--;
  if (count == 0) { 
    /* fprintf(stderr," je retire  SIGTSTP "); */
    /*XX  signal (SIGTSTP ,sci_sig_tstp); */
  }
  return 0;
}

/***********************************************************
 * Unused function just here to force linker to load some 
 * functions 
 ***********************************************************/

extern int   Blas_contents __PARAMS((int));
extern int   Lapack_contents __PARAMS((int));
extern int   Calelm_contents __PARAMS((int));
extern int   Sun_contents __PARAMS((int));
extern int   System2_contents __PARAMS((int));
extern int   System_contents __PARAMS((int));
extern int  Intersci_contents __PARAMS((int));

int ForceLink()
{
  Blas_contents(0);
  Lapack_contents(0);
  Calelm_contents(0);
  Sun_contents(0);
  System2_contents(0);
  System_contents(0);
  Intersci_contents(0);
  return 0;
}

/*-------------------------------------
 * long jump to stop interface computation 
 *-------------------------------------*/

void errjump(int n)
{
  longjmp(jmp_env,-1); 
}
