      SUBROUTINE MB01RY( SIDE, UPLO, TRANS, M, ALPHA, BETA, R, LDR, H,
     $                   LDH, B, LDB, DWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute either the upper or lower triangular part of one of the
C     matrix formulas
C        _
C        R = alpha*R + beta*op( H )*B,                               (1)
C        _
C        R = alpha*R + beta*B*op( H ),                               (2)
C                                                    _     
C     where alpha and beta are scalars, H, B, R, and R are m-by-m
C     matrices, H is an upper Hessenberg matrix, and op( H ) is one of
C     
C        op( H ) = H   or   op( H ) = H',  the transpose of H.
C     
C     The result is overwritten on R. 
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     SIDE    CHARACTER*1
C             Specifies whether the Hessenberg matrix H appears on the 
C             left or right in the matrix product as follows:
C                     _                           
C             = 'L':  R = alpha*R + beta*op( H )*B;
C                     _                           
C             = 'R':  R = alpha*R + beta*B*op( H ).
C            
C     UPLO    CHARACTER*1                               _
C             Specifies which triangles of the matrices R and R are
C             computed and given, respectively, as follows:
C             = 'U':  the upper triangular part;
C             = 'L':  the lower triangular part.
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
C     M       (input) INTEGER           _
C             The order of the matrices R, R, H and B.  M >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then R need not be  
C             set before entry.
C            
C     BETA    (input) DOUBLE PRECISION
C             The scalar beta. When beta is zero then H and B are not
C             referenced.
C            
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,M)
C             On entry with UPLO = 'U', the leading M-by-M upper
C             triangular part of this array must contain the upper
C             triangular part of the matrix R; the strictly lower
C             triangular part of the array is not referenced.
C             On entry with UPLO = 'L', the leading M-by-M lower
C             triangular part of this array must contain the lower
C             triangular part of the matrix R; the strictly upper
C             triangular part of the array is not referenced.
C             On exit, the leading M-by-M upper triangular part (if 
C             UPLO = 'U'), or lower triangular part (if UPLO = 'L') of
C             this array contains the corresponding triangular part of
C                                 _
C             the computed matrix R. 
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,M).
C
C     H       (input) DOUBLE PRECISION array, dimension (LDH,M)
C             On entry, the leading M-by-M upper Hessenberg part of
C             this array must contain the upper Hessenberg part of the
C             matrix H.
C             The elements below the subdiagonal are not referenced,
C             except possibly for those in the first column, which
C             could be overwritten, but are restored on exit.
C
C     LDH     INTEGER
C             The leading dimension of array H.  LDH >= MAX(1,M).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,M)
C             On entry, the leading M-by-M part of this array must
C             contain the matrix B.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             LDWORK >= M, if  beta <> 0 and SIDE = 'L';
C             LDWORK >= 0, if  beta =  0 or  SIDE = 'R'.
C             This array is not referenced when beta = 0 or SIDE = 'R'.
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
C     The matrix expression is efficiently evaluated taking the
C     Hessenberg/triangular structure into account. BLAS 2 operations
C     are used. A block algorithm can be constructed; it can use BLAS 3
C     GEMM operations for most computations, and calls of this BLAS 2
C     algorithm for computing the triangles.
C
C     FURTHER COMMENTS
C
C     The main application of this routine is when the result should
C     be a symmetric matrix, e.g., when B = X*op( H )', for (1), or
C     B = op( H )'*X, for (2), where B is already available and X = X'.
C     
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Feb. 1999.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Elementary matrix operations, matrix algebra, matrix operations.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         SIDE, TRANS, UPLO
      INTEGER           INFO, LDB, LDH, LDR, M
      DOUBLE PRECISION  ALPHA, BETA
C     .. Array Arguments ..
      DOUBLE PRECISION  B(LDB,*), DWORK(*), H(LDH,*), R(LDR,*)
C     .. Local Scalars ..
      LOGICAL           LSIDE, LTRANS, LUPLO
      INTEGER           I, J
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DDOT
      EXTERNAL          DDOT, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMV, DLASCL, DLASET, DSCAL, DSWAP,
     $                  DTRMV, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO   = 0
      LSIDE  = LSAME( SIDE,  'L' )
      LUPLO  = LSAME( UPLO,  'U' )
      LTRANS = LSAME( TRANS, 'T' ) .OR. LSAME( TRANS, 'C' )
