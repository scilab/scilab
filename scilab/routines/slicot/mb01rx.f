      SUBROUTINE MB01RX( SIDE, UPLO, TRANS, M, N, ALPHA, BETA, R, LDR,
     $                   A, LDA, B, LDB, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute either the upper or lower triangular part of one of the
C     matrix formulas
C        _
C        R = alpha*R + beta*op( A )*B,                               (1)
C        _
C        R = alpha*R + beta*B*op( A ),                               (2)
C                                             _     
C     where alpha and beta are scalars, R and R are m-by-m matrices,
C     op( A ) and B are m-by-n and n-by-m matrices for (1), or n-by-m
C     and m-by-n matrices for (2), respectively, and op( A ) is one of
C     
C        op( A ) = A   or   op( A ) = A',  the transpose of A.
C     
C     The result is overwritten on R. 
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     SIDE    CHARACTER*1
C             Specifies whether the matrix A appears on the left or
C             right in the matrix product as follows:
C                     _                           
C             = 'L':  R = alpha*R + beta*op( A )*B;
C                     _                           
C             = 'R':  R = alpha*R + beta*B*op( A ).
C            
C     UPLO    CHARACTER*1                               _
C             Specifies which triangles of the matrices R and R are
C             computed and given, respectively, as follows:
C             = 'U':  the upper triangular part;
C             = 'L':  the lower triangular part.
C            
C     TRANS   CHARACTER*1
C             Specifies the form of op( A ) to be used in the matrix
C             multiplication as follows:
C             = 'N':  op( A ) = A;
C             = 'T':  op( A ) = A';
C             = 'C':  op( A ) = A'.
C            
C     Input/Output Parameters
C
C     M       (input) INTEGER           _
C             The order of the matrices R and R, the number of rows of
C             the matrix op( A ) and the number of columns of the 
C             matrix B, for SIDE = 'L', or the number of rows of the
C             matrix B and the number of columns of the matrix op( A ), 
C             for SIDE = 'R'.  M >= 0.
C
C     N       (input) INTEGER
C             The number of rows of the matrix B and the number of 
C             columns of the matrix op( A ), for SIDE = 'L', or the 
C             number of rows of the matrix op( A ) and the number of
C             columns of the matrix B, for SIDE = 'R'.  N >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then R need not be  
C             set before entry.
C            
C     BETA    (input) DOUBLE PRECISION
C             The scalar beta. When beta is zero then A and B are not
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
C     A       (input) DOUBLE PRECISION array, dimension (LDA,k), where
C             k = N  when  SIDE = 'L', and TRANS = 'N', or 
C                          SIDE = 'R', and TRANS = 'T'; 
C             k = M  when  SIDE = 'R', and TRANS = 'N', or 
C                          SIDE = 'L', and TRANS = 'T'.
C             On entry, if SIDE = 'L', and TRANS = 'N', or 
C                          SIDE = 'R', and TRANS = 'T', 
C             the leading M-by-N part of this array must contain the
C             matrix A.
C             On entry, if SIDE = 'R', and TRANS = 'N', or 
C                          SIDE = 'L', and TRANS = 'T', 
C             the leading N-by-M part of this array must contain the
C             matrix A.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,l), where
C             l = M  when  SIDE = 'L', and TRANS = 'N', or 
C                          SIDE = 'R', and TRANS = 'T'; 
C             l = N  when  SIDE = 'R', and TRANS = 'N', or 
C                          SIDE = 'L', and TRANS = 'T'.
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,p), where
C             p = M  when  SIDE = 'L'; 
C             p = N  when  SIDE = 'R'.
C             On entry, the leading N-by-M part, if SIDE = 'L', or 
C             M-by-N part, if SIDE = 'R', of this array must contain the
C             matrix B.
C
C     LDB     INTEGER
C             The leading dimension of array B.  
C             LDB >= MAX(1,N), if SIDE = 'L';
C             LDB >= MAX(1,M), if SIDE = 'R'.
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
C     The matrix expression is evaluated taking the triangular
C     structure into account. BLAS 2 operations are used. A block
C     algorithm can be easily constructed; it can use BLAS 3 GEMM
C     operations for most computations, and calls of this BLAS 2
C     algorithm for computing the triangles.
C
C     FURTHER COMMENTS
C
C     The main application of this routine is when the result should
C     be a symmetric matrix, e.g., when B = X*op( A )', for (1), or
C     B = op( A )'*X, for (2), where B is already available and X = X'.
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
      INTEGER           INFO, LDA, LDB, LDR, M, N
      DOUBLE PRECISION  ALPHA, BETA
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), R(LDR,*)
C     .. Local Scalars ..
      LOGICAL           LSIDE, LTRANS, LUPLO
      INTEGER           J
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DGEMV, DLASCL, DLASET, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
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
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDR.LT.MAX( 1, M ) ) THEN
         INFO = -9
      ELSE IF( LDA.LT.1 .OR. 
     $   ( ( (      LSIDE .AND. .NOT.LTRANS ) .OR.
     $       ( .NOT.LSIDE .AND.      LTRANS ) ) .AND. LDA.LT.M ) .OR.
     $   ( ( (      LSIDE .AND.      LTRANS ) .OR.
     $       ( .NOT.LSIDE .AND. .NOT.LTRANS ) ) .AND. LDA.LT.N ) ) THEN
         INFO = -11
      ELSE IF( LDB.LT.1 .OR. 
     $       (      LSIDE .AND. LDB.LT.N ) .OR.
     $       ( .NOT.LSIDE .AND. LDB.LT.M ) ) THEN
         INFO = -13
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01RX', -INFO )
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
      IF ( N.EQ.0 ) 
     $   RETURN
