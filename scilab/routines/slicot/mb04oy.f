      SUBROUTINE MB04OY( M, N, V, TAU, A, LDA, B, LDB, DWORK )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To apply a real elementary reflector H to a real (m+1)-by-n
C     matrix C = [ A ], from the left, where A has one row. H is
C                [ B ]
C     represented in the form
C                                        ( 1 )
C           H = I - tau * u *u',    u  = (   ),
C                                        ( v )
C     where tau is a real scalar and v is a real m-vector.
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
C             The number of rows of the matrix B.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrices A and B.  N >= 0.
C
C     V       (input) DOUBLE PRECISION array, dimension (M)
C             The vector v in the representation of H.
C
C     TAU     (input) DOUBLE PRECISION
C             The scalar factor of the elementary reflector H.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading 1-by-N part of this array must
C             contain the matrix A.
C             On exit, the leading 1-by-N part of this array contains
C             the updated matrix A (the first row of H * C).
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= 1.
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             On entry, the leading M-by-N part of this array must
C             contain the matrix B. 
C             On exit, the leading M-by-N part of this array contains
C             the updated matrix B (the last m rows of H * C).
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (N)
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
C     V. Sima, Katholieke Univ. Leuven, Belgium, Feb. 1997.
C     Based on LAPACK routines DLARFX and DLATZM.
C
C     REVISIONS
C
C     Dec. 1997.
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
      INTEGER           LDA, LDB, M, N
      DOUBLE PRECISION  TAU
C     .. Array Arguments ..
      DOUBLE PRECISION  A( LDA, * ), B( LDB, * ), DWORK( * ), V( * )
C     .. Local Scalars ..
      INTEGER           J
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
C     Form  H * C, where H has order m+1.
C
      GO TO ( 10, 30, 50, 70, 90, 110, 130, 150,
     $        170, 190 ) M+1
C
C     Code for general M. Compute
C
C     w := C'*u,  C := C - tau * u * w'.
C
      CALL DCOPY( N, A, LDA, DWORK, 1 )
      CALL DGEMV( 'Transpose', M, N, ONE, B, LDB, V, 1, ONE, DWORK, 1 )
      CALL DAXPY( N, -TAU, DWORK, 1, A, LDA )
      CALL DGER( M, N, -TAU, V, 1, DWORK, 1, B, LDB )
      GO TO 210
   10 CONTINUE
C
C     Special code for 1 x 1 Householder
C
      T1 = ONE - TAU
      DO 20 J = 1, N
         A( 1, J ) = T1*A( 1, J )
   20 CONTINUE
      GO TO 210
   30 CONTINUE
C
C     Special code for 2 x 2 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      DO 40 J = 1, N
         SUM = A( 1, J ) + V1*B( 1, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
   40 CONTINUE
      GO TO 210
   50 CONTINUE
C
C     Special code for 3 x 3 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      DO 60 J = 1, N
         SUM = A( 1, J ) + V1*B( 1, J ) + V2*B( 2, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
   60 CONTINUE
      GO TO 210
   70 CONTINUE
C
C     Special code for 4 x 4 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      DO 80 J = 1, N
         SUM = A( 1, J ) + V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
   80 CONTINUE
      GO TO 210
   90 CONTINUE
C
C     Special code for 5 x 5 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      DO 100 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
  100 CONTINUE
      GO TO 210
  110 CONTINUE
C
C     Special code for 6 x 6 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      V5 = V( 5 )
      T5 = TAU*V5
      DO 120 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J ) + V5*B( 5, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
         B( 5, J ) = B( 5, J ) - SUM*T5
  120 CONTINUE
      GO TO 210
  130 CONTINUE
C
C     Special code for 7 x 7 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      V5 = V( 5 )
      T5 = TAU*V5
      V6 = V( 6 )
      T6 = TAU*V6
      DO 140 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J ) + V5*B( 5, J ) + V6*B( 6, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
         B( 5, J ) = B( 5, J ) - SUM*T5
         B( 6, J ) = B( 6, J ) - SUM*T6
  140 CONTINUE
      GO TO 210
  150 CONTINUE
C
C     Special code for 8 x 8 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      V5 = V( 5 )
      T5 = TAU*V5
      V6 = V( 6 )
      T6 = TAU*V6
      V7 = V( 7 )
      T7 = TAU*V7
      DO 160 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J ) + V5*B( 5, J ) + V6*B( 6, J ) +
     $                      V7*B( 7, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
         B( 5, J ) = B( 5, J ) - SUM*T5
         B( 6, J ) = B( 6, J ) - SUM*T6
         B( 7, J ) = B( 7, J ) - SUM*T7
  160 CONTINUE
      GO TO 210
  170 CONTINUE
C
C     Special code for 9 x 9 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      V5 = V( 5 )
      T5 = TAU*V5
      V6 = V( 6 )
      T6 = TAU*V6
      V7 = V( 7 )
      T7 = TAU*V7
      V8 = V( 8 )
      T8 = TAU*V8
      DO 180 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J ) + V5*B( 5, J ) + V6*B( 6, J ) +
     $                      V7*B( 7, J ) + V8*B( 8, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
         B( 5, J ) = B( 5, J ) - SUM*T5
         B( 6, J ) = B( 6, J ) - SUM*T6
         B( 7, J ) = B( 7, J ) - SUM*T7
         B( 8, J ) = B( 8, J ) - SUM*T8
  180 CONTINUE
      GO TO 210
  190 CONTINUE
C
C     Special code for 10 x 10 Householder
C
      V1 = V( 1 )
      T1 = TAU*V1
      V2 = V( 2 )
      T2 = TAU*V2
      V3 = V( 3 )
      T3 = TAU*V3
      V4 = V( 4 )
      T4 = TAU*V4
      V5 = V( 5 )
      T5 = TAU*V5
      V6 = V( 6 )
      T6 = TAU*V6
      V7 = V( 7 )
      T7 = TAU*V7
      V8 = V( 8 )
      T8 = TAU*V8
      V9 = V( 9 )
      T9 = TAU*V9
      DO 200 J = 1, N
         SUM = A( 1, J ) +  V1*B( 1, J ) + V2*B( 2, J ) + V3*B( 3, J ) +
     $                      V4*B( 4, J ) + V5*B( 5, J ) + V6*B( 6, J ) +
     $                      V7*B( 7, J ) + V8*B( 8, J ) + V9*B( 9, J )
         A( 1, J ) = A( 1, J ) - SUM*TAU
         B( 1, J ) = B( 1, J ) - SUM*T1
         B( 2, J ) = B( 2, J ) - SUM*T2
         B( 3, J ) = B( 3, J ) - SUM*T3
         B( 4, J ) = B( 4, J ) - SUM*T4
         B( 5, J ) = B( 5, J ) - SUM*T5
         B( 6, J ) = B( 6, J ) - SUM*T6
         B( 7, J ) = B( 7, J ) - SUM*T7
         B( 8, J ) = B( 8, J ) - SUM*T8
         B( 9, J ) = B( 9, J ) - SUM*T9
  200 CONTINUE
  210 CONTINUE
      RETURN
C *** Last line of MB04OY ***
      END