C
      IF(      ( .NOT.LSIDE  ).AND.( .NOT.LSAME( SIDE,  'R' ) ) )THEN
         INFO = -1
      ELSE IF( ( .NOT.LUPLO  ).AND.( .NOT.LSAME( UPLO,  'L' ) ) )THEN
         INFO = -2
      ELSE IF( ( .NOT.LTRANS ).AND.( .NOT.LSAME( TRANS, 'N' ) ) )THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDR.LT.MAX( 1, M ) ) THEN
         INFO = -8
      ELSE IF( LDH.LT.MAX( 1, M ) ) THEN
         INFO = -10    
      ELSE IF( LDB.LT.MAX( 1, M ) ) THEN
         INFO = -12
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01RY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( M.EQ.0 ) 
     $   RETURN
C
      IF ( BETA.EQ.ZERO ) THEN
         IF ( ALPHA.EQ.ZERO ) THEN
C
C           Special case when both alpha = 0 and beta = 0.
C
            CALL DLASET( UPLO, M, M, ZERO, ZERO, R, LDR )
         ELSE
C
C           Special case beta = 0.
C
            IF ( ALPHA.NE.ONE ) 
     $         CALL DLASCL( UPLO, 0, 0, ONE, ALPHA, M, M, R, LDR, INFO )
         END IF
         RETURN
      END IF
C     
C     General case: beta <> 0.
C     Compute the required triangle of (1) or (2) using BLAS 2 
C     operations. 
C
      IF( LSIDE ) THEN
C
C        To avoid repeated references to the subdiagonal elements of H, 
C        these are swapped with the corresponding elements of H in the
C        first column, and are finally restored.
C 
         IF( M.GT.2 ) 
     $      CALL DSWAP( M-2, H( 3, 2 ), LDH+1, H( 3, 1 ), 1 )
C
         IF( LUPLO ) THEN
            IF ( LTRANS ) THEN
C
               DO 20 J = 1, M
C              
C                 Multiply the transposed upper triangle of the leading
C                 j-by-j submatrix of H by the leading part of the j-th
C                 column of B.
C
                  CALL DCOPY( J, B( 1, J ), 1, DWORK, 1 )
                  CALL DTRMV( 'Upper', TRANS, 'Non-unit', J, H, LDH,
     $                        DWORK, 1 )
C              
C                 Add the contribution of the subdiagonal of H to
C                 the j-th column of the product.
C              
                  DO 10 I = 1, MIN( J, M - 1 )
                     R( I, J ) = ALPHA*R( I, J ) + BETA*( DWORK( I ) +
     $                           H( I+1, 1 )*B( I+1, J ) )
   10             CONTINUE
C
   20          CONTINUE
C
               R( M, M ) = ALPHA*R( M, M ) + BETA*DWORK( M )
C     
            ELSE
C     
               DO 40 J = 1, M
C              
C                 Multiply the upper triangle of the leading j-by-j
C                 submatrix of H by the leading part of the j-th column
C                 of B.
C
                  CALL DCOPY( J, B( 1, J ), 1, DWORK, 1 )
                  CALL DTRMV( 'Upper', TRANS, 'Non-unit', J, H, LDH,
     $                        DWORK, 1 )
                  IF( J.LT.M ) THEN
C
C                    Multiply the remaining right part of the leading
C                    j-by-M submatrix of H by the trailing part of the
C                    j-th column of B.
C                    
                     CALL DGEMV( TRANS, J, M-J, BETA, H( 1, J+1 ), LDH,
     $                           B( J+1, J ), 1, ALPHA, R( 1, J ), 1 )
                  ELSE
                     CALL DSCAL( M, ALPHA, R( 1, M ), 1 ) 
                  END IF
C              
C                 Add the contribution of the subdiagonal of H to
C                 the j-th column of the product.
C
                  R( 1, J ) = R( 1, J ) + BETA*DWORK( 1 )
C              
                  DO 30 I = 2, J
                     R( I, J ) = R( I, J ) + BETA*( DWORK( I ) +
     $                           H( I, 1 )*B( I-1, J ) )
   30             CONTINUE
C
   40          CONTINUE
C
            END IF
C
         ELSE
C
            IF ( LTRANS ) THEN
C
               DO 60 J = M, 1, -1
C              
C                 Multiply the transposed upper triangle of the trailing
C                 (M-j+1)-by-(M-j+1) submatrix of H by the trailing part
C                 of the j-th column of B.
C
                  CALL DCOPY( M-J+1, B( J, J ), 1, DWORK( J ), 1 )
                  CALL DTRMV( 'Upper', TRANS, 'Non-unit', M-J+1,
     $                        H( J, J ), LDH, DWORK( J ), 1 )
                  IF( J.GT.1 ) THEN
