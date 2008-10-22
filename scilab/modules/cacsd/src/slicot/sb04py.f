      SUBROUTINE SB04PY( TRANA, TRANB, ISGN, M, N, A, LDA, B, LDB, C,
     $                   LDC, SCALE, DWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To solve for X the discrete-time Sylvester equation
C
C        op(A)*X*op(B) + ISGN*X = scale*C,
C
C     where op(A) = A or A**T, A and B are both upper quasi-triangular,
C     and ISGN = 1 or -1. A is M-by-M and B is N-by-N; the right hand 
C     side C and the solution X are M-by-N; and scale is an output scale
C     factor, set less than or equal to 1 to avoid overflow in X. The
C     solution matrix X is overwritten onto C.
C
C     A and B must be in Schur canonical form (as returned by LAPACK
C     Library routine DHSEQR), that is, block upper triangular with
C     1-by-1 and 2-by-2 diagonal blocks; each 2-by-2 diagonal block has
C     its diagonal elements equal and its off-diagonal elements of
C     opposite sign.
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
C     TRANB   CHARACTER*1
C             Specifies the form of op(B) to be used, as follows:
C             = 'N':  op(B) = B    (No transpose);
C             = 'T':  op(B) = B**T (Transpose);
C             = 'C':  op(B) = B**T (Conjugate transpose = Transpose).
C
C     ISGN    INTEGER
C             Specifies the sign of the equation as described before.
C             ISGN may only be 1 or -1.
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The order of the matrix A, and the number of rows in the
C             matrices X and C.  M >= 0.
C
C     N       (input) INTEGER
C             The order of the matrix B, and the number of columns in
C             the matrices X and C.  N >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,M)
C             The leading M-by-M part of this array must contain the
C             upper quasi-triangular matrix A, in Schur canonical form.
C             The part of A below the first sub-diagonal is not
C             referenced.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,M).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,N)
C             The leading N-by-N part of this array must contain the
C             upper quasi-triangular matrix B, in Schur canonical form.
C             The part of B below the first sub-diagonal is not
C             referenced.
C
C     LDB     (input) INTEGER
C             The leading dimension of the array B.  LDB >= max(1,N).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
C             On entry, the leading M-by-N part of this array must  
C             contain the right hand side matrix C.
C             On exit, if INFO >= 0, the leading M-by-N part of this
C             array contains the solution matrix X.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,M).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (2*M)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  A and -ISGN*B have almost reciprocal eigenvalues;
C                   perturbed values were used to solve the equation
C                   (but the matrices A and B are unchanged).
C
C     METHOD
C
C     The solution matrix X is computed column-wise via a back
C     substitution scheme, an extension and refinement of the algorithm
C     in [1], similar to that used in [2] for continuous-time Sylvester
C     equations. A set of equivalent linear algebraic systems of 
C     equations of order at most four are formed and solved using 
C     Gaussian elimination with complete pivoting.
C
C     REFERENCES
C
C     [1] Bartels, R.H. and Stewart, G.W.  T
C         Solution of the matrix equation A X + XB = C.
C         Comm. A.C.M., 15, pp. 820-826, 1972.
C
C     [2] Anderson, E., Bai, Z., Bischof, C., Demmel, J., Dongarra, J.,
C         Du Croz, J., Greenbaum, A., Hammarling, S., McKenney, A.,
C         Ostrouchov, S., and Sorensen, D.
C         LAPACK Users' Guide: Second Edition.
C         SIAM, Philadelphia, 1995.
C
C     NUMERICAL ASPECTS
C
C     The algorithm is stable and reliable, since Gaussian elimination
C     with complete pivoting is used.
C
C     CONTRIBUTORS
C
C     A. Varga, German Aerospace Center, Oberpfaffenhofen, March 2000.
C     D. Sima, University of Bucharest, April 2000.
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 2000.
C     Partly based on the routine SYLSV, A. Varga, 1992.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Discrete-time system, matrix algebra, Sylvester equation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          TRANA, TRANB
      INTEGER            INFO, ISGN, LDA, LDB, LDC, M, N
      DOUBLE PRECISION   SCALE
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), C( LDC, * ),
     $                   DWORK( * )
