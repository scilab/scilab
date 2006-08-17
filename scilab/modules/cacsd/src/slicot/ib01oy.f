      SUBROUTINE IB01OY( NS, NMAX, N, SV, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To ask for user's confirmation of the system order found by 
C     SLICOT Library routine IB01OD. This routine may be modified,
C     but its interface must be preserved.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     NS      (input) INTEGER
C             The number of singular values.  NS > 0.
C
C     NMAX    (input) INTEGER
C             The maximum value of the system order.  0 <= NMAX <= NS.
C
C     N       (input/output) INTEGER
C             On entry, the estimate of the system order computed by
C             IB01OD routine.  0 <= N <= NS.
C             On exit, the user's estimate of the system order, which
C             could be identical with the input value of  N.
C             Note that the output value of  N  should be less than
C             or equal to  NMAX.
C
C     SV      (input) DOUBLE PRECISION array, dimension ( NS )
C             The singular values, in descending order, used for
C             determining the system order.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value.
C
C     CONTRIBUTORS
C
C     V. Sima, Research Institute for Informatics, Bucharest, Aug. 1999.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Identification, parameter estimation, singular values, structure
C     identification.
C 
C  *********************************************************************
C
C     .. Parameters ..
      INTEGER            INTRMN, OUTRMN
      PARAMETER          ( INTRMN = 5, OUTRMN = 6 )
C        INTRMN is the unit number for the (terminal) input device.
C        OUTRMN is the unit number for the (terminal) output device.
C     ..
C     .. Scalar Arguments ..
      INTEGER            INFO, N, NMAX, NS
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION   SV( * )
C     ..
C     .. Local Scalars ..
      LOGICAL            YES
      INTEGER            I
      CHARACTER          ANS
C     ..
C     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
C     ..
C     .. External Subroutines ..
      EXTERNAL           XERBLA
C
C     .. Executable Statements ..
C
C     Check the scalar input parameters.
C
      INFO = 0
      IF( NS.LE.0 ) THEN
         INFO = -1
      ELSE IF( NMAX.LT.0 .OR. NMAX.GT.NS ) THEN
         INFO = -2
      ELSE IF( N.LT.0 .OR. N.GT.NS ) THEN
         INFO = -3
      END IF       
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01OY', -INFO )
         RETURN
      END IF
C
      WRITE( OUTRMN, '(/'' Singular values (in descending order) used'',
     $                  '' to estimate the system order:'', //
     $                     (5D15.8) )' ) ( SV(I), I = 1, NS )
      WRITE( OUTRMN, '(/'' Estimated order of the system,  n = '', I5 )'
     $     )               N
      WRITE( OUTRMN, '(/'' Do you want this value of  n  to be used'',
     $                  '' to determine the system matrices?'' )' )
C
   10 CONTINUE
         WRITE( OUTRMN, '(/''  Type "yes" or "no":  '' )' )
         READ ( INTRMN,  '( A )' ) ANS
         YES = LSAME( ANS, 'Y' )
         IF( YES ) THEN
            IF( N.LE.NMAX ) THEN
C
C              The value of n is adequate and has been confirmed.
C
               RETURN
            ELSE
C
C              The estimated value of n is not acceptable.
C
               WRITE( OUTRMN, '(/'' n  should be less than or equal'',
     $                           '' to '', I5 )' ) NMAX
               WRITE( OUTRMN, '( '' (It may be useful to restart'',
     $                           '' with a larger tolerance.)'' )' )
               GO TO 20
            END IF
C
         ELSE IF( LSAME( ANS, 'N' ) ) THEN
            GO TO 20
         ELSE
C
C           Wrong answer should be re-entered.
C
            GO TO 10
         END IF
C
C     Enter the desired value of n.
C     
   20 CONTINUE
         WRITE( OUTRMN,'(/'' Enter the desired value of n (n <= '', I5,
     $                    '');  n = '' )' ) NMAX
         READ ( INTRMN, * ) N
         IF ( N.LT.0 ) THEN
C
C           The specified value of n is not acceptable.
C
            WRITE( OUTRMN, '(/'' n  should be larger than zero.'' )' )
            GO TO 20
         ELSE IF ( N.GT.NMAX ) THEN
C
C           The specified value of n is not acceptable.
C
            WRITE( OUTRMN, '(/'' n  should be less than or equal to '',
     $                   I5 )' ) NMAX
            GO TO 20
         END IF
C
      RETURN
C
C *** Last line of IB01OY ***
      END