C
C                    Multiply the remaining left part of the trailing 
C                    (M-j+1)-by-(j-1) submatrix of H' by the leading
C                    part of the j-th column of B.
C                    
                     CALL DGEMV( TRANS, J-1, M-J+1, BETA, H( 1, J ),
     $                           LDH, B( 1, J ), 1, ALPHA, R( J, J ), 
     $                           1 )
                  ELSE
                     CALL DSCAL( M, ALPHA, R( 1, 1 ), 1 ) 
                  END IF
C              
C                 Add the contribution of the subdiagonal of H to
C                 the j-th column of the product.
C
                  DO 50 I = J, M - 1
                     R( I, J ) = R( I, J ) + BETA*( DWORK( I ) +
     $                           H( I+1, 1 )*B( I+1, J ) )
   50             CONTINUE
C
                  R( M, J ) = R( M, J ) + BETA*DWORK( M )
   60          CONTINUE
C              
            ELSE
C
               DO 80 J = M, 1, -1
C              
C                 Multiply the upper triangle of the trailing
C                 (M-j+1)-by-(M-j+1) submatrix of H by the trailing
C                 part of the j-th column of B.
C
                  CALL DCOPY( M-J+1, B( J, J ), 1, DWORK( J ), 1 )
                  CALL DTRMV( 'Upper', TRANS, 'Non-unit', M-J+1,
     $                        H( J, J ), LDH, DWORK( J ), 1 )
C              
C                 Add the contribution of the subdiagonal of H to
C                 the j-th column of the product.
C                 
                  DO 70 I = MAX( J, 2 ), M
                     R( I, J ) = ALPHA*R( I, J ) + BETA*( DWORK( I )
     $                               + H( I, 1 )*B( I-1, J ) )
   70             CONTINUE
C
   80          CONTINUE
C
               R( 1, 1 ) = ALPHA*R( 1, 1 ) + BETA*DWORK( 1 )
C
            END IF
         END IF
C     
         IF( M.GT.2 ) 
     $      CALL DSWAP( M-2, H( 3, 2 ), LDH+1, H( 3, 1 ), 1 )
C
      ELSE
C     
C        Row-wise calculations are used for H, if SIDE = 'R' and 
C        TRANS = 'T'.
C           
         IF( LUPLO ) THEN
            IF( LTRANS ) THEN
               R( 1, 1 ) = ALPHA*R( 1, 1 ) + 
     $                     BETA*DDOT( M, B, LDB, H, LDH )
C
               DO 90 J = 2, M
                  CALL DGEMV( 'NoTranspose', J, M-J+2, BETA,
     $                        B( 1, J-1 ), LDB, H( J, J-1 ), LDH,
     $                        ALPHA, R( 1, J ), 1 )
   90          CONTINUE
C
            ELSE
C
               DO 100 J = 1, M - 1
                  CALL DGEMV( 'NoTranspose', J, J+1, BETA, B, LDB,
     $                        H( 1, J ), 1, ALPHA, R( 1, J ), 1 )
  100          CONTINUE
C
               CALL DGEMV( 'NoTranspose', M, M, BETA, B, LDB,
     $                     H( 1, M ), 1, ALPHA, R( 1, M ), 1 )
C     
            END IF
C
         ELSE
C
            IF( LTRANS ) THEN
C
               CALL DGEMV( 'NoTranspose', M, M, BETA, B, LDB, H, LDH,
     $                     ALPHA, R( 1, 1 ), 1 )
C
               DO 110 J = 2, M
                  CALL DGEMV( 'NoTranspose', M-J+1, M-J+2, BETA,
     $                        B( J, J-1 ), LDB, H( J, J-1 ), LDH, ALPHA,
     $                        R( J, J ), 1 )
  110          CONTINUE
C     
            ELSE
C     
               DO 120 J = 1, M - 1
                  CALL DGEMV( 'NoTranspose', M-J+1, J+1, BETA,
     $                        B( J, 1 ), LDB, H( 1, J ), 1, ALPHA,
     $                        R( J, J ), 1 )
  120          CONTINUE
C
               R( M, M ) = ALPHA*R( M, M ) + 
     $                     BETA*DDOT( M, B( M, 1 ), LDB, H( 1, M ), 1 )
C     
            END IF
         END IF
      END IF
C
      RETURN
C *** Last line of MB01RY ***
      END
