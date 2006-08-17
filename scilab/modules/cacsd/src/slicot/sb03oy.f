      SUBROUTINE SB03OY( DISCR, LTRANS, ISGN, S, LDS, R, LDR, A, LDA,
     $                   SCALE, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for the Cholesky factor  U  of  X,
C
C        op(U)'*op(U) = X,
C
C     where  U  is a two-by-two upper triangular matrix, either the
C     continuous-time two-by-two Lyapunov equation
C                                         2
C         op(S)'*X + X*op(S) = -ISGN*scale *op(R)'*op(R),
C
C     when DISCR = .FALSE., or the discrete-time two-by-two Lyapunov
C     equation
C                                         2
C         op(S)'*X*op(S) - X = -ISGN*scale *op(R)'*op(R),
C
C     when DISCR = .TRUE., where op(K) = K or K' (i.e., the transpose of
C     the matrix K),  S  is a two-by-two matrix with complex conjugate
C     eigenvalues,  R  is a two-by-two upper triangular matrix,
C     ISGN = -1 or 1,  and  scale  is an output scale factor, set less
C     than or equal to 1 to avoid overflow in  X.  The routine also
C     computes two matrices, B and A, so that
C                                   2
C        B*U = U*S  and  A*U = scale *R,  if  LTRANS = .FALSE.,  or
C                                   2
C        U*B = S*U  and  U*A = scale *R,  if  LTRANS = .TRUE.,
C     which are used by the general Lyapunov solver.
C     In the continuous-time case  ISGN*S  must be stable, so that its
C     eigenvalues must have strictly negative real parts.
C     In the discrete-time case  S  must be convergent if ISGN = 1, that
C     is, its eigenvalues must have moduli less than unity, or  S  must
C     be completely divergent if ISGN = -1, that is, its eigenvalues
C     must have moduli greater than unity.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     DISCR   LOGICAL
C             Specifies the equation to be solved:       2
C             = .FALSE.: op(S)'*X + X*op(S) = -ISGN*scale *op(R)'*op(R);
C                                                        2
C             = .TRUE. : op(S)'*X*op(S) - X = -ISGN*scale *op(R)'*op(R).
C
C     LTRANS  LOGICAL
C             Specifies the form of op(K) to be used, as follows:
C             = .FALSE.:  op(K) = K    (No transpose);
C             = .TRUE. :  op(K) = K**T (Transpose).
C
C     ISGN    INTEGER
C             Specifies the sign of the equation as described before.
C             ISGN may only be 1 or -1.
C
C     Input/Output Parameters
C
C     S       (input/output) DOUBLE PRECISION array, dimension (LDS,2)
C             On entry, S must contain a 2-by-2 matrix.
C             On exit, S contains a 2-by-2 matrix B such that B*U = U*S,
C             if LTRANS = .FALSE., or U*B = S*U, if LTRANS = .TRUE..
C             Notice that if U is nonsingular then
C               B = U*S*inv( U ),  if LTRANS = .FALSE.
C               B = inv( U )*S*U,  if LTRANS = .TRUE..
C
C     LDS     INTEGER
C             The leading dimension of array S.  LDS >= 2.
C
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,2)
C             On entry, R must contain a 2-by-2 upper triangular matrix.
C             The element R( 2, 1 ) is not referenced.
C             On exit, R contains U, the 2-by-2 upper triangular
C             Cholesky factor of the solution X, X = op(U)'*op(U).
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= 2.
C
C     A       (output) DOUBLE PRECISION array, dimension (LDA,2)
C             A contains a 2-by-2 upper triangular matrix A satisfying
C             A*U/scale = scale*R, if LTRANS = .FALSE., or
C             U*A/scale = scale*R, if LTRANS = .TRUE..
C             Notice that if U is nonsingular then
C               A = scale*scale*R*inv( U ),  if LTRANS = .FALSE.
C               A = scale*scale*inv( U )*R,  if LTRANS = .TRUE..
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= 2.
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor, scale, set less than or equal to 1 to
C             prevent the solution overflowing.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
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
C             = 2:  if DISCR = .FALSE., and ISGN*S is not stable or
C                   if DISCR = .TRUE., ISGN = 1 and S is not convergent
C                   or if DISCR = .TRUE., ISGN = -1 and S is not
C                   completely divergent;
C             = 4:  if S has real eigenvalues.
C
C     NOTE: In the interests of speed, this routine does not check all
C           inputs for errors.
C
C     METHOD
C
C     The LAPACK scheme for solving 2-by-2 Sylvester equations is
C     adapted for 2-by-2 Lyapunov equations, but directly computing the
C     Cholesky factor of the solution.
C
C     REFERENCES
C
C     [1] Hammarling S. J.
C         Numerical solution of the stable, non-negative definite
C         Lyapunov equation.
C         IMA J. Num. Anal., 2, pp. 303-325, 1982.
C
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C     Supersedes Release 2.0 routine SB03CY by Sven Hammarling,
C     NAG Ltd., United Kingdom, November 1986.
C     Partly based on SB03CY and PLYAP2 by A. Varga, University of
C     Bochum, May 1992.
C
C     REVISIONS
C
C     Dec. 1997, April 1998.
C
C     KEYWORDS
C
C     Lyapunov equation, orthogonal transformation, real Schur form.
C
C     *****************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE, TWO, FOUR
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0, TWO = 2.0D0,
     $                    FOUR = 4.0D0 )
