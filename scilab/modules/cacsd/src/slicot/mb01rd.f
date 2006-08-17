      SUBROUTINE MB01RD( UPLO, TRANS, M, N, ALPHA, BETA, R, LDR, A, LDA,
     $                   X, LDX, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
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
C     UPLO    CHARACTER*1                                         _
C             Specifies which triangles of the symmetric matrices R, R,
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
C             the call (which is possible only in this case).
C            
C     BETA    (input) DOUBLE PRECISION
C             The scalar beta. When beta is zero then A and X are not
C             referenced.
C            
C     R       (input/output) DOUBLE PRECISION array, dimension (LDR,M)
C             On entry with UPLO = 'U', the leading M-by-M upper
C             triangular part of this array must contain the upper
C             triangular part of the symmetric matrix R; the strictly
C             lower triangular part of the array is used as workspace.
C             On entry with UPLO = 'L', the leading M-by-M lower
C             triangular part of this array must contain the lower
C             triangular part of the symmetric matrix R; the strictly
C             upper triangular part of the array is used as workspace.
C             On exit, the leading M-by-M upper triangular part (if 
C             UPLO = 'U'), or lower triangular part (if UPLO = 'L'), of 
C             this array contains the corresponding triangular part of
C                                 _
C             the computed matrix R. If beta <> 0, the remaining
C             strictly triangular part of this array contains the 
C             corresponding part of the matrix expression 
C             beta*op( A )*T*op( A )', where T is the triangular matrix
C             defined in the Method section.
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
C             The leading dimension of array A.  LDA >= MAX(1,l),
C             where l is M when TRANS = 'N' and is N when TRANS = 'T' or
C             TRANS = 'C'.
C
C     X       (input/output) DOUBLE PRECISION array, dimension (LDX,N)
C             On entry, if UPLO = 'U', the leading N-by-N upper
C             triangular part of this array must contain the upper
C             triangular part of the symmetric matrix X and the strictly
C             lower triangular part of the array is not referenced.
C             On entry, if UPLO = 'L', the leading N-by-N lower
C             triangular part of this array must contain the lower
C             triangular part of the symmetric matrix X and the strictly
C             upper triangular part of the array is not referenced.
C             On exit, each diagonal element of this array has half its
C             input value, but the other elements are not modified.
C
C     LDX     INTEGER
C             The leading dimension of array X.  LDX >= MAX(1,N).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, the leading M-by-N part of this
C             array (with the leading dimension MAX(1,M)) returns the
C             matrix product beta*op( A )*T, where T is the triangular
C             matrix defined in the Method section.
C             This array is not referenced when beta = 0.
C
C     LDWORK  The length of the array DWORK.
C             LDWORK >= MAX(1,M*N), if  beta <> 0;
C             LDWORK >= 1,          if  beta =  0.
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
C        op( A )*X*op( A )' = B + B',
C     
C     where B := op( A )*T*op( A )'. Matrix B is not symmetric, but it
C     can be written as tri( B ) + stri( B ), where tri denotes the
C     triangular part specified by UPLO, and stri denotes the remaining
C     strictly triangular part. Let R = V + V', with V defined as T
C     above. Then, the required triangular part of the result can be
C     written as 
C
C        alpha*V + beta*tri( B )  + beta*(stri( B ))' +
C                 alpha*diag( V ) + beta*diag( tri( B ) ).
C
C     REFERENCES
C
C     None.
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
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Feb. 1997.
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
      DOUBLE PRECISION  ZERO, ONE, HALF
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0, HALF = 0.5D0 )
C     .. Scalar Arguments ..
      CHARACTER*1       TRANS, UPLO
      INTEGER           INFO, LDA, LDR, LDWORK, LDX, M, N
      DOUBLE PRECISION  ALPHA, BETA
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), DWORK(*), R(LDR,*), X(LDX,*)
C     .. Local Scalars ..
      CHARACTER*12      NTRAN
      LOGICAL           LTRANS, LUPLO
      INTEGER           J, JWORK, LDW, NROWA
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, DGEMM, DLACPY, DLASCL, DLASET,
     $                  DSCAL, DTRMM, XERBLA
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
      IF ( LTRANS ) THEN
         NROWA = N
         NTRAN = 'No transpose'
      ELSE
         NROWA = M
         NTRAN = 'Transpose'
      END IF
C
      LDW = MAX( 1, M )
C
      IF(      ( .NOT.LUPLO  ).AND.( .NOT.LSAME( UPLO,  'L' ) ) )THEN
         INFO = -1
      ELSE IF( ( .NOT.LTRANS ).AND.( .NOT.LSAME( TRANS, 'N' ) ) )THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( LDR.LT.LDW ) THEN
         INFO = -8
      ELSE IF( LDA.LT.MAX( 1, NROWA ) ) THEN
         INFO = -10    
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( ( BETA.NE.ZERO .AND. LDWORK.LT.MAX( 1, M*N ) ) 
     $     .OR.( BETA.EQ.ZERO .AND. LDWORK.LT.1 ) ) THEN
         INFO = -14
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01RD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( MAX( N, M ).EQ.0 ) 
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
C     General case: beta <> 0. Efficiently compute 
C        _
C        R = alpha*R + beta*op( A )*X*op( A )',
C
C     as described in the Method section.
C
C     Compute W = beta*op( A )*T in DWORK.
C     Workspace: need M*N.
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code.)
C
      IF( LTRANS ) THEN
         JWORK = 1
C
         DO 10 J = 1, N
            CALL DCOPY( M, A(J,1), LDA, DWORK(JWORK), 1 )
            JWORK = JWORK + LDW
 10      CONTINUE
C    
      ELSE
         CALL DLACPY( 'Full', M, N, A, LDA, DWORK, LDW )
      END IF
C
      CALL DSCAL( N, HALF, X, LDX+1 )
      CALL DTRMM( 'Right', UPLO, 'No transpose', 'Non-unit', M, N, BETA,
     $            X, LDX, DWORK, LDW )
C
C     Compute Y = alpha*V + W*op( A )' in R. First, set to zero the
C     strictly triangular part of R not specified by UPLO. That part
C     will then contain beta*stri( B ).
C
      IF ( ALPHA.NE.ZERO ) THEN
         IF ( M.GT.1 ) THEN
            IF ( LUPLO ) THEN
               CALL DLASET( 'Lower', M-1, M-1, ZERO, ZERO, R(2,1), LDR )
            ELSE
               CALL DLASET( 'Upper', M-1, M-1, ZERO, ZERO, R(1,2), LDR )
            END IF
         END IF
         CALL DSCAL( M, HALF, R, LDR+1 )
      END IF
C
      CALL DGEMM( 'No transpose', NTRAN, M, M, N, ONE, DWORK, LDW, A,
     $            LDA, ALPHA, R, LDR )
C
C     Add the term corresponding to B', with B = op( A )*T*op( A )'.
C
      IF( LUPLO ) THEN
C
         DO 20 J = 1, M
            CALL DAXPY( J, ONE, R(J,1), LDR, R(1,J), 1 ) 
   20    CONTINUE
C
      ELSE
C
         DO 30 J = 1, M
            CALL DAXPY( J, ONE, R(1,J), 1, R(J,1), LDR ) 
 30      CONTINUE
C    
      END IF
C
      RETURN
C *** Last line of MB01RD ***
      END
