      SUBROUTINE SB03OT( DISCR, LTRANS, N, S, LDS, R, LDR, SCALE, DWORK,
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for X = op(U)'*op(U) either the stable non-negative
C     definite continuous-time Lyapunov equation
C                                   2
C        op(S)'*X + X*op(S) = -scale *op(R)'*op(R)                   (1)
C
C     or the convergent non-negative definite discrete-time Lyapunov
C     equation
C                                   2
C        op(S)'*X*op(S) - X = -scale *op(R)'*op(R)                   (2)
C
C     where op(K) = K or K' (i.e., the transpose of the matrix K), S is
C     an N-by-N block upper triangular matrix with one-by-one or
C     two-by-two blocks on the diagonal, R is an N-by-N upper triangular
C     matrix, and scale is an output scale factor, set less than or
C     equal to 1 to avoid overflow in X.
C
C     In the case of equation (1) the matrix S must be stable (that
C     is, all the eigenvalues of S must have negative real parts),
C     and for equation (2) the matrix S must be convergent (that is,
C     all the eigenvalues of S must lie inside the unit circle).
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
C             The order of the matrices S and R.  N >= 0.
C
C     S       (input) DOUBLE PRECISION array of dimension (LDS,N)
C             The leading N-by-N upper Hessenberg part of this array
C             must contain the block upper triangular matrix.
C             The elements below the upper Hessenberg part of the array
C             S are not referenced. The 2-by-2 blocks must only
C             correspond to complex conjugate pairs of eigenvalues (not
C             to real eigenvalues).
C
C     LDS     INTEGER
C             The leading dimension of array S.  LDS >= MAX(1,N).
C
C     R       (input/output) DOUBLE PRECISION array of dimension (LDR,N)
C             On entry, the leading N-by-N upper triangular part of this
C             array must contain the upper triangular matrix R.
C             On exit, the leading N-by-N upper triangular part of this
C             array contains the upper triangular matrix U.
C             The strict lower triangle of R is not referenced.
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,N).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (4*N)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             = 1:  if the Lyapunov equation is (nearly) singular
C                   (warning indicator);
C                   if DISCR = .FALSE., this means that while the
C                   matrix S has computed eigenvalues with negative real
C                   parts, it is only just stable in the sense that
C                   small perturbations in S can make one or more of the
C                   eigenvalues have a non-negative real part;
C                   if DISCR = .TRUE., this means that while the
C                   matrix S has computed eigenvalues inside the unit
C                   circle, it is nevertheless only just convergent, in
C                   the sense that small perturbations in S can make one
C                   or more of the eigenvalues lie outside the unit
C                   circle;
C                   perturbed values were used to solve the equation 
C                   (but the matrix S is unchanged);
C             = 2:  if the matrix S is not stable (that is, one or more
C                   of the eigenvalues of S has a non-negative real
C                   part), if DISCR = .FALSE., or not convergent (that
C                   is, one or more of the eigenvalues of S lies outside
C                   the unit circle), if DISCR = .TRUE.;
C             = 3:  if the matrix S has two or more consecutive non-zero
C                   elements on the first sub-diagonal, so that there is
C                   a block larger than 2-by-2 on the diagonal;
C             = 4:  if the matrix S has a 2-by-2 diagonal block with
C                   real eigenvalues instead of a complex conjugate
C                   pair.
C
C     METHOD
C
C     The method used by the routine is based on a variant of the
C     Bartels and Stewart backward substitution method [1], that finds
C     the Cholesky factor op(U) directly without first finding X and
C     without the need to form the normal matrix op(R)'*op(R) [2].
C
C     The continuous-time Lyapunov equation in the canonical form
C                                                        2
C       op(S)'*op(U)'*op(U) + op(U)'*op(U)*op(S) = -scale *op(R)'*op(R),
C
C     or the discrete-time Lyapunov equation in the canonical form
C                                                        2
C       op(S)'*op(U)'*op(U)*op(S) - op(U)'*op(U) = -scale *op(R)'*op(R),
C
C     where U and R are upper triangular, is solved for U.
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
C     The Lyapunov equation may be very ill-conditioned. In particular
C     if S is only just stable (or convergent) then the Lyapunov
C     equation will be ill-conditioned. "Large" elements in U relative
C     to those of S and R, or a "small" value for scale, is a symptom
C     of ill-conditioning. A condition estimate can be computed using
C     SLICOT Library routine SB03MD.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, May 1997.
C     Supersedes Release 2.0 routine SB03CZ by Sven Hammarling,
C     NAG Ltd, United Kingdom, Oct. 1986.
C     Partly based on SB03CZ and PLYAP1 by A. Varga, University of
C     Bochum, May 1992.
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
      DOUBLE PRECISION  ZERO, ONE, TWO
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0, TWO = 2.0D0 )
C     .. Scalar Arguments ..
      LOGICAL           DISCR, LTRANS
      INTEGER           INFO, LDR, LDS, N
      DOUBLE PRECISION  SCALE