C     .. Scalar Arguments ..
      LOGICAL           DISCR, LTRANS
      INTEGER           INFO, ISGN, LDA, LDR, LDS
      DOUBLE PRECISION  SCALE
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), R(LDR,*), S(LDS,*)
C     .. Local Scalars ..
      DOUBLE PRECISION  ABSB, ABSG, ABST, ALPHA, BIGNUM, E1, E2, EPS,
     $                  ETA, P1, P3, P3I, P3R, S11, S12, S21, S22,
     $                  SCALOC, SGN, SMIN, SMLNUM, SNP, SNQ, SNT, TEMPI,
     $                  TEMPR, V1, V3
C     .. Local Arrays ..
      DOUBLE PRECISION  CSP(2), CSQ(2), CST(2), DELTA(2), DP(2), DT(2),
     $                  G(2), GAMMA(2), P2(2), T(2), TEMP(2), V2(2),
     $                  X11(2), X12(2), X21(2), X22(2), Y(2)
C     .. External Functions ..
      DOUBLE PRECISION  DLAMCH, DLAPY2, DLAPY3
      EXTERNAL          DLAMCH, DLAPY2, DLAPY3
C     .. External Subroutines ..
      EXTERNAL          DLABAD, DLANV2, SB03OV
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, MAX, SIGN, SQRT
C     .. Executable Statements ..
C
C     The comments in this routine refer to notation and equation
C     numbers in sections 6 and 10 of [1].
C
C     Find the eigenvalue  lambda = E1 - i*E2  of s11.
C
      INFO = 0
      SGN = ISGN
      S11 = S(1,1)
      S12 = S(1,2)
      S21 = S(2,1)
      S22 = S(2,2)
C
C     Set constants to control overflow.
C
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
      SMLNUM = SMLNUM*FOUR / EPS
      BIGNUM = ONE / SMLNUM
C
      SMIN = MAX( SMLNUM, EPS*MAX( ABS( S11 ), ABS( S12 ),
     $                             ABS( S21 ), ABS( S22 ) ) )
      SCALE = ONE
C
      CALL DLANV2( S11, S12, S21, S22, TEMPR, TEMPI, E1, E2, CSP, CSQ )
      IF ( TEMPI.EQ.ZERO ) THEN
         INFO = 4
         RETURN
      END IF
      ABSB = DLAPY2( E1, E2 )
      IF ( DISCR ) THEN
         IF ( SGN*( ABSB - ONE ).GE.ZERO ) THEN
            INFO = 2
            RETURN
         END IF
      ELSE
         IF ( SGN*E1.GE.ZERO ) THEN
            INFO = 2
            RETURN
         END IF
      END IF
C
C     Compute the cos and sine that define  Qhat.  The sine is real.
C
      TEMP(1) = S(1,1) - E1
      TEMP(2) = E2
      IF ( LTRANS ) TEMP(2) = -E2
      CALL SB03OV( TEMP, S(2,1), CSQ, SNQ )
