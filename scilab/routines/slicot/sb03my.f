      SUBROUTINE SB03MY( TRANA, N, A, LDA, C, LDC, SCALE, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve the real Lyapunov matrix equation
C
C            op(A)'*X + X*op(A) = scale*C
C
C     where op(A) = A or A' (A**T), A is upper quasi-triangular and C is
C     symmetric (C = C'). (A' denotes the transpose of the matrix A.)
C     A is N-by-N, the right hand side C and the solution X are N-by-N,
C     and scale is an output scale factor, set less than or equal to 1
C     to avoid overflow in X. The solution matrix X is overwritten
C     onto C.
C
C     A must be in Schur canonical form (as returned by LAPACK routines
C     DGEES or DHSEQR), that is, block upper triangular with 1-by-1 and
C     2-by-2 diagonal blocks; each 2-by-2 diagonal block has its
C     diagonal elements equal and its off-diagonal elements of opposite
C     sign.
C
C     ARGUMENTS 
C
C     Mode Parameters
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
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             upper quasi-triangular matrix A, in Schur canonical form.
C             The part of A below the first sub-diagonal is not
C             referenced.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry, the leading N-by-N part of this array must  
C             contain the symmetric matrix C.
C             On exit, if INFO >= 0, the leading N-by-N part of this
C             array contains the symmetric solution matrix X.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,N).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if A and -A have common or very close eigenvalues;
C                   perturbed values were used to solve the equation
C                   (but the matrix A is unchanged).
C
C     METHOD
C
C     Bartels-Stewart algorithm is used. A set of equivalent linear
C     algebraic systems of equations of order at most four are formed
C     and solved using Gaussian elimination with complete pivoting.
C
C     REFERENCES
C
C     [1] Bartels, R.H. and Stewart, G.W.  T
C         Solution of the matrix equation A X + XB = C.
C         Comm. A.C.M., 15, pp. 820-826, 1972.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, May 1997.
C     Supersedes Release 2.0 routine SB03AY by Control Systems Research
C     Group, Kingston Polytechnic, United Kingdom, October 1982.
C     Based on DTRLYP by P. Petkov, Tech. University of Sofia, September
C     1993.
C
C     REVISIONS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, May 1999.
C
C     KEYWORDS
C
C     Continuous-time system, Lyapunov equation, matrix algebra, real
C     Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          TRANA
      INTEGER            INFO, LDA, LDC, N
      DOUBLE PRECISION   SCALE
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            NOTRNA, LUPPER
      INTEGER            IERR, J, K, K1, K2, KNEXT, L, L1, L2, LNEXT,
     $                   MINK1N, MINK2N, MINL1N, MINL2N
      DOUBLE PRECISION   A11, BIGNUM, DA11, DB, EPS, SCALOC, SMIN,
     $                   SMLNUM, XNORM
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM( 1 ), VEC( 2, 2 ), X( 2, 2 )
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DDOT, DLAMCH, DLANHS
      EXTERNAL           DDOT, DLAMCH, DLANHS, LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLABAD, DLALN2, DLASY2, DSCAL, SB03MW, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX, MIN
C     ..
C     .. Executable Statements ..
C
C     Decode and Test input parameters.
C
      NOTRNA = LSAME( TRANA, 'N' )
      LUPPER = .TRUE.
C
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND.
     $                      .NOT.LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -6
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB03MY', -INFO )
         RETURN
      END IF
C
      SCALE = ONE
C
C     Quick return if possible.
C
      IF( N.EQ.0 )
     $   RETURN
C
C     Set constants to control overflow.
C
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
      SMLNUM = SMLNUM*DBLE( N*N ) / EPS
      BIGNUM = ONE / SMLNUM
C
      SMIN = MAX( SMLNUM, EPS*DLANHS( 'Max', N, A, LDA, DUM ) )
C
      IF( NOTRNA ) THEN
