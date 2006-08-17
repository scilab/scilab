      SUBROUTINE SB02QD( JOB, FACT, TRANA, UPLO, LYAPUN, N, A, LDA, T,
     $                   LDT, U, LDU, G, LDG, Q, LDQ, X, LDX, SEP,
     $                   RCOND, FERR, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To estimate the conditioning and compute an error bound on the
C     solution of the real continuous-time matrix algebraic Riccati
C     equation
C
C         op(A)'*X + X*op(A) + Q - X*G*X = 0,                        (1)
C
C     where op(A) = A or A' (A**T) and Q, G are symmetric (Q = Q**T,
C     G = G**T). The matrices A, Q and G are N-by-N and the solution X
C     is N-by-N.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOB     CHARACTER*1
C             Specifies the computation to be performed, as follows:
C             = 'C':  Compute the reciprocal condition number only;
C             = 'E':  Compute the error bound only;
C             = 'B':  Compute both the reciprocal condition number and 
C                     the error bound.
C
C     FACT    CHARACTER*1
C             Specifies whether or not the real Schur factorization of
C             the matrix Ac = A - G*X (if TRANA = 'N') or Ac = A - X*G
C             (if TRANA = 'T' or 'C') is supplied on entry, as follows:
C             = 'F':  On entry, T and U (if LYAPUN = 'O') contain the
C                     factors from the real Schur factorization of the
C                     matrix Ac;
C             = 'N':  The Schur factorization of Ac will be computed
C                     and the factors will be stored in T and U (if
C                     LYAPUN = 'O').
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     UPLO    CHARACTER*1
C             Specifies which part of the symmetric matrices Q and G is
C             to be used, as follows:
C             = 'U':  Upper triangular part;
C             = 'L':  Lower triangular part.
C
C     LYAPUN  CHARACTER*1
C             Specifies whether or not the original Lyapunov equations 
C             should be solved in the iterative estimation process, 
C             as follows:
C             = 'O':  Solve the original Lyapunov equations, updating
C                     the right-hand sides and solutions with the
C                     matrix U, e.g., RHS <-- U'*RHS*U;
C             = 'R':  Solve reduced Lyapunov equations only, without
C                     updating the right-hand sides and solutions.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, X, Q, and G.  N >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             If FACT = 'N' or LYAPUN = 'O', the leading N-by-N part of
C             this array must contain the matrix A.
C             If FACT = 'F' and LYAPUN = 'R', A is not referenced.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  
C             LDA >= max(1,N), if FACT = 'N' or  LYAPUN = 'O';
C             LDA >= 1,        if FACT = 'F' and LYAPUN = 'R'.
C
C     T       (input or output) DOUBLE PRECISION array, dimension
C             (LDT,N)
C             If FACT = 'F', then T is an input argument and on entry,
C             the leading N-by-N upper Hessenberg part of this array
C             must contain the upper quasi-triangular matrix T in Schur
C             canonical form from a Schur factorization of Ac (see
C             argument FACT).
C             If FACT = 'N', then T is an output argument and on exit,
C             if INFO = 0 or INFO = N+1, the leading N-by-N upper
C             Hessenberg part of this array contains the upper quasi-
C             triangular matrix T in Schur canonical form from a Schur
C             factorization of Ac (see argument FACT).
C
C     LDT     INTEGER
C             The leading dimension of the array T.  LDT >= max(1,N).
C
C     U       (input or output) DOUBLE PRECISION array, dimension
C             (LDU,N)
C             If LYAPUN = 'O' and FACT = 'F', then U is an input
C             argument and on entry, the leading N-by-N part of this
C             array must contain the orthogonal matrix U from a real
C             Schur factorization of Ac (see argument FACT).
C             If LYAPUN = 'O' and FACT = 'N', then U is an output
C             argument and on exit, if INFO = 0 or INFO = N+1, it
C             contains the orthogonal N-by-N matrix from a real Schur
C             factorization of Ac (see argument FACT).
C             If LYAPUN = 'R', the array U is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of the array U.
C             LDU >= 1,        if LYAPUN = 'R'; 
C             LDU >= MAX(1,N), if LYAPUN = 'O'.
C
C     G       (input) DOUBLE PRECISION array, dimension (LDG,N)
C             If UPLO = 'U', the leading N-by-N upper triangular part of
C             this array must contain the upper triangular part of the
C             matrix G.
C             If UPLO = 'L', the leading N-by-N lower triangular part of
C             this array must contain the lower triangular part of the
C             matrix G.                     _
C             Matrix G should correspond to G in the "reduced" Riccati
C             equation (with matrix T, instead of A), if LYAPUN = 'R'.
C             See METHOD.
C
C     LDG     INTEGER
C             The leading dimension of the array G.  LDG >= max(1,N).
C
C     Q       (input) DOUBLE PRECISION array, dimension (LDQ,N)
C             If UPLO = 'U', the leading N-by-N upper triangular part of
C             this array must contain the upper triangular part of the
C             matrix Q.
C             If UPLO = 'L', the leading N-by-N lower triangular part of
C             this array must contain the lower triangular part of the
C             matrix Q.                     _
C             Matrix Q should correspond to Q in the "reduced" Riccati
C             equation (with matrix T, instead of A), if LYAPUN = 'R'.
C             See METHOD.
C
C     LDQ     INTEGER
C             The leading dimension of the array Q.  LDQ >= max(1,N).
C
C     X       (input) DOUBLE PRECISION array, dimension (LDX,N)
C             The leading N-by-N part of this array must contain the
C             symmetric solution matrix of the original Riccati
C             equation (with matrix A), if LYAPUN = 'O', or of the
C             "reduced" Riccati equation (with matrix T), if
C             LYAPUN = 'R'. See METHOD.
C
C     LDX     INTEGER
C             The leading dimension of the array X.  LDX >= max(1,N).
C
C     SEP     (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'B', the estimated quantity 
C             sep(op(Ac),-op(Ac)').
C             If N = 0, or X = 0, or JOB = 'E', SEP is not referenced.
C
C     RCOND   (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'B', an estimate of the reciprocal
C             condition number of the continuous-time Riccati equation.
C             If N = 0 or X = 0, RCOND is set to 1 or 0, respectively.
C             If JOB = 'E', RCOND is not referenced.
C
C     FERR    (output) DOUBLE PRECISION
C             If JOB = 'E' or JOB = 'B', an estimated forward error
C             bound for the solution X. If XTRUE is the true solution,
C             FERR bounds the magnitude of the largest entry in
C             (X - XTRUE) divided by the magnitude of the largest entry
C             in X.
C             If N = 0 or X = 0, FERR is set to 0.
C             If JOB = 'C', FERR is not referenced.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0 or INFO = N+1, DWORK(1) returns the
C             optimal value of LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             Let LWA = N*N, if LYAPUN = 'O' and JOB = 'E' or 'B';
C                 LWA = 0,   otherwise.
C             If FACT = 'N', then
C                LDWORK  = MAX(1, 5*N, 2*N*N),        if JOB = 'C';
C                LDWORK  = MAX(1, LWA + 5*N, 4*N*N ), if JOB = 'E', 'B'.
C             If FACT = 'F', then
C                LDWORK  = MAX(1, 2*N*N),  if JOB = 'C';
C                LDWORK  = MAX(1, 4*N*N ), if JOB = 'E' or 'B'.
C             For good performance, LDWORK must generally be larger.
C
C     Error indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             > 0:  if INFO = i, i <= N, the QR algorithm failed to
C                   complete the reduction of the matrix Ac to Schur
C                   canonical form (see LAPACK Library routine DGEES);
C                   on exit, the matrix T(i+1:N,i+1:N) contains the
C                   partially converged Schur form, and DWORK(i+1:N) and
C                   DWORK(N+i+1:2*N) contain the real and imaginary
C                   parts, respectively, of the converged eigenvalues;
C                   this error is unlikely to appear;
C             = N+1:  if the matrices T and -T' have common or very
C                   close eigenvalues; perturbed values were used to
C                   solve Lyapunov equations, but the matrix T, if given
C                   (for FACT = 'F'), is unchanged.
C
C     METHOD
C
C     The condition number of the Riccati equation is estimated as
C
C     cond = ( norm(Theta)*norm(A) + norm(inv(Omega))*norm(Q) +
C                 norm(Pi)*norm(G) ) / norm(X),
C
C     where Omega, Theta and Pi are linear operators defined by
C
C     Omega(W) = op(Ac)'*W + W*op(Ac),
C     Theta(W) = inv(Omega(op(W)'*X + X*op(W))),
C        Pi(W) = inv(Omega(X*W*X)),
C
C     and Ac = A - G*X (if TRANA = 'N') or Ac = A - X*G (if TRANA = 'T'
C     or 'C'). Note that the Riccati equation (1) is equivalent to
C                _   _         _   _ _ _
C         op(T)'*X + X*op(T) + Q + X*G*X = 0,                        (2)
C           _           _               _
C     where X = U'*X*U, Q = U'*Q*U, and G = U'*G*U, with U the
C     orthogonal matrix reducing Ac to a real Schur form, T = U'*Ac*U.
C
C     The routine estimates the quantities
C
C     sep(op(Ac),-op(Ac)') = 1 / norm(inv(Omega)),
C
C     norm(Theta) and norm(Pi) using 1-norm condition estimator.
C
C     The forward error bound is estimated using a practical error bound
C     similar to the one proposed in [2].
C
C     REFERENCES
C
C     [1] Ghavimi, A.R. and Laub, A.J.
C         Backward error, sensitivity, and refinement of computed
C         solutions of algebraic Riccati equations.
C         Numerical Linear Algebra with Applications, vol. 2, pp. 29-49,
C         1995.
C
C     [2] Higham, N.J.
C         Perturbation theory and backward error for AX-XB=C.
C         BIT, vol. 33, pp. 124-136, 1993.
C
C     [3] Petkov, P.Hr., Konstantinov, M.M., and Mehrmann, V.
C         DGRSVX and DMSRIC: Fortran 77 subroutines for solving
C         continuous-time matrix algebraic Riccati equations with
C         condition and accuracy estimates.
C         Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
C         Chemnitz, May 1998. 
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations.
C     The accuracy of the estimates obtained depends on the solution
C     accuracy and on the properties of the 1-norm estimator.
C
C     FURTHER COMMENTS
C
C     The option LYAPUN = 'R' may occasionally produce slightly worse
C     or better estimates, and it is much faster than the option 'O'.
C     When SEP is computed and it is zero, the routine returns
C     immediately, with RCOND and FERR (if requested) set to 0 and 1,
C     respectively. In this case, the equation is singular.
C
C     CONTRIBUTOR
C
C     P.Hr. Petkov, Technical University of Sofia, December 1998.
C     V. Sima, Katholieke Univ. Leuven, Belgium, February 1999. 
C
C     REVISIONS
C
C     -   
C
C     KEYWORDS
C
C     Conditioning, error estimates, orthogonal transformation,
C     real Schur form, Riccati equation.
C 
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, FOUR
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0, 
     $                     FOUR = 4.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          FACT, JOB, LYAPUN, TRANA, UPLO
      INTEGER            INFO, LDA, LDG, LDQ, LDT, LDU, LDWORK, LDX, N
      DOUBLE PRECISION   FERR, RCOND, SEP
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), DWORK( * ),  G( LDG, * ),
     $                   Q( LDQ, * ), T( LDT, * ), U( LDU, * ),
     $                   X( LDX, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            JOBB, JOBC, JOBE, LOWER, NEEDAC, NOFACT,
     $                   NOTRNA, UPDATE
      CHARACTER          LOUP, SJOB, TRANAT
      INTEGER            I, IABS, INFO2, IRES, ITMP, IXBS, J, JJ, JX,
     $                   KASE, LDW, LWA, NN, SDIM, WRKOPT
      DOUBLE PRECISION   ANORM, BIGNUM, DENOM, EPS, EPSN, EST, GNORM,
     $                   PINORM, QNORM, SCALE, SIG, TEMP, THNORM, TMAX,
     $                   XANORM, XNORM
C     ..
C     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
C     ..
C     .. External Functions ..
      LOGICAL            LSAME, SELECT1
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANHS, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANHS, DLANSY, LSAME, SELECT1
C     ..
C     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGEES, DLACON, DLACPY, DSYMM,
     $                   DSYR2K, MA02ED, MB01RU, MB01UD, SB03MY, SB03QX,
     $                   SB03QY, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, INT, MAX, MIN
C     ..
C     .. Executable Statements ..      
C
C     Decode and Test input parameters.
C
      JOBC   = LSAME( JOB,    'C' )
      JOBE   = LSAME( JOB,    'E' )
      JOBB   = LSAME( JOB,    'B' )
      NOFACT = LSAME( FACT,   'N' )
      NOTRNA = LSAME( TRANA,  'N' )
      LOWER  = LSAME( UPLO,   'L' )
      UPDATE = LSAME( LYAPUN, 'O' )
C
      NEEDAC = UPDATE .AND. .NOT.JOBC
C
      NN = N*N
      IF( NEEDAC ) THEN
         LWA = NN
      ELSE
         LWA = 0
      END IF
C
      IF( NOFACT ) THEN
         IF( JOBC ) THEN
            LDW = MAX( 5*N, 2*NN )
         ELSE
            LDW = MAX( LWA + 5*N, 4*NN )
         END IF
      ELSE
         IF( JOBC ) THEN
            LDW = 2*NN
         ELSE
            LDW = 4*NN
         END IF
      END IF
C
      INFO = 0
      IF( .NOT.( JOBB .OR. JOBC .OR. JOBE ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.( NOFACT .OR. LSAME( FACT,   'F' ) ) ) THEN
         INFO = -2           
      ELSE IF( .NOT.( NOTRNA .OR. LSAME( TRANA,  'T' ) .OR.
     $                            LSAME( TRANA,  'C' ) ) ) THEN
         INFO = -3
      ELSE IF( .NOT.( LOWER  .OR. LSAME( UPLO,   'U' ) ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( UPDATE .OR. LSAME( LYAPUN, 'R' ) ) ) THEN
         INFO = -5          
      ELSE IF( N.LT.0 ) THEN
         INFO = -6
      ELSE IF( LDA.LT.1 .OR.  
     $       ( LDA.LT.N .AND. ( UPDATE .OR. NOFACT ) ) ) THEN
         INFO = -8
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.1 .OR. ( LDU.LT.N .AND. UPDATE ) ) THEN
         INFO = -12
      ELSE IF( LDG.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( LDQ.LT.MAX( 1, N ) ) THEN
         INFO = -16
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -18
      ELSE IF( LDWORK.LT.MAX( 1, LDW ) ) THEN
         INFO = -24
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB02QD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 ) THEN
         IF( .NOT.JOBE )
     $      RCOND = ONE
         IF( .NOT.JOBC )
     $      FERR  = ZERO
         DWORK( 1 ) = ONE
         RETURN
      END IF
C
C     Compute the 1-norm of the matrix X.
C
      XNORM = DLANSY( '1-norm', UPLO, N, X, LDX, DWORK )
      IF( XNORM.EQ.ZERO ) THEN
C
C        The solution is zero.
C
         IF( .NOT.JOBE )
     $      RCOND = ZERO
         IF( .NOT.JOBC )
     $      FERR  = ZERO
         DWORK( 1 ) = DBLE( N )
         RETURN
      END IF
C
C     Workspace usage.
C
      IXBS = 0
      ITMP = IXBS + NN
      IABS = ITMP + NN
      IRES = IABS + NN
C
C     Workspace:  LWR, where
C                 LWR = N*N, if LYAPUN = 'O' and JOB = 'E' or 'B', or
C                               FACT = 'N',
C                 LWR = 0,   otherwise.
C
      IF( NEEDAC .OR. NOFACT ) THEN
C
         CALL DLACPY( 'Full', N, N, A, LDA, DWORK, N )
         IF( NOTRNA ) THEN
C
C           Compute Ac = A - G*X.
C
            CALL DSYMM( 'Left', UPLO, N, N, -ONE, G, LDG, X, LDX, ONE,
     $                  DWORK, N )
         ELSE
C
C           Compute Ac = A - X*G.
C
            CALL DSYMM( 'Right', UPLO, N, N, -ONE, G, LDG, X, LDX, ONE,
     $                  DWORK, N )
         END IF
C
         WRKOPT = DBLE( NN )
         IF( NOFACT )
     $      CALL DLACPY( 'Full', N, N, DWORK, N, T, LDT )
      ELSE
         WRKOPT = DBLE( N )
      END IF
C
      IF( NOFACT ) THEN
C
C        Compute the Schur factorization of Ac, Ac = U*T*U'.
C        Workspace:  need   LWA + 5*N;
C                    prefer larger;
C                    LWA = N*N, if LYAPUN = 'O' and JOB = 'E' or 'B';
C                    LWA = 0,   otherwise.
C        (Note: Comments in the code beginning "Workspace:" describe the
C        minimal amount of real workspace needed at that point in the
C        code, as well as the preferred amount for good performance.)
C
         IF( UPDATE ) THEN
            SJOB = 'V'
         ELSE
            SJOB = 'N'
         END IF
         CALL DGEES( SJOB, 'Not ordered', SELECT1, N, T, LDT, SDIM,
     $               DWORK( LWA+1 ), DWORK( LWA+N+1 ), U, LDU,
     $               DWORK( LWA+2*N+1 ), LDWORK-LWA-2*N, BWORK, INFO )
         IF( INFO.GT.0 ) THEN
            IF( LWA.GT.0 )
     $         CALL DCOPY( 2*N, DWORK( LWA+1 ), 1, DWORK, 1 )
            RETURN
         END IF
C
         WRKOPT = MAX( WRKOPT, INT( DWORK( LWA+2*N+1 ) ) + LWA + 2*N )
      END IF
      IF( NEEDAC )
     $   CALL DLACPY( 'Full', N, N, DWORK, N, DWORK( IABS+1 ), N )
C
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
C
      IF( .NOT.JOBE ) THEN
C
C        Estimate sep(op(Ac),-op(Ac)') = sep(op(T),-op(T)') and
C        norm(Theta).
C        Workspace LWA + 2*N*N.
C        
         CALL SB03QY( 'Both', TRANA, LYAPUN, N, T, LDT, U, LDU, X, LDX,
     $                SEP, THNORM, IWORK, DWORK, LDWORK, INFO )
C
         WRKOPT = MAX( WRKOPT, LWA + 2*NN )
C        
C        Return if the equation is singular.
C        
         IF( SEP.EQ.ZERO ) THEN
            RCOND = ZERO
            IF( JOBB ) 
     $         FERR = ONE
            DWORK( 1 ) = DBLE( WRKOPT )
            RETURN
         END IF
C
C        Estimate norm(Pi).
C        Workspace LWA + 2*N*N.
C
         KASE = 0
C
C        REPEAT
   10    CONTINUE
         CALL DLACON( NN, DWORK( ITMP+1 ), DWORK, IWORK, EST, KASE )
         IF( KASE.NE.0 ) THEN
C
C           Select the triangular part of symmetric matrix to be used.
C
            IF( DLANSY( '1-norm', 'Upper', N, DWORK, N, DWORK( ITMP+1 ))
     $          .GE.
     $          DLANSY( '1-norm', 'Lower', N, DWORK, N, DWORK( ITMP+1 ))
     $        ) THEN
               LOUP = 'U'
            ELSE
               LOUP = 'L'
            END IF
C
C           Compute RHS = X*W*X.
C
            CALL MB01RU( LOUP, 'No Transpose', N, N, ZERO, ONE, DWORK,
     $                   N, X, LDX, DWORK, N, DWORK( ITMP+1 ), NN,
     $                   INFO2 )
C
            IF( UPDATE ) THEN
C
C              Transform the right-hand side: RHS := U'*RHS*U.
C              
               CALL MB01RU( LOUP, 'Transpose', N, N, ZERO, ONE, DWORK,
     $                      N, U, LDU, DWORK, N, DWORK( ITMP+1 ), NN,
     $                      INFO2 )
            END IF
C
C           Fill in the remaining triangle of the symmetric matrix.
C           
            CALL MA02ED( LOUP, N, DWORK, N )
C
            IF( KASE.EQ.1 ) THEN
C
C              Solve op(T)'*Y + Y*op(T) = scale*RHS.
C
               CALL SB03MY( TRANA, N, T, LDT, DWORK, N, SCALE, INFO2 )
            ELSE
C
C              Solve op(T)*W + W*op(T)' = scale*RHS.
C
               CALL SB03MY( TRANAT, N, T, LDT, DWORK, N, SCALE, INFO2 )
            END IF
C
            IF( UPDATE ) THEN
C
C              Transform back to obtain the solution: Z := U*Z*U', with
C              Z = Y or Z = W.
C              
               CALL MB01RU( LOUP, 'No transpose', N, N, ZERO, ONE,
     $                      DWORK, N, U, LDU, DWORK, N, DWORK( ITMP+1 ),
     $                      NN, INFO2 )
C
C              Fill in the remaining triangle of the symmetric matrix.
C
               CALL MA02ED( LOUP, N, DWORK, N )
            END IF
            GO TO 10
         END IF
C        UNTIL KASE = 0
C
         IF( EST.LT.SCALE ) THEN
            PINORM = EST / SCALE
         ELSE
            BIGNUM = ONE / DLAMCH( 'Safe minimum' )
            IF( EST.LT.SCALE*BIGNUM ) THEN
               PINORM = EST / SCALE
            ELSE
               PINORM = BIGNUM
            END IF
         END IF
C
C        Compute the 1-norm of A or T.
C
         IF( UPDATE ) THEN
            ANORM = DLANGE( '1-norm', N, N, A, LDA, DWORK )
         ELSE
            ANORM = DLANHS( '1-norm', N, T, LDT, DWORK )
         END IF
C
C        Compute the 1-norms of the matrices Q and G.
C
         QNORM = DLANSY( '1-norm', UPLO, N, Q, LDQ, DWORK )
         GNORM = DLANSY( '1-norm', UPLO, N, G, LDG, DWORK )
C        
C        Estimate the reciprocal condition number.
C
         TMAX = MAX( SEP, XNORM, ANORM, GNORM )
         IF( TMAX.LE.ONE ) THEN
            TEMP  = SEP*XNORM
            DENOM = QNORM + ( SEP*ANORM )*THNORM +
     $                      ( SEP*GNORM )*PINORM
         ELSE
            TEMP  =   ( SEP / TMAX )*( XNORM / TMAX )
            DENOM = ( ( ONE / TMAX )*( QNORM / TMAX ) ) +
     $              ( ( SEP / TMAX )*( ANORM / TMAX ) )*THNORM +
     $              ( ( SEP / TMAX )*( GNORM / TMAX ) )*PINORM
         END IF
         IF( TEMP.GE.DENOM ) THEN
            RCOND = ONE
         ELSE
            RCOND = TEMP / DENOM
         END IF
      END IF
C
      IF( .NOT.JOBC ) THEN
C
C        Form a triangle of the residual matrix
C          R = op(A)'*X + X*op(A) + Q - X*G*X,
C        or           _   _         _   _ _ _
C          R = op(T)'*X + X*op(T) + Q + X*G*X,
C        exploiting the symmetry.
C        Workspace 4*N*N.
C
         IF( UPDATE ) THEN
            CALL DLACPY( UPLO, N, N, Q, LDQ, DWORK( IRES+1 ), N )
            CALL DSYR2K( UPLO, TRANAT, N, N, ONE, A, LDA, X, LDX, ONE,
     $                   DWORK( IRES+1 ), N )
            SIG = -ONE
         ELSE
            CALL MB01UD( 'Right', TRANA, N, N, ONE, T, LDT, X, LDX,
     $                   DWORK( IRES+1 ), N, INFO2 )
            JJ = IRES + 1
            IF( LOWER ) THEN
               DO 20 J = 1, N
                  CALL DAXPY( N-J+1, ONE, DWORK( JJ ), N, DWORK( JJ ),
     $                        1 )
                  CALL DAXPY( N-J+1, ONE, Q( J, J ), 1, DWORK( JJ ), 1 )
                  JJ = JJ + N + 1
   20          CONTINUE
            ELSE
               DO 30 J = 1, N
                  CALL DAXPY( J, ONE, DWORK( IRES+J ), N, DWORK( JJ ),
     $                        1 )
                  CALL DAXPY( J, ONE, Q( 1, J ), 1, DWORK( JJ ), 1 )
                  JJ = JJ + N
   30          CONTINUE
            END IF
            SIG = ONE
         END IF
         CALL MB01RU( UPLO, TRANAT, N, N, ONE, SIG, DWORK( IRES+1 ),
     $                N, X, LDX, G, LDG, DWORK( ITMP+1 ), NN, INFO2 )
C        
C        Get the machine precision.
C        
         EPS  = DLAMCH( 'Epsilon' )
         EPSN = EPS*DBLE( N + 4 )
         TEMP = EPS*FOUR
C
C        Add to abs(R) a term that takes account of rounding errors in
C        forming R:
C         abs(R) := abs(R) + EPS*(4*abs(Q) + (n+4)*(abs(op(Ac))'*abs(X)
C                 + abs(X)*abs(op(Ac))) + 2*(n+1)*abs(X)*abs(G)*abs(X)),
C        or                             _                           _
C         abs(R) := abs(R) + EPS*(4*abs(Q) + (n+4)*(abs(op(T))'*abs(X)
C                       _                            _      _      _
C                 + abs(X)*abs(op(T))) + 2*(n+1)*abs(X)*abs(G)*abs(X)),
C        where EPS is the machine precision.
C
         DO 50 J = 1, N
            DO 40 I = 1, N
               DWORK( IXBS+(J-1)*N+I ) = ABS( X( I, J ) )
   40       CONTINUE
   50    CONTINUE
C
         IF( LOWER ) THEN
            DO 70 J = 1, N
               DO 60 I = J, N
                  DWORK( IRES+(J-1)*N+I ) = TEMP*ABS( Q( I, J ) ) +
     $                   ABS( DWORK( IRES+(J-1)*N+I ) )
   60          CONTINUE
   70       CONTINUE
         ELSE
            DO 90 J = 1, N
               DO 80 I = 1, J
                  DWORK( IRES+(J-1)*N+I ) = TEMP*ABS( Q( I, J ) ) +
     $                   ABS( DWORK( IRES+(J-1)*N+I ) )
   80          CONTINUE
   90       CONTINUE
         END IF
C
         IF( UPDATE ) THEN
C
            DO 110 J = 1, N
               DO 100 I = 1, N
                  DWORK( IABS+(J-1)*N+I ) =
     $               ABS( DWORK( IABS+(J-1)*N+I ) )
  100          CONTINUE
  110       CONTINUE
C
            CALL DSYR2K( UPLO, TRANAT, N, N, EPSN, DWORK( IABS+1 ), N,
     $                   DWORK( IXBS+1 ), N, ONE,  DWORK( IRES+1 ), N )
         ELSE
C
            DO 130 J = 1, N
               DO 120 I = 1, MIN( J+1, N )
                  DWORK( IABS+(J-1)*N+I ) = ABS( T( I, J ) )
  120          CONTINUE
  130       CONTINUE
C
            CALL MB01UD( 'Left', TRANAT, N, N, EPSN, DWORK( IABS+1 ), N,
     $                   DWORK( IXBS+1), N, DWORK( ITMP+1 ), N, INFO2 )
            JJ = IRES + 1
            JX = ITMP + 1
            IF( LOWER ) THEN
               DO 140 J = 1, N
                  CALL DAXPY( N-J+1, ONE, DWORK( JX ), N, DWORK( JX ),
     $                        1 )
                  CALL DAXPY( N-J+1, ONE, DWORK( JX ), 1, DWORK( JJ ),
     $                        1 )
                  JJ = JJ + N + 1
                  JX = JX + N + 1
  140          CONTINUE
            ELSE
               DO 150 J = 1, N
                  CALL DAXPY( J, ONE, DWORK( ITMP+J ), N, DWORK( JX ),
     $                        1 )
                  CALL DAXPY( J, ONE, DWORK( JX ), 1, DWORK( JJ ), 1 )
                  JJ = JJ + N
                  JX = JX + N
  150          CONTINUE
            END IF
         END IF
C
         IF( LOWER ) THEN
            DO 170 J = 1, N
               DO 160 I = J, N
                  DWORK( IABS+(J-1)*N+I ) = ABS( G( I, J ) )
  160          CONTINUE
  170       CONTINUE
         ELSE
            DO 190 J = 1, N
               DO 180 I = 1, J
                  DWORK( IABS+(J-1)*N+I ) = ABS( G( I, J ) )
  180          CONTINUE
  190       CONTINUE
         END IF
C
         CALL MB01RU( UPLO, TRANA, N, N, ONE, EPS*DBLE( 2*( N + 1 ) ),
     $                DWORK( IRES+1 ), N, DWORK( IXBS+1), N,
     $                DWORK( IABS+1 ), N, DWORK( ITMP+1 ), NN, INFO2 )
C
         WRKOPT = MAX( WRKOPT, 4*NN )
C
C        Compute forward error bound, using matrix norm estimator.
C        Workspace 4*N*N.
C
         XANORM = DLANSY( 'Max', UPLO, N, X, LDX, DWORK )
C        
         CALL SB03QX( TRANA, UPLO, LYAPUN, N, XANORM, T, LDT, U, LDU,
     $                DWORK( IRES+1 ), N, FERR, IWORK, DWORK, IRES,
     $                INFO )
      END IF
C
      DWORK( 1 ) = DBLE( WRKOPT )
      RETURN
C
C *** Last line of SB02QD ***
      END