C
C     beta in (6.9) is given by  beta = E1 + i*E2,  compute  t.
C
      TEMP(1) = CSQ(1)*S(1,2) - SNQ*S(1,1)
      TEMP(2) = CSQ(2)*S(1,2)
      TEMPR   = CSQ(1)*S(2,2) - SNQ*S(2,1)
      TEMPI   = CSQ(2)*S(2,2)
      T(1)    = CSQ(1)*TEMP(1) - CSQ(2)*TEMP(2) + SNQ*TEMPR
      T(2)    = CSQ(1)*TEMP(2) + CSQ(2)*TEMP(1) + SNQ*TEMPI
C
      IF ( LTRANS ) THEN
C                                                         (     -- )
C        Case op(M) = M'.  Note that the modified  R  is  ( p3  p2 ).
C                                                         ( 0   p1 )
C
C        Compute the cos and sine that define  Phat.
C
         TEMP(1) =  CSQ(1)*R(2,2) - SNQ*R(1,2)
         TEMP(2) = -CSQ(2)*R(2,2)
         CALL SB03OV( TEMP, -SNQ*R(1,1), CSP, SNP )
C
C        Compute p1, p2 and p3 of the relation corresponding to (6.11).
C
         P1 = TEMP(1)
         TEMP(1) =  CSQ(1)*R(1,2) + SNQ*R(2,2)
         TEMP(2) = -CSQ(2)*R(1,2)
         TEMPR   =  CSQ(1)*R(1,1)
         TEMPI   = -CSQ(2)*R(1,1)
         P2(1)   =  CSP(1)*TEMP(1) - CSP(2)*TEMP(2) + SNP*TEMPR
         P2(2)   = -CSP(1)*TEMP(2) - CSP(2)*TEMP(1) - SNP*TEMPI
         P3R     =  CSP(1)*TEMPR   + CSP(2)*TEMPI   - SNP*TEMP(1)
         P3I     =  CSP(1)*TEMPI   - CSP(2)*TEMPR   - SNP*TEMP(2)
      ELSE
C
C        Case op(M) = M.
C
C        Compute the cos and sine that define  Phat.
C
         TEMP(1) = CSQ(1)*R(1,1) + SNQ*R(1,2)
         TEMP(2) = CSQ(2)*R(1,1)
         CALL SB03OV( TEMP, SNQ*R(2,2), CSP, SNP )
C
C        Compute p1, p2 and p3 of (6.11).
C
         P1 = TEMP(1)
         TEMP(1) = CSQ(1)*R(1,2) - SNQ*R(1,1)
         TEMP(2) = CSQ(2)*R(1,2)
         TEMPR   = CSQ(1)*R(2,2)
         TEMPI   = CSQ(2)*R(2,2)
         P2(1)   = CSP(1)*TEMP(1) - CSP(2)*TEMP(2) + SNP*TEMPR
         P2(2)   = CSP(1)*TEMP(2) + CSP(2)*TEMP(1) + SNP*TEMPI
         P3R     = CSP(1)*TEMPR   + CSP(2)*TEMPI   - SNP*TEMP(1)
         P3I     = CSP(2)*TEMPR   - CSP(1)*TEMPI   + SNP*TEMP(2)
      END IF
C
C     Make  p3  real by multiplying by  conjg ( p3 )/abs( p3 )  to give
C
C     p3 := abs( p3 ).
C
      IF ( P3I.EQ.ZERO ) THEN
         P3 = ABS( P3R )
         DP(1) = SIGN( ONE, P3R )
         DP(2) = ZERO
      ELSE
         P3 = DLAPY2( P3R, P3I )
         DP(1) =  P3R/P3
         DP(2) = -P3I/P3
      END IF
C
C     Now compute the quantities v1, v2, v3 and y in (6.13) - (6.15),
C     or (10.23) - (10.25). Care is taken to avoid overflows.
C
      IF ( DISCR ) THEN
         ALPHA = SQRT( ABS( ONE - ABSB )*( ONE + ABSB ) )
      ELSE
         ALPHA = SQRT( ABS( TWO*E1 ) )
      END IF
C
      SCALOC = ONE
      IF( ALPHA.LT.SMIN ) THEN
         ALPHA = SMIN
         INFO = 1
      END IF
      ABST = ABS( P1 )
      IF( ALPHA.LT.ONE .AND. ABST.GT.ONE ) THEN
         IF( ABST.GT.BIGNUM*ALPHA )
     $      SCALOC = ONE / ABST
      END IF
      IF( SCALOC.NE.ONE ) THEN
         P1    = SCALOC*P1
         P2(1) = SCALOC*P2(1)
         P2(2) = SCALOC*P2(2)
         P3    = SCALOC*P3
         SCALE = SCALOC*SCALE
      END IF
      V1 = P1/ALPHA
