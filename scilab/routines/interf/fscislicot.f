      subroutine intmb03od(fname)
      INCLUDE '../stack.h'
      character fname*(*)
      logical createvar, getrhsvar, checklhs, checkrhs
c     
      integer A,RCOND,RANK,SVAL,JPVT,TAU,R,Q,DWORK
      integer ptrA,ptrRCOND,ptrJPVT,ptrSVAL,ptrTAU,ptrRANK
      integer ptrDWORK,ptrR,ptrQ
      double precision RCONDdef,SLVMAX
      character*(1) JOBQR
      double precision ZERO
      parameter ( ZERO = 0.0D0)

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      minrhs=1
      maxrhs=3
      minlhs=1
      maxlhs=5
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      if(.not.checkrhs(fname,minrhs,maxrhs)) return


      JOBQR='Q'
      SLVMAX=0.0d0
      RCONDdef=0.0d0

      A=1
      RCOND=2
      JPVT=3
      SVAL=4
      TAU=5
      RANK=6
      R=7
      Q=8
      DWORK=9

      if(.not.getrhsvar(A,'d',M,N,ptrA)) return

      if(rhs.eq.1) then
c                 rankqr(A)
c     create RCOND=0.0,JPVT=[0,..,0]
         if(.not.createvar(RCOND,'d',1,1,ptrRCOND)) return
         stk(ptrRCOND)=RCONDdef
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.2) then
c                 rankqr(A,RCOND)
c     get RCOND, create JPVT=[0,..,0]
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.3) then
c                 rankqr(A,RCOND,JPVT)
c     get  RCOND, JPVT
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.getrhsvar(JPVT,'i',mJ,nJ,ptrJPVT)) return
         if(mJ*nJ.ne.N) then
            buf=fname//': JPVT must have same column dim. as A'
            call error(998)
            return
         endif
      endif

c    Creating  SVAL,TAU,RANK,R,Q,DWORK
      
      if(.not.createvar(SVAL,'d',1,3,ptrSVAL)) return
      if(.not.createvar(TAU,'d',1,min(M,N),ptrTAU)) return
      if(.not.createvar(RANK,'i',1,1,ptrRANK)) return
      if(.not.createvar(R,'d',M,N,ptrR)) return
      if(.not.createvar(Q,'d',M,M,ptrQ)) return

      LDWORKMIN=max(1,3*N)
      LDWORK=maxvol(DWORK,'d')
      if(LDWORK.le.LDWORKMIN) then
         buf=fname//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(DWORK,'d',1,LDWORK,ptrDWORK)) return

      LDA=max(1,M)
      call MB03OD(JOBQR, M, N, stk(ptrA), LDA, istk(ptrJPVT),
     &  stk(ptrRCOND), SLVMAX, stk(ptrTAU), istk(ptrRANK),
     &  stk(ptrSVAL), stk(ptrDWORK), INFO)

      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     Save R (=current A)
      call dcopy(M*N,stk(ptrA),1,stk(ptrR),1)

c     Set to zero lower trapezoidal part of R:
      IRANK=istk(ptrRANK)
      IR=IRANK-M
      do 10 K=1,N
         II=II+1
         NB=M-IRANK
         IR=IR+M
         NB1=NB
         IR1=IR
         if(K.lt.IRANK) then
           IR1=IR-(IRANK-K)
           NB1=NB+(IRANK-K)
         endif
         call dcopy(NB1,ZERO,0,stk(ptrR+IR1),1)
 10   continue

c     Make Q
      if(M.le.N) then
c     A is fat
      CALL DORGQR( M, M, M, stk(ptrA), LDA, stk(ptrTAU), 
     &   stk(ptrDWORK), LDWORK, INFO )
      call dcopy(M*M,stk(ptrA),1,stk(ptrQ),1)
      else
c     A is tall => Q=[A,0]
      call dcopy(M*N,stk(ptrA),1,stk(ptrQ),1)
      call dcopy(M*(M-N),0.d0,0,stk(ptrQ+M*N),1)
      CALL DORGQR( M, M, min(M,N), stk(ptrQ), LDA, stk(ptrTAU), 
     &   stk(ptrDWORK), LDWORK, INFO )
      endif

      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      lhsvar(1)=Q
      lhsvar(2)=R
      lhsvar(3)=JPVT
      lhsvar(4)=RANK
      lhsvar(5)=SVAL

      end

      subroutine intzb03od(fname)
      INCLUDE '../stack.h'
      character fname*(*)
      logical createvar, getrhsvar, checklhs, checkrhs
