      SUBROUTINE MB01SD( JOBS, M, N, A, LDA, R, C )
C
C     RELEASE 3.0, WGS COPYRIGHT 1998.
C
C     PURPOSE
C
C     To scale a general M-by-N matrix A using the row and column
C     scaling factors in the vectors R and C.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOBS    CHARACTER*1
C             Specifies the scaling operation to be done, as follows:
C             = 'R':  row scaling, i.e., A will be premultiplied  
C                     by diag(R);
C             = 'C':  column scaling, i.e., A will be postmultiplied
C                     by diag(C);
C             = 'B':  both row and column scaling, i.e., A will be
C                     replaced by diag(R) * A * diag(C).
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrix A.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrix A.  N >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the M-by-N matrix A. 
C             On exit, the scaled matrix.  See JOBS for the form of the
C             scaled matrix.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C
C     R       (input) DOUBLE PRECISION array, dimension (M)
C             The row scale factors for A. 
C             R is not referenced if JOBS = 'C'.
C
C     C       (input) DOUBLE PRECISION array, dimension (N)
C             The column scale factors for A.
C             C is not referenced if JOBS = 'R'.
C
C
C     CONTRIBUTOR
C
C     A. Varga, German Aerospace Center,
C     DLR Oberpfaffenhofen, April 1998.
C     Based on the RASP routine DMSCAL.
C
C    ******************************************************************
C
C     .. Scalar Arguments ..
      CHARACTER          JOBS
      INTEGER            LDA, M, N
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA,*), C(*), R(*)
C     .. Local Scalars ..
      INTEGER            I, J
      DOUBLE PRECISION   CJ
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     .. Executable Statements ..
C
C     Quick return if possible.
C
      IF( M.EQ.0 .OR. N.EQ.0 )
     $   RETURN
C
      IF( LSAME( JOBS, 'C' ) ) THEN
C
C        Column scaling, no row scaling.
C
         DO 20 J = 1, N
            CJ = C(J)
            DO 10 I = 1, M
               A(I,J) = CJ*A(I,J)
   10       CONTINUE
   20    CONTINUE
      ELSE IF( LSAME( JOBS, 'R' ) ) THEN
C
C        Row scaling, no column scaling.
C
         DO 40 J = 1, N
            DO 30 I = 1, M
               A(I,J) = R(I)*A(I,J)
   30       CONTINUE
   40    CONTINUE
      ELSE IF( LSAME( JOBS, 'B' ) ) THEN
C
C        Row and column scaling.
C
         DO 60 J = 1, N
            CJ = C(J)
            DO 50 I = 1, M
               A(I,J) = CJ*R(I)*A(I,J)
   50       CONTINUE
   60    CONTINUE
      END IF
C
      RETURN
C *** Last line of MB01SD ***
      END
