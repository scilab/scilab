#include "stack-c.h"
/*     [W,VS,A]=dgeesx(A)      */
/*     [W,VS]=dgeesx(A)      */
/*     [W]=dgeesx(A)      */
/*      SUBROUTINE DGEESX( JOBVS, SORT, SELECT, SENSE, N, A, LDA, SDIM,
     $                   WR, WI, VS, LDVS, RCONDE, RCONDV, WORK, LWORK,
     $                   IWORK, LIWORK, BWORK, INFO )   */

#define MAX(x,y)	(((x)>(y))?(x):(y))
#define MIN(x,y)	(((x)<(y))?(x):(y))

#ifdef WIN32
int select()
{
	return 1;
}
#endif

int intdgeesx(fname)
     char* fname;
{ 
  int A = 1, W = 2, VS = 3, WORK;
  int M, N, lA, lVS;
  int NLHS, it=1;
  int un=1, lWORK, LWORKMIN, LWORK, IWORK, LIWORK;
  int LDA, LDVS, lWR, lWI;
  int INFO, SDIM, BWORK;
  double RCONDE,RCONDV;
  char *JOBVS, *SORT, *SENSE;
  
#ifndef WIN32
  extern select();
#endif



  CheckRhs(1,1) ;  CheckLhs(1,3) ;
  /*--------------------A---------------------------*/
  GetRhsVar(A, "d", &M, &N, &lA);
  CheckSquare(A, M, N);
  /*--------------------W---------------------------*/
  CreateCVar(W, "d", &it, &N, &un, &lWR, &lWI);

  NLHS=Lhs; LDA = MAX(1,N); LDVS=MAX(1,N); SORT="N"; SENSE="N";

  switch ( NLHS ) {
  case 3:     /*      [W,VS,A]=dgeesx(A)    */
  case 2:     /*        [W,VS]=dgeesx(A)    */
    JOBVS="V";
  /*--------------------VS---------------------------*/
    CreateVar(VS, "d", &N, &N, &lVS);
  /*--------------------WORK---------------------------*/
    WORK=4; LWORKMIN = MAX(1, 3*N);
    LWORK=Maxvol(WORK,"d");  /* max memory currently available */
    if (LWORK < LWORKMIN) Scierror(999,"%s:  not enough memory (use stacksize) \r\n",fname);
    CreateVar(WORK, "d", &LWORK, &un, &lWORK);

    C2F(dgeesx)(JOBVS, SORT, select, SENSE, &N, stk(lA), &LDA, &SDIM, 
                stk(lWR), stk(lWI),
                stk(lVS), &LDVS, &RCONDE, &RCONDV, stk(lWORK), 
                &LWORK, IWORK, LIWORK, BWORK, &INFO);
    if (INFO != 0) Errorinfo("DGEESX ", INFO);
    LhsVar(1)=W; LhsVar(2)=VS; LhsVar(3)=A;
    break;
  case 1:   /*  [W]=dgeesx(A)    */
    JOBVS="N";
  /*--------------------WORK---------------------------*/
    WORK=3; LWORKMIN = MAX(1, 3*N);
    LWORK=Maxvol(WORK, "d"); /* max memory currently available */
    if (LWORK < LWORKMIN)  Scierror(999,"%s:  not enough memory (use stacksize) \r\n",fname); 
    CreateVar(WORK, "d", &LWORK, &un, &lWORK);

    C2F(dgeesx)(JOBVS, SORT, select, SENSE, &N, stk(lA), &LDA, &SDIM, 
                stk(lWR), stk(lWI),
                VS, &LDVS, &RCONDE, &RCONDV, stk(lWORK), 
                &LWORK, IWORK, LIWORK, BWORK, &INFO);
    if (INFO != 0) Errorinfo("DGEESX ", INFO);
    LhsVar(1)=W;
    break;
  default:
    return 0;
  }
  return 0;
}



