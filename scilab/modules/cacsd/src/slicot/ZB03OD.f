
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
      INTEGER            I, ISMAX, ISMIN, LWKOPT, MN,
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