C     .. Array Arguments ..
      DOUBLE PRECISION  DWORK(*), R(LDR,*), S(LDS,*)
C     .. Local Scalars ..
      LOGICAL           CONT, TBYT
      INTEGER           INFOM, ISGN, J, J1, J2, J3, K, K1, K2, K3, 
     $                  KOUNT, KSIZE
      DOUBLE PRECISION  ABSSKK, ALPHA, BIGNUM, D1, D2, DR, EPS, SCALOC,
     $                  SMIN, SMLNUM, SUM, T1, T2, T3, T4, TAU1, TAU2,
     $                  TEMP, V1, V2, V3, V4
C     .. Local Arrays ..
      DOUBLE PRECISION  A(2,2), B(2,2), U(2,2)
C     .. External Functions ..
      DOUBLE PRECISION  DLAMCH, DLANHS
      EXTERNAL          DLAMCH, DLANHS
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, DLABAD, DLARFG, DSCAL, DSWAP,
     $                  DTRMM, DTRMV, MB04ND, MB04OD, SB03OR, SB03OY,
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, MAX, SIGN, SQRT
C     .. Executable Statements ..
C
      INFO = 0
C
C     Test the input scalar arguments.
C
      IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDS.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( LDR.LT.MAX( 1, N ) ) THEN
         INFO = -7
      END IF
C
      IF ( INFO.NE.0 ) THEN      
C
C        Error return.
C
         CALL XERBLA( 'SB03OT', -INFO )
         RETURN
      END IF
C
      SCALE = ONE
C
C     Quick return if possible.
C
      IF (N.EQ.0)
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
      SMIN = MAX( SMLNUM, EPS*DLANHS( 'Max', N, S, LDS, DWORK ) )
      INFOM = 0
C
C     Start the solution. Most of the comments refer to notation and
C     equations in sections 5 and 10 of the second reference above.
C
C     Determine whether or not the current block is two-by-two.
C     K gives the position of the start of the current block and
C     TBYT is true if the block is two-by-two.
C
      CONT = .NOT.DISCR
      ISGN = 1
      IF ( .NOT.LTRANS ) THEN
C
C        Case op(M) = M.
C
         KOUNT = 1
C
   10    CONTINUE
C        WHILE( KOUNT.LE.N )LOOP
         IF ( KOUNT.LE.N ) THEN
            K = KOUNT
            IF ( KOUNT.GE.N ) THEN
               TBYT  = .FALSE.
               KOUNT = KOUNT + 1
            ELSE IF ( S(K+1,K).EQ.ZERO ) THEN
               TBYT  = .FALSE.
               KOUNT = KOUNT + 1
            ELSE
               TBYT = .TRUE.
               IF ( (K+1).LT.N ) THEN
                  IF ( S(K+2,K+1).NE.ZERO ) THEN
                     INFO = 3
                     RETURN
                  END IF
               END IF
               KOUNT = KOUNT + 2
            END IF
            IF ( TBYT ) THEN
C
C              Solve the two-by-two Lyapunov equation (6.1) or (10.19),
C              using the routine SB03OY.
C
               B(1,1) = S(K,K)
               B(2,1) = S(K+1,K)
               B(1,2) = S(K,K+1)
               B(2,2) = S(K+1,K+1)
               U(1,1) = R(K,K)
               U(1,2) = R(K,K+1)
               U(2,2) = R(K+1,K+1)
C
               CALL SB03OY( DISCR, LTRANS, ISGN, B, 2, U, 2, A, 2,
     $                      SCALOC, INFO )
               IF ( INFO.GT.1 )
     $            RETURN
               INFOM = MAX( INFO, INFOM )
               IF( SCALOC.NE.ONE ) THEN
C
                  DO 20 J = 1, N
                     CALL DSCAL( J, SCALOC, R(1,J), 1 )
   20             CONTINUE
C
                  SCALE = SCALE*SCALOC
               END IF
               R(K,K)     = U(1,1)
               R(K,K+1)   = U(1,2)
               R(K+1,K+1) = U(2,2)
