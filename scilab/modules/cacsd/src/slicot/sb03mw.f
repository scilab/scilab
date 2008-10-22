      SUBROUTINE SB03MW( LTRAN, LUPPER, T, LDT, B, LDB, SCALE, X, LDX,
     $                   XNORM, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for the 2-by-2 symmetric matrix X in
C
C            op(T)'*X + X*op(T) = SCALE*B,
C
C     where T is 2-by-2, B is symmetric 2-by-2, and op(T) = T or T',
C     where T' denotes the transpose of T.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     LTRAN   LOGICAL
C             Specifies the form of op(T) to be used, as follows:
C             = .FALSE.:  op(T) = T,
C             = .TRUE. :  op(T) = T'.
C
C     LUPPER  LOGICAL
C             Specifies which triangle of the matrix B is used, and
C             which triangle of the matrix X is computed, as follows:
C             = .TRUE. :  The upper triangular part;
C             = .FALSE.:  The lower triangular part.
C            
C     Input/Output Parameters
C
C     T       (input) DOUBLE PRECISION array, dimension (LDT,2)
C             The leading 2-by-2 part of this array must contain the
C             matrix T.
C
C     LDT     INTEGER
C             The leading dimension of array T.  LDT >= 2.
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,2)
C             On entry with LUPPER = .TRUE., the leading 2-by-2 upper
C             triangular part of this array must contain the upper
C             triangular part of the symmetric matrix B and the strictly
C             lower triangular part of B is not referenced.
C             On entry with LUPPER = .FALSE., the leading 2-by-2 lower
C             triangular part of this array must contain the lower
C             triangular part of the symmetric matrix B and the strictly
C             upper triangular part of B is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= 2.
C
C     SCALE   (output) DOUBLE PRECISION
C             The scale factor. SCALE is chosen less than or equal to 1
C             to prevent the solution overflowing.
C
C     X       (output) DOUBLE PRECISION array, dimension (LDX,2)
C             On exit with LUPPER = .TRUE., the leading 2-by-2 upper
C             triangular part of this array contains the upper
C             triangular part of the symmetric solution matrix X and the
C             strictly lower triangular part of X is not referenced.
C             On exit with LUPPER = .FALSE., the leading 2-by-2 lower
C             triangular part of this array contains the lower
C             triangular part of the symmetric solution matrix X and the
C             strictly upper triangular part of X is not referenced.
C             Note that X may be identified with B in the calling
C             statement.
C
C     LDX     INTEGER
C             The leading dimension of array X.  LDX >= 2.
C
C     XNORM   (output) DOUBLE PRECISION
C             The infinity-norm of the solution.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             = 1:  if T and -T have too close eigenvalues, so T
C                   is perturbed to get a nonsingular equation.
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
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, May 1997.
C     Based on DLALY2 by P. Petkov, Tech. University of Sofia, September
C     1993.
C
C     REVISIONS
C
C     -   
C
C     KEYWORDS
C
C     Continuous-time system, Lyapunov equation, matrix algebra.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, FOUR
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0,
     $                     FOUR = 4.0D+0 )
C     ..
C     .. Scalar Arguments ..
      LOGICAL            LTRAN, LUPPER
      INTEGER            INFO, LDB, LDT, LDX
      DOUBLE PRECISION   SCALE, XNORM
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), T( LDT, * ), X( LDX, * )
C     ..
C     .. Local Scalars ..
      INTEGER            I, IP, IPSV, J, JP, JPSV, K
      DOUBLE PRECISION   EPS, SMIN, SMLNUM, TEMP, XMAX
C     ..
C     .. Local Arrays ..
      INTEGER            JPIV( 3 )
      DOUBLE PRECISION   BTMP( 3 ), T9( 3, 3 ), TMP( 3 )
C     ..
C     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
C     ..
C     .. External Subroutines ..
      EXTERNAL           DSWAP
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
C     ..
C     .. Executable Statements ..
C
C     Do not check the input parameters for errors
C
      INFO = 0
