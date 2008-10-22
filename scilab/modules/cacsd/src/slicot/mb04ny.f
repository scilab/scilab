      SUBROUTINE MB04NY( M, N, V, INCV, TAU, A, LDA, B, LDB, DWORK )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To apply a real elementary reflector H to a real m-by-(n+1)
C     matrix C = [ A  B ], from the right, where A has one column. H is
C     represented in the form
C                                        ( 1 )
C           H = I - tau * u *u',    u  = (   ),
C                                        ( v )
C     where tau is a real scalar and v is a real n-vector.
C
C     If tau = 0, then H is taken to be the unit matrix.
C
C     In-line code is used if H has order < 11.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrices A and B.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrix B.  N >= 0.
C
C     V       (input) DOUBLE PRECISION array, dimension
C             (1+(N-1)*ABS( INCV ))
C             The vector v in the representation of H.
C
C     INCV    (input) INTEGER
C             The increment between the elements of v.  INCV <> 0.
C
C     TAU     (input) DOUBLE PRECISION
C             The scalar factor of the elementary reflector H.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,1)
C             On entry, the leading M-by-1 part of this array must
C             contain the matrix A.
C             On exit, the leading M-by-1 part of this array contains
C             the updated matrix A (the first column of C * H).
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,M).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             On entry, the leading M-by-N part of this array must
C             contain the matrix B. 
C             On exit, the leading M-by-N part of this array contains
C             the updated matrix B (the last n columns of C * H).
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (M)
C             DWORK is not referenced if H has order less than 11.
C
C     METHOD
C
C     The routine applies the elementary reflector H, taking the special
C     structure of C into account.
C  
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Apr. 1998.
C     Based on LAPACK routines DLARFX and DLATZM.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Elementary matrix operations, elementary reflector, orthogonal
C     transformation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      INTEGER           INCV, LDA, LDB, M, N
      DOUBLE PRECISION  TAU
C     .. Array Arguments ..
      DOUBLE PRECISION  A( LDA, * ), B( LDB, * ), DWORK( * ), V( * )
C     .. Local Scalars ..
      INTEGER           IV, J
      DOUBLE PRECISION  SUM, T1, T2, T3, T4, T5, T6, T7, T8, T9, V1, V2,
     $                  V3, V4, V5, V6, V7, V8, V9
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, DGEMV, DGER
C
C     .. Executable Statements ..
C
      IF( TAU.EQ.ZERO )
     $   RETURN
C
C     Form  C * H, where H has order n+1.
C
      GO TO ( 10, 30, 50, 70, 90, 110, 130, 150,
     $        170, 190 ) N+1
C
C     Code for general N. Compute
C
C     w := C*u,  C := C - tau * w * u'.
C
      CALL DCOPY( M, A, 1, DWORK, 1 )
      CALL DGEMV( 'No transpose', M, N, ONE, B, LDB, V, INCV, ONE,
     $            DWORK, 1 )
      CALL DAXPY( M, -TAU, DWORK, 1, A, 1 )
      CALL DGER( M, N, -TAU, DWORK, 1, V, INCV, B, LDB )
      GO TO 210
   10 CONTINUE
C
C     Special code for 1 x 1 Householder
C
      T1 = ONE - TAU
      DO 20 J = 1, M
         A( J, 1 ) = T1*A( J, 1 )
   20 CONTINUE
      GO TO 210
   30 CONTINUE
