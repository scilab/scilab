#include "stack-c.h"
/*     [w,VS,A]=zgeesx(A)      */
/*     [w,VS]=zgeesx(A)      */
/*     w=zgeesx(A)      */
/*   SUBROUTINE ZGEESX( JOBVS, SORT, SELECT, SENSE, N, A, LDA, SDIM, W,   
     $                   VS, LDVS, RCONDE, RCONDV, WORK, LWORK, RWORK,
     $                   BWORK, INFO )  */

#define MAX(x,y)	(((x)>(y))?(x):(y))
#define MIN(x,y)	(((x)<(y))?(x):(y))

#ifdef WIN32
int select()
{
	return 1;
}
#endif

int intzgeesx(fname)
     char* fname;
{ 
  int A = 1, W = 2, VS = 3, RWORK = 4, WORK = 5;
  int M,N,lA;
  int NLHS;
  int lVS;
  int un=1; int lWORK, LWORKMIN, LWORK;
  int LDA, LDVS, lW, lRWORK;
  int INFO, SDIM, BWORK;
  double RCONDE,RCONDV;
  char *JOBVS;

#ifndef WIN32
  extern select();
#endif

  int IT=1;

  CheckRhs(1,1) ;  CheckLhs(1,3) ;

  /*--------------------A---------------------------*/
  GetRhsVar(A, "z", &M, &N, &lA); 
  CheckSquare(A, M, N);
  NLHS=Lhs;     LDA = MAX(1,N); LDVS=MAX(1,N);
  switch ( NLHS ) {
  case 3: 
    /*  [W,VS,As]=zgeesx(A)    */
    JOBVS="V";
  /*--------------------W, VS, RWORK---------------------------*/
    CreateVar(W, "z", &N, &un, &lW);
    CreateVar(VS, "z", &N, &N, &lVS);
    CreateVar(RWORK, "d", &N, &un, &lRWORK);
  /*--------------------WORK---------------------------*/
    LWORKMIN = MAX(1, 2*N);
    LWORK=Maxvol(WORK,"z");  /* max memory currently available */
    if (LWORK < LWORKMIN) Scierror(999,"%s:  not enough memory (use stacksize) \r\n",fname); 
    CreateVar(WORK, "z", &LWORK, &un, &lWORK);

    C2F(zgeesx)(JOBVS, "N", select, "N", &N, zstk(lA), &LDA, &SDIM, zstk(lW), 
                zstk(lVS), &LDVS, &RCONDE, &RCONDV, zstk(lWORK), 
                &LWORK, stk(lRWORK), BWORK, &INFO);
    if (INFO != 0) Errorinfo("ZGEESX ", INFO);
    LhsVar(1)=W; LhsVar(2)=VS; LhsVar(3)=A;
    break;
  case 2:
    /*  [W,VS]=zgeesx(A)    */
    JOBVS="V";
    CreateVar(W, "z", &N, &un, &lW);
    CreateVar(VS, "z", &N, &N, &lVS);
    RWORK=4;  CreateVar(RWORK, "d", &N, &un, &lRWORK);
    WORK=5;   LWORKMIN = MAX(1, 2*N);
    LWORK=Maxvol(WORK, "z"); /* max memory currently available */
    if (LWORK < LWORKMIN) Scierror(999,"%s:  not enough memory (use stacksize) \r\n",fname); 
    CreateVar(WORK, "z", &LWORK, &un, &lWORK);

    C2F(zgeesx)(JOBVS, "N", select, "N", &N, zstk(lA), &LDA, &SDIM, zstk(lW), 
                zstk(lVS), &LDVS, &RCONDE, &RCONDV, zstk(lWORK), 
                &LWORK, stk(lRWORK), BWORK, &INFO);
    if (INFO != 0) Errorinfo("DGEESX ", INFO);
    LhsVar(1)=W; LhsVar(2)=VS; 
    break;
  case 1:
    /*  w=zgeesx(A)    */
    JOBVS="N";
    CreateVar(W, "z", &N, &un, &lW);
    RWORK=3;  CreateVar(RWORK, "d", &N, &un, &lRWORK);
    WORK=4;   LWORKMIN = MAX(1, 2*N);
    LWORK=Maxvol(WORK, "z"); /* max memory currently available */
    if (LWORK < LWORKMIN) Scierror(999,"%s:  not enough memory (use stacksize) \r\n",fname); 
    CreateVar(WORK, "z", &LWORK, &un, &lWORK);

    C2F(zgeesx)(JOBVS, "N", select, "N", &N, zstk(lA), &LDA, &SDIM, zstk(lW), 
                VS, &LDVS, &RCONDE, &RCONDV, zstk(lWORK), 
                &LWORK, stk(lRWORK), BWORK, &INFO);
    if (INFO != 0) C2F(errorinfo)("zgeesx ", &INFO, 5L);
    LhsVar(1)=W;
    break;
  default:
    return 0;
  }
  return 0;
}