c     
      integer A,RCOND,RANK,SVAL,JPVT,TAU,R,Q,RWORK,DWORK
      integer ptrA,ptrRCOND,ptrJPVT,ptrSVAL,ptrTAU,ptrRANK
      integer ptrDWORK,ptrR,ptrQ,ptrRWORK
      double precision RCONDdef,SLVMAX
      character*(1) JOBQR
      complex*16 ZERO
      parameter( ZERO=(0.0D0,0.0D0) )

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      minrhs=1
      maxrhs=3
      minlhs=1
      maxlhs=5
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      if(.not.checkrhs(fname,minrhs,maxrhs)) return

      JOBQR='Q'
      RCONDdef=0.0d0
      SLVMAX=0.0d0

      A=1
      RCOND=2
      JPVT=3
      SVAL=4
      TAU=5
      RANK=6
      R=7
      Q=8
      RWORK=9
      DWORK=10
 
      if(.not.getrhsvar(A,'z',M,N,ptrA)) return

      if(rhs.eq.1) then
c                 rankqr(A)
c     create RCOND=0.0,JPVT=[0,..,0]
         if(.not.createvar(RCOND,'d',1,1,ptrRCOND)) return
         stk(ptrRCOND)=RCONDdef
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.2) then
c                 rankqr(A,RCOND)
c     get RCOND, create JPVT=[0,..,0]
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return 
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.3) then
c                 rankqr(A,RCOND,JPVT)
c     get  RCOND, JPVT
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.getrhsvar(JPVT,'i',mJ,nJ,ptrJPVT)) return
         if(mJ*nJ.ne.N) then
            buf=fname//': JPVT must have same column dim. as A'
            call error(998)
            return
         endif
      endif



c    Creating  SVAL,RANK,R,Q,RWORK,DWORK
      
      if(.not.createvar(SVAL,'d',1,3,ptrSVAL)) return
      if(.not.createvar(TAU,'z',1,min(M,N),ptrTAU)) return
      if(.not.createvar(RANK,'i',1,1,ptrRANK)) return
      if(.not.createvar(R,'z',M,N,ptrR)) return
      if(.not.createvar(Q,'z',M,M,ptrQ)) return
      if(.not.createvar(RWORK,'d',1,2*N,ptrRWORK)) return

      LDWORKMIN=max(2*min(M,N),N+1)
      LDWORK=maxvol(DWORK,'z')
      if(LDWORK.le.LDWORKMIN) then
         buf=fname//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(DWORK,'z',1,LDWORK,ptrDWORK)) return

      LDA=max(1,M)
      call ZB03OD(JOBQR, M, N, zstk(ptrA), LDA, istk(ptrJPVT),
     &  stk(ptrRCOND), SLVMAX, zstk(ptrTAU), istk(ptrRANK),
     &  stk(ptrSVAL), zstk(ptrDWORK), LDWORK, stk(ptrRWORK), INFO)
      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     Save R (=current A)
      call zcopy(M*N,zstk(ptrA),1,zstk(ptrR),1)
c     Set to zero lower trapezoidal part of R:
      IRANK=istk(ptrRANK)
      IR=IRANK-M
      do 10 K=1,N
         II=II+1
         NB=M-IRANK
         IR=IR+M
         NB1=NB
         IR1=IR
         if(K.lt.IRANK) then
           IR1=IR-(IRANK-K)
           NB1=NB+(IRANK-K)
         endif
         call zcopy(NB1,ZERO,0,zstk(ptrR+IR1),1)
 10   continue

c     Make Q
      if(M.le.N) then
c     A is fat
      CALL ZUNGQR( M, M, M, zstk(ptrA), LDA, zstk(ptrTAU), 
     &   zstk(ptrDWORK), LDWORK, INFO )
      call zcopy(M*M,zstk(ptrA),1,zstk(ptrQ),1)
      else
c     A is tall => Q=[A,0]
      call zcopy(M*N,zstk(ptrA),1,zstk(ptrQ),1)
      call zcopy(M*(M-N),ZERO,0,zstk(ptrQ+M*N),1)
      CALL ZUNGQR( M, M, min(M,N), zstk(ptrQ), LDA, zstk(ptrTAU), 
     &   zstk(ptrDWORK), LDWORK, INFO )
      endif

      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      lhsvar(1)=Q
      lhsvar(2)=R
      lhsvar(3)=JPVT
      lhsvar(4)=RANK
      lhsvar(5)=SVAL

      end


      subroutine intmucomp(fname)

