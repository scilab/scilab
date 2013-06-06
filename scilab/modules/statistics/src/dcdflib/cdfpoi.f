      SUBROUTINE cdfpoi(which,p,q,s,xlam,status,bound)
C**********************************************************************
C
C      SUBROUTINE CDFPOI( WHICH, P, Q, S, XLAM, STATUS, BOUND )
C               Cumulative Distribution Function
C               POIsson distribution
C
C
C                              Function
C
C
C     Calculates any one parameter of the Poisson
C     distribution given values for the others.
C
C
C                              Arguments
C
C
C     WHICH --> Integer indicating which  argument
C               value is to be calculated from the others.
C               Legal range: 1..3
C               iwhich = 1 : Calculate P and Q from S and XLAM
C               iwhich = 2 : Calculate A from P,Q and XLAM
C               iwhich = 3 : Calculate XLAM from P,Q and S
C                    INTEGER WHICH
C
C        P <--> The cumulation from 0 to S of the poisson density.
C               Input range: [0,1].
C                    DOUBLE PRECISION P
C
C        Q <--> 1-P.
C               Input range: (0, 1].
C               P + Q = 1.0.
C                    DOUBLE PRECISION Q
C
C        S <--> Upper limit of cumulation of the Poisson.
C               Input range: [0, +infinity).
C               Search range: [0,1E300]
C                    DOUBLE PRECISION S
C
C     XLAM <--> Mean of the Poisson distribution.
C               Input range: [0, +infinity).
C               Search range: [0,1E300]
C                    DOUBLE PRECISION XLAM
C
C     STATUS <-- 0 if calculation completed correctly
C               -I if input parameter number I is out of range
C                1 if answer appears to be lower than lowest
C                  search bound
C                2 if answer appears to be higher than greatest
C                  search bound
C                3 if P + Q .ne. 1
C                    INTEGER STATUS
C
C     BOUND <-- Undefined if STATUS is 0
C
C               Bound exceeded by parameter number I if STATUS
C               is negative.
C
C               Lower search bound if STATUS is 1.
C
C               Upper search bound if STATUS is 2.
C
C
C                              Method
C
C
C     Formula   26.4.21  of   Abramowitz  and   Stegun,   Handbook  of
C     Mathematical Functions (1966) is used  to reduce the computation
C     of  the cumulative distribution function to that  of computing a
C     chi-square, hence an incomplete gamma function.
C
C     Cumulative  distribution function  (P) is  calculated  directly.
C     Computation of other parameters involve a search for a value that
C     produces  the desired value of  P.   The  search relies  on  the
C     monotonicity of P with the other parameter.
C
C
C**********************************************************************
C     .. Parameters ..
      DOUBLE PRECISION tol
      PARAMETER (tol=1.0D-13)
      DOUBLE PRECISION atol
      PARAMETER (atol=1.0D-50)
      DOUBLE PRECISION inf
      PARAMETER (inf=1.0D300)
C     ..
C     .. Scalar Arguments ..
      DOUBLE PRECISION bound,p,q,s,xlam
      INTEGER status,which
C     ..
C     .. Local Scalars ..
      DOUBLE PRECISION fx,cum,ccum,pq
      LOGICAL qhi,qleft,qporq
C     ..
C     .. External Functions ..
      INTEGER vfinite
      DOUBLE PRECISION spmpar
      EXTERNAL spmpar
C     ..
C     .. External Subroutines ..
      EXTERNAL dinvr,dstinv,cumpoi
C     ..
C     .. Executable Statements ..
C
C     Check arguments
C
      IF (.NOT. ((which.LT.1).OR. (which.GT.3))) GO TO 30
      IF (.NOT. (which.LT.1)) GO TO 10
      bound = 1.0D0
      GO TO 20

   10 bound = 3.0D0
   20 status = -1
      RETURN

   30 IF (which.EQ.1) GO TO 70
C
C     P
C
      IF (ISANAN(p).EQ.1) THEN
         CALL RETURNANANFORTRAN(s)
         CALL RETURNANANFORTRAN(xlam)
         RETURN
      ENDIF
      IF (.NOT. ((p.LT.0.0D0).OR. (p.GT.1.0D0))) GO TO 60
      IF (.NOT. (p.LT.0.0D0)) GO TO 40
      bound = 0.0D0
      GO TO 50

   40 bound = 1.0D0
   50 status = -2
      RETURN

   60 CONTINUE
   70 IF (which.EQ.1) GO TO 110
