      SUBROUTINE DLALD2( LTRAN, T, LDT, B, LDB, SCALE, X, LDX, XNORM,
     $                   INFO )
*
*  -- RICCPACK auxiliary routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      LOGICAL            LTRAN
      INTEGER            INFO, LDB, LDT, LDX
      DOUBLE PRECISION   SCALE, XNORM
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), T( LDT, * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  DLALD2 solves for the 2 by 2 symmetric matrix X in
*
*         op(T')*X*op(T) - X = SCALE*B,
*
*  where T is 2 by 2, B is symmetric 2 by 2, and op(T) = T or T',
*  where T' denotes the transpose of T.
*
*  Arguments
*  =========
*
*  LTRAN   (input) LOGICAL
*          On entry, LTRAN specifies the op(T):
*             = .FALSE., op(T) = T,
*             = .TRUE., op(T) = T'.
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,2)
*          On entry, T contains an 2 by 2 matrix.
*
*  LDT     (input) INTEGER
*          The leading dimension of the matrix T. LDT >= 2.
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB,2)
*          On entry, the 2 by 2 matrix B contains the symmetric
*          right-hand side of the equation.
*
*  LDB     (input) INTEGER
*          The leading dimension of the matrix B. LDB >= 2.
*
*  SCALE   (output) DOUBLE PRECISION
*          On exit, SCALE contains the scale factor. SCALE is chosen
*          less than or equal to 1 to prevent the solution overflowing.
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,2)
*          On exit, X contains the 2 by 2 symmetric solution.
*
*  LDX     (input) INTEGER
*          The leading dimension of the matrix X. LDX >= 2.
*
*  XNORM   (output) DOUBLE PRECISION
*          On exit, XNORM is the infinity-norm of the solution.
*
*  INFO    (output) INTEGER
*          On exit, INFO is set to
*             0: successful exit.
*             1: T has almost reciprocal eigenvalues, so T
*                is perturbed to get a nonsingular equation.
*          NOTE: In the interests of speed, this routine does not
*                check the inputs for errors.
*
* =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      DOUBLE PRECISION   FOUR
      PARAMETER          ( FOUR = 4.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, IP, IPSV, J, JP, JPSV, K
      DOUBLE PRECISION   EPS, SMIN, SMLNUM, TEMP, XMAX
*     ..
*     .. Local Arrays ..
      INTEGER            JPIV( 3 )
      DOUBLE PRECISION   BTMP( 3 ), T9( 3, 3 ), TMP( 3 )
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DSWAP
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
*     ..
*     .. Executable Statements ..
*
*     Do not check the input parameters for errors
*
      INFO = 0
*
*     Set constants to control overflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
*
*     Solve equivalent 3 by 3 system using complete pivoting.
*     Set pivots less than SMIN to SMIN.
*
      SMIN = MAX( ABS( T( 1, 1 ) ), ABS( T( 1, 2 ) ),
     $       ABS( T( 2, 1 ) ), ABS( T( 2, 2 ) ) )
      SMIN = MAX( EPS*SMIN, SMLNUM )
      BTMP( 1 ) = ZERO
      CALL DCOPY( 9, BTMP, 0, T9, 1 )
      T9( 1, 1 ) = T( 1, 1 )*T( 1, 1 ) - ONE
      T9( 2, 2 ) = T( 1, 1 )*T( 2, 2 ) + T( 1, 2 )*T( 2, 1 ) - ONE
      T9( 3, 3 ) = T( 2, 2 )*T( 2, 2 ) - ONE
      IF( LTRAN ) THEN
         T9( 1, 2 ) = T( 1, 1 )*T( 1, 2 ) + T( 1, 1 )*T( 1, 2 )
         T9( 1, 3 ) = T( 1, 2 )*T( 1, 2 )
         T9( 2, 1 ) = T( 1, 1 )*T( 2, 1 )
         T9( 2, 3 ) = T( 1, 2 )*T( 2, 2 ) 
         T9( 3, 1 ) = T( 2, 1 )*T( 2, 1 )
         T9( 3, 2 ) = T( 2, 1 )*T( 2, 2 ) + T( 2, 1 )*T( 2, 2 )
      ELSE
         T9( 1, 2 ) = T( 1, 1 )*T( 2, 1 ) + T( 1, 1 )*T( 2, 1 )
         T9( 1, 3 ) = T( 2, 1 )*T( 2, 1 )
         T9( 2, 1 ) = T( 1, 1 )*T( 1, 2 )
         T9( 2, 3 ) = T( 2, 1 )*T( 2, 2 ) 
         T9( 3, 1 ) = T( 1, 2 )*T( 1, 2 )
         T9( 3, 2 ) = T( 1, 2 )*T( 2, 2 ) + T( 1, 2 )*T( 2, 2 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )     
      BTMP( 3 ) = B( 2, 2 )
*
*     Perform elimination
*
      DO 50 I = 1, 2
         XMAX = ZERO
         DO 20 IP = I, 3
            DO 10 JP = I, 3
               IF( ABS( T9( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( T9( IP, JP ) )
                  IPSV = IP
                  JPSV = JP
               END IF
   10       CONTINUE
   20    CONTINUE
         IF( IPSV.NE.I ) THEN
            CALL DSWAP( 3, T9( IPSV, 1 ), 3, T9( I, 1 ), 3 )
            TEMP = BTMP( I )
            BTMP( I ) = BTMP( IPSV )
            BTMP( IPSV ) = TEMP
         END IF
         IF( JPSV.NE.I )
     $      CALL DSWAP( 3, T9( 1, JPSV ), 1, T9( 1, I ), 1 )
         JPIV( I ) = JPSV
         IF( ABS( T9( I, I ) ).LT.SMIN ) THEN
            INFO = 1
            T9( I, I ) = SMIN
         END IF
         DO 40 J = I + 1, 3
            T9( J, I ) = T9( J, I ) / T9( I, I )
            BTMP( J ) = BTMP( J ) - T9( J, I )*BTMP( I )
            DO 30 K = I + 1, 3
               T9( J, K ) = T9( J, K ) - T9( J, I )*T9( I, K )
   30       CONTINUE
   40    CONTINUE
   50 CONTINUE
      IF( ABS( T9( 3, 3 ) ).LT.SMIN )
     $   T9( 3, 3 ) = SMIN
      SCALE = ONE
      IF( ( FOUR*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( T9( 1, 1 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( T9( 2, 2 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 3 ) ).GT.ABS( T9( 3, 3 ) ) ) THEN
         SCALE = ( ONE / FOUR ) / MAX( ABS( BTMP( 1 ) ),
     $           ABS( BTMP( 2 ) ), ABS( BTMP( 3 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
         BTMP( 3 ) = BTMP( 3 )*SCALE
      END IF
      DO 70 I = 1, 3
         K = 4 - I
         TEMP = ONE / T9( K, K )
         TMP( K ) = BTMP( K )*TEMP
         DO 60 J = K + 1, 3
            TMP( K ) = TMP( K ) - ( TEMP*T9( K, J ) )*TMP( J )
  60     CONTINUE
  70  CONTINUE
      DO 80 I = 1, 2
         IF( JPIV( 3-I ).NE.3-I ) THEN
            TEMP = TMP( 3-I )
            TMP( 3-I ) = TMP( JPIV( 3-I ) )
            TMP( JPIV( 3-I ) ) = TEMP
         END IF
  80  CONTINUE
      X( 1, 1 ) = TMP( 1 )
      X( 2, 1 ) = TMP( 2 )
      X( 1, 2 ) = TMP( 2 )
      X( 2, 2 ) = TMP( 3 )
      XNORM = MAX( ABS( TMP( 1 ) )+ABS( TMP( 2 ) ),
     $        ABS( TMP( 2 ) )+ABS( TMP( 3 ) ) )
      RETURN
*
*     End of DLALD2
*
      END
      SUBROUTINE DLALY2( LTRAN, T, LDT, B, LDB, SCALE, X, LDX, XNORM,
     $                   INFO )
*
*  -- RICCPACK auxiliary routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      LOGICAL            LTRAN
      INTEGER            INFO, LDB, LDT, LDX
      DOUBLE PRECISION   SCALE, XNORM
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), T( LDT, * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  DLALY2 solves for the 2 by 2 symmetric matrix X in
*
*         op(T')*X + X*op(T) = SCALE*B,
*
*  where T is 2 by 2, B is symmetric 2 by 2, and op(T) = T or T',
*  where T' denotes the transpose of T.
*
*  Arguments
*  =========
*
*  LTRAN   (input) LOGICAL
*          On entry, LTRAN specifies the op(T):
*             = .FALSE., op(T) = T,
*             = .TRUE., op(T) = T'.
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,2)
*          On entry, T contains an 2 by 2 matrix.
*
*  LDT     (input) INTEGER
*          The leading dimension of the matrix T. LDT >= 2.
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB,2)
*          On entry, the 2 by 2 matrix B contains the symmetric
*          right-hand side of the equation.
*
*  LDB     (input) INTEGER
*          The leading dimension of the matrix B. LDB >= 2.
*
*  SCALE   (output) DOUBLE PRECISION
*          On exit, SCALE contains the scale factor. SCALE is chosen
*          less than or equal to 1 to prevent the solution overflowing.
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,2)
*          On exit, X contains the 2 by 2 symmetric solution.
*
*  LDX     (input) INTEGER
*          The leading dimension of the matrix X. LDX >= 2.
*
*  XNORM   (output) DOUBLE PRECISION
*          On exit, XNORM is the infinity-norm of the solution.
*
*  INFO    (output) INTEGER
*          On exit, INFO is set to
*             0: successful exit.
*             1: T and -T have too close eigenvalues, so T
*                is perturbed to get a nonsingular equation.
*          NOTE: In the interests of speed, this routine does not
*                check the inputs for errors.
*
* =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      DOUBLE PRECISION   FOUR
      PARAMETER          ( FOUR = 4.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, IP, IPSV, J, JP, JPSV, K
      DOUBLE PRECISION   EPS, SMIN, SMLNUM, TEMP, XMAX
*     ..
*     .. Local Arrays ..
      INTEGER            JPIV( 3 )
      DOUBLE PRECISION   BTMP( 3 ), T9( 3, 3 ), TMP( 3 )
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DSWAP
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
*     ..
*     .. Executable Statements ..
*
*     Do not check the input parameters for errors
*
      INFO = 0
*
*     Set constants to control overflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
*
*     Solve equivalent 3 by 3 system using complete pivoting.
*     Set pivots less than SMIN to SMIN.
*
      SMIN = MAX( ABS( T( 1, 1 ) ), ABS( T( 1, 2 ) ),
     $       ABS( T( 2, 1 ) ), ABS( T( 2, 2 ) ) )
      SMIN = MAX( EPS*SMIN, SMLNUM )
      BTMP( 1 ) = ZERO
      CALL DCOPY( 9, BTMP, 0, T9, 1 )
      T9( 1, 1 ) = T( 1, 1 ) + T( 1, 1 )
      T9( 2, 2 ) = T( 1, 1 ) + T( 2, 2 )
      T9( 3, 3 ) = T( 2, 2 ) + T( 2, 2 )
      IF( LTRAN ) THEN
         T9( 1, 2 ) = T( 1, 2 ) + T( 1, 2 )
         T9( 2, 1 ) = T( 2, 1 )
         T9( 2, 3 ) = T( 1, 2 )
         T9( 3, 2 ) = T( 2, 1 ) + T( 2, 1 )
      ELSE
         T9( 1, 2 ) = T( 2, 1 ) + T( 2, 1 )
         T9( 2, 1 ) = T( 1, 2 )
         T9( 2, 3 ) = T( 2, 1 )
         T9( 3, 2 ) = T( 1, 2 ) + T( 1, 2 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )     
      BTMP( 3 ) = B( 2, 2 )
*
*     Perform elimination
*
      DO 50 I = 1, 2
         XMAX = ZERO
         DO 20 IP = I, 3
            DO 10 JP = I, 3
               IF( ABS( T9( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( T9( IP, JP ) )
                  IPSV = IP
                  JPSV = JP
               END IF
   10       CONTINUE
   20    CONTINUE
         IF( IPSV.NE.I ) THEN
            CALL DSWAP( 3, T9( IPSV, 1 ), 3, T9( I, 1 ), 3 )
            TEMP = BTMP( I )
            BTMP( I ) = BTMP( IPSV )
            BTMP( IPSV ) = TEMP
         END IF
         IF( JPSV.NE.I )
     $      CALL DSWAP( 3, T9( 1, JPSV ), 1, T9( 1, I ), 1 )
         JPIV( I ) = JPSV
         IF( ABS( T9( I, I ) ).LT.SMIN ) THEN
            INFO = 1
            T9( I, I ) = SMIN
         END IF
         DO 40 J = I + 1, 3
            T9( J, I ) = T9( J, I ) / T9( I, I )
            BTMP( J ) = BTMP( J ) - T9( J, I )*BTMP( I )
            DO 30 K = I + 1, 3
               T9( J, K ) = T9( J, K ) - T9( J, I )*T9( I, K )
   30       CONTINUE
   40    CONTINUE
   50 CONTINUE
      IF( ABS( T9( 3, 3 ) ).LT.SMIN )
     $   T9( 3, 3 ) = SMIN
      SCALE = ONE
      IF( ( FOUR*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( T9( 1, 1 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( T9( 2, 2 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 3 ) ).GT.ABS( T9( 3, 3 ) ) ) THEN
         SCALE = ( ONE / FOUR ) / MAX( ABS( BTMP( 1 ) ),
     $           ABS( BTMP( 2 ) ), ABS( BTMP( 3 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
         BTMP( 3 ) = BTMP( 3 )*SCALE
      END IF
      DO 70 I = 1, 3
         K = 4 - I
         TEMP = ONE / T9( K, K )
         TMP( K ) = BTMP( K )*TEMP
         DO 60 J = K + 1, 3
            TMP( K ) = TMP( K ) - ( TEMP*T9( K, J ) )*TMP( J )
  60     CONTINUE
  70  CONTINUE
      DO 80 I = 1, 2
         IF( JPIV( 3-I ).NE.3-I ) THEN
            TEMP = TMP( 3-I )
            TMP( 3-I ) = TMP( JPIV( 3-I ) )
            TMP( JPIV( 3-I ) ) = TEMP
         END IF
  80  CONTINUE
      X( 1, 1 ) = TMP( 1 )
      X( 2, 1 ) = TMP( 2 )
      X( 1, 2 ) = TMP( 2 )
      X( 2, 2 ) = TMP( 3 )
      XNORM = MAX( ABS( TMP( 1 ) )+ABS( TMP( 2 ) ),
     $        ABS( TMP( 2 ) )+ABS( TMP( 3 ) ) )
      RETURN
*
*     End of DLALY2
*
      END
      SUBROUTINE DLASD2( LTRANL, LTRANR, ISGN, N1, N2, TL, LDTL, TR,
     $                   LDTR, B, LDB, SCALE, X, LDX, XNORM, INFO )
*
*  -- RICCPACK auxiliary routine (version 1.0) --
*     May 10, 2000 
*
*     .. Scalar Arguments ..
      LOGICAL            LTRANL, LTRANR
      INTEGER            INFO, ISGN, LDB, LDTL, LDTR, LDX, N1, N2
      DOUBLE PRECISION   SCALE, XNORM
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), TL( LDTL, * ), TR( LDTR, * ),
     $                   X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  DLASD2 solves for the N1 by N2 matrix X, 1 <= N1,N2 <= 2, in
*
*         ISGN*op(TL)*X*op(TR) - X = SCALE*B,
*
*  where TL is N1 by N1, TR is N2 by N2, B is N1 by N2, and ISGN = 1 or
*  -1.  op(T) = T or T', where T' denotes the transpose of T.
*
*  Arguments
*  =========
*
*  LTRANL  (input) LOGICAL
*          On entry, LTRANL specifies the op(TL):
*             = .FALSE., op(TL) = TL,
*             = .TRUE., op(TL) = TL'.
*
*  LTRANR  (input) LOGICAL
*          On entry, LTRANR specifies the op(TR):
*            = .FALSE., op(TR) = TR,
*            = .TRUE., op(TR) = TR'.
*
*  ISGN    (input) INTEGER
*          On entry, ISGN specifies the sign of the equation
*          as described before. ISGN may only be 1 or -1.
*
*  N1      (input) INTEGER
*          On entry, N1 specifies the order of matrix TL.
*          N1 may only be 0, 1 or 2.
*
*  N2      (input) INTEGER
*          On entry, N2 specifies the order of matrix TR.
*          N2 may only be 0, 1 or 2.
*
*  TL      (input) DOUBLE PRECISION array, dimension (LDTL,2)
*          On entry, TL contains an N1 by N1 matrix.
*
*  LDTL    (input) INTEGER
*          The leading dimension of the matrix TL. LDTL >= max(1,N1).
*
*  TR      (input) DOUBLE PRECISION array, dimension (LDTR,2)
*          On entry, TR contains an N2 by N2 matrix.
*
*  LDTR    (input) INTEGER
*          The leading dimension of the matrix TR. LDTR >= max(1,N2).
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB,2)
*          On entry, the N1 by N2 matrix B contains the right-hand
*          side of the equation.
*
*  LDB     (input) INTEGER
*          The leading dimension of the matrix B. LDB >= max(1,N1).
*
*  SCALE   (output) DOUBLE PRECISION
*          On exit, SCALE contains the scale factor. SCALE is chosen
*          less than or equal to 1 to prevent the solution overflowing.
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,2)
*          On exit, X contains the N1 by N2 solution.
*
*  LDX     (input) INTEGER
*          The leading dimension of the matrix X. LDX >= max(1,N1).
*
*  XNORM   (output) DOUBLE PRECISION
*          On exit, XNORM is the infinity-norm of the solution.
*
*  INFO    (output) INTEGER
*          On exit, INFO is set to
*             0: successful exit.
*             1: TL and TR have almost reciprocal eigenvalues, so TL or
*                TR is perturbed to get a nonsingular equation.
*          NOTE: In the interests of speed, this routine does not
*                check the inputs for errors.
*
* ======================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      DOUBLE PRECISION   TWO, HALF, EIGHT
      PARAMETER          ( TWO = 2.0D+0, HALF = 0.5D+0, EIGHT = 8.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            BSWAP, XSWAP
      INTEGER            I, IP, IPIV, IPSV, J, JP, JPSV, K
      DOUBLE PRECISION   BET, EPS, GAM, L21, SGN, SMIN, SMLNUM, TAU1,
     $                   TEMP, U11, U12, U22, XMAX
*     ..
*     .. Local Arrays ..
      LOGICAL            BSWPIV( 4 ), XSWPIV( 4 )
      INTEGER            JPIV( 4 ), LOCL21( 4 ), LOCU12( 4 ),
     $                   LOCU22( 4 )
      DOUBLE PRECISION   BTMP( 4 ), T16( 4, 4 ), TMP( 4 ), X2( 2 )
*     ..
*     .. External Functions ..
      INTEGER            IDAMAX
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           IDAMAX, DLAMCH
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DSWAP
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
*     ..
*     .. Data statements ..
      DATA               LOCU12 / 3, 4, 1, 2 / , LOCL21 / 2, 1, 4, 3 / ,
     $                   LOCU22 / 4, 3, 2, 1 /
      DATA               XSWPIV / .FALSE., .FALSE., .TRUE., .TRUE. /
      DATA               BSWPIV / .FALSE., .TRUE., .FALSE., .TRUE. /
*     ..
*     .. Executable Statements ..
*
*     Do not check the input parameters for errors
*
      INFO = 0
*
*     Quick return if possible
*
      IF( N1.EQ.0 .OR. N2.EQ.0 )
     $   RETURN
*
*     Set constants to control overflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      SGN = ISGN
*
      K = N1 + N1 + N2 - 2
      GO TO ( 10, 20, 30, 50 )K
*
*     1 by 1: SGN*TL11*X*TR11 - X = B11
*
   10 CONTINUE
      TAU1 = SGN*TL( 1, 1 )*TR( 1, 1 ) - ONE
      BET = ABS( TAU1 )
      IF( BET.LE.SMLNUM ) THEN
         TAU1 = SMLNUM
         BET = SMLNUM
         INFO = 1
      END IF
*
      SCALE = ONE
      GAM = ABS( B( 1, 1 ) )
      IF( SMLNUM*GAM.GT.BET )
     $   SCALE = ONE / GAM
*
      X( 1, 1 ) = ( B( 1, 1 )*SCALE ) / TAU1
      XNORM = ABS( X( 1, 1 ) )
      RETURN
*
*     1 by 2:
*     ISGN*TL11*[X11 X12]*op[TR11 TR12]  = [B11 B12]
*                           [TR21 TR22]
*
   20 CONTINUE
*
      SMIN = MAX( EPS*MAX( ABS( TL( 1, 1 ) ), ABS( TR( 1, 1 ) ),
     $       ABS( TR( 1, 2 ) ), ABS( TR( 2, 1 ) ), ABS( TR( 2, 2 ) ) ),
     $       SMLNUM )
      TMP( 1 ) = SGN*TL( 1, 1 )*TR( 1, 1 ) - ONE
      TMP( 4 ) = SGN*TL( 1, 1 )*TR( 2, 2 ) - ONE
      IF( LTRANR ) THEN
         TMP( 2 ) = SGN*TL( 1, 1 )*TR( 2, 1 )
         TMP( 3 ) = SGN*TL( 1, 1 )*TR( 1, 2 )
      ELSE
         TMP( 2 ) = SGN*TL( 1, 1 )*TR( 1, 2 )
         TMP( 3 ) = SGN*TL( 1, 1 )*TR( 2, 1 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 1, 2 )
      GO TO 40
*
*     2 by 1:
*          ISGN*op[TL11 TL12]*[X11]*TR11  = [B11]
*                 [TL21 TL22] [X21]         [B21]
*
   30 CONTINUE
      SMIN = MAX( EPS*MAX( ABS( TR( 1, 1 ) ), ABS( TL( 1, 1 ) ),
     $       ABS( TL( 1, 2 ) ), ABS( TL( 2, 1 ) ), ABS( TL( 2, 2 ) ) ),
     $       SMLNUM )
      TMP( 1 ) = SGN*TL( 1, 1 )*TR( 1, 1 ) - ONE
      TMP( 4 ) = SGN*TL( 2, 2 )*TR( 1, 1 ) - ONE
      IF( LTRANL ) THEN
         TMP( 2 ) = SGN*TL( 1, 2 )*TR( 1, 1 )
         TMP( 3 ) = SGN*TL( 2, 1 )*TR( 1, 1 )
      ELSE
         TMP( 2 ) = SGN*TL( 2, 1 )*TR( 1, 1 )
         TMP( 3 ) = SGN*TL( 1, 2 )*TR( 1, 1 )
      END IF
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )
   40 CONTINUE
*
*     Solve 2 by 2 system using complete pivoting.
*     Set pivots less than SMIN to SMIN.
*
      IPIV = IDAMAX( 4, TMP, 1 )
      U11 = TMP( IPIV )
      IF( ABS( U11 ).LE.SMIN ) THEN
         INFO = 1
         U11 = SMIN
      END IF
      U12 = TMP( LOCU12( IPIV ) )
      L21 = TMP( LOCL21( IPIV ) ) / U11
      U22 = TMP( LOCU22( IPIV ) ) - U12*L21
      XSWAP = XSWPIV( IPIV )
      BSWAP = BSWPIV( IPIV )
      IF( ABS( U22 ).LE.SMIN ) THEN
         INFO = 1
         U22 = SMIN
      END IF
      IF( BSWAP ) THEN
         TEMP = BTMP( 2 )
         BTMP( 2 ) = BTMP( 1 ) - L21*TEMP
         BTMP( 1 ) = TEMP
      ELSE
         BTMP( 2 ) = BTMP( 2 ) - L21*BTMP( 1 )
      END IF
      SCALE = ONE
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
         XNORM = ABS( X( 1, 1 ) ) + ABS( X( 1, 2 ) )
      ELSE
         X( 2, 1 ) = X2( 2 )
         XNORM = MAX( ABS( X( 1, 1 ) ), ABS( X( 2, 1 ) ) )
      END IF
      RETURN
*
*     2 by 2:
*     ISGN*op[TL11 TL12]*[X11 X12]*op[TR11 TR12]-[X11 X12] = [B11 B12]
*            [TL21 TL22] [X21 X22]   [TR21 TR22] [X21 X22]   [B21 B22]
*
*     Solve equivalent 4 by 4 system using complete pivoting.
*     Set pivots less than SMIN to SMIN.
*
   50 CONTINUE
      SMIN = MAX( ABS( TR( 1, 1 ) ), ABS( TR( 1, 2 ) ),
     $       ABS( TR( 2, 1 ) ), ABS( TR( 2, 2 ) ) )
      SMIN = MAX( SMIN, ABS( TL( 1, 1 ) ), ABS( TL( 1, 2 ) ),
     $       ABS( TL( 2, 1 ) ), ABS( TL( 2, 2 ) ) )
      SMIN = MAX( EPS*SMIN, SMLNUM )
      BTMP( 1 ) = ZERO
      CALL DCOPY( 16, BTMP, 0, T16, 1 )
      T16( 1, 1 ) = SGN*TL( 1, 1 )*TR( 1, 1 ) - ONE
      T16( 2, 2 ) = SGN*TL( 2, 2 )*TR( 1, 1 ) - ONE
      T16( 3, 3 ) = SGN*TL( 1, 1 )*TR( 2, 2 ) - ONE
      T16( 4, 4 ) = SGN*TL( 2, 2 )*TR( 2, 2 ) - ONE
      IF( LTRANL ) THEN
         T16( 1, 2 ) = SGN*TL( 2, 1 )*TR( 1, 1 )
         T16( 2, 1 ) = SGN*TL( 1, 2 )*TR( 1, 1 )
         T16( 3, 4 ) = SGN*TL( 2, 1 )*TR( 2, 2 )
         T16( 4, 3 ) = SGN*TL( 1, 2 )*TR( 2, 2 )
      ELSE
         T16( 1, 2 ) = SGN*TL( 1, 2 )*TR( 1, 1 )
         T16( 2, 1 ) = SGN*TL( 2, 1 )*TR( 1, 1 )
         T16( 3, 4 ) = SGN*TL( 1, 2 )*TR( 2, 2 )
         T16( 4, 3 ) = SGN*TL( 2, 1 )*TR( 2, 2 )
      END IF
      IF( LTRANR ) THEN
         T16( 1, 3 ) = SGN*TL( 1, 1 )*TR( 1, 2 )
         T16( 2, 4 ) = SGN*TL( 2, 2 )*TR( 1, 2 )
         T16( 3, 1 ) = SGN*TL( 1, 1 )*TR( 2, 1 )
         T16( 4, 2 ) = SGN*TL( 2, 2 )*TR( 2, 1 )
      ELSE
         T16( 1, 3 ) = SGN*TL( 1, 1 )*TR( 2, 1 )
         T16( 2, 4 ) = SGN*TL( 2, 2 )*TR( 2, 1 )
         T16( 3, 1 ) = SGN*TL( 1, 1 )*TR( 1, 2 )
         T16( 4, 2 ) = SGN*TL( 2, 2 )*TR( 1, 2 )
      END IF
      IF( LTRANL .AND. LTRANR ) THEN
         T16( 1, 4 ) = SGN*TL( 2, 1 )*TR( 1, 2 )
         T16( 2, 3 ) = SGN*TL( 1, 2 )*TR( 1, 2 )
         T16( 3, 2 ) = SGN*TL( 2, 1 )*TR( 2, 1 )
         T16( 4, 1 ) = SGN*TL( 1, 2 )*TR( 2, 1 )
      END IF
      IF( LTRANL .AND. .NOT.LTRANR ) THEN
         T16( 1, 4 ) = SGN*TL( 2, 1 )*TR( 2, 1 )
         T16( 2, 3 ) = SGN*TL( 1, 2 )*TR( 2, 1 )
         T16( 3, 2 ) = SGN*TL( 2, 1 )*TR( 1, 2 )
         T16( 4, 1 ) = SGN*TL( 1, 2 )*TR( 1, 2 )
      END IF
      IF( .NOT.LTRANL .AND. LTRANR ) THEN
          T16( 1, 4 ) = SGN*TL( 1, 2 )*TR( 1, 2 )
          T16( 2, 3 ) = SGN*TL( 2, 1 )*TR( 1, 2 )
          T16( 3, 2 ) = SGN*TL( 1, 2 )*TR( 2, 1 )
          T16( 4, 1 ) = SGN*TL( 2, 1 )*TR( 2, 1 )
      END IF
      IF( .NOT.LTRANL .AND. .NOT.LTRANR ) THEN
          T16( 1, 4 ) = SGN*TL( 1, 2 )*TR( 2, 1 )
          T16( 2, 3 ) = SGN*TL( 2, 1 )*TR( 2, 1 )
          T16( 3, 2 ) = SGN*TL( 1, 2 )*TR( 1, 2 )
          T16( 4, 1 ) = SGN*TL( 2, 1 )*TR( 1, 2 ) 
      END IF        
      BTMP( 1 ) = B( 1, 1 )
      BTMP( 2 ) = B( 2, 1 )
      BTMP( 3 ) = B( 1, 2 )
      BTMP( 4 ) = B( 2, 2 )
*
*     Perform elimination
*
      DO 100 I = 1, 3
         XMAX = ZERO
         DO 70 IP = I, 4
            DO 60 JP = I, 4
               IF( ABS( T16( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( T16( IP, JP ) )
                  IPSV = IP
                  JPSV = JP
               END IF
   60       CONTINUE
   70    CONTINUE
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
         DO 90 J = I + 1, 4
            T16( J, I ) = T16( J, I ) / T16( I, I )
            BTMP( J ) = BTMP( J ) - T16( J, I )*BTMP( I )
            DO 80 K = I + 1, 4
               T16( J, K ) = T16( J, K ) - T16( J, I )*T16( I, K )
   80       CONTINUE
   90    CONTINUE
  100 CONTINUE
      IF( ABS( T16( 4, 4 ) ).LT.SMIN )
     $   T16( 4, 4 ) = SMIN
      SCALE = ONE
      IF( ( EIGHT*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( T16( 1, 1 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( T16( 2, 2 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 3 ) ).GT.ABS( T16( 3, 3 ) ) .OR.
     $    ( EIGHT*SMLNUM )*ABS( BTMP( 4 ) ).GT.ABS( T16( 4, 4 ) ) ) THEN
         SCALE = ( ONE / EIGHT ) / MAX( ABS( BTMP( 1 ) ),
     $           ABS( BTMP( 2 ) ), ABS( BTMP( 3 ) ), ABS( BTMP( 4 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
         BTMP( 3 ) = BTMP( 3 )*SCALE
         BTMP( 4 ) = BTMP( 4 )*SCALE
      END IF
      DO 120 I = 1, 4
         K = 5 - I
         TEMP = ONE / T16( K, K )
         TMP( K ) = BTMP( K )*TEMP
         DO 110 J = K + 1, 4
            TMP( K ) = TMP( K ) - ( TEMP*T16( K, J ) )*TMP( J )
  110    CONTINUE
  120 CONTINUE
      DO 130 I = 1, 3
         IF( JPIV( 4-I ).NE.4-I ) THEN
            TEMP = TMP( 4-I )
            TMP( 4-I ) = TMP( JPIV( 4-I ) )
            TMP( JPIV( 4-I ) ) = TEMP
         END IF
  130 CONTINUE
      X( 1, 1 ) = TMP( 1 )
      X( 2, 1 ) = TMP( 2 )
      X( 1, 2 ) = TMP( 3 )
      X( 2, 2 ) = TMP( 4 )
      XNORM = MAX( ABS( TMP( 1 ) )+ABS( TMP( 3 ) ),
     $        ABS( TMP( 2 ) )+ABS( TMP( 4 ) ) )
      RETURN
*
*     End of DLASD2
*
      END
      SUBROUTINE LYPCFR( TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU,
     $                   X, LDX, SCALE, FERR,  WORK, LWORK, IWORK,
     $                   INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),
     $                   U( LDU, * ), WORK( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPCFR estimates the forward error bound for the computed solution of
*  the matrix Lyapunov equation
*
*         transpose(op(A))*X + X*op(A) = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is a
*  scale factor, set <= 1 during the solution of the equation to avoid
*  overflow in X. If the equation is not scaled, scale should be set
*  equal to 1.
*
*  Arguments
*  =========
*   
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N) 
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal matrix U from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N) 
*          The N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (input) DOUBLE PRECISION
*          The scale factor, scale.
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 6*N*N. 
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
*  Further Details
*  ===============
*
*  The forward error bound is estimated using the practical error bound
*  proposed in [1].
*
*  References
*  ==========
*
*  [1] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, THREE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0, 
     $                     THREE = 3.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          TRANAT 
      INTEGER            I, IABS, IDLC, IJ, INFO2, IRES, ITMP, IXBS, J, 
     $                   KASE, MINWRK
      DOUBLE PRECISION   EPS, EST, SCALE2, XNORM
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEMM, DLACON, DLACPY, DSYMM, DSYR2K, LYPCTR,
     $                   XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -13
      END IF
*
*     Get the machine precision
*
      EPS = DLAMCH( 'Epsilon' )
*
*     Compute workspace
*
      MINWRK = 6*N*N
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPCFR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 )
     $   RETURN
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
*
*        Matrix all zero
*
         FERR = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      IDLC = N*N 
      ITMP = IDLC + N*N
      IABS = ITMP + N*N
      IXBS = IABS + N*N
      IRES = IXBS + N*N
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
*
*     Form residual matrix R = C - op(A')*X - X*op(A)
*
      CALL DLACPY( UPLO, N, N, C, LDC, WORK( IRES+1 ), N )
      CALL DSYR2K( UPLO, TRANAT, N, N, -ONE, A, LDA, X, LDX, SCALE, 
     $             WORK( IRES+1 ), N )
*
*     Add to abs(R) a term that takes account of rounding errors in 
*     forming R: 
*       abs(R) := abs(R) + EPS*(3*abs(C) + (n+3)*(abs(op(A'))*abs(X) +
*                        abs(X)*abs(op(A))))
*     where EPS is the machine precision
*
      DO 20 J = 1, N      
         DO 10 I = 1, N
            WORK( IABS+I+(J-1)*N ) = ABS( A( I, J ) ) 
            WORK( IXBS+I+(J-1)*N ) = ABS( X( I, J ) )
   10    CONTINUE         
   20 CONTINUE
      CALL DSYR2K( UPLO, TRANAT, N, N, ONE, WORK( IABS+1 ), N,
     $             WORK( IXBS+1 ), N, ZERO, WORK( ITMP+1 ), N )
      IF( LOWER ) THEN
         DO 40 J = 1, N
            DO 30 I = J, N
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               THREE*EPS*SCALE*ABS( C( I, J ) ) + DBLE( N+3 )*
     $               EPS*WORK( ITMP+I+(J-1)*N ) 
   30       CONTINUE
   40    CONTINUE
      ELSE 
         DO 60 J = 1, N
            DO 50 I = 1, J
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               THREE*EPS*SCALE*ABS( C( I, J ) ) + DBLE( N+3 )*
     $               EPS*WORK( ITMP+I+(J-1)*N ) 
   50       CONTINUE
   60    CONTINUE
      END IF  
*
*     Compute forward error bound, using matrix norm estimator
*
      EST = ZERO
      KASE = 0
   70 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
         IJ = 0
         IF( LOWER ) THEN
            DO 90 J = 1, N
               DO 80 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
                  END IF
   80          CONTINUE
   90       CONTINUE
         ELSE
            DO 110 J = 1, N
               DO 100 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the upper triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )   
                  END IF   
  100          CONTINUE
  110       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
         IF( KASE.EQ.2 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale2*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE2, 
     $                   INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale2*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE2,
     $                   INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         IJ = 0
         IF( LOWER ) THEN
            DO 130 J = 1, N
               DO 120 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  120          CONTINUE
  130       CONTINUE
         ELSE
            DO 150 J = 1, N
               DO 140 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the upper triangular part of symmetric   
*                    matrix 
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF   
  140          CONTINUE
  150       CONTINUE
         END IF
         GO TO 70
      END IF
*
*     Compute the estimate of the forward error 
*
      FERR =  TWO*EST / 
     $        DLANSY( 'Max', UPLO, N, X, LDX, WORK ) / SCALE2
      IF( FERR.GT.ONE ) FERR = ONE 
*       
      RETURN
*
*     End of LYPCFR
*
      END
      SUBROUTINE LYPCRC( FACT, TRANA, N, A, LDA, UPLO, C, LDC, T, LDT,
     $                   U, LDU, X, LDX, SCALE, RCOND, WORK, LWORK,
     $                   IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          FACT, TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   RCOND, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),
     $                   U( LDU, * ), WORK( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPCRC estimates the reciprocal of the condition number of the matrix
*  Lyapunov equation
*
*         transpose(op(A))*X + X*op(A) = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is a
*  scale factor, set <= 1 during the solution of the equation to avoid
*  overflow in X. If the equation is not scaled, scale should be set
*  equal to 1.
*
*  Arguments
*  =========
*
*  FACT    (input) CHARACTER*1
*          Specifies whether or not the real Schur factorization
*          of the matrix A is supplied on entry:
*          = 'F':   On entry, T and U contain the factors from the
*                     real Schur factorization of the matrix A.
*          = 'N':   The Schur factorization of A will be computed
*                     and the factors will be stored in T and U.
*    
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N) 
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input or output) DOUBLE PRECISION array, dimension (LDT,N)
*          If FACT = 'F', then T is an input argument and on entry
*          contains the upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*          If FACT = 'N', then T is an output argument and on exit  
*          returns the upper quasi-triangular matrix in Schur
*          canonical form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input or output) DOUBLE PRECISION array, dimension (LDU,N)
*          If FACT = 'F', then U is an input argument and on entry
*          contains the orthogonal matrix U from the real Schur
*          factorization of A.
*          If FACT = 'N', then U is an output argument and on exit
*          returns the orthogonal N-by-N matrix from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N) 
*          The N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (input) DOUBLE PRECISION
*          The scale factor, scale. 
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number
*          of the Lyapunov equation. 
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 3*N*N + 2*N + max(1,3*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix A cannot be reduced to Schur canonical form
*
*  Further Details
*  ===============
*
*  The condition number of the Lyapunov equation is estimated as
*
*  cond = (norm(Theta)*norm(A) + norm(inv(Omega))*norm(C))/norm(X)
*
*  where Omega and Theta are linear operators defined by
*
*  Omega(Z) = transpose(op(A))*Z + Z*op(A),
*  Theta(Z) = inv(Omega(transpose(op(Z))*X + X*op(Z))).
*
*  The program estimates the quantities
*
*  sep(op(A),-transpose(op(A)) = 1 / norm(inv(Omega))
*
*  and norm(Theta) using 1-norm condition estimator.
*
*  References
*  ==========
*
*  [1] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOFACT, NOTRNA, VOIDDUMMY
      CHARACTER          TRANAT 
      INTEGER            I, IDLC, IJ, INFO2, ITMP, IWI, IWR, IWRK, J,
     $                   KASE, LWA, MINWRK, SDIM
      DOUBLE PRECISION   ANORM, CNORM, EST, SCALE2, SEP, THNORM, XNORM
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANGE, DLANSY
      EXTERNAL           DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DLACON, DLACPY, DSYMM, DSYR2K, 
     $                   LYPCTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOFACT = LSAME( FACT, 'N' )
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -6 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -14
      END IF
*
*     Compute workspace
*
      MINWRK = 3*N*N + 2*N + MAX( 1, 3*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -18 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPCRC', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 )
     $   RETURN
*      
*     Compute the norms of the matrices A, C and X
*
      ANORM = DLANGE( '1', N, N, A, LDA, WORK )
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
*
*        Matrix all zero
*
         RCOND = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      LWA = 3*N*N + 2*N
      IDLC = N*N 
      ITMP = IDLC + N*N
      IWR = ITMP + N*N
      IWI = IWR + N
      IWRK = IWI + N
*
      IF( NOFACT ) THEN
* 
*        Compute the Schur factorization of A
*
         CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
         CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $        WORK( IWR+1 ),
     $        WORK( IWI+1 ), U, LDU, WORK( IWRK+1 ), LWORK-IWRK,
     $        BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         LWA = LWA + INT( WORK( IWRK+1 ) )
      END IF
*
*     Estimate sep(op(A),-transpose(op(A))) 
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
* 
      EST = ZERO
      KASE = 0
   10 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 30 J = 1, N
               DO 20 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   20          CONTINUE
   30       CONTINUE
         ELSE
            DO 50 J = 1, N
               DO 40 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   40          CONTINUE
   50       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale2*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE2, 
     $                   INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale2*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE2, 
     $                   INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix 
*
         IJ = 0
         IF( LOWER ) THEN
            DO 70 J = 1, N
               DO 60 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   60          CONTINUE
   70       CONTINUE
         ELSE
            DO 90 J = 1, N
               DO 80 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   80          CONTINUE
   90       CONTINUE
         END IF
         GO TO 10
      END IF
*
      SEP = SCALE2 / TWO / EST
*
*     Return if the equation is singular
*
      IF( SEP.EQ.ZERO ) THEN 
         RCOND = ZERO
         RETURN
      END IF
*
*     Estimate norm(Theta)
*
      EST = ZERO
      KASE = 0
  100 CONTINUE
      CALL DLACON( N*N, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Compute RHS = op(W')*X + X*op(W)
*
         CALL DSYR2K( UPLO, TRANAT, N, N, ONE, WORK, N, X, LDX, ZERO,  
     $                WORK( ITMP+1 ), N ) 
         CALL DLACPY( UPLO, N, N, WORK( ITMP+1 ), N, WORK, N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK( ITMP+1 ),
     $               N, ZERO, WORK, N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale2*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK, N, SCALE2, INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale2*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK, N, SCALE2, INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK( ITMP+1 ), N, U,
     $               LDU, ZERO, WORK, N )
         GO TO 100
      END IF
*
      THNORM = EST / SCALE2
*
*     Estimate the reciprocal condition number
*
      RCOND = SEP*XNORM / ( CNORM*SCALE + SEP*( THNORM*ANORM ) )
      IF( RCOND.GT.ONE ) RCOND = ONE
*       
      WORK( 1 ) = DBLE( LWA )
      RETURN
*
*     End of LYPCRC
*
      END
      SUBROUTINE LYPCSL( FACT, TRANA, N, A, LDA, UPLO, C, LDC, T, LDT,
     $                   U, LDU, WR, WI, X, LDX, SCALE, RCOND, FERR,
     $                   WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          FACT, TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),
     $                   U( LDU, * ), WI( * ), WORK( * ), WR( * ), 
     $                   X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPCSL solves the matrix Lyapunov equation
*
*         transpose(op(A))*X + X*op(A) = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is an
*  output scale factor, set <= 1 to avoid overflow in X.
*
*  Error bound on the solution and condition estimate are also provided.
*
*  Arguments
*  =========
*
*  FACT    (input) CHARACTER*1
*          Specifies whether or not the real Schur factorization
*          of the matrix A is supplied on entry:
*          = 'F':   On entry, T and U contain the factors from the
*                     real Schur factorization of the matrix A.
*          = 'N':   The Schur factorization of A will be computed
*                     and the factors will be stored in T and U.
*    
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N) 
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input or output) DOUBLE PRECISION array, dimension (LDT,N)
*          If FACT = 'F', then T is an input argument and on entry
*          contains the upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*          If FACT = 'N', then T is an output argument and on exit  
*          returns the upper quasi-triangular matrix in Schur
*          canonical form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input or output) DOUBLE PRECISION array, dimension (LDU,N)
*          If FACT = 'F', then U is an input argument and on entry
*          contains the orthogonal matrix U from the real Schur
*          factorization of A.
*          If FACT = 'N', then U is an output argument and on exit
*          returns the orthogonal N-by-N matrix from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if FACT = 'N', WR(1:N) and WI(1:N) contain the
*          real and imaginary parts, respectively, of the eigenvalues
*          of A.
*          If FACT = 'F', WR and WI are not referenced. 
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N) 
*          If INFO = 0, the N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (output) DOUBLE PRECISION
*          The scale factor, scale, set <= 1 to avoid overflow in X.
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number
*          of the Lyapunov equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 6*N*N + max(1,3*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix A cannot be reduced to Schur canonical form
*          = 2: A and -transpose(A) have common or very close
*               eigenvalues; perturbed values were used to solve the
*               equation (but the matrix A is unchanged).
*
*  Further Details
*  ===============
*
*  The matrix Lyapunov equation is solved by the Bartels-Stewart
*  algorithm [1].
*
*  The condition number of the equation is estimated using 1-norm
*  condition estimator.
*
*  The forward error bound is estimated using the practical error bound
*  proposed in [2].
*
*  References
*  ==========
*
*  [1] R.H. Bartels and G.W. Stewart. Algorithm 432: Solution of the
*      matrix equation AX + XB = C. Comm. ACM, vol. 15, pp. 820-826,
*      1972.
*  [2] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, 1993, pp. 124-136. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOFACT, NOTRNA, VOIDDUMMY
      INTEGER            INFO2, LWA, LWAMAX, MINWRK, SDIM
      DOUBLE PRECISION   CNORM
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANSY
      EXTERNAL           DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DLACPY, DLASET, DSYMM, LYPCFR,
     $                   LYPCRC, LYPCTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOFACT = LSAME( FACT, 'N' )
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -6 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -16
      END IF
*
*     Compute workspace
*
      MINWRK = 6*N*N + MAX( 1, 3*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -21 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPCSL', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 )
     $   RETURN
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      IF( CNORM.EQ.ZERO ) THEN
*
*        Matrix all zero. Return zero solution
*
         CALL DLASET( 'F', N, N, ZERO, ZERO, X, LDX )
         SCALE = ONE
         RCOND = ZERO
         FERR = ZERO
         RETURN
      END IF
*
      LWA = 0
*
      IF( NOFACT ) THEN
* 
*        Compute the Schur factorization of A
*
         CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
         CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $        WR, WI, 
     $        U, LDU,
     $        WORK, LWORK, BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         LWA = INT( WORK( 1 ) )
      END IF
      LWAMAX = LWA
*
*     Transform the right-hand side: C := U'*C*U.
*     Form TEMP = C*U then X = U'*TEMP
*
      CALL DSYMM( 'L', UPLO, N, N, ONE, C, LDC, U, LDU, ZERO, WORK, N )
      CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO, X,
     $            LDX )
*
*     Solve the quasi-triangular Lyapunov equation.
*     The answer overwrites the right-hand side
*
      CALL LYPCTR( TRANA, N, T, LDT, X, LDX, SCALE, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
      END IF
*
*     Transform back to obtain the solution: X := U*X*U'.
*     Form TEMP = U*X then X = TEMP*U'
*
      CALL DSYMM( 'R', UPLO, N, N, ONE, X, LDX, U, LDU, ZERO, WORK, N )
      CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO, X, 
     $            LDX )
*
*     Estimate the reciprocal of the condition number 
*
      CALL LYPCRC( 'F', TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU,
     $             X, LDX, SCALE, RCOND, WORK, LWORK, IWORK, INFO2 )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN 
         FERR = ONE
         RETURN
      END IF
      LWA = INT( WORK( 1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Estimate the bound on the forward error
*
      CALL LYPCFR( TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU,
     $             X, LDX, SCALE, FERR, WORK, LWORK, IWORK, INFO2 )
      LWA = 6*N*N
      LWAMAX = MAX( LWA, LWAMAX )
*       
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of LYPCSL
*
      END
      SUBROUTINE LYPCTR( TRANA, N, A, LDA, C, LDC, SCALE, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000 
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA
      INTEGER            INFO, LDA, LDC, N
      DOUBLE PRECISION   SCALE
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPCTR solves the matrix Lyapunov equation
*
*         transpose(op(A))*X + X*op(A) = scale*C 
*
*  where op(A) = A or A**T,  A is upper quasi-triangular and C is 
*  symmetric (C = C**T). A is N-by-N, the right hand side C and the
*  solution X are N-by-N, and scale is an output scale factor, 
*  set <= 1 to avoid overflow in X.
*
*  A must be in Schur canonical form (as returned by DHSEQR), that is,
*  block upper triangular with 1-by-1 and 2-by-2 diagonal blocks;
*  each 2-by-2 diagonal block has its diagonal elements equal and its
*  off-diagonal elements of opposite sign.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**H (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The upper quasi-triangular matrix A, in Schur canonical form.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
*          On entry, the symmetric N-by-N right hand side matrix C.
*          On exit, C is overwritten by the solution matrix X.
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  SCALE   (output) DOUBLE PRECISION
*          The scale factor, scale, set <= 1 to avoid overflow in X.
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: A and -A have common or very close eigenvalues;
*                perturbed values were used to solve the equation
*                (but the matrix A is unchanged).
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            NOTRNA
      INTEGER            IERR, J, K, K1, K2, KNEXT, L, L1, L2, LNEXT
      DOUBLE PRECISION   A11, BIGNUM, DA11, DB, EPS, SCALOC, SMIN,
     $                   SMLNUM, SUML, SUMR, XNORM
*     ..
*     .. Local Arrays ..
      DOUBLE PRECISION   DUM( 1 ), VEC( 2, 2 ), X( 2, 2 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DDOT, DLAMCH, DLANGE
      EXTERNAL           LSAME, DDOT, DLAMCH, DLANGE
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLABAD, DLALN2, DLALY2, DLASY2, DSCAL, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX, MIN
*     ..
*     .. Executable Statements ..
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -6
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPCTR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 )
     $   RETURN
*
*     Set constants to control overflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
      SMLNUM = SMLNUM*DBLE( N*N ) / EPS
      BIGNUM = ONE / SMLNUM
*
      SMIN = MAX( SMLNUM, EPS*DLANGE( 'M', N, N, A, LDA, DUM ) )
*
      SCALE = ONE
*
      IF( NOTRNA ) THEN
*
*        Solve    A'*X + X*A = scale*C.
*
*        The (K,L)th block of X is determined starting from
*        upper-left corner column by column by
*
*          A(K,K)'*X(K,L) + X(K,L)*A(L,L) = C(K,L) - R(K,L)
*
*        Where
*                   K-1                   L-1
*          R(K,L) = SUM [A(I,K)'*X(I,L)] +SUM [X(K,J)*A(J,L)]
*                   I=1                   J=1
*
*        Start column loop (index = L)
*        L1 (L2): column index of the first (last) row of X(K,L)
*
         LNEXT = 1
         DO 60 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 60
            IF( L.EQ.N ) THEN
               L1 = L
               L2 = L
            ELSE
               IF( A( L+1, L ).NE.ZERO ) THEN
                  L1 = L
                  L2 = L + 1
                  LNEXT = L + 2
               ELSE
                  L1 = L
                  L2 = L
                  LNEXT = L + 1
               END IF
            END IF
*
*           Start row loop (index = K)
*           K1 (K2): row index of the first (last) row of X(K,L)
*
            KNEXT = L
            DO 50 K = L, N
               IF( K.LT.KNEXT )
     $            GO TO 50
               IF( K.EQ.N ) THEN
                  K1 = K
                  K2 = K
               ELSE
                  IF( A( K+1, K ).NE.ZERO ) THEN
                     K1 = K
                     K2 = K + 1
                     KNEXT = K + 2
                  ELSE
                     K1 = K
                     K2 = K
                     KNEXT = K + 1
                  END IF
               END IF
*
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
                  SCALOC = ONE
*
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
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 10 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   10                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF  
*
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
*
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  CALL DLALN2( .TRUE., 2, 1, SMIN, ONE, A( K1, K1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( L1, L1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 20 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   20                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 )  
*
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
*
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L2 ), 1 )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUML+SUMR )

*
                  CALL DLALN2( .TRUE., 2, 1, SMIN, ONE, A( L1, L1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( K1, K1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 30 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   30                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 2, 1 )
*
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
*
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  SUMR = DDOT( L1-1, C( K1, 1 ), LDC, A( 1, L2 ), 1 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  SUMR = DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L1 ), 1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L2 ), 1 )
                  SUMR = DDOT( L1-1, C( K2, 1 ), LDC, A( 1, L2 ), 1 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUML+SUMR )
*
                  IF( K1.EQ.L1 ) THEN
                     CALL DLALY2( .FALSE., A( K1, K1 ), LDA, VEC, 2,
     $                            SCALOC, X, 2, XNORM, IERR )
                  ELSE    
                     CALL DLASY2( .TRUE., .FALSE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 40 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   40                CONTINUE
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
*
   50       CONTINUE
   60    CONTINUE
*
      ELSE
*
*        Solve    A*X + X*A' = scale*C.
*
*        The (K,L)th block of X is determined starting from
*        bottom-right corner column by column by
*
*            A(K,K)*X(K,L) + X(K,L)*A(L,L)' = C(K,L) - R(K,L)
*
*        Where
*                      N                     N
*            R(K,L) = SUM [A(K,I)*X(I,L)] + SUM [X(K,J)*A(L,J)'].
*                    I=K+1                 J=L+1
*
*        Start column loop (index = L)
*        L1 (L2): column index of the first (last) row of X(K,L)
*
         LNEXT = N
         DO 120 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 120
            IF( L.EQ.1 ) THEN
               L1 = L
               L2 = L
            ELSE
               IF( A( L, L-1 ).NE.ZERO ) THEN
                  L1 = L - 1
                  L2 = L
                  LNEXT = L - 2
               ELSE
                  L1 = L
                  L2 = L
                  LNEXT = L - 1
               END IF
            END IF
*
*           Start row loop (index = K)
*           K1 (K2): row index of the first (last) row of X(K,L)
*
            KNEXT = L
            DO 110 K = L, 1, -1
               IF( K.GT.KNEXT )
     $            GO TO 110
               IF( K.EQ.1 ) THEN
                  K1 = K
                  K2 = K
               ELSE
                  IF( A( K, K-1 ).NE.ZERO ) THEN
                     K1 = K - 1
                     K2 = K
                     KNEXT = K - 2
                  ELSE
                     K1 = K
                     K2 = K
                     KNEXT = K - 1
                  END IF
               END IF
*
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  SUML = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                   C( MIN( K1+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L1, C( K1, MIN( L1+1, N ) ), LDC,
     $                   A( L1, MIN( L1+1, N ) ), LDA )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
                  SCALOC = ONE
*
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
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 70 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   70                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF   
*
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
*
                  SUML = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L2, C( K1, MIN( L2+1, N ) ), LDC,
     $                   A( L1, MIN( L2+1, N ) ), LDA )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L2, C( K2, MIN( L2+1, N ) ), LDC,
     $                   A( L1, MIN( L2+1, N ) ), LDA )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  CALL DLALN2( .FALSE., 2, 1, SMIN, ONE, A( K1, K1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( L1, L1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 80 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   80                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 ) 
*
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
*
                  SUML = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                   C( MIN( K1+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L2, C( K1, MIN( L2+1, N ) ), LDC,
     $                   A( L1, MIN( L2+1, N ) ), LDA )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR ) 
*
                  SUML = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                   C( MIN( K1+1, N ), L2 ), 1 )
                  SUMR = DDOT( N-L2, C( K1, MIN( L2+1, N ) ), LDC,
     $                   A( L2, MIN( L2+1, N ) ), LDA )
                  VEC( 2, 1 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  CALL DLALN2( .FALSE., 2, 1, SMIN, ONE, A( L1, L1 ),
     $                         LDA, ONE, ONE, VEC, 2, -A( K1, K1 ),
     $                         ZERO, X, 2, SCALOC, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 90 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   90                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 2, 1 )
*
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
*
                  SUML = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L2, C( K1, MIN( L2+1, N ) ), LDC,
     $                   A( L1, MIN( L2+1, N ) ), LDA )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L2 ), 1 )
                  SUMR = DDOT( N-L2, C( K1, MIN( L2+1, N ) ), LDC,
     $                   A( L2, MIN( L2+1, N ) ), LDA )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L1 ), 1 )
                  SUMR = DDOT( N-L2, C( K2, MIN( L2+1, N ) ), LDC,
     $                   A( L1, MIN( L2+1, N ) ), LDA )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                   C( MIN( K2+1, N ), L2 ), 1 )
                  SUMR = DDOT( N-L2, C( K2, MIN( L2+1, N ) ), LDC,
     $                   A( L2, MIN( L2+1, N ) ), LDA )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUML+SUMR )
*
                  IF( K1.EQ.L1 ) THEN
                     CALL DLALY2( .TRUE., A( K1, K1 ), LDA, VEC, 2,
     $                            SCALOC, X, 2, XNORM, IERR )
                  ELSE  
                     CALL DLASY2( .FALSE., .TRUE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 100 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
  100                CONTINUE
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
*
  110       CONTINUE
  120    CONTINUE
*
      END IF
*
      RETURN
*
*     End of LYPCTR
*
      END
      SUBROUTINE LYPDFR( TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU, 
     $                   X, LDX, SCALE, FERR, WORK, LWORK, IWORK,
     $                   INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),  
     $                   U( LDU, * ), WORK( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPDFR estimates the forward error bound for the computed solution of
*  the discrete-time matrix Lyapunov equation
*
*         transpose(op(A))*X*op(A) - X = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is
*  scale factor, set <= 1 during the solution of the equation to avoid
*  overflow in X. If the equation is not scaled, scale should be set
*  equal to 1.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal matrix U from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (input) DOUBLE PRECISION
*          The scale factor, scale.
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE)  divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 7*N*N + 2*N. 
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
*  Further Details
*  ===============
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [1].
*
*  References
*  ==========
*
*  [1] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, THREE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0, 
     $                     THREE = 3.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          TRANAT 
      INTEGER            I, IABS, IDLC, IJ, INFO2, IRES, ITMP, IWRK,
     $                   IXBS, IXMA, J, KASE, MINWRK
      DOUBLE PRECISION   EPS, EST, SCALE2, XNORM 
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEMM, DLACON, DSYMM, LYPDTR, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -13
      END IF
*
*     Get the machine precision
*
      EPS = DLAMCH( 'Epsilon' )
*
*     Compute workspace
*
      MINWRK = 7*N*N + 2*N
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPDFR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $    RETURN
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
*
*        Matrix all zero
*
         FERR = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      IDLC = N*N
      ITMP = IDLC + N*N
      IXMA = ITMP + N*N
      IABS = IXMA + N*N
      IXBS = IABS + N*N
      IRES = IXBS + N*N
      IWRK = IRES + N*N
*
*     Compute X*op(A)
*
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, X, LDX, A, LDA, ZERO,
     $            WORK( IXMA+1 ), N )
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
*
*     Form residual matrix R = C + X - op(A')*X*op(A)
*
      CALL DGEMM( TRANAT, 'N', N, N, N, ONE, A, LDA, WORK( IXMA+1 ), N,
     $            ZERO, WORK( ITMP+1 ), N )
      IF( LOWER ) THEN
         DO 20 J = 1, N
            DO 10 I = J, N
               WORK( IRES+I+(J-1)*N ) = SCALE*C( I, J ) + X( I, J ) -
     $                      WORK( ITMP+I+(J-1)*N )
   10       CONTINUE
   20    CONTINUE
      ELSE
         DO 40 J = 1, N
            DO 30 I = 1, J
               WORK( IRES+I+(J-1)*N ) = SCALE*C( I, J ) + X( I, J ) -
     $                      WORK( ITMP+I+(J-1)*N )                
   30       CONTINUE
   40    CONTINUE
      END IF
*
*     Add to abs(R) a term that takes account of rounding errors in 
*     forming R: 
*       abs(R) := abs(R) + EPS*(3*abs(C) + 3*abs(X) + 
*                 2*(n+1)*abs(op(A'))*abs(X)*abs(op(A)))
*     where EPS is the machine precision
*
      DO 60 J = 1, N      
         DO 50 I = 1, N
            WORK( IABS+I+(J-1)*N ) = ABS( A( I, J ) ) 
            WORK( IXBS+I+(J-1)*N ) = ABS( X( I, J ) )
   50    CONTINUE         
   60 CONTINUE
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, WORK( IXBS+1 ), N, 
     $            WORK( IABS+1 ), N, ZERO, WORK( IXMA+1 ), N )
      CALL DGEMM( TRANAT, 'N', N, N, N, ONE, WORK( IABS+1 ), N,
     $            WORK( IXMA+1 ), N, ZERO, WORK( ITMP+1 ), N )
      IF( LOWER ) THEN
         DO 80 J = 1, N
            DO 70 I = J, N
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               THREE*EPS*( SCALE*ABS( C( I, J ) ) + 
     $               ABS( X( I, J ) ) ) + DBLE( 2*N + 2 )*EPS* 
     $               WORK( ITMP+I+(J-1)*N  )
   70       CONTINUE
   80    CONTINUE
      ELSE
         DO 100 J = 1, N
            DO 90 I = 1, J
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               THREE*EPS*( SCALE*ABS( C( I, J ) ) + 
     $               ABS( X( I, J ) ) ) + DBLE( 2*N + 2 )*EPS* 
     $               WORK( ITMP+I+(J-1)*N  )
   90       CONTINUE
  100    CONTINUE
      END IF
*
*     Compute forward error bound, using matrix norm estimator
*
      EST = ZERO
      KASE = 0
  110 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
         IJ = 0
         IF( LOWER ) THEN
            DO 130 J = 1, N
               DO 120 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
                  END IF
  120          CONTINUE
  130       CONTINUE
         ELSE
            DO 150 J = 1, N
               DO 140 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the upper triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )   
                  END IF   
  140          CONTINUE
  150       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
         IF( KASE.EQ.2 ) THEN
*
*           Solve op(A')*Y*op(A) - Y = scale2*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE2,
     $                   WORK( IWRK+1 ), INFO2 )
         ELSE
*
*           Solve op(A)*Z*op(A') - Z = scale2*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE2,
     $                   WORK( IWRK+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         IJ = 0
         IF( LOWER ) THEN
            DO 170 J = 1, N
               DO 160 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  160          CONTINUE
  170       CONTINUE
         ELSE
            DO 190 J = 1, N
               DO 180 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the upper triangular part of symmetric   
*                    matrix 
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF   
  180          CONTINUE
  190       CONTINUE
         END IF
         GO TO 110
      END IF
*
*     Compute the estimate of the forward error 
*
      FERR =  TWO*EST / 
     $        DLANSY( 'Max', UPLO, N, X, LDX, WORK ) / SCALE2
      IF( FERR.GT.ONE ) FERR = ONE 
*       
      RETURN
*
*     End of LYPDFR
*
      END
      SUBROUTINE LYPDRC( FACT, TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, 
     $                   U, LDU, X, LDX, SCALE, RCOND, WORK, LWORK, 
     $                   IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          FACT, TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   RCOND, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),  
     $                   U( LDU, * ), WORK( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPDRC estimates the reciprocal of the condition number of the
*  discrete-time matrix Lyapunov equation
*
*         transpose(op(A))*X*op(A) - X = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is a
*  scale factor, set <= 1 during the solution of the equation to avoid
*  overflow in X. If the equation is not scaled, scale should be set
*  equal to 1.
*
*  Arguments
*  =========
*
*  FACT    (input) CHARACTER*1
*          Specifies whether or not the real Schur factorization
*          of the matrix A is supplied on entry:
*          = 'F':   On entry, T and U contain the factors from the
*                     real Schur factorization of the matrix A.
*          = 'N':   The Schur factorization of A will be computed
*                     and the factors will be stored in T and U.
*    
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input or output) DOUBLE PRECISION array, dimension (LDT,N)
*          If FACT = 'F', then T is an input argument and on entry
*          contains the upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*          If FACT = 'N', then T is an output argument and on exit  
*          returns the upper quasi-triangular matrix in Schur
*          canonical form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input or output) DOUBLE PRECISION array, dimension (LDU,N)
*          If FACT = 'F', then U is an input argument and on entry
*          contains the orthogonal matrix U from the real Schur
*          factorization of A.
*          If FACT = 'N', then U is an output argument and on exit
*          returns the orthogonal N-by-N matrix from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (input) DOUBLE PRECISION
*          The scale factor, scale.
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number 
*          of the discrete-time Lyapunov equation. 
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 4*N*N + 2*N + max(1,3*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix A cannot be reduced to Schur canonical form
*
*  Further Details
*  ===============
*
*  The condition number of the discrete Lyapunov equation is estimated 
*  as
*
*  cond = (norm(Theta)*norm(A) + norm(inv(Omega))*norm(C))/norm(X)
*
*  where Omega and Theta are linear operators defined by
*
*  Omega(Z) = transpose(op(A))*Z*op(A) - Z,
*  Theta(Z) = inv(Omega(transpose(op(Z))*X*op(A) + 
*                 transpose(op(A))*X*op(Z))).
*
*  The program estimates the quantities
*
*  sepd(op(A),transpose(op(A)) = 1 / norm(inv(Omega))
*
*  and norm(Theta) using 1-norm condition estimator.
*
*  References
*  ==========
*
*  [1] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 ) 
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOFACT, NOTRNA, VOIDDUMMY
      CHARACTER          TRANAT 
      INTEGER            I, IDLC, IJ, INFO2, ITMP, IWI, IWR, IWRK, IXMA,
     $                   J, KASE, LWA, MINWRK, SDIM
      DOUBLE PRECISION   ANORM, CNORM, EST, SCALE2, SEPD, THNORM, XNORM 
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANGE, DLANSY
      EXTERNAL           DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DLACON, DLACPY, DSYMM, DSYR2K,
     $                   LYPDTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOFACT = LSAME( FACT, 'N' )
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -6 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -14
      END IF
*
*     Compute workspace
*
      MINWRK = 4*N*N + 2*N + MAX( 1, 3*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -18 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPDRC', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $    RETURN
*
*     Compute the norms of the matrices A and C
*
      ANORM = DLANGE( '1', N, N, A, LDA, WORK )
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
*
*        Matrix all zero
*
         RCOND = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      LWA = 4*N*N + 2*N
      IDLC = N*N
      ITMP = IDLC + N*N
      IXMA = ITMP + N*N
      IWR = IXMA + N*N
      IWI = IWR + N
      IWRK = IWI + N
*
      IF( NOFACT ) THEN
* 
*        Compute the Schur factorization of A
*
         CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
         CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $        WORK( IWR+1 ),
     $        WORK( IWI+1 ), U, LDU, WORK( IWRK+1 ), LWORK-IWRK,
     $        BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         LWA = LWA + INT( WORK( IWRK+1 ) )
      END IF
*
*     Compute X*op(A)
*
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, X, LDX, A, LDA, ZERO,
     $            WORK( IXMA+1 ), N )
*
*     Estimate sepd(op(A),transpose(op(A))) 
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
* 
      EST = ZERO
      KASE = 0
   10 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 30 J = 1, N
               DO 20 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   20          CONTINUE
   30       CONTINUE
         ELSE
            DO 50 J = 1, N
               DO 40 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   40          CONTINUE
   50       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(A')*Y*op(A) - Y = scale2*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE2,
     $                   WORK( IWR+1 ), INFO2 )
         ELSE
*
*           Solve op(A)*Z*op(A') - Z = scale2*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE2,
     $                   WORK( IWR+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix 
*
         IJ = 0
         IF( LOWER ) THEN
            DO 70 J = 1, N
               DO 60 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   60          CONTINUE
   70       CONTINUE
         ELSE
            DO 90 J = 1, N
               DO 80 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   80          CONTINUE
   90       CONTINUE
         END IF
         GO TO 10
      END IF
*
      SEPD = SCALE2 / TWO / EST
*
*     Return if the equation is singular
*
      IF( SEPD.EQ.ZERO ) THEN 
         RCOND = ZERO
         RETURN
      END IF
*
*     Estimate norm(Theta)
*
      EST = ZERO
      KASE = 0
  100 CONTINUE
      CALL DLACON( N*N, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Compute RHS = op(W')*X*op(A) + op(A')*X*op(W)
*
         CALL DSYR2K( UPLO, TRANAT, N, N, ONE, WORK, N, WORK( IXMA+1 ),
     $                N, ZERO, WORK( ITMP+1 ), N ) 
         CALL DLACPY( UPLO, N, N, WORK( ITMP+1 ), N, WORK, N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK( ITMP+1 ),
     $               N, ZERO, WORK, N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(A')*Y*op(A) - Y = scale2*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK, N, SCALE2,
     $                   WORK( IWR+1 ), INFO2 )
         ELSE
*
*           Solve op(A)*Z*op(A') - Z = scale2*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK, N, SCALE2,
     $                   WORK( IWR+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK( ITMP+1 ), N, U,
     $               LDU, ZERO, WORK, N ) 
         GO TO 100
      END IF
*
      THNORM = EST / SCALE2
*
*     Estimate the reciprocal condition number
*
      RCOND = SEPD*XNORM / ( CNORM*SCALE + SEPD*( THNORM*ANORM ) )
      IF( RCOND.GT.ONE ) RCOND = ONE
*
      WORK( 1 ) = DBLE( LWA )
      RETURN
*
*     End of LYPDRC
*
      END
      SUBROUTINE LYPDSL( FACT, TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, 
     $                   U, LDU, WR, WI, X, LDX, SCALE, RCOND, FERR,  
     $                   WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          FACT, TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), T( LDT, * ),  
     $                   U( LDU, * ), WI( * ), WORK( * ), WR( * ),
     $                   X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPDSL solves the discrete-time matrix Lyapunov equation
*
*         transpose(op(A))*X*op(A) - X = scale*C
*
*  where op(A) = A or A**T and C is symmetric (C = C**T). A is N-by-N,
*  the right hand side C and the solution X are N-by-N, and scale is
*  an output scale factor, set <= 1 to avoid overflow in X.
*
*  Error bound on the solution and condition estimate are also provided.
*
*  Arguments
*  =========
*
*  FACT    (input) CHARACTER*1
*          Specifies whether or not the real Schur factorization
*          of the matrix A is supplied on entry:
*          = 'F':   On entry, T and U contain the factors from the
*                     real Schur factorization of the matrix A.
*          = 'N':   The Schur factorization of A will be computed
*                     and the factors will be stored in T and U.
*    
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  T       (input or output) DOUBLE PRECISION array, dimension (LDT,N)
*          If FACT = 'F', then T is an input argument and on entry
*          contains the upper quasi-triangular matrix in Schur canonical 
*          form from the Schur factorization of A.
*          If FACT = 'N', then T is an output argument and on exit  
*          returns the upper quasi-triangular matrix in Schur
*          canonical form from the Schur factorization of A.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input or output) DOUBLE PRECISION array, dimension (LDU,N)
*          If FACT = 'F', then U is an input argument and on entry
*          contains the orthogonal matrix U from the real Schur
*          factorization of A.
*          If FACT = 'N', then U is an output argument and on exit
*          returns the orthogonal N-by-N matrix from the real Schur
*          factorization of A.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if FACT = 'N', WR(1:N) and WI(1:N) contain the
*          real and imaginary parts, respectively, of the eigenvalues
*          of A.
*          If FACT = 'F', WR and WI are not referenced. 
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          If INFO = 0, the N-by-N solution matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N)
*
*  SCALE   (output) DOUBLE PRECISION
*          The scale factor, scale, set <= 1 to avoid overflow in X.
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number 
*          of the discrete-time Lyapunov equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE)  divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >= 7*N*N + 2*N + max(1,3*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix A cannot be reduced to Schur canonical form
*          = 2: A has almost reciprocal eigenvalues; perturbed
*                values were used to solve the equation (but the
*                matrix A is unchanged).
*
*  Further Details
*  ===============
*
*  The discrete-time matrix Lyapunov equation is solved by the Barraud-
*  Kitagawa algorithm [1], [2].
*
*  The condition number of the equation is estimated using 1-norm
*  condition estimator.
*
*  The forward error bound is estimated using the practical error bound
*  proposed in [3].
*
*  References
*  ==========
*
*                                                    T
*  [1] A.Y. Barraud. A numerical algorithm to solve A XA - X = Q.
*      IEEE Trans. Automat. Control, vol. AC-22, pp. 883-885, 1977.
*  [2] G. Kitagawa. An algorithm for solving the matrix equation X =
*         T
*      FXF  + S. Internat. J. Control, vol. 25, pp. 745-753, 1977.
*  [3] N.J. Higham, Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, 1993, pp. 124-136. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOFACT, NOTRNA, VOIDDUMMY
      INTEGER            INFO2, LWA, LWAMAX, MINWRK, SDIM
      DOUBLE PRECISION   CNORM
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANSY
      EXTERNAL           DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DLACPY, DLASET, DSYMM, LYPDFR,
     $                   LYPDRC, LYPDTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOFACT = LSAME( FACT, 'N' )
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOFACT .AND. .NOT.LSAME( FACT, 'F' ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -6 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -10
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -16
      END IF
*
*     Compute workspace
*
      MINWRK = 7*N*N + 2*N + MAX( 1, 3*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -21 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPDSL', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $    RETURN
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      IF( CNORM.EQ.ZERO ) THEN
*
*        Matrix all zero. Return zero solution
*
         CALL DLASET( 'F', N, N, ZERO, ZERO, X, LDX )
         SCALE = ONE
         RCOND = ZERO
         FERR = ZERO
         RETURN
      END IF
*
      LWA = 0
*
      IF( NOFACT ) THEN
* 
*        Compute the Schur factorization of A
*
         CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
         CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $        WR, WI, U, LDU,
     $        WORK, LWORK, BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         LWA = INT( WORK( 1 ) )
      END IF
      LWAMAX = LWA
*
*     Transform the right-hand side: C := U'*C*U.
*     Form TEMP = C*U then X = U'*TEMP
*
      CALL DSYMM( 'L', UPLO, N, N, ONE, C, LDC, U, LDU, ZERO, WORK, N )
      CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO, X,
     $            LDX )
*
*     Solve the quasi-triangular discrete-time Lyapunov equation.
*     The answer overwrites the right-hand side
*
      CALL LYPDTR( TRANA, N, T, LDT, X, LDX, SCALE, WORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
      END IF
*
*     Transform back to obtain the solution: X := U*X*U'.
*     Form TEMP = U*X then X = TEMP*U'
*
      CALL DSYMM( 'R', UPLO, N, N, ONE, X, LDX, U, LDU, ZERO, WORK, N )
      CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO, X,
     $            LDX )
*
*     Estimate the reciprocal of the condition number 
*
      CALL LYPDRC( 'F', TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU,
     $             X, LDX, SCALE, RCOND, WORK, LWORK, IWORK, INFO2 )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
         FERR = ONE
         RETURN
      END IF
      LWA = INT( WORK( 1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Estimate the bound on the forward error 
*
      CALL LYPDFR( TRANA, N, A, LDA, UPLO, C, LDC, T, LDT, U, LDU,
     $             X, LDX, SCALE, FERR, WORK, LWORK, IWORK, INFO2 )
      LWA = 7*N*N + 2*N
      LWAMAX = MAX( LWA, LWAMAX )
*       
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of LYPDSL
*
      END
      SUBROUTINE LYPDTR( TRANA, N, A, LDA, C, LDC, SCALE, WORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000 
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA
      INTEGER            INFO, LDA, LDC, N
      DOUBLE PRECISION   SCALE
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), WORK( N, * )
*     ..
*
*  Purpose
*  =======
*
*  LYPDTR solves the discrete-time matrix Lyapunov equation
*
*         transpose(op(A))*X*op(A) - X = scale*C 
*
*  where op(A) = A or A**T,  A is upper quasi-triangular and C is 
*  symmetric (C = C**T). A is N-by-N, the right hand side C and the
*  solution X are N-by-N, and scale is an output scale factor, 
*  set <= 1 to avoid overflow in X.
*
*  A must be in Schur canonical form (as returned by DHSEQR), that is,
*  block upper triangular with 1-by-1 and 2-by-2 diagonal blocks;
*  each 2-by-2 diagonal block has its diagonal elements equal and its
*  off-diagonal elements of opposite sign.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose)
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**H (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the
*          matrices X and C. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The upper quasi-triangular matrix A, in Schur canonical form.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  C       (input/output) DOUBLE PRECISION array, dimension (LDC,N)
*          On entry, the symmetric N-by-N right hand side matrix C.
*          On exit, C is overwritten by the solution matrix X.
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N)
*
*  SCALE   (output) DOUBLE PRECISION
*          The scale factor, scale, set <= 1 to avoid overflow in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (N,2)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: A has almost reciprocal eigenvalues; perturbed
*                values were used to solve the equation (but the
*                matrix A is unchanged).
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            NOTRNA
      INTEGER            IERR, J, K, K1, K2, KNEXT, L, L1, L2, LNEXT
      DOUBLE PRECISION   A11, BIGNUM, DA11, DB, EPS, P11, P12, P21, P22,
     $                   SCALOC, SMIN, SMLNUM, SUML, SUMR, XNORM
*     ..
*     .. Local Arrays ..
      DOUBLE PRECISION   DUM( 1 ), VEC( 2, 2 ), X( 2, 2 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DDOT, DLAMCH, DLANGE
      EXTERNAL           LSAME, DDOT, DLAMCH, DLANGE
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLABAD, DLALD2, DLASD2, DSCAL, DSYMV, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX, MIN
*     ..
*     .. Executable Statements ..
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -6
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'LYPDTR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 )
     $   RETURN
*
*     Set constants to control overflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' )
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
      SMLNUM = SMLNUM*DBLE( N*N ) / EPS
      BIGNUM = ONE / SMLNUM
*
      SMIN = MAX( SMLNUM, EPS*DLANGE( 'M', N, N, A, LDA, DUM ) )
*
      SCALE = ONE
*
      IF( NOTRNA ) THEN
*
*        Solve    A'*X*A - X = scale*C.
*
*        The (K,L)th block of X is determined starting from
*        upper-left corner column by column by
*
*          A(K,K)'*X(K,L)*A(L,L) - X(K,L) = C(K,L) - R(K,L)
*
*        where
*
*                    K           L-1                  
*          R(K,L) = SUM {A(I,K)'*SUM [X(I,J)*A(J,L)]} +
*                   I=1          J=1                  
*                    
*                    K-1
*                   {SUM [A(I,K)'*X(I,L)]}*A(L,L) 
*                    I=1
*
*        Start column loop (index = L)
*        L1 (L2): column index of the first (last) row of X(K,L)
*
         LNEXT = 1
         DO 60 L = 1, N
            IF( L.LT.LNEXT )
     $         GO TO 60
            IF( L.EQ.N ) THEN
               L1 = L
               L2 = L
            ELSE
               IF( A( L+1, L ).NE.ZERO ) THEN
                  L1 = L
                  L2 = L + 1
                  LNEXT = L + 2
               ELSE
                  L1 = L
                  L2 = L
                  LNEXT = L + 1
               END IF
            END IF
*
*           Start row loop (index = K)
*           K1 (K2): row index of the first (last) row of X(K,L)
*
            CALL DSCAL( L1, ZERO, WORK( 1, 1 ), 1 ) 
            CALL DSCAL( L1, ZERO, WORK( 1, 2 ), 1 )
            CALL DSYMV( 'L', L1-1, ONE, C, LDC, A( 1, L1 ), 1,
     $                  ZERO, WORK( 1, 1 ), 1 ) 
            CALL DSYMV( 'L', L1-1, ONE, C, LDC, A( 1, L2 ), 1,
     $                  ZERO, WORK( 1, 2 ), 1 ) 
* 
            KNEXT = L
            DO 50 K = L, N
               IF( K.LT.KNEXT )
     $            GO TO 50
               IF( K.EQ.N ) THEN
                  K1 = K
                  K2 = K
               ELSE
                  IF( A( K+1, K ).NE.ZERO ) THEN
                     K1 = K
                     K2 = K + 1
                     KNEXT = K + 2
                  ELSE
                     K1 = K
                     K2 = K
                     KNEXT = K + 1
                  END IF
               END IF
*
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  WORK( K1, 1 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L1 ), 1 ) 
                  P11 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
*
                  SUML = DDOT( K1, A( 1, K1 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
                  SCALOC = ONE
*
                  A11 = A( K1, K1 )*A( L1, L1 ) - ONE
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
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 10 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   10                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 )
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF  
*
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L1 ), 1 )
                  WORK( K2, 1 ) = DDOT( L1-1, C( K2, 1 ), LDC, 
     $                            A( 1, L1 ), 1 )
                  P11 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
*   
                  SUML = DDOT( K2, A( 1, K1 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K2, A( 1, K2 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P21*A( L1, L1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  CALL DLASD2( .TRUE., .FALSE., 1, 2, 1, A( K1, K1 ),
     $                         LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                         X, 2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 20 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   20                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 ) 
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 )  
*
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L1 ), 1 )
                  WORK( K1, 2 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L2 ), 1 )
                  P11 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  P12 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
*
                  SUML = DDOT( K1, A( 1, K1 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 ) + P12*A( L2, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K1, A( 1, K1 ), 1, WORK( 1, 2 ), 1 )
                  SUMR = P11*A( L1, L2 ) + P12*A( L2, L2 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  CALL DLASD2( .TRUE., .FALSE., 1, 1, 2, A( K1, K1 ),
     $                         LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                         X, 2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 30 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   30                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 )
                     CALL DSCAL( N, SCALOC, WORK( 1, 2 ), 1 ) 
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 1, 2 )
*
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L1 ), 1 )
                  WORK( K2, 1 ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                            A( 1, L1 ), 1 )
                  WORK( K1, 2 ) = DDOT( L1-1, C( K1, 1 ), LDC,
     $                            A( 1, L2 ), 1 )
                  WORK( K2, 2 ) = DDOT( L1-1, C( K2, 1 ), LDC,
     $                            A( 1, L2 ), 1 )
                  P11 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L1 ), 1 )
                  P12 = DDOT( K1-1, A( 1, K1 ), 1, C( 1, L2 ), 1 )
                  P21 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L1 ), 1 )
                  P22 = DDOT( K1-1, A( 1, K2 ), 1, C( 1, L2 ), 1 )
*
                  SUML = DDOT( K2, A( 1, K1 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 ) + P12*A( L2, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K2, A( 1, K1 ), 1, WORK( 1, 2 ), 1 )
                  SUMR = P11*A( L1, L2 ) + P12*A( L2, L2 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K2, A( 1, K2 ), 1, WORK( 1, 1 ), 1 )
                  SUMR = P21*A( L1, L1 ) + P22*A( L2, L1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( K2, A( 1, K2 ), 1, WORK( 1, 2 ), 1 )
                  SUMR = P21*A( L1, L2 ) + P22*A( L2, L2 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUML+SUMR )
*
                  IF( K1.EQ.L1 ) THEN
                     CALL DLALD2( .FALSE., A( K1, K1 ), LDA, VEC, 2,
     $                            SCALOC, X, 2, XNORM, IERR )
                  ELSE 
                     CALL DLASD2( .TRUE., .FALSE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 40 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   40                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 )
                     CALL DSCAL( N, SCALOC, WORK( 1, 2 ), 1 ) 
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
*
   50       CONTINUE
   60    CONTINUE
*
      ELSE
*
*        Solve    A*X*A' - X = scale*C.
*
*        The (K,L)th block of X is determined starting from
*        bottom-right corner column by column by
*
*            A(K,K)*X(K,L)*A(L,L)' = C(K,L) - R(K,L)
*
*        where
*
*                    N            N                  
*          R(K,L) = SUM {A(K,I)* SUM [X(I,J)*A(L,J)']} +
*                   I=K         J=L+1                  
*                    
*                      N
*                   { SUM [A(K,J)*X(J,L)]}*A(L,L)' 
*                    J=K+1
*
*        Start column loop (index = L)
*        L1 (L2): column index of the first (last) row of X(K,L)
*
         LNEXT = N
         DO 120 L = N, 1, -1
            IF( L.GT.LNEXT )
     $         GO TO 120
            IF( L.EQ.1 ) THEN
               L1 = L
               L2 = L
            ELSE
               IF( A( L, L-1 ).NE.ZERO ) THEN
                  L1 = L - 1
                  L2 = L
                  LNEXT = L - 2
               ELSE
                  L1 = L
                  L2 = L
                  LNEXT = L - 1
               END IF
            END IF
*
*           Start row loop (index = K)
*           K1 (K2): row index of the first (last) row of X(K,L)
*
            CALL DSCAL( N-L1+1, ZERO, WORK( L1, 1 ), 1 ) 
            CALL DSCAL( N-L1+1, ZERO, WORK( L1, 2 ), 1 )
            CALL DSYMV( 'U', N-L2, ONE, C( L2+1, L2+1 ), LDC,
     $                  A( L1, L2+1 ), LDA, ZERO,
     $                  WORK( L2+1, 1 ), 1 ) 
            CALL DSYMV( 'U', N-L2, ONE, C( L2+1, L2+1 ), LDC,
     $                  A( L2, L2+1 ), LDA, ZERO,
     $                  WORK( L2+1, 2 ), 1 )
* 
            KNEXT = L
            DO 110 K = L, 1, -1
               IF( K.GT.KNEXT )
     $            GO TO 110
               IF( K.EQ.1 ) THEN
                  K1 = K
                  K2 = K
               ELSE
                  IF( A( K, K-1 ).NE.ZERO ) THEN
                     K1 = K - 1
                     K2 = K
                     KNEXT = K - 2
                  ELSE
                     K1 = K
                     K2 = K
                     KNEXT = K - 1
                  END IF
               END IF
*
               IF( L1.EQ.L2 .AND. K1.EQ.K2 ) THEN
                  WORK( K1, 1 ) = DDOT( N-L1, C( K1, MIN( L1+1, N ) ),
     $                            LDC, A( L1, MIN( L1+1, N ) ), LDA ) 
                  P11 = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                  C( MIN( K1+1, N ), L1 ), 1 )
*
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
                  SCALOC = ONE
*
                  A11 = A( K1, K1 )*A( L1, L1 ) - ONE
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
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 70 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   70                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 ) 
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  IF( K1.NE.L1 ) THEN
                     C( L1, K1 ) = X( 1, 1 )
                  END IF   
*
               ELSE IF( L1.EQ.L2 .AND. K1.NE.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( N-L1, C( K1, MIN( L1+1, N ) ),
     $                            LDC, A( L1, MIN( L1+1, N ) ), LDA )
                  WORK( K2, 1 ) = DDOT( N-L1, C( K2, MIN( L1+1, N ) ),
     $                            LDC, A( L1, MIN( L1+1, N ) ), LDA )
                  P11 = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L1 ), 1 )
                  P21 = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L1 ), 1 )
*   
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K1+1, A( K2, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P21*A( L1, L1 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  CALL DLASD2( .FALSE., .TRUE., 1, 2, 1, A( K1, K1 ),
     $                         LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                         X, 2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 80 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   80                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 ) 
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K2, L1 ) = X( 2, 1 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L1, K2 ) = X( 2, 1 )
*
               ELSE IF( L1.NE.L2 .AND. K1.EQ.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( N-L2, C( K1, MIN( L2+1, N ) ),
     $                            LDC, A( L1, MIN( L2+1, N ) ), LDA )
                  WORK( K1, 2 ) = DDOT( N-L2, C( K1, MIN( L2+1, N ) ),
     $                            LDC, A( L2, MIN( L2+1, N ) ), LDA )
                  P11 = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                  C( MIN( K1+1, N ), L1 ), 1 )
                  P12 = DDOT( N-K1, A( K1, MIN( K1+1, N ) ), LDA,
     $                  C( MIN( K1+1, N ), L2 ), 1 )
*
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 ) + P12*A( L1, L2 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 2 ), 1 )
                  SUMR = P11*A( L2, L1 ) + P12*A( L2, L2 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  CALL DLASD2( .FALSE., .TRUE., 1, 1, 2, A( K1, K1 ),
     $                         LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                         X, 2, XNORM, IERR )
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 90 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
   90                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 )
                     CALL DSCAL( N, SCALOC, WORK( 1, 2 ), 1 )  
                     SCALE = SCALE*SCALOC
                  END IF
                  C( K1, L1 ) = X( 1, 1 )
                  C( K1, L2 ) = X( 1, 2 )
                  C( L1, K1 ) = X( 1, 1 )
                  C( L2, K1 ) = X( 1, 2 )
*
               ELSE IF( L1.NE.L2 .AND. K1.NE.K2 ) THEN
*
                  WORK( K1, 1 ) = DDOT( N-L2, C( K1, MIN( L2+1, N ) ),
     $                            LDC, A( L1, MIN( L2+1, N ) ), LDA )
                  WORK( K2, 1 ) = DDOT( N-L2, C( K2, MIN( L2+1, N ) ),
     $                            LDC, A( L1, MIN( L2+1, N ) ), LDA )
                  WORK( K1, 2 ) = DDOT( N-L2, C( K1, MIN( L2+1, N ) ),
     $                            LDC, A( L2, MIN( L2+1, N ) ), LDA )
                  WORK( K2, 2 ) = DDOT( N-L2, C( K2, MIN( L2+1, N ) ),
     $                            LDC, A( L2, MIN( L2+1, N ) ), LDA )
                  P11 = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L1 ), 1 )
                  P12 = DDOT( N-K2, A( K1, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L2 ), 1 )
                  P21 = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L1 ), 1 )
                  P22 = DDOT( N-K2, A( K2, MIN( K2+1, N ) ), LDA,
     $                  C( MIN( K2+1, N ), L2 ), 1 )
*
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P11*A( L1, L1 ) + P12*A( L1, L2 )
                  VEC( 1, 1 ) = C( K1, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K1+1, A( K1, K1 ), LDA,
     $                   WORK( K1, 2 ), 1 )
                  SUMR = P11*A( L2, L1 ) + P12*A( L2, L2 )
                  VEC( 1, 2 ) = C( K1, L2 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K1+1, A( K2, K1 ), LDA,
     $                   WORK( K1, 1 ), 1 )
                  SUMR = P21*A( L1, L1 ) + P22*A( L1, L2 )
                  VEC( 2, 1 ) = C( K2, L1 ) - ( SUML+SUMR )
*
                  SUML = DDOT( N-K1+1, A( K2, K1 ), LDA,
     $                   WORK( K1, 2 ), 1 )
                  SUMR = P21*A( L2, L1 ) + P22*A( L2, L2 )
                  VEC( 2, 2 ) = C( K2, L2 ) - ( SUML+SUMR )
*
                  IF( K1.EQ.L1 ) THEN
                     CALL DLALD2( .TRUE., A( K1, K1 ), LDA, VEC, 2,
     $                            SCALOC, X, 2, XNORM, IERR )
                  ELSE 
                     CALL DLASD2( .FALSE., .TRUE., 1, 2, 2, A( K1, K1 ),
     $                            LDA, A( L1, L1 ), LDA, VEC, 2, SCALOC,
     $                            X, 2, XNORM, IERR )
                  END IF 
                  IF( IERR.NE.0 )
     $               INFO = 1
*
                  IF( SCALOC.NE.ONE ) THEN
                     DO 100 J = 1, N
                        CALL DSCAL( N, SCALOC, C( 1, J ), 1 )
  100                CONTINUE
                     CALL DSCAL( N, SCALOC, WORK( 1, 1 ), 1 )
                     CALL DSCAL( N, SCALOC, WORK( 1, 2 ), 1 )  
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
*
  110       CONTINUE
  120    CONTINUE
*
      END IF
*
      RETURN
*
*     End of LYPDTR
*
      END
      SUBROUTINE RICCFR( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   T, LDT, U, LDU, FERR, WORK, LWORK, IWORK,
     $                   INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   T( LDT, * ), U( LDU, * ), WORK( * ),
     $                   X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  RICCFR estimates the forward error bound for the computed solution of
*  the matrix algebraic Riccati equation
*
*  transpose(op(A))*X + X*op(A) + C - X*D*X = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C, D and X are N-by-N.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical form
*          from the Schur factorization of the matrix Ac = A - D*X
*          (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T' or 'C').
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal N-by-N matrix from the real Schur
*          factorization of the matrix Ac = A - D*X (if TRANA = 'N')
*          or Ac = A - X*D (if TRANA = 'T' or 'C').
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  FERR    (output) DOUBLE PRECISION
*          The estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*
*  LWORK   INTEGER
*          The dimension of the array WORK. LWORK >= 7*N*N 
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
*  Further details
*  ===============
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [1].
*
*  References
*  ==========
*
*  [1] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [2] P.Hr. Petkov, M.M. Konstantinov, and V. Mehrmann. DGRSVX and 
*      DMSRIC: Fortan 77 subroutines for solving continuous-time matrix
*      algebraic Riccati equations with condition and accuracy 
*      estimates. Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
*      Chemnitz, May 1998. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, FOUR
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0,
     $                     FOUR = 4.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          TRANAT 
      INTEGER            I, IABS, IDBS, IDLC, IJ, INFO2, IRES, ITMP,
     $                   IXBS, J, KASE, MINWRK
      DOUBLE PRECISION   EPS, EST, SCALE, XNORM
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEMM, DLACON, DLACPY, DSYMM, DSYR2K, LYPCTR,
     $                   XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -13
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -15
      END IF
*
*     Get the machine precision
*
      EPS = DLAMCH( 'Epsilon' )
*
*     Compute workspace
*
      MINWRK = 7*N*N
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -18 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICCFR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN 
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
         FERR = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      IDLC = N*N
      ITMP = IDLC + N*N
      IABS = ITMP + N*N
      IDBS = IABS + N*N
      IXBS = IDBS + N*N
      IRES = IXBS + N*N
*    
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
* 
*     Form residual matrix R = transpose(op(A))*X + X*op(A) + C - X*D*X
*
      CALL DLACPY( UPLO, N, N, C, LDC, WORK( IRES+1 ), N )
      CALL DSYR2K( UPLO, TRANAT, N, N, ONE, A, LDA, X, LDX, ONE, 
     $             WORK( IRES+1 ), N )
      CALL DSYMM( 'R', UPLO, N, N, ONE, D, LDD, X, LDX, ZERO, 
     $            WORK( ITMP+1 ), N )
      CALL DSYMM( 'R', UPLO, N, N, -ONE, X, LDX, WORK( ITMP+1 ), N, ONE,
     $            WORK( IRES+1 ), N )
*
*     Add to abs(R) a term that takes account of rounding errors in 
*     forming R: 
*       abs(R) := abs(R) + EPS*(4*abs(C) + (n+4)*(abs(op(A'))*abs(X) +
*                   abs(X)*abs(op(A))) + 2*(n+1)*abs(X)*abs(D)*abs(X))
*     where EPS is the machine precision
*
      IJ = 0
      DO 20 J = 1, N      
         DO 10 I = 1, N
            IJ = IJ + 1
            WORK( IABS+IJ ) = ABS( A( I, J ) )
            WORK( IXBS+IJ ) = ABS( X( I, J ) )
   10    CONTINUE         
   20 CONTINUE
      IF( LOWER ) THEN
         DO 40 J = 1, N
            DO 30 I = J, N
               WORK( ITMP+I+(J-1)*N ) = ABS( C( I, J ) ) 
               WORK( IDBS+I+(J-1)*N ) = ABS( D( I, J ) )
   30       CONTINUE
   40    CONTINUE
      ELSE 
         DO 60 J = 1, N
            DO 50 I = 1, J
               WORK( ITMP+I+(J-1)*N ) = ABS( C( I, J ) ) 
               WORK( IDBS+I+(J-1)*N ) = ABS( D( I, J ) )  
   50       CONTINUE
   60    CONTINUE
      END IF  
      CALL DSYR2K( UPLO, TRANAT, N, N, DBLE( N+4 )*EPS, WORK( IABS+1 ), 
     $             N, WORK( IXBS+1 ), N, FOUR*EPS, WORK( ITMP+1 ), N )
      CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( IDBS+1 ), N, 
     $            WORK( IXBS+1 ), N, ZERO, WORK( IDLC+1 ), N )
      CALL DSYMM( 'R', UPLO, N, N, DBLE( 2*N+2 )*EPS, WORK( IXBS+1 ), N, 
     $            WORK( IDLC+1 ), N, ONE, WORK( ITMP+1 ), N )
      IF( LOWER ) THEN
         DO 80 J = 1, N
            DO 70 I = J, N
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $                      WORK( ITMP+I+(J-1)*N )
   70       CONTINUE
   80    CONTINUE
      ELSE
         DO 100 J = 1, N
            DO 90 I = 1, J
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $                      WORK( ITMP+I+(J-1)*N )
   90       CONTINUE
  100    CONTINUE
      END IF
*
*     Compute forward error bound, using matrix norm estimator
*
      EST = ZERO
      KASE = 0
  110 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
         IJ = 0
         IF( LOWER ) THEN
            DO 130 J = 1, N
               DO 120 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )  
                  END IF
  120          CONTINUE
  130       CONTINUE
         ELSE
            DO 150 J = 1, N
               DO 140 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the upper triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
                  END IF
  140          CONTINUE
  150       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, 
     $               WORK, N, ZERO, WORK( ITMP+1 ), N )
         IF( KASE.EQ.2 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, 
     $               U, LDU, ZERO, WORK( ITMP+1 ), N )
         IJ = 0
         IF( LOWER ) THEN
            DO 170 J = 1, N
               DO 160 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the lower triangular part of symmetric
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  160          CONTINUE
  170       CONTINUE
         ELSE
            DO 190 J = 1, N
               DO 180 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the upper triangular part of symmetric  
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  180          CONTINUE
  190       CONTINUE
         END IF
         GO TO 110
      END IF
*
*     Compute the estimate of the forward error 
*
      FERR =  TWO*EST / DLANSY( 'Max', UPLO, N, X, LDX, WORK ) / SCALE
      IF( FERR.GT.ONE ) FERR = ONE
*
      RETURN
*
*     End of RICCFR
*
      END
      SUBROUTINE RICCMF( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   X( LDX, * ), WI( * ), WORK( * ), WR( * ) 
*     ..
*
*  Purpose
*  =======
*
*  RICCMF solves the matrix algebraic Riccati equation
*
*     transpose(op(A))*X + X*op(A) + C - X*D*X = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C and D are N-by-N and the solution X is N-by-N.
*
*  Error bound on the solution and a condition estimate are also
*  provided.
*
*  It is assumed that the matrices A, C and D are such that the
*  corresponding Hamiltonian matrix has N eigenvalues with negative
*  real parts.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WR(1:N) and WI(1:N) contain the real
*          and imaginary parts, respectively, of the eigenvalues of
*          Ac = A - D*X (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T'
*          or 'C').
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number of
*          the Riccati equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 28*N*N + 2*N + 
*          max(1,2*N). 
*          For optimum performance LWORK >= 28*N*N + 2*N + ( 2*N+1 )*NB,
*          where NB is the optimal blocksize.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the iteration for the spectral decomposition failed to
*               converge after 50 iterations, but an approximate
*               solution and error bounds have been computed    
*          = 2: the system of linear equations for the solution is
*               singular to working precision
*          = 3: the matrix A-D*X (or A-X*D) can not be reduced to Schur
*               canonical form and condition number estimate and
*               forward error estimate are not computed
*
*  Further Details
*  ===============
*
*  The matrix Riccati equation is solved by the inverse free method
*  proposed in [1].
*
*  The condition number of the equation is estimated using 1-norm
*  estimator.
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [2].
*
*  References 
*  ==========
*
*  [1] Z. Bai and Q. Qian. Inverse free parallel method for the
*      numerical solution of algebraic Riccati equations. In J.G. Lewis,
*      editor, Proc. Fifth SIAM Conf. on Appl. Lin. Algebra, Snowbird,
*      UT, June 1994, pp. 167-171. SIAM, Philadelphia, PA, 1994.
*  [2] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [3] P.Hr. Petkov, M.M. Konstantinov, and V. Mehrmann. DGRSVX and 
*      DMSRIC: Fortan 77 subroutines for solving continuous-time matrix
*      algebraic Riccati equations with condition and accuracy 
*      estimates. Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
*      Chemnitz, May 1998. 
* 
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            MAXIT
      PARAMETER          ( MAXIT = 50 )
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          EQUED 
      INTEGER            I, IA, IAF, IB, IBR, IC, IFR, IJ, IJ1, IJ2,
     $                   INFO2, IQ, IR, IS, ISCL, ITAU, ITER, IU, IV,
     $                   IWRK, J, LWA, LWA0, LWAMAX, MINWRK, N2, N4
      DOUBLE PRECISION   CNORM, CNORM2, DNORM, DNORM2, EPS, RDNORM,
     $                   RNORM, TEMP, TOL
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DAXPY, DGEMM, DGEQP3, DGEQRF, DGERQF, DLACPY,
     $                   DLASET, DORMQR, DORMRQ, DGESVX, DLASCL, DSCAL, 
     $                   RICCFR, RICCRC, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      END IF
*
*     Set tol
*
      EPS = DLAMCH( 'Epsilon' )
      TOL = 10.0D+0*DBLE(N)*EPS
*
*     Compute workspace
*
      MINWRK = 28*N*N + 2*N + MAX( 1, 2*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICCMF', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
*
*     Compute the norms of the matrices C and D
*
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
      N2 = 2*N
      N4 = 4*N
*
*     Construct the Hamiltonian matrix
*
      DO 20 J = 1, N
         DO 10 I = 1, N
            IJ = ( J - 1 )*N2 + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = A( I, J )
            ELSE
               WORK( IJ ) = A( J, I )
            END IF
            IJ = ( J - 1 )*N2 + N + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            END IF
            IJ = ( N + J - 1 )*N2 + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = -D( I, J )
               ELSE
                  WORK( IJ ) = -D( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = -D( I, J )
               ELSE
                  WORK( IJ ) = -D( J, I )
               END IF
            END IF
            IJ = ( N + J - 1)*N2 + N + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = -A( J, I )
            ELSE
               WORK( IJ ) = -A( I, J )
            END IF
   10    CONTINUE
   20 CONTINUE
*
*     Scale the Hamiltonian matrix     
*
      CNORM2 = SQRT( CNORM )
      DNORM2 = SQRT( DNORM )
      ISCL = 0
      IF( CNORM2.GT.DNORM2 .AND. DNORM2.GT.ZERO ) THEN
         CALL DLASCL( 'G', 0, 0, CNORM2, DNORM2, N, N, WORK( N+1 ), N2, 
     $                INFO2 ) 
         CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, WORK( N2*N+1 ), 
     $                N2, INFO2 )
         ISCL = 1
      END IF
*      
*     Workspace usage
*
      LWA0 = 28*N*N + 2*N
      LWAMAX = 0
      IA = N2*N2
      IR = IA + N2*N2
      IS = IR + N4*N2
      IQ = IS + N2*N2
      ITAU = IQ + N4*N2
      IWRK = ITAU + N2
*
*     Compute B0 and -A0
*
      DO 40 J = 1, N2
         DO 30 I = 1, N2
            IJ1 = ( J - 1 )*N2 + I
            IJ2 = IA + ( J - 1 )*N2 + I
            TEMP = WORK( IJ1 )
            IF( I.EQ.J ) THEN
               WORK( IJ1 ) = ONE + TEMP
               WORK( IJ2 ) = -ONE + TEMP
            ELSE
               WORK( IJ2 ) = TEMP
            END IF
   30    CONTINUE
   40 CONTINUE
      CALL DLACPY( 'F', N2, N2, WORK, N2, WORK( IR+1 ), N4 )
      CALL DLACPY( 'F', N2, N2, WORK( IA+1 ), N2, WORK( IR+N2+1 ), N4 )
*
*     Main iteration loop
*
      DO 80 ITER = 1, MAXIT
*
*                            [ Bj]
*        QR decomposition of [   ]
*                            [-Aj]
*
         CALL DGEQRF( N4, N2, WORK( IR+1 ), N4, WORK( ITAU+1 ),
     $                WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
         LWA = LWA0 + INT( WORK( IWRK+1 ) )
         LWAMAX = MAX( LWA, LWAMAX )
*
*        Make the diagonal elements of Rj positive
*
         DO 50 I = 1, N2
            IF( WORK( IR+(I-1)*N4+I ).LT.ZERO ) 
     $         CALL DSCAL( N2-I+1, -ONE, WORK( IR+(I-1)*N4+I ), N4 )
   50    CONTINUE
         IF( ITER.GT.1 ) THEN
*
*           Compute Rj+1 - Rj
*
            DO 70 J = 1, N2
               DO 60 I = 1, J
                  IJ1 = IR + ( J - 1 )*N4 + I
                  IJ2 = IS + ( J - 1 )*N2 + I
                  WORK( IJ2 ) = WORK( IJ1 ) - WORK( IJ2 )
   60          CONTINUE
   70       CONTINUE
            RDNORM = DLANGE( '1', N2, N2, WORK( IS+1 ), N2,
     $                       WORK( IWRK+1 ))
         END IF
*
*        Save Rj for future use
*
         CALL DLACPY( 'U', N2, N2, WORK( IR+1 ), N4, WORK( IS+1 ), N2 )
         IF( ITER.EQ.1 )
     $      CALL DLASET( 'L', N2-1, N2-1, ZERO, ZERO, WORK( IS+2 ), N2 )
*
*        Generate the matrices Q12 and Q22
*
         CALL DLASET( 'F', N2, N2, ZERO, ZERO, WORK( IQ+1 ), N4 )
         CALL DLASET( 'F', N2, N2, ZERO, ONE, WORK( IQ+N2+1 ), N4 )
         CALL DORMQR( 'L', 'N', N4, N2, N2, WORK( IR+1 ), N4,
     $                WORK( ITAU+1 ), WORK( IQ+1 ), N4, WORK( IWRK+1 ),
     $                LWORK-IWRK, INFO2 )
         LWA = LWA0 + INT( WORK( IWRK+1 ) )
         LWAMAX = MAX( LWA, LWAMAX )
*
*        Compute Bj and -Aj
*
         CALL DGEMM( 'T', 'N', N2, N2, N2, ONE, WORK( IQ+N2+1 ),
     $               N4, WORK, N2, ZERO, WORK( IR+1 ), N4 )
         CALL DGEMM( 'T', 'N', N2, N2, N2, ONE, WORK( IQ+1 ), N4,
     $               WORK( IA+1 ), N2, ZERO, WORK( IR+N2+1 ), N4 )
         CALL DLACPY( 'F', N2, N2, WORK( IR+1 ), N4, WORK, N2 )
         CALL DLACPY( 'F', N2, N2, WORK( IR+N2+1 ), N4, WORK( IA+1 ),
     $                N2 )
*
*        Test for convergence
*
         IF( ITER.GT.1 .AND. RDNORM.LE.TOL*RNORM ) GO TO 90
         RNORM = DLANGE( '1', N2, N2, WORK( IS+1 ), N2,
     $                   WORK( IWRK+1 ))
   80 CONTINUE
      INFO = 1
   90 LWA0 = 10*N*N + 2*N
      IQ = IA + N2*N2
      ITAU = IQ + N2*N
      IWRK = ITAU + N2
*
*     Compute Ap + Bp
*
      CALL DSCAL( N2*N2, -ONE, WORK( IA+1 ), 1 )
      CALL DAXPY( N2*N2, ONE, WORK( IA+1 ), 1, WORK, 1 )
*
*     QR decomposition with column pivoting of Ap
*
      DO 100 J = 1, N2
         IWORK( J ) = 0
  100 CONTINUE
      CALL DGEQP3( N2, N2, WORK( IA+1 ), N2, IWORK, WORK( ITAU+1 ),
     $             WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*               T
*     Compute Q1 (Ap + Bp) 
*
      CALL DORMQR( 'L', 'T', N2, N2, N2, WORK( IA+1 ), N2,
     $             WORK( ITAU+1 ), WORK, N2, WORK( IWRK+1 ), LWORK-IWRK,
     $             INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*                           T
*     RQ decomposition of Q1 (Ap + Bp)
*
      CALL DGERQF( N2, N2, WORK, N2, WORK( ITAU+1 ), WORK( IWRK+1 ),
     $             LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Generate Q11 and Q21
*
      CALL DLASET( 'F', N, N, ZERO, ONE, WORK( IQ+1 ), N2 )
      CALL DLASET( 'F', N, N, ZERO, ZERO, WORK( IQ+N+1 ), N2 )
      CALL DORMRQ( 'L', 'T', N2, N, N2, WORK, N2, WORK( ITAU+1 ),
     $             WORK( IQ+1 ), N2, WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Store the matrices Q11 and Q21
*
      DO 120 J = 1, N
         DO 110 I = 1, N
            IJ = ( J - 1 )*N + I
            IV = IQ + ( I - 1 )*N2 + J
            WORK( IJ ) = WORK( IV )
            IJ = ( J - 1 )*N + 2*N*N + I
            IV = IQ + ( I - 1 )*N2 + N + J
            WORK( IJ ) = WORK( IV )
  110    CONTINUE
  120 CONTINUE
*
*     Workspace usage
*
      IAF = N*N  
      IB = IAF + N*N
      IR = IB + N*N
      IC = IR + N
      IFR = IC + N
      IBR = IFR + N
      IWRK = IBR + N
* 
*     Compute the solution matrix X
*
      CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N,
     $             IWORK, EQUED, WORK( IR+1 ), WORK( IC+1 ),
     $             WORK( IB+1 ), N, X, LDX, RCOND, WORK( IFR+1 ),
     $             WORK( IBR+1 ), WORK( IWRK+1 ), IWORK( N+1 ), 
     $             INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
*
*     Symmetrize the solution
*
      IF( N.GT.1 ) THEN
         DO 140 I = 1, N - 1
            DO 130 J = I + 1, N
               TEMP = ( X( I, J ) + X( J, I ) ) / TWO
               X( I, J ) = TEMP
               X( J, I ) = TEMP
  130       CONTINUE
  140    CONTINUE
      END IF
*
*     Undo scaling for the solution matrix
*
      IF( ISCL.EQ.1 ) 
     $   CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, X, LDX, INFO2 )
*
*     Workspace usage
*
      LWA = 2*N*N
      IU = N*N
      IWRK = IU + N*N
*
*     Estimate the reciprocal condition number
*
      CALL RICCRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             RCOND, WORK, N, WORK( IU+1 ), N, WR, WI,
     $             WORK( IWRK+1 ), LWORK-IWRK, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 3
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
         FERR = ONE
         WORK( 1 ) = DBLE( LWAMAX )
         RETURN
      END IF
*
*     Estimate the bound on the forward error
*
      CALL RICCFR( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             WORK, N, WORK( IU+1 ), N, FERR, WORK( IWRK+1 ),
     $             LWORK-IWRK, IWORK, INFO2 )
      LWA = 9*N*N
      LWAMAX = MAX( LWA, LWAMAX )
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of RICCMF
*
      END
      SUBROUTINE RICCMS( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   X( LDX, * ), WI( * ), WORK( * ), WR( * ) 
*     ..
*
*  Purpose
*  =======
*
*  RICCMS solves the matrix algebraic Riccati equation
*
*     transpose(op(A))*X + X*op(A) + C - X*D*X = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C and D are N-by-N and the solution X is N-by-N.
*
*  Error bound on the solution and a condition estimate are also
*  provided.
*
*  It is assumed that the matrices A, C and D are such that the
*  corresponding Hamiltonian matrix has N eigenvalues with negative
*  real parts.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N) 
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N) 
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          On exit, if INFO = 0, the N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WR(1:N) and WI(1:N) contain the real
*          and imaginary parts, respectively, of the eigenvalues of
*          Ac = A - D*X (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T'
*          or 'C').
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number of
*          the Riccati equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 9*N*N + 7*N + 1. 
*          For optimum performance LWORK >= 9*N*N + 5*N + ( 2*N+1 )*NB,
*          where NB is the optimal blocksize.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the Hamiltonian matrix has eigenvalues on the imaginary
*               axis, so the solution and error bounds could not be
*               computed
*          = 2: the iteration for the matrix sign function failed to
*               converge after 50 iterations, but an approximate
*               solution and error bounds have been computed    
*          = 3: the system of linear equations for the solution is
*               singular to working precision, so the solution and
*               error bounds could not be computed
*          = 4: the matrix A-D*X (or A-X*D) can not be reduced to Schur
*               canonical form and condition number estimate and
*               forward error estimate have not been computed.
*
*  Further Details
*  ===============
*
*  The Riccati equation is solved by the matrix sign function approach
*  [1], [2] implementing a scaling which enhances the numerical 
*  stability [4].
*
*  The condition number of the equation is estimated using 1-norm
*  condition estimator.
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [3].
*
*  References
*  ==========
*
*  [1] Z. Bai, J. Demmel, J. Dongarra, A. Petitet, H. Robinson, and
*      K. Stanley. The spectral decomposition of nonsymmetric matrices
*      on distributed memory parallel computers. SIAM J. Sci. Comput.,
*      vol. 18, pp. 1446-1461, 1997.
*  [2] R. Byers, C. He, and V. Mehrmann. The matrix sign function method
*      and the computation of invariant subspaces. SIAM J. Matrix Anal.
*      Appl., vol. 18, pp. 615-632, 1997.
*  [3] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [4] P.Hr. Petkov, M.M. Konstantinov, and V. Mehrmann. DGRSVX and 
*      DMSRIC: Fortan 77 subroutines for solving continuous-time matrix
*      algebraic Riccati equations with condition and accuracy 
*      estimates. Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
*      Chemnitz, May 1998. 
* 
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            MAXIT
      PARAMETER          ( MAXIT = 50 )
      DOUBLE PRECISION   ZERO, HALF, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, HALF = 0.5D+0, ONE = 1.0D+0,
     $                     TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          EQUED
      INTEGER            I, IAF, IB, IBR, IC, IFR, IJ, IJ1, IJ2, INFO2,
     $                   IR, ISCL, ITAU, ITER, IU, IV, IVS, IWRK, J, JI,
     $                   LWA, LWAMAX, MINWRK, N2
      DOUBLE PRECISION   CNORM, CNORM2, CONV, DNORM, DNORM2, EPS, HNORM,
     $                   HINNRM, SCALE, TEMP, TOL
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGEQP3, DGESVX, DLASCL, DLASET,
     $                   DORMQR, DSCAL, DSYTRF, DSYTRI, RICCFR, RICCRC,
     $                   XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      END IF
*
*     Set tol
*
      EPS = DLAMCH( 'Epsilon' )
      TOL = 10.0D+0*DBLE(N)*EPS
*
*     Compute workspace
*
      MINWRK = 9*N*N + 7*N + 1
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICCMS', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
*
*     Compute the norms of the matrices C and D
*
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK )
*
      N2 = 2*N
*
*     Construct the block-permuted Hamiltonian matrix
*
      DO 20 J = 1, N
         DO 10 I = 1, N
            IF( .NOT.LOWER ) THEN
               IJ = (N + J - 1 )*N2 + I
               IF( NOTRNA ) THEN
                  WORK( IJ ) = -A( J, I )
               ELSE
                  WORK( IJ ) = -A( I, J )
               END IF
            ELSE   
               IJ = ( J - 1 )*N2 + N + I
               IF( NOTRNA ) THEN
                  WORK( IJ ) = -A( I, J )
               ELSE
                  WORK( IJ ) = -A( J, I )
               END IF
            END IF   
            IJ = ( J - 1 )*N2 + I         
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) WORK( IJ ) = -C( I, J )
            ELSE   
               IF( I.GE.J ) WORK( IJ ) = -C( I, J )
            END IF
            IJ = ( N + J - 1 )*N2 + N + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) WORK( IJ ) = D( I, J )
            ELSE
               IF( I.GE.J ) WORK( IJ ) = D( I, J )
            END IF
   10    CONTINUE
   20 CONTINUE
*
*     Block-scaling     
*
      CNORM2 = SQRT( CNORM )
      DNORM2 = SQRT( DNORM )
      ISCL = 0
      IF( CNORM2.GT.DNORM2 .AND. DNORM2.GT.ZERO ) THEN
         CALL DLASCL( UPLO, 0, 0, CNORM2, DNORM2, N, N, WORK, N2,
     $                INFO2 )
         CALL DLASCL( UPLO, 0, 0, DNORM2, CNORM2, N, N,
     $                WORK( N2*N+N+1 ), N2, INFO2 )
         ISCL = 1
      END IF
*
*     Workspace usage
*     
      IVS = N2*N2
      ITAU = IVS + N2*N2
      IWRK = ITAU + N2
*
*     Compute the matrix sign function
*
      CALL DCOPY( N2*N2, WORK, 1, WORK( IVS+1 ), 1 )
      LWAMAX = 0
*
      DO 50 ITER = 1, MAXIT
*
*        Store the norm of the Hamiltonian matrix
*
         HNORM = DLANSY( 'F', UPLO, N2, WORK, N2, WORK )
*
*        Compute the inverse of the block-permuted Hamiltonian matrix
*  
         CALL DSYTRF( UPLO, N2, WORK( IVS+1 ), N2, IWORK,
     $                WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         LWA = INT( WORK( IWRK+1 ) )         
         LWAMAX = MAX( LWA, LWAMAX )  
         CALL DSYTRI( UPLO, N2, WORK( IVS+1 ), N2, IWORK,
     $                WORK( IWRK+1 ), INFO2 )         
*
*        Block-permutation of the inverse matrix
*
         DO 40 J = 1, N
            DO 30 I = 1, N
               IJ1 = IVS + ( J - 1 )*N2 + I
               IJ2 = IVS + ( N + J - 1 )*N2 + N + I
               IF( .NOT.LOWER ) THEN
                  IF( I.LE.J ) THEN
                     TEMP = WORK( IJ1 )
                     WORK( IJ1 ) = -WORK( IJ2 )
                     WORK( IJ2 ) = -TEMP
                  END IF
               ELSE
                  IF( I.GE.J ) THEN 
                     TEMP = WORK( IJ1 )
                     WORK( IJ1 ) = -WORK( IJ2 )
                     WORK( IJ2 ) = -TEMP
                  END IF
               END IF 
               IF( .NOT.LOWER ) THEN
                  IF( I.LT.J ) THEN 
                     IJ1 = IVS + ( N + J - 1 )*N2 + I
                     IJ2 = IVS + ( N + I - 1 )*N2 + J
                     TEMP = WORK( IJ1 )
                     WORK( IJ1 ) = WORK( IJ2 )
                     WORK( IJ2 ) = TEMP
                  END IF   
               ELSE
                  IF( I.GT.J ) THEN
                     IJ1 = IVS + ( J - 1 )*N2 + N + I
                     IJ2 = IVS + ( I - 1 )*N2 + N + J
                     TEMP = WORK( IJ1 )
                     WORK( IJ1 ) = WORK( IJ2 )
                     WORK( IJ2 ) = TEMP
                  END IF
               END IF      
   30       CONTINUE
   40    CONTINUE
*
*        Scale the Hamiltonian matrix and its inverse
*                       
         HINNRM = DLANSY( 'F', UPLO, N2, WORK( IVS+1 ), N2, WORK )
         SCALE = SQRT( HINNRM/HNORM )
         CALL DSCAL( N2*N2, ONE/SCALE, WORK( IVS+1 ), 1 )
*
*        Compute the next iteration
*
         CALL DAXPY( N2*N2, SCALE, WORK, 1, WORK( IVS+1 ), 1 )
         CALL DSCAL( N2*N2, HALF, WORK( IVS+1 ), 1 )
         CALL DAXPY( N2*N2, -ONE, WORK( IVS+1 ), 1, WORK, 1 )
*
*        Test for convergence
*
         CONV = DLANSY( 'F', UPLO, N2, WORK, N2, WORK )
         IF( CONV.LE.TOL*HNORM ) GO TO 60
         CALL DCOPY( N2*N2, WORK( IVS+1 ), 1, WORK, 1 )
   50 CONTINUE   
      IF( CONV.GT.TOL*HNORM ) THEN
         INFO = 2
      END IF   
   60 DO 80 J = 1, N2         
         DO 70 I = 1, N2
            IJ = IVS + ( J - 1 )*N2 + I
            JI = IVS + ( I - 1 )*N2 + J
            IF( .NOT.LOWER ) THEN
               IF( I.LT.J ) WORK( JI ) = WORK( IJ )
            ELSE
               IF( I.GT.J ) WORK( JI ) = WORK( IJ )
            END IF
   70    CONTINUE
   80 CONTINUE      
*
*     Back block-permutation
*
      DO 100 J = 1, N
         DO 90 I = 1, N
            IJ1 = IVS + ( J - 1 )*N2 + I
            IJ2 = IVS + ( J - 1 )*N2 + N + I
            TEMP = WORK( IJ1 )
            WORK( IJ1 ) = -WORK( IJ2 )
            WORK( IJ2 ) = TEMP
            IJ1 = IVS + ( N + J - 1 )*N2 + I
            IJ2 = IVS + ( N + J - 1 )*N2 + N + I
            TEMP = WORK( IJ1 )
            WORK( IJ1 ) = -WORK( IJ2 )
            WORK( IJ2 ) = TEMP
   90    CONTINUE
  100 CONTINUE  
*              
*     Compute the QR decomposition of the projector onto the stable
*     invariant subspace
* 
      CALL DLASET( 'F', N2, N2, ZERO, ONE, WORK, N2 )
      CALL DAXPY( N2*N2, -ONE, WORK( IVS+1 ), 1, WORK, 1 )
      CALL DSCAL( N2*N2, HALF, WORK, 1 )
      DO 110 I = 1, N2
         IWORK( I ) = 0
  110 CONTINUE     
      CALL DGEQP3( N2, N2, WORK, N2, IWORK, WORK( ITAU+1 ), 
     $             WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
      LWA = INT( WORK( IWRK+1 ) )         
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Accumulate the orthogonal transformations
* 
      CALL DLASET( 'F', N2, N, ZERO, ONE, WORK( IVS+1 ), N2 )
      CALL DORMQR( 'L', 'N', N2, N, N, WORK, N2, WORK( ITAU+1 ),
     $             WORK( IVS+1 ), N2, WORK( IWRK+1 ), LWORK-IWRK,
     $             INFO2 )
      LWA = INT( WORK( IWRK+1 ) )         
      LWAMAX = MAX( LWA, LWAMAX )  
*
*     Store the matrices V11 and V21
*
      DO 130 J = 1, N
         DO 120 I = 1, N
            IJ = ( J - 1 )*N + I
            IV = ( I - 1 )*N2 + IVS + J
            WORK( IJ ) = WORK( IV )
            IJ = ( J - 1 )*N + 2*N*N + I
            IV = ( I - 1 )*N2 + IVS + N + J
            WORK( IJ ) = WORK( IV )
  120    CONTINUE
  130 CONTINUE
*
*     Workspace usage
*
      IAF = N*N  
      IB = IAF + N*N
      IR = IB + N*N
      IC = IR + N
      IFR = IC + N
      IBR = IFR + N
      IWRK = IBR + N
* 
*     Compute the solution matrix X
*
      CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N,
     $             IWORK, EQUED, WORK( IR+1 ), WORK( IC+1 ),
     $             WORK( IB+1 ), N, X, LDX, RCOND, WORK( IFR+1 ),
     $             WORK( IBR+1 ), WORK( IWRK+1 ), IWORK( N+1 ), 
     $             INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 3
         RETURN
      END IF
*
*     Symmetrize the solution
*
      IF( N.GT.1 ) THEN
         DO 150 I = 1, N - 1
            DO 140 J = I + 1, N
               TEMP = ( X( I, J ) + X( J, I ) ) / TWO
               X( I, J ) = TEMP
               X( J, I ) = TEMP
  140       CONTINUE
  150    CONTINUE
      END IF
*
*     Undo scaling for the solution matrix
*
      IF( ISCL.EQ.1 )
     $   CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, X, LDX, INFO2 )
*
*     Workspace usage
*
      LWA = 2*N*N
      IU = N*N
      IWRK = IU + N*N
*
*     Estimate the reciprocal condition number
*
      CALL RICCRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             RCOND, WORK, N, WORK( IU+1 ), N, WR, WI,
     $             WORK( IWRK+1 ), LWORK-IWRK, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
          FERR = ONE
          WORK( 1 ) = DBLE( LWAMAX )
          RETURN
      END IF
*
*     Estimate the bound on the forward error
*
      CALL RICCFR( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             WORK, N, WORK( IU+1 ), N, FERR, WORK( IWRK+1 ),
     $             LWORK-IWRK, IWORK, INFO2 )
      LWA = 9*N*N
      LWAMAX = MAX( LWA, LWAMAX )
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of RICCMS
*
      END
      SUBROUTINE RICCRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   RCOND, T, LDT, U, LDU, WR, WI, WORK, LWORK,
     $                   IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),
     $                   T( LDT, * ), U( LDU, * ), WI( * ), WORK( * ),
     $                   WR( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  RICCRC estimates the reciprocal of the condition number of the matrix
*  algebraic Riccati equation
*
*  transpose(op(A))*X + X*op(A) + C - X*D*X = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C, D and X are N-by-N.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number 
*          of the Riccati equation.
*          If X = 0, RCOND is set to zero.
*
*  T       (output) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical form
*          from the Schur factorization of the matrix Ac = A - D*X
*          (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T' or 'C').
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (output) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal N-by-N matrix from the real Schur
*          factorization of the matrix Ac = A - D*X (if TRANA = 'N')
*          or Ac = A - X*D (if TRANA = 'T' or 'C').
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, WR(1:N) and WI(1:N) contain the real and imaginary
*          parts, respectively, of the eigenvalues of Ac = A - D*X (if
*          TRANA = 'N') or Ac = A - X*D (if TRANA = 'T' or 'C').
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   INTEGER
*          The dimension of the array WORK. LWORK >= 3*N*N + max(1,3*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix Ac can not be reduced to Schur canonical
*               form and condition number estimate is not computed
*
*  Further details
*  ===============
*
*  The condition number of the Riccati equation is estimated as
*
*  cond = ( norm(Theta)*norm(A) + norm(inv(Omega))*norm(C) + 
*               norm(Pi)*norm(D) ) / norm(X)
*
*  where Omega, Theta and Pi are linear operators defined by
*
*  Omega(Z) = transpose(op(Ac))*Z + Z*op(Ac),
*  Theta(Z) = inv(Omega(transpose(op(Z))*X + X*op(Z))),
*  Pi(Z) = inv(Omega(X*Z*X))
*
*  and Ac = A - D*X (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T' or
*  'C').
*
*  The program estimates the quantities
*
*  sep(op(Ac),-transpose(op(Ac)) = 1 / norm(inv(Omega)),
*
*  norm(Theta) and norm(Pi) using 1-norm condition estimator.
*
*  References
*  ==========
*
*  [1] A.R. Ghavimi and A.J. Laub. Backward error, sensitivity, and
*      refinment of computed solutions of algebraic Riccati equations.
*      Numerical Linear Algebra with Applications, vol. 2, pp. 29-49, 
*      1995.
*  [2] P.Hr. Petkov, M.M. Konstantinov, and V. Mehrmann. DGRSVX and 
*      DMSRIC: Fortan 77 subroutines for solving continuous-time matrix
*      algebraic Riccati equations with condition and accuracy 
*      estimates. Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
*      Chemnitz, May 1998. 
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA, VOIDDUMMY
      CHARACTER          TRANAT 
      INTEGER            I, IDLC, IJ, INFO2, ITMP, IWRK, J, KASE, LWA,    
     $                   MINWRK, SDIM
      DOUBLE PRECISION   ANORM, CNORM, DNORM, EST, PINORM, SCALE, SEP, 
     $                   THNORM, XNORM
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANGE, DLANSY
      EXTERNAL           DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DLACON, DLACPY, DSYMM, DSYR2K,
     $                   LYPCTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -16
      END IF
*
*     Compute workspace
*
      MINWRK = 3*N*N + MAX( 1, 3*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -20 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICCRC', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
         RCOND = ZERO
         RETURN
      END IF
*
*     Compute the norms of the matrices A, C and D
*
      ANORM = DLANGE( '1', N, N, A, LDA, WORK )
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
*     Workspace usage
*
      LWA = 3*N*N
      IDLC = N*N
      ITMP = IDLC + N*N
      IWRK = ITMP + N*N
*
      CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
      IF( NOTRNA ) THEN
*
*        Compute Ac = A - D*X
*
         CALL DSYMM( 'L', UPLO, N, N, -ONE, D, LDD, X, LDX, ONE,
     $               T, LDT )
      ELSE
*
*        Compute Ac = A - X*D
*
         CALL DSYMM( 'R', UPLO, N, N, -ONE, D, LDD, X, LDX, ONE,
     $               T, LDT )
      END IF
*
*     Compute the Schur factorization of Ac 
*
      CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $     WR, WI, U, LDU,
     $     WORK( IWRK+1 ), LWORK-IWRK, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
*
*     Estimate sep(op(Ac),-transpose(Ac)) 
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
* 
      EST = ZERO
      KASE = 0
   10 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 30 J = 1, N
               DO 20 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   20          CONTINUE
   30       CONTINUE
         ELSE
            DO 50 J = 1, N
               DO 40 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   40          CONTINUE
   50       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U 
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, 
     $               ZERO, WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale*RHS 
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U' 
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, 
     $               ZERO, WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 70 J = 1, N
               DO 60 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   60          CONTINUE
   70       CONTINUE
         ELSE
            DO 90 J = 1, N
               DO 80 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
   80          CONTINUE
   90       CONTINUE
         END IF
         GO TO 10
      END IF
*
      SEP = SCALE / TWO / EST
*
*     Return if the equation is singular
*
      IF( SEP.EQ.ZERO ) THEN 
         RCOND = ZERO
         RETURN
      END IF
*
*     Estimate norm(Theta)
*
      EST = ZERO
      KASE = 0
  100 CONTINUE
      CALL DLACON( N*N, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )  
      IF( KASE.NE.0 ) THEN
*
*        Compute RHS = op(W')*X + X*op(W)
*
         CALL DSYR2K( UPLO, TRANAT, N, N, ONE, WORK, N, X, LDX, ZERO,  
     $                WORK( ITMP+1 ), N ) 
         CALL DLACPY( UPLO, N, N, WORK( ITMP+1 ), N, WORK, N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK, N, U, LDU, 
     $               ZERO, WORK( ITMP+1 ), N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, 
     $               WORK( ITMP+1 ), N, ZERO, WORK, N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(Ac')*Y + Y*op(Ac) = scale*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK, N, SCALE, 
     $                   INFO2 )
         ELSE
*
*           Solve op(Ac)*Z + Z*op(Ac') = scale*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK, N, SCALE, 
     $                   INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U' 
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK, N, U, LDU, 
     $               ZERO, WORK( ITMP+1 ), N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         GO TO 100
      END IF
*
      THNORM = EST / SCALE
*
*     Estimate norm(Pi)
*
      EST = ZERO
      KASE = 0
  110 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 130 J = 1, N
               DO 120 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
  120          CONTINUE
  130       CONTINUE
         ELSE
            DO 150 J = 1, N
               DO 140 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
  140          CONTINUE
  150       CONTINUE
         END IF
*
*        Compute RHS = X*W*X
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, X, LDX, 
     $               ZERO, WORK, N )
         CALL DSYMM( 'R', UPLO, N, N, ONE, X, LDX, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, 
     $               WORK, N, ZERO, WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(Ac')*Y + Y*op(Ac) = scale*RHS
*
            CALL LYPCTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         ELSE
*
*           Solve op(Ac)*Z + Z*op(Ac') = scale*RHS
*
            CALL LYPCTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, 
     $                   SCALE, INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U' .
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, 
     $               U, LDU, ZERO, WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 170 J = 1, N
               DO 160 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  160          CONTINUE
  170       CONTINUE
         ELSE
            DO 190 J = 1, N
               DO 180 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  180          CONTINUE
  190       CONTINUE
         END IF
         GO TO 110
      END IF
*
      PINORM =  TWO*EST / SCALE
*
*     Estimate the reciprocal condition number
*
      RCOND = SEP*XNORM / ( CNORM + SEP*( THNORM*ANORM + 
     $        PINORM*DNORM ) )
      IF( RCOND.GT.ONE ) RCOND = ONE
*
      WORK( 1 ) = DBLE( LWA )
      RETURN
*
*     End of RICCRC
*
      END
      SUBROUTINE RICCSL( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK,
     $                   INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND
*     ..
*     .. Array Arguments ..
      LOGICAL            BWORK( * )
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   X( LDX, * ), WI( * ), WORK( * ), WR( * ) 
*     ..
*
*  Purpose
*  =======
*
*  RICCSL solves the matrix algebraic Riccati equation
*
*     transpose(op(A))*X + X*op(A) + C - X*D*X = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C and D are N-by-N and the solution X is N-by-N.
*
*  Error bound on the solution and a condition estimate are also
*  provided.
*
*  It is assumed that the matrices A, C and D are such that the
*  corresponding Hamiltonian matrix has N eigenvalues with negative
*  real parts.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WR(1:N) and WI(1:N) contain the real
*          and imaginary parts, respectively, of the eigenvalues of
*          Ac = A - D*X (if TRANA = 'N') or Ac = A - X*D (if TRANA = 'T'
*          or 'C').
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number of
*          the Riccati equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 9*N*N + 4*N + 
*          max(1,6*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  BWORK   (workspace) LOGICAL array, dimension (2*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the QR algorithm failed to compute the eigenvalues of 
*               the Hamiltonian matrix
*          = 2: the eigenvalues of the Hamiltonian matrix could not be
*               reordered because some eigenvalues were too close to
*               separate
*          = 3: after reordering, roundoff changed values of some
*               complex eigenvalues so that leading eigenvalues in 
*               the Schur form have no longer negative real parts
*          = 4: the Hamiltonian matrix has less than N eigenvalues
*               with negative real parts
*          = 5: the system of linear equations for the solution is
*               singular to working precision
*          = 6: the matrix A-D*X (or A-X*D) can not be reduced to Schur
*               canonical form and condition number estimate and
*               forward error estimate are not computed
*
*  Further Details
*  ===============
*
*  The matrix Riccati equation is solved by the Schur method [1].
*
*  The condition number of the equation is estimated using 1-norm
*  estimator.
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [3].
*
*  References 
*  ==========
*
*  [1] A.J. Laub. A Schur method for solving algebraic Riccati
*      equations. IEEE Trans. Autom. Control, vol. 24, pp. 913-921,
*      1979. 
*  [2] A.R. Ghavimi and A.J. Laub. Backward error, sensitivity, and
*      refinment of computed solutions of algebraic Riccati equations.
*      Numerical Linear Algebra with Applications, vol. 2, pp. 29-49, 
*      1995.
*  [3] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [4] P.Hr. Petkov, M.M. Konstantinov, and V. Mehrmann. DGRSVX and 
*      DMSRIC: Fortan 77 subroutines for solving continuous-time matrix
*      algebraic Riccati equations with condition and accuracy 
*      estimates. Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
*      Chemnitz, May 1998. 
* 
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          EQUED 
      INTEGER            I, IAF, IB, IBR, IC, IFR, IJ, INFO2, IR, ISCL,
     $                   IU, IV, IVS, IWI, IWR, IWRK, J, LWA, LWAMAX,
     $                   MINWRK, N2, SDIM
      DOUBLE PRECISION   CNORM, CNORM2, DNORM, DNORM2, TEMP
*     ..
*     .. External Functions ..
      LOGICAL            LSAME, SELNEG
      DOUBLE PRECISION   DLANSY
      EXTERNAL           DLANSY, LSAME, SELNEG
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGESVX, DLASCL, RICCFR, RICCRC, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      END IF
*
*     Compute workspace
*
      MINWRK = 9*N*N + 4*N + MAX( 1, 6*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICCSL', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
*
*     Compute the norms of the matrices C and D
*
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
      N2 = 2*N
*
*     Construct the Hamiltonian matrix
*
      DO 20 J = 1, N
         DO 10 I = 1, N
            IJ = ( J - 1 )*N2 + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = A( I, J )
            ELSE
               WORK( IJ ) = A( J, I )
            END IF
            IJ = ( J - 1 )*N2 + N + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            END IF
            IJ = ( N + J - 1 )*N2 + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = -D( I, J )
               ELSE
                  WORK( IJ ) = -D( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = -D( I, J )
               ELSE
                  WORK( IJ ) = -D( J, I )
               END IF
            END IF
            IJ = ( N + J - 1)*N2 + N + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = -A( J, I )
            ELSE
               WORK( IJ ) = -A( I, J )
            END IF
   10    CONTINUE
   20 CONTINUE
*
*     Scale the Hamiltonian matrix     
*
      CNORM2 = SQRT( CNORM )
      DNORM2 = SQRT( DNORM )
      ISCL = 0
      IF( CNORM2.GT.DNORM2 .AND. DNORM2.GT.ZERO ) THEN
         CALL DLASCL( 'G', 0, 0, CNORM2, DNORM2, N, N, WORK( N+1 ), N2, 
     $                INFO2 ) 
         CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, WORK( N2*N+1 ), 
     $                N2, INFO2 )
         ISCL = 1
      END IF
*      
*     Workspace usage
*
      LWA = 8*N*N + 4*N
      IWR = N2*N2
      IWI = IWR + N2
      IVS = IWI + N2
      IWRK = IVS + N2*N2
*
*     Compute the Schur factorization of the Hamiltonian matrix
*
      CALL DGEES( 'V', 'S', SELNEG, N2, WORK, N2, SDIM,
     $             WORK( IWR+1 ), WORK( IWI+1 ), WORK( IVS+1 ),
     $             N2, WORK( IWRK+1 ), LWORK-IWRK, BWORK, INFO2 )
      IF( INFO2.GT.0 .AND. INFO2.LE.N2 ) THEN
         INFO = 1
         RETURN
      ELSE IF( INFO2.EQ.N2+1 ) THEN
         INFO = 2
         RETURN
      ELSE IF( INFO2.EQ.N2+2 ) THEN
         INFO = 3
         RETURN
      ELSE IF( SDIM.NE.N ) THEN
         INFO = 4
         RETURN
      END IF
      LWAMAX = LWA + INT( WORK( IWRK+1 ) )
*
*     Store the matrices V11 and V21
*
      DO 40 J = 1, N
         DO 30 I = 1, N
            IJ = ( J - 1 )*N + I
            IV = ( I - 1 )*N2 + IVS + J
            WORK( IJ ) = WORK( IV )
            IJ = ( J - 1 )*N + 2*N*N + I
            IV = ( I - 1 )*N2 + IVS + N + J
            WORK( IJ ) = WORK( IV )
   30    CONTINUE
   40 CONTINUE
*
*     Workspace usage
*
      IAF = N*N  
      IB = IAF + N*N
      IR = IB + N*N
      IC = IR + N
      IFR = IC + N
      IBR = IFR + N
      IWRK = IBR + N
* 
*     Compute the solution matrix X
*
      CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N,
     $             IWORK, EQUED, WORK( IR+1 ), WORK( IC+1 ),
     $             WORK( IB+1 ), N, X, LDX, RCOND, WORK( IFR+1 ),
     $             WORK( IBR+1 ), WORK( IWRK+1 ), IWORK( N+1 ), 
     $             INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
*
*     Symmetrize the solution
*
      IF( N.GT.1 ) THEN
         DO 60 I = 1, N - 1
            DO 50 J = I + 1, N
               TEMP = ( X( I, J ) + X( J, I ) ) / TWO
               X( I, J ) = TEMP
               X( J, I ) = TEMP
   50       CONTINUE
   60    CONTINUE
      END IF
*
*     Undo scaling for the solution matrix
*
      IF( ISCL.EQ.1 )
     $   CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, X, LDX, INFO2 )
*
*     Workspace usage
*
      LWA = 2*N*N
      IU = N*N
      IWRK = IU + N*N
*
*     Estimate the reciprocal condition number
*
      CALL RICCRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             RCOND, WORK, N, WORK( IU+1 ), N, WR, WI,
     $             WORK( IWRK+1 ), LWORK-IWRK, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 6
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
          FERR = ONE
          WORK( 1 ) = DBLE( LWAMAX )
          RETURN
      END IF
*
*     Estimate the bound on the forward error
*
      CALL RICCFR( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             WORK, N, WORK( IU+1 ), N, FERR, WORK( IWRK+1 ),
     $             LWORK-IWRK, IWORK, INFO2 )
      LWA = 9*N*N
      LWAMAX = MAX( LWA, LWAMAX )
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of RICCSL
*
      END

      LOGICAL FUNCTION SELNEG( WR, WI )
*
*  -- LISPACK auxiliary routine (version 3.0) --
*     Tech. University of Sofia
*     July 5, 1999
*
*     .. Scalar Arguments ..
      DOUBLE PRECISION WR, WI
*     ..
*
*     Purpose
*     =======
*
*     SELNEG is used to select eigenvalues with negative real parts
*     to sort to the top left of the Schur form of the Hamiltonian
*     matrix in solving matrix algebraic Riccati equations
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D+0 )
*
      IF( WR.LT.ZERO ) THEN
         SELNEG = .TRUE.
      ELSE
         SELNEG = .FALSE.
      END IF
*
*     End of SELNEG
*
      END
      SUBROUTINE RICDFR( TRANA, N, A, LDA, UPLO, C, LDC, X, LDX, AC, 
     $                   LDAC, T, LDT, U, LDU, WFERR, FERR, WORK, LWORK,
     $                   IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDAC, LDC, LDT, LDU, LDX, LWORK, N
      DOUBLE PRECISION   FERR
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), AC( LDAC, * ), C( LDC, * ),
     $                   T( LDT, * ), U( LDU, * ), WFERR( * ),
     $                   WORK( * ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  RICDFR estimates the forward error bound for the computed solution of
*  the discrete-time matrix algebraic Riccati equation
*                               -1          
*  transpose(op(A))*X*(In + D*X)  *op(A) - X + C = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C, D and X are N-by-N.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangle of C is stored;
*          = 'L':  Lower triangle of C is stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDC >= max(1,N).
*
*  AC      (input) DOUBLE PRECISION array, dimension (LDAC,N)
*                                   -1
*          The matrix Ac = (I + D*X)  *A (if TRANA = 'N') or
*                          -1
*          Ac = A*(I + X*D)  (if TRANA = 'T' or 'C').
*
*  LDAC    (input) INTEGER
*          The leading dimension of the array AC. LDAC >= max(1,N).
*
*  T       (input) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical form
*          from the Schur factorization of the matrix Ac.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (input) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal N-by-N matrix from the real Schur
*          factorization of the matrix Ac.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  WFERR   (input) DOUBLE PRECISION array, dimension (N)
*          The vector of estimated forward error bound for each column
*          of the matrix Ac, as obtained by the subroutine RICDRC.
*
*  FERR    (output) DOUBLE PRECISION
*          The estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*
*  LWORK   INTEGER
*          The dimension of the array WORK. LWORK >= 7*N*N + 2*N
*
*  IWORK   (workspace) INTEGER array, dimension (N*N)
*
*  INFO    INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*
*  Further details
*  ===============
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [1].
*
*  References
*  ==========
*
*  [1] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, FOUR
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0,
     $                     FOUR = 4.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          TRANAT 
      INTEGER            I, IABS, IDLC, IJ, INFO2, IRES, ITMP, IWRK,
     $                   IXBS, IXMA, J, KASE, MINWRK
      DOUBLE PRECISION   ACJMAX, EPS, EST, SCALE, XNORM
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEMM, DLACON, DSYMM, LYPDTR, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -9
      ELSE IF( LDAC.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN
         INFO = -13
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN
         INFO = -15
      END IF
*
*     Get the machine precision
*
      EPS = DLAMCH( 'Epsilon' )
*
*     Compute workspace
*
      MINWRK = 7*N*N + 2*N
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -19 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICDFR', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN 
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
         FERR = ZERO
         RETURN
      END IF
*
*     Workspace usage
*
      IDLC = N*N
      ITMP = IDLC + N*N
      IXMA = ITMP + N*N
      IABS = IXMA + N*N
      IXBS = IABS + N*N
      IRES = IXBS + N*N
      IWRK = IRES + N*N
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
*
*     Form residual matrix R = transpose(op(A))*X*op(Ac) + C - X
*
      CALL DGEMM( TRANAT, 'N', N, N, N, ONE, A, LDA, X, LDX, ZERO,
     $            WORK( IXMA+1 ), N )
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, WORK( IXMA+1 ), N,
     $            AC, LDAC, ZERO, WORK( ITMP+1 ), N )
      IF( LOWER ) THEN
         DO 20 J = 1, N
            DO 10 I = J, N
               WORK( IRES+I+(J-1)*N ) = C( I, J ) - X( I, J ) +
     $                      WORK( ITMP+I+(J-1)*N )
   10       CONTINUE
   20    CONTINUE
      ELSE
         DO 40 J = 1, N
            DO 30 I = 1, J
               WORK( IRES+I+(J-1)*N ) = C( I, J ) - X( I, J ) +
     $                      WORK( ITMP+I+(J-1)*N )                
   30       CONTINUE
   40    CONTINUE
      END IF
*
*     Add to abs(R) a term that takes account of rounding errors in 
*     forming R: 
*       abs(R) := abs(R) + EPS*( 4*abs(C) + 4*abs(X) +
*                 (2*n+3)*abs(op(A'))*abs(X)*abs(op(Ac) +
*                 2*(n+1)*abs(op(A'))*abs(X)*abs(op(DAc) )
*     where EPS is the machine precision and DAc is a bound on the
*     absolute error in computing the matrix Ac
*
      DO 60 J = 1, N      
         DO 50 I = 1, N
            IJ = I + ( J - 1 )*N
            WORK( IABS+IJ ) = ABS( A( I, J ) )
            WORK( IXBS+IJ ) = ABS( X( I, J ) )
            WORK( IDLC+IJ ) = ABS( AC( I, J ) )
   50    CONTINUE         
   60 CONTINUE
      CALL DGEMM( TRANAT, 'N', N, N, N, ONE, WORK( IABS+1 ), N,
     $            WORK( IXBS+1 ), N, ZERO, WORK( IXMA+1 ), N )
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, WORK( IXMA+1 ), N,
     $            WORK( IDLC+1 ), N, ZERO, WORK( ITMP+1 ), N )
      DO 80 J = 1, N
         ACJMAX = DLANGE( 'M', N, 1, AC( 1, J ), LDAC, WORK )
         DO 70 I = 1, N
            WORK( IABS+I+(J-1)*N ) = ACJMAX*WFERR( J )
   70    CONTINUE
   80 CONTINUE
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, WORK( IXMA+1 ), N,
     $            WORK( IABS+1 ), N, ZERO, WORK( IDLC+1 ), N )
      IF( LOWER ) THEN
         DO 100 J = 1, N
            DO 90 I = J, N
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               FOUR*EPS*( ABS( C( I, J ) ) + ABS( X( I, J ) ) ) +
     $               DBLE( 2*N + 3 )*EPS*WORK( ITMP+I+(J-1)*N  ) +
     $               DBLE( 2*N + 2 )*WORK( IDLC+1 )
   90       CONTINUE
  100    CONTINUE
      ELSE
         DO 120 J = 1, N
            DO 110 I = 1, J
               WORK( IRES+I+(J-1)*N ) =  ABS( WORK( IRES+I+(J-1)*N ) ) + 
     $               FOUR*EPS*( ABS( C( I, J ) ) + ABS( X( I, J ) ) ) +
     $               DBLE( 2*N + 3 )*EPS*WORK( ITMP+I+(J-1)*N  ) +
     $               DBLE( 2*N + 2 )*WORK( IDLC+1 )
  110       CONTINUE
  120    CONTINUE
      END IF
*
*     Compute forward error bound, using matrix norm estimator
*
      EST = ZERO
      KASE = 0
  130 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
         IJ = 0
         IF( LOWER ) THEN
            DO 150 J = 1, N
               DO 140 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the lower triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )  
                  END IF
  140          CONTINUE
  150       CONTINUE
         ELSE
            DO 170 J = 1, N
               DO 160 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Scale by the residual matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )*
     $                     WORK( IRES+I+(J-1)*N )
                  ELSE
*
*                    Unpack the upper triangular part of symmetric 
*                    matrix
*
                     WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
                  END IF
  160          CONTINUE
  170       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, 
     $               WORK, N, ZERO, WORK( ITMP+1 ), N )
         IF( KASE.EQ.2 ) THEN
*
*           Solve op(A')*Y + Y*op(A) = scale*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE, 
     $                   WORK( IWRK+1 ), INFO2 )
         ELSE
*
*           Solve op(A)*Z + Z*op(A') = scale*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, 
     $               U, LDU, ZERO, WORK( ITMP+1 ), N )
         IJ = 0
         IF( LOWER ) THEN
            DO 190 J = 1, N
               DO 180 I = J, N
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the lower triangular part of symmetric
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  180          CONTINUE
  190       CONTINUE
         ELSE
            DO 210 J = 1, N
               DO 200 I = 1, J
                  IJ = IJ + 1
                  IF( KASE.EQ.2 ) THEN
*
*                    Pack the upper triangular part of symmetric  
*                    matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
                  ELSE
*
*                    Scale by the residual matrix
*
                     WORK( IJ ) = WORK( ITMP+I+(J-1)*N )*
     $                     WORK( IRES+I+(J-1)*N )
                  END IF
  200          CONTINUE
  210       CONTINUE
         END IF
         GO TO 130
      END IF
*
*     Compute the estimate of the forward error 
*
      FERR =  TWO*EST / DLANSY( 'Max', UPLO, N, X, LDX, WORK ) / SCALE
      IF( FERR.GT.ONE ) FERR = ONE
*
      RETURN
*
*     End of RICDFR
*
      END
      SUBROUTINE RICDMF( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   WR, WI, RCOND, FERR, WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   X( LDX, * ), WI( * ), WORK( * ), WR( * ) 
*     ..
*
*  Purpose
*  =======
*
*  RICDMF solves the discrete-time matrix algebraic Riccati equation
*                               -1          
*  transpose(op(A))*X*(In + D*X)  *op(A) - X + C = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C and D are N-by-N and the solution X is N-by-N.
*
*  Error bound on the solution and a condition estimate are also
*  provided.
*
*  It is assumed that the matrices A, C and D are such that the
*  corresponding matrix pencil has N eigenvalues with moduli
*  less than one.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WR(1:N) and WI(1:N) contain the real
*          and imaginary parts, respectively, of the eigenvalues of
*                        -1                                      -1
*          Ac = (I + D*X)  *A (if TRANA = 'N') or Ac = A*(I + X*D)
*          (if TRANA = 'T' or 'C').
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number of
*          the discrete-time Riccati equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 28*N*N + 2*N + 
*          max(1,2*N). 
*          For optimum performance LWORK >= 28*N*N + 2*N + ( 2*N+1 )*NB,
*          where NB is the optimal blocksize.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the iteration for the spectral decomposition failed to
*               converge after 50 iterations, but an approximate
*               solution and error bounds have been computed
*          = 2: the system of linear equations for the solution is
*               singular to working precision
*                                        -1                      -1
*          = 3: the matrix Ac = (I + D*X)  *A or Ac = A*(I + X*D)
*               can not be reduced to Schur canonical form and condition
*               number estimate and forward error estimate are not
*               computed
*
*  Further Details
*  ===============
*
*  The discrete-time matrix Riccati equation is solved by using the 
*  inverse free spectral decomposition method, proposed in [1].
*
*  The condition number of the equation is estimated using 1-norm
*  estimator.
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [2].
*
*  References 
*  ==========
*
*  [1] Z. Bai, J. Demmel and M. Gu. An inverse free parallel spectral 
*      divide and conquer algorithm for nonsymmetric eigenproblems.
*      Numer. Math., vol. 76, pp. 279-308, 1997. 
*  [2] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [3] M.M. Konstantinov, P.Hr. Petkov, and N.D. Christov. Perturbation
*      analysis of the discrete Riccati equation. Kybernetica (Prague),
*      vol. 29,pp. 18-29, 1993.
* 
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            MAXIT
      PARAMETER          ( MAXIT = 50 )
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          EQUED 
      INTEGER            I, IA, IAC, IAF, IB, IBR, IC, IFR, IJ, IJ1,
     $                   IJ2,INFO2, IQ, IR, IS, ISCL, ITAU, ITER, IU,
     $                   IV, IWFERR, IWRK, J, LWA, LWA0, LWAMAX, MINWRK,
     $                   N2, N4
      DOUBLE PRECISION   CNORM, CNORM2, DNORM, DNORM2, EPS, RDNORM,
     $                   RNORM, TEMP, TOL
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DAXPY, DGEMM, DGEQP3, DGEQRF, DGERQF, DLACPY,
     $                   DLASET, DORMQR, DORMRQ, DGESVX, DLASCL, DSCAL,
     $                   RICDFR, RICDRC, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      END IF
*
*     Set tol
*
      EPS = DLAMCH( 'Epsilon' )
      TOL = 10.0D+0*DBLE(N)*EPS
*
*     Compute workspace
*
      MINWRK = 28*N*N + 2*N + MAX( 1, 2*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICDMF', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
*
*     Compute the norms of the matrices C and D
*
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
      N2 = 2*N
      N4 = 4*N
*
*     Construct B0 and -A0
*
      DO 20 J = 1, N
         DO 10 I = 1, N
            IJ = ( N + J - 1 )*N2 + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = D( I, J )
               ELSE
                  WORK( IJ ) = D( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = D( I, J )
               ELSE
                  WORK( IJ ) = D( J, I )
               END IF
            END IF
            IJ = ( N + J - 1 )*N2 + N + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = A( J, I )
            ELSE
               WORK( IJ ) = A( I, J )
            END IF
            IJ = N2*N2 + ( J - 1 )*N2 + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = -A( I, J )
            ELSE
               WORK( IJ ) = -A( J, I )
            END IF
            IJ = N2*N2 + ( J - 1)*N2 + N + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = C( I, J )
               ELSE
                  WORK( IJ ) = C( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = C( I, J )
               ELSE
                  WORK( IJ ) = C( J, I )
               END IF
            END IF
   10    CONTINUE
   20 CONTINUE
      CALL DLASET( 'Full', N, N, ZERO, ZERO, WORK( N+1 ), N2 )
      CALL DLASET( 'Full', N, N, ZERO, ZERO, WORK( N2*N2+N2*N+1 ), N2 )
      CALL DLASET( 'Full', N, N, ZERO, ONE, WORK, N2 )
      CALL DLASET( 'Full', N, N, ZERO, -ONE, WORK( N2*N2+N2*N+N+1 ),
     $             N2 )
*
*     Scale the matrices B0 and -A0
*
      CNORM2 = SQRT( CNORM )
      DNORM2 = SQRT( DNORM )
      ISCL = 0
      IF( CNORM2.GT.DNORM2 .AND. DNORM2.GT.ZERO ) THEN
         CALL DLASCL( 'G', 0, 0, CNORM2, DNORM2, N, N,
     $                WORK( N2*N2+N+1 ), N2, INFO2 ) 
         CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N,
     $                WORK( N2*N+1 ), N2, INFO2 )
         ISCL = 1
      END IF
*      
*     Workspace usage
*
      LWA0 = 28*N*N + 2*N
      LWAMAX = 0
      IA = N2*N2
      IR = IA + N2*N2
      IS = IR + N4*N2
      IQ = IS + N2*N2
      ITAU = IQ + N4*N2
      IWRK = ITAU + N2
*
*     Copy B0 and -A0
*
      CALL DLACPY( 'F', N2, N2, WORK, N2, WORK( IR+1 ), N4 )
      CALL DLACPY( 'F', N2, N2, WORK( IA+1 ), N2, WORK( IR+N2+1 ), N4 )
*
*     Main iteration loop
*
      DO 60 ITER = 1, MAXIT
*
*                            [ Bj]
*        QR decomposition of [   ]
*                            [-Aj]
*
         CALL DGEQRF( N4, N2, WORK( IR+1 ), N4, WORK( ITAU+1 ),
     $                WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
         LWA = LWA0 + INT( WORK( IWRK+1 ) )
         LWAMAX = MAX( LWA, LWAMAX )
*
*        Make the diagonal elements of Rj positive
*
         DO 30 I = 1, N2
            IF( WORK( IR+(I-1)*N4+I ).LT.ZERO ) 
     $         CALL DSCAL( N2-I+1, -ONE, WORK( IR+(I-1)*N4+I ), N4 )
   30    CONTINUE
         IF( ITER.GT.1 ) THEN
*
*           Compute Rj+1 - Rj
*
            DO 50 J = 1, N2
               DO 40 I = 1, J
                  IJ1 = IR + ( J - 1 )*N4 + I
                  IJ2 = IS + ( J - 1 )*N2 + I
                  WORK( IJ2 ) = WORK( IJ1 ) - WORK( IJ2 )
   40          CONTINUE
   50       CONTINUE
            RDNORM = DLANGE( '1', N2, N2, WORK( IS+1 ), N2,
     $               WORK( IWRK+1 ))
         END IF
*
*        Save Rj for future use
*
         CALL DLACPY( 'U', N2, N2, WORK( IR+1 ), N4, WORK( IS+1 ), N2 )
         IF( ITER.EQ.1 )
     $      CALL DLASET( 'L', N2-1, N2-1, ZERO, ZERO, WORK( IS+2 ), N2 )
*
*        Generate the matrices Q12 and Q22
*
         CALL DLASET( 'F', N2, N2, ZERO, ZERO, WORK( IQ+1 ), N4 )
         CALL DLASET( 'F', N2, N2, ZERO, ONE, WORK( IQ+N2+1 ), N4 )
         CALL DORMQR( 'L', 'N', N4, N2, N2, WORK( IR+1 ), N4,
     $                WORK( ITAU+1 ), WORK( IQ+1 ), N4, WORK( IWRK+1 ),
     $                LWORK-IWRK, INFO2 )
         LWA = LWA0 + INT( WORK( IWRK+1 ) )
         LWAMAX = MAX( LWA, LWAMAX )
*
*        Compute Bj and -Aj
*
         CALL DGEMM( 'T', 'N', N2, N2, N2, ONE, WORK( IQ+N2+1 ),
     $               N4, WORK, N2, ZERO, WORK( IR+1 ), N4 )
         CALL DGEMM( 'T', 'N', N2, N2, N2, ONE, WORK( IQ+1 ), N4,
     $               WORK( IA+1 ), N2, ZERO, WORK( IR+N2+1 ), N4 )
         CALL DLACPY( 'F', N2, N2, WORK( IR+1 ), N4, WORK, N2 )
         CALL DLACPY( 'F', N2, N2, WORK( IR+N2+1 ), N4, WORK( IA+1 ),
     $                N2 )
*
*        Test for convergence
*
         IF( ITER.GT.1 .AND. RDNORM.LE.TOL*RNORM ) GO TO 70
         RNORM = DLANGE( '1', N2, N2, WORK( IS+1 ), N2,
     $                   WORK( IWRK+1 ))
   60 CONTINUE
      INFO = 1
   70 LWA0 =10*N*N + 2*N
      IQ = IA + N2*N2
      ITAU = IQ + N2*N
      IWRK = ITAU + N2
*
*     Compute Ap + Bp
*
      CALL DSCAL( N2*N2, -ONE, WORK( IA+1 ), 1 )
      CALL DAXPY( N2*N2, ONE, WORK, 1, WORK( IA+1 ), 1 )
*
*     QR decomposition with column pivoting of Bp
*
      DO 80 J = 1, N2
         IWORK( J ) = 0
   80 CONTINUE
      CALL DGEQP3( N2, N2, WORK, N2, IWORK, WORK( ITAU+1 ),
     $             WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*               T
*     Compute Q1 (Ap + Bp) 
*
      CALL DORMQR( 'L', 'T', N2, N2, N2, WORK, N2, WORK( ITAU+1 ),
     $             WORK( IA+1 ), N2, WORK( IWRK+1 ), LWORK-IWRK,
     $             INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*                           T
*     RQ decomposition of Q1 (Ap + Bp)
*
      CALL DGERQF( N2, N2, WORK( IA+1 ), N2, WORK( ITAU+1 ),
     $             WORK( IWRK+1 ), LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Generate Q11 and Q21
*
      CALL DLASET( 'F', N, N, ZERO, ONE, WORK( IQ+1 ), N2 )
      CALL DLASET( 'F', N, N, ZERO, ZERO, WORK( IQ+N+1 ), N2 )
      CALL DORMRQ( 'L', 'T', N2, N, N2, WORK( IA+1 ), N2,
     $             WORK( ITAU+1 ), WORK( IQ+1 ), N2, WORK( IWRK+1 ),
     $             LWORK-IWRK, INFO2 )
      LWA = LWA0 + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Store the matrices Q11 and Q21
*
      DO 100 J = 1, N
         DO 90 I = 1, N
            IJ = ( J - 1 )*N + I
            IV = IQ + ( I - 1 )*N2 + J
            WORK( IJ ) = WORK( IV )
            IJ = ( J - 1 )*N + 2*N*N + I
            IV = IQ + ( I - 1 )*N2 + N + J
            WORK( IJ ) = WORK( IV )
   90    CONTINUE
  100 CONTINUE
*
*     Workspace usage
*
      IAF = N*N  
      IB = IAF + N*N
      IR = IB + N*N
      IC = IR + N
      IFR = IC + N
      IBR = IFR + N
      IWRK = IBR + N
* 
*     Compute the solution matrix X
*
      CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N,
     $             IWORK, EQUED, WORK( IR+1 ), WORK( IC+1 ),
     $             WORK( IB+1 ), N, X, LDX, RCOND, WORK( IFR+1 ),
     $             WORK( IBR+1 ), WORK( IWRK+1 ), IWORK( N+1 ), 
     $             INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
*
*     Symmetrize the solution
*
      IF( N.GT.1 ) THEN
         DO 120 I = 1, N - 1
            DO 110 J = I + 1, N
               TEMP = ( X( I, J ) + X( J, I ) ) / TWO
               X( I, J ) = TEMP
               X( J, I ) = TEMP
  110       CONTINUE
  120    CONTINUE
      END IF
*
*     Undo scaling for the solution matrix
*
      IF( ISCL.EQ.1 )
     $    CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, X, LDX, INFO2 )
*
*     Workspace usage
*
      LWA = 3*N*N + N
      IU = N*N
      IWFERR = IU + N*N
      IAC = IWFERR + N
      IWRK = IAC + N*N
*
*     Estimate the reciprocal condition number
*
      CALL RICDRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             RCOND, WORK( IAC+1 ), N, WORK, N, WORK( IU+1 ), N,
     $             WR, WI, WORK( IWFERR+1 ), WORK( IWRK+1 ), LWORK-IWRK,
     $             IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 3
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
          FERR = ONE
          WORK( 1 ) = DBLE( LWAMAX )
          RETURN
      END IF
*
*     Estimate the bound on the forward error
*
      CALL RICDFR( TRANA, N, A, LDA, UPLO, C, LDC, X, LDX,
     $             WORK( IAC+1 ), N, WORK, N, WORK( IU+1 ), N,
     $             WORK( IWFERR+1 ), FERR, WORK( IWRK+1 ),
     $             LWORK-IWRK, IWORK, INFO2 )
      LWA = 9*N*N + 3*N
      LWAMAX = MAX( LWA, LWAMAX )
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of RICDMF
*
      END
      SUBROUTINE RICDRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   RCOND, AC, LDAC, T, LDT, U, LDU, WR, WI, WFERR,
     $                   WORK, LWORK, IWORK, INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDAC, LDC, LDD, LDT, LDU, LDX,
     $                   LWORK, N
      DOUBLE PRECISION   RCOND
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), AC( LDAC, * ), C( LDC, * ),  
     $                   D( LDD, * ), T( LDT, * ), U( LDU, * ),
     $                   WFERR( * ), WI( * ), WORK( * ), WR( * ),
     $                   X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  RICDRC estimates the reciprocal of the condition number of the 
*  discrete-time matrix algebraic Riccati equation
*
*                               -1          
*  transpose(op(A))*X*(In + D*X)  *op(A) - X + C = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C, D and X are N-by-N.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDC >= max(1,N).
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number 
*          of the discrete-time Riccati equation.
*          If X = 0, RCOND is set to zero.
*
*  AC      (output) DOUBLE PRECISION array, dimension (LDAC,N)
*          On exit, if INFO = 0, AC contains the matrix
*                        -1                                      -1
*          Ac = (I + D*X)  *A (if TRANA = 'N') or Ac = A*(I + X*D)
*          (if TRANA = 'T' or 'C').
*
*  LDAC    (input) INTEGER
*          The leading dimension of the array AC. LDAC >= max(1,N).
*
*  T       (output) DOUBLE PRECISION array, dimension (LDT,N)
*          The upper quasi-triangular matrix in Schur canonical form
*          from the Schur factorization of the matrix Ac.
*
*  LDT     (input) INTEGER
*          The leading dimension of the array T. LDT >= max(1,N)
*     
*  U       (output) DOUBLE PRECISION array, dimension (LDU,N)
*          The orthogonal N-by-N matrix from the real Schur
*          factorization of the matrix Ac.
*
*  LDU     (input) INTEGER
*          The leading dimension of the array U. LDU >= max(1,N)
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, WR(1:N) and WI(1:N) contain the real and imaginary
*          parts, respectively, of the eigenvalues of the matrix Ac.
*
*  WFERR   (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WFERR contains the estimated forward
*          error bound for each column of the matrix Ac.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   INTEGER
*          The dimension of the array WORK. LWORK >= 5*N*N + 3*N +
*          max(1,4*N).
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  INFO    INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the matrix I + D*X is singular to working precision
*               and condition number estimate is not computed
*          = 2: the matrix Ac can not be reduced to Schur canonical
*               form and condition number estimate is not computed
*          
*  Further details
*  ===============
*
*  The condition number of the discrete-time Riccati equation is
*  estimated as
*
*  cond = ( norm(Theta)*norm(A) + norm(inv(Omega))*norm(C) + 
*               norm(Pi)*norm(D) ) / norm(X)
*
*  where Omega, Theta and Pi are linear operators defined by
*
*  Omega(Z) = transpose(op(Ac))*Z*op(Ac) - Z,
*  Theta(Z) = inv(Omega(transpose(op(Z))*X*op(Ac) +
*                 transpose(op(Ac))*X*op(Z))),
*  Pi(Z) = inv(Omega(transpose(op(Ac))*X*Z*X*op(Ac)))
*                    -1                                      -1
*  and Ac = (I + D*X)  *A (if TRANA = 'N') or Ac = A*(I + X*D)
*  (if TRANA = 'T' or 'C').
*
*  The program estimates the quantities
*
*  sepd(op(Ac),transpose(op(Ac)) = 1 / norm(inv(Omega)),
*
*  norm(Theta) and norm(Pi) using 1-norm condition estimator.
*
*  References
*  ==========
*
*  [1] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [2] M.M. Konstantinov, P.Hr. Petkov, and N.D. Christov. Perturbation
*      analysis of the discrete Riccati equation. Kybernetica (Prague),
*      vol. 29,pp. 18-29, 1993.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA, VOIDDUMMY
      CHARACTER          EQUED, TRANAT 
      INTEGER            I, IAF, IBR, IC, IDLC, IJ, INFO2, IR, ITMP,
     $                   IWRK, IXMA, J, KASE, LWA, MINWRK, SDIM
      DOUBLE PRECISION   ANORM, CNORM, DNORM, EST, PINORM, SCALE, SEPD,
     $                   THNORM, WRCON, XNORM
*     ..
*     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLANGE, DLANSY
      EXTERNAL           DLANGE, DLANSY, LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEES, DGEMM, DGESVX, DLACON, DLACPY, DLASET,
     $                   DSYMM, DSYR2K, LYPDTR, XERBLA, VOIDDUMMY
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      ELSE IF( LDAC.LT.MAX( 1, N ) ) THEN 
         INFO = -14
      ELSE IF( LDT.LT.MAX( 1, N ) ) THEN 
         INFO = -16
      ELSE IF( LDU.LT.MAX( 1, N ) ) THEN 
         INFO = -18
      END IF
*
*     Compute workspace
*
      MINWRK = 5*N*N +3*N + MAX( 1, 4*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -23 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICDRC', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
      XNORM = DLANSY( '1', UPLO, N, X, LDX, WORK )
      IF( XNORM.EQ.ZERO ) THEN
         RCOND = ZERO
         RETURN
      END IF
*
*     Compute the norms of the matrices A, C and D
*
      ANORM = DLANGE( '1', N, N, A, LDA, WORK )
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
*     Workspace usage
*
      LWA = 5*N*N + 3*N
      IDLC = N*N
      ITMP = IDLC + N*N
      IXMA = ITMP + N*N
      IAF = IXMA + N*N
      IR = IAF + N*N
      IC = IR + N
      IBR = IC + N
      IWRK = IBR + N
*
      CALL DLASET( 'Full', N, N, ZERO, ONE, WORK, N )
      CALL DSYMM( 'L', UPLO, N, N, ONE, D, LDD, X, LDX, ONE,
     $            WORK, N )
      IF( NOTRNA ) THEN
*                              -1
*        Compute Ac = (I + D*X)  *A
*
         CALL DLACPY( 'F', N, N, A, LDA, T, LDT )
         CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N, IWORK,
     $                EQUED, WORK( IR+1 ), WORK( IC+1 ), T, LDT, AC, 
     $                LDAC, WRCON, WFERR, WORK( IBR+1 ), WORK( IWRK+1 ),
     $                IWORK( N+1 ), INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
      ELSE
*                                -1
*        Compute Ac = A*(I + X*D)
*
         DO 20 J = 1, N
            DO 10 I = 1, N
               T( I, J ) = A( J, I )
   10       CONTINUE
   20    CONTINUE
         CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N, IWORK,
     $                EQUED, WORK( IR+1 ), WORK( IC+1 ), T, LDT,
     $                WORK( ITMP+1 ), N, WRCON, WFERR, WORK( IBR+1 ),
     $                WORK( IWRK+1 ), IWORK( N+1 ), INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 1
            RETURN
         END IF
         DO 40 J = 1, N
            DO 30 I = 1, N 
               AC( I, J ) = WORK( ITMP+J+(I-1)*N )
   30       CONTINUE
   40    CONTINUE
      END IF
*
*     Compute the Schur factorization of Ac 
*
      CALL DLACPY( 'F', N, N, AC, LDAC, T, LDT )
      CALL DGEES( 'V', 'N', VOIDDUMMY, N, T, LDT, SDIM, 
     $     WR, WI, U, LDU,
     $     WORK( IWRK+1 ), LWORK-IWRK, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
*
*     Compute X*op(Ac)
*
      CALL DGEMM( 'N', TRANA, N, N, N, ONE, X, LDX, AC, LDAC, ZERO,
     $            WORK( IXMA+1 ), N )
*
*     Estimate sepd(op(Ac),transpose(op(Ac))) 
*
      IF( NOTRNA ) THEN
         TRANAT = 'T'
      ELSE
         TRANAT = 'N'
      END IF
* 
      EST = ZERO
      KASE = 0
   50 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 70 J = 1, N
               DO 60 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   60          CONTINUE
   70       CONTINUE
         ELSE
            DO 90 J = 1, N
               DO 80 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
   80          CONTINUE
   90       CONTINUE
         END IF
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK, N, ZERO,
     $               WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(Ac')*Y*op(Ac) - Y = scale*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         ELSE
*
*           Solve op(Ac)*Z*op(Ac') - Z = scale*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, U, LDU, 
     $               ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix 
*
         IJ = 0
         IF( LOWER ) THEN
            DO 110 J = 1, N
               DO 100 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  100          CONTINUE
  110       CONTINUE
         ELSE
            DO 130 J = 1, N
               DO 120 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  120          CONTINUE
  130       CONTINUE
         END IF
         GO TO 50
      END IF
*
      SEPD = SCALE / TWO / EST
*
*     Return if the equation is singular
*
      IF( SEPD.EQ.ZERO ) THEN 
         RCOND = ZERO
         RETURN
      END IF
*
*     Estimate norm(Theta)
*
      EST = ZERO
      KASE = 0
  140 CONTINUE
      CALL DLACON( N*N, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Compute RHS = op(W')*X*op(A) + op(A')*X*op(W)
*
         CALL DSYR2K( UPLO, TRANAT, N, N, ONE, WORK, N, WORK( IXMA+1 ),
     $                N, ZERO, WORK( ITMP+1 ), N ) 
         CALL DLACPY( UPLO, N, N, WORK( ITMP+1 ), N, WORK, N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, WORK( ITMP+1 ),
     $               N, ZERO, WORK, N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(Ac')*Y*op(Ac) - Y = scale*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK, N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         ELSE
*
*           Solve op(Ac)*Z*op(Ac') - Z = scale*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK, N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         END IF
*
*        Transform back to obtain the solution: X := U*X*U'
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK, N, U, LDU, ZERO,
     $               WORK( ITMP+1 ), N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK( ITMP+1 ), N, U,
     $               LDU, ZERO, WORK, N ) 
         GO TO 140
      END IF
*
      THNORM = EST / SCALE
*
*     Estimate norm(Pi)
*
      EST = ZERO
      KASE = 0
  150 CONTINUE
      CALL DLACON( N*(N+1)/2, WORK( IDLC+1 ), WORK, IWORK, EST, KASE )
      IF( KASE.NE.0 ) THEN
*
*        Unpack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 170 J = 1, N
               DO 160 I = J, N
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
  160          CONTINUE
  170       CONTINUE
         ELSE
            DO 190 J = 1, N
               DO 180 I = 1, J
                  IJ = IJ + 1
                  WORK( ITMP+I+(J-1)*N ) = WORK( IJ )
  180          CONTINUE
  190       CONTINUE
         END IF
*
*        Compute RHS = op(Ac')*X*W*X*op(Ac)
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N,
     $               WORK( IXMA+1 ), N, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, WORK( IXMA+1 ), N, WORK,
     $               N, ZERO, WORK( ITMP+1 ), N )
*
*        Transform the right-hand side: RHS := U'*RHS*U
*
         CALL DSYMM( 'L', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'T', 'N', N, N, N, ONE, U, LDU, 
     $               WORK, N, ZERO, WORK( ITMP+1 ), N )
         IF( KASE.EQ.1 ) THEN
*
*           Solve op(Ac')*Y*op(Ac) - Y = scale*RHS
*
            CALL LYPDTR( TRANA, N, T, LDT, WORK( ITMP+1 ), N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )
         ELSE
*
*           Solve op(Ac)*Z*op(Ac') - Z = scale*RHS
*
            CALL LYPDTR( TRANAT, N, T, LDT, WORK( ITMP+1 ), N, SCALE,
     $                   WORK( IWRK+1 ), INFO2 )         
         END IF
*
*        Transform back to obtain the solution: X := U*X*U' .
*
         CALL DSYMM( 'R', UPLO, N, N, ONE, WORK( ITMP+1 ), N, 
     $               U, LDU, ZERO, WORK, N )
         CALL DGEMM( 'N', 'T', N, N, N, ONE, WORK, N, 
     $               U, LDU, ZERO, WORK( ITMP+1 ), N )
*
*        Pack the triangular part of symmetric matrix
*
         IJ = 0
         IF( LOWER ) THEN
            DO 210 J = 1, N
               DO 200 I = J, N
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  200          CONTINUE
  210       CONTINUE
         ELSE
            DO 230 J = 1, N
               DO 220 I = 1, J
                  IJ = IJ + 1
                  WORK( IJ ) = WORK( ITMP+I+(J-1)*N )
  220          CONTINUE
  230       CONTINUE
         END IF
         GO TO 150
      END IF
*
      PINORM =  TWO*EST / SCALE
*
*     Estimate the reciprocal condition number
*
      RCOND = SEPD*XNORM / ( CNORM + SEPD*( THNORM*ANORM + 
     $        PINORM*DNORM ) )
      IF( RCOND.GT.ONE ) RCOND = ONE
*
      WORK( 1 ) = DBLE( LWA )
      RETURN
*
*     End of RICDRC
*
      END
      SUBROUTINE RICDSL( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $                   WR, WI, RCOND, FERR, WORK, LWORK, IWORK, BWORK,
     $                   INFO )
*
*  -- RICCPACK routine (version 1.0) --
*     May 10, 2000
*
*     .. Scalar Arguments ..
      CHARACTER          TRANA, UPLO
      INTEGER            INFO, LDA, LDC, LDD, LDX, LWORK, N
      DOUBLE PRECISION   FERR, RCOND
*     ..
*     .. Array Arguments ..
      LOGICAL            BWORK( * )
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), C( LDC, * ), D( LDD, * ),  
     $                   X( LDX, * ), WI( * ), WORK( * ), WR( * ) 
*     ..
*
*  Purpose
*  =======
*
*  RICDSL solves the discrete-time matrix algebraic Riccati equation
*                               -1          
*  transpose(op(A))*X*(In + D*X)  *op(A) - X + C = 0
*
*  where op(A) = A or A**T and C, D are symmetric (C = C**T, D = D**T). 
*  The matrices A, C and D are N-by-N and the solution X is N-by-N.
*
*  Error bound on the solution and a condition estimate are also
*  provided.
*
*  It is assumed that the matrices A, C and D are such that the
*  corresponding matrix pencil has N eigenvalues with moduli
*  less than one.
*
*  Arguments
*  =========
*
*  TRANA   (input) CHARACTER*1
*          Specifies the option op(A):
*          = 'N': op(A) = A    (No transpose) 
*          = 'T': op(A) = A**T (Transpose)
*          = 'C': op(A) = A**T (Conjugate transpose = Transpose)
*
*  N       (input) INTEGER
*          The order of the matrix A, and the order of the 
*          matrices C, D and X. N >= 0.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          The N-by-N matrix A. 
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  UPLO    (input) CHARACTER*1
*          = 'U':  Upper triangles of C and D are stored;
*          = 'L':  Lower triangles of C and D are stored.
*
*  C       (input) DOUBLE PRECISION array, dimension (LDC,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of C
*          contains the upper triangular part of the matrix C. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of C 
*          contains the lower triangular part of the matrix C. 
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1,N).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD,N)
*          If UPLO = 'U', the leading N-by-N upper triangular part of D
*          contains the upper triangular part of the matrix D. 
*          If UPLO = 'L', the leading N-by-N lower triangular part of D 
*          contains the lower triangular part of the matrix D. 
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1,N).
*
*  X       (output) DOUBLE PRECISION array, dimension (LDX,N)
*          The N-by-N solution matrix X. 
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X. LDX >= max(1,N).
*
*  WR      (output) DOUBLE PRECISION array, dimension (N)
*  WI      (output) DOUBLE PRECISION array, dimension (N)
*          On exit, if INFO = 0, WR(1:N) and WI(1:N) contain the real
*          and imaginary parts, respectively, of the eigenvalues of
*                        -1                                      -1
*          Ac = (I + D*X)  *A (if TRANA = 'N') or Ac = A*(I + X*D)
*          (if TRANA = 'T' or 'C').
*
*  RCOND   (output) DOUBLE PRECISION
*          On exit, an estimate of the reciprocal condition number of
*          the discrete-time Riccati equation. 
*
*  FERR    (output) DOUBLE PRECISION
*          On exit, an estimated forward error bound for the solution X.
*          If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) contains the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 12*N*N + 22*N + 
*          max(16,4*N). 
*          For good performance, LWORK must generally be larger.
*
*  IWORK   (workspace) INTEGER array, dimension max(2*N,N*N)
*
*  BWORK   (workspace) LOGICAL array, dimension (2*N)
*
*  INFO    (output) INTEGER
*          = 0: successful exit
*          < 0: if INFO = -i, the i-th argument had an illegal value
*          = 1: the QZ algorithm failed to compute the eigenvalues of 
*               the matrix pencil
*          = 2: after reordering, roundoff changed values of some
*               complex eigenvalues so that leading eigenvalues in 
*               the generalized Schur form have no longer moduli
*               less than one
*          = 3: reordering of the generalized Shur form failed
*          = 4: the matrix pencil has less than N generalized
*               eigenvalues with moduli less than one
*          = 5: the system of linear equations for the solution is
*               singular to working precision
*                                        -1                      -1
*          = 6: the matrix Ac = (I + D*X)  *A or Ac = A*(I + X*D)
*               can not be reduced to Schur canonical form and condition
*               number estimate and forward error estimate are not
*               computed
*
*  Further Details
*  ===============
*
*  The discrete-time matrix Riccati equation is solved by the
*  generalized Schur method [1].
*
*  The condition number of the equation is estimated using 1-norm
*  estimator.
*
*  The forward error bound is estimated using a practical error bound
*  similar to the one proposed in [2].
*
*  References 
*  ==========
*
*  [1] W.F Arnold, III and A.J. Laub. Generalized eigenproblem
*      algorithms and software for algebraic Riccati equations,
*      Proc. IEEE, vol. 72, pp. 1746-1754, 1984. 
*  [2] N.J. Higham. Perturbation theory and backward error for AX - XB =
*      C, BIT, vol. 33, pp. 124-136, 1993.
*  [3] M.M. Konstantinov, P.Hr. Petkov, and N.D. Christov. Perturbation
*      analysis of the discrete Riccati equation. Kybernetica (Prague),
*      vol. 29,pp. 18-29, 1993.
* 
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            LOWER, NOTRNA
      CHARACTER          EQUED 
      INTEGER            I, IAC, IAF, IALFAI, IALFAR, IB, IBETA, IBR, 
     $                   IC, IFR, IJ, INFO2, IR, ISCL, IU, IV, IVS,
     $                   IWFERR,IWRK, J, LWA, LWAMAX, MINWRK, N2, SDIM
      DOUBLE PRECISION   CNORM, CNORM2, DNORM, DNORM2, TEMP
*     ..
*     .. External Functions ..
      LOGICAL            LSAME, SELMLO
      DOUBLE PRECISION   DLANSY
      EXTERNAL           DLANSY, LSAME, SELMLO
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGESVX, DGGES, DLASCL, DLASET, RICDFR, RICDRC,
     $                   XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
*     ..
*     .. Executable Statements ..      
*
*     Decode and Test input parameters
*
      NOTRNA = LSAME( TRANA, 'N' )
      LOWER = LSAME( UPLO, 'L' )   
*
      INFO = 0
      IF( .NOT.NOTRNA .AND. .NOT.LSAME( TRANA, 'T' ) .AND. .NOT.
     $    LSAME( TRANA, 'C' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( .NOT.( LOWER .OR. LSAME( UPLO, 'U' ) ) ) THEN
         INFO = -5 
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDD.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -11
      END IF
*
*     Compute workspace
*
      MINWRK = 12*N*N + 22*N + MAX( 16, 4*N )
      IF( LWORK.LT.MINWRK ) THEN
         INFO = -17 
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'RICDSL', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) 
     $   RETURN
*
*     Compute the norms of the matrices C and D
*
      CNORM = DLANSY( '1', UPLO, N, C, LDC, WORK )
      DNORM = DLANSY( '1', UPLO, N, D, LDD, WORK ) 
*
      N2 = 2*N
*
*     Construct the matrix pencil
*
      DO 20 J = 1, N
         DO 10 I = 1, N
            IJ = ( J - 1 )*N2 + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = A( I, J )
            ELSE
               WORK( IJ ) = A( J, I )
            END IF
            IJ = ( J - 1 )*N2 + N + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = -C( I, J )
               ELSE
                  WORK( IJ ) = -C( J, I )
               END IF
            END IF
            IJ = N2*N2 + ( N + J - 1 )*N2 + I
            IF( .NOT.LOWER ) THEN
               IF( I.LE.J ) THEN
                  WORK( IJ ) = D( I, J )
               ELSE
                  WORK( IJ ) = D( J, I )
               END IF
            ELSE
               IF( I.GE.J ) THEN
                  WORK( IJ ) = D( I, J )
               ELSE
                  WORK( IJ ) = D( J, I )
               END IF
            END IF
            IJ = N2*N2 + ( N + J - 1)*N2 + N + I
            IF( NOTRNA ) THEN
               WORK( IJ ) = A( J, I )
            ELSE
               WORK( IJ ) = A( I, J )
            END IF
   10    CONTINUE
   20 CONTINUE
      CALL DLASET( 'Full', N, N, ZERO, ZERO, WORK( N2*N+1 ), N2 )
      CALL DLASET( 'Full', N, N, ZERO, ZERO, WORK( N2*N2+N+1 ), N2 )
      CALL DLASET( 'Full', N, N, ZERO, ONE, WORK( N2*N+N+1 ), N2 )
      CALL DLASET( 'Full', N, N, ZERO, ONE, WORK( N2*N2+1 ), N2 )
*
*     Scale the matrix pencil
*
      CNORM2 = SQRT( CNORM )
      DNORM2 = SQRT( DNORM )
      ISCL = 0
      IF( CNORM2.GT.DNORM2 .AND. DNORM2.GT.ZERO ) THEN
         CALL DLASCL( 'G', 0, 0, CNORM2, DNORM2, N, N, WORK( N+1 ), N2, 
     $                INFO2 ) 
         CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N,
     $                WORK( N2*N2+N2*N+1 ), N2, INFO2 )
         ISCL = 1
      END IF
*      
*     Workspace usage
*
      LWA = 12*N*N + 6*N
      IALFAR = 2*N2*N2
      IALFAI = IALFAR + N2
      IBETA = IALFAI + N2
      IVS = IBETA + N2
      IWRK = IVS + N2*N2
*
*     Compute the generalized Schur factorization of the matrix pencil
*
      CALL DGGES( 'N', 'V', 'S', SELMLO, N2, WORK, N2, WORK( N2*N2+1 ),
     $             N2, SDIM, WORK( IALFAR+1 ), WORK( IALFAI+1 ),
     $             WORK( IBETA+1 ), WORK( IVS+1 ), N2, WORK( IVS+1 ),
     $             N2, WORK( IWRK+1 ), LWORK-IWRK, BWORK, INFO2 )
      IF( INFO2.GT.0 .AND. INFO2.LE.N2+1 ) THEN
         INFO = 1
         RETURN
      ELSE IF( INFO2.EQ.N2+2 ) THEN
         INFO = 2
         RETURN
      ELSE IF( INFO2.EQ.N2+3 ) THEN
         INFO = 3
         RETURN
      ELSE IF( SDIM.NE.N ) THEN
         INFO = 4
         RETURN
      END IF
      LWAMAX = LWA + INT( WORK( IWRK+1 ) )
*
*     Store the matrices V11 and V21
*
      DO 40 J = 1, N
         DO 30 I = 1, N
            IJ = ( J - 1 )*N + I
            IV = ( I - 1 )*N2 + IVS + J
            WORK( IJ ) = WORK( IV )
            IJ = ( J - 1 )*N + 2*N*N + I
            IV = ( I - 1 )*N2 + IVS + N + J
            WORK( IJ ) = WORK( IV )
   30    CONTINUE
   40 CONTINUE
*
*     Workspace usage
*
      IAF = N*N  
      IB = IAF + N*N
      IR = IB + N*N
      IC = IR + N
      IFR = IC + N
      IBR = IFR + N
      IWRK = IBR + N
* 
*     Compute the solution matrix X
*
      CALL DGESVX( 'E', 'N', N, N, WORK, N, WORK( IAF+1 ), N,
     $             IWORK, EQUED, WORK( IR+1 ), WORK( IC+1 ),
     $             WORK( IB+1 ), N, X, LDX, RCOND, WORK( IFR+1 ),
     $             WORK( IBR+1 ), WORK( IWRK+1 ), IWORK( N+1 ), 
     $             INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
*
*     Symmetrize the solution
*
      IF( N.GT.1 ) THEN
         DO 60 I = 1, N - 1
            DO 50 J = I + 1, N
               TEMP = ( X( I, J ) + X( J, I ) ) / TWO
               X( I, J ) = TEMP
               X( J, I ) = TEMP
   50       CONTINUE
   60    CONTINUE
      END IF
*
*     Undo scaling for the solution matrix
*
      IF( ISCL.EQ.1 ) THEN
         CALL DLASCL( 'G', 0, 0, DNORM2, CNORM2, N, N, X, LDX, INFO2 )
      END IF
*
*     Workspace usage
*
      LWA = 3*N*N + N
      IU = N*N
      IWFERR = IU + N*N
      IAC = IWFERR + N
      IWRK = IAC + N*N
*
*     Estimate the reciprocal condition number
*
      CALL RICDRC( TRANA, N, A, LDA, UPLO, C, LDC, D, LDD, X, LDX,
     $             RCOND, WORK( IAC+1 ), N, WORK, N, WORK( IU+1 ), N,
     $             WR, WI, WORK( IWFERR+1 ), WORK( IWRK+1 ), LWORK-IWRK,
     $             IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 6
         RETURN
      END IF
      LWA = LWA + INT( WORK( IWRK+1 ) )
      LWAMAX = MAX( LWA, LWAMAX )
*
*     Return if the equation is singular
*
      IF( RCOND.EQ.ZERO ) THEN
          FERR = ONE
          RETURN
      END IF
*
*     Estimate the bound on the forward error
*
      CALL RICDFR( TRANA, N, A, LDA, UPLO, C, LDC, X, LDX, 
     $             WORK( IAC+1 ), N, WORK, N, WORK( IU+1 ), N,
     $             WORK( IWFERR+1 ), FERR, WORK( IWRK+1 ),
     $             LWORK-IWRK, IWORK, INFO2 )
      LWA = 9*N*N + 3*N
      LWAMAX = MAX( LWA, LWAMAX )
      WORK( 1 ) = DBLE( LWAMAX )
      RETURN
*
*     End of RICDSL
*
      END

      LOGICAL FUNCTION SELMLO( ALPHAR, ALPHAI, BETA )
*
*  -- LISPACK auxiliary routine (version 3.0) --
*     Tech. University of Sofia
*     September 22, 1999
*
*     .. Scalar Arguments ..
      DOUBLE PRECISION   ALPHAR, ALPHAI, BETA
*     ..
*
*     Purpose
*     =======
*
*     SELMLO is used to select eigenvalues with modules less than one
*     to sort to the top left of the generalized Schur form of the
*     matrix pencil in solving discrete-time matrix algebraic Riccati
*     equations
*
*     .. External Functions ..
      DOUBLE PRECISION   DLAPY2
      EXTERNAL           DLAPY2
*
*     .. Intrinsic Functions ..
      INTRINSIC          ABS
*
*     .. Executable Statements ..
*
      SELMLO = DLAPY2( ALPHAR, ALPHAI ).LT.ABS( BETA )
*
*     End of SELMLO
*
      END