c     [bound,D,G] = mucomp(Z,K,T)
c     [bound,D] = mucomp(Z,K,T)
c     bound = mucomp(Z,K,T)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)

       minrhs=3
       maxrhs=3
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lZ)) return
       if(M.ne.N) then
         buf='mucomp'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           lhsvar(1) = 2
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           if(.not.createvar(3,'d', N, 1, lD)) return
           lhsvar(1)=2
           lhsvar(2)=3
           return
         else if(lhs.eq.3) then
           if(.not.createvar(2,'d', N, 1, lBOUND)) return
           if(.not.createvar(3,'d', N, 1, lD)) return
           if(.not.createvar(4,'d', N, 1, lG)) return 
           lhsvar(1)=2
           lhsvar(2)=3
           lhsvar(3)=4
           return
         endif
       endif
       if(.not.getrhsvar(2,'i', M1, N1, lK)) return
       if(.not.getrhsvar(3,'i', M2, N2, lT)) return
       if(M1*N1.ne.M2*N2) then
         buf='mucomp'//': K and T must have equal lengths'
         call error(998)
         return
       endif
       M = M1*N1
       if(.not.createvar(4,'d', 1, 1, lBOUND)) return
       if(.not.createvar(5,'d', N, 1, lD)) return
       if(.not.createvar(6,'d', N, 1, lG)) return 
       if(.not.createvar(7,'d', 2*N-1, 1, lX)) return
       if(.not.createvar(8,'i', 4*N-2, 1, lIWORK)) return 
       LRWRK = 2*N*N*N + 9*N*N +  44*N - 11
       if(.not.createvar(9,'d', LRWRK, 1, lRWORK)) return
       LZWRKMIN  = 6*N*N*N + 12*N*N + 12*N - 3
       LZWRK=maxvol(10,'z')
       if(LZWRK.le.LZWRKMIN) then
         buf='mucomp'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(10,'z',1,LZWRK,lZWORK)) return 
    
       call AB13MD( 'N', N, zstk(lZ), N, M, istk(lK), istk(lT),
     $    stk(lX), stk(lBOUND), stk(lD), stk(lG), istk(lIWORK),
     $    stk(lRWORK), LRWRK, zstk(lZWORK), LZWRK, INFO )
c       SUBROUTINE AB13MD( FACT, N, Z, N, M, NBLOCK, ITYPE, X,
c     $    BOUND, D, G, IWORK, DWORK, LDWORK, ZWORK, LZWORK,
c     $    INFO )
      if(info.ne.0) then
        call errorinfo("mucomp",info)
        return
      endif
    
      if(lhs.eq.1) then
        lhsvar(1) = 4
      else if(lhs.eq.2) then
        lhsvar(1)=4
        lhsvar(2)=5
      else if(lhs.eq.3) then
        lhsvar(1)=4
        lhsvar(2)=5
        lhsvar(3)=6
      endif
c
       end



      SUBROUTINE ZB03OD( JOBQR, M, N, A, LDA, JPVT, RCOND, SVLMAX, TAU,
     $                   RANK, SVAL, WORK, LWORK, RWORK, INFO )
*
*     .. Scalar Arguments ..
      CHARACTER*1        JOBQR
      INTEGER            INFO, LDA, LWORK, M, N,  RANK
      DOUBLE PRECISION   RCOND, SVLMAX
*     ..
*     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   SVAL(3), RWORK( * )
      COMPLEX*16         A( LDA, * ), TAU( * ), WORK( * )
