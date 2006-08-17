      SUBROUTINE MB01TD( N, A, LDA, B, LDB, DWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the matrix product A * B, where A and B are upper
C     quasi-triangular matrices (that is, block upper triangular with
C     1-by-1 or 2-by-2 diagonal blocks) with the same structure.
C     The result is returned in the array B.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A and B.  N >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             upper quasi-triangular matrix A. The elements below the
C             subdiagonal are not referenced.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
C             On entry, the leading N-by-N part of this array must
C             contain the upper quasi-triangular matrix B, with the same
C             structure as matrix A.
C             On exit, the leading N-by-N part of this array contains
C             the computed product A * B, with the same structure as
C             on entry.
C             The elements below the subdiagonal are not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.  LDB >= max(1,N).
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (N-1)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the matrices A and B have not the same structure,
C                   and/or A and B are not upper quasi-triangular.
C
C     METHOD
C
C     The matrix product A * B is computed column by column, using
C     BLAS 2 and BLAS 1 operations.
C
C     FURTHER COMMENTS
C
C     This routine can be used, for instance, for computing powers of
C     a real Schur form matrix.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, June 1998.
C
C     REVISIONS
C
C     V. Sima, Feb. 2000.
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
      INTEGER           INFO, LDA, LDB, N
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*)
C     .. Local Scalars ..
      INTEGER           I, J, JMIN, JMNM
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DTRMV, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, MIN
C
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO  = 0
      IF( N.LT.0 ) THEN
         INFO = -1
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -3
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -5
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01TD', -INFO )
         RETURN
      END IF
C
C     Quick return, if possible.
C
      IF ( N.EQ.0 ) THEN
         RETURN
      ELSE IF ( N.EQ.1 ) THEN
         B(1,1) = A(1,1)*B(1,1)
         RETURN
      END IF
C
C     Test the upper quasi-triangular structure of A and B for identity.
C
      DO 10 I = 1, N - 1
         IF ( A(I+1,I).EQ.ZERO ) THEN
            IF ( B(I+1,I).NE.ZERO ) THEN
               INFO = 1
               RETURN
            END IF
         ELSE IF ( I.LT.N-1 ) THEN
            IF ( A(I+2,I+1).NE.ZERO ) THEN
               INFO = 1
               RETURN
            END IF
         END IF
   10 CONTINUE
C
      DO 30 J = 1, N
         JMIN = MIN( J+1,  N )
         JMNM = MIN( JMIN, N-1 )
C
C        Compute the contribution of the subdiagonal of A to the
C        j-th column of the product.
C
         DO 20 I = 1, JMNM
            DWORK(I) = A(I+1,I)*B(I,J)
   20    CONTINUE
C
C        Multiply the upper triangle of A by the j-th column of B,
C        and add to the above result.
C
         CALL DTRMV( 'Upper', 'No transpose', 'Non-unit', JMIN, A, LDA,
     $               B(1,J), 1 )
         CALL DAXPY( JMNM, ONE, DWORK, 1, B(2,J), 1 )
   30 CONTINUE
C
      RETURN
C *** Last line of MB01TD ***
      END
