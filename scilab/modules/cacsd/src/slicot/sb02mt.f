      SUBROUTINE SB02MT( JOBG, JOBL, FACT, UPLO, N, M, A, LDA, B, LDB,
     $                   Q, LDQ, R, LDR, L, LDL, IPIV, OUFACT, G, LDG,
     $                   IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the following matrices 
C
C                -1
C         G = B*R  *B',
C
C         -          -1
C         A = A - B*R  *L',
C
C         -          -1
C         Q = Q - L*R  *L',
C      
C     where A, B, Q, R, L, and G are N-by-N, N-by-M, N-by-N, M-by-M,
C     N-by-M, and N-by-N matrices, respectively, with Q, R and G
C     symmetric matrices. 
C
C     When R is well-conditioned with respect to inversion, standard
C     algorithms for solving linear-quadratic optimization problems will
C     then also solve optimization problems with coupling weighting 
C     matrix L. Moreover, a gain in efficiency is possible using matrix
C     G in the deflating subspace algorithms (see SLICOT Library routine
C     SB02OD).
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOBG    CHARACTER*1
C             Specifies whether or not the matrix G is to be computed,
C             as follows:
C             = 'G':  Compute G;
C             = 'N':  Do not compute G.
C
C     JOBL    CHARACTER*1
C             Specifies whether or not the matrix L is zero, as follows:
C             = 'Z':  L is zero;
C             = 'N':  L is nonzero.
C
C     FACT    CHARACTER*1
C             Specifies how the matrix R is given (factored or not), as 
C             follows:
C             = 'N':  Array R contains the matrix R;
C             = 'C':  Array R contains the Cholesky factor of R;
C             = 'U':  Array R contains the symmetric indefinite UdU' or
C                     LdL' factorization of R. 
C
C     UPLO    CHARACTER*1
C             Specifies which triangle of the matrices R and Q (if
C             JOBL = 'N') is stored, as follows:
C             = 'U':  Upper triangle is stored;
C             = 'L':  Lower triangle is stored.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, Q, and G, and the number of
C             rows of the matrices B and L.  N >= 0.
C
C     M       (input) INTEGER
C             The order of the matrix R, and the number of columns of
C             the matrices B and L.  M >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, if JOBL = 'N', the leading N-by-N part of this
C             array must contain the matrix A.
C             On exit, if JOBL = 'N', and INFO = 0, the leading N-by-N
C                                                    -          -1    
C             part of this array contains the matrix A = A - B*R  L'.
C             If JOBL = 'Z', this array is not referenced.
C
C     LDA     INTEGER
C             The leading dimension of array A.
C             LDA >= MAX(1,N) if JOBL = 'N';
C             LDA >= 1        if JOBL = 'Z'.
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,M)
C             On entry, the leading N-by-M part of this array must
C             contain the matrix B.
C             On exit, if OUFACT = 1, and INFO = 0, the leading N-by-M
C                                                             -1  
C             part of this array contains the matrix B*chol(R)  .
C             On exit, B is unchanged if OUFACT = 2 (hence also when
C             FACT = 'U').
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     Q       (input/output) DOUBLE PRECISION array, dimension (LDQ,N)
C             On entry, if JOBL = 'N', the leading N-by-N upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the upper 
C             triangular part or lower triangular part, respectively, of
C             the symmetric matrix Q. The stricly lower triangular part 
C             (if UPLO = 'U') or stricly upper triangular part (if 
C             UPLO = 'L') is not referenced.
C             On exit, if JOBL = 'N' and INFO = 0, the leading N-by-N
C             upper triangular part (if UPLO = 'U') or lower triangular
C             part (if UPLO = 'L') of this array contains the upper 
C             triangular part or lower triangular part, respectively, of
C                                  -          -1
C             the symmetric matrix Q = Q - L*R  *L'.
C             If JOBL = 'Z', this array is not referenced.
C
C     LDQ     INTEGER
C             The leading dimension of array Q.
C             LDQ >= MAX(1,N) if JOBL = 'N';
C             LDQ >= 1        if JOBL = 'Z'.
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,M)
C             On entry, if FACT = 'N', the leading M-by-M upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the upper
C             triangular part or lower triangular part, respectively,
C             of the symmetric input weighting matrix R.
C             On entry, if FACT = 'C', the leading M-by-M upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the Cholesky
C             factor of the positive definite input weighting matrix R
C             (as produced by LAPACK routine DPOTRF).
C             On entry, if FACT = 'U', the leading M-by-M upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array must contain the factors of
C             the UdU' or LdL' factorization, respectively, of the 
C             symmetric indefinite input weighting matrix R (as produced
C             by LAPACK routine DSYTRF).
C             If FACT = 'N', the stricly lower triangular part (if UPLO
C             = 'U') or stricly upper triangular part (if UPLO = 'L') of
C             this array is used as workspace.
C             On exit, if OUFACT = 1, and INFO = 0 (or INFO = M+1),
C             the leading M-by-M upper triangular part (if UPLO = 'U')
C             or lower triangular part (if UPLO = 'L') of this array 
C             contains the Cholesky factor of the given input weighting
C             matrix. 
C             On exit, if OUFACT = 2, and INFO = 0 (or INFO = M+1),
C             the leading M-by-M upper triangular part (if UPLO = 'U')
C             or lower triangular part (if UPLO = 'L') of this array 
C             contains the factors of the UdU' or LdL' factorization, 
C             respectively, of the given input weighting matrix. 
C             On exit R is unchanged if FACT = 'C' or 'U'.
C             
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,M).
C
C     L       (input/output) DOUBLE PRECISION array, dimension (LDL,M)
C             On entry, if JOBL = 'N', the leading N-by-M part of this
C             array must contain the matrix L.
C             On exit, if JOBL = 'N', OUFACT = 1, and INFO = 0, the
C             leading N-by-M part of this array contains the matrix 
C                      -1
C             L*chol(R)  .
C             On exit, L is unchanged if OUFACT = 2 (hence also when
C             FACT = 'U').
C             L is not referenced if JOBL = 'Z'.
C
C     LDL     INTEGER
C             The leading dimension of array L.
C             LDL >= MAX(1,N) if JOBL = 'N';
C             LDL >= 1        if JOBL = 'Z'.
C
C     IPIV    (input/output) INTEGER array, dimension (M)
C             On entry, if FACT = 'U', this array must contain details
C             of the interchanges performed and the block structure of
C             the d factor in the UdU' or LdL' factorization of matrix R
C             (as produced by LAPACK routine DSYTRF).
C             On exit, if OUFACT = 2, this array contains details of
C             the interchanges performed and the block structure of the
C             d factor in the UdU' or LdL' factorization of matrix R, 
C             as produced by LAPACK routine DSYTRF.
C             This array is not referenced if FACT = 'C'.
C
C     OUFACT  (output) INTEGER 
C             Information about the factorization finally used.
C             OUFACT = 1:  Cholesky factorization of R has been used;
C             OUFACT = 2:  UdU' (if UPLO = 'U') or LdL' (if UPLO = 'L') 
C                          factorization of R has been used.
C
C     G       (output) DOUBLE PRECISION array, dimension (LDG,N)
C             If JOBG = 'G', and INFO = 0, the leading N-by-N upper
C             triangular part (if UPLO = 'U') or lower triangular part
C             (if UPLO = 'L') of this array contains the upper
C             triangular part (if UPLO = 'U') or lower triangular part
C                                                                 -1   
C             (if UPLO = 'L'), respectively, of the matrix G = B*R  B'. 
C             If JOBG = 'N', this array is not referenced.
C
C     LDG     INTEGER
C             The leading dimension of array G.
C             LDG >= MAX(1,N) if JOBG = 'G',
C             LDG >= 1        if JOBG = 'N'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (M)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK; if FACT = 'N', DWORK(2) contains the reciprocal
C             condition number of the given matrix R.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= 1              if FACT = 'C';
C             LDWORK >= MAX(2,3*M,N*M) if FACT = 'N';
C             LDWORK >= MAX(1,N*M)     if FACT = 'U'.
C             For optimum performance LDWORK should be larger than 3*M,
C             if FACT = 'N'.
C             The N*M workspace is not needed for FACT = 'N', if matrix
C             R is positive definite.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = i:  if the i-th element (1 <= i <= M) of the d factor is
C                   exactly zero; the UdU' (or LdL') factorization has
C                   been completed, but the block diagonal matrix d is
C                   exactly singular;
C             = M+1:  if the matrix R is numerically singular.
C
C     METHOD
C                            -     -  
C     The matrices G, and/or A and Q are evaluated using the given or
C     computed symmetric factorization of R. 
C
C     NUMERICAL ASPECTS
C
C     The routine should not be used when R is ill-conditioned.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, discrete-time system, optimal regulator, Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         FACT, JOBG, JOBL, UPLO
      INTEGER           INFO, LDA, LDB, LDG, LDL, LDQ, LDR, LDWORK, M,
     $                  N, OUFACT