C
C     Set constants to control overflow
C
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
C
C     Solve equivalent 3-by-3 system using complete pivoting.
C     Set pivots less than SMIN to SMIN.
C
      SMIN = MAX( MAX( ABS( T( 1, 1 ) ), ABS( T( 1, 2 ) ),
     $                 ABS( T( 2, 1 ) ), ABS( T( 2, 2 ) ) )*EPS,
     $            SMLNUM )
      T9( 1, 3 ) = ZERO
      T9( 3, 1 ) = ZERO
      T9( 1, 1 ) = T( 1, 1 )
      T9( 2, 2 ) = T( 1, 1 ) + T( 2, 2 )
      T9( 3, 3 ) = T( 2, 2 )
      IF( LTRAN ) THEN
         T9( 1, 2 ) = T( 1, 2 )
         T9( 2, 1 ) = T( 2, 1 )
         T9( 2, 3 ) = T( 1, 2 )
         T9( 3, 2 ) = T( 2, 1 )
      ELSE
         T9( 1, 2 ) = T( 2, 1 )
         T9( 2, 1 ) = T( 1, 2 )
         T9( 2, 3 ) = T( 2, 1 )
         T9( 3, 2 ) = T( 1, 2 )
      END IF
      BTMP( 1 ) = B( 1, 1 )/TWO
      IF ( LUPPER ) THEN
         BTMP( 2 ) = B( 1, 2 )
      ELSE
         BTMP( 2 ) = B( 2, 1 )
      END IF
      BTMP( 3 ) = B( 2, 2 )/TWO
C
C     Perform elimination
C
      DO 50 I = 1, 2
         XMAX = ZERO
C
         DO 20 IP = I, 3
C
            DO 10 JP = I, 3
               IF( ABS( T9( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( T9( IP, JP ) )
                  IPSV = IP
                  JPSV = JP
               END IF
   10       CONTINUE
C
   20    CONTINUE
C
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
C
         DO 40 J = I + 1, 3
            T9( J, I ) = T9( J, I ) / T9( I, I )
            BTMP( J ) = BTMP( J ) - T9( J, I )*BTMP( I )
C
            DO 30 K = I + 1, 3
               T9( J, K ) = T9( J, K ) - T9( J, I )*T9( I, K )
   30       CONTINUE
C
   40    CONTINUE
C
   50 CONTINUE
C
      IF( ABS( T9( 3, 3 ) ).LT.SMIN )
     $   T9( 3, 3 ) = SMIN
      SCALE = ONE
      IF( ( FOUR*SMLNUM )*ABS( BTMP( 1 ) ).GT.ABS( T9( 1, 1 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 2 ) ).GT.ABS( T9( 2, 2 ) ) .OR.
     $    ( FOUR*SMLNUM )*ABS( BTMP( 3 ) ).GT.ABS( T9( 3, 3 ) ) ) THEN
         SCALE = ( ONE / FOUR ) / MAX( ABS( BTMP( 1 ) ),
     $               ABS( BTMP( 2 ) ), ABS( BTMP( 3 ) ) )
         BTMP( 1 ) = BTMP( 1 )*SCALE
         BTMP( 2 ) = BTMP( 2 )*SCALE
         BTMP( 3 ) = BTMP( 3 )*SCALE
      END IF
C
      DO 70 I = 1, 3
         K = 4 - I
         TEMP = ONE / T9( K, K )
         TMP( K ) = BTMP( K )*TEMP
C
         DO 60 J = K + 1, 3
            TMP( K ) = TMP( K ) - ( TEMP*T9( K, J ) )*TMP( J )
  60     CONTINUE
C
  70  CONTINUE
C
      DO 80 I = 1, 2
         IF( JPIV( 3-I ).NE.3-I ) THEN
            TEMP = TMP( 3-I )
            TMP( 3-I ) = TMP( JPIV( 3-I ) )
            TMP( JPIV( 3-I ) ) = TEMP
         END IF
  80  CONTINUE
C
      X( 1, 1 ) = TMP( 1 )
      IF ( LUPPER ) THEN
         X( 1, 2 ) = TMP( 2 )
      ELSE
         X( 2, 1 ) = TMP( 2 )
      END IF
      X( 2, 2 ) = TMP( 3 )
      XNORM = MAX( ABS( TMP( 1 ) ) + ABS( TMP( 2 ) ),
     $             ABS( TMP( 2 ) ) + ABS( TMP( 3 ) ) )
C
      RETURN
C *** Last line of SB03MW ***
      END
