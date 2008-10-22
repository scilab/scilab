      SUBROUTINE MB01UD( SIDE, TRANS, M, N, ALPHA, H, LDH, A, LDA, B,
     $                   LDB, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute one of the matrix products
C 
C        B = alpha*op( H ) * A, or B = alpha*A * op( H ), 
C
C     where alpha is a scalar, A and B are m-by-n matrices, H is an 
C     upper Hessenberg matrix, and op( H ) is one of
C     
C        op( H ) = H   or   op( H ) = H',  the transpose of H.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     SIDE    CHARACTER*1
C             Specifies whether the Hessenberg matrix H appears on the 
C             left or right in the matrix product as follows:
C             = 'L':  B = alpha*op( H ) * A;
C             = 'R':  B = alpha*A * op( H ).
C            
C     TRANS   CHARACTER*1
C             Specifies the form of op( H ) to be used in the matrix
C             multiplication as follows:
C             = 'N':  op( H ) = H;
C             = 'T':  op( H ) = H';
C             = 'C':  op( H ) = H'.
C            
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrices A and B.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrices A and B.  N >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then H is not   
C             referenced and A need not be set before entry.
C            
C     H       (input) DOUBLE PRECISION array, dimension (LDH,k)
C             where k is M when SIDE = 'L' and is N when SIDE = 'R'.
C             On entry with SIDE = 'L', the leading M-by-M upper
C             Hessenberg part of this array must contain the upper 
C             Hessenberg matrix H.
C             On entry with SIDE = 'R', the leading N-by-N upper
C             Hessenberg part of this array must contain the upper 
C             Hessenberg matrix H.
C             The elements below the subdiagonal are not referenced,
C             except possibly for those in the first column, which
C             could be overwritten, but are restored on exit.
C
C     LDH     INTEGER
C             The leading dimension of the array H.  LDH >= max(1,k),  
C             where k is M when SIDE = 'L' and is N when SIDE = 'R'.
C             
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading M-by-N part of this array must contain the
C             matrix A.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C
C     B       (output) DOUBLE PRECISION array, dimension (LDB,N)
C             The leading M-by-N part of this array contains the
C             computed product.
C
C     LDB     INTEGER
C             The leading dimension of the array B.  LDB >= max(1,M).
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value.
C
C     METHOD
C
C     The required matrix product is computed in two steps. In the first
C     step, the upper triangle of H is used; in the second step, the
C     contribution of the subdiagonal is added. A fast BLAS 3 DTRMM
C     operation is used in the first step. 
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, January 1999.
C
C     REVISIONS
C
C     -   
C
C     KEYWORDS
C
C     Elementary matrix operations, matrix operations.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         SIDE, TRANS
      INTEGER           INFO, LDA, LDB, LDH, M, N
      DOUBLE PRECISION  ALPHA
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), H(LDH,*)
C     .. Local Scalars ..
      LOGICAL           LSIDE, LTRANS
      INTEGER           I, J
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DLACPY, DLASET, DSWAP, DTRMM, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO   = 0
      LSIDE  = LSAME( SIDE,  'L' )
      LTRANS = LSAME( TRANS, 'T' ) .OR. LSAME( TRANS, 'C' )
C
      IF(      ( .NOT.LSIDE  ).AND.( .NOT.LSAME( SIDE,  'R' ) ) )THEN
         INFO = -1
      ELSE IF( ( .NOT.LTRANS ).AND.( .NOT.LSAME( TRANS, 'N' ) ) )THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDH.LT.1 .OR. ( LSIDE .AND. LDH.LT.M ) .OR.
     $                  ( .NOT.LSIDE .AND. LDH.LT.N ) ) THEN
         INFO = -7
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -9
      ELSE IF( LDB.LT.MAX( 1, M ) ) THEN
         INFO = -11
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01UD', -INFO )
         RETURN
      END IF
C
C     Quick return, if possible.
C
      IF ( MIN( M, N ).EQ.0 )
     $   RETURN
C
      IF( ALPHA.EQ.ZERO ) THEN
C           
C        Set B to zero and return.
C           
         CALL DLASET( 'Full', M, N, ZERO, ZERO, B, LDB )
         RETURN
      END IF
C
C     Copy A in B and compute one of the matrix products 
C       B = alpha*op( triu( H ) ) * A, or 
C       B = alpha*A * op( triu( H ) ),
C     involving the upper triangle of H.
C     
      CALL DLACPY( 'Full', M, N, A, LDA, B, LDB )
      CALL DTRMM( SIDE, 'Upper', TRANS, 'Non-unit', M, N, ALPHA, H,
     $            LDH, B, LDB )
C     
C     Add the contribution of the subdiagonal of H.
C     If SIDE = 'L', the subdiagonal of H is swapped with the 
C     corresponding elements in the first column of H, and the
C     calculations are organized for column operations.
C     
      IF( LSIDE ) THEN
         IF( M.GT.2 ) 
     $      CALL DSWAP( M-2, H( 3, 2 ), LDH+1, H( 3, 1 ), 1 ) 
         IF( LTRANS ) THEN
            DO 20 J = 1, N
               DO 10 I = 1, M - 1
                  B( I, J ) = B( I, J ) + ALPHA*H( I+1, 1 )*A( I+1, J ) 
   10          CONTINUE
   20       CONTINUE
         ELSE
            DO 40 J = 1, N
               DO 30 I = 2, M
                  B( I, J ) = B( I, J ) + ALPHA*H( I, 1 )*A( I-1, J ) 
   30          CONTINUE
   40       CONTINUE
         END IF
         IF( M.GT.2 ) 
     $      CALL DSWAP( M-2, H( 3, 2 ), LDH+1, H( 3, 1 ), 1 ) 
C     
      ELSE
C     
         IF( LTRANS ) THEN
            DO 50 J = 1, N - 1
               IF ( H( J+1, J ).NE.ZERO ) 
     $            CALL DAXPY( M, ALPHA*H( J+1, J ), A( 1, J ), 1, 
     $                        B( 1, J+1 ), 1 )
   50       CONTINUE
         ELSE
            DO 60 J = 1, N - 1
               IF ( H( J+1, J ).NE.ZERO ) 
     $            CALL DAXPY( M, ALPHA*H( J+1, J ), A( 1, J+1 ), 1,
     $                        B( 1, J ), 1 )
   60       CONTINUE
         END IF
      END IF
C
      RETURN
C *** Last line of MB01UD ***
      END
