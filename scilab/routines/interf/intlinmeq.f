C INTLINMEQ.F - Gateway function for solving Sylvester and Lyapunov matrix
C            equations using SLICOT routines SB04MD, SB04ND, SB04PD,
C            SB04QD, SB04RD, SB03MD, and SB03OD.
C
C Copyright INRIA 2001
C Scilab syntax
C   [X(,sep)] = linmeq(task,A(,B),C,flag,trans(,schur))
C
C   task = 1 :      [X] = linmeq(1,A,B,C,flag,trans,schur)
C   task = 2 :  [X,sep] = linmeq(2,A,C,flag,trans)
C                   [X] = linmeq(2,A,C,flag,trans)
C   task = 3 :      [X] = linmeq(3,A,C,flag,trans)
C         
C Purpose:
C   To solve the Sylvester and Lyapunov linear matrix equations
C
C   task = 1:
C
C         op(A)*X + X*op(B) = C,                          (1a)
C
C         op(A)*X*op(B) + X = C,                          (1b)
C
C   task = 2:
C
C         op(A)'*X + X*op(A) = C,                         (2a)
C
C         op(A)'*X*op(A) - X = C,                         (2b)
C
C   task = 3:
C
C         op(A)'*(op(X)'*op(X)) + (op(X)'*op(X))*op(A) =
C                               -  op(C)'*op(C),          (3a)
C
C         op(A)'*(op(X)'*op(X))*op(A) - op(X)'*op(X) =
C                                     - op(C)'*op(C),     (3b)
C
C   where op(M) = M, if trans = 0, and op(M) = M', if trans = 1.
C
C Input parameters: 
C   task  - integer option to determine the equation type:
C           = 1 : solve the Sylvester equation (1a) or (1b);
C           = 2 : solve the Lyapunov equation (2a) or (2b);
C           = 3 : solve for the Cholesky factor op(X) the Lyapunov
C                 equation (3a) or (3b).
C   A     - real coefficient N-by-N matrix.
C           When task = 3, matrix A must be stable.
C   B     - another real coefficient M-by-M matrix for
C           equations (1a) or (1b).
C   C     - right hand side matrix.
C           task = 1 : C is N-by-M;
C           task = 2 : C is N-by-N symmetric;
C           task = 3 : op(C) is P-by-N.
C   flag  - (optional) integer vector of length 3 or 2 containing
C           options.
C           task = 1 : flag has length 3
C                flag(1) = 0 : solve the continuous-time equation (1a);
C                              otherwise, solve the discrete-time
C                              equation (1b).
C                flag(2) = 1 : A is (quasi) upper triangular;
C                          2 : A is upper Hessenberg;
C                              otherwise, A is in general form.
C                flag(3) = 1 : B is (quasi) upper triangular;
C                          2 : B is upper Hessenberg;
C                              otherwise, B is in general form.
C           task = 2 : flag has length 2
C                flag(1) = 0 : solve continuous-time equation (2a);
C                              otherwise, solve discrete-time
C                              equation (2b).
C                flag(2) = 1 : A is (quasi) upper triangular;
C                              otherwise, A is in general form.
C           task = 3 : flag has length 2
C                flag(1) = 0 : solve continuous-time equation (3a);
C                              otherwise, solve discrete-time
C                              equation (3b).
C                flag(2) = 1 : A is (quasi) upper triangular;
C                              otherwise, A is in general form.
C           Default:    flag(1) = 0, flag(2) = 0 (, flag(3) = 0).
C   trans - (optional) integer specifying a transposition option.
C           trans = 0 : solve the equations (1) - (3) with op(M) = M.
C           trans = 1 : solve the equations (1) - (3) with op(M) = M'.
C           trans = 2 : solve the equations (1) with op(A) = A',
C                                                    op(B) = B.
C           trans = 3 : solve the equations (1) with op(A) = A,
C                                                    op(B) = B'.
C           Default:    trans = 0.
C   schur - (optional) integer specifying whether the Hessenberg-Schur
C           or Schur method should be used.
C           Available for task = 1. 
C           schur = 1 : Hessenberg-Schur method (one matrix is reduced
C                       to Schur form).
C           schur = 2 : Schur method (two matrices are reduced to Schur
C                       form).
C           Default:    schur = 1.
C
C Output parameters:
C   X     - solution of the equation (or its Cholesky factor for (3)).
C   sep   - (optional) estimator of Sep(op(A),-op(A)') for (2.a) or
C           Sepd(A,A') for (2.b).
C
C Comments:
C   1. For equation (1a) or (1b), when schur = 1, the Hessenberg-Schur 
C      method is used, reducing one matrix to Hessenberg form and the
C      other one to a real Schur form.
C      Otherwise, both matrices are reduced to real Schur forms.
C      If one or both matrices are already reduced to Schur/Hessenberg
C      forms, this could be specified by flag(2) and flag(3).
C      For general matrices, the Hessenberg-Schur method could be 
C      significantly more efficient than the Schur method.
C   2. For equation (3a) or (3b), the computed matrix X is the Cholesky
C      factor of the solution, i.e., the real solution is op(X)'*op(X),
C      where X is an N-by-N upper triangular matrix.
C
C References:
C   This interface is based on the SLICOT routines:
C   SB04PD, SB04MD, SB04QD, DTRSYL, SB04PY, SB04ND,  SB04RD, SB03MD, SB03OD
C   
C
C Revisions:
C   Adapted from the Slicot Matlab Mexfile by S. Steer Oct 2001
C
C **********************************************************************
C
C
      SUBROUTINE intlinmeq  (fname)

      include '../stack.h'
      character*(*) fname


C .. Parameters ..
      double precision ONE ,ZERO 
      parameter (ONE =1.0D0,ZERO =0.0D0)

C .. Scalar parameters used by SLICOT subroutines ..
      character DICO ,FACT ,FACTA ,FACTB ,JOB ,SCHU ,TRANA ,TRANB ,ULA ,
     $   ULB 
      integer INFO ,ISGN ,LDA ,LDB ,LDC ,LDU ,LDV ,NDWORK ,M ,N ,P 
      double precision FERR ,SCALE ,SEP ,TOL 

      integer lA, lB, lC, lDWORK, lU, lV, lWR, lWI
C
C .. Local variables and constant dimension arrays ..
      logical PERTRB 
      integer FLAG (3),IB ,IP  ,J ,LDW1 ,LDW2 ,NIWORK ,MC ,MXMN ,
     $   NC ,NM ,NSCHUR ,TASK ,TRANS 
      double precision TEMP 
C
C .. External functions ..
      logical lsame, createvar, checkrhs, checklhs, getrhsvar
      logical getscalar, getrmat, iscomplex
      external lsame , createvar, checkrhs, checklhs, getrhsvar
      external getscalar, getrmat
C
C .. External subroutines ..
      external dlacpy ,dlaset ,dswap ,dtrsyl ,sb03md ,sb03od ,sb04md ,
     $   sb04nd ,sb04pd ,sb04py ,sb04qd ,sb04rd 
C
C ..Intrinsic functions..
      intrinsic max ,min 

C Check for proper number of arguments.

C
      if(.not.checkrhs(fname,3,7)) return
      if(.not.checklhs(fname,1,2)) return

C
C Check dimensions of input parameters and read/set scalar parameters.
C
C   task

      if(.not.getrhsvar(1,'i', Mt, Nt, ilTASK)) return
      if(Mt*Nt.ne.1) then
         err=1
         call error(204)
         return
      endif
      TASK=istk(ilTASK)
  
      if (TASK.LT.1 .OR.TASK.GT.3 )THEN 
         buf='TASK HAS 1, 2, OR 3 THE ONLY ADMISSIBLE VALUES'
         call error(1003)
         return
      endif 
C
      if (TASK.EQ.1 )THEN 
         if (RHS.LT.4 )THEN 
            buf='TASK=1: LINMEQ REQUIRES AT LEAST 4 INPUT ARGUMENTS'
            call error(1003)
            return
         endif 
         IP =6
      else 
         IP =5
      endif 
C
C   trans
C
      TRANS =0
      if (RHS.GE.IP )THEN 
         if(.not.getrhsvar(IP,'i', Mt, Nt, ilTRANS)) return
         if(Mt*Nt.gt.1) then
            err=1
            call error(204)
            return
         endif
         if(Mt*Nt.eq.1) TRANS=istk(ilTRANS)
      endif 
      if (TASK.EQ.1 .AND.(TRANS.LT.0 .OR.TRANS.GT.3 ))THEN 
         buf='TRANS HAS 0, 1, 2, OR 3 THE ONLY ADMISSIBLE VALUES'
         call error(1003)
         return
      elseif (TASK.NE.1 .AND.(TRANS.LT.0 .OR.TRANS.GT.1 ))THEN 
         buf='TRANS HAS 0, OR 1 THE ONLY ADMISSIBLE VALUES'
         call error(1003)
         return
      endif 
C
C   schur
C
      if (TASK.EQ.1 )THEN 
         NSCHUR =1
         if (RHS.EQ.IP +1) THEN 
            if(.not.getrhsvar(IP+1,'i', Mt, Nt, ilSCHUR)) return
            if(Mt*Nt.gt.1) then
               err=1
               call error(204)
               return
            endif
            if(Mt*Nt.eq.1) NSCHUR=istk(ilSCHUR)
            if (NSCHUR.LT.1 .OR.NSCHUR.GT.2 )THEN 
               buf='SCHUR HAS 1, OR 2 THE ONLY ADMISSIBLE VALUES'
               call error(1003)
               return
            endif 
         endif 
      endif 
C
C   A(NxN), (B(MxM),) C(NxM), or C(NxN), or op(C)(PxN)
C
      if(.not.getrhsvar(2,'d', MA, NA, lA)) return
      if(iscomplex(2)) then
         err=2
         call error(52)
         return
      endif
      if(MA.NE.NA) then
         err=2
         call error(20)
         return
      endif

      N=MA

      if (TASK.EQ.1) THEN 
         if(.not.getrhsvar(3,'d', MB, NB, lB)) return
         if(iscomplex(3)) then
            err=3
            call error(52)
            return
         endif
         if(MB.NE.NB) then
            err=3
            call error(20)
            return
         endif
 
         M=Mb
         if(.not.getrhsvar(4,'d', MC, NC, lC)) return
         if(iscomplex(4)) then
            err=4
            call error(52)
            return
         endif
         if(MC.NE.N) then
            err=4
            call error(89)
            return
         endif

         M=NC
         lhsvar(1)=4
      else
         if(.not.getrhsvar(3,'d', MC, NC, lC)) return
         if(iscomplex(3)) then
            err=3
            call error(52)
            return
         endif
c         if (TASK.EQ.3 )THEN 
         if (TRANS.EQ.0 )THEN 
            P = MC
            if(NC.ne.N) then
               err=3
               call error(89)
               return
            endif
         else 
            if(MC.ne.N) then
               err=3
               call error(89)
               return
            endif
            P = NC
c            endif 
         endif 
         lhsvar(1)=3
      endif 
C
      if(TASK.eq.1) then
         IP=5
      else
         IP=4
      endif
      FLAG (1)=0
      FLAG (2)=0
      FLAG (3)=0
      if (RHS.GE.IP )THEN 
C
C   flag
C
         if(.not.getrhsvar(IP,'i', Mt, Nt, ilFLAG)) return
         if (TASK.EQ.1 )THEN 
            if (MT*NT.GT.3) then
               buf='FLAG MUST BE A VECTOR WITH AT MOST 3 ELEMENTS'
               call error(1003)
               return
            endif
         else 
            if (MT*NT.GT.2 ) THEN
               buf='FLAG MUST BE A VECTOR WITH AT MOST 2 ELEMENTS'
               call error(1003)
               return
            endif
         endif 
         DO I=1,MT*NT
            FLAG(I)=istk(ilFLAG-1+i)
         enddo
      endif 
C
C Determine the lenghts of working arrays.
C Use a larger value for NDWORK for enabling calls of block algorithms
C in DGEES, and possibly in DGEHRD, DGEQRF, DGERQF, SB04PD.
C
      LDA =MAX (1,N )
      if (TASK.EQ.1 )THEN 
         LDB =MAX (1,M )
         if (NSCHUR.EQ.2 )THEN 
            if (FLAG (2).EQ.1)THEN 
               LDW1 =0
               LDW2 =0
            else 
               LDW1 =1+2*N 
               LDW2 =3*N 
            endif 
            IB =0
            if (FLAG (3).NE.1)THEN 
               IB =2*M 
               if (FLAG (2).EQ.1)IB =IB +1
               LDW2 =MAX (LDW2 ,IB +3*M )
            endif 
            LDW2 =MAX (LDW2 ,IB +2*N )
            NDWORK =MAX (1,LDW1 +LDW2 )
         else 
            if (FLAG (2)*FLAG (3).EQ.1)THEN 
               NIWORK =0
               if (FLAG (1).NE.0)THEN 
                  NDWORK =2*N 
               else 
                  NDWORK =0
               endif 
            elseif (FLAG (2)*FLAG (3).EQ.2)THEN 
               MXMN =MAX (M ,N )
               NIWORK =2*MXMN 
               NDWORK =2*MXMN *(4+2*MXMN )
            else 
               NIWORK =4*N 
               NDWORK =MAX (1,5*M ,N +M )
               if (FLAG (1).EQ.0)THEN 
                  NDWORK =MAX (NDWORK ,2*N *N +8*N )
               else 
                  NDWORK =MAX (NDWORK ,2*N *N +9*N )
               endif 
            endif 
         endif 
         NM =M 
C
      elseif (TASK.EQ.2 )THEN 
         NDWORK =MAX (1,N *N ,3*N )
         if (LHS.EQ.2 )THEN 
            NDWORK =MAX (NDWORK ,2*N *N )
            if (FLAG (1).NE.0)NDWORK =MAX (NDWORK ,2*(N *N +N ))
         endif 
         NM =N 
      endif 
      if (TASK.NE.3 )THEN 
         LDC =LDA 
      else 
         if (TRANS.EQ.0 )THEN 
            LDC =MAX (1,N ,P )
         else 
            LDC =LDA 
         endif 
         MXMN =MIN (P ,N )
         NDWORK =MAX (1,4*N +MXMN )
         NM =N 
      endif 
C
C Allocate variable dimension local arrays.
C
      if (TASK.EQ.1 )THEN 
         if(.NOT.CREATEVAR(NBVARS+1,'d',NDWORK,1,lDWORK)) RETURN
         if (NSCHUR.EQ.2 )THEN 
            if (FLAG (2).EQ.1)THEN 
               FACTA ='S'
               LDU =1
               if(.NOT.CREATEVAR(NBVARS+1,'d',LDU,1,lU)) RETURN
            else 
               FACTA ='N'
               LDU =LDA 
               if(.NOT.CREATEVAR(NBVARS+1,'d',LDU,N,lU)) RETURN
            endif 
            if (FLAG (3).EQ.1)THEN 
               FACTB ='S'
               LDV =1
               if(.NOT.CREATEVAR(NBVARS+1,'d',LDV,1,lV)) RETURN
            else 
               FACTB ='N'
               LDV =LDB 
               if(.NOT.CREATEVAR(NBVARS+1,'d',LDV,M,lV)) RETURN
            endif 
         else 
            SCHU ='N'
            if(.NOT.CREATEVAR(NBVARS+1,'i',NIWORK,1,lIWORK)) RETURN
            if (FLAG (2).EQ.1)THEN 
               if (FLAG (3).EQ.1)THEN 
                  SCHU ='S'
               elseif (FLAG (3).EQ.2)THEN 
                  SCHU ='A'
               endif 
            elseif (FLAG (2).EQ.2.AND. FLAG (3).EQ.1)THEN 
               SCHU ='B'
            endif 
C
            if (LSAME (SCHU ,'N'))THEN 
               LDU =LDB 
               if(.NOT.CREATEVAR(NBVARS+1,'d',LDU,M,lU)) RETURN
            endif 
         endif 
C
      elseif (TASK.EQ.2 )THEN 
         LDU =LDA 
         if(.NOT.CREATEVAR(NBVARS+1,'d',LDU,N,lU)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',NDWORK,1,lDWORK)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',N,1,lWI)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',N,1,lWR)) RETURN
         if (LHS.EQ.1 )THEN 
            if(.NOT.CREATEVAR(NBVARS+1,'i',1,1,lIWORK)) RETURN
         else 
            if(.NOT.CREATEVAR(NBVARS+1,'i',N*N,1,lIWORK)) RETURN
         endif 
      else 
         LDU =LDA 
         if(.NOT.CREATEVAR(NBVARS+1,'d',LDU,N,lU)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',NDWORK,1,lDWORK)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',N,1,lWI)) RETURN
         if(.NOT.CREATEVAR(NBVARS+1,'d',N,1,lWR)) RETURN
      endif 


C
C Do the actual computations.
C
      if (TASK.EQ.1 )THEN 
C
         if (NSCHUR.EQ.2 )THEN 
            if (TRANS.EQ.0 )THEN 
               TRANA ='N'
               TRANB ='N'
            elseif (TRANS.EQ.1 )THEN 
               TRANA ='T'
               TRANB ='T'
            elseif (TRANS.EQ.2 )THEN 
               TRANA ='T'
               TRANB ='N'
            elseif (TRANS.EQ.3 )THEN 
               TRANA ='N'
               TRANB ='T'
            endif 
            if (FLAG (1).NE.0)THEN 
               DICO ='D'
            else 
               DICO ='C'
            endif 
            ISGN =1
            call SB04PD (DICO ,FACTA ,FACTB ,TRANA ,TRANB ,ISGN ,N ,M ,
     $         stk(lA), LDA ,stk(lU),LDU ,stk(lB),LDB ,stk(lV),LDV ,
     $         stk(lC), LDC ,SCALE ,stk(lDWORK),NDWORK ,INFO )
         else 
            if (TRANS.EQ.0 )THEN 
               if (LSAME (SCHU ,'S'))THEN 
                  TRANA ='N'
                  TRANB ='N'
               else 
                  ULA ='U'
                  ULB ='U'
               endif 
            elseif (TRANS.EQ.1 )THEN 
               if (LSAME (SCHU ,'S'))THEN 
                  TRANA ='T'
                  TRANB ='T'
               else 
                  ULA ='L'
                  ULB ='L'
                  DO 10J =2,N 
                     call DSWAP (J -1,stk(lA+1-1 +(J-1)*LDA),1,stk(lA+J-
     $                  1 +(1-1)*LDA),LDA )
   10             CONTINUE 
                  DO 20J =2,M 
                     call DSWAP (J -1,stk(lB+1-1 +(J-1)*LDB),1,stk(lB+J-
     $                  1 +(1-1)*LDB),LDB )
   20             CONTINUE 
               endif 
            elseif (TRANS.EQ.2 )THEN 
               if (LSAME (SCHU ,'S'))THEN 
                  TRANA ='T'
                  TRANB ='N'
               else 
                  ULA ='L'
                  ULB ='U'
                  DO 30J =2,N 
                     call DSWAP (J -1,stk(lA+1-1 +(J-1)*LDA),1,stk(lA+J-
     $                  1 +(1-1)*LDA),LDA )
   30             CONTINUE 
               endif 
            elseif (TRANS.EQ.3 )THEN 
               if (LSAME (SCHU ,'S'))THEN 
                  TRANA ='N'
                  TRANB ='T'
               else 
                  ULA ='U'
                  ULB ='L'
                  DO 40J =2,M 
                     call DSWAP (J -1,stk(lB+1-1 +(J-1)*LDB),1,stk(lB+J-
     $                  1 +(1-1)*LDB),LDB )
   40             CONTINUE 
               endif 
            endif 
C
            if (LSAME (SCHU ,'N'))THEN 
C
               SCALE =ONE 
               if (FLAG (1).EQ.0)THEN 
                  call SB04MD (N ,M ,stk(lA),LDA ,stk(lB),LDB ,stk(lC),
     $               LDC ,stk(lU),LDU ,istk(lIWORK),stk(lDWORK),NDWORK ,
     $               INFO )
               else 
                  call SB04QD (N ,M ,stk(lA),LDA ,stk(lB),LDB ,stk(lC),
     $               LDC ,stk(lU),LDU ,istk(lIWORK),stk(lDWORK),NDWORK ,
     $               INFO )
               endif 
C
            elseif (LSAME (SCHU ,'S'))THEN 
C
               if (FLAG (1).EQ.0)THEN 
                  call DTRSYL (TRANA ,TRANB ,1,N ,M ,stk(lA),LDA ,
     $               stk(lB), LDB ,stk(lC),LDC ,SCALE ,INFO )
               else 
                  call SB04PY (TRANA ,TRANB ,1,N ,M ,stk(lA),LDA ,
     $               stk(lB), LDB ,stk(lC),LDC ,SCALE ,stk(lDWORK),INFO)
               endif 
            else 
               SCALE =ONE 
               TOL =ZERO 
C
C              Default tolerance (epsilon_machine) is used.
C
               if (FLAG (1).EQ.0)THEN 
                  call SB04ND (SCHU ,ULA ,ULB ,N ,M ,stk(lA),LDA ,
     $               stk(lB), LDB ,stk(lC),LDC ,TOL ,istk(lIWORK),
     $               stk(lDWORK), NDWORK ,INFO )
               else 
                  call SB04RD (SCHU ,ULA ,ULB ,N ,M ,stk(lA),LDA ,
     $               stk(lB), LDB ,stk(lC),LDC ,TOL ,istk(lIWORK),
     $               stk(lDWORK), NDWORK ,INFO )
               endif 
C
            endif 
         endif 
C
      else 
C
         if (FLAG (1).EQ.0)THEN 
            DICO ='C'
         else 
            DICO ='D'
         endif 
         if (FLAG (2).NE.1)THEN 
            FACT ='N'
         else 
            FACT ='F'
            call DLASET ('FULL',N ,N ,ZERO ,ONE ,stk(lU),LDU )
         endif 
C
         if (TRANS.EQ.0 )THEN 
            TRANA ='N'
         else 
            TRANA ='T'
         endif 
C
         if (TASK.EQ.2 )THEN 
            if (LHS.EQ.2 )THEN 
               JOB ='B'
            else 
               JOB ='X'
            endif 
C
            call SB03MD (DICO ,JOB ,FACT ,TRANA ,N ,stk(lA), LDA ,
     $         stk(lU), LDU ,stk(lC),LDC ,SCALE ,SEP ,FERR, stk(lWR), 
     $         stk(lWI), istk(lIWORK), stk(lDWORK), NDWORK ,INFO )
C
         else 
C
            call SB03OD (DICO ,FACT ,TRANA ,N ,P ,stk(lA),LDA ,stk(lU),
     $         LDU ,stk(lC),LDC ,SCALE ,stk(lWR),stk(lWI),stk(lDWORK),
     $         NDWORK ,INFO )
C
         endif 
      endif 
C
C form output
C
      PERTRB =(TASK.EQ.1 .AND.(INFO.EQ.N +M +1.OR. (FLAG (2)*FLAG (3)
     $     .EQ.1.AND. INFO.EQ.1 ))).OR.
     $     (TASK.EQ.2 .AND.INFO.EQ.N +1).OR.(TASK.EQ.3 .AND.INFO.EQ.1 )
      if (INFO.EQ.0 .OR.PERTRB )THEN 
         if (LHS.GE.1 )THEN 
            if (TASK.EQ.3 )THEN 
               if (TRANS.EQ.0 .AND.P.GT.N )call DLACPY ('UPPER',N ,N ,
     $            stk(lC),LDC ,stk(lC),LDA )
               if (N.GT.1 )call DLASET ('LOWER',N -1,N -1,ZERO ,ZERO ,
     $            stk(lC+2-1 +(1-1)*LDC),LDA )
            endif 
         endif 
C
         if (TASK.EQ.2 )THEN 
            if (LHS.GE.2 )THEN 
               if (N.EQ.ZERO ) SEP =ZERO 
               if(.NOT.CREATEVAR(NBVARS+1,'d',1,1,lSEP)) RETURN
               stk(lSEP)=SEP
               LHSVAR(2)=NBVARS
            endif 
         endif 
      endif 
C
C Error and warning handling.
C
      if (INFO.GT.0 )THEN 
         if (TASK.EQ.1 )THEN 
            if (NSCHUR.EQ.2 )THEN 
c     .        messages from SB04PD
               if(INFO.le.M+N) then
c     .           Failure when computing eigenvalues
                  call error(24)
               elseif(INFO.gt.M+N) then
c     .           Equation is singular
                  call error(19)
               endif
               return
            else 
               if (LSAME (SCHU ,'N')) THEN 
c     .           messages from SB04MD or SB04QD
                  if(INFO.le.M) then
c     .              Failure when computing eigenvalues
                     call error(24)
                  elseif(INFO.gt.M) then
c     .              Equation is singular
                     call error(19)
                  endif
               else 
c     .           Equation is singular
                  call error(19)                  
               endif 
               return
            endif 
         elseif (TASK.EQ.2 )THEN 
            if(INFO.le.N) then
c     .        Failure when computing eigenvalues
               call error(24)
            elseif(INFO.gt.N) then
c     .        Equation is singular
               call error(19) 
            endif
            return
         elseif (TASK.EQ.3 )THEN 
            if (INFO.eq.1) then
c     .        Equation is singular
               call error(19)
            elseif (INFO.eq.2.or.INFO.eq.3) then
               err=1
               if(lsame(DICO,'C')) then
c     .        Matrix is not stable (cont)
                  call error(269)
               else
                  call error(270)
               endif
            elseif (INFO.eq.4.or.INFO.eq.5) then
c     .        not a schur form
               call error(270)
            elseif (INFO.eq.6) then
c     .        Failure when computing eigenvalues
               call error(24)
            endif 
            return
         endif
      elseif(INFO.LT.0) then
c     .  message displayed by slicot or lapack
         buf=' '
         call error(999)
      endif 
C
      if ((INFO.EQ.0 .OR.PERTRB ).AND.SCALE.NE.ONE )THEN 
         if (TASK.LE.2 )THEN 
            TEMP =SCALE 
         else 
            TEMP =SCALE **2
         endif 
         WRITE (buf ,'
     $      ( '' WARNING: THE RIGHT HAND SIDES WERE '',''SCALED BY'',
     $       D13.6, '' TO AVOID OVERFLOW. '' )')TEMP 
      endif 
C
      if (INFO.NE.0 .AND..NOT.PERTRB )THEN 
         call error(1004)
      elseif (SCALE.NE.ONE )THEN 
         call msgs(1000,0)
      endif 
      if (PERTRB )THEN 
         WRITE (buf ,'
     $      ( '' WARNING: THE EQUATION IS (ALMOST) '',
     $      ''SINGULAR; PERTURBED VALUES HAVE BEEN USED. '' )')
          call msgs(1000,0)
      endif 
C
      RETURN 
C *** Last line of LINMEQ ***
      end 

      logical function iscomplex(pos)
      include '../stack.h'
      integer pos
c
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il=iadr(lstk(pos + top - rhs))
      if (istk(il).lt.0)  il=iadr(istk(il+2))
      
      if (istk(il+3).eq.1) then
         iscomplex=.true.
      else
         iscomplex=.false.
      endif
      return
      end
