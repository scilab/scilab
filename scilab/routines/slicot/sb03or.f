      SUBROUTINE SB03OR( DISCR, LTRANS, N, M, S, LDS, A, LDA, C, LDC,
     $                   SCALE, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the solution of the Sylvester equations
C
C        op(S)'*X + X*op(A) = scale*C, if DISCR = .FALSE.  or
C
C        op(S)'*X*op(A) - X = scale*C, if DISCR = .TRUE.
C
C     where op(K) = K or K' (i.e., the transpose of the matrix K), S is
C     an N-by-N block upper triangular matrix with one-by-one and
C     two-by-two blocks on the diagonal, A is an M-by-M matrix (M = 1 or
C     M = 2), X and C are each N-by-M matrices, and scale is an output
C     scale factor, set less than or equal to 1 to avoid overflow in X.
C     The solution X is overwritten on C.
C
C     SB03OR  is a service routine for the Lyapunov solver  SB03OT.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     DISCR   LOGICAL
C             Specifies the equation to be solved:
C             = .FALSE.:  op(S)'*X + X*op(A) = scale*C;
C             = .TRUE. :  op(S)'*X*op(A) - X = scale*C.
C
C     LTRANS  LOGICAL
C             Specifies the form of op(K) to be used, as follows:
C             = .FALSE.:  op(K) = K    (No transpose);
C             = .TRUE. :  op(K) = K**T (Transpose).
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix  S  and also the number of rows of
C             matrices  X and C.  N >= 0.
C
C     M       (input) INTEGER
C             The order of the matrix  A  and also the number of columns
C             of matrices  X and C.  M = 1 or M = 2.
C
C     S       (input) DOUBLE PRECISION array, dimension (LDS,N)
C             The leading  N-by-N  upper Hessenberg part of the array  S
C             must contain the block upper triangular matrix. The
C             elements below the upper Hessenberg part of the array  S
C             are not referenced.  The array  S  must not contain
C             diagonal blocks larger than two-by-two and the two-by-two
C             blocks must only correspond to complex conjugate pairs of
C             eigenvalues, not to real eigenvalues.
C
C     LDS     INTEGER
C             The leading dimension of array S.  LDS >= MAX(1,N).
C
C     A       (input) DOUBLE PRECISION array, dimension (LDS,M)
C             The leading  M-by-M  part of this array must contain a
C             given matrix, where M = 1 or M = 2.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= M.
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,M)
C             On entry, C must contain an N-by-M matrix, where M = 1 or
C             M = 2.
C             On exit, C contains the N-by-M matrix X, the solution of
C             the Sylvester equation.
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
C             = 1:  if DISCR = .FALSE., and S and -A have common
C                   eigenvalues, or if DISCR = .TRUE., and S and A have
C                   eigenvalues whose product is equal to unity;
C                   a solution has been computed using slightly
C                   perturbed values.
C
C     METHOD
C
C     The LAPACK scheme for solving Sylvester equations is adapted.
C
C     REFERENCES
C
C     [1] Hammarling, S.J.
C         Numerical solution of the stable, non-negative definite
C         Lyapunov equation.
C         IMA J. Num. Anal., 2, pp. 303-325, 1982.
C
C     NUMERICAL ASPECTS
C                               2
C     The algorithm requires 0(N M) operations and is backward stable.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, May 1997.
C     Supersedes Release 2.0 routines SB03CW and SB03CX by
C     Sven Hammarling, NAG Ltd, United Kingdom, Oct. 1986.
C     Partly based on routine PLYAP4 by A. Varga, University of Bochum,
C     May 1992.
C
C     REVISIONS
C
C     December 1997, April 1998, May 1999, April 2000.
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
C     .. Scalar Arguments ..
      LOGICAL            DISCR, LTRANS
      INTEGER            INFO, LDA, LDS, LDC, M, N
      DOUBLE PRECISION   SCALE
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), S( LDS, * )
C     .. Local Scalars ..
      LOGICAL            TBYT
      INTEGER            DL, INFOM, ISGN, J, L, L1, L2, L2P1, LNEXT
      DOUBLE PRECISION   G11, G12, G21, G22, SCALOC, XNORM
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION   AT( 2, 2 ), VEC( 2, 2 ), X( 2, 2 )
C     ..
C     .. External Functions ..
      DOUBLE PRECISION   DDOT
      EXTERNAL           DDOT
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLASY2, DSCAL, SB04PX, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          MAX, MIN
C     ..
C     .. Executable Statements ..
C
      INFO = 0
C
C     Test the input scalar arguments.
C
      IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( .NOT.( M.EQ.1 .OR. M.EQ.2 ) ) THEN
         INFO = -4
      ELSE IF( LDS.LT.MAX( 1, N ) ) THEN
         INFO = -6
      ELSE IF( LDA.LT.M ) THEN
         INFO = -8
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -10
      END IF
