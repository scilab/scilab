/* Copyright Enpc/Cergrene */

#include "../../routines/stack-c.h"
#include "grand.h"

/** external functions to be called through this interface **/


/**************************************************
 *  hand written interface for the randlib 
 ***********************************************************************/

int RandI(fname)
     char* fname;
{ 
  int pipo;
  int minrhs = 1,maxrhs = 10,minlhs=1,maxlhs=2;
  int ResL,ResC,suite,m2,n2,l2,m1,n1,l1,ls,ms,ns,la,lr,lb,lc;
  int which,status,ierr,i;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  if ( GetType(1) != 1) 
    {
      int un=1,deux=2;
      GetRhsVar(1,"c",&ms,&ns,&ls);
      if ( strcmp(cstk(ls),"getsd")==0) 
	{
	  if ( Rhs != 1) 
	    {
	      sciprint("Rhs should be 1 for 'getsd' option");
	      Error(999);return 0;
	    }
	  CreateVar(2,"i",&un,&deux,&lr);
	  C2F(getsd)(istk(lr),istk(lr+1));
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if ( strcmp(cstk(ls),"setall")==0 ) 
	{
	  if ( Rhs != 3 ) 
	    {
	      sciprint("Rhs should be 3 for 'setall'  option");
	      Error(999);return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  if ( m1*n1 != 1) { sciprint("second argument must be scalar\r\n");
	  Error(999);return 0;}
	  GetRhsVar(3,"i",&m1,&n1,&l2);
	  if ( m1*n1 != 1) { sciprint("third argument must be scalar\r\n");
	  Error(999);return 0;}
	  C2F(setall)(istk(l1),istk(l2));
	  LhsVar(1) = 1;
	  PutLhsVar();
	  return(0);
	}
      else if ( strcmp(cstk(ls),"setsd")==0 ) 
	{
	  if ( Rhs != 3 ) 
	    {
	      sciprint("Rhs should be 3 for  'setsd' option");
	      Error(999);return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  if ( m1*n1 != 1) { sciprint("second argument must be scalar\r\n");
	  Error(999);return 0;}
	  GetRhsVar(3,"i",&m1,&n1,&l2);
	  if ( m1*n1 != 1) { sciprint("third argument must be scalar\r\n");
	  Error(999);return 0;}
	  C2F(setsd)(istk(l1),istk(l2));
	  LhsVar(1) = 1;
	  PutLhsVar();
	  return(0);
	}
      else if (strcmp("phr2sd",cstk(ls)) == 0) 
	{
	  if ( Rhs != 2) 
	    {
	      sciprint("Rhs should be 2 for 'phr2sd' option");
	      Error(999);return 0;
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
	  if ( Rhs != 2) 
	    {
	      sciprint("Rhs should be 2 for 'initgn' option");
	      Error(999);return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  if ( *istk(l1) != 0 && *istk(l1)!= -1 && *istk(l1) != 1)
	    {
	      sciprint("for initgn option argument must be -1,0 or 1\r\n");
	      Error(999);return 0;
	    }
	  C2F(initgn)(istk(l1));
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("setcgn",cstk(ls))==0) 
	{
	  if ( Rhs != 2) 
	    {
	      sciprint("Rhs should be 2 for 'setcgn' option");
	      Error(999);return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  C2F(setcgn)(istk(l1));
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("advnst",cstk(ls))==0) 
	{
	  if ( Rhs != 2) 
	    {
	      sciprint("Rhs should be 2 for 'advnst' option");
	      Error(999);return 0;
	    }
	  GetRhsVar(2,"i",&m1,&n1,&l1);
	  C2F(advnst)(istk(l1));
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else if (strcmp("getcgn",cstk(ls))==0) 
	{
	  if ( Rhs != 1) 
	    {
	      sciprint("Rhs should be 1 for 'getcgn' option");
	      Error(999);return 0;
	    }
	  CreateVar(2,"i",&un,&un,&l1);
	  C2F(getcgn)(istk(l1));
	  LhsVar(1) = 2;
	  PutLhsVar();
	  return 0;
	}
      else 
	{
	  sciprint("%s Wrong first argument %s\r\n",fname,cstk(ls));
	  Error(999);
	  return 0;
	}      
    }
  minrhs = 2;
  CheckRhs(minrhs,maxrhs);
  if ( GetType(2) == 1 ) /** m,n,'string' */
    {
      GetRhsVar(1, "i", &m1, &n1, &l1)
      if ( m1*n1 != 1) 
	{ sciprint("First argument must be scalar\r\n");Error(999);return 0;}
      ResL= *istk(l1);
      GetRhsVar(2, "i", &m2, &n2, &l2);
      if ( m2*n2 != 1) 
	{ sciprint("First argument must be scalar\r\n");Error(999);return 0;}
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
	{ sciprint("Missing A and B for beta law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("A must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("B must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < minlog || *stk(lb) < minlog)
	{
	  sciprint("Rand(...,'bet',..): A or B < %f \r\b",minlog);
	  Error(999);return 0;
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
	{ sciprint("Missing Dfn and Dfd for F law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Dfn must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("Dfd must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) <= 0.0 || *stk(lb) <= 0.0)
	{
	  sciprint("Degrees of freedom nonpositive \r\n");
	  Error(999);return 0;
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
	{ sciprint("First argument for 'mul' option must be the number of random deviate \r\n");
	Error(999);return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite + 1) 
	{ sciprint("Missing N and P for MULtinomial law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("N must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m2, &n2, &lb);
      if ( n2 != 1 ) 
	{ 
	  sciprint("P must be a column vector\r\n");
	  Error(999);return 0;
	}
      ncat = m2+1;
      CreateVar(suite+2,"i",&ncat,&nn,&lr);
      if ( *istk(la) < 0 ) 
	{
	  sciprint("N < 0 \r\n");
	  Error(999);return 0;
	}
      if ( ncat <= 1) 
	{
	  sciprint("Ncat <= 1 \r\n");
	  Error(999);return 0;
	}
      ptot = 0.0;
      for ( i= 0 ; i < ncat -1 ; i++ )
	{
	  if ( *stk(lb+i) < 0 ) 
	    {
	      sciprint("P(%d) < 0 \r\n",i+1);
	      Error(999);return 0;
	    }
	  if ( *stk(lb+i) > 1 ) 
	    {
	      sciprint("P(%d) > 1 \r\n",i+1);
	      Error(999);return 0;
	    }
	  ptot += *stk(lb+i);
	}
      if ( ptot > 0.99999) 
	{
	  sciprint("Sum of P(i) > 1 \r\n");
	  Error(999);return 0;
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
	{ sciprint("Missing shape and scale for Gamma law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("shape must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("scale must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( (*stk(la)) <= 0.0 ||  (*stk(lb)) <= 0.0 )
	{
	  sciprint("Rand(..'gam',A,R) : A <= 0.0 or R <= 0.0 \r\n"); Error(999);return 0;
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
	{ sciprint("Missing Av and Sd for Normal law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Av must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("Sd must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0 ) 
	{
	  sciprint("SD < 0.0 \r\n");Error(999);return 0;}
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
      if ( Rhs != suite + 1) 
	{ sciprint("Missing Low and High for Uniform Real law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Low must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("High must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) > *stk(lb) ) 
	{
	  sciprint("Low > High \r\n");
	  Error(999);return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{	
	  *stk(lr+i)= C2F(genunf)(stk(la),stk(lb));
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"uin")==0) 
    {
      if ( Rhs != suite + 1) 
	{ sciprint("Missing Low and High for Uniform integer law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Low must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "i", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("High must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= C2F(ignuin)(istk(la),istk(lb),&ierr);
	  if (ierr > 0 ) 
	    {
	     Error(999);return 0;
	    } 
	}
      
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"lgi")==0) 
    {
      if ( Rhs != suite -1 ) 
	{ 
	  sciprint("Only %d arguments required for 'lgi' option",suite-1);
	  Error(999);return 0;
	}
      CreateVar(suite,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ignlgi)();
	}
      LhsVar(1) = suite;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"prm")==0)
    {
      int nn;
      if ( suite != 3 || ResL*ResC != 1)
	{ 
	  sciprint("First argument for 'prm' option must be the number of random simulation \r\n");
	  Error(999);return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite) 
	{ 
	  sciprint("Missing vect for random permutation\r\n");
	  Error(999);return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( n1 != 1) { sciprint("vect must be column vector\r\n");
      Error(999);return 0;}
      CreateVar(suite+1,"i",&m1,&nn,&lr);
      for ( i=0 ; i < nn ; i++) 
	{
	  int j ; 
	  for (j=0; j < m1 ; j++ ) *istk(lr+(m1)*i+j)= *istk(la+j);
	  C2F(genprm)(istk(lr+(m1)*i),&m1);
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"nbn")==0) 
    {
      if ( Rhs != suite + 1) 
	{ sciprint("Missing N and P for Negative Binomial law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("N must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("P must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 ) 
	{
	  sciprint("P is not in [0,1] \r\n");
	  Error(999);return 0;
	}
      if ( *istk(la) < 0 ) 
	{
	  sciprint("N < 0 \r\n");
	  Error(999);return 0;
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
	{ sciprint("Missing N and P for Binomial law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "i", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("N must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("P must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(lb) < 0.0 || *stk(lb) > 1.0 ) 
	{
	  sciprint("P is not in [0,1] \r\n");
	  Error(999);return 0;
	}
      if ( *istk(la) < 0 ) 
	{
	  sciprint("N < 0 \r\n");
	  Error(999);return 0;
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
	{ sciprint("First argument for 'mn' option must be the number of random simulation \r\n");Error(999);return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite + 1) 
	{ sciprint("Missing Mean and Cov for Multivariate Normal law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( n1 != 1) { sciprint("Mean must be column vector\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m2, &n2, &lb);
      if ( m2 != n2 ) { sciprint("Cov must be a square matrix\r\n");Error(999);return 0;}
      if ( m2 != m1 ) { sciprint("Mean and Cov have incompatible dimensions\n");Error(999);return 0;}
      
      CreateVar(suite+2,"d",&m1,&nn,&lr);
      CreateVar(suite+3,"d",&m1,&un,&work);
      mp=m1*(m1+3)/2 + 1;
      CreateVar(suite+4,"d",&mp,&un,&parm);
      if ( m1 <= 0 ) 
	{
	  sciprint("Mean and Cov are of null size\r\n");Error(999);
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
      int nn,un=1,work,mp,parm,ierr,n1p1,lr1,j,icur,mm,jj;
      if ( suite != 3 || ResL*ResC != 1)
	{ sciprint("First argument for 'markov' option must be the number of random simulation \r\n");Error(999);return 0;
	}
      nn= *istk(l1);
      if ( Rhs != suite +1 )
	{ sciprint("Missing P matrix and X0 for Markov chain\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      GetRhsVar(suite+1, "i", &m2, &n2, &lb);
      if ( m1 != n1 ) { sciprint("P must be a square matrix\r\n");Error(999);return 0;}

      if ( m2*n2 == 0 ) { sciprint("X0 is empty\r\n");Error(999);return 0;} 
      
      for ( i = 0 ; i < m2*n2 ; i++)
	if ( *istk(lb+i)-1 < 0 || *istk(lb+i)-1 >= n1 ) 
	  {
	    sciprint("X0(%d) must be in the range [1,%d]\r\n",i,n1);
	    Error(999);return 0;
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
		  sciprint("P(%d,%d) < 0 \r\n",i+1,j+1);
		  Error(999);return 0;
		}
	      if ( *stk(la+i+m1*j) > 1 ) 
		{
		  sciprint("P(%d,%d) > 1 \r\n",i+1,j+1);
		  Error(999);return 0;
		}
	      ptot += *stk(la+i+m1*j) ;
	    }
	  if ( ptot -1.0 > 1.e-6 ) 
	    {
	      sciprint("Sum of P(%d,1:%d)=%f > 1 \r\n",i+1,n1,ptot);
	      Error(999);return 0;
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
	      double dmi=0.0e0, dma=1.0e0;
	      double rr = C2F(genunf)(&dmi,&dma);
	      while ( rr >= *stk(lr1+ icur +m1*niv) && niv < n1p1 ) 
		{
		  niv++;
		}
	      /** projection to avoid boundaries **/
	      niv = Max(Min(niv,n1),1); 
	      *istk(lr+jj+mm*i)= niv ; icur=niv-1;
	    }
	}
      LhsVar(1) = suite+2;
      PutLhsVar();
      return 0;
    }
  else if ( strcmp(cstk(ls),"def")==0) 
    {
      if ( Rhs != suite -1 ) 
	{ sciprint("no argument required for 'def' option\r\n");Error(999);return 0;}
      CreateVar(suite,"d",&ResL,&ResC,&lr);
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ranf)();
	}
      LhsVar(1) = suite;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"nch")==0) 
    {
      if ( Rhs != suite + 1) 
	{ sciprint("Missing Df and Xnonc for non-central chi-square law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Df must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("Xnonc must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+2,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 1.0 || *stk(lb) < 0.0 )
	{
	  sciprint("DF < 1 or XNONC < 0 \r\n");
	  Error(999);return 0;
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
	{ sciprint("Missing Dfn, Dfd and Xnonc for non-central F law\r\n");Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Dfn must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+1, "d", &m1, &n1, &lb);
      if ( m1*n1 != 1) { sciprint("Dfd must be scalar\r\n");Error(999);return 0;}
      GetRhsVar(suite+2, "d", &m1, &n1, &lc);
      if ( m1*n1 != 1) { sciprint("Xnonc must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+3,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 1.0 || *stk(lb) < 0.0 || *stk(lc) < 0.0 ) 
	{
	  sciprint("DF < 1.0 or DF <= 0.0 or Xnonc < 0.0 \r\n");
	  Error(999);return 0;
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
	{ sciprint("Missing Df for chi-square law\r\n");
	Error(999);return 0;
	}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Df must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if  ( *stk(la) <= 0.0)
	{
	  sciprint("Rand: DF <= 0 \r\n");Error(999);return 0;
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
	{ sciprint("Missing Av for Poisson law\r\n");
	Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Av must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 0.0 )
	{
	  sciprint("Av < 0 \r\n");
	  Error(999);return 0;
	}
      for ( i=0 ; i < ResL*ResC ; i++) 
	{
	  *stk(lr+i)= (double) C2F(ignpoi)(stk(la));
	}
      LhsVar(1) = suite+1;
      PutLhsVar();
      return 0;
    }

  else if ( strcmp(cstk(ls),"exp")==0)
    {
      if ( Rhs != suite ) 
	{ sciprint("Missing Av for exponential law\r\n");
	Error(999);return 0;}
      GetRhsVar(suite, "d", &m1, &n1, &la);
      if ( m1*n1 != 1) { sciprint("Av must be scalar\r\n");Error(999);return 0;}
      CreateVar(suite+1,"d",&ResL,&ResC,&lr);
      if ( *stk(la) < 0.0 ) 
	{
	  sciprint("Av < 0.0 \r\n");
	  Error(999);return 0;
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
      sciprint("%s Wrong argument %s\r\n",fname,cstk(ls));
      Error(999);
      return 0;
    }      
}

static TabF Tab[]={ 
  {RandI, "Rand"},
};

int C2F(randlib)()
{
#ifdef DEBUG 
  sciprint("Inside randlib Fin == %d  %s\r\n",Fin,Tab[Fin-1].name);
#endif
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name);
  return 0;
}
