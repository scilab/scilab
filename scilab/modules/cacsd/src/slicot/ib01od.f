      SUBROUTINE IB01OD( CTRL, NOBR, L, SV, N, TOL, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the system order, based on the singular values of the
C     relevant part of the triangular factor of the concatenated block
C     Hankel matrices.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     CTRL    CHARACTER*1
C             Specifies whether or not the user's confirmation of the
C             system order estimate is desired, as follows:
C             = 'C':  user's confirmation;
C             = 'N':  no confirmation.
C             If  CTRL = 'C',  a reverse communication routine,  IB01OY,
C             is called, and, after inspecting the singular values and
C             system order estimate,  n,  the user may accept  n  or set
C             a new value.
C             IB01OY  is not called by the routine if CTRL = 'N'.
C
C     Input/Output Parameters
C
C     NOBR    (input) INTEGER
C             The number of block rows,  s,  in the processed input and
C             output block Hankel matrices.  NOBR > 0.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     SV      (input) DOUBLE PRECISION array, dimension ( L*NOBR )
C             The singular values of the relevant part of the triangular
C             factor from the QR factorization of the concatenated block
C             Hankel matrices.
C
C     N       (output) INTEGER
C             The estimated order of the system.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             Absolute tolerance used for determining an estimate of
C             the system order. If  TOL >= 0,  the estimate is
C             indicated by the index of the last singular value greater
C             than or equal to  TOL.  (Singular values less than  TOL
C             are considered as zero.) When  TOL = 0,  an internally
C             computed default value,  TOL = NOBR*EPS*SV(1),  is used,
C             where  SV(1)  is the maximal singular value, and  EPS  is
C             the relative machine precision (see LAPACK Library routine
C             DLAMCH). When  TOL < 0,  the estimate is indicated by the
C             index of the singular value that has the largest
C             logarithmic gap to its successor.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 3:  all singular values were exactly zero, hence  N = 0.
C                   (Both input and output were identically zero.)
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
C     The singular values are compared to the given, or default TOL, and
C     the estimated order  n  is returned, possibly after user's
C     confirmation.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Aug. 1999.
C
C     REVISIONS
C
C     August 2000.
C
C     KEYWORDS
C
C     Identification methods, multivariable systems, singular value 
C     decomposition.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, N, NOBR
      CHARACTER          CTRL
C     .. Array Arguments ..
      DOUBLE PRECISION   SV(*)
C     .. Local Scalars ..
      DOUBLE PRECISION   GAP, RNRM, TOLL
      INTEGER            I, IERR, LNOBR
      LOGICAL            CONTRL
C     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, LSAME
C     .. External Subroutines ..
      EXTERNAL           IB01OY, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, LOG10
C     ..
C     .. Executable Statements ..
C
C     Check the scalar input parameters.
C
      CONTRL = LSAME( CTRL, 'C' )
      LNOBR  = L*NOBR
      IWARN  = 0
      INFO   = 0
      IF( .NOT.( CONTRL .OR. LSAME( CTRL, 'N' ) ) ) THEN
         INFO = -1
      ELSE IF( NOBR.LE.0 ) THEN
         INFO = -2
      ELSE IF( L.LE.0 ) THEN
         INFO = -3
      END IF       
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01OD', -INFO )
         RETURN
      END IF
C
C     Set  TOL  if necessay.
C
      TOLL = TOL
      IF ( TOLL.EQ.ZERO)
     $   TOLL = DLAMCH( 'Precision' )*SV(1)*DBLE( NOBR )
C
C     Obtain the system order.
C
      N = 0
      IF ( SV(1).NE.ZERO ) THEN
         N = NOBR
         IF ( TOLL.GE.ZERO) THEN
C
C           Estimate  n  based on the tolerance  TOLL.
C
            DO 10 I = 1, NOBR - 1
               IF ( SV(I+1).LT.TOLL ) THEN
                  N = I
                  GO TO 30
               END IF
   10       CONTINUE
         ELSE
C
C           Estimate  n  based on the largest logarithmic gap between
C           two consecutive singular values.
C
            GAP = ZERO
            DO 20 I = 1, NOBR - 1
               RNRM = SV(I+1)
               IF ( RNRM.NE.ZERO ) THEN
                  RNRM = LOG10( SV(I) ) - LOG10( RNRM )
                  IF ( RNRM.GT.GAP ) THEN
                     GAP = RNRM
                     N = I
                  END IF
               ELSE
                  IF ( GAP.EQ.ZERO )
     $               N = I
                  GO TO 30
               END IF
   20       CONTINUE
         END IF
      END IF
C
   30 CONTINUE
      IF ( N.EQ.0 ) THEN
C
C        Return with  N = 0  if all singular values are zero.
C
         IWARN = 3
         RETURN
      END IF
C
      IF ( CONTRL ) THEN
C
C        Ask confirmation of the system order.
C
         CALL IB01OY( LNOBR, NOBR-1, N, SV, IERR )
      END IF
      RETURN
C
C *** Last line of IB01OD ***
      END