C
C              If we are not at the end of S then set up and solve
C              equation (6.2) or (10.20).
C
C              Note that  SB03OY  returns  ( u11*s11*inv( u11 ) ) in  B
C              and returns scaled alpha in  A.  ksize is the order of 
C              the remainder of  S.  k1, k2 and k3  point to the start
C              of vectors in  DWORK.
C
               IF ( KOUNT.LE.N ) THEN
                  KSIZE = N - K - 1
                  K1 = KSIZE + 1
                  K2 = KSIZE + K1
                  K3 = KSIZE + K2
C
C                 Form the right-hand side of (6.2) or (10.20), the
C                 first column in DWORK( 1 ) ,..., DWORK( n - k - 1 )
C                 the second in DWORK( n - k ) ,...,
C                 DWORK( 2*( n - k - 1 ) ).
C
                  CALL DCOPY( KSIZE, R(K,K+2), LDR, DWORK, 1 )
                  CALL DCOPY( KSIZE, R(K+1,K+2), LDR, DWORK(K1), 1 )
                  CALL DTRMM( 'Right', 'Upper', 'No transpose',
     $                        'Non-unit', KSIZE, 2, -ONE, A, 2, DWORK,
     $                        KSIZE )
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -R(K,K), S(K,K+2), LDS, DWORK,
     $                           1 )
                     CALL DAXPY( KSIZE, -R(K,K+1), S(K+1,K+2), LDS,
     $                           DWORK, 1)
                     CALL DAXPY( KSIZE, -R(K+1,K+1), S(K+1,K+2), LDS,
     $                           DWORK(K1), 1 )
                  ELSE
                     CALL DAXPY( KSIZE, -R(K,K)*B(1,1), S(K,K+2), LDS,
     $                           DWORK, 1 )
                     CALL DAXPY( KSIZE, -( R(K,K+1)*B(1,1) + R(K+1,K+1)
     $                           *B(2,1) ), S(K+1,K+2), LDS, DWORK, 1 )
                     CALL DAXPY( KSIZE, -R(K,K)*B(1,2), S(K,K+2), LDS,
     $                           DWORK(K1), 1 )
                     CALL DAXPY( KSIZE, -( R(K,K+1)*B(1,2) + R(K+1,K+1)
     $                           *B(2,2) ), S(K+1,K+2), LDS, DWORK(K1),
     $                           1 )
                  END IF
C
C                 SB03OR  solves the Sylvester equations. The solution
C                 is overwritten on DWORK.
C
                  CALL SB03OR( DISCR, LTRANS, KSIZE, 2, S(K+2,K+2), LDS,
     $                         B, 2, DWORK, KSIZE, SCALOC, INFO )
                  INFOM = MAX( INFO, INFOM )
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 30 J = 1, N
                        CALL DSCAL( J, SCALOC, R(1,J), 1 )
   30                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
C
C                 Copy the solution into the next  2*( n - k - 1 )
C                 elements of  DWORK.
C
                  CALL DCOPY( 2*KSIZE, DWORK, 1, DWORK(K2), 1 )
C
C                 Now form the matrix  Rhat  of equation (6.4) or
C                 (10.22). Note that (10.22) is incorrect, so here we
C                 implement a corrected version of (10.22).
C
                  IF ( CONT ) THEN
C
C                    Swap the two rows of R with DWORK.
C
                     CALL DSWAP( KSIZE, DWORK, 1, R(K,K+2), LDR )
                     CALL DSWAP( KSIZE, DWORK(K1), 1, R(K+1,K+2), LDR )
C
C                    1st column:
C
                     CALL DAXPY( KSIZE, -A(1,1), DWORK(K2), 1, DWORK,
     $                           1 )
                     CALL DAXPY( KSIZE, -A(1,2), DWORK(K3), 1, DWORK,
     $                           1 )
C
C                    2nd column:
C
                     CALL DAXPY( KSIZE, -A(2,2), DWORK(K3), 1,
     $                           DWORK(K1), 1 )
                  ELSE
C
C                    Form  v = S1'*u + s*u11', overwriting  v  on DWORK.
C
C                    Compute  S1'*u,  first multiplying by the
C                    triangular part of  S1.
C
                     CALL DTRMM( 'Left', 'Upper', 'Transpose',
     $                           'Non-unit', KSIZE, 2, ONE, S(K+2,K+2),
     $                           LDS, DWORK, KSIZE )
C
C                    Then multiply by the subdiagonal of  S1  and add in
C                    to the above result.
C
                     J1 = K1
                     J2 = K + 2