C
C     Special code for 2 x 2 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      DO 40 J = 1, M
         SUM = A( J, 1 ) + V1*B( J, 1 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
   40 CONTINUE
      GO TO 210
   50 CONTINUE
C
C     Special code for 3 x 3 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      DO 60 J = 1, M
         SUM = A( J, 1 ) + V1*B( J, 1 ) + V2*B( J, 2 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
   60 CONTINUE
      GO TO 210
   70 CONTINUE
C
C     Special code for 4 x 4 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      DO 80 J = 1, M
         SUM = A( J, 1 ) + V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
   80 CONTINUE
      GO TO 210
   90 CONTINUE
C
C     Special code for 5 x 5 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      DO 100 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
  100 CONTINUE
      GO TO 210
  110 CONTINUE
C
C     Special code for 6 x 6 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      IV = IV + INCV
      V5 = V( IV )
      T5 = TAU*V5
      DO 120 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 ) + V5*B( J, 5 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
         B( J, 5 ) = B( J, 5 ) - SUM*T5
  120 CONTINUE
      GO TO 210
  130 CONTINUE
C
C     Special code for 7 x 7 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      IV = IV + INCV
      V5 = V( IV )
      T5 = TAU*V5
      IV = IV + INCV
      V6 = V( IV )
      T6 = TAU*V6
      DO 140 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 ) + V5*B( J, 5 ) + V6*B( J, 6 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
         B( J, 5 ) = B( J, 5 ) - SUM*T5
         B( J, 6 ) = B( J, 6 ) - SUM*T6
  140 CONTINUE
      GO TO 210
  150 CONTINUE
C
C     Special code for 8 x 8 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      IV = IV + INCV
      V5 = V( IV )
      T5 = TAU*V5
      IV = IV + INCV
      V6 = V( IV )
      T6 = TAU*V6
      IV = IV + INCV
      V7 = V( IV )
      T7 = TAU*V7
      DO 160 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 ) + V5*B( J, 5 ) + V6*B( J, 6 ) +
     $                      V7*B( J, 7 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
         B( J, 5 ) = B( J, 5 ) - SUM*T5
         B( J, 6 ) = B( J, 6 ) - SUM*T6
         B( J, 7 ) = B( J, 7 ) - SUM*T7
  160 CONTINUE
      GO TO 210
  170 CONTINUE
C
C     Special code for 9 x 9 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      IV = IV + INCV
      V5 = V( IV )
      T5 = TAU*V5
      IV = IV + INCV
      V6 = V( IV )
      T6 = TAU*V6
      IV = IV + INCV
      V7 = V( IV )
      T7 = TAU*V7
      IV = IV + INCV
      V8 = V( IV )
      T8 = TAU*V8
      DO 180 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 ) + V5*B( J, 5 ) + V6*B( J, 6 ) +
     $                      V7*B( J, 7 ) + V8*B( J, 8 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
         B( J, 5 ) = B( J, 5 ) - SUM*T5
         B( J, 6 ) = B( J, 6 ) - SUM*T6
         B( J, 7 ) = B( J, 7 ) - SUM*T7
         B( J, 8 ) = B( J, 8 ) - SUM*T8
  180 CONTINUE
      GO TO 210
  190 CONTINUE
C
C     Special code for 10 x 10 Householder
C
      IV = 1
      IF( INCV.LT.0 )
     $   IV = (-N+1)*INCV + 1
      V1 = V( IV )
      T1 = TAU*V1
      IV = IV + INCV
      V2 = V( IV )
      T2 = TAU*V2
      IV = IV + INCV
      V3 = V( IV )
      T3 = TAU*V3
      IV = IV + INCV
      V4 = V( IV )
      T4 = TAU*V4
      IV = IV + INCV
      V5 = V( IV )
      T5 = TAU*V5
      IV = IV + INCV
      V6 = V( IV )
      T6 = TAU*V6
      IV = IV + INCV
      V7 = V( IV )
      T7 = TAU*V7
      IV = IV + INCV
      V8 = V( IV )
      T8 = TAU*V8
      IV = IV + INCV
      V9 = V( IV )
      T9 = TAU*V9
      DO 200 J = 1, M
         SUM = A( J, 1 ) +  V1*B( J, 1 ) + V2*B( J, 2 ) + V3*B( J, 3 ) +
     $                      V4*B( J, 4 ) + V5*B( J, 5 ) + V6*B( J, 6 ) +
     $                      V7*B( J, 7 ) + V8*B( J, 8 ) + V9*B( J, 9 )
         A( J, 1 ) = A( J, 1 ) - SUM*TAU
         B( J, 1 ) = B( J, 1 ) - SUM*T1
         B( J, 2 ) = B( J, 2 ) - SUM*T2
         B( J, 3 ) = B( J, 3 ) - SUM*T3
         B( J, 4 ) = B( J, 4 ) - SUM*T4
         B( J, 5 ) = B( J, 5 ) - SUM*T5
         B( J, 6 ) = B( J, 6 ) - SUM*T6
         B( J, 7 ) = B( J, 7 ) - SUM*T7
         B( J, 8 ) = B( J, 8 ) - SUM*T8
         B( J, 9 ) = B( J, 9 ) - SUM*T9
  200 CONTINUE
  210 CONTINUE
      RETURN
C *** Last line of MB04NY ***
      END
