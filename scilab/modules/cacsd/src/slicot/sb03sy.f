      SUBROUTINE SB03SY( JOB, TRANA, LYAPUN, N, T, LDT, U, LDU, XA,
     $                   LDXA, SEPD, THNORM, IWORK, DWORK, LDWORK, 
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To estimate the "separation" between the matrices op(A) and
C     op(A)',
C
C     sepd(op(A),op(A)') = min norm(op(A)'*X*op(A) - X)/norm(X)
C                        = 1 / norm(inv(Omega))
C
C     and/or the 1-norm of Theta, where op(A) = A or A' (A**T), and
C     Omega and Theta are linear operators associated to the real 
C     discrete-time Lyapunov matrix equation
C
C            op(A)'*X*op(A) - X = C,
C
C     defined by
C
C     Omega(W) = op(A)'*W*op(A) - W,
C     Theta(W) = inv(Omega(op(W)'*X*op(A) + op(A)'*X*op(W))).
C
C     The 1-norm condition estimators are used.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     JOB     CHARACTER*1
C             Specifies the computation to be performed, as follows:
C             = 'S':  Compute the separation only;
C             = 'T':  Compute the norm of Theta only;
C             = 'B':  Compute both the separation and the norm of Theta.
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     LYAPUN  CHARACTER*1
C             Specifies whether or not the original Lyapunov equations 
C             should be solved, as follows:
C             = 'O':  Solve the original Lyapunov equations, updating
C                     the right-hand sides and solutions with the
C                     matrix U, e.g., X <-- U'*X*U;
C             = 'R':  Solve reduced Lyapunov equations only, without
C                     updating the right-hand sides and solutions.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A and X.  N >= 0.
C
C     T       (input) DOUBLE PRECISION array, dimension (LDT,N)
C             The leading N-by-N upper Hessenberg part of this array
C             must contain the upper quasi-triangular matrix T in Schur
C             canonical form from a Schur factorization of A.
C
C     LDT     INTEGER
C             The leading dimension of array T.  LDT >= MAX(1,N).
C
C     U       (input) DOUBLE PRECISION array, dimension (LDU,N)
C             The leading N-by-N part of this array must contain the
C             orthogonal matrix U from a real Schur factorization of A.
C             If LYAPUN = 'R', the array U is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of array U.  
C             LDU >= 1,        if LYAPUN = 'R';
C             LDU >= MAX(1,N), if LYAPUN = 'O'.
C
C     XA      (input) DOUBLE PRECISION array, dimension (LDXA,N)
C             The leading N-by-N part of this array must contain the
C             matrix product X*op(A), if LYAPUN = 'O', or U'*X*U*op(T), 
C             if LYAPUN = 'R', in the Lyapunov equation.
C             If JOB = 'S', the array XA is not referenced.
C
C     LDXA    INTEGER
C             The leading dimension of array XA.
C             LDXA >= 1,        if JOB = 'S';
C             LDXA >= MAX(1,N), if JOB = 'T' or 'B'.
C
C     SEPD    (output) DOUBLE PRECISION
C             If JOB = 'S' or JOB = 'B', and INFO >= 0, SEPD contains
C             the estimated quantity sepd(op(A),op(A)').
C             If JOB = 'T' or N = 0, SEPD is not referenced.
C
C     THNORM  (output) DOUBLE PRECISION
C             If JOB = 'T' or JOB = 'B', and INFO >= 0, THNORM contains
C             the estimated 1-norm of operator Theta.
C             If JOB = 'S' or N = 0, THNORM is not referenced.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= 0,            if N = 0;
C             LDWORK >= MAX(3,2*N*N), if N > 0.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             = N+1:  if T has (almost) reciprocal eigenvalues;
C                   perturbed values were used to solve Lyapunov
C                   equations (but the matrix T is unchanged).
C
C     METHOD
C
C     SEPD is defined as
C
C            sepd( op(A), op(A)' ) = sigma_min( K )
C
C     where sigma_min(K) is the smallest singular value of the
C     N*N-by-N*N matrix
C
C        K = kprod( op(A)', op(A)' ) - I(N**2).
C
C     I(N**2) is an N*N-by-N*N identity matrix, and kprod denotes the
C     Kronecker product. The routine estimates sigma_min(K) by the
C     reciprocal of an estimate of the 1-norm of inverse(K), computed as
C     suggested in [1]. This involves the solution of several discrete-
C     time Lyapunov equations, either direct or transposed. The true
C     reciprocal 1-norm of inverse(K) cannot differ from sigma_min(K) by
C     more than a factor of N.
C     The 1-norm of Theta is estimated similarly.
C
C     REFERENCES
C
C     [1] Higham, N.J.
C         FORTRAN codes for estimating the one-norm of a real or
C         complex matrix, with applications to condition estimation.
C         ACM Trans. Math. Softw., 14, pp. 381-396, 1988.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations.
C
C     FURTHER COMMENTS
C
C     When SEPD is zero, the routine returns immediately, with THNORM
C     (if requested) not set. In this case, the equation is singular.
C     The option LYAPUN = 'R' may occasionally produce slightly worse
C     or better estimates, and it is much faster than the option 'O'.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Romania,
C     Oct. 1998. Partly based on DDLSVX (and then SB03SD) by P. Petkov,
C     Tech. University of Sofia, March 1998 (and December 1998).
C
C     REVISIONS
C
C     February 6, 1999, V. Sima, Katholieke Univ. Leuven, Belgium.  
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation, real Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          JOB, LYAPUN, TRANA
      INTEGER            INFO, LDT, LDU, LDWORK, LDXA, N
      DOUBLE PRECISION   SEPD, THNORM
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   DWORK( * ), T( LDT, * ), U( LDU, * ),
     $                   XA( LDXA, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            NOTRNA, UPDATE, WANTS, WANTT
      CHARACTER          TRANAT, UPLO
      INTEGER            INFO2, ITMP, KASE, NN
      DOUBLE PRECISION   BIGNUM, EST, SCALE
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY, LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLACON, DLACPY, DSYR2K, MA02ED, MB01RU, SB03MX,
     $                   XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          MAX
C     ..
C     .. Executable Statements ..
C
C     Decode and Test input parameters.
C
      WANTS  = LSAME( JOB,    'S' )
      WANTT  = LSAME( JOB,    'T' )
      NOTRNA = LSAME( TRANA,  'N' )
      UPDATE = LSAME( LYAPUN, 'O' )
C
      NN   = N*N
      INFO = 0
      IF( .NOT. ( WANTS .OR. WANTT .OR. LSAME( JOB, 'B' ) ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( NOTRNA .OR. LSAME( TRANA, 'T' ) .OR.
     $                            LSAME( TRANA, 'C' ) ) ) THEN
         INFO = -2           
      ELSE IF( .NOT.( UPDATE .OR. LSAME( LYAPUN, 'R' ) ) ) THEN
         INFO = -3           
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -6
      ELSE IF( LDU.LT.1 .OR. ( UPDATE .AND. LDU.LT.N ) ) THEN
         INFO = -8
      ELSE IF( LDXA.LT.1 .OR. ( .NOT.WANTS .AND. LDXA.LT.N ) ) THEN
         INFO = -10
      ELSE IF( LDWORK.LT.0 .OR.
     $       ( LDWORK.LT.MAX( 3, 2*NN ) .AND. N.GT.0 ) ) THEN
         INFO = -15
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB03SY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 )
     $   RETURN
C
      ITMP = NN + 1
C
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
C
      IF( .NOT.WANTT ) THEN
C
C        Estimate sepd(op(A),op(A)').
C        Workspace:  max(3,2*N*N).
C
         KASE = 0
C
C        REPEAT
   10    CONTINUE
         CALL DLACON( NN, DWORK( ITMP ), DWORK, IWORK, EST, KASE )
         IF( KASE.NE.0 ) THEN
C
C           Select the triangular part of symmetric matrix to be used.
C
            IF( DLANSY( '1-norm', 'Upper', N, DWORK, N, DWORK( ITMP ) ) 
     $          .GE.
     $          DLANSY( '1-norm', 'Lower', N, DWORK, N, DWORK( ITMP ) )
     $        ) THEN
               UPLO = 'U'
            ELSE
               UPLO = 'L'
            END IF
C
            IF( UPDATE ) THEN
C
C              Transform the right-hand side: RHS := U'*RHS*U.
C              
               CALL MB01RU( UPLO, 'Transpose', N, N, ZERO, ONE, DWORK,
     $                      N, U, LDU, DWORK, N, DWORK( ITMP ), NN, 
     $                      INFO2 )
            END IF
            CALL MA02ED( UPLO, N, DWORK, N )
C
            IF( KASE.EQ.1 ) THEN
C
C              Solve op(T)'*Y*op(T) - Y = scale*RHS.
C
               CALL SB03MX( TRANA, N, T, LDT, DWORK, N, SCALE,
     $                      DWORK( ITMP ), INFO2 )
            ELSE
C
C              Solve op(T)*W*op(T)' - W = scale*RHS.
C
               CALL SB03MX( TRANAT, N, T, LDT, DWORK, N, SCALE,
     $                      DWORK( ITMP ), INFO2 )
            END IF
C
            IF( INFO2.GT.0 )
     $         INFO = N + 1
C
            IF( UPDATE ) THEN
C
C              Transform back to obtain the solution: Z := U*Z*U', with
C              Z = Y or Z = W.
C
               CALL MB01RU( UPLO, 'No transpose', N, N, ZERO, ONE,
     $                      DWORK, N, U, LDU, DWORK, N, DWORK( ITMP ),
     $                      NN, INFO2 )
C
C              Fill in the remaining triangle of the symmetric matrix.
C
               CALL MA02ED( UPLO, N, DWORK, N )
            END IF
C
            GO TO 10
         END IF
C        UNTIL KASE = 0
C
         IF( EST.GT.SCALE ) THEN
            SEPD = SCALE / EST
         ELSE
            BIGNUM = ONE / DLAMCH( 'Safe minimum' )
            IF( SCALE.LT.EST*BIGNUM ) THEN
               SEPD = SCALE / EST
            ELSE
               SEPD = BIGNUM
            END IF
         END IF
C
C        Return if the equation is singular.
C
         IF( SEPD.EQ.ZERO )
     $      RETURN
      END IF
C
      IF( .NOT.WANTS ) THEN
C
C        Estimate norm(Theta).
C        Workspace:  max(3,2*N*N).
C
         KASE = 0
C
C        REPEAT
   20    CONTINUE
         CALL DLACON( NN, DWORK( ITMP ), DWORK, IWORK, EST, KASE )
         IF( KASE.NE.0 ) THEN
C
C           Select the triangular part of symmetric matrix to be used.
C
            IF( DLANSY( '1-norm', 'Upper', N, DWORK, N, DWORK( ITMP ) ) 
     $          .GE.
     $          DLANSY( '1-norm', 'Lower', N, DWORK, N, DWORK( ITMP ) )
     $        ) THEN
               UPLO = 'U'
            ELSE
               UPLO = 'L'
            END IF
C
C           Fill in the remaining triangle of the symmetric matrix.
C           
            CALL MA02ED( UPLO, N, DWORK, N )
C
C           Compute RHS = op(W)'*X*op(A) + op(A)'*X*op(W).
C
            CALL DSYR2K( UPLO, TRANAT, N, N, ONE, DWORK, N, XA, LDXA,
     $                   ZERO, DWORK( ITMP ), N )
            CALL DLACPY( UPLO, N, N, DWORK( ITMP ), N, DWORK, N )
C
            IF( UPDATE ) THEN
C
C              Transform the right-hand side: RHS := U'*RHS*U.
C              
               CALL MB01RU( UPLO, 'Transpose', N, N, ZERO, ONE, DWORK,
     $                      N, U, LDU, DWORK, N, DWORK( ITMP ), NN,
     $                      INFO2 )
            END IF
            CALL MA02ED( UPLO, N, DWORK, N )
C
            IF( KASE.EQ.1 ) THEN
C
C              Solve op(T)'*Y*op(T) - Y = scale*RHS.
C
               CALL SB03MX( TRANA, N, T, LDT, DWORK, N, SCALE, 
     $                      DWORK( ITMP ), INFO2 )
            ELSE
C
C              Solve op(T)*W*op(T)' - W = scale*RHS.
C
               CALL SB03MX( TRANAT, N, T, LDT, DWORK, N, SCALE, 
     $                      DWORK( ITMP ), INFO2 )
            END IF
C
            IF( INFO2.GT.0 )
     $         INFO = N + 1
C
            IF( UPDATE ) THEN
C
C              Transform back to obtain the solution: Z := U*Z*U', with
C              Z = Y or Z = W.
C              
               CALL MB01RU( UPLO, 'No transpose', N, N, ZERO, ONE,
     $                      DWORK, N, U, LDU, DWORK, N, DWORK( ITMP ),
     $                      NN, INFO2 )
C
C              Fill in the remaining triangle of the symmetric matrix.
C
               CALL MA02ED( UPLO, N, DWORK, N )
            END IF
C
            GO TO 20
         END IF
C        UNTIL KASE = 0
C
         IF( EST.LT.SCALE ) THEN
            THNORM = EST / SCALE
         ELSE
            BIGNUM = ONE / DLAMCH( 'Safe minimum' )
            IF( EST.LT.SCALE*BIGNUM ) THEN
               THNORM = EST / SCALE
            ELSE
               THNORM = BIGNUM
            END IF
         END IF
      END IF 
C
      RETURN
C *** Last line of SB03SY ***
      END