C
                     DO 40 J = 1, KSIZE-1
                        IF ( S(J2+1,J2).NE.ZERO ) THEN
                           DWORK(J)  = S(J2+1,J2)*DWORK(K2+J) + DWORK(J)
                           DWORK(J1) = S(J2+1,J2)*DWORK(K3+J) +
     $                                 DWORK(J1)
                        END IF
                        J1 = J1 + 1
                        J2 = J2 + 1
   40                CONTINUE
C
C                    Add in s*u11'.
C
                     CALL DAXPY( KSIZE, R(K,K), S(K,K+2), LDS, DWORK,
     $                           1 )
                     CALL DAXPY( KSIZE, R(K,K+1), S(K+1,K+2), LDS,
     $                           DWORK, 1 )
                     CALL DAXPY( KSIZE, R(K+1,K+1), S(K+1,K+2), LDS,
     $                           DWORK(K1), 1 )
C
C                    Next recover r from R, swapping r with u.
C
                     CALL DSWAP( KSIZE, DWORK(K2), 1, R(K,K+2), LDR )
                     CALL DSWAP( KSIZE, DWORK(K3), 1, R(K+1,K+2), LDR )
C
C                    Now we perform the QR factorization.
C
C                    ( a ) = Q*( t ),
C                    ( b )
C
C                    and form
C
C                    ( p' ) = Q'*( r' ).
C                    ( y' )      ( v' )
C
C                    y  is then the correct vector to use in (10.22).
C                    Note that  a  is upper triangular and that  t  and
C                    p  are not required.
C
                     CALL DLARFG( 3, A(1,1), B(1,1), 1, TAU1 )
                     V1  = B(1,1)
                     T1  = TAU1*V1
                     V2  = B(2,1)
                     T2  = TAU1*V2
                     SUM = A(1,2) + V1*B(1,2) + V2*B(2,2)
                     B(1,2) = B(1,2) - SUM*T1
                     B(2,2) = B(2,2) - SUM*T2
                     CALL DLARFG( 3, A(2,2), B(1,2), 1, TAU2 )
                     V3 = B(1,2)
                     T3 = TAU2*V3
                     V4 = B(2,2)
                     T4 = TAU2*V4
                     J1 = K1
                     J2 = K2
                     J3 = K3
C
                     DO 50 J = 1, KSIZE
                        SUM = DWORK(J2) + V1*DWORK(J) + V2*DWORK(J1)
                        D1  = DWORK(J)  - SUM*T1
                        D2  = DWORK(J1) - SUM*T2
                        SUM = DWORK(J3) + V3*D1 + V4*D2
                        DWORK(J)  =  D1 - SUM*T3
                        DWORK(J1) =  D2 - SUM*T4
                        J1 = J1 + 1
                        J2 = J2 + 1
                        J3 = J3 + 1
   50                CONTINUE
C
                  END IF
C
C                 Now update  R1  to give  Rhat.
C
                  CALL DCOPY( KSIZE, DWORK, 1, DWORK(K2), 1 )
                  CALL DCOPY( KSIZE, DWORK(K1), 1, DWORK(2), 2 )
                  CALL DCOPY( KSIZE, DWORK(K2), 1, DWORK(1), 2 )
                  CALL MB04OD( 'Full', KSIZE, 0, 2, R(K+2,K+2), LDR,
     $                         DWORK, 2, DWORK, 1, DWORK, 1, DWORK(K2),
     $                         DWORK(K3) )
               END IF
            ELSE
C
C              1-by-1 block.
C
C              Make sure S is stable or convergent and find u11 in
C              equation (5.13) or (10.15).
C
               IF ( DISCR ) THEN
                  ABSSKK = ABS( S(K,K) )
                  IF ( ( ABSSKK - ONE ).GE.ZERO ) THEN
                     INFO = 2
                     RETURN
                  END IF
                  TEMP = SQRT( ( ONE - ABSSKK )*( ONE + ABSSKK ) )
               ELSE
                  IF ( S(K,K).GE.ZERO ) THEN
                     INFO = 2
                     RETURN
                  END IF
                  TEMP = SQRT( ABS( TWO*S(K,K) ) )
               END IF
C
               SCALOC = ONE
               IF( TEMP.LT.SMIN ) THEN
                  TEMP  = SMIN
                  INFOM = 1
               END IF
               DR = ABS( R(K,K) )
               IF( TEMP.LT.ONE .AND. DR.GT.ONE ) THEN
                  IF( DR.GT.BIGNUM*TEMP )
     $               SCALOC = ONE / DR
               END IF
               ALPHA = SIGN( TEMP, R(K,K) )
               R(K,K) = R(K,K)/ALPHA
               IF( SCALOC.NE.ONE ) THEN
C
                  DO 60 J = 1, N
                     CALL DSCAL( J, SCALOC, R(1,J), 1 )
   60             CONTINUE
C
                  SCALE = SCALE*SCALOC
               END IF
C
C              If we are not at the end of  S  then set up and solve
C              equation (5.14) or (10.16).  ksize is the order of the
C              remainder of  S.  k1 and k2 point to the start of vectors
C              in  DWORK.
C
               IF ( KOUNT.LE.N ) THEN
                  KSIZE = N - K
                  K1 = KSIZE + 1
                  K2 = KSIZE + K1
C
C                 Form the right-hand side in DWORK( 1 ),...,
C                 DWORK( n - k ).
C
                  CALL DCOPY( KSIZE, R(K,K+1), LDR, DWORK, 1 )
                  CALL DSCAL( KSIZE, -ALPHA, DWORK, 1 )
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -R(K,K), S(K,K+1), LDS, DWORK,
     $                          1 )
                  ELSE
                     CALL DAXPY( KSIZE, -S(K,K)*R(K,K), S(K,K+1), LDS,
     $                          DWORK, 1 )
                  END IF
C
C                 SB03OR solves the Sylvester equations. The solution is
C                 overwritten on  DWORK.
C
                  CALL SB03OR( DISCR, LTRANS, KSIZE, 1, S(K+1,K+1), LDS,
     $                         S(K,K), 1, DWORK, KSIZE, SCALOC, INFO )
                  INFOM = MAX( INFO, INFOM )
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 70 J = 1, N
                        CALL DSCAL( J, SCALOC, R(1,J), 1 )
   70                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
C
C                 Copy the solution into the next  ( n - k ) elements
C                 of  DWORK,  copy the solution back into  R  and copy
C                 the row of  R  back into  DWORK.
C
                  CALL DCOPY( KSIZE, DWORK, 1, DWORK(K1), 1 )
                  CALL DSWAP( KSIZE, DWORK, 1, R(K,K+1), LDR )
C
C                 Now form the matrix  Rhat  of equation (5.15) or
C                 (10.17), first computing  y  in  DWORK,  and then
C                 updating  R1.
C
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -ALPHA, DWORK(K1), 1, DWORK, 1 )
                  ELSE
