/*------------------------------------------------------------------------
 *    Interface for grand 
 *    Copyright Enpc/Cermics 
 *    jpc@cermics.enpc.fr 
 *    stuff to deal with several generators added 
 *         by Bruno Pincon (12/11/2001) 
 *
 --------------------------------------------------------------------------*/
#include <string.h>
#if WIN32
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#include "../stack-c.h"

/** external functions to be called through this interface **/

#include "grand.h"
#include "clcg4.h"
#include "others_generators.h"
#include <math.h>

enum {MT, KISS, CLCG4, CLCG2, URAND, FSULTRA};

/* the current generator : */
static int current_gen = MT;  

/* for clcg4 : the current virtual gen (current_clcg4 in [0, Maxgen]) */
static int current_clcg4 = 0; 

/* clcg4 must be called with the virtual generator number */
unsigned long int clcg4_with_gen(void)
{
  return ( clcg4(current_clcg4) );
}

#define NbGenInScilab 6

/*  pointers onto the generators func */
unsigned long int (*gen[NbGenInScilab])() = { randmt, kiss,  clcg4_with_gen, clcg2 , urandc , fsultra};

/*  names at the scilab level */
static char *names_gen[NbGenInScilab] = { "mt",  "kiss","clcg4", "clcg2", "urand", "fsultra" };

/* all the generators provided integers in [0, RngMaxInt] :        */
static
unsigned long RngMaxInt[NbGenInScilab] = { 4294967295ul,  /* mt    */
					   4294967295ul,  /* kiss  */
					   2147483646ul,  /* clcg4 */
					   2147483561ul,  /* clcg2 */
					   2147483647ul,  /* urand */
					   4294967295ul}; /* fsultra*/
/* the factors (1/(RngMaxInt+1)) to get reals in [0,1) :           */
static
double factor[NbGenInScilab] = { 2.3283064365386963e-10,  /* mt    */
				 2.3283064365386963e-10,  /* kiss  */
                                 4.6566128752457969e-10,  /* clcg4 */
		                 4.6566130595601735e-10,  /* clcg2 */
		                 4.6566128730773926e-10,  /* urand */
				 2.3283064365386963e-10}; /* fsultra*/

double C2F(ranf)(void)   
{
  /* random deviate from U[0,1) */
  return ( (double) gen[current_gen]() * factor[current_gen] );
}

double ignlgi(void)
{
  /* random deviate from Ui[0,RngMaxInt] (direct output of the current gen) */
  return ( (double) gen[current_gen]() );
}

double C2F(ignuin)(double *a, double *b)
{
  /*  random deviate from Ui[a,b] 
   *  it is assumed that : (i)  a and b are integers (stored in double) 
   *                       (ii) b-a+1 <= RngMaxInt[current_gen]
   *  (these verif are done at the calling level)
   *
   *  We use the classic method with a minor difference : to choose
   *  uniformly an integer in [a,b] (ie d=b-a+1 numbers) with a generator
   *  which provides uniformly integers in [0,RngMaxInt] (ie m=RngMaxInt+1
   *  numbers) we do the Euclidian division :
   *                                           m = q d + r,   r in [0,d-1]
   * 
   *  and accept only numbers l in [0, qd-1], then the output is k = a + (l mod d)
   *  (ie numbers falling in [qd , RngMaxInt] are rejected).
   *  The problem is that RngMaxInt is 2^32-1 for mt and kiss so that RngMaxInt+1 = 0
   *  with the 32 bits unsigned integer arithmetic. So in place of rejected r
   *  numbers we reject r+1 by using RngMaxInt in place of m. The constraint is
   *  then that (b-a+1) <= RngMaxInt and if we doesn't want to deal we each generator
   *  we take (b-a+1) <= Min RngMaxInt =  2147483561 (clcg2)
   */                 
  unsigned long k, d = (unsigned long)((*b-*a)+1), qd;
  
  if ( d == 1)
    return (*a);

  qd = RngMaxInt[current_gen] - RngMaxInt[current_gen] % d;
  do 
    { 
      k = (unsigned long)ignlgi();
    } 
  while ( k >= qd );
  return ( *a + (double)(k % d) );
}

/**************************************************
 *  hand written interface for the randlib 
 ***********************************************************************/

