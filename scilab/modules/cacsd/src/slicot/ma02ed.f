      SUBROUTINE MA02ED( UPLO, N, A, LDA )
C
C     RELEASE 3.0, WGS COPYRIGHT 1998.
C
C     PURPOSE
C
C     To store by symmetry the upper or lower triangle of a symmetric
C     matrix, given the other triangle.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     UPLO    CHARACTER*1
C             Specifies which part of the matrix is given as follows:
C             = 'U':  Upper triangular part;
C             = 'L':  Lower triangular part.
C             For all other values, the array A is not referenced.
C      
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix A.  N >= 0.
C             
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N upper triangular part
C             (if UPLO = 'U'), or lower triangular part (if UPLO = 'L'),
C             of this array must contain the corresponding upper or
C             lower triangle of the symmetric matrix A.
C             On exit, the leading N-by-N part of this array contains
C             the symmetric matrix A with all elements stored.
C             
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,N).
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Romania,
C     Oct. 1998.
C
C     REVISIONS
C
C     -   
C
C     ******************************************************************
C
C     .. Scalar Arguments ..
      CHARACTER          UPLO
      INTEGER            LDA, N
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA,*)
C     .. Local Scalars ..
      INTEGER            J
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. External Subroutines ..
      EXTERNAL           DCOPY
C
C     .. Executable Statements ..
C
C     For efficiency reasons, the parameters are not checked for errors.
C
      IF( LSAME( UPLO, 'L' ) ) THEN
C
C        Construct the upper triangle of A.
C
         DO 20 J = 2, N
            CALL DCOPY( J-1, A(J,1), LDA, A(1,J), 1 )
   20    CONTINUE
C
      ELSE IF( LSAME( UPLO, 'U' ) ) THEN
C
C        Construct the lower triangle of A.
C
         DO 40 J = 2, N
            CALL DCOPY( J-1, A(1,J), 1, A(J,1), LDA )
   40    CONTINUE
C
      END IF
      RETURN
C *** Last line of MA02ED ***
      END