C
      IF ( INFO.NE.0 ) THEN      
C
C        Error return.
C
         CALL XERBLA( 'SB03OR', -INFO )
         RETURN
      END IF
C
      SCALE = ONE
C
C     Quick return if possible.
C
      IF ( N.EQ.0 )
     $   RETURN
C
      ISGN = 1
      TBYT = M.EQ.2
      INFOM = 0
C
C     Construct A'.
C
      AT(1,1) = A(1,1)
      IF ( TBYT ) THEN
         AT(1,2) = A(2,1)
         AT(2,1) = A(1,2)
         AT(2,2) = A(2,2)
      END IF
C
      IF ( LTRANS ) THEN
C
C        Start row loop (index = L).
C        L1 (L2) : row index of the first (last) row of X(L).
C
         LNEXT = N
C
         DO 20 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 20
            L1 = L
            L2 = L
            IF( L.GT.1 ) THEN
               IF( S( L, L-1 ).NE.ZERO )
     $            L1 = L1 - 1
               LNEXT = L1 - 1
            END IF
            DL = L2 - L1 + 1
            L2P1 = MIN( L2+1, N )
C
            IF ( DISCR ) THEN
C
C              Solve  S*X*A' - X = scale*C.
C
C              The L-th block of X is determined from
C
C              S(L,L)*X(L)*A' - X(L) = C(L) - R(L),
C
C              where
C
C                      N
C              R(L) = SUM [S(L,J)*X(J)] * A' .
C                    J=L+1
C
               G11 = -DDOT( N-L2, S( L1, L2P1 ), LDS, C( L2P1, 1 ), 1 )
               IF ( TBYT ) THEN
                  G12 = -DDOT( N-L2, S( L1, L2P1 ), LDS, C( L2P1, 2 ),
     $                         1 )
                  VEC( 1, 1 ) = C( L1, 1 ) + G11*AT(1,1) + G12*AT(2,1)
                  VEC( 1, 2 ) = C( L1, 2 ) + G11*AT(1,2) + G12*AT(2,2)
               ELSE
                  VEC (1, 1 ) = C( L1, 1 ) + G11*AT(1,1)
               END IF
               IF ( DL.NE.1 ) THEN
                  G21 = -DDOT( N-L2, S( L2, L2P1 ), LDS, C( L2P1, 1 ),
     $                         1 )
                  IF ( TBYT ) THEN
                     G22 = -DDOT( N-L2, S( L2, L2P1 ), LDS,
     $                            C( L2P1, 2 ), 1 )
                     VEC( 2, 1 ) = C( L2, 1 ) + G21*AT(1,1) +
     $                                          G22*AT(2,1)
                     VEC( 2, 2 ) = C( L2, 2 ) + G21*AT(1,2) +
     $                                          G22*AT(2,2)
                  ELSE
                     VEC( 2, 1 ) = C( L2, 1 ) + G21*AT(1,1)
                  END IF
               END IF
               CALL SB04PX( .FALSE., .FALSE., -ISGN, DL, M, S( L1, L1 ),
     $                      LDS, AT, 2, VEC, 2, SCALOC, X, 2, XNORM,
     $                      INFO )
            ELSE
C
C              Solve  S*X + X*A' = scale*C.
C
C              The L-th block of X is determined from
C
C              S(L,L)*X(L) + X(L)*A' = C(L) - R(L),
C
C              where
C                       N
C              R(L) =  SUM S(L,J)*X(J) .
C                     J=L+1
C
               VEC( 1, 1 ) = C( L1, 1 ) -
     $                       DDOT( N-L2, S( L1, L2P1 ), LDS,
     $                             C( L2P1, 1 ), 1 )
               IF ( TBYT )
     $            VEC( 1, 2 ) = C( L1, 2 ) -
     $                          DDOT( N-L2, S( L1, L2P1 ), LDS,
     $                                C( L2P1, 2 ), 1 )
C
               IF ( DL.NE.1 ) THEN
                  VEC( 2, 1 ) = C( L2, 1 ) -
     $                          DDOT( N-L2, S( L2, L2P1 ), LDS,
     $                                C( L2P1, 1 ), 1 )
                  IF ( TBYT )
     $               VEC( 2, 2 ) = C( L2, 2 ) -
     $                             DDOT( N-L2, S( L2, L2P1 ), LDS,
     $                                   C( L2P1, 2 ), 1 )
               END IF
               CALL DLASY2( .FALSE., .FALSE., ISGN, DL, M, S( L1, L1 ),
     $                      LDS, AT, 2, VEC, 2, SCALOC, X, 2, XNORM,
     $                      INFO )
            END IF
            INFOM = MAX( INFO, INFOM )
            IF ( SCALOC.NE.ONE ) THEN
C
               DO 10 J = 1, M
                  CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   10          CONTINUE