C
      IF ( DISCR ) THEN
         G(1) = ( ONE - E1 )*( ONE + E1 ) + E2**2
         G(2) = -TWO*E1*E2
         ABSG =  DLAPY2( G(1), G(2) )
         SCALOC = ONE
         IF( ABSG.LT.SMIN ) THEN
            ABSG = SMIN
            INFO = 1
         END IF
         TEMP(1) = SGN*ALPHA*P2(1) + V1*( E1*T(1) - E2*T(2) )
         TEMP(2) = SGN*ALPHA*P2(2) + V1*( E1*T(2) + E2*T(1) )
         ABST    = MAX( ABS( TEMP(1) ), ABS( TEMP(2) ) )
         IF( ABSG.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSG )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            V1      = SCALOC*V1
            TEMP(1) = SCALOC*TEMP(1)
            TEMP(2) = SCALOC*TEMP(2)
            P1      = SCALOC*P1
            P2(1)   = SCALOC*P2(1)
            P2(2)   = SCALOC*P2(2)
            P3      = SCALOC*P3
            SCALE   = SCALOC*SCALE
         END IF
         TEMP(1) = TEMP(1)/ABSG
         TEMP(2) = TEMP(2)/ABSG
C
         SCALOC = ONE
         V2(1)  = G(1)*TEMP(1) + G(2)*TEMP(2)
         V2(2)  = G(1)*TEMP(2) - G(2)*TEMP(1)
         ABST   = MAX( ABS( V2(1) ), ABS( V2(2) ) )
         IF( ABSG.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSG )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            V1    = SCALOC*V1
            V2(1) = SCALOC*V2(1)
            V2(2) = SCALOC*V2(2)
            P1    = SCALOC*P1
            P2(1) = SCALOC*P2(1)
            P2(2) = SCALOC*P2(2)
            P3    = SCALOC*P3
            SCALE = SCALOC*SCALE
         END IF
         V2(1) = V2(1)/ABSG
         V2(2) = V2(2)/ABSG
C
         SCALOC  = ONE
         TEMP(1) = P1*T(1) - TWO*E2*P2(2)
         TEMP(2) = P1*T(2) + TWO*E2*P2(1)
         ABST    = MAX( ABS( TEMP(1) ), ABS( TEMP(2) ) )
         IF( ABSG.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSG )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            TEMP(1) = SCALOC*TEMP(1)
            TEMP(2) = SCALOC*TEMP(2)
            V1      = SCALOC*V1
            V2(1)   = SCALOC*V2(1)
            V2(2)   = SCALOC*V2(2)
            P3      = SCALOC*P3
            SCALE   = SCALOC*SCALE
         END IF
         TEMP(1) = TEMP(1)/ABSG
         TEMP(2) = TEMP(2)/ABSG
C
         SCALOC  = ONE
         Y(1)    = -( G(1)*TEMP(1) + G(2)*TEMP(2) )
         Y(2)    = -( G(1)*TEMP(2) - G(2)*TEMP(1) )
         ABST    = MAX( ABS( Y(1) ), ABS( Y(2) ) )
         IF( ABSG.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSG )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            Y(1)  = SCALOC*Y(1)
            Y(2)  = SCALOC*Y(2)
            V1    = SCALOC*V1
            V2(1) = SCALOC*V2(1)
            V2(2) = SCALOC*V2(2)
            P3    = SCALOC*P3
            SCALE = SCALOC*SCALE
         END IF
         Y(1) = Y(1)/ABSG
         Y(2) = Y(2)/ABSG
      ELSE