*     ..
*
*  Purpose
*  =======
*
C
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C
C     PURPOSE
C
C     To compute (optionally) a rank-revealing QR factorization of a 
C     real general M-by-N matrix  A,  which may be rank-deficient,
C     and estimate its effective rank using incremental condition 
C     estimation.
C
C     The routine uses a QR factorization with column pivoting:
C        A * P = Q * R,  where  R = [ R11 R12 ],
C                                   [  0  R22 ]
C     with R11 defined as the largest leading submatrix whose estimated
C     condition number is less than 1/RCOND.  The order of R11, RANK,
C     is the effective rank of A.
C
C     ZB03OD  does not perform any scaling of the matrix A.
*
*  Arguments
*  =========
*
C    Mode Parameters
C
C    JOBQR   CHARACTER*1
C            = 'Q': Perform a QR factorization with column pivoting;
C            = 'N': Do not perform the QR factorization (but ssumes
C                   that it has been done outside).
*
*  M       (input) INTEGER
*          The number of rows of the matrix A.  M >= 0.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A.  N >= 0.
*
*  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
*          On entry, the M-by-N matrix A.
*          On exit, A has been overwritten by details of its
*          complete orthogonal factorization.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1,M).
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B. LDB >= max(1,M,N).
*
*  JPVT    (input/output) INTEGER array, dimension (N)
*          On entry, if JPVT(i) .ne. 0, the i-th column of A is permuted
*          to the front of AP, otherwise column i is a free column.
*          On exit, if JPVT(i) = k, then the i-th column of A*P
*          was the k-th column of A.
*
*  RCOND   (input) DOUBLE PRECISION
*          RCOND is used to determine the effective rank of A, which
*          is defined as the order of the largest leading triangular
*          submatrix R11 in the QR factorization with pivoting of A,
*          whose estimated condition number < 1/RCOND.
*
C            
C     TAU     (output) COMPLEX*16 array, dimension ( MIN( M, N ) )
C             On exit with JOBQR = 'Q', the leading min(M,N) elements of
C             TAU contain the scalar factors of the elementary 
C             reflectors.
C             Array TAU is not referenced when JOBQR = 'N'.
C            
*  RANK    (output) INTEGER
*          The effective rank of A, i.e., the order of the submatrix
*          R11.  This is the same as the order of the submatrix T11
*          in the complete orthogonal factorization of A.
*
C     SVAL    (output) DOUBLE PRECISION array, dimension ( 3 )
C             The estimates of some of the singular values of the 
C             triangular factor R:
C             SVAL(1): largest singular value of R(1:RANK,1:RANK);
C             SVAL(2): smallest singular value of R(1:RANK,1:RANK);
C             SVAL(3): smallest singular value of R(1:RANK+1,1:RANK+1),
C                      if RANK < MIN( M, N ), or of R(1:RANK,1:RANK),
C                      otherwise.
C             If the triangular factorization is a rank-revealing one
C             (which will be the case if the leading columns were well-
C             conditioned), then SVAL(1) will also be an estimate for
C             the largest singular value of A, and SVAL(2) and SVAL(3)
C             will be estimates for the RANK-th and (RANK+1)-st singular
C             values of A, respectively.
C             By examining these values, one can confirm that the rank
C             is well defined with respect to the chosen value of RCOND.
C             The ratio SVAL(1)/SVAL(2) is an estimate of the condition
C             number of R(1:RANK,1:RANK).
C
*  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*
*          If JOBQR = 'Q':
*             The unblocked strategy requires that:
*               LWORK >= MAX( 2*MN, N+1 )
*             where MN = min(M,N).
*             The block algorithm requires that:
*               LWORK >= MAX( 2*MN, NB*(N+1) )
*             where NB is an upper bound on the blocksize returned
*             by ILAENV for the routines ZGEQP3 and ZUNMQR.
*
*          LDWORK = max( 1, 2*min( M, N ) ), if JOBQR = 'N'.        
*
*          If LWORK = -1, then a workspace query is assumed; the routine
*          only calculates the optimal size of the WORK array, returns
*          this value as the first entry of the WORK array, and no error
*          message related to LWORK is issued by XERBLA.
*
*  RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
C     METHOD
C
C     The routine computes or uses a QR factorization with column 
C     pivoting of A,  A * P = Q * R,  with  R  defined above, and then
C     finds the largest leading submatrix whose estimated condition
C     number is less than 1/RCOND, taking the possible positive value of
C     SVLMAX into account.  This is performed using the LAPACK
C     incremental condition estimation scheme and a slightly modified
C     rank decision test.
C
C     CONTRIBUTOR
C
C     Complex version of MB03OD
C
*
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            IMAX, IMIN
      PARAMETER          ( IMAX = 1, IMIN = 2 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      COMPLEX*16         CZERO, CONE
      PARAMETER          ( CZERO = ( 0.0D+0, 0.0D+0 ),
     $                   CONE = ( 1.0D+0, 0.0D+0 ) )
*     ..
*     .. Local Scalars ..
      LOGICAL            LJOBQR,  LQUERY
      INTEGER            I, ISMAX, ISMIN, J, LWKOPT, MN,
     $                   NB, NB1, NB2
      DOUBLE PRECISION   SMAX, SMAXPR, SMIN, SMINPR
      COMPLEX*16         C1, C2, S1, S2
*     ..
*     .. External Subroutines ..
      EXTERNAL           XERBLA, ZGEQP3, ZLAIC1
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      EXTERNAL           ILAENV, LSAME
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, DCMPLX, MAX, MIN
*     ..
*     .. Executable Statements ..
*
      LJOBQR = LSAME( JOBQR, 'Q' )
      MN = MIN( M, N )
      ISMIN = 1
      ISMAX = MN + 1
*
*     Test the input arguments.
*
      INFO = 0
      NB1 = ILAENV( 1, 'ZGEQRF', ' ', M, N, -1, -1 )
      NB2 = ILAENV( 1, 'ZUNMQR', ' ', M, N, NRHS, -1 )
      NB = MAX( NB1, NB2 )
      LWKOPT = MAX( 1, 2*N+NB*( N+1 ) )
      WORK( 1 ) = DCMPLX( LWKOPT )
      LQUERY = ( LWORK.EQ.-1 )
C
      IF( .NOT.LJOBQR .AND. .NOT.LSAME( JOBQR, 'N' ) ) THEN
         INFO = -1
      ELSE IF( M.LT.0 ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -5
      ELSE IF( RCOND.LT.ZERO ) THEN
         INFO = -7
      ELSE IF( SVLMAX.LT.ZERO ) THEN
         INFO = -8
      ELSE IF( LWORK.LT.( MAX( 2*MN, N+1 ) ) .AND. .NOT.
     $         LQUERY ) THEN
         INFO = -13
      END IF
*
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'ZB03OD', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( MN.EQ.0 ) THEN
         SVAL( 1 ) = ZERO
         SVAL( 2 ) = ZERO
         SVAL( 3 ) = ZERO
         RANK = 0
         RETURN
      END IF
C
      IF( LJOBQR ) THEN
*
*        Compute QR factorization with column pivoting of A:
*           A * P = Q * R
*
         CALL ZGEQP3( M, N, A, LDA, JPVT, TAU, WORK, LWORK,
     $                RWORK, INFO )    
*
*        complex workspace: MN+NB*(N+1). real workspace 2*N.
*        Details of Householder rotations stored in WORK(1:MN).
      END IF
*
*     Determine RANK using incremental condition estimation
*
      WORK( ISMIN ) = CONE
      WORK( ISMAX ) = CONE
      SMAX = ABS( A( 1, 1 ) )
      SMIN = SMAX
      IF( SMAX.EQ.ZERO .OR. SVLMAX*RCOND.GT.SMAX) THEN
         RANK = 0
         SVAL( 1 ) = SMAX
         SVAL( 2 ) = ZERO
         SVAL( 3 ) = ZERO
      ELSE
         RANK = 1
         SMINPR = SMIN
*
   10    CONTINUE
         IF( RANK.LT.MN ) THEN
            I = RANK + 1
            CALL ZLAIC1( IMIN, RANK, WORK( ISMIN ), SMIN, A( 1, I ),
     $                   A( I, I ), SMINPR, S1, C1 )
            CALL ZLAIC1( IMAX, RANK, WORK( ISMAX ), SMAX, A( 1, I ),
     $                   A( I, I ), SMAXPR, S2, C2 )
*
            IF( SVLMAX*RCOND.LE.SMAXPR ) THEN
               IF( SVLMAX*RCOND.LE.SMINPR ) THEN
                  IF( SMAXPR*RCOND.LE.SMINPR ) THEN
                     DO 20 I = 1, RANK
                        WORK( ISMIN+I-1 ) = S1*WORK( ISMIN+I-1 )
                        WORK( ISMAX+I-1 ) = S2*WORK( ISMAX+I-1 )
   20                CONTINUE
                     WORK( ISMIN+RANK ) = C1
                     WORK( ISMAX+RANK ) = C2
                     SMIN = SMINPR
                     SMAX = SMAXPR
                     RANK = RANK + 1
                     GO TO 10
                  END IF
               END IF
            END IF
         END IF
         SVAL( 1 ) = SMAX
         SVAL( 2 ) = SMIN
         SVAL( 3 ) = SMINPR
      END IF
      WORK( 1 ) = DCMPLX( LWKOPT )
C
      RETURN
C *** Last line of ZB03OD ***
      END


      subroutine intricc2(fname)

c     [X,RCOND,FERR]=ricc(A,C,D,'type','method')
c     [X,RCOND]=ricc(A,C,D,'type','method')
c     X = riccsl(A,C,D,'type','method')
c     'type' is 'continuous' or 'discrete'
c     'method' is 'schr' or 'sign' for continuous-time systems
c             and 'schr' or 'invf' for discrete-tyme systems 

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4 TYPE, METHOD 
      logical WANTC, WANTD, WSCHUR, WSIGN, WINVF
 
      minrhs=4
      maxrhs=5
      minlhs=1
      maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA.ne.NA) then
         buf='ricc'//': the matrix A must be square'
         call error(998)
         return
       endif       
       if(.not.getrhsvar(2,'d', MD, ND, lD)) return
       if(MD.ne.ND) then
         buf='ricc'//': the matrix D must be square'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(MC.ne.NC) then
         buf='ricc'//': the matrix C must be square'
         call error(998)
         return
       endif
       if(MA.ne.MC .or. MC.ne.MD .or. MA.ne.MD) then
         buf='ricc'//
     $     ': the matrices A, C and D must have the same order'
         call error(998)
         return

       endif
       N = MA

       if(.not.getrhsvar(4,'c', M1, N1, lTYPE)) return
       TYPE(1:4) = cstk(lTYPE:lTYPE+4)
       WANTC = (TYPE(1:4).eq.'cont' .or. TYPE(1:4).eq.'CONT')
       WANTD = (TYPE(1:4).eq.'disc' .or. TYPE(1:4).eq.'DISC')
       if(.not.WANTC .and. .not.WANTD) then
         buf='ricc'//': type must be continuous or discrete'
         call error(998)
         return
       endif
       
       k = 5
       WSCHUR = .TRUE.
       if(rhs.eq.5) then
         if(.not.getrhsvar(5,'c', M1, N1, lMETHOD)) return
         METHOD(1:4) = cstk(lMETHOD:lMETHOD+4)
         if(WANTC) then
           WSCHUR = (METHOD(1:4).eq.'schr' .or. METHOD(1:4).eq.'SCHR')
           WSIGN = (METHOD(1:4).eq.'sign' .or. METHOD(1:4).eq.'SIGN')
           if(.not.WSCHUR .and. .not.WSIGN) then
             buf='ricc'//': method must be schur or sign'
             call error(998)
             return
           endif
         else
           WSCHUR = (METHOD(1:4).eq.'schr' .or. METHOD(1:4).eq.'SCHR')
           WINVF = (METHOD(1:4).eq.'invf' .or. METHOD(1:4).eq.'INVF')
           if(.not.WSCHUR .and. .not.WINVF) then
             buf='ricc'//': method must be schur or invf'
             call error(998)
             return
           endif
         endif
         k = 6
       endif
       if(.not.createvar(k,'d', N, N, lX)) return      
       if(.not.createvar(k+1,'d', N, 1, lWR)) return
       if(.not.createvar(k+2,'d', N, 1, lWI)) return
       if(.not.createvar(k+3,'d', 1, 1, lRCOND)) return
       if(.not.createvar(k+4,'d', 1, 1, lFERR)) return 
       if(.not.createvar(k+5,'i', 1, max(2*N,N*N), lIWORK)) return       
       if(.not.createvar(k+6,'i', 1, 2*N, lBWORK)) return
       if(WANTC) then
         if(WSCHUR) then
           LWORKMIN = 9*N*N + 4*N + max(1,6*N)
         else if(WSIGN) then
           LWORKMIN = 9*N*N + 7*N + 1
         endif
       else
         if(WSCHUR) then
           LWORKMIN = 12*N*N + 22*N + max(16,4*N)
         else if(WINVF) then
           LWORKMIN = 28*N*N + 2*N + max(1,2*N) 
         endif
       endif
       LWORK=maxvol(k+7,'d')
       if(LWORK.le.LWORKMIN) then
         buf='ricc'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(k+7,'d',1,LWORK,lDWORK)) return

       if(WANTC) then
         if(WSCHUR) then 
            call RICCSL( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD), 
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND), 
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK),
     $        istk(lBWORK), INFO )
