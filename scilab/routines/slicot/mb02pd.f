      SUBROUTINE MB02PD( FACT, TRANS, N, NRHS, A, LDA, AF, LDAF, IPIV,
     $                   EQUED, R, C, B, LDB, X, LDX, RCOND, FERR, BERR,
     $                   IWORK, DWORK, INFO )
C
C     RELEASE 3.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve (if well-conditioned) the matrix equations
C     
C        op( A )*X = B,
C     
C     where X and B are N-by-NRHS matrices, A is an N-by-N matrix and
C     op( A ) is one of
C     
C        op( A ) = A   or   op( A ) = A'.
C     
C     Error bounds on the solution and a condition estimate are also
C     provided.
C     
C     ARGUMENTS
C
C     Mode Parameters
C
C     FACT    CHARACTER*1
C             Specifies whether or not the factored form of the matrix A
C             is supplied on entry, and if not, whether the matrix A
C             should be equilibrated before it is factored.
C             = 'F':  On entry, AF and IPIV contain the factored form
C                     of A. If EQUED is not 'N', the matrix A has been
C                     equilibrated with scaling factors given by R
C                     and C. A, AF, and IPIV are not modified.
C             = 'N':  The matrix A will be copied to AF and factored.
C             = 'E':  The matrix A will be equilibrated if necessary,
C                     then copied to AF and factored.
C
C     TRANS   CHARACTER*1
C             Specifies the form of the system of equations as follows:
C             = 'N':  A * X = B     (No transpose);
C             = 'T':  A**T * X = B  (Transpose);
C             = 'C':  A**H * X = B  (Transpose).
C            
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The number of linear equations, i.e., the order of the
C             matrix A.  N >= 0.
C            
C     NRHS    (input) INTEGER
C             The number of right hand sides, i.e., the number of
C             columns of the matrices B and X.  NRHS >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N part of this array must
C             contain the matrix A.  If FACT = 'F' and EQUED is not 'N',
C             then A must have been equilibrated by the scaling factors
C             in R and/or C.  A is not modified if FACT = 'F' or 'N',
C             or if FACT = 'E' and EQUED = 'N' on exit.
C             On exit, if EQUED .NE. 'N', the leading N-by-N part of
C             this array contains the matrix A scaled as follows:
C             EQUED = 'R':  A := diag(R) * A;
C             EQUED = 'C':  A := A * diag(C);
C             EQUED = 'B':  A := diag(R) * A * diag(C).
C            
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,N).
C            
C     AF      (input or output) DOUBLE PRECISION array, dimension
C             (LDAF,N)
C             If FACT = 'F', then AF is an input argument and on entry
C             the leading N-by-N part of this array must contain the
C             factors L and U from the factorization A = P*L*U as
C             computed by DGETRF.  If EQUED .NE. 'N', then AF is the
C             factored form of the equilibrated matrix A.
C             If FACT = 'N', then AF is an output argument and on exit
C             the leading N-by-N part of this array contains the factors
C             L and U from the factorization A = P*L*U of the original
C             matrix A.
C             If FACT = 'E', then AF is an output argument and on exit
C             the leading N-by-N part of this array contains the factors
C             L and U from the factorization A = P*L*U of the
C             equilibrated matrix A (see the description of A for the
C             form of the equilibrated matrix).
C
C     LDAF    (input) INTEGER
C             The leading dimension of the array AF.  LDAF >= max(1,N).
C
C     IPIV    (input or output) INTEGER array, dimension (N)
C             If FACT = 'F', then IPIV is an input argument and on entry
C             it must contain the pivot indices from the factorization
C             A = P*L*U as computed by DGETRF; row i of the matrix was
C             interchanged with row IPIV(i).
C             If FACT = 'N', then IPIV is an output argument and on exit
C             it contains the pivot indices from the factorization
C             A = P*L*U of the original matrix A.
C             If FACT = 'E', then IPIV is an output argument and on exit
C             it contains the pivot indices from the factorization
C             A = P*L*U of the equilibrated matrix A.
C
C     EQUED   (input or output) CHARACTER*1
C             Specifies the form of equilibration that was done as
C             follows:
C             = 'N':  No equilibration (always true if FACT = 'N');
C             = 'R':  Row equilibration, i.e., A has been premultiplied
C                     by diag(R);
C             = 'C':  Column equilibration, i.e., A has been
C                     postmultiplied by diag(C);
C             = 'B':  Both row and column equilibration, i.e., A has
C                     been replaced by diag(R) * A * diag(C).
C             EQUED is an input argument if FACT = 'F'; otherwise, it is
C             an output argument.
C
C     R       (input or output) DOUBLE PRECISION array, dimension (N)
C             The row scale factors for A.  If EQUED = 'R' or 'B', A is
C             multiplied on the left by diag(R); if EQUED = 'N' or 'C',
C             R is not accessed.  R is an input argument if FACT = 'F';
C             otherwise, R is an output argument.  If FACT = 'F' and
C             EQUED = 'R' or 'B', each element of R must be positive.
C
C     C       (input or output) DOUBLE PRECISION array, dimension (N)
C             The column scale factors for A.  If EQUED = 'C' or 'B',
C             A is multiplied on the right by diag(C); if EQUED = 'N'
C             or 'R', C is not accessed.  C is an input argument if
C             FACT = 'F'; otherwise, C is an output argument.  If
C             FACT = 'F' and EQUED = 'C' or 'B', each element of C must
C             be positive.
C
C     B       (input/output) DOUBLE PRECISION array, dimension
C             (LDB,NRHS)
C             On entry, the leading N-by-NRHS part of this array must
C             contain the right-hand side matrix B.
C             On exit,
C             if EQUED = 'N', B is not modified;
C             if TRANS = 'N' and EQUED = 'R' or 'B', the leading
C             N-by-NRHS part of this array contains diag(R)*B;
C             if TRANS = 'T' or 'C' and EQUED = 'C' or 'B', the leading
C             N-by-NRHS part of this array contains diag(C)*B.
C
C     LDB     INTEGER
C             The leading dimension of the array B.  LDB >= max(1,N).
C     
C     X       (output) DOUBLE PRECISION array, dimension (LDX,NRHS)
C             If INFO = 0 or INFO = N+1, the leading N-by-NRHS part of
C             this array contains the solution matrix X to the original
C             system of equations.  Note that A and B are modified on
C             exit if EQUED .NE. 'N', and the solution to the
C             equilibrated system is inv(diag(C))*X if TRANS = 'N' and
C             EQUED = 'C' or 'B', or inv(diag(R))*X if TRANS = 'T' or
C             'C' and EQUED = 'R' or 'B'.
C
C     LDX     (input) INTEGER
C             The leading dimension of the array X.  LDX >= max(1,N).
C
C     RCOND   (output) DOUBLE PRECISION
C             The estimate of the reciprocal condition number of the
C             matrix A after equilibration (if done).  If RCOND is less
C             than the machine precision (in particular, if RCOND = 0),
C             the matrix is singular to working precision.  This
C             condition is indicated by a return code of INFO > 0.
C             For efficiency reasons, RCOND is computed only when the
C             matrix A is factored, i.e., for FACT = 'N' or 'E'.  For
C             FACT = 'F', RCOND is not used, but it is assumed that it
C             has been computed and checked before the routine call.
C
C     FERR    (output) DOUBLE PRECISION array, dimension (NRHS)
C             The estimated forward error bound for each solution vector
C             X(j) (the j-th column of the solution matrix X).
C             If XTRUE is the true solution corresponding to X(j),
C             FERR(j) is an estimated upper bound for the magnitude of
C             the largest element in (X(j) - XTRUE) divided by the
C             magnitude of the largest element in X(j).  The estimate
C             is as reliable as the estimate for RCOND, and is almost
C             always a slight overestimate of the true error.
C
C     BERR    (output) DOUBLE PRECISION array, dimension (NRHS)
C             The componentwise relative backward error of each solution
C             vector X(j) (i.e., the smallest relative change in
C             any element of A or B that makes X(j) an exact solution).
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (N)
C
C     DWORK   DOUBLE PRECISION array, dimension (4*N)
C             On exit, DWORK(1) contains the reciprocal pivot growth
C             factor norm(A)/norm(U). The "max absolute element" norm is
C             used. If DWORK(1) is much less than 1, then the stability
C             of the LU factorization of the (equilibrated) matrix A
C             could be poor. This also means that the solution X,
C             condition estimator RCOND, and forward error bound FERR
C             could be unreliable. If factorization fails with
C             0 < INFO <= N, then DWORK(1) contains the reciprocal pivot
C             growth factor for the leading INFO columns of A.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             > 0:  if INFO = i, and i is
C                   <= N:  U(i,i) is exactly zero.  The factorization
C                          has been completed, but the factor U is
C                          exactly singular, so the solution and error
C                          bounds could not be computed. RCOND = 0 is
C                          returned.
C                   = N+1: U is nonsingular, but RCOND is less than
C                          machine precision, meaning that the matrix is
C                          singular to working precision.  Nevertheless,
C                          the solution and error bounds are computed
C                          because there are a number of situations
C                          where the computed solution can be more
C                          accurate than the value of RCOND would
C                          suggest.
C             The positive values for INFO are set only when the
C             matrix A is factored, i.e., for FACT = 'N' or 'E'.
C
C     METHOD
C
C     The following steps are performed:
C
C     1. If FACT = 'E', real scaling factors are computed to equilibrate
C        the system:
C
C        TRANS = 'N':  diag(R)*A*diag(C)     *inv(diag(C))*X = diag(R)*B
C        TRANS = 'T': (diag(R)*A*diag(C))**T *inv(diag(R))*X = diag(C)*B
C        TRANS = 'C': (diag(R)*A*diag(C))**H *inv(diag(R))*X = diag(C)*B
C
C        Whether or not the system will be equilibrated depends on the
C        scaling of the matrix A, but if equilibration is used, A is
C        overwritten by diag(R)*A*diag(C) and B by diag(R)*B
C        (if TRANS='N') or diag(C)*B (if TRANS = 'T' or 'C').
C
C     2. If FACT = 'N' or 'E', the LU decomposition is used to factor
C        the matrix A (after equilibration if FACT = 'E') as
C           A = P * L * U,
C        where P is a permutation matrix, L is a unit lower triangular
C        matrix, and U is upper triangular.
C
C     3. If some U(i,i)=0, so that U is exactly singular, then the
C        routine returns with INFO = i. Otherwise, the factored form
C        of A is used to estimate the condition number of the matrix A.
C        If the reciprocal of the condition number is less than machine
C        precision, INFO = N+1 is returned as a warning, but the routine
C        still goes on to solve for X and compute error bounds as
C        described below.
C
C     4. The system of equations is solved for X using the factored form
C        of A.
C
C     5. Iterative refinement is applied to improve the computed
C        solution matrix and calculate error bounds and backward error
C        estimates for it.
C
C     6. If equilibration was used, the matrix X is premultiplied by
C        diag(C) (if TRANS = 'N') or diag(R) (if TRANS = 'T' or 'C') so
C        that it solves the original system before equilibration.
C
C     REFERENCES
C
C     [1] Anderson, E., Bai, Z., Bischof, C., Demmel, J., Dongarra, J.,
C         Du Croz, J., Greenbaum, A., Hammarling, S., McKenney, A.,
C         Ostrouchov, S., Sorensen, D.
C         LAPACK Users' Guide: Second Edition, SIAM, Philadelphia, 1995.
C
C     FURTHER COMMENTS
C
C     This is a simplified version of the LAPACK Library routine DGESVX,
C     useful when several sets of matrix equations with the same
C     coefficient matrix  A and/or A'  should be solved.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations.
C
C     CONTRIBUTORS
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 1999.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Condition number, matrix algebra, matrix operations.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         EQUED, FACT, TRANS
      INTEGER           INFO, LDA, LDAF, LDB, LDX, N, NRHS
      DOUBLE PRECISION  RCOND
