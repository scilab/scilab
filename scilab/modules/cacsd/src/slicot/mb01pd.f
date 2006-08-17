      SUBROUTINE MB01PD( SCUN, TYPE, M, N, KL, KU, ANRM, NBL, NROWS, A,
     $                   LDA, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To scale a matrix or undo scaling.  Scaling is performed, if 
C     necessary, so that the matrix norm will be in a safe range of 
C     representable numbers.  
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     SCUN    CHARACTER*1
C             SCUN indicates the operation to be performed.
C             = 'S':  scale the matrix.
C             = 'U':  undo scaling of the matrix.
C            
C     TYPE    CHARACTER*1
C             TYPE indicates the storage type of the input matrix.
C             = 'G':  A is a full matrix.
C             = 'L':  A is a (block) lower triangular matrix.
C             = 'U':  A is an (block) upper triangular matrix.
C             = 'H':  A is an (block) upper Hessenberg matrix.
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
C             The number of rows of the matrix A. M >= 0. 
C            
C     N       (input) INTEGER
C             The number of columns of the matrix A. N >= 0. 
C            
C     KL      (input) INTEGER
C             The lower bandwidth of A.  Referenced only if TYPE = 'B',
C             'Q' or 'Z'.
C            
C     KU      (input) INTEGER
C             The upper bandwidth of A.  Referenced only if TYPE = 'B',
C             'Q' or 'Z'.
C            
C     ANRM    (input) DOUBLE PRECISION
C             The norm of the initial matrix A.  ANRM >= 0.
C             When  ANRM = 0  then an immediate return is effected.
C             ANRM should be preserved between the call of the routine
C             with SCUN = 'S' and the corresponding one with SCUN = 'U'.
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
C             The elements of the array  NROWS  are not referenced if
C             NBL = 0.
C            
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading M by N part of this array must
C             contain the matrix to be scaled/unscaled. 
C             On exit, the leading M by N part of A will contain 
C             the modified matrix.
C             The storage mode of A is specified by TYPE.
C            
C     LDA     (input) INTEGER
C             The leading dimension of the array A.  LDA  >= max(1,M).
C            
C     Error Indicator
C
C     INFO    (output) INTEGER
C             = 0:  successful exit
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value.
C
C     METHOD
C
C     Denote by ANRM the norm of the matrix, and by SMLNUM and BIGNUM, 
C     two positive numbers near the smallest and largest safely
C     representable numbers, respectively.  The matrix is scaled, if
C     needed, such that the norm of the result is in the range 
C     [SMLNUM, BIGNUM].  The scaling factor is represented as a ratio
C     of two numbers, one of them being ANRM, and the other one either 
C     SMLNUM or BIGNUM, depending on ANRM being less than SMLNUM or 
C     larger than BIGNUM, respectively.  For undoing the scaling, the 
C     norm is again compared with SMLNUM or BIGNUM, and the reciprocal
C     of the previous scaling factor is used. 
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
C     .. Scalar Arguments ..
      CHARACTER*1        SCUN, TYPE
      INTEGER            INFO, KL, KU, LDA, M, MN, N, NBL
      DOUBLE PRECISION   ANRM
C     .. Array Arguments ..
      INTEGER            NROWS ( * )
      DOUBLE PRECISION   A( LDA, * )
C     .. Local Scalars ..
      LOGICAL            FIRST, LSCALE
      INTEGER            I, ISUM, ITYPE
      DOUBLE PRECISION   BIGNUM, SMLNUM
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLABAD, MB01QD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          MAX, MIN
C     .. Save statement ..
      SAVE               BIGNUM, FIRST, SMLNUM
C     .. Data statements ..
      DATA               FIRST/.TRUE./
C     ..
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      INFO = 0
      LSCALE = LSAME( SCUN, 'S' )
      IF( .NOT.LSCALE .AND. .NOT.LSAME( SCUN, 'U' ) ) THEN
         INFO = -1
      ELSE IF( LSAME( TYPE, 'G' ) ) THEN
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
      ELSE IF( LSAME( TYPE, 'Z' ) ) THEN
         ITYPE = 6
      ELSE
         ITYPE = -1
      END IF
C
      MN = MIN( M, N )
C
      IF( NBL.GT.0 ) THEN
         ISUM = 0
         DO 10 I = 1, NBL
            ISUM = ISUM + NROWS(I)
 10      CONTINUE       
      END IF
C
      IF( ITYPE.EQ.-1 ) THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( N.LT.0 .OR. 
     $         ( ( ITYPE.EQ.4 .OR. ITYPE.EQ.5 ) .AND. N.NE.M ) ) THEN
         INFO = -4
      ELSE IF( ANRM.LT.ZERO ) THEN
         INFO = -7
      ELSE IF( NBL.LT.0 ) THEN
         INFO = -8
      ELSE IF( NBL.GT.0 .AND. ISUM.NE.MN ) THEN
         INFO = -9
      ELSE IF( ITYPE.LE.3 .AND. LDA.LT.MAX( 1, M ) ) THEN
         INFO = -11
      ELSE IF( ITYPE.GE.4 ) THEN
         IF( KL.LT.0 .OR. KL.GT.MAX( M-1, 0 ) ) THEN
            INFO = -5
         ELSE IF( KU.LT.0 .OR. KU.GT.MAX( N-1, 0 ) .OR.
     $            ( ( ITYPE.EQ.4 .OR. ITYPE.EQ.5 ) .AND. KL.NE.KU ) )
     $             THEN
            INFO = -6
         ELSE IF( ( ITYPE.EQ.4 .AND. LDA.LT.KL+1 ) .OR.
     $            ( ITYPE.EQ.5 .AND. LDA.LT.KU+1 ) .OR.
     $            ( ITYPE.EQ.6 .AND. LDA.LT.2*KL+KU+1 ) ) THEN
            INFO = -11
         END IF
      END IF
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB01PD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( MN.EQ.0 .OR. ANRM.EQ.ZERO ) 
     $   RETURN
C
      IF ( FIRST ) THEN
C
C        Get machine parameters.
C
         SMLNUM = DLAMCH( 'S' ) / DLAMCH( 'P' )
         BIGNUM = ONE / SMLNUM
         CALL DLABAD( SMLNUM, BIGNUM )
         FIRST = .FALSE.
      END IF
C
      IF ( LSCALE ) THEN
C
C        Scale A, if its norm is outside range [SMLNUM,BIGNUM].
C
         IF( ANRM.LT.SMLNUM ) THEN
C
C           Scale matrix norm up to SMLNUM.
C
            CALL MB01QD( TYPE, M, N, KL, KU, ANRM, SMLNUM, NBL, NROWS,
     $                   A, LDA, INFO )
         ELSE IF( ANRM.GT.BIGNUM ) THEN
C
C           Scale matrix norm down to BIGNUM.
C
            CALL MB01QD( TYPE, M, N, KL, KU, ANRM, BIGNUM, NBL, NROWS,
     $                   A, LDA, INFO )
         END IF
C
      ELSE  
C
C        Undo scaling.
C
         IF( ANRM.LT.SMLNUM ) THEN
            CALL MB01QD( TYPE, M, N, KL, KU, SMLNUM, ANRM, NBL, NROWS,
     $                   A, LDA, INFO )
         ELSE IF( ANRM.GT.BIGNUM ) THEN
            CALL MB01QD( TYPE, M, N, KL, KU, BIGNUM, ANRM, NBL, NROWS,
     $                   A, LDA, INFO )
         END IF
      END IF
C
      RETURN
C *** Last line of MB01PD ***
      END