int RandI(char *fname,unsigned long fname_len)
{ 
  int minrhs = 1,maxrhs = 10,minlhs=1,maxlhs=2;
  int ResL,ResC,suite,m2,n2,l2,m1,n1,l1,ls,ms,ns,la,lr,lb,lc;
  int l3,l4;
  int i;

  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  if ( GetType(1) != 1) 
    {
     int un=1,deux=2, dim_state_mt=625, dim_state_fsultra = 40, dim_state_4=4;
      GetRhsVar(1,"c",&ms,&ns,&ls);
      if ( strcmp(cstk(ls),"getsd")==0) 
	{
	  if ( Rhs != 1  ||  Lhs != 1) 
	    {
	      Scierror(999,"Lhs and Rhs should be 1 for 'getsd' option\n\r");
	      return 0;
	    }
	      switch(current_gen)
		{
		case(MT) :
		  CreateVar(2,"d",&dim_state_mt,&un,&lr);
		  get_state_mt(stk(lr));
		  break;
		case(KISS) :
		  CreateVar(2,"d",&dim_state_4,&un,&lr);
		  get_state_kiss(stk(lr));
		  break;
		case(CLCG4) :
		  CreateVar(2,"d",&dim_state_4,&un,&lr);
		  get_state_clcg4(current_clcg4, stk(lr));
		  break;
		case(CLCG2) :
		  CreateVar(2,"d",&deux,&un,&lr);
		  get_state_clcg2(stk(lr));
		  break;
		case(URAND) : 
		  CreateVar(2,"d",&un,&un,&lr);
		  get_state_urand(stk(lr));
		  break;
		case(FSULTRA) : 
		  CreateVar(2,"d",&dim_state_fsultra,&un,&lr);
		  get_state_fsultra(stk(lr));
		  break;
		};
	      LhsVar(1) = 2;
	      PutLhsVar();
	      return 0;
	    }
	  else if ( strcmp(cstk(ls),"setall")==0 ) 
	    {
	      if ( current_gen != CLCG4 )
		sciprint("the setall option affect only the clcg4 generator !\n\r");
	      if ( Rhs != 5 ) 
		{
		  Scierror(999,"Rhs should be 5 for 'setall'  option\n\r");
		  return 0;
		}
	      GetRhsVar(2,"d",&m1,&n1,&l1);
	      if ( m1*n1 != 1) { Scierror(999,"second argument must be scalar\r\n");
	      return 0;}
	      GetRhsVar(3,"d",&m1,&n1,&l2);
	      if ( m1*n1 != 1) { Scierror(999,"third argument must be scalar\r\n");
	      return 0;}
	      GetRhsVar(4,"d",&m1,&n1,&l3);
	      if ( m1*n1 != 1) { Scierror(999,"fourth argument must be scalar\r\n");
	      return 0;}
	      GetRhsVar(5,"d",&m1,&n1,&l4);
	      if ( m1*n1 != 1) { Scierror(999,"fifth argument must be scalar\r\n");
	      return 0;}

	      if (! set_initial_seed_clcg4(*stk(l1),*stk(l2), *stk(l3), *stk(l4)) )
		{   /* => seeds were not good  (info is display by the function) */
		  Error(999);return 0;
		}
	      LhsVar(1) = 1;
	      PutLhsVar();
	      return(0);
	    }
      else if ( strcmp(cstk(ls),"setsd")==0 ) 
	{
	  switch(current_gen)
	    {
	    case(MT) :
	      if ( Rhs != 2 ) 
		{
		  Scierror(999,"Rhs should be 2 for 'setsd' option with the mt generator\n\r");
		  return 0;
		}
	      GetRhsVar(2,"d",&m1,&n1,&l1);
	      if ( m1*n1 == 1)          /* simple init of mt     */
		{ if (! set_state_mt_simple(*stk(l1)) ) {Error(999); return(0);}; }
	      else if ( m1*n1 == 625 )  /* init of all the state */
		{ if (! set_state_mt(stk(l1))) {Error(999); return(0);}; }
	      else
		{
		  Scierror(999,"for mt you must init the state with a vector of 1 or 625 values !\n\r");
		  return 0;
		};
	      break;

	    case(FSULTRA) :  
	      if ( Rhs == 2 ) /* init via a "complete" state */ 
		{
		  GetRhsVar(2,"d",&m1,&n1,&l1);
		  if ( m1 != 40  ||  n1 != 1)
		    {
		      Scierror(999,"the state for fsultra must be a 40x1 vector !\n\r");
		      return 0;
		    };
		  if (! set_state_fsultra(stk(l1)) ) {Error(999); return(0);}; 
		}
	      else if ( Rhs == 3 ) /* init with 2 integers (like before) */
		{
		  GetRhsVar(2,"d",&m1,&n1,&l1);
		  if ( m1*n1 != 1) 
		    { Scierror(999,"second argument must be scalar\r\n"); return 0;};
		  GetRhsVar(3,"d",&m1,&n1,&l2);
		  if ( m1*n1 != 1) 
		    { Scierror(999,"third argument must be scalar\r\n"); return 0;};
		  if (! set_state_fsultra_simple(*stk(l1),*stk(l2)) ) {Error(999); return(0);}; 
		}
	      else
		{
		  Scierror(999,"Rhs should be 2 or 3 for 'setsd' option with the fsultra generator\n\r");
		  return 0;
		}
	      break;

	    case(KISS) :
	    case(CLCG4) :
	      if ( Rhs != 5 ) 
		{
		  Scierror(999,"Rhs should be 5 for 'setsd' option with the kiss or clcg4 generator\n\r");
		  return 0;
		}
	      GetRhsVar(2,"d",&m1,&n1,&l1);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"second argument must be scalar\r\n"); return 0;}
	      GetRhsVar(3,"d",&m1,&n1,&l2);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"third argument must be scalar\r\n"); return 0;}
	      GetRhsVar(4,"d",&m1,&n1,&l3);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"fourth argument must be scalar\r\n"); return 0;}
	      GetRhsVar(5,"d",&m1,&n1,&l4);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"fifth argument must be scalar\r\n"); return 0;}
	      if (current_gen == KISS) 
		{if (! set_state_kiss(*stk(l1),*stk(l2),*stk(l3),*stk(l4))) {Error(999); return 0;};}
	      else
		{if (! set_seed_clcg4(current_clcg4,*stk(l1),*stk(l2),*stk(l3),*stk(l4)))
		  {Error(999); return 0;};}
	      break;

	    case(CLCG2) :
	      if ( Rhs != 3 ) 
		{
		  Scierror(999,"Rhs should be 3 for 'setsd' option with the clcg2 generator\n\r");
		  return 0;
		}
	      GetRhsVar(2,"d",&m1,&n1,&l1);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"second argument must be scalar\r\n"); return 0;};
	      GetRhsVar(3,"d",&m1,&n1,&l2);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"third argument must be scalar\r\n"); return 0;};
	      if (! set_state_clcg2(*stk(l1),*stk(l2))) 
		{ Error(999); return 0;};
	      break;

	    case(URAND) :
	      if ( Rhs != 2 ) 
		{
		  Scierror(999,"Rhs should be 2 for 'setsd' option with the urand generator\n\r");
		  return 0;
		}
	      GetRhsVar(2,"d",&m1,&n1,&l1);
	      if ( m1*n1 != 1) 
		{ Scierror(999,"second argument must be scalar\r\n"); return 0;};
	      if (! set_state_urand(*stk(l1))) 
		{Error(999); return 0;};
	      break;
	    };
	  LhsVar(1) = 0;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("phr2sd",cstk(ls)) == 0) 
	{
	  if ( Rhs != 2  ||  Lhs > 1 ) 
	    {
	      Scierror(999,"Rhs should be 2 for 'phr2sd' option and Lhs should be 1\n\r");
	      return 0;
	    }
	  GetRhsVar(2,"c",&m1,&n1,&l1);
	  CreateVar(3,"i",&un,&deux,&l2);
	  
	  C2F(phrtsd)(cstk(l1),&m1,istk(l2),istk(l2+1),m1);
	  LhsVar(1) = 3;
	  PutLhsVar();
	  return 0;
	}

      else if (strcmp("initgn",cstk(ls))==0) 
	{
	  SeedType Where;
	  if ( current_gen != CLCG4 )
	    sciprint("this option affect only the clcg4 generator\n\r");
	  if ( Rhs != 2) 
	    {
	      Scierror(999,"Rhs should be 2 for 'initgn' option\n\r");
	      return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  if ( *istk(l1) != 0 && *istk(l1)!= -1 && *istk(l1) != 1)
	    {
	      Scierror(999,"for initgn option argument must be -1,0 or 1\r\n");
	      return 0;
	    }
	  Where = (SeedType) (*istk(l1) + 1);
	  init_generator_clcg4(current_clcg4, Where);	  
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("setcgn",cstk(ls))==0) 
	{
	  if ( current_gen != CLCG4 )
	    sciprint("the setcgn option affect only the clcg4 generator\n\r");
	  if ( Rhs != 2) 
	    {
	      Scierror(999,"Rhs should be 2 for 'setcgn' option\n\r");
	      return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  if ( *istk(l1) < 0 || *istk(l1) > Maxgen )
	    {
	      Scierror(999,"bad virtual number generator (must be in [0,%d])\n\r",Maxgen);
	      return 0;
	    }
	  current_clcg4 = *istk(l1);
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("advnst",cstk(ls))==0) 
	{
	  int k;
	  if ( current_gen != CLCG4 )
	    sciprint("the 'advnst' option affect only the clcg4 generator !\n\r");
	  if ( Rhs != 2) 
	    {
	      Scierror(999,"Rhs should be 2 for 'advnst' option\n\r");
	      return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  k = *istk(l1);
	  if ( k < 1 )
	    {
	      Scierror(999,"parameter K must be > 0 for 'advnst' option\n\r");
	      return 0;
	    }
	  advance_state_clcg4(current_clcg4, k);
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("getcgn",cstk(ls))==0) 
	{
	  if ( Rhs != 1) 
	    {
	      Scierror(999,"Rhs should be 1 for 'getcgn' option\n\r");
	      return 0;
	    }
	  if ( current_gen != CLCG4 )
	    sciprint("this information concerns only the clcg4 generator\n\r");
	  CreateVar(2,"i",&un,&un,&l1);
	  *istk(l1) = current_clcg4; 
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("setgen",cstk(ls))==0) 
	{
	  int msb, nsb, lsb;
	  if ( Rhs != 2) 
	    {
	      Scierror(999,"Rhs should be 2 for 'setgen' option\n\r");
	      return 0;
	    }
	  GetRhsVar(2,"c",&msb,&nsb,&lsb);
	  if (strcmp("mt",cstk(lsb))==0) 	  
	    current_gen = MT;
	  else if (strcmp("kiss",cstk(lsb))==0)
	    current_gen = KISS;
	  else if (strcmp("clcg4",cstk(lsb))==0)
	    current_gen = CLCG4;
	  else if (strcmp("clcg2",cstk(lsb))==0)
	    current_gen = CLCG2;
	  else if (strcmp("urand",cstk(lsb))==0)
	    current_gen = URAND;
	  else if (strcmp("fsultra",cstk(lsb))==0)
	    current_gen = FSULTRA;
	  else
	    {
	      Scierror(999,"unknown generator (choose among : mt kiss clcg4 clcg2 urand fsultra) \n\r");
	      return 0;
	    }
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("getgen",cstk(ls))==0) 
	{
	  int un=1;
	  if ( Rhs != 1) 
	    {
	      Scierror(999,"Rhs should be 1 for 'getgen' option\n\r");
	      return 0;
	    }
	  CreateVarFromPtr( 2, "S", &un, &un, &names_gen[current_gen]);
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else 
	{
	  Scierror(999,"%s Wrong first argument %s\r\n",fname,cstk(ls));
	  
	  return 0;
	}      
    }
  minrhs = 2;
  CheckRhs(minrhs,maxrhs);
  if ( GetType(2) == 1 ) /** m,n,'string' */
    {
      GetRhsVar(1, "i", &m1, &n1, &l1)
	if ( m1*n1 != 1) 
	  { Scierror(999,"First argument must be scalar\r\n");return 0;}
      ResL= *istk(l1);
      GetRhsVar(2, "i", &m2, &n2, &l2);
      if ( m2*n2 != 1) 
	{ Scierror(999,"First argument must be scalar\r\n");return 0;}
      ResC= *istk(l2);
      GetRhsVar(3, "c", &ms, &ns, &ls);
      suite=4;
    }
  else 
    {
      GetRhsVar(1, "i", &ResL, &ResC, &l1);
      GetRhsVar(2, "c", &ms, &ns, &ls);
      suite = 3;
    }
  if ( strcmp(cstk(ls),"bet")==0) 
    {
      double minlog=1.e-37;
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing A and B for beta law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"A must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"B must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < minlog || *stk(lb) < minlog)
	{
	  Scierror(999,"Rand(...,'bet',..): A or B < %f \r\b",minlog);
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(genbet)(stk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"f")==0) 
    {
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Dfn and Dfd for F law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Dfn must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"Dfd must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) <= 0.0 || *stk(lb) <= 0.0)
	{
	  Scierror(999,"Degrees of freedom nonpositive \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(genf)(stk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"mul")==0) 
    {
      int i,nn,ncat;
      double ptot;
      if ( suite != 3 || ResL*ResC != 1)
	{ Scierror(999,"First argument for 'mul' option must be the number of random deviate \r\n");
	return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing N and P for MULtinomial law\r\n");return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"N must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m2, &n2, &lb);
      if ( n2 != 1 ) 
	{ 
	  Scierror(999,"P must be a column vector\r\n");
	  return 0;
	}
      ncat = m2+1;
      CreateVar(suite+2,"i",&ncat,&nn,&lr);
      if ( *istk(la) < 0 ) 
	{
	  Scierror(999,"N < 0 \r\n");
	  return 0;
	}
      if ( ncat <= 1) 
	{
	  Scierror(999,"Ncat <= 1 \r\n");
	  return 0;
	}
      ptot = 0.0;
      for ( i= 0 ; i < ncat -1 ; i++ )
	{
	  if ( *stk(lb+i) < 0 ) 
	    {
	      Scierror(999,"P(%d) < 0 \r\n",i+1);
	      return 0;
	    }
	  if ( *stk(lb+i) > 1 ) 
	    {
	      Scierror(999,"P(%d) > 1 \r\n",i+1);
	      return 0;
	    }
	  ptot += *stk(lb+i);
	}
      if ( ptot > 0.99999) 
	{
	  Scierror(999,"Sum of P(i) > 1 \r\n");
	  return 0;
	}
      for ( i=0 ; i < nn ; i++) 
	{
	  C2F(genmul)(istk(la),stk(lb),&ncat,istk(lr+ncat*i));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"gam")==0) 
    {
      if ( Rhs != suite + 1) 

	/*  ETRE PLUS CONSISTANT ICI : choisir entre shape , scale ou
            bien A et R (idem pour le man)
	*/
	{ Scierror(999,"Missing shape and scale for Gamma law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"shape must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"scale must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( (*stk(la)) <= 0.0 ||  (*stk(lb)) <= 0.0 )
	{
	  Scierror(999,"grand(..'gam',A,R) : A <= 0.0 or R <= 0.0 \r\n"); return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  /** WARNING : order is changed in parameters for 
	      compatibility between Rand(...'gam',..) and cdfgam 
	  **/
	  *stk(lr+i)= C2F(gengam)(stk(lb),stk(la));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"nor")==0) 
    {
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Av and Sd for Normal law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Av must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"Sd must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0 ) 
	{
	  Scierror(999,"SD < 0.0 \r\n");return 0;}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(gennor)(stk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"unf")==0) 
    {
      double low, high;
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Low and High for Uniform Real law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Low must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"High must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      low = *stk(la) ; high =  *stk(lb);
      if ( low > high ) 
	{
	  Scierror(999,"Low > High \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	*stk(lr+i)= low + (high - low)* C2F(ranf)();
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"uin")==0) 
    {
      double a, b;
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Low and High for Uniform integer law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Low must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"High must be scalar\r\n");return 0;}
      a = *stk(la) ; b = *stk(lb);
      if ( a != floor(a) || b != floor(b) || (b-a+1) > 2147483561 )
	{
	  Scierror(999," a and b must integers with (b-a+1) <= 2147483561");
	  return 0;
	}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	*stk(lr+i)= C2F(ignuin)(stk(la),stk(lb));
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"lgi")==0) 
    {
      if ( Rhs != suite -1 ) 
	{ 
	  Scierror(999,"Only %d arguments required for 'lgi' option",suite-1);
	  return 0;
	}
      CreateVar(suite,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	*stk(lr+i)= ignlgi();
      LhsVar(1) = suite;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"prm")==0)
    {
      int nn;
      if ( suite != 3 || ResL*ResC != 1)
	{ 
	  Scierror(999,"First argument for 'prm' option must be the number of random simulation \r\n");
	  return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite) 
	{ 
	  Scierror(999,"Missing vect for random permutation\r\n");
	  return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( n1 != 1) { Scierror(999,"vect must be column vector\r\n");
      return 0;}
      CreateVar(suite+1,"d",&m1,&nn,&lr);
      for ( i=0 ; i < nn ; i++) 
	{
	  int j ; 
	  for (j=0; j < m1 ; j++ ) *stk(lr+(m1)*i+j)= *stk(la+j);
	  C2F(genprm)(stk(lr+(m1)*i),&m1);
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"nbn")==0) 
    {
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing N and P for Negative Binomial law\r\n");return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"N must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"P must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 ) 
	{
	  Scierror(999,"P is not in [0,1] \r\n");
	  return 0;
	}
      if ( *istk(la) < 0 ) 
	{
	  Scierror(999,"N < 0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ignnbn)(istk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"bin")==0) 
    {
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing N and P for Binomial law\r\n");return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"N must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"P must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 ) 
	{
	  Scierror(999,"P is not in [0,1] \r\n");
	  return 0;
	}
      if ( *istk(la) < 0 ) 
	{
	  Scierror(999,"N < 0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ignbin)(istk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"mn")==0) 
    {
      int nn,un=1,work,mp,parm,ierr;
      if ( suite != 3 || ResL*ResC != 1)
	{ Scierror(999,"First argument for 'mn' option must be the number of random simulation \r\n");return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Mean and Cov for Multivariate Normal law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( n1 != 1) { Scierror(999,"Mean must be column vector\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m2, &n2, &lb);
      if ( m2 != n2 ) { Scierror(999,"Cov must be a square matrix\r\n");return 0;}
      if ( m2 != m1 ) { Scierror(999,"Mean and Cov have incompatible dimensions\n");return 0;}
      
      CreateVar(suite+2,"d",&m1,&nn,&lr);
      CreateVar(suite+3,"d",&m1,&un,&work);
      mp=m1*(m1+3)/2 + 1;
      CreateVar(suite+4,"d",&mp,&un,&parm);
      if ( m1 <= 0 ) 
	{
	  Scierror(999,"Mean and Cov are of null size\r\n");
	  return 0;
	}
      C2F(setgmn)(stk(la),stk(lb),&m2,&m1,stk(parm),&ierr);
      if ( ierr == 1) 
	{
	  Error(999);return 0;
	} 
      for ( i=0 ; i < nn ; i++) 
	{
	  C2F(genmn)(stk(parm),stk(lr+(m1)*i),stk(work));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"markov")==0) 
    {
      int nn,n1p1,lr1,j,icur,mm,jj;
      if ( suite != 3 || ResL*ResC != 1)
	{ Scierror(999,"First argument for 'markov' option must be the number of random simulation \r\n");return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite +1 )
	{ Scierror(999,"Missing P matrix and X0 for Markov chain\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      GetRhsVar(suite+1, "i", &m2, &n2, &lb);
      if ( m1 != n1 && m1 != 1 ) 
	{ 
	  Scierror(999,"P must be a square matrix or a row vector\r\n");return 0;
	}

      if ( m2*n2 == 0 ) { Scierror(999,"X0 is empty\r\n");return 0;} 
      
      for ( i = 0 ; i < m2*n2 ; i++)
	if ( *istk(lb+i)-1 < 0 || *istk(lb+i)-1 >= n1 ) 
	  {
	    Scierror(999,"X0(%d) must be in the range [1,%d]\r\n",i,n1);
	    return 0;
	  }
      mm= m2*n2;
      CreateVar(suite+2,"i",&mm,&nn,&lr);

      n1p1=n1+1;
      CreateVar(suite+3,"d",&m1,&n1p1,&lr1);
      for ( i= 0 ; i < m1 ; i++ )
	{
	  double ptot = 0.0;
	  for ( j = 0 ; j < n1 ; j++ )
	    {
	      if ( *stk(la+i+m1*j) < 0 ) 
		{
		  Scierror(999,"P(%d,%d) < 0 \r\n",i+1,j+1);
		  return 0;
		}
	      if ( *stk(la+i+m1*j) > 1 ) 
		{
		  Scierror(999,"P(%d,%d) > 1 \r\n",i+1,j+1);
		  return 0;
		}
	      ptot += *stk(la+i+m1*j) ;
	    }
	  if ( fabs(ptot-1.0) > 1e-8 ) 
	    {
	      Scierror(999,"Sum of P(%d,1:%d)=%f ~= 1 \r\n",i+1,n1,ptot);
	      return 0;
	    }
	}
      /** Computing the cumulative sum of the P matrix **/
      for ( i = 0 ; i < m1 ; i++) 
	{
	  double cumsum=0.0;
	  *stk(lr1 +i) = cumsum;
	  for ( j= 1; j < n1p1 ; j++ ) 
	    {
	      cumsum += *stk(la + i + m1*(j-1));
	      *stk(lr1+i+m1*j) = cumsum;
	    }
	}
      for ( jj = 0 ; jj < mm ; jj++) 
	{
	  icur = *istk(lb+jj)-1;
	  for ( i=0 ; i < nn ; i++) 
	    {
	      int niv=0;
	      double rr = C2F(ranf)();
	      if ( m1 == 1 ) icur =0;
	      while ( rr >= *stk(lr1+ icur +m1*niv) && niv < n1p1 ) 
		{
		  niv++;
		}
	      /** projection to avoid boundaries **/
	      niv = Max(Min(niv,n1),1); 
	      *istk(lr+jj+mm*i)= niv ; 
	      icur=niv-1;
	    }
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"def")==0) 
    {
      if ( Rhs != suite -1 ) 
	{ Scierror(999,"no argument required for 'def' option\r\n");return 0;}
      CreateVar(suite,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(ranf)();
	}
      LhsVar(1) = suite;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"nch")==0) 
    {
      if ( Rhs != suite + 1) 
	{ Scierror(999,"Missing Df and Xnonc for non-central chi-square law\r\n");return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Df must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"Xnonc must be scalar\r\n");return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 1.0 || *stk(lb) < 0.0 )
	{
	  Scierror(999,"DF < 1 or XNONC < 0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(gennch)(stk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"nf")==0) 
    {
      if ( Rhs != suite + 2) 
	{ 
	  Scierror(999,"Missing Dfn, Dfd and Xnonc for non-central F law\r\n");
	  return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Dfn must be scalar\r\n");return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { Scierror(999,"Dfd must be scalar\r\n");return 0;}
      GetRhsVar(suite+2, "d", &m1, &n1, &lc);
      if ( m1*n1 != 1) { Scierror(999,"Xnonc must be scalar\r\n");return 0;}
      CreateVar(suite+3,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 1.0 || *stk(lb) < 0.0 || *stk(lc) < 0.0 ) 
	{
	  Scierror(999,"DF < 1.0 or DF <= 0.0 or Xnonc < 0.0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(gennf)(stk(la),stk(lb),stk(lc));
	}
      LhsVar(1) = suite+3;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"chi")==0)
    {
      if ( Rhs != suite ) 
	{ Scierror(999,"Missing Df for chi-square law\r\n");
	return 0;
	}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Df must be scalar\r\n");return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if  ( *stk(la) <= 0.0)
	{
	  Scierror(999,"Rand: DF <= 0 \r\n");return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(genchi)(stk(la));
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"poi")==0)
    {
      if ( Rhs != suite ) 
	{ Scierror(999,"Missing Av for Poisson law\r\n");
	return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Av must be scalar\r\n");return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 0.0 )
	{
	  Scierror(999,"Av < 0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ignpoi)(stk(la));
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"geom")==0)
    {
      double p;
      if ( Rhs != suite ) 
	{ Scierror(999,"Missing p for Geometric law\r\n");
	return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"p must be scalar\r\n");return 0;}
      p = *stk(la);
      if ( p < 1.3e-307 || p > 1 ) { Scierror(999,"p must be in [pmin,1]\r\n");return 0;}
      
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= igngeom(p);
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"exp")==0)
    {
      if ( Rhs != suite ) 
	{ Scierror(999,"Missing Av for exponential law\r\n");
	return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { Scierror(999,"Av must be scalar\r\n");return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 0.0 ) 
	{
	  Scierror(999,"Av < 0.0 \r\n");
	  return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(genexp)(stk(la));
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }

  else 
    {
      Scierror(999,"%s Wrong argument %s\r\n",fname,cstk(ls));
      return 0;
    }      
}

static TabF Tab[]={ 
  {RandI, "Rand"},
};

int C2F(randlib)(void)
{
	Rhs = Max(0, Rhs);
	#if WIN32
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	return 0;
}
