/* Copyright Jean-Philippe Chancelier 
   ENPC */
#include <string.h>
#include "../../routines/stack-c.h"

/** external functions to be called through this interface **/
#ifdef __STDC__
void  sciprint(char *fmt,...);
#else
/*VARARGS0*/
void sciprint();
#endif  
 
extern int C2F(cdfbet) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdfnbn) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdffnc) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdfbin) __PARAMS((int *,double *,double *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdfchi) __PARAMS((int *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdft) __PARAMS((int *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdfpoi) __PARAMS((int *,double *,double *,double *,double *, int *,double *));
extern int C2F(cdff) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));
extern int C2F(cdfgam) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));
extern int C2F(cdff) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));
extern int C2F(cdfchn) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));
extern int C2F(cdfnor) __PARAMS((int *,double *,double *,double*,double *,double *, int *,double *));

static int CdfBase __PARAMS((char *fname,int inarg,int oarg,int *callpos,char *option,char *errnames,int which,int (*fonc)(),void (*foncErr)()));

static void cdfbetErr __PARAMS((int status,double bound));
static void cdfbinErr __PARAMS((int status,double bound));
static void cdfchiErr __PARAMS((int status,double bound));
static void cdffErr __PARAMS((int status,double bound));
static void cdffncErr __PARAMS((int status,double bound));
static void cdfgamErr __PARAMS((int status,double bound));
static void cdfnbnErr __PARAMS((int status,double bound));
static void cdfnorErr __PARAMS((int status,double bound));
static void cdfpoiErr __PARAMS((int status,double bound));
static void cdftErr __PARAMS((int status,double bound));
static void cdfchnErr __PARAMS((int status,double bound));

/**************************************************
 *  hand written interface 
 *      Interface for cdfbet 
 *
 *      SUBROUTINE CDFBET( WHICH, P, Q, X, Y, A, B, STATUS, BOUND )
 *               Cumulative Distribution Function
 *                         BETa Distribution
 ***********************************************************************/

int cdfbetI(fname)
  char* fname;
{ 
  int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[6] = {4,5,0,1,2,3};
      CdfBase(fname,4,2,callpos,"PQ","X,Y,A and B",1,C2F(cdfbet),
	       cdfbetErr);
    }
  else if ( strcmp(cstk(l1),"XY")==0) 
    {
      static int callpos[6] = {2,3,4,5,0,1};
      CdfBase(fname,4,2,callpos,"XY","A,B,P and Q",2,C2F(cdfbet),
	       cdfbetErr);
    }
  else if ( strcmp(cstk(l1),"A")==0) 
    {
      static int callpos[6] = {1,2,3,4,5,0};
      CdfBase(fname,5,1,callpos,"A","B,P,Q,X and Y",3,C2F(cdfbet),
	       cdfbetErr);
    }
  else if ( strcmp(cstk(l1),"B")==0) 
    {
      static int callpos[6] = {0,1,2,3,4,5};
      CdfBase(fname,5,1,callpos,"B","P,Q,X,Y and A",4,C2F(cdfbet),
	       cdfbetErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
      return 0;
    }
  return 0;
}

