      SUBROUTINE MA02AD( JOB, M, N, A, LDA, B, LDB )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To transpose all or part of a two-dimensional matrix A into 
C     another matrix B.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOB     CHARACTER*1
C             Specifies the part of the matrix A to be transposed into B
C             as follows:
C             = 'U': Upper triangular part;
C             = 'L': Lower triangular part;
C             Otherwise:  All of the matrix A.
C      
C     Input/Output Parameters
C
C     M      (input) INTEGER
C            The number of rows of the matrix A.  M >= 0.
C
C     N      (input) INTEGER
C            The number of columns of the matrix A.  N >= 0.
C
C     A      (input) DOUBLE PRECISION array, dimension (LDA,N)
C            The m-by-n matrix A.  If JOB = 'U', only the upper 
C            triangle or trapezoid is accessed; if JOB = 'L', only the 
C            lower triangle or trapezoid is accessed.
C
C     LDA    INTEGER
C            The leading dimension of the array A.  LDA >= max(1,M).
C
C     B      (output) DOUBLE PRECISION array, dimension (LDB,M)
C            B = A' in the locations specified by JOB.
C
C     LDB    INTEGER
C            The leading dimension of the array B.  LDB >= max(1,N).
C
C     CONTRIBUTOR
C
C     A. Varga, German Aerospace Center,
C     DLR Oberpfaffenhofen, March 1998.
C     Based on the RASP routine DMTRA.
C
C     REVISIONS
C
C     -   
C
C     ******************************************************************
C
C     .. Scalar Arguments ..
      CHARACTER          JOB
      INTEGER            LDA, LDB, M, N
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA,*), B(LDB,*)
C     .. Local Scalars ..
      INTEGER            I, J
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. Intrinsic Functions ..
      INTRINSIC          MIN
C
C     .. Executable Statements ..
C
      IF( LSAME( JOB, 'U' ) ) THEN
         DO 20 J = 1, N
            DO 10 I = 1, MIN( J, M )
               B(J,I) = A(I,J)
   10       CONTINUE
   20    CONTINUE
      ELSE IF( LSAME( JOB, 'L' ) ) THEN
         DO 40 J = 1, N
            DO 30 I = J, M
               B(J,I) = A(I,J)
   30       CONTINUE
   40    CONTINUE
      ELSE
         DO 60 J = 1, N
            DO 50 I = 1, M
               B(J,I) = A(I,J)
   50       CONTINUE
   60    CONTINUE
      END IF
C
      RETURN
C *** Last line of MA02AD ***
      END
