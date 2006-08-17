      SUBROUTINE SB02SD( JOB, FACT, TRANA, UPLO, LYAPUN, N, A, LDA, T,
     $                   LDT, U, LDU, G, LDG, Q, LDQ, X, LDX, SEPD,
     $                   RCOND, FERR, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To estimate the conditioning and compute an error bound on the
C     solution of the real discrete-time matrix algebraic Riccati
C     equation (see FURTHER COMMENTS)
C                                 -1
C         X = op(A)'*X*(I_n + G*X)  *op(A) + Q,                      (1)
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
C             the matrix Ac = inv(I_n + G*X)*A (if TRANA = 'N'), or
C             Ac = A*inv(I_n + X*G) (if TRANA = 'T' or 'C'), is supplied
C             on entry, as follows:
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
C     SEPD    (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'B', the estimated quantity 
C             sepd(op(Ac),op(Ac)').
C             If N = 0, or X = 0, or JOB = 'E', SEPD is not referenced.
C
C     RCOND   (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'B', an estimate of the reciprocal
C             condition number of the discrete-time Riccati equation.
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
C             Let LWA = N*N, if LYAPUN = 'O';
C                 LWA = 0,   otherwise,
C             and LWN = N,   if LYAPUN = 'R' and JOB = 'E' or 'B';
C                 LWN = 0,   otherwise.
C             If FACT = 'N', then
C                LDWORK  = MAX(LWA + 5*N, MAX(3,2*N*N) + N*N), 
C                                                 if JOB = 'C';
C                LDWORK  = MAX(LWA + 5*N, MAX(3,2*N*N) + 2*N*N + LWN),
C                                                 if JOB = 'E' or 'B'.
C             If FACT = 'F', then
C                LDWORK  = MAX(3,2*N*N) + N*N,    if JOB = 'C';
C                LDWORK  = MAX(3,2*N*N) + 2*N*N + LWN,
C                                                 if JOB = 'E' or 'B'.
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
C             = N+1:  if T has almost reciprocal eigenvalues; perturbed
C                   values were used to solve Lyapunov equations, but
C                   the matrix T, if given (for FACT = 'F'), is
C                   unchanged.
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
C     Omega(W) = op(Ac)'*W*op(Ac) - W,
C     Theta(W) = inv(Omega(op(W)'*X*op(Ac) + op(Ac)'X*op(W))),
C        Pi(W) = inv(Omega(op(Ac)'*X*W*X*op(Ac))),
C
C     and Ac = inv(I_n + G*X)*A (if TRANA = 'N'), or
C         Ac = A*inv(I_n + X*G) (if TRANA = 'T' or 'C').
C
C     Note that the Riccati equation (1) is equivalent to
C
C         X = op(Ac)'*X*op(Ac) + op(Ac)'*X*G*X*op(Ac) + Q,           (2)
C
C     and to
C         _          _                _ _ _         _
C         X = op(T)'*X*op(T) + op(T)'*X*G*X*op(T) + Q,               (3)
C           _           _               _
C     where X = U'*X*U, Q = U'*Q*U, and G = U'*G*U, with U the
C     orthogonal matrix reducing Ac to a real Schur form, T = U'*Ac*U.
C
C     The routine estimates the quantities
C
C     sepd(op(Ac),op(Ac)') = 1 / norm(inv(Omega)),
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
C     When SEPD is computed and it is zero, the routine returns
C     immediately, with RCOND and FERR (if requested) set to 0 and 1,
C     respectively. In this case, the equation is singular.
C
C     Let B be an N-by-M matrix (if TRANA = 'N') or an M-by-N matrix
C     (if TRANA = 'T' or 'C'), let R be an M-by-M symmetric positive
C     definite matrix (R = R**T), and denote G = op(B)*inv(R)*op(B)'.
C     Then, the Riccati equation (1) is equivalent to the standard
C     discrete-time matrix algebraic Riccati equation
C
C         X = op(A)'*X*op(A) -                                       (4)
C                                                -1
C             op(A)'*X*op(B)*(R + op(B)'*X*op(B))  *op(B)'*X*op(A) + Q. 
C
C     By symmetry, the equation (1) is also equivalent to
C                               -1
C         X = op(A)'*(I_n + X*G)  *X*op(A) + Q.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, and
C     P.Hr. Petkov, Technical University of Sofia, March 1999.
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
      DOUBLE PRECISION   FERR, RCOND, SEPD
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
      INTEGER            I, IABS, INFO2, IRES, IWRK, IXBS, IXMA, J, JJ,
     $                   KASE, LDW, LWA, LWR, NN, SDIM, WRKOPT
      DOUBLE PRECISION   ANORM, BIGNUM, DENOM, EPS, EPSN, EPST, EST,
     $                   GNORM, PINORM, QNORM, SCALE, TEMP, THNORM,
     $                   TMAX, XANORM, XNORM
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
      EXTERNAL           DAXPY, DCOPY, DGEES, DGEMM, DGESV, DLACON,
     $                   DLACPY, DLASET, DSWAP, DSYMM, MA02ED, MB01RU, 
     $                   MB01RX, MB01RY, MB01UD, SB03MX, SB03SX, SB03SY,
     $                   XERBLA
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
      IF( UPDATE ) THEN
         LWA = NN
      ELSE
         LWA = 0
      END IF
C
      IF( JOBC ) THEN
         LDW = MAX( 3, 2*NN ) + NN
      ELSE
         LDW = MAX( 3, 2*NN ) + 2*NN
         IF( .NOT.UPDATE )
     $      LDW = LDW + N
      END IF
      IF( NOFACT )
     $   LDW = MAX( LWA + 5*N, LDW )
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
      ELSE IF( LDWORK.LT.LDW ) THEN
         INFO = -24
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB02SD', -INFO )
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
      IRES = 0
      IXBS = IRES + NN
      IXMA = MAX( 3, 2*NN )
      IABS = IXMA + NN
      IWRK = IABS + NN
C
C     Workspace:  LWK, where
C                 LWK = 2*N*N, if LYAPUN = 'O', or FACT = 'N',
C                 LWK = N,     otherwise.
C
      IF( UPDATE .OR. NOFACT ) THEN
C
         CALL DLASET( 'Full', N, N, ZERO, ONE, DWORK( IXBS+1 ), N )
         CALL DSYMM(  'Left', UPLO, N, N, ONE, G, LDG, X, LDX, ONE,
     $                DWORK( IXBS+1 ), N )
         IF( NOTRNA ) THEN
C                                   -1
C           Compute Ac = (I_n + G*X)  *A.
C
            CALL DLACPY( 'Full', N, N, A, LDA, DWORK, N )
            CALL DGESV( N, N, DWORK( IXBS+1 ), N, IWORK, DWORK, N,
     $                  INFO2 )
         ELSE
C                                     -1
C           Compute Ac = A*(I_n + X*G)  .
C
            DO 10 J = 1, N
               CALL DCOPY( N, A( 1, J ), 1, DWORK( J ), N )
   10       CONTINUE
            CALL DGESV( N, N, DWORK( IXBS+1 ), N, IWORK, DWORK, N,
     $                  INFO2 )
            DO 20 J = 2, N
               CALL DSWAP( J-1, DWORK( (J-1)*N+1 ), 1, DWORK( J ), N )
   20       CONTINUE
         END IF
C
         WRKOPT = DBLE( 2*NN )
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
C                    LWA = N*N, if LYAPUN = 'O';
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
      IF( NEEDAC ) THEN
         CALL DLACPY( 'Full', N, N, DWORK, N, DWORK( IABS+1 ), N )
         LWR = NN
      ELSE
         LWR = 0
      END IF
C
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
C                         _
C     Compute X*op(Ac) or X*op(T).
C
      IF( UPDATE ) THEN
         CALL DGEMM( 'NoTranspose', TRANA, N, N, N, ONE, X, LDX, DWORK,
     $               N, ZERO, DWORK( IXMA+1 ), N )
      ELSE
         CALL MB01UD( 'Right', TRANA, N, N, ONE, T, LDT, X, LDX,
     $                DWORK( IXMA+1 ), N, INFO2 )
      END IF
C
      IF( .NOT.JOBE ) THEN
C
C        Estimate sepd(op(Ac),op(Ac)') = sepd(op(T),op(T)') and
C        norm(Theta).
C        Workspace LWR + MAX(3,2*N*N) + N*N, where
C                  LWR = N*N, if LYAPUN = 'O' and JOB = 'B',
C                  LWR = 0,   otherwise.
C
         CALL SB03SY( 'Both', TRANA, LYAPUN, N, T, LDT, U, LDU,
     $                DWORK( IXMA+1 ), N, SEPD, THNORM, IWORK, DWORK,
     $                IXMA, INFO )
C
         WRKOPT = MAX( WRKOPT, LWR + MAX( 3, 2*NN ) + NN )
C        
C        Return if the equation is singular.
C        
         IF( SEPD.EQ.ZERO ) THEN
            RCOND = ZERO
            IF( JOBB ) 
     $         FERR = ONE
            DWORK( 1 ) = DBLE( WRKOPT )
            RETURN
         END IF
C
C        Estimate norm(Pi).
C        Workspace LWR + MAX(3,2*N*N) + N*N.
C
         KASE = 0
C
C        REPEAT
   30    CONTINUE
         CALL DLACON( NN, DWORK( IXBS+1 ), DWORK, IWORK, EST, KASE )
         IF( KASE.NE.0 ) THEN
C
C           Select the triangular part of symmetric matrix to be used.
C
            IF( DLANSY( '1-norm', 'Upper', N, DWORK, N, DWORK( IXBS+1 ))
     $          .GE.
     $          DLANSY( '1-norm', 'Lower', N, DWORK, N, DWORK( IXBS+1 ))
     $        ) THEN
               LOUP = 'U'
            ELSE
               LOUP = 'L'
            END IF
C                                                        _   _
C           Compute RHS = op(Ac)'*X*W*X*op(Ac) or op(T)'*X*W*X*op(T).
C
            CALL MB01RU( LOUP, TRANAT, N, N, ZERO, ONE, DWORK, N,
     $                   DWORK( IXMA+1 ), N, DWORK, N, DWORK( IXBS+1 ),
     $                   NN, INFO2 )
C
            IF( UPDATE ) THEN
C
C              Transform the right-hand side: RHS := U'*RHS*U.
C              
               CALL MB01RU( LOUP, 'Transpose', N, N, ZERO, ONE, DWORK,
     $                      N, U, LDU, DWORK, N, DWORK( IXBS+1 ), NN,
     $                      INFO2 )
            END IF
C
C           Fill in the remaining triangle of the symmetric matrix.
C           
            CALL MA02ED( LOUP, N, DWORK, N )
C
            IF( KASE.EQ.1 ) THEN
C
C              Solve op(T)'*Y*op(T) - Y = scale*RHS.
C
               CALL SB03MX( TRANA, N, T, LDT, DWORK, N, SCALE,
     $                      DWORK( IXBS+1 ), INFO2 )
            ELSE
C
C              Solve op(T)*W*op(T)' - W = scale*RHS.
C
               CALL SB03MX( TRANAT, N, T, LDT, DWORK, N, SCALE,
     $                      DWORK( IXBS+1 ), INFO2 )
            END IF
C
            IF( UPDATE ) THEN
C
C              Transform back to obtain the solution: Z := U*Z*U', with
C              Z = Y or Z = W.
C              
               CALL MB01RU( LOUP, 'No transpose', N, N, ZERO, ONE,
     $                      DWORK, N, U, LDU, DWORK, N, DWORK( IXBS+1 ),
     $                      NN, INFO2 )
C
C              Fill in the remaining triangle of the symmetric matrix.
C
               CALL MA02ED( LOUP, N, DWORK, N )
            END IF
            GO TO 30
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
         TMAX = MAX( SEPD, XNORM, ANORM, GNORM )
         IF( TMAX.LE.ONE ) THEN
            TEMP  = SEPD*XNORM
            DENOM = QNORM + ( SEPD*ANORM )*THNORM +
     $                      ( SEPD*GNORM )*PINORM
         ELSE
            TEMP  =   ( SEPD / TMAX )*( XNORM / TMAX )
            DENOM = ( ( ONE  / TMAX )*( QNORM / TMAX ) ) +
     $              ( ( SEPD / TMAX )*( ANORM / TMAX ) )*THNORM +
     $              ( ( SEPD / TMAX )*( GNORM / TMAX ) )*PINORM
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
C          R = op(Ac)'*X*op(Ac) + op(Ac)'*X*G*X*op(Ac) + Q - X,
C        or           _                _ _ _         _   _
C          R = op(T)'*X*op(T) + op(T)'*X*G*X*op(T) + Q - X,
C        exploiting the symmetry. Actually, the equivalent formula
C          R = op(A)'*X*op(Ac) + Q - X
C        is used in the first case.
C        Workspace MAX(3,2*N*N) + 2*N*N,     if LYAPUN = 'O';
C                  MAX(3,2*N*N) + 2*N*N + N, if LYAPUN = 'R'.
C
         CALL DLACPY( UPLO, N, N, Q, LDQ, DWORK( IRES+1 ), N )
         JJ = IRES + 1
         IF( LOWER ) THEN
            DO 40 J = 1, N
               CALL DAXPY( N-J+1, -ONE, X( J, J ), 1, DWORK( JJ ), 1 )
               JJ = JJ + N + 1
   40       CONTINUE
         ELSE
            DO 50 J = 1, N
               CALL DAXPY( J, -ONE, X( 1, J ), 1, DWORK( JJ ), 1 )
               JJ = JJ + N
   50       CONTINUE
         END IF
C
         IF( UPDATE ) THEN
            CALL MB01RX( 'Left', UPLO, TRANAT, N, N, ONE, ONE,
     $                   DWORK( IRES+1 ), N, A, LDA, DWORK( IXMA+1 ), N,
     $                   INFO2 )
         ELSE
            CALL MB01RY( 'Left', UPLO, TRANAT, N, ONE, ONE,
     $                   DWORK( IRES+1 ), N, T, LDT, DWORK( IXMA+1 ), N,
     $                   DWORK( IWRK+1 ), INFO2 )
            CALL DSYMM(  'Left', UPLO, N, N, ONE, G, LDG,
     $                   DWORK( IXMA+1 ), N, ZERO, DWORK( IXBS+1 ), N )
            CALL MB01RX( 'Left', UPLO, 'Transpose', N, N, ONE, ONE,
     $                   DWORK( IRES+1 ), N, DWORK( IXMA+1 ), N,
     $                   DWORK( IXBS+1 ), N, INFO2 )
         END IF
C        
C        Get the machine precision.
C        
         EPS  = DLAMCH( 'Epsilon' )
         EPSN = EPS*DBLE( N + 4 )
         EPST = EPS*DBLE( 2*( N + 1 ) )
         TEMP = EPS*FOUR
C
C        Add to abs(R) a term that takes account of rounding errors in
C        forming R:
C         abs(R) := abs(R) + EPS*(4*abs(Q) + 4*abs(X) +
C                   (n+4)*abs(op(Ac))'*abs(X)*abs(op(Ac)) + 2*(n+1)*
C                   abs(op(Ac))'*abs(X)*abs(G)*abs(X)*abs(op(Ac))),
C        or                             _          _
C         abs(R) := abs(R) + EPS*(4*abs(Q) + 4*abs(X) +
C                                         _
C                   (n+4)*abs(op(T))'*abs(X)*abs(op(T)) +
C                                         _      _      _
C                 2*(n+1)*abs(op(T))'*abs(X)*abs(G)*abs(X)*abs(op(T))),
C        where EPS is the machine precision.
C
         DO 70 J = 1, N
            DO 60 I = 1, N
               DWORK( IXBS+(J-1)*N+I ) = ABS( X( I, J ) )
   60       CONTINUE
   70    CONTINUE
C
         IF( LOWER ) THEN
            DO 90 J = 1, N
               DO 80 I = J, N
                  DWORK( IRES+(J-1)*N+I ) = TEMP*( ABS( Q( I, J ) ) +
     $                                             ABS( X( I, J ) ) ) +
     $                                 ABS( DWORK( IRES+(J-1)*N+I ) )
   80          CONTINUE
   90       CONTINUE
         ELSE
            DO 110 J = 1, N
               DO 100 I = 1, J
                  DWORK( IRES+(J-1)*N+I ) = TEMP*( ABS( Q( I, J ) ) +
     $                                             ABS( X( I, J ) ) ) +
     $                                 ABS( DWORK( IRES+(J-1)*N+I ) )
  100          CONTINUE
  110       CONTINUE
         END IF
C
         IF( UPDATE ) THEN
C
            DO 130 J = 1, N
               DO 120 I = 1, N
                  DWORK( IABS+(J-1)*N+I ) =
     $               ABS( DWORK( IABS+(J-1)*N+I ) )
  120          CONTINUE
  130       CONTINUE
C
            CALL DGEMM( 'NoTranspose', TRANA, N, N, N, ONE,
     $                  DWORK( IXBS+1 ), N, DWORK( IABS+1 ), N, ZERO,
     $                  DWORK( IXMA+1 ), N )
            CALL MB01RX( 'Left', UPLO, TRANAT, N, N, ONE, EPSN,
     $                   DWORK( IRES+1 ), N, DWORK( IABS+1 ), N,
     $                   DWORK( IXMA+1 ), N, INFO2 )
         ELSE
C
            DO 150 J = 1, N
               DO 140 I = 1, MIN( J+1, N )
                  DWORK( IABS+(J-1)*N+I ) = ABS( T( I, J ) )
  140          CONTINUE
  150       CONTINUE
C
            CALL MB01UD( 'Right', TRANA, N, N, ONE, DWORK( IABS+1 ), N,
     $                   DWORK( IXBS+1 ), N, DWORK( IXMA+1 ), N, INFO2 )
            CALL MB01RY( 'Left', UPLO, TRANAT, N, ONE, EPSN,
     $                   DWORK( IRES+1 ), N, DWORK( IABS+1 ), N,
     $                   DWORK( IXMA+1 ), N, DWORK( IWRK+1 ), INFO2 )
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
         IF( UPDATE ) THEN
            CALL MB01RU( UPLO, TRANAT, N, N, ONE, EPST, DWORK( IRES+1 ),
     $                   N, DWORK( IXMA+1 ), N, DWORK( IABS+1 ), N,
     $                   DWORK( IXBS+1 ), NN, INFO2 )
            WRKOPT = MAX( WRKOPT, MAX( 3, 2*NN ) + 2*NN )
         ELSE
            CALL DSYMM(  'Left', UPLO, N, N, ONE,  DWORK( IABS+1 ), N,
     $                   DWORK( IXMA+1 ), N, ZERO, DWORK( IXBS+1 ), N )
            CALL MB01RY( 'Left', UPLO, TRANAT, N, ONE, EPST,
     $                   DWORK( IRES+1 ), N, DWORK( IXMA+1 ), N,
     $                   DWORK( IXBS+1 ), N, DWORK( IWRK+1 ), INFO2 )
            WRKOPT = MAX( WRKOPT, MAX( 3, 2*NN ) + 2*NN + N )
         END IF
C
C        Compute forward error bound, using matrix norm estimator.
C        Workspace MAX(3,2*N*N) + N*N.
C
         XANORM = DLANSY( 'Max', UPLO, N, X, LDX, DWORK )
C
         CALL SB03SX( TRANA, UPLO, LYAPUN, N, XANORM, T, LDT, U, LDU,
     $                DWORK( IRES+1 ), N, FERR, IWORK, DWORK( IXBS+1 ),
     $                IXMA, INFO )
      END IF
C
      DWORK( 1 ) = DBLE( WRKOPT )
      RETURN
C
C *** Last line of SB02SD ***
      END
