*
*  original code from the Slatec library
*
*  slight modifications by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> :
* 
*     1/ some (minor modifications) so that the "enter" is
*        now X and not THETA (X=cos(THETA)). This leads to
*        better accuracy for x near 0 and seems more
*        natural but may be there is some drawback ? 
*     2/ remove parts which send warning messages to the 
*        Slatec XERMSG routine (nevertheless all the errors
*        flags are communicated throw IERROR).
*        Normaly the scilab interface verify the validity of the
*        input arguments but the verifications in this code are
*        still here.
*     3/ substitute calls to I1MACH by calls to dlamch
*        (Scilab uses dlamch and not I1MACH to get machine
*        parameter so it seems more logical).
*
*    IERROR values :
*          210 :  DNU1, NUDIFF, MU1, MU2, or ID not valid
*          211 :  X out of range (must be in [0,1)
*          201, 202, 203, 204 : invalid input was provided to DXSET
*                       (should not occured in IEEE floating point)
*          205, 206 : internal consistency error occurred in DXSET 
*                     (probably due to a software malfunction in the 
*                      library routine I1MACH) Should not occured  
*                      in IEEE floating point, if dlamch works well.
*          207 : an overflow or underflow of an extended-range number
*                was detected in DXADJ.
*          208 : an error which may occur in DXC210 but this one is not
*                call from DXLEGF (don't know why it is given below).
*
*     Normally on the return to scilab, only 207 may be present.


*DECK DXLEGF
      SUBROUTINE DXLEGF(DNU1, NUDIFF, MU1, MU2, X, ID, PQA, IPQA,
     1   IERROR)
C***BEGIN PROLOGUE  DXLEGF
C***PURPOSE  Compute normalized Legendre polynomials and associated
C            Legendre functions.
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XLEGF-S, DXLEGF-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***DESCRIPTION
C
C   DXLEGF: Extended-range Double-precision Legendre Functions
C
C   A feature of the DXLEGF subroutine for Legendre functions is
C the use of extended-range arithmetic, a software extension of
C ordinary floating-point arithmetic that greatly increases the
C exponent range of the representable numbers. This avoids the
C need for scaling the solutions to lie within the exponent range
C of the most restrictive manufacturer's hardware. The increased
C exponent range is achieved by allocating an integer storage
C location together with each floating-point storage location.
C
C   The interpretation of the pair (X,I) where X is floating-point
C and I is integer is X*(IR**I) where IR is the internal radix of
C the computer arithmetic.
C
C   This subroutine computes one of the following vectors:
C
C 1. Legendre function of the first kind of negative order, either
C    a. P(-MU1,NU,X), P(-MU1-1,NU,X), ..., P(-MU2,NU,X) or
C    b. P(-MU,NU1,X), P(-MU,NU1+1,X), ..., P(-MU,NU2,X)
C 2. Legendre function of the second kind, either
C    a. Q(MU1,NU,X), Q(MU1+1,NU,X), ..., Q(MU2,NU,X) or
C    b. Q(MU,NU1,X), Q(MU,NU1+1,X), ..., Q(MU,NU2,X)
C 3. Legendre function of the first kind of positive order, either
C    a. P(MU1,NU,X), P(MU1+1,NU,X), ..., P(MU2,NU,X) or
C    b. P(MU,NU1,X), P(MU,NU1+1,X), ..., P(MU,NU2,X)
C 4. Normalized Legendre polynomials, either
C    a. PN(MU1,NU,X), PN(MU1+1,NU,X), ..., PN(MU2,NU,X) or
C    b. PN(MU,NU1,X), PN(MU,NU1+1,X), ..., PN(MU,NU2,X)
C
C where X = COS(THETA).
C
C   The input values to DXLEGF are DNU1, NUDIFF, MU1, MU2, THETA (now X),
C and ID. These must satisfy
C
C    DNU1 is DOUBLE PRECISION and greater than or equal to -0.5;
C    NUDIFF is INTEGER and non-negative;
C    MU1 is INTEGER and non-negative;
C    MU2 is INTEGER and greater than or equal to MU1;

C    THETA is DOUBLE PRECISION and in the half-open interval (0,PI/2];
*    modification :  X is given (and not THETA) X must be in [0,1)

C    ID is INTEGER and equal to 1, 2, 3 or 4;
C
C and  additionally either NUDIFF = 0 or MU2 = MU1.
C
C   If ID=1 and NUDIFF=0, a vector of type 1a above is computed
C with NU=DNU1.
C
C   If ID=1 and MU1=MU2, a vector of type 1b above is computed
C with NU1=DNU1, NU2=DNU1+NUDIFF and MU=MU1.
C
C   If ID=2 and NUDIFF=0, a vector of type 2a above is computed
C with NU=DNU1.
C
C   If ID=2 and MU1=MU2, a vector of type 2b above is computed
C with NU1=DNU1, NU2=DNU1+NUDIFF and MU=MU1.
C
C   If ID=3 and NUDIFF=0, a vector of type 3a above is computed
C with NU=DNU1.
C
C   If ID=3 and MU1=MU2, a vector of type 3b above is computed
C with NU1=DNU1, NU2=DNU1+NUDIFF and MU=MU1.
C
C   If ID=4 and NUDIFF=0, a vector of type 4a above is computed
C with NU=DNU1.
C
C   If ID=4 and MU1=MU2, a vector of type 4b above is computed
C with NU1=DNU1, NU2=DNU1+NUDIFF and MU=MU1.
C
C   In each case the vector of computed Legendre function values
C is returned in the extended-range vector (PQA(I),IPQA(I)). The
C length of this vector is either MU2-MU1+1 or NUDIFF+1.
C
C   Where possible, DXLEGF returns IPQA(I) as zero. In this case the
C value of the Legendre function is contained entirely in PQA(I),
C so it can be used in subsequent computations without further
C consideration of extended-range arithmetic. If IPQA(I) is nonzero,
C then the value of the Legendre function is not representable in
C floating-point because of underflow or overflow. The program that
C calls DXLEGF must test IPQA(I) to ensure correct usage.
C
C   IERROR is an error indicator. If no errors are detected, IERROR=0
C when control returns to the calling routine. If an error is detected,
C IERROR is returned as nonzero. The calling routine must check the
C value of IERROR.
C
C   If IERROR=210 or 211, invalid input was provided to DXLEGF.
C   If IERROR=201,202,203, or 204, invalid input was provided to DXSET.
C   If IERROR=205 or 206, an internal consistency error occurred in
C DXSET (probably due to a software malfunction in the library routine
C I1MACH).
C   If IERROR=207, an overflow or underflow of an extended-range number
C was detected in DXADJ.
C   If IERROR=208, an overflow or underflow of an extended-range number
C was detected in DXC210.
C
C***SEE ALSO  DXSET
C***REFERENCES  Olver and Smith, Associated Legendre Functions on the
C                 Cut, J Comp Phys, v 51, n 3, Sept 1983, pp 502--518.
C               Smith, Olver and Lozier, Extended-Range Arithmetic and
C                 Normalized Legendre Polynomials, ACM Trans on Math
C                 Softw, v 7, n 1, March 1981, pp 93--105.
C***ROUTINES CALLED  DXPMU, DXPMUP, DXPNRM, DXPQNU, DXQMU, DXQNU, DXRED,
C                    DXSET, XERMSG
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C           CALLs to XERROR changed to CALLs to XERMSG.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXLEGF
      DOUBLE PRECISION PQA,DNU1,DNU2,SX,X,PI2
      DIMENSION PQA(*),IPQA(*)
C
C***FIRST EXECUTABLE STATEMENT  DXLEGF
      IERROR=0
      CALL DXSET (0, 0, 0.0D0, 0,IERROR)
      IF (IERROR.NE.0) RETURN
      PI2=2.D0*ATAN(1.D0)
C
C        ZERO OUTPUT ARRAYS
C
      L=(MU2-MU1)+NUDIFF+1
      DO 290 I=1,L
      PQA(I)=0.D0
  290 IPQA(I)=0
C
C        CHECK FOR VALID INPUT VALUES
C
*** normally all these are verified by the scilab interface
      IF(NUDIFF.LT.0) GO TO 400
      IF(DNU1.LT.-.5D0) GO TO 400
      IF(MU2.LT.MU1) GO TO 400
      IF(MU1.LT.0) GO TO 400
*      IF(THETA.LE.0.D0.OR.THETA.GT.PI2) GO TO 420
      IF(X.LT.0.D0.OR.X.GE.1.d0) GO TO 420
      IF(ID.LT.1.OR.ID.GT.4) GO TO 400
      IF((MU1.NE.MU2).AND.(NUDIFF.GT.0)) GO TO 400
C
C        IF DNU1 IS NOT AN INTEGER, NORMALIZED P(MU,DNU,X)
C        CANNOT BE CALCULATED.  IF DNU1 IS AN INTEGER AND
C        MU1.GT.DNU2 THEN ALL VALUES OF P(+MU,DNU,X) AND
C        NORMALIZED P(MU,NU,X) WILL BE ZERO.
C
      DNU2=DNU1+NUDIFF
      IF((ID.EQ.3).AND.(MOD(DNU1,1.D0).NE.0.D0)) GO TO 295
      IF((ID.EQ.4).AND.(MOD(DNU1,1.D0).NE.0.D0)) GO TO 400
      IF((ID.EQ.3.OR.ID.EQ.4).AND.MU1.GT.DNU2) RETURN
  295 CONTINUE
C
*      X=COS(THETA)
*      SX=1.D0/SIN(THETA)
      SX=1.D0/SQRT((1.d0-X)*(1.d0+X))
      IF(ID.EQ.2) GO TO 300
      IF(MU2-MU1.LE.0) GO TO 360
C
C        FIXED NU, VARIABLE MU
C        CALL DXPMU TO CALCULATE P(-MU1,NU,X),....,P(-MU2,NU,X)
C
      CALL DXPMU(DNU1,DNU2,MU1,MU2,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      GO TO 380
C
  300 IF(MU2.EQ.MU1) GO TO 320
C
C        FIXED NU, VARIABLE MU
C        CALL DXQMU TO CALCULATE Q(MU1,NU,X),....,Q(MU2,NU,X)
C
      CALL DXQMU(DNU1,DNU2,MU1,MU2,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      GO TO 390
C
C        FIXED MU, VARIABLE NU
C        CALL DXQNU TO CALCULATE Q(MU,DNU1,X),....,Q(MU,DNU2,X)
C
  320 CALL DXQNU(DNU1,DNU2,MU1,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      GO TO 390
C
C        FIXED MU, VARIABLE NU
C        CALL DXPQNU TO CALCULATE P(-MU,DNU1,X),....,P(-MU,DNU2,X)
C
  360 CALL DXPQNU(DNU1,DNU2,MU1,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
C
C        IF ID = 3, TRANSFORM P(-MU,NU,X) VECTOR INTO
C        P(MU,NU,X) VECTOR.
C
  380 IF(ID.EQ.3) CALL DXPMUP(DNU1,DNU2,MU1,MU2,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
C
C        IF ID = 4, TRANSFORM P(-MU,NU,X) VECTOR INTO
C        NORMALIZED P(MU,NU,X) VECTOR.
C
      IF(ID.EQ.4) CALL DXPNRM(DNU1,DNU2,MU1,MU2,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
C
C        PLACE RESULTS IN REDUCED FORM IF POSSIBLE
C        AND RETURN TO MAIN PROGRAM.
C
  390 DO 395 I=1,L
      CALL DXRED(PQA(I),IPQA(I),IERROR)
      IF (IERROR.NE.0) RETURN
  395 CONTINUE
      RETURN
C
C        *****     ERROR TERMINATION     *****
C
*  400 CALL XERMSG ('SLATEC', 'DXLEGF',
*     +             'DNU1, NUDIFF, MU1, MU2, or ID not valid', 210, 1)
 400  continue
      IERROR=210
      RETURN
*  420 CALL XERMSG ('SLATEC', 'DXLEGF', 'THETA out of range', 211, 1)
 420  continue
      IERROR=211
      RETURN
      END
*DECK DXPMU
      SUBROUTINE DXPMU (NU1, NU2, MU1, MU2, X, SX, ID, PQA, IPQA,
     1   IERROR)
C***BEGIN PROLOGUE  DXPMU
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            Method: backward mu-wise recurrence for P(-MU,NU,X) for
C            fixed nu to obtain P(-MU2,NU1,X), P(-(MU2-1),NU1,X), ...,
C            P(-MU1,NU1,X) and store in ascending mu order.
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XPMU-S, DXPMU-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADD, DXADJ, DXPQNU
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXPMU
      DOUBLE PRECISION PQA,NU1,NU2,P0,X,SX,X1,X2
      DIMENSION PQA(*),IPQA(*)
C
C        CALL DXPQNU TO OBTAIN P(-MU2,NU,X)
C
C***FIRST EXECUTABLE STATEMENT  DXPMU
      IERROR=0
      CALL DXPQNU(NU1,NU2,MU2,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      P0=PQA(1)
      IP0=IPQA(1)
      MU=MU2-1
C
C        CALL DXPQNU TO OBTAIN P(-MU2-1,NU,X)
C
      CALL DXPQNU(NU1,NU2,MU,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      N=MU2-MU1+1
      PQA(N)=P0
      IPQA(N)=IP0
      IF(N.EQ.1) GO TO 300
      PQA(N-1)=PQA(1)
      IPQA(N-1)=IPQA(1)
      IF(N.EQ.2) GO TO 300
      J=N-2
  290 CONTINUE
C
C        BACKWARD RECURRENCE IN MU TO OBTAIN
C              P(-MU2,NU1,X),P(-(MU2-1),NU1,X),....P(-MU1,NU1,X)
C              USING
C              (NU-MU)*(NU+MU+1.)*P(-(MU+1),NU,X)=
C                2.*MU*X*SQRT((1./(1.-X**2))*P(-MU,NU,X)-P(-(MU-1),NU,X)
C
      X1=2.D0*MU*X*SX*PQA(J+1)
      X2=-(NU1-MU)*(NU1+MU+1.D0)*PQA(J+2)
      CALL DXADD(X1,IPQA(J+1),X2,IPQA(J+2),PQA(J),IPQA(J),IERROR)
      IF (IERROR.NE.0) RETURN
      CALL DXADJ(PQA(J),IPQA(J),IERROR)
      IF (IERROR.NE.0) RETURN
      IF(J.EQ.1) GO TO 300
      J=J-1
      MU=MU-1
      GO TO 290
  300 RETURN
      END
*DECK DXPMUP
      SUBROUTINE DXPMUP (NU1, NU2, MU1, MU2, PQA, IPQA, IERROR)
C***BEGIN PROLOGUE  DXPMUP
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            This subroutine transforms an array of Legendre functions
C            of the first kind of negative order stored in array PQA
C            into Legendre functions of the first kind of positive
C            order stored in array PQA. The original array is destroyed.
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XPMUP-S, DXPMUP-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADJ
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXPMUP
      DOUBLE PRECISION DMU,NU,NU1,NU2,PQA,PROD
      DIMENSION PQA(*),IPQA(*)
C***FIRST EXECUTABLE STATEMENT  DXPMUP
      IERROR=0
      NU=NU1
      MU=MU1
      DMU=MU
      N=INT(NU2-NU1+.1D0)+(MU2-MU1)+1
      J=1
*      IF(MOD(REAL(NU),1.).NE.0.) GO TO 210
      IF(MOD(NU,1.d0).NE.0.d0) GO TO 210
  200 IF(DMU.LT.NU+1.D0) GO TO 210
      PQA(J)=0.D0
      IPQA(J)=0
      J=J+1
      IF(J.GT.N) RETURN
C        INCREMENT EITHER MU OR NU AS APPROPRIATE.
      IF(NU2-NU1.GT..5D0) NU=NU+1.D0
      IF(MU2.GT.MU1) MU=MU+1
      GO TO 200
C
C        TRANSFORM P(-MU,NU,X) TO P(MU,NU,X) USING
C        P(MU,NU,X)=(NU-MU+1)*(NU-MU+2)*...*(NU+MU)*P(-MU,NU,X)*(-1)**MU
C
  210 PROD=1.D0
      IPROD=0
      K=2*MU
      IF(K.EQ.0) GO TO 222
      DO 220 L=1,K
      PROD=PROD*(DMU-NU-L)
  220 CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
  222 CONTINUE
      DO 240 I=J,N
      IF(MU.EQ.0) GO TO 225
      PQA(I)=PQA(I)*PROD*(-1)**MU
      IPQA(I)=IPQA(I)+IPROD
      CALL DXADJ(PQA(I),IPQA(I),IERROR)
      IF (IERROR.NE.0) RETURN
  225 IF(NU2-NU1.GT..5D0) GO TO 230
      PROD=(DMU-NU)*PROD*(-DMU-NU-1.D0)
      CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
      MU=MU+1
      DMU=DMU+1.D0
      GO TO 240
  230 PROD=PROD*(-DMU-NU-1.D0)/(DMU-NU-1.D0)
      CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
      NU=NU+1.D0
  240 CONTINUE
      RETURN
      END
*DECK DXPNRM
      SUBROUTINE DXPNRM (NU1, NU2, MU1, MU2, PQA, IPQA, IERROR)
C***BEGIN PROLOGUE  DXPNRM
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            This subroutine transforms an array of Legendre functions
C            of the first kind of negative order stored in array PQA
C            into normalized Legendre polynomials stored in array PQA.
C            The original array is destroyed.
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XPNRM-S, DXPNRM-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADJ
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXPNRM
      DOUBLE PRECISION C1,DMU,NU,NU1,NU2,PQA,PROD
      DIMENSION PQA(*),IPQA(*)
C***FIRST EXECUTABLE STATEMENT  DXPNRM
      IERROR=0
      L=(MU2-MU1)+(NU2-NU1+1.5D0)
      MU=MU1
      DMU=MU1
      NU=NU1
C
C         IF MU .GT.NU, NORM P =0.
C
      J=1
  500 IF(DMU.LE.NU) GO TO 505
      PQA(J)=0.D0
      IPQA(J)=0
      J=J+1
      IF(J.GT.L) RETURN
C
C        INCREMENT EITHER MU OR NU AS APPROPRIATE.
C
      IF(MU2.GT.MU1) DMU=DMU+1.D0
      IF(NU2-NU1.GT..5D0) NU=NU+1.D0
      GO TO 500
C
C         TRANSFORM P(-MU,NU,X) INTO NORMALIZED P(MU,NU,X) USING
C              NORM P(MU,NU,X)=
C                 SQRT((NU+.5)*FACTORIAL(NU+MU)/FACTORIAL(NU-MU))
C                              *P(-MU,NU,X)
C
  505 PROD=1.D0
      IPROD=0
      K=2*MU
      IF(K.LE.0) GO TO 520
      DO 510 I=1,K
      PROD=PROD*SQRT(NU+DMU+1.D0-I)
  510 CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
  520 DO 540 I=J,L
      C1=PROD*SQRT(NU+.5D0)
      PQA(I)=PQA(I)*C1
      IPQA(I)=IPQA(I)+IPROD
      CALL DXADJ(PQA(I),IPQA(I),IERROR)
      IF (IERROR.NE.0) RETURN
      IF(NU2-NU1.GT..5D0) GO TO 530
      IF(DMU.GE.NU) GO TO 525
      PROD=SQRT(NU+DMU+1.D0)*PROD
      IF(NU.GT.DMU) PROD=PROD*SQRT(NU-DMU)
      CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
      MU=MU+1
      DMU=DMU+1.D0
      GO TO 540
  525 PROD=0.D0
      IPROD=0
      MU=MU+1
      DMU=DMU+1.D0
      GO TO 540
  530 PROD=SQRT(NU+DMU+1.D0)*PROD
      IF(NU.NE.DMU-1.D0) PROD=PROD/SQRT(NU-DMU+1.D0)
      CALL DXADJ(PROD,IPROD,IERROR)
      IF (IERROR.NE.0) RETURN
      NU=NU+1.D0
  540 CONTINUE
      RETURN
      END
*DECK DXPQNU
      SUBROUTINE DXPQNU (NU1, NU2, MU, X, SX, ID, PQA, IPQA, IERROR)
C***BEGIN PROLOGUE  DXPQNU
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            This subroutine calculates initial values of P or Q using
C            power series, then performs forward nu-wise recurrence to
C            obtain P(-MU,NU,X), Q(0,NU,X), or Q(1,NU,X). The nu-wise
C            recurrence is stable for P for all mu and for Q for mu=0,1.
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XPQNU-S, DXPQNU-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADD, DXADJ, DXPSI
C***COMMON BLOCKS    DXBLK1
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXPQNU
      DOUBLE PRECISION A,NU,NU1,NU2,PQ,PQA,DXPSI,R,W,X,X1,X2,SX,XS,
     1 Y,Z
      DOUBLE PRECISION DI,DMU,PQ1,PQ2,FACTMU,FLOK
      DIMENSION PQA(*),IPQA(*)
      COMMON /DXBLK1/ NBITSF
      SAVE /DXBLK1/
C
C        J0, IPSIK, AND IPSIX ARE INITIALIZED IN THIS SUBROUTINE.
C        J0 IS THE NUMBER OF TERMS USED IN SERIES EXPANSION
C        IN SUBROUTINE DXPQNU.
C        IPSIK, IPSIX ARE VALUES OF K AND X RESPECTIVELY
C        USED IN THE CALCULATION OF THE DXPSI FUNCTION.
C
C***FIRST EXECUTABLE STATEMENT  DXPQNU
      IERROR=0
      J0=NBITSF
      IPSIK=1+(NBITSF/10)
      IPSIX=5*IPSIK
      IPQ=0
C        FIND NU IN INTERVAL [-.5,.5) IF ID=2  ( CALCULATION OF Q )
      NU=MOD(NU1,1.D0)
      IF(NU.GE..5D0) NU=NU-1.D0
C        FIND NU IN INTERVAL (-1.5,-.5] IF ID=1,3, OR 4  ( CALC. OF P )
      IF(ID.NE.2.AND.NU.GT.-.5D0) NU=NU-1.D0
C        CALCULATE MU FACTORIAL
      K=MU
      DMU=MU
      IF(MU.LE.0) GO TO 60
      FACTMU=1.D0
      IF=0
      DO 50 I=1,K
      FACTMU=FACTMU*I
   50 CALL DXADJ(FACTMU,IF,IERROR)
      IF (IERROR.NE.0) RETURN
   60 IF(K.EQ.0) FACTMU=1.D0
      IF(K.EQ.0) IF=0
C
C        X=COS(THETA)
C        Y=SIN(THETA/2)**2=(1-X)/2=.5-.5*X
C        R=TAN(THETA/2)=SQRT((1-X)/(1+X)
C
      Y=0.5d0*(1.d0-X)
      R=sqrt((1.d0-X)/(1.d0+X))
C
C        USE ASCENDING SERIES TO CALCULATE TWO VALUES OF P OR Q
C        FOR USE AS STARTING VALUES IN RECURRENCE RELATION.
C
      PQ2=0.0D0
      DO 100 J=1,2
      IPQ1=0
      IF(ID.EQ.2) GO TO 80
C
C        SERIES FOR P ( ID = 1, 3, OR 4 )
C        P(-MU,NU,X)=1./FACTORIAL(MU)*SQRT(((1.-X)/(1.+X))**MU)
C                *SUM(FROM 0 TO J0-1)A(J)*(.5-.5*X)**J
C
      IPQ=0
      PQ=1.D0
      A=1.D0
      IA=0
      DO 65 I=2,J0
      DI=I
      A=A*Y*(DI-2.D0-NU)*(DI-1.D0+NU)/((DI-1.D0+DMU)*(DI-1.D0))
      CALL DXADJ(A,IA,IERROR)
      IF (IERROR.NE.0) RETURN
      IF(A.EQ.0.D0) GO TO 66
      CALL DXADD(PQ,IPQ,A,IA,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
   65 CONTINUE
   66 CONTINUE
      IF(MU.LE.0) GO TO 90
      X2=R
      X1=PQ
      K=MU
      DO 77 I=1,K
      X1=X1*X2
   77 CALL DXADJ(X1,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      PQ=X1/FACTMU
      IPQ=IPQ-IF
      CALL DXADJ(PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      GO TO 90
C
C        Z=-LN(R)=.5*LN((1+X)/(1-X))
C
   80 Z=-LOG(R)
      W=DXPSI(NU+1.D0,IPSIK,IPSIX)
      XS = SX  ! pour le cas ou XS serait modifie par la suite
*      XS=1.D0/SIN(THETA)
C
C        SERIES SUMMATION FOR Q ( ID = 2 )
C        Q(0,NU,X)=SUM(FROM 0 TO J0-1)((.5*LN((1+X)/(1-X))
C    +DXPSI(J+1,IPSIK,IPSIX)-DXPSI(NU+1,IPSIK,IPSIX)))*A(J)*(.5-.5*X)**J
C
C        Q(1,NU,X)=-SQRT(1./(1.-X**2))+SQRT((1-X)/(1+X))
C             *SUM(FROM 0 T0 J0-1)(-NU*(NU+1)/2*LN((1+X)/(1-X))
C                 +(J-NU)*(J+NU+1)/(2*(J+1))+NU*(NU+1)*
C     (DXPSI(NU+1,IPSIK,IPSIX)-DXPSI(J+1,IPSIK,IPSIX))*A(J)*(.5-.5*X)**J
C
C        NOTE, IN THIS LOOP K=J+1
C
      PQ=0.D0
      IPQ=0
      IA=0
      A=1.D0
      DO 85 K=1,J0
      FLOK=K
      IF(K.EQ.1) GO TO 81
      A=A*Y*(FLOK-2.D0-NU)*(FLOK-1.D0+NU)/((FLOK-1.D0+DMU)*(FLOK-1.D0))
      CALL DXADJ(A,IA,IERROR)
      IF (IERROR.NE.0) RETURN
   81 CONTINUE
      IF(MU.GE.1) GO TO 83
      X1=(DXPSI(FLOK,IPSIK,IPSIX)-W+Z)*A
      IX1=IA
      CALL DXADD(PQ,IPQ,X1,IX1,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      GO TO 85
   83 X1=(NU*(NU+1.D0)*(Z-W+DXPSI(FLOK,IPSIK,IPSIX))+(NU-FLOK+1.D0)
     1  *(NU+FLOK)/(2.D0*FLOK))*A
      IX1=IA
      CALL DXADD(PQ,IPQ,X1,IX1,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
   85 CONTINUE
      IF(MU.GE.1) PQ=-R*PQ
      IXS=0
      IF(MU.GE.1) CALL DXADD(PQ,IPQ,-XS,IXS,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      IF(J.EQ.2) MU=-MU
      IF(J.EQ.2) DMU=-DMU
   90 IF(J.EQ.1) PQ2=PQ
      IF(J.EQ.1) IPQ2=IPQ
      NU=NU+1.D0
  100 CONTINUE
      K=0
      IF(NU-1.5D0.LT.NU1) GO TO 120
      K=K+1
      PQA(K)=PQ2
      IPQA(K)=IPQ2
      IF(NU.GT.NU2+.5D0) RETURN
  120 PQ1=PQ
      IPQ1=IPQ
      IF(NU.LT.NU1+.5D0) GO TO 130
      K=K+1
      PQA(K)=PQ
      IPQA(K)=IPQ
      IF(NU.GT.NU2+.5D0) RETURN
C
C        FORWARD NU-WISE RECURRENCE FOR F(MU,NU,X) FOR FIXED MU
C        USING
C        (NU+MU+1)*F(MU,NU,X)=(2.*NU+1)*F(MU,NU,X)-(NU-MU)*F(MU,NU-1,X)
C        WHERE F(MU,NU,X) MAY BE P(-MU,NU,X) OR IF MU IS REPLACED
C        BY -MU THEN F(MU,NU,X) MAY BE Q(MU,NU,X).
C        NOTE, IN THIS LOOP, NU=NU+1
C
  130 X1=(2.D0*NU-1.D0)/(NU+DMU)*X*PQ1
      X2=(NU-1.D0-DMU)/(NU+DMU)*PQ2
      CALL DXADD(X1,IPQ1,-X2,IPQ2,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      CALL DXADJ(PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      NU=NU+1.D0
      PQ2=PQ1
      IPQ2=IPQ1
      GO TO 120
C
      END
*DECK DXPSI
      DOUBLE PRECISION FUNCTION DXPSI (A, IPSIK, IPSIX)
C***BEGIN PROLOGUE  DXPSI
C***SUBSIDIARY
C***PURPOSE  To compute values of the Psi function for DXLEGF.
C***LIBRARY   SLATEC
C***CATEGORY  C7C
C***TYPE      DOUBLE PRECISION (XPSI-S, DXPSI-D)
C***KEYWORDS  PSI FUNCTION
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXPSI
      DOUBLE PRECISION A,B,C,CNUM,CDENOM
      DIMENSION CNUM(12),CDENOM(12)
      SAVE CNUM, CDENOM
C
C        CNUM(I) AND CDENOM(I) ARE THE ( REDUCED ) NUMERATOR
C        AND 2*I*DENOMINATOR RESPECTIVELY OF THE 2*I TH BERNOULLI
C        NUMBER.
C
      DATA CNUM(1),CNUM(2),CNUM(3),CNUM(4),CNUM(5),CNUM(6),CNUM(7),
     1CNUM(8),CNUM(9),CNUM(10),CNUM(11),CNUM(12)
     2    / 1.D0,     -1.D0,    1.D0,     -1.D0, 1.D0,
     3   -691.D0,  1.D0,     -3617.D0, 43867.D0, -174611.D0, 77683.D0,
     4   -236364091.D0/
      DATA CDENOM(1),CDENOM(2),CDENOM(3),CDENOM(4),CDENOM(5),CDENOM(6),
     1 CDENOM(7),CDENOM(8),CDENOM(9),CDENOM(10),CDENOM(11),CDENOM(12)
     2/12.D0,120.D0,   252.D0,   240.D0,132.D0,
     3  32760.D0, 12.D0,  8160.D0, 14364.D0, 6600.D0, 276.D0, 65520.D0/
C***FIRST EXECUTABLE STATEMENT  DXPSI
      N=MAX(0,IPSIX-INT(A))
      B=N+A
      K1=IPSIK-1
C
C        SERIES EXPANSION FOR A .GT. IPSIX USING IPSIK-1 TERMS.
C
      C=0.D0
      DO 12 I=1,K1
      K=IPSIK-I
   12 C=(C+CNUM(K)/CDENOM(K))/B**2
      DXPSI=LOG(B)-(C+.5D0/B)
      IF(N.EQ.0) GO TO 20
      B=0.D0
C
C        RECURRENCE FOR A .LE. IPSIX.
C
      DO 15 M=1,N
   15 B=B+1.D0/(N-M+A)
      DXPSI=DXPSI-B
   20 RETURN
      END
*DECK DXQMU
      SUBROUTINE DXQMU (NU1, NU2, MU1, MU2, X, SX, ID, PQA, IPQA,
     1   IERROR)
C***BEGIN PROLOGUE  DXQMU
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            Method: forward mu-wise recurrence for Q(MU,NU,X) for fixed
C            nu to obtain Q(MU1,NU,X), Q(MU1+1,NU,X), ..., Q(MU2,NU,X).
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XQMU-S, DXQMU-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADD, DXADJ, DXPQNU
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXQMU
      DIMENSION PQA(*),IPQA(*)
      DOUBLE PRECISION DMU,NU,NU1,NU2,PQ,PQA,PQ1,PQ2,SX,X,X1,X2
C***FIRST EXECUTABLE STATEMENT  DXQMU
      IERROR=0
      MU=0
C
C        CALL DXPQNU TO OBTAIN Q(0.,NU1,X)
C
      CALL DXPQNU(NU1,NU2,MU,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      PQ2=PQA(1)
      IPQ2=IPQA(1)
      MU=1
C
C        CALL DXPQNU TO OBTAIN Q(1.,NU1,X)
C
      CALL DXPQNU(NU1,NU2,MU,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      NU=NU1
      K=0
      MU=1
      DMU=1.D0
      PQ1=PQA(1)
      IPQ1=IPQA(1)
      IF(MU1.GT.0) GO TO 310
      K=K+1
      PQA(K)=PQ2
      IPQA(K)=IPQ2
      IF(MU2.LT.1) GO TO 330
  310 IF(MU1.GT.1) GO TO 320
      K=K+1
      PQA(K)=PQ1
      IPQA(K)=IPQ1
      IF(MU2.LE.1) GO TO 330
  320 CONTINUE
C
C        FORWARD RECURRENCE IN MU TO OBTAIN
C                  Q(MU1,NU,X),Q(MU1+1,NU,X),....,Q(MU2,NU,X) USING
C             Q(MU+1,NU,X)=-2.*MU*X*SQRT(1./(1.-X**2))*Q(MU,NU,X)
C                               -(NU+MU)*(NU-MU+1.)*Q(MU-1,NU,X)
C
      X1=-2.D0*DMU*X*SX*PQ1
      X2=(NU+DMU)*(NU-DMU+1.D0)*PQ2
      CALL DXADD(X1,IPQ1,-X2,IPQ2,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      CALL DXADJ(PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      PQ2=PQ1
      IPQ2=IPQ1
      PQ1=PQ
      IPQ1=IPQ
      MU=MU+1
      DMU=DMU+1.D0
      IF(MU.LT.MU1) GO TO 320
      K=K+1
      PQA(K)=PQ
      IPQA(K)=IPQ
      IF(MU2.GT.MU) GO TO 320
  330 RETURN
      END
*DECK DXQNU
      SUBROUTINE DXQNU (NU1, NU2, MU1, X, SX, ID, PQA, IPQA,
     1   IERROR)
C***BEGIN PROLOGUE  DXQNU
C***SUBSIDIARY
C***PURPOSE  To compute the values of Legendre functions for DXLEGF.
C            Method: backward nu-wise recurrence for Q(MU,NU,X) for
C            fixed mu to obtain Q(MU1,NU1,X), Q(MU1,NU1+1,X), ...,
C            Q(MU1,NU2,X).
C***LIBRARY   SLATEC
C***CATEGORY  C3A2, C9
C***TYPE      DOUBLE PRECISION (XQNU-S, DXQNU-D)
C***KEYWORDS  LEGENDRE FUNCTIONS
C***AUTHOR  Smith, John M., (NBS and George Mason University)
C***ROUTINES CALLED  DXADD, DXADJ, DXPQNU
C***REVISION HISTORY  (YYMMDD)
C   820728  DATE WRITTEN
C   890126  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXQNU
      DIMENSION PQA(*),IPQA(*)
      DOUBLE PRECISION DMU,NU,NU1,NU2,PQ,PQA,PQ1,PQ2,SX,X,X1,X2
      DOUBLE PRECISION PQL1,PQL2
C***FIRST EXECUTABLE STATEMENT  DXQNU
      IERROR=0
      K=0
      PQ2=0.0D0
      IPQ2=0
      PQL2=0.0D0
      IPQL2=0
      IF(MU1.EQ.1) GO TO 290
      MU=0
C
C        CALL DXPQNU TO OBTAIN Q(0.,NU2,X) AND Q(0.,NU2-1,X)
C
      CALL DXPQNU(NU1,NU2,MU,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      IF(MU1.EQ.0) RETURN
      K=(NU2-NU1+1.5D0)
      PQ2=PQA(K)
      IPQ2=IPQA(K)
      PQL2=PQA(K-1)
      IPQL2=IPQA(K-1)
  290 MU=1
C
C        CALL DXPQNU TO OBTAIN Q(1.,NU2,X) AND Q(1.,NU2-1,X)
C
      CALL DXPQNU(NU1,NU2,MU,X,SX,ID,PQA,IPQA,IERROR)
      IF (IERROR.NE.0) RETURN
      IF(MU1.EQ.1) RETURN
      NU=NU2
      PQ1=PQA(K)
      IPQ1=IPQA(K)
      PQL1=PQA(K-1)
      IPQL1=IPQA(K-1)
  300 MU=1
      DMU=1.D0
  320 CONTINUE
C
C        FORWARD RECURRENCE IN MU TO OBTAIN Q(MU1,NU2,X) AND
C              Q(MU1,NU2-1,X) USING
C              Q(MU+1,NU,X)=-2.*MU*X*SQRT(1./(1.-X**2))*Q(MU,NU,X)
C                   -(NU+MU)*(NU-MU+1.)*Q(MU-1,NU,X)
C
C              FIRST FOR NU=NU2
C
      X1=-2.D0*DMU*X*SX*PQ1
      X2=(NU+DMU)*(NU-DMU+1.D0)*PQ2
      CALL DXADD(X1,IPQ1,-X2,IPQ2,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      CALL DXADJ(PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      PQ2=PQ1
      IPQ2=IPQ1
      PQ1=PQ
      IPQ1=IPQ
      MU=MU+1
      DMU=DMU+1.D0
      IF(MU.LT.MU1) GO TO 320
      PQA(K)=PQ
      IPQA(K)=IPQ
      IF(K.EQ.1) RETURN
      IF(NU.LT.NU2) GO TO 340
C
C              THEN FOR NU=NU2-1
C
      NU=NU-1.D0
      PQ2=PQL2
      IPQ2=IPQL2
      PQ1=PQL1
      IPQ1=IPQL1
      K=K-1
      GO TO 300
C
C         BACKWARD RECURRENCE IN NU TO OBTAIN
C              Q(MU1,NU1,X),Q(MU1,NU1+1,X),....,Q(MU1,NU2,X)
C              USING
C              (NU-MU+1.)*Q(MU,NU+1,X)=
C                       (2.*NU+1.)*X*Q(MU,NU,X)-(NU+MU)*Q(MU,NU-1,X)
C
  340 PQ1=PQA(K)
      IPQ1=IPQA(K)
      PQ2=PQA(K+1)
      IPQ2=IPQA(K+1)
  350 IF(NU.LE.NU1) RETURN
      K=K-1
      X1=(2.D0*NU+1.D0)*X*PQ1/(NU+DMU)
      X2=-(NU-DMU+1.D0)*PQ2/(NU+DMU)
      CALL DXADD(X1,IPQ1,X2,IPQ2,PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      CALL DXADJ(PQ,IPQ,IERROR)
      IF (IERROR.NE.0) RETURN
      PQ2=PQ1
      IPQ2=IPQ1
      PQ1=PQ
      IPQ1=IPQ
      PQA(K)=PQ
      IPQA(K)=IPQ
      NU=NU-1.D0
      GO TO 350
      END
*DECK DXRED
      SUBROUTINE DXRED (X, IX, IERROR)
C***BEGIN PROLOGUE  DXRED
C***PURPOSE  To provide double-precision floating-point arithmetic
C            with an extended exponent range.
C***LIBRARY   SLATEC
C***CATEGORY  A3D
C***TYPE      DOUBLE PRECISION (XRED-S, DXRED-D)
C***KEYWORDS  EXTENDED-RANGE DOUBLE-PRECISION ARITHMETIC
C***AUTHOR  Lozier, Daniel W., (National Bureau of Standards)
C           Smith, John M., (NBS and George Mason University)
C***DESCRIPTION
C     DOUBLE PRECISION X
C     INTEGER IX
C
C                  IF
C                  RADIX**(-2L) .LE. (ABS(X),IX) .LE. RADIX**(2L)
C                  THEN DXRED TRANSFORMS (X,IX) SO THAT IX=0.
C                  IF (X,IX) IS OUTSIDE THE ABOVE RANGE,
C                  THEN DXRED TAKES NO ACTION.
C                  THIS SUBROUTINE IS USEFUL IF THE
C                  RESULTS OF EXTENDED-RANGE CALCULATIONS
C                  ARE TO BE USED IN SUBSEQUENT ORDINARY
C                  DOUBLE-PRECISION CALCULATIONS.
C
C***SEE ALSO  DXSET
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***COMMON BLOCKS    DXBLK2
C***REVISION HISTORY  (YYMMDD)
C   820712  DATE WRITTEN
C   881020  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXRED
      DOUBLE PRECISION X
      INTEGER IX
      DOUBLE PRECISION RADIX, RADIXL, RAD2L, DLG10R, XA
      INTEGER L, L2, KMAX
      COMMON /DXBLK2/ RADIX, RADIXL, RAD2L, DLG10R, L, L2, KMAX
      SAVE /DXBLK2/
C
C***FIRST EXECUTABLE STATEMENT  DXRED
      IERROR=0
      IF (X.EQ.0.0D0) GO TO 90
      XA = ABS(X)
      IF (IX.EQ.0) GO TO 70
      IXA = ABS(IX)
      IXA1 = IXA/L2
      IXA2 = MOD(IXA,L2)
      IF (IX.GT.0) GO TO 40
   10 CONTINUE
      IF (XA.GT.1.0D0) GO TO 20
      XA = XA*RAD2L
      IXA1 = IXA1 + 1
      GO TO 10
   20 XA = XA/RADIX**IXA2
      IF (IXA1.EQ.0) GO TO 70
      DO 30 I=1,IXA1
        IF (XA.LT.1.0D0) GO TO 100
        XA = XA/RAD2L
   30 CONTINUE
      GO TO 70
C
   40 CONTINUE
      IF (XA.LT.1.0D0) GO TO 50
      XA = XA/RAD2L
      IXA1 = IXA1 + 1
      GO TO 40
   50 XA = XA*RADIX**IXA2
      IF (IXA1.EQ.0) GO TO 70
      DO 60 I=1,IXA1
        IF (XA.GT.1.0D0) GO TO 100
        XA = XA*RAD2L
   60 CONTINUE
   70 IF (XA.GT.RAD2L) GO TO 100
      IF (XA.GT.1.0D0) GO TO 80
      IF (RAD2L*XA.LT.1.0D0) GO TO 100
   80 X = SIGN(XA,X)
   90 IX = 0
  100 RETURN
      END
*DECK DXSET
      SUBROUTINE DXSET (IRAD, NRADPL, DZERO, NBITS, IERROR)
C***BEGIN PROLOGUE  DXSET
C***PURPOSE  To provide double-precision floating-point arithmetic
C            with an extended exponent range.
C***LIBRARY   SLATEC
C***CATEGORY  A3D
C***TYPE      DOUBLE PRECISION (XSET-S, DXSET-D)
C***KEYWORDS  EXTENDED-RANGE DOUBLE-PRECISION ARITHMETIC
C***AUTHOR  Lozier, Daniel W., (National Bureau of Standards)
C           Smith, John M., (NBS and George Mason University)
C***DESCRIPTION
C
C   SUBROUTINE  DXSET  MUST BE CALLED PRIOR TO CALLING ANY OTHER
C EXTENDED-RANGE SUBROUTINE. IT CALCULATES AND STORES SEVERAL
C MACHINE-DEPENDENT CONSTANTS IN COMMON BLOCKS. THE USER MUST
C SUPPLY FOUR CONSTANTS THAT PERTAIN TO HIS PARTICULAR COMPUTER.
C THE CONSTANTS ARE
C
C          IRAD = THE INTERNAL BASE OF DOUBLE-PRECISION
C                 ARITHMETIC IN THE COMPUTER.
C        NRADPL = THE NUMBER OF RADIX PLACES CARRIED IN
C                 THE DOUBLE-PRECISION REPRESENTATION.
C         DZERO = THE SMALLEST OF 1/DMIN, DMAX, DMAXLN WHERE
C                 DMIN = THE SMALLEST POSITIVE DOUBLE-PRECISION
C                 NUMBER OR AN UPPER BOUND TO THIS NUMBER,
C                 DMAX = THE LARGEST DOUBLE-PRECISION NUMBER
C                 OR A LOWER BOUND TO THIS NUMBER,
C                 DMAXLN = THE LARGEST DOUBLE-PRECISION NUMBER
C                 SUCH THAT LOG10(DMAXLN) CAN BE COMPUTED BY THE
C                 FORTRAN SYSTEM (ON MOST SYSTEMS DMAXLN = DMAX).
C         NBITS = THE NUMBER OF BITS (EXCLUSIVE OF SIGN) IN
C                 AN INTEGER COMPUTER WORD.
C
C ALTERNATIVELY, ANY OR ALL OF THE CONSTANTS CAN BE GIVEN
C THE VALUE 0 (0.0D0 FOR DZERO). IF A CONSTANT IS ZERO, DXSET TRIES
C TO ASSIGN AN APPROPRIATE VALUE BY CALLING I1MACH
C (SEE P.A.FOX, A.D.HALL, N.L.SCHRYER, ALGORITHM 528 FRAMEWORK
C FOR A PORTABLE LIBRARY, ACM TRANSACTIONS ON MATH SOFTWARE,
C V.4, NO.2, JUNE 1978, 177-188).
C
C   THIS IS THE SETTING-UP SUBROUTINE FOR A PACKAGE OF SUBROUTINES
C THAT FACILITATE THE USE OF EXTENDED-RANGE ARITHMETIC. EXTENDED-RANGE
C ARITHMETIC ON A PARTICULAR COMPUTER IS DEFINED ON THE SET OF NUMBERS
C OF THE FORM
C
C               (X,IX) = X*RADIX**IX
C
C WHERE X IS A DOUBLE-PRECISION NUMBER CALLED THE PRINCIPAL PART,
C IX IS AN INTEGER CALLED THE AUXILIARY INDEX, AND RADIX IS THE
C INTERNAL BASE OF THE DOUBLE-PRECISION ARITHMETIC.  OBVIOUSLY,
C EACH REAL NUMBER IS REPRESENTABLE WITHOUT ERROR BY MORE THAN ONE
C EXTENDED-RANGE FORM.  CONVERSIONS BETWEEN  DIFFERENT FORMS ARE
C ESSENTIAL IN CARRYING OUT ARITHMETIC OPERATIONS.  WITH THE CHOICE
C OF RADIX WE HAVE MADE, AND THE SUBROUTINES WE HAVE WRITTEN, THESE
C CONVERSIONS ARE PERFORMED WITHOUT ERROR (AT LEAST ON MOST COMPUTERS).
C (SEE SMITH, J.M., OLVER, F.W.J., AND LOZIER, D.W., EXTENDED-RANGE
C ARITHMETIC AND NORMALIZED LEGENDRE POLYNOMIALS, ACM TRANSACTIONS ON
C MATHEMATICAL SOFTWARE, MARCH 1981).
C
C   AN EXTENDED-RANGE NUMBER  (X,IX)  IS SAID TO BE IN ADJUSTED FORM IF
C X AND IX ARE ZERO OR
C
C           RADIX**(-L) .LE. ABS(X) .LT. RADIX**L
C
C IS SATISFIED, WHERE L IS A COMPUTER-DEPENDENT INTEGER DEFINED IN THIS
C SUBROUTINE. TWO EXTENDED-RANGE NUMBERS IN ADJUSTED FORM CAN BE ADDED,
C SUBTRACTED, MULTIPLIED OR DIVIDED (IF THE DIVISOR IS NONZERO) WITHOUT
C CAUSING OVERFLOW OR UNDERFLOW IN THE PRINCIPAL PART OF THE RESULT.
C WITH PROPER USE OF THE EXTENDED-RANGE SUBROUTINES, THE ONLY OVERFLOW
C THAT CAN OCCUR IS INTEGER OVERFLOW IN THE AUXILIARY INDEX. IF THIS
C IS DETECTED, THE SOFTWARE CALLS XERROR (A GENERAL ERROR-HANDLING
C FORTRAN SUBROUTINE PACKAGE).
C
C   MULTIPLICATION AND DIVISION IS PERFORMED BY SETTING
C
C                 (X,IX)*(Y,IY) = (X*Y,IX+IY)
C OR
C                 (X,IX)/(Y,IY) = (X/Y,IX-IY).
C
C PRE-ADJUSTMENT OF THE OPERANDS IS ESSENTIAL TO AVOID
C OVERFLOW OR  UNDERFLOW OF THE PRINCIPAL PART. SUBROUTINE
C DXADJ (SEE BELOW) MAY BE CALLED TO TRANSFORM ANY EXTENDED-
C RANGE NUMBER INTO ADJUSTED FORM.
C
C   ADDITION AND SUBTRACTION REQUIRE THE USE OF SUBROUTINE DXADD
C (SEE BELOW).  THE INPUT OPERANDS NEED NOT BE IN ADJUSTED FORM.
C HOWEVER, THE RESULT OF ADDITION OR SUBTRACTION IS RETURNED
C IN ADJUSTED FORM.  THUS, FOR EXAMPLE, IF (X,IX),(Y,IY),
C (U,IU),  AND (V,IV) ARE IN ADJUSTED FORM, THEN
C
C                 (X,IX)*(Y,IY) + (U,IU)*(V,IV)
C
C CAN BE COMPUTED AND STORED IN ADJUSTED FORM WITH NO EXPLICIT
C CALLS TO DXADJ.
C
C   WHEN AN EXTENDED-RANGE NUMBER IS TO BE PRINTED, IT MUST BE
C CONVERTED TO AN EXTENDED-RANGE FORM WITH DECIMAL RADIX.  SUBROUTINE
C DXCON IS PROVIDED FOR THIS PURPOSE.
C
C   THE SUBROUTINES CONTAINED IN THIS PACKAGE ARE
C
C     SUBROUTINE DXADD
C USAGE
C                  CALL DXADD(X,IX,Y,IY,Z,IZ,IERROR)
C                  IF (IERROR.NE.0) RETURN
C DESCRIPTION
C                  FORMS THE EXTENDED-RANGE SUM  (Z,IZ) =
C                  (X,IX) + (Y,IY).  (Z,IZ) IS ADJUSTED
C                  BEFORE RETURNING. THE INPUT OPERANDS
C                  NEED NOT BE IN ADJUSTED FORM, BUT THEIR
C                  PRINCIPAL PARTS MUST SATISFY
C                  RADIX**(-2L).LE.ABS(X).LE.RADIX**(2L),
C                  RADIX**(-2L).LE.ABS(Y).LE.RADIX**(2L).
C
C     SUBROUTINE DXADJ
C USAGE
C                  CALL DXADJ(X,IX,IERROR)
C                  IF (IERROR.NE.0) RETURN
C DESCRIPTION
C                  TRANSFORMS (X,IX) SO THAT
C                  RADIX**(-L) .LE. ABS(X) .LT. RADIX**L.
C                  ON MOST COMPUTERS THIS TRANSFORMATION DOES
C                  NOT CHANGE THE MANTISSA OF X PROVIDED RADIX IS
C                  THE NUMBER BASE OF DOUBLE-PRECISION ARITHMETIC.
C
C     SUBROUTINE DXC210
C USAGE
C                  CALL DXC210(K,Z,J,IERROR)
C                  IF (IERROR.NE.0) RETURN
C DESCRIPTION
C                  GIVEN K THIS SUBROUTINE COMPUTES J AND Z
C                  SUCH THAT  RADIX**K = Z*10**J, WHERE Z IS IN
C                  THE RANGE 1/10 .LE. Z .LT. 1.
C                  THE VALUE OF Z WILL BE ACCURATE TO FULL
C                  DOUBLE-PRECISION PROVIDED THE NUMBER
C                  OF DECIMAL PLACES IN THE LARGEST
C                  INTEGER PLUS THE NUMBER OF DECIMAL
C                  PLACES CARRIED IN DOUBLE-PRECISION DOES NOT
C                  EXCEED 60. DXC210 IS CALLED BY SUBROUTINE
C                  DXCON WHEN NECESSARY. THE USER SHOULD
C                  NEVER NEED TO CALL DXC210 DIRECTLY.
C
C     SUBROUTINE DXCON
C USAGE
C                  CALL DXCON(X,IX,IERROR)
C                  IF (IERROR.NE.0) RETURN
C DESCRIPTION
C                  CONVERTS (X,IX) = X*RADIX**IX
C                  TO DECIMAL FORM IN PREPARATION FOR
C                  PRINTING, SO THAT (X,IX) = X*10**IX
C                  WHERE 1/10 .LE. ABS(X) .LT. 1
C                  IS RETURNED, EXCEPT THAT IF
C                  (ABS(X),IX) IS BETWEEN RADIX**(-2L)
C                  AND RADIX**(2L) THEN THE REDUCED
C                  FORM WITH IX = 0 IS RETURNED.
C
C     SUBROUTINE DXRED
C USAGE
C                  CALL DXRED(X,IX,IERROR)
C                  IF (IERROR.NE.0) RETURN
C DESCRIPTION
C                  IF
C                  RADIX**(-2L) .LE. (ABS(X),IX) .LE. RADIX**(2L)
C                  THEN DXRED TRANSFORMS (X,IX) SO THAT IX=0.
C                  IF (X,IX) IS OUTSIDE THE ABOVE RANGE,
C                  THEN DXRED TAKES NO ACTION.
C                  THIS SUBROUTINE IS USEFUL IF THE
C                  RESULTS OF EXTENDED-RANGE CALCULATIONS
C                  ARE TO BE USED IN SUBSEQUENT ORDINARY
C                  DOUBLE-PRECISION CALCULATIONS.
C
C***REFERENCES  Smith, Olver and Lozier, Extended-Range Arithmetic and
C                 Normalized Legendre Polynomials, ACM Trans on Math
C                 Softw, v 7, n 1, March 1981, pp 93--105.
C***ROUTINES CALLED  I1MACH, XERMSG
C***COMMON BLOCKS    DXBLK1, DXBLK2, DXBLK3
C***REVISION HISTORY  (YYMMDD)
C   820712  DATE WRITTEN
C   881020  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C           CALLs to XERROR changed to CALLs to XERMSG.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXSET
      INTEGER IRAD, NRADPL, NBITS
      DOUBLE PRECISION DZERO, DZEROX
      COMMON /DXBLK1/ NBITSF
      SAVE /DXBLK1/
      DOUBLE PRECISION RADIX, RADIXL, RAD2L, DLG10R
      INTEGER L, L2, KMAX
      COMMON /DXBLK2/ RADIX, RADIXL, RAD2L, DLG10R, L, L2, KMAX
      SAVE /DXBLK2/
      INTEGER NLG102, MLG102, LG102
      COMMON /DXBLK3/ NLG102, MLG102, LG102(21)
      SAVE /DXBLK3/
      INTEGER IFLAG
      SAVE IFLAG

*     dlamch is used in place of i1mach :
      double precision dlamch
      external         dlamch
C
      DIMENSION LOG102(20), LGTEMP(20)
      SAVE LOG102
C
C   LOG102 CONTAINS THE FIRST 60 DIGITS OF LOG10(2) FOR USE IN
C CONVERSION OF EXTENDED-RANGE NUMBERS TO BASE 10 .
      DATA LOG102 /301,029,995,663,981,195,213,738,894,724,493,026,768,
     * 189,881,462,108,541,310,428/
C
C FOLLOWING CODING PREVENTS DXSET FROM BEING EXECUTED MORE THAN ONCE.
C THIS IS IMPORTANT BECAUSE SOME SUBROUTINES (SUCH AS DXNRMP AND
C DXLEGF) CALL DXSET TO MAKE SURE EXTENDED-RANGE ARITHMETIC HAS
C BEEN INITIALIZED. THE USER MAY WANT TO PRE-EMPT THIS CALL, FOR
C EXAMPLE WHEN I1MACH IS NOT AVAILABLE. SEE CODING BELOW.
      DATA IFLAG /0/
C***FIRST EXECUTABLE STATEMENT  DXSET
      IERROR=0
      IF (IFLAG .NE. 0) RETURN
      IRADX = IRAD
      NRDPLC = NRADPL
      DZEROX = DZERO
      IMINEX = 0
      IMAXEX = 0
      NBITSX = NBITS
C FOLLOWING 5 STATEMENTS SHOULD BE DELETED IF I1MACH IS
C NOT AVAILABLE OR NOT CONFIGURED TO RETURN THE CORRECT
C MACHINE-DEPENDENT VALUES.
*
* modif : use a call to dlamch in place of I1MACH
      IF (IRADX .EQ. 0) IRADX = int(dlamch('b'))       ! I1MACH (10)
      IF (NRDPLC .EQ. 0) NRDPLC = int(dlamch('n'))     ! I1MACH (14)
      IF (DZEROX .EQ. 0.0D0) IMINEX = int(dlamch('m')) ! I1MACH (15)
      IF (DZEROX .EQ. 0.0D0) IMAXEX = int(dlamch('l')) ! I1MACH (16)
      IF (NBITSX .EQ. 0) NBITSX = 31                   ! I1MACH (8)
      IF (IRADX.EQ.2) GO TO 10
      IF (IRADX.EQ.4) GO TO 10
      IF (IRADX.EQ.8) GO TO 10
      IF (IRADX.EQ.16) GO TO 10
*      CALL XERMSG ('SLATEC', 'DXSET', 'IMPROPER VALUE OF IRAD', 201, 1)
      IERROR=201
      RETURN
   10 CONTINUE
      LOG2R=0
      IF (IRADX.EQ.2) LOG2R = 1
      IF (IRADX.EQ.4) LOG2R = 2
      IF (IRADX.EQ.8) LOG2R = 3
      IF (IRADX.EQ.16) LOG2R = 4
      NBITSF=LOG2R*NRDPLC
      RADIX = IRADX
      DLG10R = LOG10(RADIX)
      IF (DZEROX .NE. 0.0D0) GO TO 14
      LX = MIN ((1-IMINEX)/2, (IMAXEX-1)/2)
      GO TO 16
   14 LX = 0.5D0*LOG10(DZEROX)/DLG10R
C RADIX**(2*L) SHOULD NOT OVERFLOW, BUT REDUCE L BY 1 FOR FURTHER
C PROTECTION.
      LX=LX-1
   16 L2 = 2*LX
      IF (LX.GE.4) GO TO 20
*      CALL XERMSG ('SLATEC', 'DXSET', 'IMPROPER VALUE OF DZERO', 202, 1)
      IERROR=202
      RETURN
   20 L = LX
      RADIXL = RADIX**L
      RAD2L = RADIXL**2
C    IT IS NECESSARY TO RESTRICT NBITS (OR NBITSX) TO BE LESS THAN SOME
C UPPER LIMIT BECAUSE OF BINARY-TO-DECIMAL CONVERSION. SUCH CONVERSION
C IS DONE BY DXC210 AND REQUIRES A CONSTANT THAT IS STORED TO SOME FIXED
C PRECISION. THE STORED CONSTANT (LOG102 IN THIS ROUTINE) PROVIDES
C FOR CONVERSIONS ACCURATE TO THE LAST DECIMAL DIGIT WHEN THE INTEGER
C WORD LENGTH DOES NOT EXCEED 63. A LOWER LIMIT OF 15 BITS IS IMPOSED
C BECAUSE THE SOFTWARE IS DESIGNED TO RUN ON COMPUTERS WITH INTEGER WORD
C LENGTH OF AT LEAST 16 BITS.
      IF (15.LE.NBITSX .AND. NBITSX.LE.63) GO TO 30
*      CALL XERMSG ('SLATEC', 'DXSET', 'IMPROPER VALUE OF NBITS', 203, 1)
      IERROR=203
      RETURN
   30 CONTINUE
      KMAX = 2**(NBITSX-1) - L2
      NB = (NBITSX-1)/2
      MLG102 = 2**NB
      IF (1.LE.NRDPLC*LOG2R .AND. NRDPLC*LOG2R.LE.120) GO TO 40
*      CALL XERMSG ('SLATEC', 'DXSET', 'IMPROPER VALUE OF NRADPL', 204,
*     +             1)
      IERROR=204
      RETURN
   40 CONTINUE
      NLG102 = NRDPLC*LOG2R/NB + 3
      NP1 = NLG102 + 1
C
C   AFTER COMPLETION OF THE FOLLOWING LOOP, IC CONTAINS
C THE INTEGER PART AND LGTEMP CONTAINS THE FRACTIONAL PART
C OF LOG10(IRADX) IN RADIX 1000.
      IC = 0
      DO 50 II=1,20
        I = 21 - II
        IT = LOG2R*LOG102(I) + IC
        IC = IT/1000
        LGTEMP(I) = MOD(IT,1000)
   50 CONTINUE
C
C   AFTER COMPLETION OF THE FOLLOWING LOOP, LG102 CONTAINS
C LOG10(IRADX) IN RADIX MLG102. THE RADIX POINT IS
C BETWEEN LG102(1) AND LG102(2).
      LG102(1) = IC
      DO 80 I=2,NP1
        LG102X = 0
        DO 70 J=1,NB
          IC = 0
          DO 60 KK=1,20
            K = 21 - KK
            IT = 2*LGTEMP(K) + IC
            IC = IT/1000
            LGTEMP(K) = MOD(IT,1000)
   60     CONTINUE
          LG102X = 2*LG102X + IC
   70   CONTINUE
        LG102(I) = LG102X
   80 CONTINUE
C
C CHECK SPECIAL CONDITIONS REQUIRED BY SUBROUTINES...
      IF (NRDPLC.LT.L) GO TO 90
*      CALL XERMSG ('SLATEC', 'DXSET', 'NRADPL .GE. L', 205, 1)
      IERROR=205
      RETURN
   90 IF (6*L.LE.KMAX) GO TO 100
*      CALL XERMSG ('SLATEC', 'DXSET', '6*L .GT. KMAX', 206, 1)
      IERROR=206
      RETURN
  100 CONTINUE
      IFLAG = 1
      RETURN
      END

      SUBROUTINE DXADD (X, IX, Y, IY, Z, IZ, IERROR)
C***BEGIN PROLOGUE  DXADD
C***PURPOSE  To provide double-precision floating-point arithmetic
C            with an extended exponent range.
C***LIBRARY   SLATEC
C***CATEGORY  A3D
C***TYPE      DOUBLE PRECISION (XADD-S, DXADD-D)
C***KEYWORDS  EXTENDED-RANGE DOUBLE-PRECISION ARITHMETIC
C***AUTHOR  Lozier, Daniel W., (National Bureau of Standards)
C           Smith, John M., (NBS and George Mason University)
C***DESCRIPTION
C     DOUBLE PRECISION X, Y, Z
C     INTEGER IX, IY, IZ
C
C                  FORMS THE EXTENDED-RANGE SUM  (Z,IZ) =
C                  (X,IX) + (Y,IY).  (Z,IZ) IS ADJUSTED
C                  BEFORE RETURNING. THE INPUT OPERANDS
C                  NEED NOT BE IN ADJUSTED FORM, BUT THEIR
C                  PRINCIPAL PARTS MUST SATISFY
C                  RADIX**(-2L).LE.ABS(X).LE.RADIX**(2L),
C                  RADIX**(-2L).LE.ABS(Y).LE.RADIX**(2L).
C
C***SEE ALSO  DXSET
C***REFERENCES  (NONE)
C***ROUTINES CALLED  DXADJ
C***COMMON BLOCKS    DXBLK2
C***REVISION HISTORY  (YYMMDD)
C   820712  DATE WRITTEN
C   881020  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXADD
      DOUBLE PRECISION X, Y, Z
      INTEGER IX, IY, IZ
      DOUBLE PRECISION RADIX, RADIXL, RAD2L, DLG10R
      INTEGER L, L2, KMAX
      COMMON /DXBLK2/ RADIX, RADIXL, RAD2L, DLG10R, L, L2, KMAX
      SAVE /DXBLK2/
      DOUBLE PRECISION S, T
C
C   THE CONDITIONS IMPOSED ON L AND KMAX BY THIS SUBROUTINE
C ARE
C     (1) 1 .LT. L .LE. 0.5D0*LOGR(0.5D0*DZERO)
C
C     (2) NRADPL .LT. L .LE. KMAX/6
C
C     (3) KMAX .LE. (2**NBITS - 4*L - 1)/2
C
C THESE CONDITIONS MUST BE MET BY APPROPRIATE CODING
C IN SUBROUTINE DXSET.
C
C***FIRST EXECUTABLE STATEMENT  DXADD
      IERROR=0
      IF (X.NE.0.0D0) GO TO 10
      Z = Y
      IZ = IY
      GO TO 220
   10 IF (Y.NE.0.0D0) GO TO 20
      Z = X
      IZ = IX
      GO TO 220
   20 CONTINUE
      IF (IX.GE.0 .AND. IY.GE.0) GO TO 40
      IF (IX.LT.0 .AND. IY.LT.0) GO TO 40
      IF (ABS(IX).LE.6*L .AND. ABS(IY).LE.6*L) GO TO 40
      IF (IX.GE.0) GO TO 30
      Z = Y
      IZ = IY
      GO TO 220
   30 CONTINUE
      Z = X
      IZ = IX
      GO TO 220
   40 I = IX - IY
      if (I .lt. 0) then 
         goto 80
      elseif (I .eq. 0) then 
         goto 50
      else
         goto 90
      endif
   50 IF (ABS(X).GT.1.0D0 .AND. ABS(Y).GT.1.0D0) GO TO 60
      IF (ABS(X).LT.1.0D0 .AND. ABS(Y).LT.1.0D0) GO TO 70
      Z = X + Y
      IZ = IX
      GO TO 220
   60 S = X/RADIXL
      T = Y/RADIXL
      Z = S + T
      IZ = IX + L
      GO TO 220
   70 S = X*RADIXL
      T = Y*RADIXL
      Z = S + T
      IZ = IX - L
      GO TO 220
   80 S = Y
      IS = IY
      T = X
      GO TO 100
   90 S = X
      IS = IX
      T = Y
  100 CONTINUE
C
C  AT THIS POINT, THE ONE OF (X,IX) OR (Y,IY) THAT HAS THE
C LARGER AUXILIARY INDEX IS STORED IN (S,IS). THE PRINCIPAL
C PART OF THE OTHER INPUT IS STORED IN T.
C
      I1 = ABS(I)/L
      I2 = MOD(ABS(I),L)
      IF (ABS(T).GE.RADIXL) GO TO 130
      IF (ABS(T).GE.1.0D0) GO TO 120
      IF (RADIXL*ABS(T).GE.1.0D0) GO TO 110
      J = I1 + 1
      T = T*RADIX**(L-I2)
      GO TO 140
  110 J = I1
      T = T*RADIX**(-I2)
      GO TO 140
  120 J = I1 - 1
      IF (J.LT.0) GO TO 110
      T = T*RADIX**(-I2)/RADIXL
      GO TO 140
  130 J = I1 - 2
      IF (J.LT.0) GO TO 120
      T = T*RADIX**(-I2)/RAD2L
  140 CONTINUE
C
C  AT THIS POINT, SOME OR ALL OF THE DIFFERENCE IN THE
C AUXILIARY INDICES HAS BEEN USED TO EFFECT A LEFT SHIFT
C OF T.  THE SHIFTED VALUE OF T SATISFIES
C
C       RADIX**(-2*L) .LE. ABS(T) .LE. 1.0D0
C
C AND, IF J=0, NO FURTHER SHIFTING REMAINS TO BE DONE.
C
      IF (J.EQ.0) GO TO 190
      IF (ABS(S).GE.RADIXL .OR. J.GT.3) GO TO 150
      IF (ABS(S).GE.1.0D0) GO TO (180, 150, 150), J
      IF (RADIXL*ABS(S).GE.1.0D0) GO TO (180, 170, 150), J
      GO TO (180, 170, 160), J
  150 Z = S
      IZ = IS
      GO TO 220
  160 S = S*RADIXL
  170 S = S*RADIXL
  180 S = S*RADIXL
  190 CONTINUE
C
C   AT THIS POINT, THE REMAINING DIFFERENCE IN THE
C AUXILIARY INDICES HAS BEEN USED TO EFFECT A RIGHT SHIFT
C OF S.  IF THE SHIFTED VALUE OF S WOULD HAVE EXCEEDED
C RADIX**L, THEN (S,IS) IS RETURNED AS THE VALUE OF THE
C SUM.
C
      IF (ABS(S).GT.1.0D0 .AND. ABS(T).GT.1.0D0) GO TO 200
      IF (ABS(S).LT.1.0D0 .AND. ABS(T).LT.1.0D0) GO TO 210
      Z = S + T
      IZ = IS - J*L
      GO TO 220
  200 S = S/RADIXL
      T = T/RADIXL
      Z = S + T
      IZ = IS - J*L + L
      GO TO 220
  210 S = S*RADIXL
      T = T*RADIXL
      Z = S + T
      IZ = IS - J*L - L
  220 CALL DXADJ(Z, IZ,IERROR)
      RETURN
      END
*DECK DXADJ
      SUBROUTINE DXADJ (X, IX, IERROR)
C***BEGIN PROLOGUE  DXADJ
C***PURPOSE  To provide double-precision floating-point arithmetic
C            with an extended exponent range.
C***LIBRARY   SLATEC
C***CATEGORY  A3D
C***TYPE      DOUBLE PRECISION (XADJ-S, DXADJ-D)
C***KEYWORDS  EXTENDED-RANGE DOUBLE-PRECISION ARITHMETIC
C***AUTHOR  Lozier, Daniel W., (National Bureau of Standards)
C           Smith, John M., (NBS and George Mason University)
C***DESCRIPTION
C     DOUBLE PRECISION X
C     INTEGER IX
C
C                  TRANSFORMS (X,IX) SO THAT
C                  RADIX**(-L) .LE. ABS(X) .LT. RADIX**L.
C                  ON MOST COMPUTERS THIS TRANSFORMATION DOES
C                  NOT CHANGE THE MANTISSA OF X PROVIDED RADIX IS
C                  THE NUMBER BASE OF DOUBLE-PRECISION ARITHMETIC.
C
C***SEE ALSO  DXSET
C***REFERENCES  (NONE)
C***ROUTINES CALLED  XERMSG
C***COMMON BLOCKS    DXBLK2
C***REVISION HISTORY  (YYMMDD)
C   820712  DATE WRITTEN
C   881020  Revised to meet SLATEC CML recommendations.  (DWL and JMS)
C   901019  Revisions to prologue.  (DWL and WRB)
C   901106  Changed all specific intrinsics to generic.  (WRB)
C           Corrected order of sections in prologue and added TYPE
C           section.  (WRB)
C           CALLs to XERROR changed to CALLs to XERMSG.  (WRB)
C   920127  Revised PURPOSE section of prologue.  (DWL)
C***END PROLOGUE  DXADJ
      DOUBLE PRECISION X
      INTEGER IX
      DOUBLE PRECISION RADIX, RADIXL, RAD2L, DLG10R
      INTEGER L, L2, KMAX
      COMMON /DXBLK2/ RADIX, RADIXL, RAD2L, DLG10R, L, L2, KMAX
      SAVE /DXBLK2/
C
C   THE CONDITION IMPOSED ON L AND KMAX BY THIS SUBROUTINE
C IS
C     2*L .LE. KMAX
C
C THIS CONDITION MUST BE MET BY APPROPRIATE CODING
C IN SUBROUTINE DXSET.
C
C***FIRST EXECUTABLE STATEMENT  DXADJ
      IERROR=0
      IF (X.EQ.0.0D0) GO TO 50
      IF (ABS(X).GE.1.0D0) GO TO 20
      IF (RADIXL*ABS(X).GE.1.0D0) GO TO 60
      X = X*RAD2L
      IF (IX.LT.0) GO TO 10
      IX = IX - L2
      GO TO 70
   10 IF (IX.LT.-KMAX+L2) GO TO 40
      IX = IX - L2
      GO TO 70
   20 IF (ABS(X).LT.RADIXL) GO TO 60
      X = X/RAD2L
      IF (IX.GT.0) GO TO 30
      IX = IX + L2
      GO TO 70
   30 IF (IX.GT.KMAX-L2) GO TO 40
      IX = IX + L2
      GO TO 70
 40   continue
*   40 CALL XERMSG ('SLATEC', 'DXADJ', 'overflow in auxiliary index',
*     +             207, 1)
      IERROR=207
      RETURN
   50 IX = 0
   60 IF (ABS(IX).GT.KMAX) GO TO 40
   70 RETURN
      END
