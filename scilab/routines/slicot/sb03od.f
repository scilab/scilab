      SUBROUTINE SB03OD( DICO, FACT, TRANS, N, M, A, LDA, Q, LDQ, B,
     $                   LDB, SCALE, WR, WI, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for X = op(U)'*op(U) either the stable non-negative
C     definite continuous-time Lyapunov equation
C                                   2
C        op(A)'*X + X*op(A) = -scale *op(B)'*op(B)                   (1)
C
C     or the convergent non-negative definite discrete-time Lyapunov
C     equation
C                                   2
C        op(A)'*X*op(A) - X = -scale *op(B)'*op(B)                   (2)
C
C     where op(K) = K or K' (i.e., the transpose of the matrix K), A is
C     an N-by-N matrix, op(B) is an M-by-N matrix, U is an upper
C     triangular matrix containing the Cholesky factor of the solution
C     matrix X, X = op(U)'*op(U), and scale is an output scale factor,
C     set less than or equal to 1 to avoid overflow in X. If matrix B
C     has full rank then the solution matrix X will be positive-definite
C     and hence the Cholesky factor U will be nonsingular, but if B is
C     rank deficient then X may be only positive semi-definite and U
C     will be singular.
C
C     In the case of equation (1) the matrix A must be stable (that
C     is, all the eigenvalues of A must have negative real parts),
C     and for equation (2) the matrix A must be convergent (that is,
C     all the eigenvalues of A must lie inside the unit circle).
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     DICO    CHARACTER*1
C             Specifies the type of Lyapunov equation to be solved as
C             follows:
C             = 'C':  Equation (1), continuous-time case;
C             = 'D':  Equation (2), discrete-time case.
C
C     FACT    CHARACTER*1
C             Specifies whether or not the real Schur factorization
C             of the matrix A is supplied on entry, as follows:
C             = 'F':  On entry, A and Q contain the factors from the
C                     real Schur factorization of the matrix A;
C             = 'N':  The Schur factorization of A will be computed
C                     and the factors will be stored in A and Q.
C
C     TRANS   CHARACTER*1
C             Specifies the form of op(K) to be used, as follows:
C             = 'N':  op(K) = K    (No transpose);
C             = 'T':  op(K) = K**T (Transpose).
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix A and the number of columns in
C             matrix op(B).  N >= 0.
C
C     M       (input) INTEGER
C             The number of rows in matrix op(B).  M >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N part of this array must
C             contain the matrix A. If FACT = 'F', then A contains
C             an upper quasi-triangular matrix S in Schur canonical
C             form; the elements below the upper Hessenberg part of the
C             array A are not referenced.
C             On exit, the leading N-by-N upper Hessenberg part of this
C             array contains the upper quasi-triangular matrix S in
C             Schur canonical form from the Shur factorization of A.
C             The contents of array A is not modified if FACT = 'F'.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     Q       (input or output) DOUBLE PRECISION array, dimension
C             (LDQ,N)
C             On entry, if FACT = 'F', then the leading N-by-N part of
C             this array must contain the orthogonal matrix Q of the
C             Schur factorization of A.
C             Otherwise, Q need not be set on entry.
C             On exit, the leading N-by-N part of this array contains
C             the orthogonal matrix Q of the Schur factorization of A.
C             The contents of array Q is not modified if FACT = 'F'.
C
C     LDQ     INTEGER
C             The leading dimension of array Q.  LDQ >= MAX(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             if TRANS = 'N', and dimension (LDB,max(M,N)), if
C             TRANS = 'T'.
C             On entry, if TRANS = 'N', the leading M-by-N part of this
C             array must contain the coefficient matrix B of the
C             equation.
C             On entry, if TRANS = 'T', the leading N-by-M part of this
C             array must contain the coefficient matrix B of the
C             equation.
C             On exit, the leading N-by-N upper triangular part of this
C             array contains the Cholesky factor of the solution matrix
C             X of the problem, X = op(U)'*op(U).
C
C     LDB     INTEGER
C             The leading dimension of array B.
C             LDB >= MAX(1,N,M), if TRANS = 'N';
C             LDB >= MAX(1,N),   if TRANS = 'T'.
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     WR      (output) DOUBLE PRECISION array, dimension (N)
C     WI      (output) DOUBLE PRECISION array, dimension (N)
C             If FACT = 'N', and INFO >= 0 and INFO <= 2, WR and WI
C             contain the real and imaginary parts, respectively, of
C             the eigenvalues of A.
C             If FACT = 'F', WR and WI are not referenced.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, or INFO = 1, DWORK(1) returns the
C             optimal value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK = MAX(1,4*N + MIN(M,N)).
C             For optimum performance LDWORK should sometimes be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the Lyapunov equation is (nearly) singular
C                   (warning indicator);
C                   if DICO = 'C' this means that while the matrix A
C                   (or the factor S) has computed eigenvalues with
C                   negative real parts, it is only just stable in the
C                   sense that small perturbations in A can make one or
C                   more of the eigenvalues have a non-negative real
C                   part;
C                   if DICO = 'D' this means that while the matrix A
C                   (or the factor S) has computed eigenvalues inside
C                   the unit circle, it is nevertheless only just
C                   convergent, in the sense that small perturbations
C                   in A can make one or more of the eigenvalues lie
C                   outside the unit circle;
C                   perturbed values were used to solve the equation;
C             = 2:  if FACT = 'N' and DICO = 'C', but the matrix A is
C                   not stable (that is, one or more of the eigenvalues
C                   of A has a non-negative real part), or DICO = 'D',
C                   but the matrix A is not convergent (that is, one or
C                   more of the eigenvalues of A lies outside the unit
C                   circle); however, A will still have been factored
C                   and the eigenvalues of A returned in WR and WI.
C             = 3:  if FACT = 'F' and DICO = 'C', but the Schur factor S
C                   supplied in the array A is not stable (that is, one
C                   or more of the eigenvalues of S has a non-negative
C                   real part), or DICO = 'D', but the Schur factor S
C                   supplied in the array A is not convergent (that is,
C                   one or more of the eigenvalues of S lies outside the
C                   unit circle);
C             = 4:  if FACT = 'F' and the Schur factor S supplied in
C                   the array A has two or more consecutive non-zero
C                   elements on the first sub-diagonal, so that there is
C                   a block larger than 2-by-2 on the diagonal;
C             = 5:  if FACT = 'F' and the Schur factor S supplied in
C                   the array A has a 2-by-2 diagonal block with real
C                   eigenvalues instead of a complex conjugate pair;
C             = 6:  if FACT = 'N' and the LAPACK Library routine DGEES
C                   has failed to converge. This failure is not likely
C                   to occur. The matrix B will be unaltered but A will
C                   be destroyed.
C
C     METHOD
C
C     The method used by the routine is based on the Bartels and Stewart
C     method [1], except that it finds the upper triangular matrix U
C     directly without first finding X and without the need to form the
C     normal matrix op(B)'*op(B).
C
C     The Schur factorization of a square matrix A is given by
C
C        A = QSQ',
C
C     where Q is orthogonal and S is an N-by-N block upper triangular
C     matrix with 1-by-1 and 2-by-2 blocks on its diagonal (which
C     correspond to the eigenvalues of A). If A has already been
C     factored prior to calling the routine however, then the factors
C     Q and S may be supplied and the initial factorization omitted.
C
C     If TRANS = 'N', the matrix B is factored as (QR factorization)
C            _   _                   _   _  _
C        B = P ( R ),  M >= N,   B = P ( R  Z ),  M < N,
C              ( 0 )
C           _                                    _
C     where P is an M-by-M orthogonal matrix and R is a square upper
C                                         _   _      _     _  _
C     triangular matrix. Then, the matrix B = RQ, or B = ( R  Z )Q (if
C     M < N) is factored as
C        _                       _
C        B = P ( R ),  M >= N,   B = P ( R  Z ),  M < N.
C
C     If TRANS = 'T', the matrix B is factored as (RQ factorization)
C                                         _
C                 _   _                 ( Z ) _
C        B = ( 0  R ) P,  M >= N,   B = ( _ ) P,  M < N,
C                                       ( R )
C           _                                    _
C     where P is an M-by-M orthogonal matrix and R is a square upper
C                                         _     _     _       _   _
C     triangular matrix. Then, the matrix B = Q'R, or B = Q'( Z'  R' )'
C     (if M < N) is factored as
C        _                       _
C        B = ( R ) P,  M >= N,   B = ( Z ) P,  M < N.
C                                    ( R )
C
C     These factorizations are utilised to either transform the
C     continuous-time Lyapunov equation to the canonical form
C                                                        2
C       op(S)'*op(V)'*op(V) + op(V)'*op(V)*op(S) = -scale *op(F)'*op(F),
C
C     or the discrete-time Lyapunov equation to the canonical form
C                                                        2
C       op(S)'*op(V)'*op(V)*op(S) - op(V)'*op(V) = -scale *op(F)'*op(F),
C
C     where V and F are upper triangular, and
C
C        F = R,  M >= N,   F = ( R  Z ),  M < N, if TRANS = 'N';
C                              ( 0  0 )
C
C        F = R,  M >= N,   F = ( 0  Z ),  M < N, if TRANS = 'T'.
C                              ( 0  R )
C
C     The transformed equation is then solved for V, from which U is
C     obtained via the QR factorization of V*Q', if TRANS = 'N', or
C     via the RQ factorization of Q*V, if TRANS = 'T'.
C
C     REFERENCES
C
C     [1] Bartels, R.H. and Stewart, G.W.
C         Solution of the matrix equation  A'X + XB = C.
C         Comm. A.C.M., 15, pp. 820-826, 1972.
C
C     [2] Hammarling, S.J.
C         Numerical solution of the stable, non-negative definite
C         Lyapunov equation.
C         IMA J. Num. Anal., 2, pp. 303-325, 1982.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations and is backward stable.
C
C     FURTHER COMMENTS
C
C     The Lyapunov equation may be very ill-conditioned. In particular,
C     if A is only just stable (or convergent) then the Lyapunov
C     equation will be ill-conditioned.  A symptom of ill-conditioning
C     is "large" elements in U relative to those of A and B, or a
C     "small" value for scale. A condition estimate can be computed
C     using SLICOT Library routine SB03MD.
C
C     SB03OD routine can be also used for solving "unstable" Lyapunov
C     equations, i.e., when matrix A has all eigenvalues with positive
C     real parts, if DICO = 'C', or with moduli greater than one,
C     if DICO = 'D'. Specifically, one may solve for X = op(U)'*op(U)
C     either the continuous-time Lyapunov equation
C                                  2
C        op(A)'*X + X*op(A) = scale *op(B)'*op(B),                   (3)
C
C     or the discrete-time Lyapunov equation
C                                  2
C        op(A)'*X*op(A) - X = scale *op(B)'*op(B),                   (4)
C
C     provided, for equation (3), the given matrix A is replaced by -A,
C     or, for equation (4), the given matrices A and B are replaced by
C     inv(A) and B*inv(A), if TRANS = 'N' (or inv(A)*B, if TRANS = 'T'),
C     respectively. Although the inversion generally can rise numerical
C     problems, in case of equation (4) it is expected that the matrix A
C     is enough well-conditioned, having only eigenvalues with moduli 
C     greater than 1. However, if A is ill-conditioned, it could be
C     preferable to use the more general SLICOT Lyapunov solver SB03MD.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C     Supersedes Release 2.0 routine SB03CD by Sven Hammarling,
C     NAG Ltd, United Kingdom.
C
C     REVISIONS
C
C     Dec. 1997, April 1998, May 1998, May 1999.
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation, real Schur form,
C     Sylvester equation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         DICO, FACT, TRANS
      INTEGER           INFO, LDA, LDB, LDQ, LDWORK, M, N
      DOUBLE PRECISION  SCALE
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*), Q(LDQ,*), WI(*), 
     $                  WR(*)
C     .. Local Scalars ..
      LOGICAL           CONT, LTRANS, NOFACT
      INTEGER           I, IFAIL, INFORM, ITAU, J, JWORK, K, L, MINMN,
     $                  NE, SDIM, WRKOPT
      DOUBLE PRECISION  EMAX, TEMP
C     .. Local Arrays ..
      LOGICAL           BWORK(1)
C     .. External Functions ..
      LOGICAL           LSAME, SELECT1
      DOUBLE PRECISION  DLAPY2
      EXTERNAL          DLAPY2, LSAME, SELECT1
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEES, DGEMM, DGEMV, DGEQRF, DGERQF,
     $                  DLACPY, DTRMM, SB03OU, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         INT, MAX, MIN
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      CONT   = LSAME( DICO,  'C' )
      NOFACT = LSAME( FACT,  'N' )
      LTRANS = LSAME( TRANS, 'T' )
      MINMN  = MIN( M, N )
C
      INFO = 0
      IF( .NOT.CONT .AND. .NOT.LSAME( DICO, 'D' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -2
      ELSE IF( .NOT.LTRANS .AND. .NOT.LSAME( TRANS, 'N' ) ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( M.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDQ.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( ( LDB.LT.MAX( 1, N ) )  .OR. 
     $         ( LDB.LT.MAX( 1, N, M ) .AND. .NOT.LTRANS ) ) THEN
         INFO = -11
      ELSE IF( LDWORK.LT.MAX( 1, 4*N + MINMN ) ) THEN
         INFO = -16
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB03OD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( MINMN.EQ.0 ) THEN
         SCALE = ONE
         DWORK(1) = ONE
         RETURN
      END IF
C
C     Start the solution.
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C
      IF ( NOFACT ) THEN
C
C        Find the Schur factorization of A,   A = Q*S*Q'.
C        Workspace:  need   3*N;
C                    prefer larger.
C
         CALL DGEES( 'Vectors', 'Not ordered', SELECT1, N, A, LDA, SDIM,
     $               WR, WI, Q, LDQ, DWORK, LDWORK, BWORK, INFORM )
         IF ( INFORM.NE.0 ) THEN
            INFO = 6
            RETURN
         END IF
         WRKOPT = DWORK(1)
C
C        Check the eigenvalues for stability.
C
         IF ( CONT ) THEN
            EMAX = WR(1)
C
            DO 20 J = 2, N
               IF ( WR(J).GT.EMAX )
     $            EMAX = WR(J)
   20       CONTINUE
C
         ELSE
            EMAX = DLAPY2( WR(1), WI(1) )
C
            DO 40 J = 2, N
               TEMP = DLAPY2( WR(J), WI(J) )
               IF ( TEMP.GT.EMAX )
     $            EMAX = TEMP
   40       CONTINUE
C
         END IF
C
         IF (    ( CONT ) .AND. ( EMAX.GE.ZERO ) .OR.
     $      ( .NOT.CONT ) .AND. ( EMAX.GE.ONE  ) ) THEN
            INFO = 2
            RETURN
         END IF
      END IF
C
C     Perform the QR or RQ factorization of B,
C            _   _           _   _  _
C        B = P ( R ), or B = P ( R  Z ), if TRANS = 'N', or
C              ( 0 )
C                                 _
C                 _   _         ( Z ) _
C        B = ( 0  R ) P, or B = ( _ ) P, if TRANS = 'T'.
C                               ( R )
C     Workspace:  need   MIN(M,N) + N;
C                 prefer MIN(M,N) + N*NB.
C
      ITAU  = 1
      JWORK = ITAU + MINMN
      IF ( LTRANS ) THEN
         CALL DGERQF( N, M, B, LDB, DWORK(ITAU), DWORK(JWORK),
     $                LDWORK-JWORK+1, IFAIL )
         JWORK = ITAU
C
C        Form in B
C        _      _              _         _   _                    _
C        B := Q'R,   m >= n,   B := Q'*( Z'  R' )',   m < n, with B an
C        n-by-min(m,n) matrix.
C        Use a BLAS 3 operation if enough workspace, and BLAS 2,
C                   _
C        otherwise: B is formed column by column.
C
         IF ( LDWORK.GE.JWORK+MINMN*N-1 ) THEN
            K = JWORK
C
            DO 60 I = 1, MINMN
               CALL DCOPY( N, Q(N-MINMN+I,1), LDQ, DWORK(K), 1 )
               K = K + N
   60       CONTINUE
C
            CALL DTRMM( 'Right', 'Upper', 'No transpose', 'Non-unit',
     $                  N, MINMN, ONE, B(N-MINMN+1,M-MINMN+1), LDB,
     $                  DWORK(JWORK), N )
            IF ( M.LT.N )
     $         CALL DGEMM( 'Transpose', 'No transpose', N, M, N-M,
     $                     ONE, Q, LDQ, B, LDB, ONE, DWORK(JWORK), N )
            CALL DLACPY( 'Full', N, MINMN, DWORK(JWORK), N, B, LDB )
         ELSE
            NE = N - MINMN
C
            DO 80 J = 1, MINMN
               NE = NE + 1
               CALL DCOPY( NE, B(1,M-MINMN+J), 1, DWORK(JWORK), 1 )
               CALL DGEMV( 'Transpose', NE, N, ONE, Q, LDQ,
     $                     DWORK(JWORK), 1, ZERO, B(1,J), 1 )
   80       CONTINUE
C
         END IF
      ELSE
         CALL DGEQRF( M, N, B, LDB, DWORK(ITAU), DWORK(JWORK),
     $                LDWORK-JWORK+1, IFAIL )
         JWORK = ITAU
C
C        Form in B
C        _    _               _      _  _                    _
C        B := RQ,   m >= n,   B := ( R  Z )*Q,   m < n, with B an
C        min(m,n)-by-n matrix.
C        Use a BLAS 3 operation if enough workspace, and BLAS 2,
C                   _
C        otherwise: B is formed row by row.
C
         IF ( LDWORK.GE.JWORK+MINMN*N-1 ) THEN
            CALL DLACPY( 'Full', MINMN, N, Q, LDQ, DWORK(JWORK), MINMN )
            CALL DTRMM( 'Left', 'Upper', 'No transpose', 'Non-unit',
     $                  MINMN, N, ONE, B, LDB, DWORK(JWORK), MINMN )
            IF ( M.LT.N )
     $         CALL DGEMM( 'No transpose', 'No transpose', M, N, N-M,
     $                     ONE, B(1,M+1), LDB, Q(M+1,1), LDQ, ONE,
     $                     DWORK(JWORK), MINMN )
            CALL DLACPY( 'Full', MINMN, N, DWORK(JWORK), MINMN, B, LDB )
         ELSE
            NE = MINMN + MAX( 0, N-M )
C
            DO 100 J = 1, MINMN
               CALL DCOPY( NE, B(J,J), LDB, DWORK(JWORK), 1 )
               CALL DGEMV( 'Transpose', NE, N, ONE, Q(J,1), LDQ,
     $                     DWORK(JWORK), 1, ZERO, B(J,1), LDB )
               NE = NE - 1
  100       CONTINUE
C
         END IF
      END IF
      WRKOPT = MAX( WRKOPT, INT( DWORK(JWORK) ) + JWORK - 1, MINMN*N )
      JWORK  = ITAU + MINMN
C
C     Solve for U the transformed Lyapunov equation
C                                                      2    _      _
C     op(S)'*op(U)'*op(U) + op(U)'*op(U)*op(S) = -scale *op(B)'*op(B),
C
C     or
C                                                      2    _      _
C     op(S)'*op(U)'*op(U)*op(S) - op(U)'*op(U) = -scale *op(B)'*op(B)
C
C     Workspace:  need   MIN(M,N) + 4*N;
C                 prefer larger.
C
      CALL SB03OU( .NOT.CONT, LTRANS, N, MINMN, A, LDA, B, LDB,
     $             DWORK(ITAU), B, LDB, SCALE, DWORK(JWORK),
     $             LDWORK-JWORK+1, INFO )
      IF ( INFO.GT.1 ) THEN
         INFO = INFO + 1
         RETURN
      END IF
      WRKOPT = MAX( WRKOPT, INT( DWORK(JWORK) ) + JWORK - 1 )
      JWORK  = ITAU
C
C     Form   U :=  U*Q' or U := Q*U in the array B.
C     Use a BLAS 3 operation if enough workspace, and BLAS 2, otherwise.
C     Workspace:  need   N;
C                 prefer N*N;
C
      IF ( LDWORK.GE.JWORK+N*N-1 ) THEN
         IF ( LTRANS ) THEN
            CALL DLACPY( 'Full', N, N, Q, LDQ, DWORK(JWORK), N )
            CALL DTRMM( 'Right', 'Upper', 'No transpose', 'Non-unit', N,
     $                  N, ONE, B, LDB, DWORK(JWORK), N )
         ELSE
            K = JWORK
C
            DO 120 I = 1, N
               CALL DCOPY( N, Q(1,I), 1, DWORK(K), N )
               K = K + 1
  120       CONTINUE
C
            CALL DTRMM( 'Left', 'Upper', 'No transpose', 'Non-unit', N,
     $                  N, ONE, B, LDB, DWORK(JWORK), N )
         END IF
         CALL DLACPY( 'Full', N, N, DWORK(JWORK), N, B, LDB )
         WRKOPT = MAX( WRKOPT, JWORK + N*N - 1 )
      ELSE
         IF ( LTRANS ) THEN
C
C           U is formed column by column ( U := Q*U ).
C
            DO 140 I = 1, N
               CALL DCOPY( I, B(1,I), 1, DWORK(JWORK), 1 )
               CALL DGEMV( 'No transpose', N, I, ONE, Q, LDQ,
     $                     DWORK(JWORK), 1, ZERO, B(1,I), 1 )
  140       CONTINUE
         ELSE
C
C           U is formed row by row ( U' := Q*U' ).
C
            DO 160 I = 1, N
               CALL DCOPY( N-I+1, B(I,I), LDB, DWORK(JWORK), 1 )
               CALL DGEMV( 'No transpose', N, N-I+1, ONE, Q(1,I), LDQ,
     $                     DWORK(JWORK), 1, ZERO, B(I,1), LDB )
  160       CONTINUE
         END IF
      END IF
C
C     Lastly find the QR or RQ factorization of U, overwriting on B,
C     to give the required Cholesky factor.
C     Workspace:  need   2*N;
C                 prefer N + N*NB;
C
      JWORK = ITAU + N
      IF ( LTRANS ) THEN
         CALL DGERQF( N, N, B, LDB, DWORK(ITAU), DWORK(JWORK),
     $                LDWORK-JWORK+1, IFAIL )
      ELSE
         CALL DGEQRF( N, N, B, LDB, DWORK(ITAU), DWORK(JWORK),
     $                LDWORK-JWORK+1, IFAIL )
      END IF
      WRKOPT = MAX( WRKOPT, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C     Make the diagonal elements of U non-negative.
C
      IF ( LTRANS ) THEN
C
         DO 200 J = 1, N
            IF ( B(J,J).LT.ZERO ) THEN
C
               DO 180 I = 1, J
                  B(I,J) = -B(I,J)
  180          CONTINUE
C
            END IF
  200    CONTINUE
C
      ELSE
         K = JWORK
C
         DO 240 J = 1, N
            DWORK(K) = B(J,J)
            L = JWORK
C
            DO 220 I = 1, J
               IF ( DWORK(L).LT.ZERO ) B(I,J) = -B(I,J)
               L = L + 1
  220       CONTINUE
C
            K = K + 1
  240    CONTINUE
      END IF
C
C     Set the optimal workspace.
C
      DWORK(1) = WRKOPT
C
      RETURN
C *** Last line of SB03OD ***
      END