C
               SCALE = SCALE*SCALOC
            END IF
            C( L1, 1 ) = X( 1, 1 )
            IF ( TBYT ) C( L1, 2 ) = X( 1, 2 )
            IF ( DL.NE.1 ) THEN
               C( L2, 1 ) = X( 2, 1 )
               IF ( TBYT ) C( L2, 2 ) = X( 2, 2 )
            END IF
   20    CONTINUE
C
      ELSE
C
C        Start row loop (index = L).
C        L1 (L2) : row index of the first (last) row of X(L).
C
         LNEXT = 1
C
         DO 40 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 40
            L1 = L
            L2 = L
            IF( L.LT.N ) THEN
               IF( S( L+1, L ).NE.ZERO )
     $            L2 = L2 + 1
               LNEXT = L2 + 1
            END IF
            DL = L2 - L1 + 1
C
            IF ( DISCR ) THEN
C
C              Solve  A'*X'*S - X' = scale*C'.
C
C              The L-th block of X is determined from
C
C              A'*X(L)'*S(L,L) - X(L)' = C(L)' - R(L),
C
C              where
C
C                          L-1
C              R(L) = A' * SUM [X(J)'*S(J,L)] .
C                          J=1
C
               G11 = -DDOT( L1-1, C, 1, S( 1, L1 ), 1 )
               IF ( TBYT ) THEN
                  G21 = -DDOT( L1-1, C( 1, 2 ), 1, S( 1, L1 ), 1 )
                  VEC( 1, 1 ) = C( L1, 1 ) + AT(1,1)*G11 + AT(1,2)*G21
                  VEC( 2, 1 ) = C( L1, 2 ) + AT(2,1)*G11 + AT(2,2)*G21
               ELSE
                  VEC (1, 1 ) = C( L1, 1 ) + AT(1,1)*G11
               END IF
               IF ( DL .NE. 1 ) THEN
                  G12 = -DDOT( L1-1, C, 1, S( 1, L2 ), 1 )
                  IF ( TBYT ) THEN
                     G22 = -DDOT( L1-1, C( 1, 2 ), 1, S( 1, L2 ), 1 )
                     VEC( 1, 2 ) = C( L2, 1 ) + AT(1,1)*G12 +
     $                                          AT(1,2)*G22
                     VEC( 2, 2 ) = C( L2, 2 ) + AT(2,1)*G12 +
     $                                          AT(2,2)*G22
                  ELSE
                     VEC( 1, 2 ) = C( L2, 1 ) + AT(1,1)*G12
                  END IF
               END IF
               CALL SB04PX( .FALSE., .FALSE., -ISGN, M, DL, AT, 2,
     $                      S( L1, L1 ), LDS, VEC, 2, SCALOC, X, 2,
     $                      XNORM, INFO )
            ELSE
C
C              Solve  A'*X' + X'*S = scale*C'.
C
C              The L-th block of X is determined from
C
C              A'*X(L)' + X(L)'*S(L,L) = C(L)' - R(L),
C
C              where
C                     L-1
C              R(L) = SUM [X(J)'*S(J,L)].
C                     J=1
C
               VEC( 1, 1 ) = C( L1, 1 ) -
     $                       DDOT( L1-1, C, 1, S( 1, L1 ), 1 )
               IF ( TBYT )
     $            VEC( 2, 1 ) = C( L1, 2 ) -
     $                          DDOT( L1-1, C( 1, 2 ), 1, S( 1, L1 ), 1)
C
               IF ( DL.NE.1 ) THEN
                  VEC( 1, 2 ) = C( L2, 1 ) -
     $                          DDOT( L1-1, C, 1, S( 1, L2 ), 1 )
                  IF ( TBYT )
     $            VEC( 2, 2 ) = C( L2, 2 ) -
     $                          DDOT( L1-1, C( 1, 2 ), 1, S( 1, L2 ), 1)
               END IF
               CALL DLASY2( .FALSE., .FALSE., ISGN, M, DL, AT, 2,
     $                      S( L1, L1 ), LDS, VEC, 2, SCALOC, X, 2,
     $                      XNORM, INFO )
            END IF
            INFOM = MAX( INFO, INFOM )
            IF ( SCALOC.NE.ONE ) THEN
C
               DO 30 J = 1, M
                  CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   30          CONTINUE
C
               SCALE = SCALE*SCALOC
            END IF
            C( L1, 1 ) = X( 1, 1 )
            IF ( TBYT ) C( L1, 2 ) = X( 2, 1 )
            IF ( DL.NE.1 ) THEN
               C( L2, 1 ) = X( 1, 2 )
               IF ( TBYT ) C( L2, 2 ) = X( 2, 2 )
            END IF
   40    CONTINUE
      END IF
C
      INFO = INFOM
      RETURN
C *** Last line of SB03OR ***
      END