C     ..
C     .. Array Arguments ..
      INTEGER           IPIV( * ), IWORK( * )
      DOUBLE PRECISION  A( LDA, * ), AF( LDAF, * ), B( LDB, * ),
     $                  BERR( * ), C( * ), DWORK( * ), FERR( * ),
     $                  R( * ), X( LDX, * )
C     ..
C     .. Local Scalars ..
      LOGICAL           COLEQU, EQUIL, NOFACT, NOTRAN, ROWEQU
      CHARACTER         NORM
      INTEGER           I, INFEQU, J
      DOUBLE PRECISION  AMAX, ANORM, BIGNUM, COLCND, RCMAX, RCMIN,
     $                  ROWCND, RPVGRW, SMLNUM
C     ..
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH, DLANGE, DLANTR
      EXTERNAL          LSAME, DLAMCH, DLANGE, DLANTR
C     ..
C     .. External Subroutines ..
      EXTERNAL          DGECON, DGEEQU, DGERFS, DGETRF, DGETRS, DLACPY,
     $                  DLAQGE, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C     ..
C     .. Save Statement ..
      SAVE              RPVGRW
C     ..
C     .. Executable Statements ..
C
      INFO = 0
      NOFACT = LSAME( FACT,  'N' )
      EQUIL  = LSAME( FACT,  'E' )
      NOTRAN = LSAME( TRANS, 'N' )
      IF( NOFACT .OR. EQUIL ) THEN
         EQUED = 'N'
         ROWEQU = .FALSE.
         COLEQU = .FALSE.
      ELSE
         ROWEQU = LSAME( EQUED, 'R' ) .OR. LSAME( EQUED, 'B' )
         COLEQU = LSAME( EQUED, 'C' ) .OR. LSAME( EQUED, 'B' )
         SMLNUM = DLAMCH( 'Safe minimum' )
         BIGNUM = ONE / SMLNUM
      END IF
