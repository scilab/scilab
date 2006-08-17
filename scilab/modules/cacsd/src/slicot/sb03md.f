      SUBROUTINE SB03MD( DICO, JOB, FACT, TRANA, N, A, LDA, U, LDU, C,
     $                   LDC, SCALE, SEP, FERR, WR, WI, IWORK, DWORK,
     $                   LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for X either the real continuous-time Lyapunov equation
C
C        op(A)'*X + X*op(A) = scale*C                             (1)
C
C     or the real discrete-time Lyapunov equation
C
C        op(A)'*X*op(A) - X = scale*C                             (2)
C
C     and/or estimate an associated condition number, called separation,
C     where op(A) = A or A' (A**T) and C is symmetric (C = C').
C     (A' denotes the transpose of the matrix A.) A is N-by-N, the right
C     hand side C and the solution X are N-by-N, and scale is an output
C     scale factor, set less than or equal to 1 to avoid overflow in X.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     DICO    CHARACTER*1
C             Specifies the equation from which X is to be determined
C             as follows:
C             = 'C':  Equation (1), continuous-time case;
C             = 'D':  Equation (2), discrete-time case.
C
C     JOB     CHARACTER*1
C             Specifies the computation to be performed, as follows:
C             = 'X':  Compute the solution only;
C             = 'S':  Compute the separation only;
C             = 'B':  Compute both the solution and the separation.
C
C     FACT    CHARACTER*1
C             Specifies whether or not the real Schur factorization
C             of the matrix A is supplied on entry, as follows:
C             = 'F':  On entry, A and U contain the factors from the
C                     real Schur factorization of the matrix A;
C             = 'N':  The Schur factorization of A will be computed
C                     and the factors will be stored in A and U.
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, X, and C.  N >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N part of this array must
C             contain the matrix A. If FACT = 'F', then A contains
C             an upper quasi-triangular matrix in Schur canonical form;
C             the elements below the upper Hessenberg part of the
C             array A are not referenced.
C             On exit, if INFO = 0 or INFO = N+1, the leading N-by-N
C             upper Hessenberg part of this array contains the upper
C             quasi-triangular matrix in Schur canonical form from the
C             Schur factorization of A. The contents of array A is not
C             modified if FACT = 'F'.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     U       (input or output) DOUBLE PRECISION array, dimension
C             (LDU,N)
C             If FACT = 'F', then U is an input argument and on entry
C             the leading N-by-N part of this array must contain the
C             orthogonal matrix U of the real Schur factorization of A.
C             If FACT = 'N', then U is an output argument and on exit,
C             if INFO = 0 or INFO = N+1, it contains the orthogonal
C             N-by-N matrix from the real Schur factorization of A.
C
C     LDU     INTEGER
C             The leading dimension of array U.  LDU >= MAX(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry with JOB = 'X' or 'B', the leading N-by-N part of
C             this array must contain the symmetric matrix C.
C             On exit with JOB = 'X' or 'B', if INFO = 0 or INFO = N+1,
C             the leading N-by-N part of C has been overwritten by the
C             symmetric solution matrix X.
C             If JOB = 'S', C is not referenced.
C
C     LDC     INTEGER
C             The leading dimension of array C.
C             LDC >= 1,        if JOB = 'S';
C             LDC >= MAX(1,N), otherwise.
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     SEP     (output) DOUBLE PRECISION
C             If JOB = 'S' or JOB = 'B', and INFO = 0 or INFO = N+1, SEP
C             contains the estimated separation of the matrices op(A)
C             and -op(A)', if DICO = 'C' or of op(A) and op(A)', if
C             DICO = 'D'.
C             If JOB = 'X' or N = 0, SEP is not referenced.
C
C     FERR    (output) DOUBLE PRECISION
C             If JOB = 'B', and INFO = 0 or INFO = N+1, FERR contains an
C             estimated forward error bound for the solution X.
C             If XTRUE is the true solution, FERR bounds the relative
C             error in the computed solution, measured in the Frobenius
C             norm:  norm(X - XTRUE)/norm(XTRUE).
C             If JOB = 'X' or JOB = 'S', FERR is not referenced.
C
C     WR      (output) DOUBLE PRECISION array, dimension (N)
C     WI      (output) DOUBLE PRECISION array, dimension (N)
C             If FACT = 'N', and INFO = 0 or INFO = N+1, WR and WI
C             contain the real and imaginary parts, respectively, of
C             the eigenvalues of A.
C             If FACT = 'F', WR and WI are not referenced.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (N*N)
C             This array is not referenced if JOB = 'X'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0 or INFO = N+1, DWORK(1) returns the
C             optimal value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.  LDWORK >= 1, and
C             If JOB = 'X' then
C                If FACT = 'F', LDWORK >= N*N,           for DICO = 'C';
C                               LDWORK >= MAX(N*N, 2*N), for DICO = 'D';
C                If FACT = 'N', LDWORK >= MAX(N*N, 3*N).
C             If JOB = 'S' or JOB = 'B' then
C                If FACT = 'F', LDWORK >= 2*N*N,       for DICO = 'C';
C                               LDWORK >= 2*N*N + 2*N, for DICO = 'D'.
C                If FACT = 'N', LDWORK >= MAX(2*N*N, 3*N), DICO = 'C';
C                               LDWORK >= 2*N*N + 2*N, for DICO = 'D'.
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             > 0:  if INFO = i, the QR algorithm failed to compute all
C                   the eigenvalues (see LAPACK Library routine DGEES);
C                   elements i+1:n of WR and WI contain eigenvalues
C                   which have converged, and A contains the partially
C                   converged Schur form;
C             = N+1:  if DICO = 'C', and the matrices A and -A' have
C                   common or very close eigenvalues, or
C                   if DICO = 'D', and matrix A has almost reciprocal
C                   eigenvalues (that is, lambda(i) = 1/lambda(j) for
C                   some i and j, where lambda(i) and lambda(j) are
C                   eigenvalues of A and i <> j); perturbed values were
C                   used to solve the equation (but the matrix A is
C                   unchanged).
C
C     METHOD
C
C     The Schur factorization of a square matrix  A  is given by
C
C        A = U*S*U'
C
C     where U is orthogonal and S is block upper triangular with 1-by-1
C     and 2-by-2 blocks on its diagonal, these blocks corresponding to
C     the eigenvalues of A, the 2-by-2 blocks being complex conjugate
C     pairs. This factorization is obtained by numerically stable
C     methods: first A is reduced to upper Hessenberg form (if FACT =
C     'N') by means of Householder transformations and then the
C     QR Algorithm is applied to reduce the Hessenberg form to S, the
C     transformation matrices being accumulated at each step to give U.
C     If A has already been factorized prior to calling the routine
C     however, then the factors U and S may be supplied and the initial
C     factorization omitted.
C                   _            _
C     If we now put C = U'CU and X = UXU' equations (1) and (2) (see
C     PURPOSE) become (for TRANS = 'N')
C          _   _    _
C        S'X + XS = C,                                               (3)
C     and
C          _    _   _
C        S'XS - X = C,                                               (4)
C
C     respectively. Partition S, C and X as
C                            _   _         _   _
C            (s    s')      (c   c')      (x   x')
C            ( 11    )  _   ( 11   )  _   ( 11   )
C        S = (       ), C = (      ), X = (      )
C            (       )      ( _    )      ( _    )
C            ( 0   S )      ( c  C )      ( x  X )
C                   1             1             1
C                _      _
C     where s  , c  and x  are either scalars or 2-by-2 matrices and s,
C            11   11     11
C     _     _
C     c and x are either (N-1) element vectors or matrices with two
C     columns. Equations (3) and (4) can then be re-written as
C           _     _        _
C        s' x   + x  s   = c                                       (3.1)
C         11 11    11 11    11
C
C          _   _           _    _
C        S'x + xs        = c - sx                                  (3.2)
C         1      11              11
C
C                                _    _
C        S'X + X S       = C - (sx' + xs')                         (3.3)
C         1 1   1 1         1
C     and
C           _       _       _
C        s' x  s  - x     = c                                      (4.1)
C         11 11 11   11      11
C
C          _     _          _    _
C        S'xs  - x        = c - sx  s                              (4.2)
C         1  11                   11 11
C
C                                _            _        _
C        S'X S - X        = C - sx  s' - [s(S'x)' + (S'x)s']       (4.3)
C         1 1 1   1          1    11         1        1
C                                                  _
C     respectively. If DICO = 'C' ['D'], then once x   has been
C                                                   11
C     found from equation (3.1) [(4.1)], equation (3.2) [(4.2)] can be
C                                        _
C     solved by forward substitution for x and then equation (3.3)
C     [(4.3)] is of the same form as (3) [(4)] but of the order (N-1) or
C     (N-2) depending upon whether s   is 1-by-1 or 2-by-2.
C                                   11
C                             _      _
C     When s   is 2-by-2 then x  and c   will be 1-by-2 matrices and s,
C           11                 11     11
C     _     _
C     x and c are matrices with two columns. In this case, equation
C     (3.1) [(4.1)] defines the three equations in the unknown elements
C        _
C     of x   and equation (3.2) [(4.2)] can then be solved by forward
C         11                 _
C     substitution, a row of x being found at each step.
C
C     REFERENCES
C
C     [1] Barraud, A.Y.                   T
C         A numerical algorithm to solve A XA - X = Q.
C         IEEE Trans. Auto. Contr., AC-22, pp. 883-885, 1977.
C
C     [2] Bartels, R.H. and Stewart, G.W.  T
C         Solution of the matrix equation A X + XB = C.
C         Comm. A.C.M., 15, pp. 820-826, 1972.
C
C     [3] Hammarling, S.J.
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
C     If DICO = 'C', SEP is defined as the separation of op(A) and
C     -op(A)':
C
C            sep( op(A), -op(A)' ) = sigma_min( T )
C
C     and if DICO = 'D', SEP is defined as
C
C            sep( op(A), op(A)' ) = sigma_min( T )
C
C     where sigma_min(T) is the smallest singular value of the
C     N*N-by-N*N matrix
C
C       T = kprod( I(N), op(A)' ) + kprod( op(A)', I(N) )  (DICO = 'C'),
C
C       T = kprod( op(A)', op(A)' ) - I(N**2)              (DICO = 'D').
C
C     I(x) is an x-by-x identity matrix, and kprod denotes the Kronecker
C     product. The program estimates sigma_min(T) by the reciprocal of
C     an estimate of the 1-norm of inverse(T). The true reciprocal
C     1-norm of inverse(T) cannot differ from sigma_min(T) by more
C     than a factor of N.
C
C     When SEP is small, small changes in A, C can cause large changes
C     in the solution of the equation. An approximate bound on the
C     maximum relative error in the computed solution is
C
C                      EPS * norm(A) / SEP      (DICO = 'C'),
C
C                      EPS * norm(A)**2 / SEP   (DICO = 'D'),
C
C     where EPS is the machine precision.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, July 1997.
C     Supersedes Release 2.0 routine SB03AD by Control Systems Research
C     Group, Kingston Polytechnic, United Kingdom.
C
C     REVISIONS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, May 1999.
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
      CHARACTER         DICO, FACT, JOB, TRANA
      INTEGER           INFO, LDA, LDC, LDU, LDWORK, N
      DOUBLE PRECISION  FERR, SCALE, SEP
C     .. Array Arguments ..
      INTEGER           IWORK( * )
      DOUBLE PRECISION  A( LDA, * ), C( LDC, * ), DWORK( * ),
     $                  U( LDU, * ), WI( * ), WR( * )
C     .. Local Scalars ..
      LOGICAL           CONT, NOFACT, NOTA, WANTBH, WANTSP, WANTX
      CHARACTER         NOTRA, NTRNST, TRANST, UPLO
      INTEGER           I, IERR, KASE, LWA, MINWRK, NN, NN2, SDIM
      DOUBLE PRECISION  EPS, EST, SCALEF
C     .. Local Arrays ..
      LOGICAL           BWORK( 1 )
C     .. External Functions ..
      LOGICAL           LSAME, SELECT1
      DOUBLE PRECISION  DLAMCH, DLANHS
      EXTERNAL          DLAMCH, DLANHS, LSAME, SELECT1
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEES, DLACON, MB01RD, SB03MX, SB03MY, 
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         DBLE, INT, MAX
C     .. Executable Statements ..
C
C     Decode and Test input parameters.
C
      CONT   = LSAME( DICO,  'C' )
      WANTX  = LSAME( JOB,   'X' )
      WANTSP = LSAME( JOB,   'S' )
      WANTBH = LSAME( JOB,   'B' )
      NOFACT = LSAME( FACT,  'N' )
      NOTA   = LSAME( TRANA, 'N' )
      NN  = N*N
      NN2 = 2*NN
C
      INFO = 0
      IF( .NOT.CONT .AND. .NOT.LSAME( DICO, 'D' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.WANTBH .AND. .NOT.WANTSP .AND. .NOT.WANTX ) THEN
         INFO = -2
      ELSE IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -3
      ELSE IF( .NOT.NOTA .AND. .NOT.LSAME( TRANA, 'T' ) .AND.
     $                         .NOT.LSAME( TRANA, 'C' ) ) THEN
         INFO = -4
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( WANTSP .AND. LDC.LT.1 .OR.
     $    .NOT.WANTSP .AND. LDC.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE
         IF ( WANTX ) THEN
            IF ( NOFACT ) THEN
               MINWRK = MAX( NN, 3*N )
            ELSE IF ( CONT ) THEN
               MINWRK = NN
            ELSE
               MINWRK = MAX( NN, 2*N )
            END IF
         ELSE
            IF ( CONT ) THEN
               IF ( NOFACT ) THEN
                  MINWRK = MAX( NN2, 3*N )
               ELSE
                  MINWRK = NN2
               END IF
            ELSE
               MINWRK = NN2 + 2*N
            END IF
         END IF
         IF( LDWORK.LT.MAX( 1, MINWRK ) )
     $      INFO = -19
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB03MD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 ) THEN
         SCALE = ONE
         IF( WANTBH )
     $      FERR  = ZERO
         DWORK(1) = ONE
         RETURN
      END IF
C
      LWA = 0
C
      IF( NOFACT ) THEN
C
C        Compute the Schur factorization of A.
C        Workspace:  need   3*N;
C                    prefer larger.
C        (Note: Comments in the code beginning "Workspace:" describe the
C        minimal amount of real workspace needed at that point in the
C        code, as well as the preferred amount for good performance.
C        NB refers to the optimal block size for the immediately
C        following subroutine, as returned by ILAENV.)
C
         CALL DGEES( 'Vectors', 'Not ordered', SELECT1, N, A, LDA, SDIM,
     $               WR, WI, U, LDU, DWORK, LDWORK, BWORK, INFO )
         IF( INFO.GT.0 )
     $      RETURN
         LWA = INT( DWORK( 1 ) )
      END IF
C
      IF( .NOT.WANTSP ) THEN
C
C        Transform the right-hand side.
C        Workspace:  N*N.
C
         NTRNST = 'N'
         TRANST = 'T'
         UPLO   = 'U'
         CALL MB01RD( UPLO, TRANST, N, N, ZERO, ONE, C, LDC, U, LDU, C,
     $                LDC, DWORK, LDWORK, INFO )
C
         DO 10 I = 2, N
            CALL DCOPY( I-1, C(1,I), 1, C(I,1), LDC )
   10    CONTINUE    
C
         LWA = MAX( LWA, NN )
C
C        Solve the transformed equation.
C        Workspace for DICO = 'D':  2*N.
C
         IF ( CONT ) THEN
            CALL SB03MY( TRANA, N, A, LDA, C, LDC, SCALE, INFO )
         ELSE                   
            CALL SB03MX( TRANA, N, A, LDA, C, LDC, SCALE, DWORK, INFO )
         END IF
         IF( INFO.GT.0 )
     $      INFO = N + 1
C
C        Transform back the solution.
C        Workspace:  N*N.
C
         CALL MB01RD( UPLO, NTRNST, N, N, ZERO, ONE, C, LDC, U, LDU, C,
     $                LDC, DWORK, LDWORK, IERR )
C
         DO 20 I = 2, N
            CALL DCOPY( I-1, C(1,I), 1, C(I,1), LDC )
   20    CONTINUE    
C
      END IF
C
      IF( .NOT.WANTX ) THEN
C
C        Estimate the separation.
C        Workspace:  2*N*N       for DICO = 'C';
C                    2*N*N + 2*N for DICO = 'D'.
C
         IF( NOTA ) THEN
            NOTRA = 'T'
         ELSE
            NOTRA = 'N'
         END IF
C
         EST = ZERO
         KASE = 0
C        REPEAT
   30    CONTINUE
         CALL DLACON( NN, DWORK(NN+1), DWORK, IWORK, EST, KASE )
         IF( KASE.NE.0 ) THEN
            IF( KASE.EQ.1 ) THEN
               IF( CONT ) THEN
                  CALL SB03MY( TRANA, N, A, LDA, DWORK, N, SCALEF,
     $                         IERR )
               ELSE                   
                  CALL SB03MX( TRANA, N, A, LDA, DWORK, N, SCALEF,
     $                         DWORK(NN2+1), IERR )
               END IF
            ELSE
               IF( CONT ) THEN
                  CALL SB03MY( NOTRA, N, A, LDA, DWORK, N, SCALEF,
     $                         IERR )
               ELSE                   
                  CALL SB03MX( NOTRA, N, A, LDA, DWORK, N, SCALEF,
     $                         DWORK(NN2+1), IERR )
               END IF
            END IF
            GO TO 30
         END IF
C        UNTIL KASE = 0
C
         SEP = SCALEF / EST
C
         IF( WANTBH ) THEN
C
C           Get the machine precision.
C
            EPS = DLAMCH( 'P' )
C
C           Compute the estimate of the relative error.
C
            IF ( CONT ) THEN
               FERR = EPS*DLANHS( 'Frobenius', N, A, LDA, DWORK )/SEP
            ELSE
               FERR = EPS*DLANHS( 'Frobenius', N, A, LDA, DWORK )**2/SEP
            END IF
         END IF
      END IF 
C
      DWORK( 1 ) = DBLE( MAX( LWA, MINWRK ) )
      RETURN
C *** Last line of SB03MD ***
      END