C     
C     General case: beta <> 0.
C     Compute the required triangle of (1) or (2) using BLAS 2 
C     operations.
C
      IF( LSIDE ) THEN
         IF( LUPLO ) THEN
            IF ( LTRANS ) THEN
               DO 10 J = 1, M
                  CALL DGEMV( TRANS, N, J, BETA, A, LDA, B(1,J), 1,
     $                        ALPHA, R(1,J), 1 )
   10          CONTINUE
            ELSE
               DO 20 J = 1, M
                  CALL DGEMV( TRANS, J, N, BETA, A, LDA, B(1,J), 1,
     $                        ALPHA, R(1,J), 1 )
   20          CONTINUE
            END IF
         ELSE
            IF ( LTRANS ) THEN
               DO 30 J = 1, M
                  CALL DGEMV( TRANS, N, M-J+1, BETA, A(1,J), LDA,
     $                        B(1,J), 1, ALPHA, R(J,J), 1 )
   30          CONTINUE
            ELSE
               DO 40 J = 1, M
                  CALL DGEMV( TRANS, M-J+1, N, BETA, A(J,1), LDA,
     $                        B(1,J), 1, ALPHA, R(J,J), 1 )
   40          CONTINUE
            END IF
         END IF
C
      ELSE
         IF( LUPLO ) THEN
            IF( LTRANS ) THEN
               DO 50 J = 1, M
                  CALL DGEMV( 'NoTranspose', J, N, BETA, B, LDB, A(J,1),
     $                        LDA, ALPHA, R(1,J), 1 )
   50          CONTINUE
            ELSE
               DO 60 J = 1, M
                  CALL DGEMV( 'NoTranspose', J, N, BETA, B, LDB, A(1,J),
     $                        1, ALPHA, R(1,J), 1 )
   60          CONTINUE
            END IF
         ELSE
            IF( LTRANS ) THEN
               DO 70 J = 1, M
                  CALL DGEMV( 'NoTranspose', M-J+1, N, BETA, B(J,1),
     $                        LDB, A(J,1), LDA, ALPHA, R(J,J), 1 )
   70           CONTINUE
            ELSE
               DO 80 J = 1, M
                  CALL DGEMV( 'NoTranspose', M-J+1, N, BETA, B(J,1),
     $                        LDB, A(1,J), 1, ALPHA, R(J,J), 1 )
   80          CONTINUE
            END IF
         END IF
      END IF
C
      RETURN
C *** Last line of MB01RX ***
      END