C
C        Solve    A'*X + X*A = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        upper-left corner column by column by
C
C          A(K,K)'*X(K,L) + X(K,L)*A(L,L) = C(K,L) - R(K,L),
C
C        where
C                   K-1                    L-1
C          R(K,L) = SUM [A(I,K)'*X(I,L)] + SUM [X(K,J)*A(J,L)].
C                   I=1                    J=1
C
C        Start column loop (index = L).
C        L1 (L2): column index of the first (last) row of X(K,L).
C
         LNEXT = 1
C
         DO 60 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 60
            L1 = L
            L2 = L
            IF( L.LT.N ) THEN
               IF( A( L+1, L ).NE.ZERO )
     $            L2 = L2 + 1
               LNEXT = L2 + 1
            END IF
C
C           Start row loop (index = K).
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = L
C
            DO 50 K = L, N
               IF( K.LT.KNEXT )
     $            GO TO 50
               K1 = K
               K2 = K
               IF( K.LT.N ) THEN
                  IF( A( K+1, K ).NE.ZERO )
     $               K2 = K2 + 1
                  KNEXT = K2 + 1
               END IF
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 ) )
                  SCALOC = ONE
C
                  A11 = A( K1, K1 ) + A( L1, L1 )
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11 = SMIN
                     DA11 = SMIN
                     INFO = 1
                  END IF
                  DB = ABS( VEC( 1, 1 ) )
                  IF( DA11.LT.ONE .AND. DB.GT.ONE ) THEN
                     IF( DB.GT.BIGNUM*DA11 )
     $                  SCALOC = ONE / DB
                  END IF
                  X( 1, 1 ) = ( VEC( 1, 1 )*SCALOC ) / A11
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 10 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   10                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF  
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 ) )
C
                  VEC( 2, 1 ) = C( K2, L1 ) -
     $               ( DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L1 ), 1 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, ONE, A( K1, K1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( L1, L1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 20 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   20                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 )  
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 ) )
C
                  VEC( 2, 1 ) = C( K1, L2 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L2 ), 1 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, ONE, A( L1, L1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( K1, K1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 30 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   30                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 ) )
C
                  VEC( 1, 2 ) = C( K1, L2 ) -
     $               ( DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 ) +
     $                 DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L2 ), 1 ) )
C                 
                  VEC( 2, 1 ) = C( K2, L1 ) -
     $               ( DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 ) +
     $                 DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L1 ), 1 ) )
C
                  VEC( 2, 2 ) = C( K2, L2 ) -
     $               ( DDOT( K1-1, A( 1, K2 ), 1, C( 1, L2 ), 1 ) +
     $                 DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L2 ), 1 ) )
C
                  IF( K1.EQ.L1 ) THEN
                     CALL SB03MW( .FALSE., LUPPER, A( K1, K1 ), LDA,
     $                            VEC, 2, SCALOC, X, 2, XNORM, IERR )
                     IF( LUPPER ) THEN
                        X( 2, 1 ) = X( 1, 2 ) 
                     ELSE
                        X( 1, 2 ) = X( 2, 1 ) 
                     END IF
                  ELSE    
                     CALL DLASY2( .TRUE., .FALSE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 40 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   40                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                     C( L2, K1 ) = X( 1, 2 )
                     C( L1, K2 ) = X( 2, 1 )
                     C( L2, K2 ) = X( 2, 2 )
                  END IF   
               END IF
C
   50       CONTINUE
C
   60    CONTINUE
C
      ELSE
C
C        Solve    A*X + X*A' = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        bottom-right corner column by column by
C
C            A(K,K)*X(K,L) + X(K,L)*A(L,L)' = C(K,L) - R(K,L),
C
C        where
C                      N                     N
C            R(K,L) = SUM [A(K,I)*X(I,L)] + SUM [X(K,J)*A(L,J)'].
C                    I=K+1                 J=L+1
C
C        Start column loop (index = L).
C        L1 (L2): column index of the first (last) row of X(K,L).
C
         LNEXT = N
C
         DO 120 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 120
            L1 = L
            L2 = L
            IF( L.GT.1 ) THEN
               IF( A( L, L-1 ).NE.ZERO )
     $            L1 = L1 - 1
               LNEXT = L1 - 1
            END IF
            MINL1N = MIN( L1+1, N )
            MINL2N = MIN( L2+1, N )
C
C           Start row loop (index = K).
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = L
C
            DO 110 K = L, 1, -1
               IF( K.GT.KNEXT )
     $            GO TO 110
               K1 = K
               K2 = K
               IF( K.GT.1 ) THEN
                  IF( A( K, K-1 ).NE.ZERO )
     $               K1 = K1 - 1
                  KNEXT = K1 - 1
               END IF
               MINK1N = MIN( K1+1, N )
               MINK2N = MIN( K2+1, N )
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( N-K1, A( K1, MINK1N ), LDA,
     $                       C( MINK1N, L1 ), 1 ) +
     $                 DDOT( N-L1, C( K1, MINL1N ), LDC,
     $                       A( L1, MINL1N ), LDA ) )
                  SCALOC = ONE
C
                  A11 = A( K1, K1 ) + A( L1, L1 )
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11 = SMIN
                     DA11 = SMIN
                     INFO = 1
                  END IF
                  DB = ABS( VEC( 1, 1 ) )
                  IF( DA11.LT.ONE .AND. DB.GT.ONE ) THEN
                     IF( DB.GT.BIGNUM*DA11 )
     $                  SCALOC = ONE / DB
                  END IF
                  X( 1, 1 ) = ( VEC( 1, 1 )*SCALOC ) / A11
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 70 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   70                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF   
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( N-K2, A( K1, MINK2N ), LDA,
     $                       C( MINK2N, L1 ), 1 ) +
     $                 DDOT( N-L2, C( K1, MINL2N ), LDC,
     $                       A( L1, MINL2N ), LDA ) )
