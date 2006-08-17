      SUBROUTINE MB01QD( TYPE, M, N, KL, KU, CFROM, CTO, NBL, NROWS, A,
     $                   LDA, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To multiply the M by N real matrix A by the real scalar CTO/CFROM.
C     This is done without over/underflow as long as the final result
C     CTO*A(I,J)/CFROM does not over/underflow. TYPE specifies that
C     A may be full, (block) upper triangular, (block) lower triangular,
C     (block) upper Hessenberg, or banded.
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     TYPE    CHARACTER*1
C             TYPE indices the storage type of the input matrix.
C             = 'G':  A is a full matrix.
C             = 'L':  A is a (block) lower triangular matrix.
C             = 'U':  A is a (block) upper triangular matrix.
C             = 'H':  A is a (block) upper Hessenberg matrix.
C             = 'B':  A is a symmetric band matrix with lower bandwidth
C                     KL and upper bandwidth KU and with the only the
C                     lower half stored.
C             = 'Q':  A is a symmetric band matrix with lower bandwidth
C                     KL and upper bandwidth KU and with the only the
C                     upper half stored.
C             = 'Z':  A is a band matrix with lower bandwidth KL and
C                     upper bandwidth KU.
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The number of rows of the matrix A.  M >= 0.
C
C     N       (input) INTEGER
C             The number of columns of the matrix A.  N >= 0.
C
C     KL      (input) INTEGER
C             The lower bandwidth of A.  Referenced only if TYPE = 'B',
C             'Q' or 'Z'.
C
C     KU      (input) INTEGER
C             The upper bandwidth of A.  Referenced only if TYPE = 'B',
C             'Q' or 'Z'.
C
C     CFROM   (input) DOUBLE PRECISION
C     CTO     (input) DOUBLE PRECISION
C             The matrix A is multiplied by CTO/CFROM. A(I,J) is
C             computed without over/underflow if the final result
C             CTO*A(I,J)/CFROM can be represented without over/
C             underflow.  CFROM must be nonzero.
C
C     NBL     (input) INTEGER
C             The number of diagonal blocks of the matrix A, if it has a
C             block structure.  To specify that matrix A has no block
C             structure, set NBL = 0.  NBL >= 0.
C
C     NROWS   (input) INTEGER array, dimension max(1,NBL)
C             NROWS(i) contains the number of rows and columns of the 
C             i-th diagonal block of matrix A.  The sum of the values
C             NROWS(i),  for  i = 1: NBL,  should be equal to min(M,N).
C             The array  NROWS  is not referenced if NBL = 0.
C            
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             The matrix to be multiplied by CTO/CFROM.  See TYPE for
C             the storage type.
C
C     LDA     (input) INTEGER
C             The leading dimension of the array A.  LDA >= max(1,M).
C
C     Error Indicator
C
C     INFO    INTEGER
C             Not used in this implementation.
C
C     METHOD
C
C     Matrix A is multiplied by the real scalar CTO/CFROM, taking into
C     account the specified storage mode of the matrix.
C     MB01QD is a version of the LAPACK routine DLASCL, modified for
C     dealing with block triangular, or block Hessenberg matrices.
C     For efficiency, no tests of the input scalar parameters are
C     performed.
C      
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Nov. 1996.
C
C    ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          TYPE
      INTEGER            INFO, KL, KU, LDA, M, N, NBL
      DOUBLE PRECISION   CFROM, CTO
C     ..
C     .. Array Arguments ..
      INTEGER            NROWS ( * )
      DOUBLE PRECISION   A( LDA, * )
C     ..
C     .. Local Scalars ..
      LOGICAL            DONE, NOBLC
      INTEGER            I, IFIN, ITYPE, J, JFIN, JINI, K, K1, K2, K3, 
     $                   K4
      DOUBLE PRECISION   BIGNUM, CFROM1, CFROMC, CTO1, CTOC, MUL, SMLNUM
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           LSAME, DLAMCH
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX, MIN
C     ..
C     .. Executable Statements ..
C
      IF( LSAME( TYPE, 'G' ) ) THEN
         ITYPE = 0
      ELSE IF( LSAME( TYPE, 'L' ) ) THEN
         ITYPE = 1
      ELSE IF( LSAME( TYPE, 'U' ) ) THEN
         ITYPE = 2
      ELSE IF( LSAME( TYPE, 'H' ) ) THEN
         ITYPE = 3
      ELSE IF( LSAME( TYPE, 'B' ) ) THEN
         ITYPE = 4
      ELSE IF( LSAME( TYPE, 'Q' ) ) THEN
         ITYPE = 5
      ELSE 
         ITYPE = 6
      END IF
C
C     Quick return if possible.
C
      IF( MIN( M, N ).EQ.0 )
     $   RETURN
C
C     Get machine parameters.
C
      SMLNUM = DLAMCH( 'S' )
      BIGNUM = ONE / SMLNUM
C
      CFROMC = CFROM
      CTOC = CTO
C
   10 CONTINUE
      CFROM1 = CFROMC*SMLNUM
      CTO1 = CTOC / BIGNUM
      IF( ABS( CFROM1 ).GT.ABS( CTOC ) .AND. CTOC.NE.ZERO ) THEN
         MUL = SMLNUM
         DONE = .FALSE.
         CFROMC = CFROM1
      ELSE IF( ABS( CTO1 ).GT.ABS( CFROMC ) ) THEN
         MUL = BIGNUM
         DONE = .FALSE.
         CTOC = CTO1
      ELSE
         MUL = CTOC / CFROMC
         DONE = .TRUE.
      END IF
C
      NOBLC = NBL.EQ.0
C
      IF( ITYPE.EQ.0 ) THEN
C
C        Full matrix
C
         DO 30 J = 1, N
            DO 20 I = 1, M
               A( I, J ) = A( I, J )*MUL
   20       CONTINUE
   30    CONTINUE
C
      ELSE IF( ITYPE.EQ.1 ) THEN
C
         IF ( NOBLC ) THEN
C
C           Lower triangular matrix
C
            DO 50 J = 1, N
               DO 40 I = J, M
                  A( I, J ) = A( I, J )*MUL
   40          CONTINUE
   50       CONTINUE
C
         ELSE
C
C           Block lower triangular matrix
C
            JFIN = 0
            DO 80 K = 1, NBL
               JINI = JFIN + 1
               JFIN = JFIN + NROWS( K )
               DO 70 J = JINI, JFIN
                  DO 60 I = JINI, M
                     A( I, J ) = A( I, J )*MUL
   60             CONTINUE
   70          CONTINUE
   80       CONTINUE
         END IF
C
      ELSE IF( ITYPE.EQ.2 ) THEN
C
         IF ( NOBLC ) THEN
C
C           Upper triangular matrix
C
            DO 100 J = 1, N
               DO 90 I = 1, MIN( J, M )
                  A( I, J ) = A( I, J )*MUL
   90          CONTINUE
  100       CONTINUE
C
         ELSE
C
C           Block upper triangular matrix
C
            JFIN = 0
            DO 130 K = 1, NBL
               JINI = JFIN + 1
               JFIN = JFIN + NROWS( K )
               IF ( K.EQ.NBL ) JFIN = N
               DO 120 J = JINI, JFIN
                  DO 110 I = 1, MIN( JFIN, M )
                     A( I, J ) = A( I, J )*MUL
  110             CONTINUE
  120          CONTINUE
  130       CONTINUE
         END IF
C
      ELSE IF( ITYPE.EQ.3 ) THEN
C
         IF ( NOBLC ) THEN
C
C           Upper Hessenberg matrix
C 
            DO 150 J = 1, N
               DO 140 I = 1, MIN( J+1, M )
                  A( I, J ) = A( I, J )*MUL
  140          CONTINUE
  150       CONTINUE
C
         ELSE
C
C           Block upper Hessenberg matrix
C
            JFIN = 0
            DO 180 K = 1, NBL
               JINI = JFIN + 1
               JFIN = JFIN + NROWS( K )
C
               IF ( K.EQ.NBL ) THEN
                  JFIN = N
                  IFIN = N
               ELSE
                  IFIN = JFIN + NROWS( K+1 )
               END IF 
C
               DO 170 J = JINI, JFIN
                  DO 160 I = 1, MIN( IFIN, M )
                     A( I, J ) = A( I, J )*MUL
  160             CONTINUE
  170          CONTINUE
  180       CONTINUE
         END IF
C
      ELSE IF( ITYPE.EQ.4 ) THEN
C
C        Lower half of a symmetric band matrix
C
         K3 = KL + 1
         K4 = N + 1
         DO 200 J = 1, N
            DO 190 I = 1, MIN( K3, K4-J )
               A( I, J ) = A( I, J )*MUL
  190       CONTINUE
  200    CONTINUE
C
      ELSE IF( ITYPE.EQ.5 ) THEN
C
C        Upper half of a symmetric band matrix
C
         K1 = KU + 2
         K3 = KU + 1
         DO 220 J = 1, N
            DO 210 I = MAX( K1-J, 1 ), K3
               A( I, J ) = A( I, J )*MUL
  210       CONTINUE
  220    CONTINUE
C
      ELSE IF( ITYPE.EQ.6 ) THEN
C
C        Band matrix
C
         K1 = KL + KU + 2
         K2 = KL + 1
         K3 = 2*KL + KU + 1
         K4 = KL + KU + 1 + M
         DO 240 J = 1, N
            DO 230 I = MAX( K1-J, K2 ), MIN( K3, K4-J )
               A( I, J ) = A( I, J )*MUL
  230       CONTINUE
  240    CONTINUE
C
      END IF
C
      IF( .NOT.DONE )
     $   GO TO 10
C
      RETURN
C *** Last line of MB01QD ***
      END