C
         SCALOC = ONE
         IF( ABSB.LT.SMIN ) THEN
            ABSB = SMIN
            INFO = 1
         END IF
         TEMP(1) = SGN*ALPHA*P2(1) + V1*T(1)
         TEMP(2) = SGN*ALPHA*P2(2) + V1*T(2)
         ABST    = MAX( ABS( TEMP(1) ), ABS( TEMP(2) ) )
         IF( ABSB.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSB )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            V1      = SCALOC*V1
            TEMP(1) = SCALOC*TEMP(1)
            TEMP(2) = SCALOC*TEMP(2)
            P2(1)   = SCALOC*P2(1)
            P2(2)   = SCALOC*P2(2)
            P3      = SCALOC*P3
            SCALE   = SCALOC*SCALE
         END IF
         TEMP(1) = TEMP(1)/( TWO*ABSB )
         TEMP(2) = TEMP(2)/( TWO*ABSB )
         SCALOC  = ONE
         V2(1)   = -( E1*TEMP(1) + E2*TEMP(2) )
         V2(2)   = -( E1*TEMP(2) - E2*TEMP(1) )
         ABST = MAX( ABS( V2(1) ), ABS( V2(2) ) )
         IF( ABSB.LT.ONE .AND. ABST.GT.ONE ) THEN
            IF( ABST.GT.BIGNUM*ABSB )
     $         SCALOC = ONE / ABST
         END IF
         IF( SCALOC.NE.ONE ) THEN
            V1    = SCALOC*V1
            V2(1) = SCALOC*V2(1)
            V2(2) = SCALOC*V2(2)
            P2(1) = SCALOC*P2(1)
            P2(2) = SCALOC*P2(2)
            P3    = SCALOC*P3
            SCALE = SCALOC*SCALE
         END IF
         V2(1) = V2(1)/ABSB
         V2(2) = V2(2)/ABSB
         Y(1)  = P2(1) - ALPHA*V2(1)
         Y(2)  = P2(2) - ALPHA*V2(2)
      END IF
C
      SCALOC = ONE
      V3     = DLAPY3( P3, Y(1), Y(2) )
      IF( ALPHA.LT.ONE .AND. V3.GT.ONE ) THEN
         IF( V3.GT.BIGNUM*ALPHA )
     $      SCALOC = ONE / V3
      END IF
      IF( SCALOC.NE.ONE ) THEN
         V1    = SCALOC*V1
         V2(1) = SCALOC*V2(1)
         V2(2) = SCALOC*V2(2)
         V3    = SCALOC*V3
         P3    = SCALOC*P3
         SCALE = SCALOC*SCALE
      END IF
      V3 = V3/ALPHA
C
      IF ( LTRANS ) THEN