C
C                    First form  lambda( 1 )*r  and then add in
C                    alpha*u11*s.
C
                     CALL DSCAL( KSIZE, -S(K,K), DWORK, 1 )
                     CALL DAXPY( KSIZE, ALPHA*R(K,K), S(K,K+1), LDS,
     $                           DWORK, 1 )
C
C                    Now form  alpha*S1'*u,  first multiplying by the
C                    sub-diagonal of  S1  and then the triangular part
C                    of  S1,  and add the result in DWORK.
C
                     J1 = K + 1
C
                     DO 80 J = 1, KSIZE-1
                        IF ( S(J1+1,J1).NE.ZERO ) DWORK(J)
     $                         = ALPHA*S(J1+1,J1)*DWORK(K1+J) + DWORK(J)
                        J1 = J1 + 1
   80                CONTINUE
C
                     CALL DTRMV( 'Upper', 'Transpose', 'Non-unit',
     $                           KSIZE, S(K+1,K+1), LDS, DWORK(K1), 1 )
                     CALL DAXPY( KSIZE, ALPHA, DWORK(K1), 1, DWORK, 1 )
                  END IF
                  CALL MB04OD( 'Full', KSIZE, 0, 1, R(K+1,K+1), LDR,
     $                         DWORK, 1, DWORK, 1, DWORK, 1, DWORK(K2),
     $                         DWORK(K1) )
               END IF
            END IF
            GO TO 10
         END IF
C        END WHILE 10
C
      ELSE
C
C        Case op(M) = M'.
C
         KOUNT = N
C
   90    CONTINUE
C        WHILE( KOUNT.GE.1 )LOOP
         IF ( KOUNT.GE.1 ) THEN
            K = KOUNT
            IF ( KOUNT.EQ.1 ) THEN
               TBYT  = .FALSE.
               KOUNT = KOUNT - 1
            ELSE IF ( S(K,K-1).EQ.ZERO ) THEN
               TBYT  = .FALSE.
               KOUNT = KOUNT - 1
            ELSE
               TBYT = .TRUE.
               K = K - 1
               IF ( K.GT.1 ) THEN
                  IF ( S(K,K-1).NE.ZERO ) THEN
                     INFO = 3
                     RETURN
                  END IF
               END IF
               KOUNT = KOUNT - 2
            END IF
            IF ( TBYT ) THEN