C     ..
C     .. Local Scalars ..
      LOGICAL            NOTRNA, NOTRNB
      INTEGER            IERR, J, K, K1, K2, KNEXT, L, L1, L2, LNEXT, 
     $                   MNK1, MNK2, MNL1, MNL2
      DOUBLE PRECISION   A11, BIGNUM, DA11, DB, EPS, P11, P12, P21, P22,
     $                   SCALOC, SGN, SMIN, SMLNUM, SUMR, XNORM
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM( 1 ), VEC( 2, 2 ), X( 2, 2 )
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DDOT, DLAMCH, DLANGE
      EXTERNAL           DDOT, DLAMCH, DLANGE, LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLABAD, DLALN2, DSCAL, SB04PX, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX, MIN
C     ..
C     .. Executable Statements ..
C
C     Decode and Test input parameters
C
      NOTRNA = LSAME( TRANA, 'N' )
      NOTRNB = LSAME( TRANB, 'N' )
C
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND.
     $    .NOT.LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.NOTRNB .AND. .NOT.LSAME( TRANB, 'T' ) .AND.
     $         .NOT.LSAME( TRANB, 'C' ) ) THEN
         INFO = -2
      ELSE IF( ISGN.NE.1 .AND. ISGN.NE.-1 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -7
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDC.LT.MAX( 1, M ) ) THEN
         INFO = -11
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB04PY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      SCALE = ONE
      IF( M.EQ.0 .OR. N.EQ.0 )
     $   RETURN
C
C     Set constants to control overflow.
C
      EPS    = DLAMCH( 'Precision' )
      SMLNUM = DLAMCH( 'Safe minimum' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
      SMLNUM = SMLNUM*DBLE( M*N ) / EPS
      BIGNUM = ONE / SMLNUM
C
      SMIN = MAX( SMLNUM, EPS*DLANGE( 'M', M, M, A, LDA, DUM ),
     $                    EPS*DLANGE( 'M', N, N, B, LDB, DUM ) )
C
      SGN = ISGN
C
      IF( NOTRNA .AND. NOTRNB ) THEN
C
C        Solve    A*X*B + ISGN*X = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        bottom-left corner column by column by
C
C           A(K,K)*X(K,L)*B(L,L) + ISGN*X(K,L) = C(K,L) - R(K,L)
C
C        where
C                       M
C           R(K,L) = { SUM [A(K,J)*X(J,L)] } * B(L,L) +
C                     J=K+1
C                       M             L-1
C                      SUM { A(K,J) * SUM [X(J,I)*B(I,L)] }.
C                      J=K            I=1
C
C        Start column loop (index = L)
C        L1 (L2) : column index of the first (last) row of X(K,L).
C
         LNEXT = 1
C
         DO 60 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 60
            L1 = L
            IF( L.EQ.N ) THEN
               L2 = L
            ELSE
               IF( B( L+1, L ).NE.ZERO ) THEN
                  L2 = L + 1
               ELSE
                  L2 = L
               END IF
               LNEXT = L2 + 1
            END IF
C
C           Start row loop (index = K)
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = M
C
            DO 50 K = M, 1, -1
               IF( K.GT.KNEXT )
     $            GO TO 50
               K2 = K
               IF( K.EQ.1 ) THEN
                  K1 = K
               ELSE
                  IF( A( K, K-1 ).NE.ZERO ) THEN
                     K1 = K - 1
                  ELSE
                     K1 = K
                  END IF
                  KNEXT = K1 - 1
               END IF
C
               MNK1 = MIN( K1+1, M )
               MNK2 = MIN( K2+1, M )
               P11  = DDOT( M-K2, A( K1, MNK2 ), LDA, C( MNK2, L1 ), 1 )
               DWORK( K1 ) = DDOT( L1-1, C( K1, 1 ), LDC, B( 1, L1 ),
     $                             1 )
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
C
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
                  SCALOC = ONE
C
                  A11  = A( K1, K1 )*B( L1, L1 ) + SGN
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11  = SMIN
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
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   10                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L1 ),
     $                        1 )
                  DWORK( K2 ) = DDOT( L1-1, C( K2, 1 ), LDC, B( 1, L1 ),
     $                                1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, B( L1, L1 ),
     $                         A( K1, K1 ), LDA, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 20 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   20                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  P12  = DDOT( M-K1, A( K1, MNK1 ), LDA, C( MNK1, L2 ),
     $                         1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L2, L1 ) )
C
                  DWORK( K1+M ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUMR + P11*B( L1, L2 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, A( K1, K1 ),
     $                         B( L1, L1 ), LDB, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 30 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   30                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1+M ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L1 ),
     $                        1 )
                  P12 = DDOT( M-K2, A( K1, MNK2 ), LDA, C( MNK2, L2 ),
     $                        1 )
                  P22 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L2 ),
     $                        1 )