static void cdfbetErr(status,bound)
     int status;
     double bound;
{
  static char param[]="-PQXYAB";
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 4 : Scierror(999," X + Y .ne. 1 \r\n");break;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdfbin
 *
 *     SUBROUTINE CDFBIN ( WHICH, P, Q, S, XN, PR, OMPR, STATUS, BOUND )
 *              Cumulative Distribution Function
 *                        BINomial distribution
 ***********************************************************************/


int cdfbinI(fname)
  char* fname;
{ 
  int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[6] = {4,5,0,1,2,3};
      CdfBase(fname,4,2,callpos,"PQ","S,Xn,Pr and Ompr",1,C2F(cdfbin),
	       cdfbinErr);
    }
  else if ( strcmp(cstk(l1),"S")==0) 
    {
      static int callpos[6] = {3,4,5,0,1,2};
      CdfBase(fname,5,1,callpos,"S","Xn,Pr,Ompr,P and Q",2,C2F(cdfbin),
	       cdfbinErr);
    }
  else if ( strcmp(cstk(l1),"Xn")==0) 
    {
      static int callpos[6] = {2,3,4,5,0,1};
      CdfBase(fname,5,1,callpos,"Xn","Pr,OMPr,P,Q and S",3,C2F(cdfbin),
	       cdfbinErr);
    }
  else if ( strcmp(cstk(l1),"PrOmpr")==0) 
    {
      static int callpos[6] = {0,1,2,3,4,5};
      CdfBase(fname,4,2,callpos,"PrOmpr","P,Q,S  and Xn",4,C2F(cdfbin),
	       cdfbinErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}

static void cdfbinErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"Which", "P","Q","Pr","Ompr","S","Xn"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 4 : Scierror(999," Pr + Ompr .ne. 1 \r\n");break;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdfchi
 *
 *      SUBROUTINE CDFCHI( WHICH, P, Q, X, DF, STATUS, BOUND )
 *              Cumulative Distribution Function
 *              CHI-Square distribution
 ***********************************************************************/


int cdfchiI(fname)
  char* fname;
{ 
  int minrhs = 3,maxrhs = 4,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[4] = {2,3,0,1};
      CdfBase(fname,2,2,callpos,"PQ","X and Df",1,C2F(cdfchi),
	       cdfchiErr);
    }
  else if ( strcmp(cstk(l1),"X")==0) 
    {
      static int callpos[4] = {1,2,3,0};
      CdfBase(fname,3,1,callpos,"X","Df, P and Q",2,C2F(cdfchi),
	       cdfchiErr);
    }
  else if ( strcmp(cstk(l1),"Df")==0) 
    {
      static int callpos[4] = {0,1,2,3};
      CdfBase(fname,3,1,callpos,"Df","P,Q and X",3,C2F(cdfchi),
	       cdfchiErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}

static void cdfchiErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","X","Df"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 10 : Scierror(999,"cdfchi: Error in  cumgam\r\n");break;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdff
 *               Cumulative Distribution Function
 *              F distribution
 ***********************************************************************/


int cdffI(fname)
  char* fname;
{ 
  int minrhs = 4,maxrhs = 5,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[5] = {3,4,0,1,2};
      CdfBase(fname,3,2,callpos,"PQ","F,Dfn and Dfd",1,C2F(cdff),
	       cdffErr);
    }
  else if ( strcmp(cstk(l1),"F")==0) 
    {
      static int callpos[5] = {2,3,4,0,1};
      CdfBase(fname,4,1,callpos,"F","Dfn,Dfd,P and Q",2,C2F(cdff),
	       cdffErr);
    }
  else if ( strcmp(cstk(l1),"Dfn")==0) 
    {
      static int callpos[5] = {1,2,3,4,0};
      CdfBase(fname,4,1,callpos,"Dfn","Dfd,P,Q and F",3,C2F(cdff),
	       cdffErr);
    }
  else if ( strcmp(cstk(l1),"Dfd")==0) 
    {
      static int callpos[5] = {0,1,2,3,4};
      CdfBase(fname,4,1,callpos,"Dfd","P,Q,F and Dfn",4,C2F(cdff),
	       cdffErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}


static void cdffErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","F","Dfn","Dfd"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}



/**************************************************
 *  hand written interface 
 *      Interface for cdffnc
 *               Cumulative Distribution Function
 *              Non-central F distribution
 ***********************************************************************/


int cdffncI(fname)
  char* fname;
{ 
  int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[6] = {4,5,0,1,2,3};
      CdfBase(fname,4,2,callpos,"PQ","F,Dfn,Dfd and Pnonc",1,C2F(cdffnc),
	       cdffncErr);
    }
  else if ( strcmp(cstk(l1),"F")==0) 
    {
      static int callpos[6] = {3,4,5,0,1,2};
      CdfBase(fname,5,1,callpos,"F","Dfn,Dfd,Pnonc,P and Q",2,C2F(cdffnc),
	       cdffncErr);
    }
  else if ( strcmp(cstk(l1),"Dfn")==0) 
    {
      static int callpos[6] = {2,3,4,5,0,1};
      CdfBase(fname,5,1,callpos,"Dfn","Dfd,Pnonc,P,Q and F",3,C2F(cdffnc),
	       cdffncErr);
    }
  else if ( strcmp(cstk(l1),"Dfd")==0) 
    {
      static int callpos[6] = {1,2,3,4,5,0};
      CdfBase(fname,5,1,callpos,"Dfd","Pnonc,P,Q,F and Dfn",4,C2F(cdffnc),
	       cdffncErr);
    }
  else if ( strcmp(cstk(l1),"Pnonc")==0) 
    {
      static int callpos[6] = {0,1,2,3,4,5};
      CdfBase(fname,5,1,callpos,"Pnonc","P,Q,F,Dfn and Dfd",5,C2F(cdffnc),
	       cdffncErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}


static void cdffncErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","F","Dfn","Dfd","Pnonc"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}



/**************************************************
 *  hand written interface 
 *      Interface for cdfgam
 *               Cumulative Distribution Function
 *              F distribution
 ***********************************************************************/


int cdfgamI(fname)
  char* fname;
{ 
  int minrhs = 4,maxrhs = 5,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[5] = {3,4,0,1,2};
      CdfBase(fname,3,2,callpos,"PQ","X,Shape and Scale",1,C2F(cdfgam),
	       cdfgamErr);
    }
  else if ( strcmp(cstk(l1),"X")==0) 
    {
      static int callpos[5] = {2,3,4,0,1};
      CdfBase(fname,4,1,callpos,"X","Shape,Scale,P and Q",2,C2F(cdfgam),
	       cdfgamErr);
    }
  else if ( strcmp(cstk(l1),"Shape")==0) 
    {
      static int callpos[5] = {1,2,3,4,0};
      CdfBase(fname,4,1,callpos,"Shape","Scale,P,Q and X",3,C2F(cdfgam),
	       cdfgamErr);
    }
  else if ( strcmp(cstk(l1),"Scale")==0) 
    {
      static int callpos[5] = {0,1,2,3,4};
      CdfBase(fname,4,1,callpos,"Scale","P,Q,X and Shape",4,C2F(cdfgam),
	       cdfgamErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}


static void cdfgamErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","X","Shape","Scale"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 10 : Scierror(999," cannot compute the answer \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdfnbn 
 * SUBROUTINE cdfnbn(which,p,q,s,xn,pr,ompr,status,bound)
 *               Cumulative Distribution Function
 *               Negative BiNomial distribution
 ***********************************************************************/

int cdfnbnI(fname)
  char* fname;
{ 
  int minrhs = 5,maxrhs = 6,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[6] = {4,5,0,1,2,3};
      CdfBase(fname,4,2,callpos,"PQ","S,XN,PR and OMPR",1,C2F(cdfnbn),
	       cdfnbnErr);
    }
  else if ( strcmp(cstk(l1),"S")==0) 
    {
      static int callpos[6] = {3,4,5,0,1,2};
      CdfBase(fname,5,1,callpos,"S","XN,PR,OMPR,P and Q",2,C2F(cdfnbn),
	       cdfnbnErr);
    }
  else if ( strcmp(cstk(l1),"Xn")==0) 
    {
      static int callpos[6] = {2,3,4,5,0,1};
      CdfBase(fname,5,1,callpos,"Xn","PR,OMPR,P,Q and S",3,C2F(cdfnbn),
	       cdfnbnErr);
      return 0;
    }
  else if ( strcmp(cstk(l1),"PrOmpr")==0) 
    {
      static int callpos[6] = {0,1,2,3,4,5};
      CdfBase(fname,4,2,callpos,"PrOmpr","P,Q,S and Xn",4,C2F(cdfnbn),
	       cdfnbnErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}

static void cdfnbnErr(status,bound)
     int status;
     double bound;
{
  static char param[]="-PQXYAB";
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 4 : Scierror(999," Pr + Ompr .ne. 1 \r\n");break;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}



/**************************************************
 *  hand written interface 
 *      Interface for cdfnor
 *               Cumulative Distribution Function
 *               NORmal distribution
 ***********************************************************************/


int cdfnorI(fname)
  char* fname;
{ 
  int minrhs = 4,maxrhs = 5,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[5] = {3,4,0,1,2};
      CdfBase(fname,3,2,callpos,"PQ","X,Mean and Std",1,C2F(cdfnor),
	       cdfnorErr);
    }
  else if ( strcmp(cstk(l1),"X")==0) 
    {
      static int callpos[5] = {2,3,4,0,1};
      CdfBase(fname,4,1,callpos,"X","Mean,Std,P and Q",2,C2F(cdfnor),
	       cdfnorErr);
    }
  else if ( strcmp(cstk(l1),"Mean")==0) 
    {
      static int callpos[5] = {1,2,3,4,0};
      CdfBase(fname,4,1,callpos,"Mean","Std,P,Q and X",3,C2F(cdfnor),
	       cdfnorErr);
    }
  else if ( strcmp(cstk(l1),"Std")==0) 
    {
      static int callpos[5] = {0,1,2,3,4};
      CdfBase(fname,4,1,callpos,"Std","P,Q,X and Mean",4,C2F(cdfnor),
	       cdfnorErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}


static void cdfnorErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","X","Mean","Std"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    case 4 : Scierror(999," Std must not be zero \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdfpoi
 * POIsson distribution
 ***********************************************************************/


int cdfpoiI(fname)
  char* fname;
{ 
  int minrhs = 3,maxrhs = 4,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[4] = {2,3,0,1};
      CdfBase(fname,2,2,callpos,"PQ","S and Xlam",1,C2F(cdfpoi),
	       cdfpoiErr);
    }
  else if ( strcmp(cstk(l1),"S")==0) 
    {
      static int callpos[4] = {1,2,3,0};
      CdfBase(fname,3,1,callpos,"S","Xlam,P and Q",2,C2F(cdfpoi),
	       cdfpoiErr);
    }
  else if ( strcmp(cstk(l1),"Xlam")==0) 
    {
      static int callpos[4] = {0,1,2,3};
      CdfBase(fname,3,1,callpos,"Xlam","P,Q and S",3,C2F(cdfpoi),
	       cdfpoiErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}

static void cdfpoiErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","S","Xlam"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}



/**************************************************
 *  hand written interface 
 *      Interface for cdft
 *              T distribution
 ***********************************************************************/


int cdftI(fname)
  char* fname;
{ 
  int minrhs = 3,maxrhs = 4,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[4] = {2,3,0,1};
      CdfBase(fname,2,2,callpos,"PQ","T and Df",1,C2F(cdft),
	       cdftErr);
    }
  else if ( strcmp(cstk(l1),"T")==0) 
    {
      static int callpos[4] = {1,2,3,0};
      CdfBase(fname,3,1,callpos,"T","Df, P and Q",2,C2F(cdft),
	       cdftErr);
    }
  else if ( strcmp(cstk(l1),"Df")==0) 
    {
      static int callpos[4] = {0,1,2,3};
      CdfBase(fname,3,1,callpos,"Df","P,Q and T",3,C2F(cdft),
	       cdftErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}

static void cdftErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","T","Df"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}


/**************************************************
 *  hand written interface 
 *      Interface for cdfchn
 *  Non-central Chi-Square
 ***********************************************************************/


int cdfchnI(fname)
  char* fname;
{ 
  int minrhs = 4,maxrhs = 5,minlhs=1,maxlhs=2,m1,n1,l1;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);
  GetRhsVar(1, "c", &m1, &n1, &l1);
  if ( strcmp(cstk(l1),"PQ")==0) 
    {
      static int callpos[5] = {3,4,0,1,2};
      CdfBase(fname,3,2,callpos,"PQ","X,Df and Pnonc",1,C2F(cdfchn),
	       cdfchnErr);
    }
  else if ( strcmp(cstk(l1),"X")==0) 
    {
      static int callpos[5] = {2,3,4,0,1};
      CdfBase(fname,4,1,callpos,"X","Df,Pnonc,P and Q",2,C2F(cdfchn),
	       cdfchnErr);
    }
  else if ( strcmp(cstk(l1),"Df")==0) 
    {
      static int callpos[5] = {1,2,3,4,0};
      CdfBase(fname,4,1,callpos,"Df","Pnonc,P,Q and X",3,C2F(cdfchn),
	       cdfchnErr);
    }
  else if ( strcmp(cstk(l1),"Pnonc")==0) 
    {
      static int callpos[5] = {0,1,2,3,4};
      CdfBase(fname,4,1,callpos,"Pnonc","P,Q,X and Df",4,C2F(cdfchn),
	       cdfchnErr);
    }
  else 
    {
      Scierror(999,"%s: Wrong first argument %s\r\n",fname,cstk(l1));
    }      
  return 0;
}


static void cdfchnErr(status,bound)
     int status;
     double bound;
{
  static char *param[7]={"X", "P","Q","F","Dfn","Dfd"};
  switch ( status ) 
    {
    case 1 : Scierror(999,"answer appears to be lower than lowest search bound %f\r\n",bound);break;
    case 2 : Scierror(999,"answer appears to be higher than greatest search bound %f\r\n",bound);break;
    case 3 : Scierror(999," P + Q .ne. 1 \r\n");break ;
    default : 
      Scierror(999,"input parameter %c is out of range \r\n\tbound exceeded: %f\r\n",
		    param[-status-1],bound);
    }
}



static TabF Tab[]={ 
  {cdfbetI, "cdfbet"},
  {cdfbinI, "cdfbin"},
  {cdfchiI, "cdfchi"},
  {cdfchnI, "cdfchn"},
  {cdffI, "cdff"},
  {cdffncI, "cdffnc"},
  {cdfgamI, "cdfgam"},
  {cdfnbnI, "cdfnbn"},
  {cdfnorI, "cdfnor"},
  {cdfpoiI, "cdfpoi"},
  {cdftI, "cdft"},
};

int C2F(dcd)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name);
  return 0;
}

static int  CdfBase(fname,inarg,oarg,callpos,option,errnames,which,fonc,foncErr)
     int inarg,oarg,callpos[],which;
     char *option,*errnames,*fname;
     int (*fonc)();
     void (*foncErr)();
{
  int i,status=0,m[6],n[6],l[6];
  double bound;
  if ( Rhs != inarg+1 ) 
    {
      Scierror(999,"%s: Rhs must be %d for '%s' option'\r\n",fname,inarg+1,option);
      return 1;
    }
  for ( i = 0 ; i < inarg ; i++ )
    {
      GetRhsVar(i+2, "d", &m[i], &n[i], &l[i]);
    }
  for ( i = 1 ; i < inarg ; i++) 
    if ( m[i] != m[i-1] || n[i] != n[i-1]) 
      {
	Scierror(999,"%s %s must have same size\r\n",fname,errnames);
	return 1;
      } 
  for ( i = 0 ; i < oarg ; i++) 
    CreateVar(i+2+inarg,"d",&m[0],&n[0],&l[i+inarg]);
  switch ( inarg+oarg) 
    {
    case 5:
      for ( i=0 ; i < m[0]*n[0]; i++) 
	{
	  (*fonc)(&which,stk(l[callpos[0]]+i),stk(l[callpos[1]]+i),
		  stk(l[callpos[2]] +i),stk(l[callpos[3]]+i),
		  stk(l[callpos[4]]+i),
		  &status,&bound);
	  if (status != 0) 
	    {
	      (*foncErr)(status,bound); return 1;
	    }
	}
      break;
    case 6:
      for ( i=0 ; i < m[0]*n[0]; i++) 
	{
	  (*fonc)(&which,stk(l[callpos[0]]+i),stk(l[callpos[1]]+i),
		  stk(l[callpos[2]] +i),stk(l[callpos[3]]+i),
		  stk(l[callpos[4]] +i),stk(l[callpos[5]]+i),
		  &status,&bound);
	  if (status != 0) 
	    {
	      /** Scierror(999,"i=%d\r\n",i); **/
	      (*foncErr)(status,bound); return 1;
	    }
	}
      break;
    case 4:
      for ( i=0 ; i < m[0]*n[0]; i++) 
	{
	  (*fonc)(&which,stk(l[callpos[0]]+i),stk(l[callpos[1]]+i),
		  stk(l[callpos[2]] +i),stk(l[callpos[3]]+i),
		  &status,&bound);
	  if (status != 0) 
	    {
	      (*foncErr)(status,bound); return 1;
	    }
	}
      break;
    }
  for ( i = 0 ; i < oarg ; i++) 
    LhsVar(i+1) = i+2+inarg;
  PutLhsVar();
  return 0;
}