C
C              Solve the two-by-two Lyapunov equation corresponding to
C              (6.1) or (10.19), using the routine SB03OY.
C
               B(1,1) = S(K,K)
               B(2,1) = S(K+1,K)
               B(1,2) = S(K,K+1)
               B(2,2) = S(K+1,K+1)
               U(1,1) = R(K,K)
               U(1,2) = R(K,K+1)
               U(2,2) = R(K+1,K+1)
C
               CALL SB03OY( DISCR, LTRANS, ISGN, B, 2, U, 2, A, 2,
     $                      SCALOC, INFO )
               IF ( INFO.GT.1 )
     $            RETURN
               INFOM = MAX( INFO, INFOM )
               IF( SCALOC.NE.ONE ) THEN
C
                  DO 100 J = 1, N
                     CALL DSCAL( J, SCALOC, R(1,J), 1 )
  100             CONTINUE
C
                  SCALE = SCALE*SCALOC
               END IF
               R(K,K)     = U(1,1)
               R(K,K+1)   = U(1,2)
               R(K+1,K+1) = U(2,2)
C
C              If we are not at the front of S then set up and solve
C              equation corresponding to (6.2) or (10.20).
C
C              Note that  SB03OY  returns  ( inv( u11 )*s11*u11 ) in  B
C              and returns scaled alpha, alpha = inv( u11 )*r11, in  A.
C              ksize is the order of the remainder leading part of  S.
C              k1, k2 and k3 point to the start of vectors in  DWORK.
C
               IF ( KOUNT.GE.1 ) THEN
                  KSIZE = K - 1
                  K1 = KSIZE + 1
                  K2 = KSIZE + K1
                  K3 = KSIZE + K2
C
C                 Form the right-hand side of equations corresponding to
C                 (6.2) or (10.20), the first column in DWORK( 1 ) ,...,
C                 DWORK( k - 1 ) the second in DWORK( k ) ,...,
C                 DWORK( 2*( k - 1 ) ).
C
                  CALL DCOPY( KSIZE, R(1,K), 1, DWORK, 1 )
                  CALL DCOPY( KSIZE, R(1,K+1), 1, DWORK(K1), 1 )
                  CALL DTRMM( 'Right', 'Upper', 'Transpose', 'Non-unit',
     $                        KSIZE, 2, -ONE, A, 2, DWORK, KSIZE )
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -R(K,K), S(1,K), 1, DWORK, 1 )
                     CALL DAXPY( KSIZE, -R(K,K+1), S(1,K), 1, DWORK(K1),
     $                           1)
                     CALL DAXPY( KSIZE, -R(K+1,K+1), S(1,K+1), 1,
     $                           DWORK(K1), 1 )
                  ELSE
                     CALL DAXPY( KSIZE, -( R(K,K)*B(1,1) + R(K,K+1)
     $                           *B(1,2) ), S(1,K), 1, DWORK, 1 )
                     CALL DAXPY( KSIZE, -R(K+1,K+1)*B(1,2), S(1,K+1), 1,
     $                           DWORK, 1 )
                     CALL DAXPY( KSIZE, -( R(K,K)*B(2,1) + R(K,K+1)
     $                           *B(2,2) ), S(1,K), 1, DWORK(K1), 1 )
                     CALL DAXPY( KSIZE, -R(K+1,K+1)*B(2,2), S(1,K+1), 1,
     $                           DWORK(K1), 1 )
                  END IF
C
C                 SB03OR  solves the Sylvester equations. The solution
C                 is overwritten on DWORK.
C
                  CALL SB03OR( DISCR, LTRANS, KSIZE, 2, S, LDS, B, 2,
     $                         DWORK, KSIZE, SCALOC, INFO )
                  INFOM = MAX( INFO, INFOM )
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 110 J = 1, N
                        CALL DSCAL( J, SCALOC, R(1,J), 1 )
  110                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
C
C                 Copy the solution into the next  2*( k - 1 ) elements
C                 of  DWORK.
C
                  CALL DCOPY( 2*KSIZE, DWORK, 1, DWORK(K2), 1 )
C
C                 Now form the matrix  Rhat  of equation corresponding
C                 to (6.4) or (10.22) (corrected version).
C
                  IF ( CONT ) THEN
C
C                    Swap the two columns of R with DWORK.
C
                     CALL DSWAP( KSIZE, DWORK, 1, R(1,K), 1 )
                     CALL DSWAP( KSIZE, DWORK(K1), 1, R(1,K+1), 1 )
C
C                    1st column:
C
                     CALL DAXPY( KSIZE, -A(1,1), DWORK(K2), 1, DWORK,
     $                           1 )
