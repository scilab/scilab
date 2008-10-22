      SUBROUTINE MB01RU( UPLO, TRANS, M, N, ALPHA, BETA, R, LDR, A, LDA,
     $                   X, LDX, DWORK, LDWORK, INFO )
C
C     RELEASE 3.0, WGS COPYRIGHT 1997.
C
C     PURPOSE
C
C     To compute the matrix formula
C        _
C        R = alpha*R + beta*op( A )*X*op( A )',
C                                                 _     
C     where alpha and beta are scalars, R, X, and R are symmetric 
C     matrices, A is a general matrix, and op( A ) is one of
C     
C        op( A ) = A   or   op( A ) = A'.
C     
C     The result is overwritten on R. 
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     UPLO    CHARACTER*1                                         
C             Specifies which triangles of the symmetric matrices R 
C             and X are given as follows:
C             = 'U':  the upper triangular part is given;
C             = 'L':  the lower triangular part is given.
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
C             The order of the matrices R and R and the number of rows
C             of the matrix op( A ).  M >= 0.
C
C     N       (input) INTEGER
C             The order of the matrix X and the number of columns of the
C             the matrix op( A ).  N >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then R need not be  
C             set before entry, except when R is identified with X in
C             the call.
C            
C     BETA    (input) DOUBLE PRECISION
C             The scalar beta. When beta is zero then A and X are not
C             referenced.
C            
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,M)
C             On entry with UPLO = 'U', the leading M-by-M upper
C             triangular part of this array must contain the upper
C             triangular part of the symmetric matrix R.
C             On entry with UPLO = 'L', the leading M-by-M lower
C             triangular part of this array must contain the lower
C             triangular part of the symmetric matrix R.
C             On exit, the leading M-by-M upper triangular part (if 
C             UPLO = 'U'), or lower triangular part (if UPLO = 'L'), of 
C             this array contains the corresponding triangular part of
C                                 _
C             the computed matrix R.
C
C     LDR     INTEGER
C             The leading dimension of array R.  LDR >= MAX(1,M).
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,k)
C             where k is N when TRANS = 'N' and is M when TRANS = 'T' or
C             TRANS = 'C'.
C             On entry with TRANS = 'N', the leading M-by-N part of this
C             array must contain the matrix A.
C             On entry with TRANS = 'T' or TRANS = 'C', the leading
C             N-by-M part of this array must contain the matrix A.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,k),
C             where k is M when TRANS = 'N' and is N when TRANS = 'T' or
C             TRANS = 'C'.
C
C     X       (input) DOUBLE PRECISION array, dimension (LDX,N)
C             On entry, if UPLO = 'U', the leading N-by-N upper
C             triangular part of this array must contain the upper
C             triangular part of the symmetric matrix X and the strictly
C             lower triangular part of the array is not referenced.
C             On entry, if UPLO = 'L', the leading N-by-N lower
C             triangular part of this array must contain the lower
C             triangular part of the symmetric matrix X and the strictly
C             upper triangular part of the array is not referenced.
C             The diagonal elements of this array are modified
C             internally, but are restored on exit.
C
C     LDX     INTEGER
C             The leading dimension of array X.  LDX >= MAX(1,N).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             This array is not referenced when beta = 0, or M*N = 0.
C
C     LDWORK  The length of the array DWORK.
C             LDWORK >= M*N, if  beta <> 0;
C             LDWORK >= 0,   if  beta =  0.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -k, the k-th argument had an illegal
C                   value.
C
C     METHOD
C
C     The matrix expression is efficiently evaluated taking the symmetry
C     into account. Specifically, let X = T + T', with T an upper or
C     lower triangular matrix, defined by 
C
C        T = triu( X ) - (1/2)*diag( X ),  if UPLO = 'U',
C        T = tril( X ) - (1/2)*diag( X ),  if UPLO = 'L',
C     
C     where triu, tril, and diag denote the upper triangular part, lower
C     triangular part, and diagonal part of X, respectively. Then,
C
C        A*X*A' = ( A*T )*A' + A*( A*T )',  for TRANS = 'N',
C        A'*X*A = A'*( T*A ) + ( T*A )'*A,  for TRANS = 'T', or 'C',
C     
C     which involve BLAS 3 operations (DTRMM and DSYR2K).
C
C     NUMERICAL ASPECTS
C
C     The algorithm requires approximately
C
C                   2         2
C        3/2 x M x N + 1/2 x M 
C
C     operations.
C
C     FURTHER COMMENTS
C
C     This is a simpler version for MB01RD.
C
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Jan. 1999.
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
      DOUBLE PRECISION  ZERO, ONE, TWO, HALF
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0, TWO = 2.0D0, 
     $                  HALF = 0.5D0 )
C     .. Scalar Arguments ..
      CHARACTER         TRANS, UPLO
      INTEGER           INFO, LDA, LDR, LDWORK, LDX, M, N
      DOUBLE PRECISION  ALPHA, BETA
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), DWORK(*), R(LDR,*), X(LDX,*)
C     .. Local Scalars ..
      LOGICAL           LTRANS, LUPLO
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DLACPY, DLASCL, DLASET, DSCAL, DSYR2K, DTRMM, 
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO = 0
      LUPLO  = LSAME( UPLO,  'U' )
      LTRANS = LSAME( TRANS, 'T' ) .OR. LSAME( TRANS, 'C' )
C
      IF(      ( .NOT.LUPLO  ).AND.( .NOT.LSAME( UPLO,  'L' ) ) )THEN
         INFO = -1
      ELSE IF( ( .NOT.LTRANS ).AND.( .NOT.LSAME( TRANS, 'N' ) ) )THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDR.LT.M ) THEN
         INFO = -8
      ELSE IF( LDA.LT.1 .OR. ( LTRANS .AND. LDA.LT.N ) .OR.
     $                  ( .NOT.LTRANS .AND. LDA.LT.M ) ) THEN
         INFO = -10    
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( ( BETA.NE.ZERO .AND. LDWORK.LT.M*N ) 
     $     .OR.( BETA.EQ.ZERO .AND. LDWORK.LT.0 ) ) THEN
         INFO = -14
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01RU', -INFO )
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
C     Compute W = op( A )*T or W = T*op( A ) in DWORK, and apply the
C     updating formula (see METHOD section).
C     Workspace: need M*N.
C
      CALL DSCAL( N, HALF, X, LDX+1 )
C
      IF( LTRANS ) THEN
C
         CALL DLACPY( 'Full', N, M, A, LDA, DWORK, N )
         CALL DTRMM(  'Left', UPLO, 'NoTranspose', 'Non-unit', N, M,
     $                ONE, X, LDX, DWORK, N )
         CALL DSCAL( N, TWO, X, LDX+1 )
         CALL DSYR2K( UPLO, TRANS, M, N, BETA, DWORK, N, A, LDA, ALPHA,
     $                R, LDR )
C
      ELSE
C
         CALL DLACPY( 'Full', M, N, A, LDA, DWORK, M )
         CALL DTRMM(  'Right', UPLO, 'NoTranspose', 'Non-unit', M, N,
     $                ONE, X, LDX, DWORK, M )
         CALL DSCAL( N, TWO, X, LDX+1 )
         CALL DSYR2K( UPLO, TRANS, M, N, BETA, DWORK, M, A, LDA, ALPHA,
     $                R, LDR )
C
      END IF
C
      RETURN
C *** Last line of MB01RU ***
      END