C
                  DWORK( K2 ) = DDOT( L1-1, C( K2, 1 ), LDC, B( 1, L1 ),
     $                                1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L2, L1 ) )
C
                  DWORK( K1+M ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  DWORK( K2+M ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUMR + P11*B( L1, L2 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) +
     $                                                 P22*B( L2, L1 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUMR + P21*B( L1, L2 ) +
     $                                                 P22*B( L2, L2 ) )
C
                  CALL SB04PX( .FALSE., .FALSE., ISGN, 2, 2,
     $                         A( K1, K1 ), LDA, B( L1, L1 ), LDB, VEC,
     $                         2, SCALOC, X, 2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 40 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   40                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1+M ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
               END IF
C
   50       CONTINUE
C
   60    CONTINUE
C
      ELSE IF( .NOT.NOTRNA .AND. NOTRNB ) THEN
C
C        Solve     A'*X*B + ISGN*X = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        upper-left corner column by column by
C
C         A(K,K)'*X(K,L)*B(L,L) + ISGN*X(K,L) = C(K,L) - R(K,L)
C
C        where
C                      K-1
C           R(K,L) = { SUM [A(J,K)'*X(J,L)] } * B(L,L) +
C                      J=1
C                       K              L-1
C                      SUM A(J,K)' * { SUM [X(J,I)*B(I,L)] }.
C                      J=1             I=1
C
C        Start column loop (index = L)
C        L1 (L2): column index of the first (last) row of X(K,L).
C
         LNEXT = 1
C
         DO 120 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 120
            L1 = L
            IF( L.EQ.N ) THEN
               L2 = L
            ELSE
               IF( B( L+1, L ).NE.ZERO ) THEN
                  L2 = L + 1
               ELSE
                  L2 = L
               END IF
               LNEXT = L2 + 1
            END IF
C
C           Start row loop (index = K)
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = 1
C
            DO 110 K = 1, M
               IF( K.LT.KNEXT )
     $            GO TO 110
               K1 = K
               IF( K.EQ.M ) THEN
                  K2 = K
               ELSE
                  IF( A( K+1, K ).NE.ZERO ) THEN
                     K2 = K + 1
                  ELSE
                     K2 = K
                  END IF
                  KNEXT = K2 + 1
               END IF
C
               P11 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
               DWORK( K1 ) = DDOT( L1-1, C( K1, 1 ), LDC, B( 1, L1),
     $                             1 )
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN                  
C
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
                  SCALOC = ONE
C
                  A11  = A( K1, K1 )*B( L1, L1 ) + SGN
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11  = SMIN
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
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   70                CONTINUE
C
                     CALL DSCAL( K1, SCALOC, DWORK, 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  DWORK( K2 ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                                B( 1, L1), 1 )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK, 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, B( L1, L1 ),
     $                         A( K1, K1 ), LDA, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 80 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   80                CONTINUE
C
                     CALL DSCAL( K2, SCALOC, DWORK, 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  P12  = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L2, L1 ) )
C
                  DWORK( K1+M ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK( M+1 ), 1 )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUMR + P11*B( L1, L2 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, A( K1, K1 ),
     $                         B( L1, L1 ), LDB, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 90 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
   90                CONTINUE
C
                     CALL DSCAL( K1, SCALOC, DWORK, 1 )
                     CALL DSCAL( K1, SCALOC, DWORK( M+1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  P12 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  P22 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L2 ), 1 )
C
                  DWORK( K2 ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                                B( 1, L1), 1 )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L2, L1 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK, 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) +
     $                                                 P22*B( L2, L1 ) )
C
                  DWORK( K1+M ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  DWORK( K2+M ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                                  B( 1, L2 ), 1 )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK( M+1 ), 1 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUMR + P11*B( L1, L2 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK( M+1 ), 1 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUMR + P21*B( L1, L2 ) +
     $                                                 P22*B( L2, L2 ) )
C
                  CALL SB04PX( .TRUE., .FALSE., ISGN, 2, 2, A( K1, K1 ),
     $                         LDA, B( L1, L1 ), LDB, VEC, 2, SCALOC, X,
     $                         2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 100 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  100                CONTINUE
C
                     CALL DSCAL( K2, SCALOC, DWORK, 1 )
                     CALL DSCAL( K2, SCALOC, DWORK( M+1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
               END IF
C
  110       CONTINUE
C
  120    CONTINUE
C
      ELSE IF( .NOT.NOTRNA .AND. .NOT.NOTRNB ) THEN
C
C        Solve    A'*X*B' + ISGN*X = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        top-right corner column by column by
C
C           A(K,K)'*X(K,L)*B(L,L)' + ISGN*X(K,L) = C(K,L) - R(K,L)
C
C        where
C                      K-1
C           R(K,L) = { SUM [A(J,K)'*X(J,L)] } * B(L,L)' +
C                      J=1
C                       K               N
C                      SUM A(J,K)' * { SUM [X(J,I)*B(L,I)'] }.
C                      J=1            I=L+1
C
C        Start column loop (index = L)
C        L1 (L2): column index of the first (last) row of X(K,L).
C
         LNEXT = N
C
         DO 180 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 180
            L2 = L
            IF( L.EQ.1 ) THEN
               L1 = L
            ELSE
               IF( B( L, L-1 ).NE.ZERO ) THEN
                  L1 = L - 1
               ELSE
                  L1 = L
               END IF
               LNEXT = L1 - 1
            END IF
C
C           Start row loop (index = K)
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = 1
C
            DO 170 K = 1, M
               IF( K.LT.KNEXT )
     $            GO TO 170
               K1 = K
               IF( K.EQ.M ) THEN
                  K2 = K
               ELSE
                  IF( A( K+1, K ).NE.ZERO ) THEN
                     K2 = K + 1
                  ELSE
                     K2 = K
                  END IF
                  KNEXT = K2 + 1
               END IF
C
               MNL1 = MIN( L1+1, N )
               MNL2 = MIN( L2+1, N )
               P11  = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
               DWORK( K1 ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                             B( L1, MNL2 ), LDB )
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN                  
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
                  SCALOC = ONE
C
                  A11  = A( K1, K1 )*B( L1, L1 ) + SGN
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11  = SMIN
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
                     DO 130 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  130                CONTINUE
C
                     CALL DSCAL( K1, SCALOC, DWORK, 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  DWORK( K2 ) = DDOT( N-L1, C( K2, MNL1 ), LDC,
     $                                B( L1, MNL1 ), LDB )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK, 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) )
C
                  CALL DLALN2( .TRUE., 2, 1, SMIN, B( L1, L1 ),
     $                         A( K1, K1 ), LDA, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 140 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  140                CONTINUE
C
                     CALL DSCAL( K2, SCALOC, DWORK, 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  P12  = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L1, L2 ) )
C
                  DWORK( K1+M ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  SUMR = DDOT( K1, A( 1, K1 ), 1, DWORK( M+1 ), 1 )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUMR + P11*B( L2, L1 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, A( K1, K1 ),
     $                         B( L1, L1 ), LDB, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 150 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  150                CONTINUE
C
                     CALL DSCAL( K1, SCALOC, DWORK, 1 )
                     CALL DSCAL( K1, SCALOC, DWORK(M+1), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  P12 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  P22 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L2 ), 1 )
C
                  DWORK( K2 ) = DDOT( N-L2, C( K2, MNL2 ), LDC,
     $                                B( L1, MNL2 ), LDB )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK, 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L1, L2 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK, 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) +
     $                                                 P22*B( L1, L2 ) )
C
                  DWORK( K1+M ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  DWORK( K2+M ) = DDOT( N-L2, C( K2, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  SUMR = DDOT( K2, A( 1, K1 ), 1, DWORK( M+1 ), 1 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUMR + P11*B( L2, L1 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  SUMR = DDOT( K2, A( 1, K2 ), 1, DWORK( M+1 ), 1 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUMR + P21*B( L2, L1 ) +
     $                                                 P22*B( L2, L2 ) )
C
                  CALL SB04PX( .TRUE., .TRUE., ISGN, 2, 2, A( K1, K1 ),
     $                         LDA, B( L1, L1 ), LDB, VEC, 2, SCALOC, X,
     $                         2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 160 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  160                CONTINUE
C
                     CALL DSCAL( K2, SCALOC, DWORK, 1 )
                     CALL DSCAL( K2, SCALOC, DWORK(M+1), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
               END IF
C
  170       CONTINUE
C
  180    CONTINUE
C
      ELSE
C
C        Solve    A*X*B' + ISGN*X = scale*C.
C
C        The (K,L)th block of X is determined starting from
C        bottom-right corner column by column by
C
C            A(K,K)*X(K,L)*B(L,L)' + ISGN*X(K,L) = C(K,L) - R(K,L)
C
C        where
C                       M
C           R(K,L) = { SUM [A(K,J)*X(J,L)] } * B(L,L)' +
C                     J=K+1
C                       M              N
C                      SUM { A(K,J) * SUM [X(J,I)*B(L,I)'] }.
C                      J=K           I=L+1
C
C        Start column loop (index = L)
C        L1 (L2): column index of the first (last) row of X(K,L).
C
         LNEXT = N
C
         DO 240 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 240
            L2 = L
            IF( L.EQ.1 ) THEN
               L1 = L
            ELSE
               IF( B( L, L-1 ).NE.ZERO ) THEN
                  L1 = L - 1
               ELSE
                  L1 = L
               END IF
               LNEXT = L1 - 1
            END IF
C
C           Start row loop (index = K)
C           K1 (K2): row index of the first (last) row of X(K,L).
C
            KNEXT = M
C
            DO 230 K = M, 1, -1
               IF( K.GT.KNEXT )
     $            GO TO 230
               K2 = K
               IF( K.EQ.1 ) THEN
                  K1 = K
               ELSE
                  IF( A( K, K-1 ).NE.ZERO ) THEN
                     K1 = K - 1
                  ELSE
                     K1 = K
                  END IF
                  KNEXT = K1 - 1
               END IF
C
               MNK1 = MIN( K1+1, M )
               MNK2 = MIN( K2+1, M )
               MNL1 = MIN( L1+1, N )
               MNL2 = MIN( L2+1, N )
               P11  = DDOT( M-K2, A( K1, MNK2 ), LDA, C( MNK2, L1 ), 1 )
               DWORK( K1 ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                             B( L1, MNL2 ), LDB )
C
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
C
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
                  SCALOC = ONE
C
                  A11  = A( K1, K1 )*B( L1, L1 ) + SGN
                  DA11 = ABS( A11 )
                  IF( DA11.LE.SMIN ) THEN
                     A11  = SMIN
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
                     DO 190 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  190                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
C
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L1 ),
     $                        1 )
                  DWORK( K2 ) = DDOT( N-L1, C( K2, MNL1 ), LDC,
     $                                B( L1, MNL1 ), LDB )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, B( L1, L1 ),
     $                         A( K1, K1 ), LDA, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 200 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  200                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
C
                  P12  = DDOT( M-K1, A( K1, MNK1 ), LDA, C( MNK1, L2 ),
     $                         1 )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L1, L2 ) )
C
                  DWORK( K1+M ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUMR + P11*B( L2, L1 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  CALL DLALN2( .FALSE., 2, 1, SMIN, A( K1, K1 ),
     $                         B( L1, L1 ), LDB, ONE, ONE, VEC, 2, -SGN,
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 210 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  210                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1+M ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
C
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
C
                  P21 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L1 ),
     $                        1 )
                  P12 = DDOT( M-K2, A( K1, MNK2 ), LDA, C( MNK2, L2 ),
     $                        1 )
                  P22 = DDOT( M-K2, A( K2, MNK2 ), LDA, C( MNK2, L2 ),
     $                        1 )
C
                  DWORK( K2 ) = DDOT( N-L2, C( K2, MNL2 ), LDC,
     $                                B( L1, MNL2 ), LDB )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUMR + P11*B( L1, L1 ) +
     $                                                 P12*B( L1, L2 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1 ),
     $                         1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUMR + P21*B( L1, L1 ) +
     $                                                 P22*B( L1, L2 ) )
C
                  DWORK( K1+M ) = DDOT( N-L2, C( K1, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  DWORK( K2+M ) = DDOT( N-L2, C( K2, MNL2 ), LDC,
     $                                  B( L2, MNL2 ), LDB )
                  SUMR = DDOT( M-K1+1, A( K1, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUMR + P11*B( L2, L1 ) +
     $                                                 P12*B( L2, L2 ) )
C
                  SUMR = DDOT( M-K1+1, A( K2, K1 ), LDA, DWORK( K1+M ),
     $                         1 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUMR + P21*B( L2, L1 ) +
     $                                                 P22*B( L2, L2 ) )
C
                  CALL SB04PX( .FALSE., .TRUE., ISGN, 2, 2, A( K1, K1 ),
     $                         LDA, B( L1, L1 ), LDB, VEC, 2, SCALOC, X,
     $                         2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
C
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 220 J = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, J ), 1 )
  220                CONTINUE
C
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1 ), 1 )
                     CALL DSCAL( M-K1+1, SCALOC, DWORK( K1+M ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( K2, L2 ) = X( 2, 2 )
               END IF
C
  230       CONTINUE
C
  240    CONTINUE
C
      END IF
C
      RETURN
C *** Last line of SB04PY ***
      END