C
C                    2nd column:
C
                     CALL DAXPY( KSIZE, -A(1,2), DWORK(K2), 1,
     $                           DWORK(K1), 1 )
                     CALL DAXPY( KSIZE, -A(2,2), DWORK(K3), 1,
     $                           DWORK(K1), 1 )
                  ELSE
C
C                    Form  v = S1*u + s*u11, overwriting  v  on DWORK.
C
C                    Compute  S1*u,  first multiplying by the triangular
C                    part of  S1.
C
                     CALL DTRMM( 'Left', 'Upper', 'No transpose',
     $                           'Non-unit', KSIZE, 2, ONE, S, LDS,
     $                           DWORK, KSIZE )
C
C                    Then multiply by the subdiagonal of  S1  and add in
C                    to the above result.
C
                     J1 = K1
C
                     DO 120 J = 2, KSIZE
                        J1 = J1 + 1
                        IF ( S(J,J-1).NE.ZERO ) THEN
                           DWORK(J)  = S(J,J-1)*DWORK(K2+J-2) + DWORK(J)
                           DWORK(J1) = S(J,J-1)*DWORK(K3+J-2) +
     $                                 DWORK(J1)
                        END IF
  120                CONTINUE
C
C                    Add in s*u11.
C
                     CALL DAXPY( KSIZE, R(K,K), S(1,K), 1, DWORK, 1 )
                     CALL DAXPY( KSIZE, R(K,K+1), S(1,K), 1, DWORK(K1),
     $                           1 )
                     CALL DAXPY( KSIZE, R(K+1,K+1), S(1,K+1), 1,
     $                           DWORK(K1), 1 )
C
C                    Next recover r from R, swapping r with u.
C
                     CALL DSWAP( KSIZE, DWORK(K2), 1, R(1,K), 1 )
                     CALL DSWAP( KSIZE, DWORK(K3), 1, R(1,K+1), 1 )
