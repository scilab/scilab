      SUBROUTINE SB04PX( LTRANL, LTRANR, ISGN, N1, N2, TL, LDTL, TR,
     $                   LDTR, B, LDB, SCALE, X, LDX, XNORM, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To solve for the N1-by-N2 matrix X, 1 <= N1,N2 <= 2, in
C
C            op(TL)*X*op(TR) + ISGN*X = SCALE*B,
C
C     where TL is N1-by-N1, TR is N2-by-N2, B is N1-by-N2, and ISGN = 1
C     or -1.  op(T) = T or T', where T' denotes the transpose of T.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     LTRANL  LOGICAL
C             Specifies the form of op(TL) to be used, as follows:
C             = .FALSE.:  op(TL) = TL,
C             = .TRUE. :  op(TL) = TL'.
C
C     LTRANR  LOGICAL
C             Specifies the form of op(TR) to be used, as follows:
C             = .FALSE.:  op(TR) = TR,
C             = .TRUE. :  op(TR) = TR'.
C
C     ISGN    INTEGER
C             Specifies the sign of the equation as described before.
C             ISGN may only be 1 or -1.
C
C     Input/Output Parameters
C
C     N1      (input) INTEGER
C             The order of matrix TL.  N1 may only be 0, 1 or 2.
C
C     N2      (input) INTEGER
C             The order of matrix TR.  N2 may only be 0, 1 or 2.
C
C     TL      (input) DOUBLE PRECISION array, dimension (LDTL,N1)
C             The leading N1-by-N1 part of this array must contain the
C             matrix TL.
C
C     LDTL    INTEGER
C             The leading dimension of array TL.  LDTL >= MAX(1,N1).
C
C     TR      (input) DOUBLE PRECISION array, dimension (LDTR,N2)
C             The leading N2-by-N2 part of this array must contain the
C             matrix TR.
C
C     LDTR    INTEGER
C             The leading dimension of array TR.  LDTR >= MAX(1,N2).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,N2)
C             The leading N1-by-N2 part of this array must contain the
C             right-hand side of the equation.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N1).
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor. SCALE is chosen less than or equal to 1
C             to prevent the solution overflowing.
C
C     X       (output) DOUBLE PRECISION array, dimension (LDX,N2)
C             The leading N1-by-N2 part of this array contains the
C             solution of the equation.
C             Note that X may be identified with B in the calling
C             statement.
C
C     LDX     INTEGER
C             The leading dimension of array X.  LDX >= MAX(1,N1).
C
C     XNORM   (output) DOUBLE PRECISION
C             The infinity-norm of the solution.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             = 1:  if TL and -ISGN*TR have almost reciprocal
C                   eigenvalues, so TL or TR is perturbed to get a 
C                   nonsingular equation.
C
C             NOTE: In the interests of speed, this routine does not
C                   check the inputs for errors.
C
C     METHOD
C
C     The equivalent linear algebraic system of equations is formed and
C     solved using Gaussian elimination with complete pivoting.
C
C     REFERENCES
C
C     [1] Anderson, E., Bai, Z., Bischof, C., Demmel, J., Dongarra, J.,
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
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, May 2000.
C     This is a modification and slightly more efficient version of 
C     SLICOT Library routine SB03MU.
C
C     REVISIONS
C
C     -  
C
C     KEYWORDS
C
C     Discrete-time system, Sylvester equation, matrix algebra.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, HALF, EIGHT
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0,
     $                     TWO = 2.0D+0, HALF = 0.5D+0, EIGHT = 8.0D+0 )