C
                  VEC( 2, 1 ) = C( K2, L1 ) -
     $               ( DDOT( N-K2, A( K2, MINK2N ), LDA,
     $                     C( MINK2N, L1 ), 1 ) +
     $                 DDOT( N-L2, C( K2, MINL2N ), LDC,
     $                     A( L1, MINL2N ), LDA ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, ONE, A( K1, K1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( L1, L1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 80 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   80                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 ) 
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( N-K1, A( K1, MINK1N ), LDA,
     $                       C( MINK1N, L1 ), 1 ) +
     $                 DDOT( N-L2, C( K1, MINL2N ), LDC,
     $                       A( L1, MINL2N ), LDA ) )
C
                  VEC( 2, 1 ) = C( K1, L2 ) -
     $               ( DDOT( N-K1, A( K1, MINK1N ), LDA,
     $                       C( MINK1N, L2 ), 1 ) +
     $                 DDOT( N-L2, C( K1, MINL2N ), LDC,
     $                       A( L2, MINL2N ), LDA ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, ONE, A( L1, L1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( K1, K1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 90 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   90                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  VEC( 1, 1 ) = C( K1, L1 ) -
     $               ( DDOT( N-K2, A( K1, MINK2N ), LDA,
     $                       C( MINK2N, L1 ), 1 ) +
     $                 DDOT( N-L2, C( K1, MINL2N ), LDC,
     $                       A( L1, MINL2N ), LDA ) )
C
                  VEC( 1, 2 ) = C( K1, L2 ) -
     $               ( DDOT( N-K2, A( K1, MINK2N ), LDA,
     $                       C( MINK2N, L2 ), 1 ) +
     $                 DDOT( N-L2, C( K1, MINL2N ), LDC,
     $                       A( L2, MINL2N ), LDA ) )
C                 
                  VEC( 2, 1 ) = C( K2, L1 ) -
     $               ( DDOT( N-K2, A( K2, MINK2N ), LDA,
     $                       C( MINK2N, L1 ), 1 ) +
     $                 DDOT( N-L2, C( K2, MINL2N ), LDC,
     $                       A( L1, MINL2N ), LDA ) )
C
                  VEC( 2, 2 ) = C( K2, L2 ) -
     $               ( DDOT( N-K2, A( K2, MINK2N ), LDA,
     $                       C( MINK2N, L2 ), 1 ) +
     $                 DDOT( N-L2, C( K2, MINL2N ), LDC,
     $                       A( L2, MINL2N ), LDA ) )
C
                  IF( K1.EQ.L1 ) THEN
                     CALL SB03MW( .TRUE., LUPPER, A( K1, K1 ), LDA, VEC,
     $                            2, SCALOC, X, 2, XNORM, IERR )
                     IF( LUPPER ) THEN
                        X( 2, 1 ) = X( 1, 2 ) 
                     ELSE
                        X( 1, 2 ) = X( 2, 1 ) 
                     END IF
                  ELSE  
                     CALL DLASY2( .FALSE., .TRUE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 100 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
  100                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                     C( L2, K1 ) = X( 1, 2 )
                     C( L1, K2 ) = X( 2, 1 )
                     C( L2, K2 ) = X( 2, 2 )
                  END IF  
               END IF
C
  110       CONTINUE
C
  120    CONTINUE
C
      END IF
C
      RETURN
C *** Last line of SB03MY ***
      END