C
C     Test the input parameters.
C
      IF( .NOT.NOFACT .AND. .NOT.EQUIL .AND. .NOT.LSAME( FACT, 'F' ) )
     $     THEN
         INFO = -1
      ELSE IF( .NOT.NOTRAN .AND. .NOT.LSAME( TRANS, 'T' ) .AND. .NOT.
     $                                LSAME( TRANS, 'C' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( NRHS.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -6
      ELSE IF( LDAF.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LSAME( FACT, 'F' ) .AND. .NOT.
     $         ( ROWEQU .OR. COLEQU .OR. LSAME( EQUED, 'N' ) ) ) THEN
         INFO = -10
      ELSE
         IF( ROWEQU ) THEN
            RCMIN = BIGNUM
            RCMAX = ZERO
            DO 10 J = 1, N
               RCMIN = MIN( RCMIN, R( J ) )
               RCMAX = MAX( RCMAX, R( J ) )
   10       CONTINUE
            IF( RCMIN.LE.ZERO ) THEN
               INFO = -11
            ELSE IF( N.GT.0 ) THEN
               ROWCND = MAX( RCMIN, SMLNUM ) / MIN( RCMAX, BIGNUM )
            ELSE
               ROWCND = ONE
            END IF
         END IF
         IF( COLEQU .AND. INFO.EQ.0 ) THEN
            RCMIN = BIGNUM
            RCMAX = ZERO
            DO 20 J = 1, N
               RCMIN = MIN( RCMIN, C( J ) )
               RCMAX = MAX( RCMAX, C( J ) )
   20       CONTINUE
            IF( RCMIN.LE.ZERO ) THEN
               INFO = -12
            ELSE IF( N.GT.0 ) THEN
               COLCND = MAX( RCMIN, SMLNUM ) / MIN( RCMAX, BIGNUM )
            ELSE
               COLCND = ONE
            END IF
         END IF
         IF( INFO.EQ.0 ) THEN
            IF( LDB.LT.MAX( 1, N ) ) THEN
               INFO = -14
            ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
               INFO = -16
            END IF
         END IF
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB02PD', -INFO )
         RETURN
      END IF
C
      IF( EQUIL ) THEN
C
C        Compute row and column scalings to equilibrate the matrix A.
C
         CALL DGEEQU( N, N, A, LDA, R, C, ROWCND, COLCND, AMAX, INFEQU )
         IF( INFEQU.EQ.0 ) THEN
C
C           Equilibrate the matrix.
C
            CALL DLAQGE( N, N, A, LDA, R, C, ROWCND, COLCND, AMAX,
     $                   EQUED )
            ROWEQU = LSAME( EQUED, 'R' ) .OR. LSAME( EQUED, 'B' )
            COLEQU = LSAME( EQUED, 'C' ) .OR. LSAME( EQUED, 'B' )
         END IF
      END IF
C
C     Scale the right hand side.
C
      IF( NOTRAN ) THEN
         IF( ROWEQU ) THEN
            DO 40 J = 1, NRHS
               DO 30 I = 1, N
                  B( I, J ) = R( I )*B( I, J )
   30          CONTINUE
   40       CONTINUE
         END IF
      ELSE IF( COLEQU ) THEN
         DO 60 J = 1, NRHS
            DO 50 I = 1, N
               B( I, J ) = C( I )*B( I, J )
   50       CONTINUE
   60    CONTINUE
      END IF
C
      IF( NOFACT .OR. EQUIL ) THEN
C
C        Compute the LU factorization of A.
C
         CALL DLACPY( 'Full', N, N, A, LDA, AF, LDAF )
         CALL DGETRF( N, N, AF, LDAF, IPIV, INFO )
C
C        Return if INFO is non-zero.
C
         IF( INFO.NE.0 ) THEN
            IF( INFO.GT.0 ) THEN
C
C              Compute the reciprocal pivot growth factor of the
C              leading rank-deficient INFO columns of A.
C
               RPVGRW = DLANTR( 'M', 'U', 'N', INFO, INFO, AF, LDAF,
     $                  DWORK )
               IF( RPVGRW.EQ.ZERO ) THEN
                  RPVGRW = ONE
               ELSE
                  RPVGRW = DLANGE( 'M', N, INFO, A, LDA, DWORK ) /
     $                     RPVGRW
               END IF
               DWORK( 1 ) = RPVGRW
               RCOND = ZERO
            END IF
            RETURN
         END IF
C
C        Compute the norm of the matrix A and the
C        reciprocal pivot growth factor RPVGRW.
C
         IF( NOTRAN ) THEN
            NORM = '1'
         ELSE
            NORM = 'I'
         END IF
         ANORM = DLANGE( NORM, N, N, A, LDA, DWORK )
         RPVGRW = DLANTR( 'M', 'U', 'N', N, N, AF, LDAF, DWORK )
         IF( RPVGRW.EQ.ZERO ) THEN
            RPVGRW = ONE
         ELSE
            RPVGRW = DLANGE( 'M', N, N, A, LDA, DWORK ) / RPVGRW
         END IF
C
C        Compute the reciprocal of the condition number of A.
C
         CALL DGECON( NORM, N, AF, LDAF, ANORM, RCOND, DWORK, IWORK,
     $                INFO )
C
C        Set INFO = N+1 if the matrix is singular to working precision.
C
         IF( RCOND.LT.DLAMCH( 'Epsilon' ) )
     $      INFO = N + 1
      END IF
C
C     Compute the solution matrix X.
C
      CALL DLACPY( 'Full', N, NRHS, B, LDB, X, LDX )
      CALL DGETRS( TRANS, N, NRHS, AF, LDAF, IPIV, X, LDX, INFO )
C
C     Use iterative refinement to improve the computed solution and
C     compute error bounds and backward error estimates for it.
C
      CALL DGERFS( TRANS, N, NRHS, A, LDA, AF, LDAF, IPIV, B, LDB, X,
     $             LDX, FERR, BERR, DWORK, IWORK, INFO )
C
C     Transform the solution matrix X to a solution of the original
C     system.
C
      IF( NOTRAN ) THEN
         IF( COLEQU ) THEN
            DO 80 J = 1, NRHS
               DO 70 I = 1, N
                  X( I, J ) = C( I )*X( I, J )
   70          CONTINUE
   80       CONTINUE
            DO 90 J = 1, NRHS
               FERR( J ) = FERR( J ) / COLCND
   90       CONTINUE
         END IF
      ELSE IF( ROWEQU ) THEN
         DO 110 J = 1, NRHS
            DO 100 I = 1, N
               X( I, J ) = R( I )*X( I, J )
  100       CONTINUE
  110    CONTINUE
         DO 120 J = 1, NRHS
            FERR( J ) = FERR( J ) / ROWCND
  120    CONTINUE
      END IF
C
      DWORK( 1 ) = RPVGRW
      RETURN
C
C *** Last line of MB02PD ***
      END