C     ..
C     .. Scalar Arguments ..
      LOGICAL            LTRANL, LTRANR
      INTEGER            INFO, ISGN, LDB, LDTL, LDTR, LDX, N1, N2
      DOUBLE PRECISION   SCALE, XNORM
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), TL( LDTL, * ), TR( LDTR, * ),
     $                   X( LDX, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            BSWAP, XSWAP
      INTEGER            I, IP, IPIV, IPSV, J, JP, JPSV, K
      DOUBLE PRECISION   BET, EPS, GAM, L21, SGN, SMIN, SMLNUM, TAU1,
     $                   TEMP, U11, U12, U22, XMAX
C     ..
C     .. Local Arrays ..
      LOGICAL            BSWPIV( 4 ), XSWPIV( 4 )
      INTEGER            JPIV( 4 ), LOCL21( 4 ), LOCU12( 4 ),
     $                   LOCU22( 4 )
      DOUBLE PRECISION   BTMP( 4 ), T16( 4, 4 ), TMP( 4 ), X2( 2 )
C     ..
C     .. External Functions ..
      INTEGER            IDAMAX
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, IDAMAX
C     ..
C     .. External Subroutines ..
      EXTERNAL           DSWAP
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
C     ..
C     .. Data statements ..
      DATA               LOCU12 / 3, 4, 1, 2 / , LOCL21 / 2, 1, 4, 3 / ,
     $                   LOCU22 / 4, 3, 2, 1 /
      DATA               XSWPIV / .FALSE., .FALSE., .TRUE., .TRUE. /
      DATA               BSWPIV / .FALSE., .TRUE., .FALSE., .TRUE. /
C     ..
C     .. Executable Statements ..
C
C     Do not check the input parameters for errors.
C
      INFO  = 0
      SCALE = ONE
C
C     Quick return if possible.
C
      IF( N1.EQ.0 .OR. N2.EQ.0 ) THEN
         XNORM = ZERO
         RETURN
      END IF
C
C     Set constants to control overflow.
C
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      SGN = ISGN
C
      K = N1 + N1 + N2 - 2
      GO TO ( 10, 20, 30, 50 )K
C
C     1-by-1: TL11*X*TR11 + ISGN*X = B11.
C
   10 CONTINUE
      TAU1 = TL( 1, 1 )*TR( 1, 1 ) + SGN
      BET  = ABS( TAU1 )
      IF( BET.LE.SMLNUM ) THEN
         TAU1 = SMLNUM
         BET  = SMLNUM
         INFO = 1
      END IF
C
      GAM = ABS( B( 1, 1 ) )
      IF( SMLNUM*GAM.GT.BET )
     $   SCALE = ONE / GAM
C
      X( 1, 1 ) = ( B( 1, 1 )*SCALE ) / TAU1
      XNORM = ABS( X( 1, 1 ) )
      RETURN
C
C     1-by-2:
C     TL11*[X11 X12]*op[TR11 TR12] + ISGN*[X11 X12] = [B11 B12].
C                      [TR21 TR22]
C
   20 CONTINUE
C
      SMIN = MAX( MAX( ABS( TR( 1, 1 ) ), ABS( TR( 1, 2 ) ),
     $                 ABS( TR( 2, 1 ) ), ABS( TR( 2, 2 ) ) )
     $                *ABS( TL( 1, 1 ) )*EPS,
     $            SMLNUM )
      TMP( 1 ) = TL( 1, 1 )*TR( 1, 1 ) + SGN
      TMP( 4 ) = TL( 1, 1 )*TR( 2, 2 ) + SGN
      IF( LTRANR ) THEN
         TMP( 2 ) = TL( 1, 1 )*TR( 2, 1 )
         TMP( 3 ) = TL( 1, 1 )*TR( 1, 2 )
      ELSE
         TMP( 2 ) = TL( 1, 1 )*TR( 1, 2 )
         TMP( 3 ) = TL( 1, 1 )*TR( 2, 1 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 1, 2 )
      GO TO 40
C
C     2-by-1:
C     op[TL11 TL12]*[X11]*TR11 + ISGN*[X11] = [B11].
C       [TL21 TL22] [X21]             [X21]   [B21]
C
   30 CONTINUE
      SMIN = MAX( MAX( ABS( TL( 1, 1 ) ), ABS( TL( 1, 2 ) ),
     $                 ABS( TL( 2, 1 ) ), ABS( TL( 2, 2 ) ) )
     $                *ABS( TR( 1, 1 ) )*EPS,
     $            SMLNUM )
      TMP( 1 ) = TL( 1, 1 )*TR( 1, 1 ) + SGN
      TMP( 4 ) = TL( 2, 2 )*TR( 1, 1 ) + SGN
      IF( LTRANL ) THEN
         TMP( 2 ) = TL( 1, 2 )*TR( 1, 1 )
         TMP( 3 ) = TL( 2, 1 )*TR( 1, 1 )
      ELSE
         TMP( 2 ) = TL( 2, 1 )*TR( 1, 1 )
         TMP( 3 ) = TL( 1, 2 )*TR( 1, 1 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )
   40 CONTINUE
C
C     Solve 2-by-2 system using complete pivoting.
C     Set pivots less than SMIN to SMIN.
C
      IPIV = IDAMAX( 4, TMP, 1 )
      U11  = TMP( IPIV )
      IF( ABS( U11 ).LE.SMIN ) THEN
         INFO = 1
         U11  = SMIN
      END IF
      U12 = TMP( LOCU12( IPIV ) )
      L21 = TMP( LOCL21( IPIV ) ) / U11
      U22 = TMP( LOCU22( IPIV ) ) - U12*L21
      XSWAP = XSWPIV( IPIV )
      BSWAP = BSWPIV( IPIV )
      IF( ABS( U22 ).LE.SMIN ) THEN
         INFO = 1
         U22  = SMIN
      END IF
      IF( BSWAP ) THEN
         TEMP = BTMP( 2 )
         BTMP( 2 ) = BTMP( 1 ) - L21*TEMP
         BTMP( 1 ) = TEMP
      ELSE
         BTMP( 2 ) = BTMP( 2 ) - L21*BTMP( 1 )
      END IF
      IF( ( TWO*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( U22 ) .OR.
     $    ( TWO*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( U11 ) ) THEN
         SCALE = HALF / MAX( ABS( BTMP( 1 ) ), ABS( BTMP( 2 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
      END IF
      X2( 2 ) = BTMP( 2 ) / U22
      X2( 1 ) = BTMP( 1 ) / U11 - ( U12 / U11 )*X2( 2 )
      IF( XSWAP ) THEN
         TEMP = X2( 2 )
         X2( 2 ) = X2( 1 )
         X2( 1 ) = TEMP
      END IF
      X( 1, 1 ) = X2( 1 )
      IF( N1.EQ.1 ) THEN
         X( 1, 2 ) = X2( 2 )
         XNORM = ABS( X2( 1 ) ) + ABS( X2( 2 ) )
      ELSE
         X( 2, 1 ) = X2( 2 )
         XNORM = MAX( ABS( X2( 1 ) ), ABS( X2( 2 ) ) )
      END IF
      RETURN
C
C     2-by-2:
C     op[TL11 TL12]*[X11 X12]*op[TR11 TR12] + ISGN*[X11 X12] = [B11 B12]
C       [TL21 TL22] [X21 X22]   [TR21 TR22]        [X21 X22]   [B21 B22]
C
C     Solve equivalent 4-by-4 system using complete pivoting.
C     Set pivots less than SMIN to SMIN.
C
   50 CONTINUE
      SMIN = MAX( ABS( TR( 1, 1 ) ), ABS( TR( 1, 2 ) ),
     $            ABS( TR( 2, 1 ) ), ABS( TR( 2, 2 ) ) )
      SMIN = MAX( ABS( TL( 1, 1 ) ), ABS( TL( 1, 2 ) ),
     $            ABS( TL( 2, 1 ) ), ABS( TL( 2, 2 ) ) )*SMIN
      SMIN = MAX( EPS*SMIN, SMLNUM )
      T16( 1, 1 ) = TL( 1, 1 )*TR( 1, 1 ) + SGN
      T16( 2, 2 ) = TL( 2, 2 )*TR( 1, 1 ) + SGN
      T16( 3, 3 ) = TL( 1, 1 )*TR( 2, 2 ) + SGN
      T16( 4, 4 ) = TL( 2, 2 )*TR( 2, 2 ) + SGN
      IF( LTRANL ) THEN
         T16( 1, 2 ) = TL( 2, 1 )*TR( 1, 1 )
         T16( 2, 1 ) = TL( 1, 2 )*TR( 1, 1 )
         T16( 3, 4 ) = TL( 2, 1 )*TR( 2, 2 )
         T16( 4, 3 ) = TL( 1, 2 )*TR( 2, 2 )
      ELSE
         T16( 1, 2 ) = TL( 1, 2 )*TR( 1, 1 )
         T16( 2, 1 ) = TL( 2, 1 )*TR( 1, 1 )
         T16( 3, 4 ) = TL( 1, 2 )*TR( 2, 2 )
         T16( 4, 3 ) = TL( 2, 1 )*TR( 2, 2 )
      END IF
      IF( LTRANR ) THEN
         T16( 1, 3 ) = TL( 1, 1 )*TR( 1, 2 )
         T16( 2, 4 ) = TL( 2, 2 )*TR( 1, 2 )
         T16( 3, 1 ) = TL( 1, 1 )*TR( 2, 1 )
         T16( 4, 2 ) = TL( 2, 2 )*TR( 2, 1 )
      ELSE
         T16( 1, 3 ) = TL( 1, 1 )*TR( 2, 1 )
         T16( 2, 4 ) = TL( 2, 2 )*TR( 2, 1 )
         T16( 3, 1 ) = TL( 1, 1 )*TR( 1, 2 )
         T16( 4, 2 ) = TL( 2, 2 )*TR( 1, 2 )
      END IF
      IF( LTRANL .AND. LTRANR ) THEN
         T16( 1, 4 ) = TL( 2, 1 )*TR( 1, 2 )
         T16( 2, 3 ) = TL( 1, 2 )*TR( 1, 2 )
         T16( 3, 2 ) = TL( 2, 1 )*TR( 2, 1 )
         T16( 4, 1 ) = TL( 1, 2 )*TR( 2, 1 )
      ELSE IF( LTRANL .AND. .NOT.LTRANR ) THEN
         T16( 1, 4 ) = TL( 2, 1 )*TR( 2, 1 )
         T16( 2, 3 ) = TL( 1, 2 )*TR( 2, 1 )
         T16( 3, 2 ) = TL( 2, 1 )*TR( 1, 2 )
         T16( 4, 1 ) = TL( 1, 2 )*TR( 1, 2 )
      ELSE IF( .NOT.LTRANL .AND. LTRANR ) THEN
          T16( 1, 4 ) = TL( 1, 2 )*TR( 1, 2 )
          T16( 2, 3 ) = TL( 2, 1 )*TR( 1, 2 )
          T16( 3, 2 ) = TL( 1, 2 )*TR( 2, 1 )
          T16( 4, 1 ) = TL( 2, 1 )*TR( 2, 1 )
      ELSE
          T16( 1, 4 ) = TL( 1, 2 )*TR( 2, 1 )
          T16( 2, 3 ) = TL( 2, 1 )*TR( 2, 1 )
          T16( 3, 2 ) = TL( 1, 2 )*TR( 1, 2 )
          T16( 4, 1 ) = TL( 2, 1 )*TR( 1, 2 ) 
      END IF        
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )
      BTMP( 3 ) = B( 1, 2 )
      BTMP( 4 ) = B( 2, 2 )
C
C     Perform elimination.
C
      DO 100 I = 1, 3
         XMAX = ZERO
C
         DO 70 IP = I, 4
C
            DO 60 JP = I, 4
               IF( ABS( T16( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( T16( IP, JP ) )
                  IPSV = IP
                  JPSV = JP
               END IF
   60       CONTINUE
C
   70    CONTINUE
C
         IF( IPSV.NE.I ) THEN
            CALL DSWAP( 4, T16( IPSV, 1 ), 4, T16( I, 1 ), 4 )
            TEMP = BTMP( I )
            BTMP( I ) = BTMP( IPSV )
            BTMP( IPSV ) = TEMP
         END IF
         IF( JPSV.NE.I )
     $      CALL DSWAP( 4, T16( 1, JPSV ), 1, T16( 1, I ), 1 )
         JPIV( I ) = JPSV
         IF( ABS( T16( I, I ) ).LT.SMIN ) THEN
            INFO = 1
            T16( I, I ) = SMIN
         END IF
C
         DO 90 J = I + 1, 4
            T16( J, I ) = T16( J, I ) / T16( I, I )
            BTMP( J ) = BTMP( J ) - T16( J, I )*BTMP( I )
C
            DO 80 K = I + 1, 4
               T16( J, K ) = T16( J, K ) - T16( J, I )*T16( I, K )
   80       CONTINUE
C
   90    CONTINUE
C
  100 CONTINUE
C
      IF( ABS( T16( 4, 4 ) ).LT.SMIN )
     $   T16( 4, 4 ) = SMIN
      IF( ( EIGHT*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( T16( 1, 1 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( T16( 2, 2 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 3 ) ).GT.ABS( T16( 3, 3 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 4 ) ).GT.ABS( T16( 4, 4 ) ) ) THEN
         SCALE = ( ONE / EIGHT ) / MAX( ABS( BTMP( 1 ) ),
     $                ABS( BTMP( 2 ) ), ABS( BTMP( 3 ) ),
     $                ABS( BTMP( 4 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
         BTMP( 3 ) = BTMP( 3 )*SCALE
         BTMP( 4 ) = BTMP( 4 )*SCALE
      END IF
C
      DO 120 I = 1, 4
         K = 5 - I
         TEMP = ONE / T16( K, K )
         TMP( K ) = BTMP( K )*TEMP
C
         DO 110 J = K + 1, 4
            TMP( K ) = TMP( K ) - ( TEMP*T16( K, J ) )*TMP( J )
  110    CONTINUE
C
  120 CONTINUE
C
      DO 130 I = 1, 3
         IF( JPIV( 4-I ).NE.4-I ) THEN
            TEMP = TMP( 4-I )
            TMP( 4-I ) = TMP( JPIV( 4-I ) )
            TMP( JPIV( 4-I ) ) = TEMP
         END IF
  130 CONTINUE
C
      X( 1, 1 ) = TMP( 1 )
      X( 2, 1 ) = TMP( 2 )
      X( 1, 2 ) = TMP( 3 )
      X( 2, 2 ) = TMP( 4 )
      XNORM = MAX( ABS( TMP( 1 ) ) + ABS( TMP( 3 ) ),
     $             ABS( TMP( 2 ) ) + ABS( TMP( 4 ) ) )
C
      RETURN
C *** Last line of SB04PX ***
      END
