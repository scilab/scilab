      SUBROUTINE SB03OU( DISCR, LTRANS, N, M, A, LDA, B, LDB, TAU, U,
     $                   LDU, SCALE, DWORK, LDWORK, INFO )
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
C     an N-by-N matrix in real Schur form, op(B) is an M-by-N matrix,
C     U is an upper triangular matrix containing the Cholesky factor of
C     the solution matrix X, X = op(U)'*op(U), and scale is an output
C     scale factor, set less than or equal to 1 to avoid overflow in X.
C     If matrix B has full rank then the solution matrix X will be
C     positive-definite and hence the Cholesky factor U will be
C     nonsingular, but if B is rank deficient then X may only be
C     positive semi-definite and U will be singular.
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
C     DISCR   LOGICAL
C             Specifies the type of Lyapunov equation to be solved as
C             follows:
C             = .TRUE. :  Equation (2), discrete-time case;
C             = .FALSE.:  Equation (1), continuous-time case.
C
C     LTRANS  LOGICAL
C             Specifies the form of op(K) to be used, as follows:
C             = .FALSE.:  op(K) = K    (No transpose);
C             = .TRUE. :  op(K) = K**T (Transpose).
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
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N upper Hessenberg part of this array
C             must contain a real Schur form matrix S. The elements
C             below the upper Hessenberg part of the array A are not
C             referenced. The 2-by-2 blocks must only correspond to
C             complex conjugate pairs of eigenvalues (not to real
C             eigenvalues).
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             if LTRANS = .FALSE., and dimension (LDB,M), if
C             LTRANS = .TRUE..
C             On entry, if LTRANS = .FALSE., the leading M-by-N part of
C             this array must contain the coefficient matrix B of the
C             equation.
C             On entry, if LTRANS = .TRUE., the leading N-by-M part of
C             this array must contain the coefficient matrix B of the
C             equation.
C             On exit, if LTRANS = .FALSE., the leading
C             MIN(M,N)-by-MIN(M,N) upper triangular part of this array
C             contains the upper triangular matrix R (as defined in
C             METHOD), and the M-by-MIN(M,N) strictly lower triangular
C             part together with the elements of the array TAU are
C             overwritten by details of the matrix P (also defined in
C             METHOD). When M < N, columns (M+1),...,N of the array B
C             are overwritten by the matrix Z (see METHOD).
C             On exit, if LTRANS = .TRUE., the leading
C             MIN(M,N)-by-MIN(M,N) upper triangular part of
C             B(1:N,M-N+1), if M >= N, or of B(N-M+1:N,1:M), if M < N,
C             contains the upper triangular matrix R (as defined in
C             METHOD), and the remaining elements (below the diagonal
C             of R) together with the elements of the array TAU are
C             overwritten by details of the matrix P (also defined in
C             METHOD). When M < N, rows 1,...,(N-M) of the array B
C             are overwritten by the matrix Z (see METHOD).
C
C     LDB     INTEGER
C             The leading dimension of array B.
C             LDB >= MAX(1,M), if LTRANS = .FALSE.,
C             LDB >= MAX(1,N), if LTRANS = .TRUE..
C
C     TAU     (output) DOUBLE PRECISION array of dimension (MIN(N,M))
C             This array contains the scalar factors of the elementary
C             reflectors defining the matrix P.
C
C     U       (output) DOUBLE PRECISION array of dimension (LDU,N)
C             The leading N-by-N upper triangular part of this array
C             contains the Cholesky factor of the solution matrix X of
C             the problem, X = op(U)'*op(U).
C             The array U may be identified with B in the calling
C             statement, if B is properly dimensioned, and the
C             intermediate results returned in B are not needed.
C
C     LDU     INTEGER
C             The leading dimension of array U.  LDU >= MAX(1,N).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, or INFO = 1, DWORK(1) returns the
C             optimal value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK. LDWORK >= MAX(1,4*N).
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
C                   if DISCR = .FALSE., this means that while the matrix
C                   A has computed eigenvalues with negative real parts,
C                   it is only just stable in the sense that small
C                   perturbations in A can make one or more of the
C                   eigenvalues have a non-negative real part;
C                   if DISCR = .TRUE., this means that while the matrix
C                   A has computed eigenvalues inside the unit circle,
C                   it is nevertheless only just convergent, in the
C                   sense that small perturbations in A can make one or
C                   more of the eigenvalues lie outside the unit circle;
C                   perturbed values were used to solve the equation
C                   (but the matrix A is unchanged);
C             = 2:  if matrix A is not stable (that is, one or more of
C                   the eigenvalues of A has a non-negative real part),
C                   if DISCR = .FALSE., or not convergent (that is, one
C                   or more of the eigenvalues of A lies outside the
C                   unit circle), if DISCR = .TRUE.;
C             = 3:  if matrix A has two or more consecutive non-zero
C                   elements on the first sub-diagonal, so that there is
C                   a block larger than 2-by-2 on the diagonal;
C             = 4:  if matrix A has a 2-by-2 diagonal block with real
C                   eigenvalues instead of a complex conjugate pair.
C
C     METHOD
C
C     The method used by the routine is based on the Bartels and
C     Stewart method [1], except that it finds the upper triangular
C     matrix U directly without first finding X and without the need
C     to form the normal matrix op(B)'*op(B) [2].
C
C     If LTRANS = .FALSE., the matrix B is factored as 
C
C        B = P ( R ),  M >= N,   B = P ( R  Z ),  M < N,
C              ( 0 )
C
C     (QR factorization), where P is an M-by-M orthogonal matrix and
C     R is a square upper triangular matrix.
C
C     If LTRANS = .TRUE., the matrix B is factored as 
C
C        B = ( 0  R ) P,  M >= N,  B = ( Z ) P,  M < N,
C                                      ( R )
C
C     (RQ factorization), where P is an M-by-M orthogonal matrix and 
C     R is a square upper triangular matrix.
C
C     These factorizations are used to solve the continuous-time
C     Lyapunov equation in the canonical form
C                                                        2
C       op(A)'*op(U)'*op(U) + op(U)'*op(U)*op(A) = -scale *op(F)'*op(F),
C
C     or the discrete-time Lyapunov equation in the canonical form
C                                                        2
C       op(A)'*op(U)'*op(U)*op(A) - op(U)'*op(U) = -scale *op(F)'*op(F),
C
C     where U and F are N-by-N upper triangular matrices, and
C
C        F = R,                                  if M >= N, or
C
C        F = ( R ),    if LTRANS = .FALSE.,  or
C            ( 0 )
C
C        F = ( 0  Z ), if LTRANS = .TRUE.,       if M < N.
C            ( 0  R )
C
C     The canonical equation is solved for U.
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
C     equation will be ill-conditioned. "Large" elements in U relative
C     to those of A and B, or a "small" value for scale, are symptoms
C     of ill-conditioning. A condition estimate can be computed using
C     SLICOT Library routine SB03MD.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, May 1997.
C     Supersedes Release 2.0 routine SB03CZ by Sven Hammarling,
C     NAG Ltd, United Kingdom.
C     Partly based on routine PLYAPS by A. Varga, University of Bochum,
C     May 1992.
C
C     REVISIONS
C
C     Dec. 1997, April 1998, May 1999.
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation, real Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      LOGICAL           DISCR, LTRANS
      INTEGER           INFO, LDA, LDB, LDU, LDWORK, M, N
      DOUBLE PRECISION  SCALE
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*), TAU(*), U(LDU,*)
C     .. Local Scalars ..
      INTEGER           I, J, K, L, MN, WRKOPT
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEQRF, DGERQF, DLACPY, DLASET, SB03OT,
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C     .. Executable Statements ..
C
      INFO = 0