C     .. Array Arguments ..
      INTEGER           IPIV(*), IWORK(*)
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*), G(LDG,*), 
     $                  L(LDL,*), Q(LDQ,*), R(LDR,*)
C     .. Local Scalars ..
      LOGICAL           LFACTA, LFACTC, LFACTU, LJOBG, LJOBL, LUPLOU
      CHARACTER         TRANS
      INTEGER           I, J, WRKOPT 
      DOUBLE PRECISION  EPS, RCOND, RNORM
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH, DLANSY
      EXTERNAL          DLAMCH, DLANSY, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMM, DGEMV, DPOCON, DPOTRF, DSYCON, 
     $                  DSYRK, DSYTRF, DSYTRS, DTRSM, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         INT, MAX
C     .. Executable Statements ..
C
      INFO   = 0
      LJOBG  = LSAME( JOBG, 'G' )
      LJOBL  = LSAME( JOBL, 'N' )
      LFACTC = LSAME( FACT, 'C' )
      LFACTU = LSAME( FACT, 'U' )
      LUPLOU = LSAME( UPLO, 'U' )
      LFACTA = LFACTC.OR.LFACTU
C
C     Test the input scalar arguments.
C
      IF( .NOT.LJOBG .AND. .NOT.LSAME( JOBG, 'N' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.LJOBL .AND. .NOT.LSAME( JOBL, 'Z' ) ) THEN
         INFO = -2
      ELSE IF( .NOT.LFACTA .AND. .NOT.LSAME( FACT, 'N' ) ) THEN
         INFO = -3
      ELSE IF( .NOT.LUPLOU .AND. .NOT.LSAME( UPLO, 'L' ) ) THEN
         INFO = -4
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( M.LT.0 ) THEN
         INFO = -6
      ELSE IF( ( LDA.LT.1 ) .OR. ( LJOBL .AND. LDA.LT.N ) ) THEN
         INFO = -8
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( ( LDQ.LT.1 ) .OR. ( LJOBL .AND. LDQ.LT.N ) ) THEN
         INFO = -12
      ELSE IF( LDR.LT.MAX( 1, M ) ) THEN
         INFO = -14
      ELSE IF( ( LDL.LT.1 ) .OR. ( LJOBL .AND. LDL.LT.N ) ) THEN
         INFO = -16
      ELSE IF( ( LDG.LT.1 ) .OR. ( LJOBG .AND. LDG.LT.N ) ) THEN
         INFO = -20
      ELSE IF( ( LFACTC .AND. LDWORK.LT.1 ) .OR.
     $         ( LFACTU .AND. LDWORK.LT.MAX( 1, N*M ) ) .OR.
     $    ( .NOT.LFACTA .AND. LDWORK.LT.MAX( 2, N*M, 3*M ) ) ) THEN
         INFO = -23
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB02MT', -INFO )
         RETURN
      END IF
C
      IF ( LFACTC ) THEN
         OUFACT = 1
      ELSE IF ( LFACTU ) THEN
         OUFACT = 2
      END IF
C
C     Quick return if possible.
C
      IF ( N.EQ.0 .OR. M.EQ.0 .OR. .NOT.( LJOBL.OR.LJOBG ) ) THEN
        DWORK(1) = ONE
        IF ( .NOT.LFACTA ) DWORK(2) = ONE
        RETURN
      END IF
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of workspace needed at that point in the code,
C     as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C 
      WRKOPT = 1
C
C     Set relative machine precision.
C
      EPS = DLAMCH( 'Epsilon' )
C
      IF ( .NOT.LFACTA ) THEN
C     
C        Compute the norm of the matrix R, which is not factored.
C        Then save the given triangle of R in the other strict triangle
C        and the diagonal in the workspace, and try Cholesky 
C        factorization.
C        Workspace: need M.
C     
         RNORM = DLANSY( '1-norm', UPLO, M, R, LDR, DWORK )
         CALL DCOPY( M, R, LDR+1, DWORK, 1 ) 
         IF( LUPLOU ) THEN
C        
            DO 20 J = 2, M
               CALL DCOPY( J-1, R(1,J), 1, R(J,1), LDR )
   20       CONTINUE
C        
         ELSE
C        
            DO 40 J = 2, M
               CALL DCOPY( J-1, R(J,1), LDR, R(1,J), 1 )
   40       CONTINUE
C        
         END IF
         CALL DPOTRF( UPLO, M, R, LDR, INFO )
         IF( INFO.EQ.0 ) THEN
C        
C           Compute the reciprocal of the condition number of R.
C           Workspace: need 3*M.
C           
            CALL DPOCON( UPLO, M, R, LDR, RNORM, RCOND, DWORK, IWORK,
     $                   INFO )
C           
C           Return if the matrix is singular to working precision.
C           
            OUFACT = 1
            DWORK(2) = RCOND
            IF( RCOND.LT.EPS ) THEN
               INFO = M + 1
               RETURN
            END IF
            WRKOPT = MAX( WRKOPT, 3*M )
         ELSE
C        
C           Use UdU' or LdL' factorization, first restoring the saved
C           triangle.
C        
            CALL DCOPY( M, DWORK, 1, R, LDR+1 )
            IF( LUPLOU ) THEN
C           
               DO 60 J = 2, M
                  CALL DCOPY( J-1, R(J,1), LDR, R(1,J), 1 )
   60          CONTINUE
C           
            ELSE
C           
               DO 80 J = 2, M
                  CALL DCOPY( J-1, R(1,J), 1, R(J,1), LDR )
   80          CONTINUE
C           
            END IF
C        
C           Compute the UdU' or LdL' factorization.
C           Workspace: need   1,
C                      prefer M*NB.
C        
            CALL DSYTRF( UPLO, M, R, LDR, IPIV, DWORK, LDWORK, INFO ) 
            OUFACT = 2
            IF( INFO.GT.0 ) THEN
               DWORK(2) = ONE
               RETURN
            END IF 
            WRKOPT = MAX( WRKOPT, INT( DWORK(1) ) )
C        
C           Compute the reciprocal of the condition number of R.
C           Workspace: need 2*M.
C           
            CALL DSYCON( UPLO, M, R, LDR, IPIV, RNORM, RCOND, DWORK,
     $                   IWORK, INFO )
C           
C           Return if the matrix is singular to working precision.
C           
            DWORK(2) = RCOND
            IF( RCOND.LT.EPS ) THEN
               INFO = M + 1
               RETURN
            END IF
         END IF
      END IF
C
      IF (OUFACT.EQ.1 ) THEN
C           
C        Solve positive definite linear system(s).
C              
         IF ( LUPLOU ) THEN
            TRANS = 'N'
         ELSE
            TRANS = 'T'
         END IF
C
C        Solve the system X*U = B, overwriting B with X.
C        
         CALL DTRSM( 'Right', UPLO, TRANS, 'Non-unit', N, M,
     $               ONE, R, LDR, B, LDB )
C
         IF ( LJOBG ) THEN
C                                      -1
C           Compute the matrix  G = B*R  *B', multiplying X*X' in G.
C        
            CALL DSYRK( UPLO, 'No transpose', N, M, ONE, B, LDB, ZERO,
     $                  G, LDG )
         END IF
C        
         IF( LJOBL ) THEN
C        
C           Update matrices A and Q.
C        
C           Solve the system Y*U = L, overwriting L with Y.
C        
            CALL DTRSM( 'Right', UPLO, TRANS, 'Non-unit', N, M,
     $                  ONE, R, LDR, L, LDL )
C        
C           Compute A <- A - X*Y'.
C        
            CALL DGEMM( 'No transpose', 'Transpose', N, N, M, -ONE, B,
     $                  LDB, L, LDL, ONE, A, LDA )
C        
C           Compute Q <- Q - Y*Y'.
C        
            CALL DSYRK( UPLO, 'No transpose', N, M, -ONE, L, LDL, ONE,
     $                  Q, LDQ )
         END IF
      ELSE
C           
C        Solve indefinite linear system(s).
C              
C        Solve the system UdU'*X = B' (or LdL'*X = B').
C        Workspace: need N*M.
C        
         DO 100 J = 1, M
            CALL DCOPY( N, B(1,J), 1, DWORK(J), M )
  100    CONTINUE
C        
         CALL DSYTRS( UPLO, M, N, R, LDR, IPIV, DWORK, M, INFO )
C
         IF ( LJOBG ) THEN
C                                                    -1
C           Compute a triangle of the matrix  G = B*R  *B' = B*X.
C        
            IF ( LUPLOU ) THEN
               I = 1
C
               DO 120 J = 1, N
                  CALL DGEMV( 'No transpose', J, M, ONE, B, LDB,
     $                        DWORK(I), 1, ZERO, G(1,J), 1 )
                  I = I + M
  120          CONTINUE
C
            ELSE
C           
               DO 140 J = 1, N
                  CALL DGEMV( 'Transpose', M, J, ONE, DWORK, M, B(J,1),
     $                        LDB, ZERO, G(J,1), LDG )
  140          CONTINUE
C
            END IF
         END IF
C        
         IF( LJOBL ) THEN
C        
C           Update matrices A and Q.
C
C           Solve the system UdU'*Y = L' (or LdL'*Y = L').
C        
            DO 160 J = 1, M
               CALL DCOPY( N, L(1,J), 1, DWORK(J), M )
  160       CONTINUE
C        
            CALL DSYTRS( UPLO, M, N, R, LDR, IPIV, DWORK, M, INFO )
C        
C           A <- A - B*Y.
C        
            CALL DGEMM( 'No transpose', 'No transpose', N, N, M, -ONE,
     $                  B, LDB, DWORK, M, ONE, A, LDA )
C                                            -          -1
C           Compute a triangle of the matrix Q = Q - L*R  *L' = Q - L*Y.
C        
            IF ( LUPLOU ) THEN
               I = 1
C
               DO 180 J = 1, N
                  CALL DGEMV( 'No transpose', J, M, -ONE, L, LDL,
     $                        DWORK(I), 1, ONE, Q(1,J), 1 )
                  I = I + M
  180          CONTINUE
C
            ELSE
C           
               DO 200 J = 1, N
                  CALL DGEMV( 'Transpose', M, J, -ONE, DWORK, M, L(J,1),
     $                        LDL, ONE, Q(J,1), LDQ )
  200          CONTINUE
C
            END IF
         END IF
      END IF
C
      DWORK(1) = WRKOPT
      IF ( .NOT.LFACTA ) DWORK(2) = RCOND
C
C *** Last line of SB02MT ***
      RETURN
      END