c            SUBROUTINE RICCSL( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
c     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK, INFO )
         else if(WSIGN) then
            call RICCMS( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK), INFO )
c            SUBROUTINE RICCMS( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
c     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
         endif
       else
         if(WSCHUR) then
            call RICDSL( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK),
     $        istk(lBWORK), INFO )
c            SUBROUTINE RICDSL( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
C     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK, INFO )
         else if (WINVF) then
            call RICDMF( 'N', N, stk(lA), N, 'U', stk(lC), N, stk(lD),
     $        N, stk(lX), N, stk(lWR), stk(lWI), stk(lRCOND),
     $        stk(lFERR), stk(lDWORK), LWORK, istk(lIWORK), INFO )
c            SUBROUTINE RICDMF( TRANS, N, A, LDA, C, LDC, D, LDD, X, LDX,
C     $        WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
         endif
      endif
      if(info.ne.0) then
        call errorinfo("ricc",info)
        return
      endif
   
    
      if(lhs.eq.1) then
        lhsvar(1)=k
      else if(lhs.eq.2) then
        lhsvar(1)=k
        lhsvar(2)=k+3
      else if(lhs.eq.3) then 
          lhsvar(1)=k
          lhsvar(2)=k+3
          lhsvar(3)=k+4
      endif
c
       end

      subroutine inthinf(fname)

c     [Ak,Bk,Ck,Dk,RCOND]=hinf(A,B,C,D,ncon,nmeas,gamma)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision GAMMA, TOL, EPS
      integer N, M, R, Q
      double precision dlamch
      external dlamch
c
       minrhs=7
       maxrhs=7
       minlhs=4
       maxlhs=5
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA .ne. NA) then
         buf='hinf'//': A must be a square matrix'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(MA .ne. MB) then
         buf='hinf'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(NA .ne. NC) then
         buf='hinf'//': A and C must have equal number of columns'
         call error(998)
         return
       endif
       if(.not.getrhsvar(4,'d', MD, ND, lD)) return
       if(NB .ne. ND) then
         buf='hinf'//': B and D must have equal number of columns'
         call error(998)
         return
       endif
       if(MC .ne. MD) then
         buf='hinf'//': C and D must have equal number of rows'
         call error(998)
         return
       endif
       N = MA
       M = NB
       R = MC
       if(N.eq.0 .or. M.eq.0 .or. R.eq.0) then
         if(.not.createvar(5,'d', 0, 0, lAK)) return
         if(.not.createvar(6,'d', 0, 0, lBK)) return
         if(.not.createvar(7,'d', 0, 0, lCK)) return
         if(.not.createvar(8,'d', 0, 0, lDK)) return
         if(.not.createvar(9,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 5
         lhsvar(2) = 6
         lhsvar(3) = 7
         lhsvar(4) = 8
         lhsvar(5) = 9
         return
       endif
       if(.not.getrhsvar(5,'i', M1, N1, lNCON)) return
       if(M1.ne.1 .or. N1.ne.1 ) then
         buf='hinf'//': NCON must be a scalar'
         call error(998)
         return
       endif
       NCON = istk(lNCON)

       if(.not.getrhsvar(6,'i', M2, N2, lNMEAS)) return
       if(M2.ne.1 .or. N2.ne.1 ) then
         buf='hinf'//': NMEAS must be a scalar'
         call error(998)
         return
       endif
       NMEAS = istk(lNMEAS)

       if(.not.getrhsvar(7,'d', M3, N3, lGAMMA)) return
       if(M3.ne.1 .or. N3.ne.1 ) then
         buf='hinf'//': GAMMA must be a scalar'
         call error(998)
         return
       endif
       GAMMA = stk(lGAMMA)
       EPS = dlamch('eps')
       TOL = sqrt(EPS)
       
       if(.not.createvar(8,'d', N, N, lAK)) return
       if(.not.createvar(9,'d', N, NMEAS, lBK)) return
       if(.not.createvar(10,'d', NCON, N, lCK)) return
       if(.not.createvar(11,'d', NCON, NMEAS, lDK)) return
       if(.not.createvar(12,'d', 4, 1, lRCOND)) return
       LINTWORK = max(2*max(N,M-NCON,R-NMEAS,NCON),N*N)

       if(.not.createvar(13,'i', LINTWORK, 1, lIWORK)) return
       if(.not.createvar(14,'i', 2*N, 1, lBWORK)) return

       Q = MAX( M - NCON, NCON, R - NMEAS, NMEAS )
       LWORKMIN = 2*Q*( 3*Q + 2*N ) + 
     $          MAX( 1, ( N + Q )*( N + Q + 6 ), 
     $               Q*( Q + MAX( N, Q, 5 ) + 1 ), 2*N*( N + 2*Q ) +
     $               MAX( 1, 4*Q*Q +
     $                    MAX( 2*Q, 3*N*N + 
     $                         MAX( 2*N*Q, 10*N*N + 12*N + 5 ) ),
     $                         Q*( 3*N + 3*Q + 
     $                             MAX( 2*N, 4*Q + max( N, Q ) ) ) ) )
       LWORK=maxvol(15,'d')
       if(LWORK.le.LWORKMIN) then
         buf='hinf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(15,'d',1,LWORK,lDWORK)) return
      
      CALL SB10FD( N, M, R, NCON, NMEAS, GAMMA, stk(lA), N,
     $             stk(lB), N, stk(lC), R, stk(lD), R, stk(lAK),
     $             N, stk(lBK), N, stk(lCK), NCON, stk(lDK),
     $             NCON, stk(lRCOND), TOL, istk(lIWORK),
     $             stk(lDWORK), LDWORK, istk(lBWORK), INFO )
      if(info.ne.0) then
        call errorinfo("hinf",info)
        return
      endif      

      lhsvar(1) = 8
      lhsvar(2) = 9
      lhsvar(3) = 10
      lhsvar(4) = 11
      if(lhs.eq.5) lhsvar(5) = 12

c
       end


      subroutine intdhinf(fname)

c     [Ak,Bk,Ck,Dk,RCOND]=dhinf(A,B,C,D,ncon,nmeas,gamma)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision GAMMA, TOL, EPS
      integer N, M, R, Q
      double precision dlamch
      external dlamch
c
       minrhs=7
       maxrhs=7
       minlhs=4
       maxlhs=5
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA .ne. NA) then
         buf='dhinf'//': A must be a square matrix'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(MA .ne. MB) then
         buf='dhinf'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       if(.not.getrhsvar(3,'d', MC, NC, lC)) return
       if(NA .ne. NC) then
         buf='dhinf'//': A and C must have equal number of columns'
         call error(998)
         return
       endif
       if(.not.getrhsvar(4,'d', MD, ND, lD)) return
       if(NB .ne. ND) then
         buf='dhinf'//': B and D must have equal number of columns'
         call error(998)
         return
       endif
       if(MC .ne. MD) then
         buf='dhinf'//': C and D must have equal number of rows'
         call error(998)
         return
       endif
       N = MA
       M = NB
       R = MC
       if(N.eq.0 .or. M.eq.0 .or. R.eq.0) then
         if(.not.createvar(5,'d', 0, 0, lAK)) return
         if(.not.createvar(6,'d', 0, 0, lBK)) return
         if(.not.createvar(7,'d', 0, 0, lCK)) return
         if(.not.createvar(8,'d', 0, 0, lDK)) return
         if(.not.createvar(9,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 5
         lhsvar(2) = 6
         lhsvar(3) = 7
         lhsvar(4) = 8
         lhsvar(5) = 9
         return
       endif
       if(.not.getrhsvar(5,'i', M1, N1, lNCON)) return
       if(M1.ne.1 .or. N1.ne.1 ) then
         buf='dhinf'//': NCON must be a scalar'
         call error(998)
         return
       endif
       NCON = istk(lNCON)

       if(.not.getrhsvar(6,'i', M2, N2, lNMEAS)) return
       if(M2.ne.1 .or. N2.ne.1 ) then
         buf='dhinf'//': NMEAS must be a scalar'
         call error(998)
         return
       endif
       NMEAS = istk(lNMEAS)

       if(.not.getrhsvar(7,'d', M3, N3, lGAMMA)) return
       if(M3.ne.1 .or. N3.ne.1 ) then
         buf='dhinf'//': GAMMA must be a scalar'
         call error(998)
         return
       endif
       GAMMA = stk(lGAMMA)
       EPS = dlamch('eps')
       TOL = sqrt(EPS)
       
       if(.not.createvar(8,'d', N, N, lAK)) return
       if(.not.createvar(9,'d', N, NMEAS, lBK)) return
       if(.not.createvar(10,'d', NCON, N, lCK)) return
       if(.not.createvar(11,'d', NCON, NMEAS, lDK)) return
       if(.not.createvar(12,'d', N, N, lX)) return
       if(.not.createvar(13,'d', N, N, lZ)) return
       if(.not.createvar(14,'d', 8, 1, lRCOND)) return
       LINTWORK = max(2*max(NCON,N),M,NCON+NMEAS,N*N)
       if(.not.createvar(15,'i', LINTWORK, 1, lIWORK)) return
       if(.not.createvar(16,'i', 2*N, 1, lBWORK)) return

       Q = MAX( M - NCON, NCON, R - NMEAS, NMEAS )
       LWORKMIN = max((N+Q)*(N+Q+6),13*N*N + M*M + 2*Q*Q + N*(M+Q) +
     $            max(M*(M+7*N),2*Q*(8*N+M+2*Q)) + 6*N + 
     $            max(14*N+23,16*N,2*N+max(M,2*Q),3*max(M,2*Q)))
       LWORK=maxvol(17,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dhinf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(17,'d',1,LWORK,lDWORK)) return
      
      CALL SB10DD( N, M, R, NCON, NMEAS, GAMMA, stk(lA), N,
     $             stk(lB), N, stk(lC), R, stk(lD), R, stk(lAK),
     $             N, stk(lBK), N, stk(lCK), NCON, stk(lDK),
     $             NCON, stk(lX), N, stk(lZ), N, stk(lRCOND), TOL,
     $             istk(lIWORK), stk(lDWORK), LDWORK, istk(lBWORK),
     $             INFO )
      if(info.ne.0) then
        call errorinfo("dhinf",info)
        return
      endif      

      lhsvar(1) = 8
      lhsvar(2) = 9
      lhsvar(3) = 10
      lhsvar(4) = 11
      if(lhs.eq.5) lhsvar(5) = 14

c
       end