C
C                    Now we perform the QL factorization.
C
C                    ( a' ) = Q*( t ),
C                    ( b' )
C
C                    and form
C
C                    ( p' ) = Q'*( r' ).
C                    ( y' )      ( v' )
C
C                    y  is then the correct vector to use in the
C                    relation corresponding to (10.22).
C                    Note that  a  is upper triangular and that  t  and
C                    p  are not required.
C
                     CALL DLARFG( 3, A(2,2), B(2,1), 2, TAU1 )
                     V1  = B(2,1)
                     T1  = TAU1*V1
                     V2  = B(2,2)
                     T2  = TAU1*V2
                     SUM = A(1,2) + V1*B(1,1) + V2*B(1,2)
                     B(1,1) = B(1,1) - SUM*T1
                     B(1,2) = B(1,2) - SUM*T2
                     CALL DLARFG( 3, A(1,1), B(1,1), 2, TAU2 )
                     V3 = B(1,1)
                     T3 = TAU2*V3
                     V4 = B(1,2)
                     T4 = TAU2*V4
                     J1 = K1
                     J2 = K2
                     J3 = K3
C
                     DO 130 J = 1, KSIZE
                        SUM = DWORK(J3) + V1*DWORK(J) + V2*DWORK(J1)
                        D1  = DWORK(J)  - SUM*T1
                        D2  = DWORK(J1) - SUM*T2
                        SUM = DWORK(J2) + V3*D1 + V4*D2
                        DWORK(J)  =  D1 - SUM*T3
                        DWORK(J1) =  D2 - SUM*T4
                        J1 = J1 + 1
                        J2 = J2 + 1
                        J3 = J3 + 1
  130                CONTINUE
C
                  END IF
C
C                 Now update  R1  to give  Rhat.
C
                  CALL MB04ND( 'Full', KSIZE, 0, 2, R, LDR, DWORK,
     $                         KSIZE, DWORK, 1, DWORK, 1, DWORK(K2),
     $                         DWORK(K3) )
               END IF
            ELSE
C
C              1-by-1 block.
C
C              Make sure S is stable or convergent and find u11 in
C              equation corresponding to (5.13) or (10.15).
C
               IF ( DISCR ) THEN
                  ABSSKK = ABS( S(K,K) )
                  IF ( ( ABSSKK - ONE ).GE.ZERO ) THEN
                     INFO = 2
                     RETURN
                  END IF
                  TEMP = SQRT( ( ONE - ABSSKK )*( ONE + ABSSKK ) )
               ELSE
                  IF ( S(K,K).GE.ZERO ) THEN
                     INFO = 2
                     RETURN
                  END IF
                  TEMP = SQRT( ABS( TWO*S(K,K) ) )
               END IF
C
               SCALOC = ONE
               IF( TEMP.LT.SMIN ) THEN
                  TEMP  = SMIN
                  INFOM = 1
               END IF
               DR = ABS( R(K,K) )
               IF( TEMP.LT.ONE .AND. DR.GT.ONE ) THEN
                  IF( DR.GT.BIGNUM*TEMP )
     $               SCALOC = ONE / DR
               END IF
               ALPHA = SIGN( TEMP, R(K,K) )
               R(K,K) = R(K,K)/ALPHA
               IF( SCALOC.NE.ONE ) THEN
C
                  DO 140 J = 1, N
                     CALL DSCAL( J, SCALOC, R(1,J), 1 )
  140             CONTINUE
C
                  SCALE = SCALE*SCALOC
               END IF
C
C              If we are not at the front of  S  then set up and solve
C              equation corresponding to (5.14) or (10.16).  ksize is
C              the order of the remainder leading part of  S.  k1 and k2
C              point to the start of vectors in  DWORK.
C
               IF ( KOUNT.GE.1 ) THEN
                  KSIZE = K - 1
                  K1 = KSIZE + 1
                  K2 = KSIZE + K1
C
C                 Form the right-hand side in DWORK( 1 ),...,
C                 DWORK( k - 1 ).
C
                  CALL DCOPY( KSIZE, R(1,K), 1, DWORK, 1 )
                  CALL DSCAL( KSIZE, -ALPHA, DWORK, 1 )
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -R(K,K), S(1,K), 1, DWORK, 1 )
                  ELSE
                     CALL DAXPY( KSIZE, -S(K,K)*R(K,K), S(1,K), 1,
     $                          DWORK, 1 )
                  END IF
C
C                 SB03OR solves the Sylvester equations. The solution is
C                 overwritten on  DWORK.
C
                  CALL SB03OR( DISCR, LTRANS, KSIZE, 1, S, LDS, S(K,K),
     $                         1, DWORK, KSIZE, SCALOC, INFO )
                  INFOM = MAX( INFO, INFOM )
                  IF( SCALOC.NE.ONE ) THEN
C
                     DO 150 J = 1, N
                        CALL DSCAL( J, SCALOC, R(1,J), 1 )
  150                CONTINUE
C
                     SCALE = SCALE*SCALOC
                  END IF
C
C                 Copy the solution into the next  ( k - 1 ) elements
C                 of  DWORK,  copy the solution back into  R  and copy
C                 the column of  R  back into  DWORK.
C
                  CALL DCOPY( KSIZE, DWORK, 1, DWORK(K1), 1 )
                  CALL DSWAP( KSIZE, DWORK, 1, R(1,K), 1 )
C
C                 Now form the matrix  Rhat  of equation corresponding
C                 to (5.15) or (10.17), first computing  y  in  DWORK,
C                 and then updating  R1.
C
                  IF ( CONT ) THEN
                     CALL DAXPY( KSIZE, -ALPHA, DWORK(K1), 1, DWORK, 1 )
                  ELSE
C
C                    First form  lambda( 1 )*r  and then add in
C                    alpha*u11*s.
C
                     CALL DSCAL( KSIZE, -S(K,K), DWORK, 1 )
                     CALL DAXPY( KSIZE, ALPHA*R(K,K), S(1,K), 1, DWORK,
     $                           1 )
C
C                    Now form  alpha*S1*u,  first multiplying by the
C                    sub-diagonal of  S1  and then the triangular part
C                    of  S1,  and add the result in DWORK.
C
                     DO 160 J = 2, KSIZE
                        IF ( S(J,J-1).NE.ZERO ) DWORK(J)
     $                         = ALPHA*S(J,J-1)*DWORK(K1+J-2) + DWORK(J)
  160                CONTINUE
C
                     CALL DTRMV( 'Upper', 'No transpose', 'Non-unit',
     $                           KSIZE, S, LDS, DWORK(K1), 1 )
                     CALL DAXPY( KSIZE, ALPHA, DWORK(K1), 1, DWORK, 1 )
                  END IF
                  CALL MB04ND( 'Full', KSIZE, 0, 1, R, LDR, DWORK,
     $                         KSIZE, DWORK, 1, DWORK, 1, DWORK(K2),
     $                         DWORK(K1) )
               END IF
            END IF
            GO TO 90
         END IF
C        END WHILE 90
C
      END IF
      INFO = INFOM
      RETURN
C *** Last line of SB03OT ***
      END