C
C        Case op(M) = M'.
C
C        Form  X = conjg( Qhat' )*v11.
C
         X11(1) =  CSQ(1)*V3
         X11(2) =  CSQ(2)*V3
         X21(1) =  SNQ*V3
         X12(1) =  CSQ(1)*V2(1) + CSQ(2)*V2(2) - SNQ*V1
         X12(2) = -CSQ(1)*V2(2) + CSQ(2)*V2(1)
         X22(1) =  CSQ(1)*V1 + SNQ*V2(1)
         X22(2) = -CSQ(2)*V1 - SNQ*V2(2)
C
C        Obtain u11 from the RQ-factorization of X. The conjugate of
C        X22 should be taken.
C
         X22(2) = -X22(2)
         CALL SB03OV( X22, X21(1), CST, SNT )
         R(2,2) = X22(1)
         R(1,2) = CST(1)*X12(1) - CST(2)*X12(2) + SNT*X11(1)
         TEMPR  = CST(1)*X11(1) + CST(2)*X11(2) - SNT*X12(1)
         TEMPI  = CST(1)*X11(2) - CST(2)*X11(1) - SNT*X12(2)
         IF ( TEMPI.EQ.ZERO ) THEN
            R(1,1) = ABS( TEMPR )
            DT(1)  = SIGN( ONE, TEMPR )
            DT(2)  = ZERO
         ELSE
            R(1,1) =  DLAPY2( TEMPR, TEMPI )
            DT(1)  =  TEMPR/R(1,1)
            DT(2)  = -TEMPI/R(1,1)
         END IF
      ELSE
C
C        Case op(M) = M.
C
C        Now form  X = v11*conjg( Qhat' ).
C
         X11(1) =  CSQ(1)*V1 - SNQ*V2(1)
         X11(2) = -CSQ(2)*V1 + SNQ*V2(2)
         X21(1) = -SNQ*V3
         X12(1) =  CSQ(1)*V2(1) + CSQ(2)*V2(2) + SNQ*V1
         X12(2) = -CSQ(1)*V2(2) + CSQ(2)*V2(1)
         X22(1) =  CSQ(1)*V3
         X22(2) =  CSQ(2)*V3
C
C        Obtain u11 from the QR-factorization of X.
C
         CALL SB03OV( X11, X21(1), CST, SNT )
         R(1,1) = X11(1)
         R(1,2) = CST(1)*X12(1) + CST(2)*X12(2) + SNT*X22(1)
         TEMPR  = CST(1)*X22(1) - CST(2)*X22(2) - SNT*X12(1)
         TEMPI  = CST(1)*X22(2) + CST(2)*X22(1) - SNT*X12(2)
         IF ( TEMPI.EQ.ZERO ) THEN
            R(2,2) = ABS( TEMPR )
            DT(1)  = SIGN( ONE, TEMPR )
            DT(2)  = ZERO
         ELSE
            R(2,2) =  DLAPY2( TEMPR, TEMPI )
            DT(1)  =  TEMPR/R(2,2)
            DT(2)  = -TEMPI/R(2,2)
         END IF
      END IF
C
C     The computations below are not needed when B and A are not
C     useful. Compute delta, eta and gamma as in (6.21) or (10.26).
C
      IF ( ( Y(1).EQ.ZERO ).AND.( Y(2).EQ.ZERO ) ) THEN
         DELTA(1) = ZERO
         DELTA(2) = ZERO
         GAMMA(1) = ZERO
         GAMMA(2) = ZERO
         ETA = ALPHA
      ELSE
         DELTA(1) =  Y(1)/V3
         DELTA(2) =  Y(2)/V3
         GAMMA(1) = -ALPHA*DELTA(1)
         GAMMA(2) = -ALPHA*DELTA(2)
         ETA = P3/V3
         IF ( DISCR ) THEN
            TEMPR    = E1*DELTA(1) - E2*DELTA(2)
            DELTA(2) = E1*DELTA(2) + E2*DELTA(1)
            DELTA(1) = TEMPR
         END IF
      END IF
C
      IF ( LTRANS ) THEN
C
C        Case op(M) = M'.
C
C        Find  X = conjg( That' )*( inv( v11 )*s11hat*v11 ).
C        ( Defer the scaling.)
C
         X11(1) =  CST(1)*E1 + CST(2)*E2
         X11(2) = -CST(1)*E2 + CST(2)*E1
         X21(1) =  SNT*E1
         X21(2) = -SNT*E2
         X12(1) =  SGN*(  CST(1)*GAMMA(1) + CST(2)*GAMMA(2) ) - SNT*E1
         X12(2) =  SGN*( -CST(1)*GAMMA(2) + CST(2)*GAMMA(1) ) - SNT*E2
         X22(1) =  CST(1)*E1 + CST(2)*E2 + SGN*SNT*GAMMA(1)
         X22(2) =  CST(1)*E2 - CST(2)*E1 - SGN*SNT*GAMMA(2)
C
C        Now find  B = X*That. ( Include the scaling here.)
C
         S(1,1) = CST(1)*X11(1) + CST(2)*X11(2) - SNT*X12(1)
         TEMPR  = CST(1)*X21(1) + CST(2)*X21(2) - SNT*X22(1)
         TEMPI  = CST(1)*X21(2) - CST(2)*X21(1) - SNT*X22(2)
         S(2,1) = DT(1)*TEMPR   - DT(2)*TEMPI
         TEMPR  = CST(1)*X12(1) - CST(2)*X12(2) + SNT*X11(1)
         TEMPI  = CST(1)*X12(2) + CST(2)*X12(1) + SNT*X11(2)
         S(1,2) = DT(1)*TEMPR   + DT(2)*TEMPI
         S(2,2) = CST(1)*X22(1) - CST(2)*X22(2) + SNT*X21(1)
C
C        Form  X = ( inv( v11 )*p11 )*conjg( Phat' ).
C
         TEMPR  =  DP(1)*ETA
         TEMPI  = -DP(2)*ETA
         X11(1) =  CSP(1)*TEMPR - CSP(2)*TEMPI + SNP*DELTA(1)
         X11(2) =  CSP(1)*TEMPI + CSP(2)*TEMPR - SNP*DELTA(2)
         X21(1) =  SNP*ALPHA
         X12(1) = -SNP*TEMPR + CSP(1)*DELTA(1) - CSP(2)*DELTA(2)
         X12(2) = -SNP*TEMPI - CSP(1)*DELTA(2) - CSP(2)*DELTA(1)
         X22(1) =  CSP(1)*ALPHA
         X22(2) = -CSP(2)*ALPHA
C
C        Finally form  A = conjg( That' )*X.
C
         TEMPR  = CST(1)*X11(1) - CST(2)*X11(2) - SNT*X21(1)
         TEMPI  = CST(1)*X22(2) + CST(2)*X22(1)
         A(1,1) = DT(1)*TEMPR   + DT(2)*TEMPI
         TEMPR  = CST(1)*X12(1) - CST(2)*X12(2) - SNT*X22(1)
         TEMPI  = CST(1)*X12(2) + CST(2)*X12(1) - SNT*X22(1)
         A(1,2) = DT(1)*TEMPR   + DT(2)*TEMPI
         A(2,1) = ZERO
         A(2,2) = CST(1)*X22(1) + CST(2)*X22(2) + SNT*X12(1)
      ELSE
C
C        Case op(M) = M.
C
C        Find  X = That*( v11*s11hat*inv( v11 ) ). ( Defer the scaling.)
C
         X11(1) =  CST(1)*E1 + CST(2)*E2
         X11(2) =  CST(1)*E2 - CST(2)*E1
         X21(1) = -SNT*E1
         X21(2) = -SNT*E2
         X12(1) =  SGN*(  CST(1)*GAMMA(1) - CST(2)*GAMMA(2) ) + SNT*E1
         X12(2) =  SGN*( -CST(1)*GAMMA(2) - CST(2)*GAMMA(1) ) - SNT*E2
         X22(1) =  CST(1)*E1 + CST(2)*E2 - SGN*SNT*GAMMA(1)
         X22(2) = -CST(1)*E2 + CST(2)*E1 + SGN*SNT*GAMMA(2)
C
C        Now find  B = X*conjg( That' ). ( Include the scaling here.)
C
         S(1,1) = CST(1)*X11(1) - CST(2)*X11(2) + SNT*X12(1)
         TEMPR  = CST(1)*X21(1) - CST(2)*X21(2) + SNT*X22(1)
         TEMPI  = CST(1)*X21(2) + CST(2)*X21(1) + SNT*X22(2)
         S(2,1) = DT(1)*TEMPR   - DT(2)*TEMPI
         TEMPR  = CST(1)*X12(1) + CST(2)*X12(2) - SNT*X11(1)
         TEMPI  = CST(1)*X12(2) - CST(2)*X12(1) - SNT*X11(2)
         S(1,2) = DT(1)*TEMPR   + DT(2)*TEMPI
         S(2,2) = CST(1)*X22(1) + CST(2)*X22(2) - SNT*X21(1)
C
C        Form  X = Phat*( p11*inv( v11 ) ).
C
         TEMPR  =  DP(1)*ETA
         TEMPI  = -DP(2)*ETA
         X11(1) =  CSP(1)*ALPHA
         X11(2) =  CSP(2)*ALPHA
         X21(1) =  SNP*ALPHA
         X12(1) =  CSP(1)*DELTA(1) + CSP(2)*DELTA(2) - SNP*TEMPR
         X12(2) = -CSP(1)*DELTA(2) + CSP(2)*DELTA(1) - SNP*TEMPI
         X22(1) =  CSP(1)*TEMPR + CSP(2)*TEMPI + SNP*DELTA(1)
         X22(2) =  CSP(1)*TEMPI - CSP(2)*TEMPR - SNP*DELTA(2)
C
C        Finally form  A = X*conjg( That' ).
C
         A(1,1) = CST(1)*X11(1) - CST(2)*X11(2) + SNT*X12(1)
         A(2,1) = ZERO
         A(1,2) = CST(1)*X12(1) + CST(2)*X12(2) - SNT*X11(1)
         TEMPR  = CST(1)*X22(1) + CST(2)*X22(2) - SNT*X21(1)
         TEMPI  = CST(1)*X22(2) - CST(2)*X22(1)
         A(2,2) = DT(1)*TEMPR   + DT(2)*TEMPI
      END IF
C
      IF( SCALE.NE.ONE ) THEN
         A(1,1) = SCALE*A(1,1)
         A(1,2) = SCALE*A(1,2)
         A(2,2) = SCALE*A(2,2)
      END IF
C
      RETURN
C *** Last line of SB03OY ***
      END