C
C     Test the input scalar arguments.
C
      IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -6
      ELSE IF( ( LDB.LT.MAX( 1, M ) .AND. .NOT.LTRANS ) .OR. 
     $         ( LDB.LT.MAX( 1, N ) .AND.      LTRANS ) ) THEN
         INFO = -8
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDWORK.LT.MAX( 1, 4*N ) ) THEN
         INFO = -14
      END IF
C
      IF ( INFO.NE.0 ) THEN      
C
C        Error return.
C
         CALL XERBLA( 'SB03OU', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      MN = MIN( N, M )
      IF ( MN.EQ.0 ) THEN
         SCALE = ONE
         DWORK(1) = ONE
         RETURN
      END IF
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C
      IF ( LTRANS ) THEN
C
C        Case op(K) = K'.
C
C        Perform the RQ factorization of B.
C        Workspace: need   N;
C                   prefer N*NB.
C
         CALL DGERQF( N, M, B, LDB, TAU, DWORK, LDWORK, INFO )
C
C        The triangular matrix F is constructed in the array U so that 
C        U can share the same memory as B.
C
         IF ( M.GE.N ) THEN
            CALL DLACPY( 'Upper', MN, N, B(1,M-N+1), LDB, U, LDU )
         ELSE
C
            DO 10 I = M, 1, -1
               CALL DCOPY( N-M+I, B(1,I), 1, U(1,N-M+I), 1 )
   10       CONTINUE
C
            CALL DLASET( 'Full', N, N-M, ZERO, ZERO, U, LDU )
         END IF
      ELSE
C
C        Case op(K) = K.
C
C        Perform the QR factorization of B.
C        Workspace: need   N;
C                   prefer N*NB.
C
         CALL DGEQRF( M, N, B, LDB, TAU, DWORK, LDWORK, INFO )
         CALL DLACPY( 'Upper', MN, N, B, LDB, U, LDU )
         IF ( M.LT.N )
     $      CALL DLASET( 'Upper', N-M, N-M, ZERO, ZERO, U(M+1,M+1),
     $                   LDU )
      END IF
      WRKOPT = DWORK(1)
C
C     Solve the canonical Lyapunov equation
C                                                      2
C     op(A)'*op(U)'*op(U) + op(U)'*op(U)*op(A) = -scale *op(F)'*op(F),
C
C     or
C                                                      2
C     op(A)'*op(U)'*op(U)*op(A) - op(U)'*op(U) = -scale *op(F)'*op(F)
C
C     for U.
C
      CALL SB03OT( DISCR, LTRANS, N, A, LDA, U, LDU, SCALE, DWORK,
     $             INFO )
      IF ( INFO.NE.0 .AND. INFO.NE.1 )
     $   RETURN
C
C     Make the diagonal elements of U non-negative.
C
      IF ( LTRANS ) THEN
C
         DO 30 J = 1, N
            IF ( U(J,J).LT.ZERO ) THEN
C
               DO 20 I = 1, J
                  U(I,J) = -U(I,J)
   20          CONTINUE
C
            END IF
   30    CONTINUE
C
      ELSE
         K = 1
C
         DO 50 J = 1, N
            DWORK(K) = U(J,J)
            L = 1
C
            DO 40 I = 1, J
               IF ( DWORK(L).LT.ZERO ) U(I,J) = -U(I,J)
               L = L + 1
   40       CONTINUE
C
            K = K + 1
   50    CONTINUE
C
      END IF
C
      DWORK(1) = MAX( WRKOPT, 4*N )
      RETURN
C *** Last line of SB03OU ***
      END