C
C     Q
C
      IF (ISANAN(q).EQ.1) THEN
         CALL RETURNANANFORTRAN(s)
         CALL RETURNANANFORTRAN(xlam)
         RETURN
      ENDIF
      IF (.NOT. ((q.LE.0.0D0).OR. (q.GT.1.0D0))) GO TO 100
      IF (.NOT. (q.LE.0.0D0)) GO TO 80
      bound = 0.0D0
      GO TO 90

   80 bound = 1.0D0
   90 status = -3
      RETURN

  100 CONTINUE
  110 IF (which.EQ.2) GO TO 130
C
C     S
C
      IF (ISANAN(s).EQ.1) THEN
         CALL RETURNANANFORTRAN(p)
         CALL RETURNANANFORTRAN(q)
         CALL RETURNANANFORTRAN(xlam)
         RETURN
      ENDIF
      IF (vfinite(1,s).EQ.0) then
         IF (which.EQ.1) then
            IF (s.GT.0) then
               p = 1
               q = 0
               RETURN
            ENDIF
         ELSE
            s = SIGN(1D300,s)
         ENDIF
      ENDIF
      IF (.NOT. (s.LT.0.0D0)) GO TO 120
      bound = 0.0D0
      status = -4
      RETURN

  120 CONTINUE
  130 IF (which.EQ.3) GO TO 150
C
C     XLAM
C
      IF (ISANAN(xlam).EQ.1) THEN
         CALL RETURNANANFORTRAN(p)
         CALL RETURNANANFORTRAN(q)
         CALL RETURNANANFORTRAN(s)
         RETURN
      ENDIF
      IF (vfinite(1,xlam).EQ.0) xlam = SIGN(inf,xlam)
      IF (.NOT. (xlam.LT.0.0D0)) GO TO 140
      bound = 0.0D0
      status = -5
      RETURN

  140 CONTINUE
  150 IF (which.EQ.1) GO TO 190
C
C     P + Q
C
      pq = p + q
      IF (.NOT. (abs(((pq)-0.5D0)-0.5D0).GT.
     +    (3.0D0*spmpar(1)))) GO TO 180
      IF (.NOT. (pq.LT.0.0D0)) GO TO 160
      bound = 0.0D0
      GO TO 170

  160 bound = 1.0D0
  170 status = 3
      RETURN

  180 CONTINUE
  190 IF (.NOT. (which.EQ.1)) qporq = p .LE. q
C
C     Select the minimum of P or Q
C
C
C     Calculate ANSWERS
C
      IF ((1).EQ. (which)) THEN
C
C     Calculating P
C
          CALL cumpoi(s,xlam,p,q)
          status = 0

      ELSE IF ((2).EQ. (which)) THEN
C
C     Calculating S
C
          s = 5.0D0
          CALL dstinv(0.0D0,inf,0.5D0,0.5D0,5.0D0,atol,tol)
          status = 0
          CALL dinvr(status,s,fx,qleft,qhi)
  200     IF (.NOT. (status.EQ.1)) GO TO 230
          CALL cumpoi(s,xlam,cum,ccum)
          IF (.NOT. (qporq)) GO TO 210
          fx = cum - p
          GO TO 220

  210     fx = ccum - q
  220     CALL dinvr(status,s,fx,qleft,qhi)
          GO TO 200

  230     IF (.NOT. (status.EQ.-1)) GO TO 260
          IF (.NOT. (qleft)) GO TO 240
          status = 1
          bound = 0.0D0
          GO TO 250

  240     status = 2
          bound = inf
  250     CONTINUE
  260     CONTINUE

      ELSE IF ((3).EQ. (which)) THEN
C
C     Calculating XLAM
C
          xlam = 5.0D0
          CALL dstinv(0.0D0,inf,0.5D0,0.5D0,5.0D0,atol,tol)
          status = 0
          CALL dinvr(status,xlam,fx,qleft,qhi)
  270     IF (.NOT. (status.EQ.1)) GO TO 300
          CALL cumpoi(s,xlam,cum,ccum)
          IF (.NOT. (qporq)) GO TO 280
          fx = cum - p
          GO TO 290

  280     fx = ccum - q
  290     CALL dinvr(status,xlam,fx,qleft,qhi)
          GO TO 270

  300     IF (.NOT. (status.EQ.-1)) GO TO 330
          IF (.NOT. (qleft)) GO TO 310
          status = 1
          bound = 0.0D0
          GO TO 320

  310     status = 2
          bound = inf
  320     CONTINUE
  330 END IF

      RETURN

      END
