      SUBROUTINE MB03OD( JOBQR, M, N, A, LDA, JPVT, RCOND, SVLMAX, TAU, 
     $                   RANK, SVAL, DWORK, INFO )
C
C     RELEASE 3.0, WGS COPYRIGHT 1997.
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
C     MB03OD  does not perform any scaling of the matrix A.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     JOBQR   CHARACTER*1
C             = 'Q':  Perform a QR factorization with column pivoting;
C             = 'N':  Do not perform the QR factorization (but assume
C                     that it has been done outside).  
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrix A.  M >= 0.
C            
C     N       (input) INTEGER
C             The number of columns of the matrix A.  N >= 0.
C            
C     A       (input/output) DOUBLE PRECISION array, dimension 
C             ( LDA, N )
C             On entry with JOBQR = 'Q', the leading M by N part of this
C             array must contain the given matrix A.
C             On exit with JOBQR = 'Q', the leading min(M,N) by N upper
C             triangular part of A contains the triangular factor R, 
C             and the elements below the diagonal, with the array TAU, 
C             represent the orthogonal matrix Q as a product of 
C             min(M,N) elementary reflectors.
C             On entry and on exit with JOBQR = 'N', the leading
C             min(M,N) by N upper triangular part of A contains the
C             triangular factor R, as determined by the QR factorization
C             with pivoting.  The elements below the diagonal of A are 
C             not referenced.
C            
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C            
C     JPVT    (input/output) INTEGER array, dimension ( N )
C             On entry with JOBQR = 'Q', if JPVT(i) <> 0, the i-th
C             column of A is an initial column, otherwise it is a free 
C             column. Before the QR factorization of A, all initial
C             columns are permuted to the leading positions; only the
C             remaining free columns are moved as a result of column
C             pivoting during the factorization.  For rank determination
C             it is preferable that all columns be free.
C             On exit with JOBQR = 'Q', if JPVT(i) = k, then the i-th
C             column of A*P was the k-th column of A.
C             Array JPVT is not referenced when JOBQR = 'N'.
C            
C     RCOND   (input) DOUBLE PRECISION
C             RCOND is used to determine the effective rank of A, which
C             is defined as the order of the largest leading triangular
C             submatrix R11 in the QR factorization with pivoting of A,
C             whose estimated condition number is less than 1/RCOND.
C             RCOND >= 0.
C             NOTE that when SVLMAX > 0, the estimated rank could be
C             less than that defined above (see SVLMAX).
C            
C     SVLMAX  (input) DOUBLE PRECISION
C             If A is a submatrix of another matrix B, and the rank 
C             decision should be related to that matrix, then SVLMAX
C             should be an estimate of the largest singular value of B 
C             (for instance, the Frobenius norm of B).  If this is not 
C             the case, the input value SVLMAX = 0 should work.
C             SVLMAX >= 0.
C            
C     TAU     (output) DOUBLE PRECISION array, dimension ( MIN( M, N ) )
C             On exit with JOBQR = 'Q', the leading min(M,N) elements of
C             TAU contain the scalar factors of the elementary 
C             reflectors.
C             Array TAU is not referenced when JOBQR = 'N'.
C            
C     RANK    (output) INTEGER
C             The effective (estimated) rank of A, i.e. the order of 
C             the submatrix R11.
C            
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
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension ( LDWORK )
C             where LDWORK = max( 1, 3*N ), if JOBQR = 'Q'; 
C                   LDWORK = max( 1, 2*min( M, N ) ), if JOBQR = 'N'.
C            
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value.
C
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
C     V. Sima, Katholieke Univ. Leuven, Belgium, Nov. 1996.
C
C    ******************************************************************
C
C     .. Parameters ..
      INTEGER            IMAX, IMIN
      PARAMETER          ( IMAX = 1, IMIN = 2 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER*1        JOBQR
      INTEGER            INFO, LDA, M, N, RANK
      DOUBLE PRECISION   RCOND, SVLMAX
C     .. Array Arguments ..
      INTEGER            JPVT( * )
      DOUBLE PRECISION   A( LDA, * ), SVAL( 3 ), TAU( * ), DWORK( * )
C     .. Local Scalars ..
      LOGICAL            LJOBQR
      INTEGER            I, ISMAX, ISMIN, MN
      DOUBLE PRECISION   C1, C2, S1, S2, SMAX, SMAXPR, SMIN, SMINPR
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. External Subroutines ..
      EXTERNAL           DGEQPF, DLAIC1, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX, MIN
C     ..
C     .. Executable Statements ..
C
      LJOBQR = LSAME( JOBQR, 'Q' ) 
      MN = MIN( M, N )
      ISMIN = 1
      ISMAX = MN + 1
C
C     Test the input scalar arguments.
C
      INFO = 0
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
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB03OD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible
C
      IF( MN.EQ.0 ) THEN
         RANK = 0
         SVAL( 1 ) = ZERO
         SVAL( 2 ) = ZERO
         SVAL( 3 ) = ZERO
         RETURN
      END IF
C
      IF ( LJOBQR ) THEN
C
C        Compute QR factorization with column pivoting of A:
C           A * P = Q * R
C        Workspace 3*N. Details of Householder rotations stored in TAU.
C
         CALL DGEQPF( M, N, A, LDA, JPVT, TAU, DWORK( 1 ), INFO )
      END IF
C
C     Determine RANK using incremental condition estimation
C
      DWORK( ISMIN ) = ONE
      DWORK( ISMAX ) = ONE
      SMAX = ABS( A( 1, 1 ) )
      SMIN = SMAX
      IF( SMAX.EQ.ZERO .OR. SVLMAX*RCOND.GT.SMAX ) THEN
         RANK = 0
         SVAL( 1 ) = SMAX
         SVAL( 2 ) = ZERO
         SVAL( 3 ) = ZERO
      ELSE
         RANK = 1
         SMINPR = SMIN
C
   10    CONTINUE
         IF( RANK.LT.MN ) THEN
            I = RANK + 1
            CALL DLAIC1( IMIN, RANK, DWORK( ISMIN ), SMIN, A( 1, I ),
     $                   A( I, I ), SMINPR, S1, C1 )
            CALL DLAIC1( IMAX, RANK, DWORK( ISMAX ), SMAX, A( 1, I ),
     $                   A( I, I ), SMAXPR, S2, C2 )
C
            IF( SVLMAX*RCOND.LE.SMAXPR ) THEN
               IF( SVLMAX*RCOND.LE.SMINPR ) THEN
                  IF( SMAXPR*RCOND.LE.SMINPR ) THEN
                     DO 20 I = 1, RANK
                        DWORK( ISMIN+I-1 ) = S1*DWORK( ISMIN+I-1 )
                        DWORK( ISMAX+I-1 ) = S2*DWORK( ISMAX+I-1 )
   20                CONTINUE
                     DWORK( ISMIN+RANK ) = C1
                     DWORK( ISMAX+RANK ) = C2
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
C
      RETURN
C *** Last line of MB03OD ***
      END
