      DOUBLE PRECISION FUNCTION DBVALU(T,A,N,K,IDERIV,X,INBV,WORK)
C***BEGIN PROLOGUE  DBVALU
C***DATE WRITTEN   800901   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***CATEGORY NO.  E3,K6
C***KEYWORDS  B-SPLINE,DATA FITTING,DOUBLE PRECISION,INTERPOLATION,
C             SPLINE
C***AUTHOR  AMOS, D. E., (SNLA)
C***PURPOSE  Evaluates the B-representation of a B-spline at X for the
C            function value or any of its derivatives.
C***DESCRIPTION
C
C     Written by Carl de Boor and modified by D. E. Amos
C
C     Reference
C         SIAM J. Numerical Analysis, 14, No. 3, June, 1977, pp.441-472.
C
C     Abstract   **** a double precision routine ****
C         DBVALU is the BVALUE function of the reference.
C
C         DBVALU evaluates the B-representation (T,A,N,K) of a B-spline
C         at X for the function value on IDERIV=0 or any of its
C         derivatives on IDERIV=1,2,...,K-1.  Right limiting values
C         (right derivatives) are returned except at the right end
C         point X=T(N+1) where left limiting values are computed.  The
C         spline is defined on T(K) .LE. X .LE. T(N+1).  DBVALU returns
C         a fatal error message when X is outside of this interval.
C
C         To compute left derivatives or left limiting values at a
C         knot T(I), replace N by I-1 and set X=T(I), I=K+1,N+1.
C
C         DBVALU calls DINTRV
C
C     Description of Arguments
C
C         Input      T,A,X are double precision
C          T       - knot vector of length N+K
C          A       - B-spline coefficient vector of length N
C          N       - number of B-spline coefficients
C                    N = sum of knot multiplicities-K
C          K       - order of the B-spline, K .GE. 1
C          IDERIV  - order of the derivative, 0 .LE. IDERIV .LE. K-1
C                    IDERIV = 0 returns the B-spline value
C          X       - argument, T(K) .LE. X .LE. T(N+1)
C          INBV    - an initialization parameter which must be set
C                    to 1 the first time DBVALU is called.
C
C         Output     WORK,DBVALU are double precision
C          INBV    - INBV contains information for efficient process-
C                    ing after the initial call and INBV must not
C                    be changed by the user.  Distinct splines require
C                    distinct INBV parameters.
C          WORK    - work vector of length 3*K.
C          DBVALU  - value of the IDERIV-th derivative at X
C
C     Error Conditions
C         An improper input is a fatal error
C***REFERENCES  C. DE BOOR, *PACKAGE FOR CALCULATING WITH B-SPLINES*,
C                 SIAM JOURNAL ON NUMERICAL ANALYSIS, VOLUME 14, NO. 3,
C                 JUNE 1977, PP. 441-472.
C***ROUTINES CALLED  DINTRV,XERROR
C***END PROLOGUE  DBVALU
C
C
      INTEGER I,IDERIV,IDERP1,IHI,IHMKMJ,ILO,IMK,IMKPJ, INBV, IPJ,
     1 IP1, IP1MJ, J, JJ, J1, J2, K, KMIDER, KMJ, KM1, KPK, MFLAG, N
      DOUBLE PRECISION A, FKMJ, T, WORK, X
      DIMENSION T(*), A(N), WORK(*)
C***FIRST EXECUTABLE STATEMENT  DBVALU
      DBVALU = 0.0D0
      IF(K.LT.1) GO TO 102
      IF(N.LT.K) GO TO 101
      IF(IDERIV.LT.0 .OR. IDERIV.GE.K) GO TO 110
      KMIDER = K - IDERIV
C
C *** FIND *I* IN (K,N) SUCH THAT T(I) .LE. X .LT. T(I+1)
C     (OR, .LE. T(I+1) IF T(I) .LT. T(I+1) = T(N+1)).
      KM1 = K - 1
      CALL DINTRV(T, N+1, X, INBV, I, MFLAG)
      IF (X.LT.T(K)) GO TO 120
      IF (MFLAG.EQ.0) GO TO 20
      IF (X.GT.T(I)) GO TO 130
   10 IF (I.EQ.K) GO TO 140
      I = I - 1
      IF (X.EQ.T(I)) GO TO 10
C
C *** DIFFERENCE THE COEFFICIENTS *IDERIV* TIMES
C     WORK(I) = AJ(I), WORK(K+I) = DP(I), WORK(K+K+I) = DM(I), I=1.K
C
   20 IMK = I - K
      DO 30 J=1,K
        IMKPJ = IMK + J
        WORK(J) = A(IMKPJ)
   30 CONTINUE
      IF (IDERIV.EQ.0) GO TO 60
      DO 50 J=1,IDERIV
        KMJ = K - J
        FKMJ = DBLE(FLOAT(KMJ))
        DO 40 JJ=1,KMJ
          IHI = I + JJ
          IHMKMJ = IHI - KMJ
          WORK(JJ) = (WORK(JJ+1)-WORK(JJ))/(T(IHI)-T(IHMKMJ))*FKMJ
   40   CONTINUE
   50 CONTINUE
C
C *** COMPUTE VALUE AT *X* IN (T(I),(T(I+1)) OF IDERIV-TH DERIVATIVE,
C     GIVEN ITS RELEVANT B-SPLINE COEFF. IN AJ(1),...,AJ(K-IDERIV).
   60 IF (IDERIV.EQ.KM1) GO TO 100
      IP1 = I + 1
      KPK = K + K
      J1 = K + 1
      J2 = KPK + 1
      DO 70 J=1,KMIDER
        IPJ = I + J
        WORK(J1) = T(IPJ) - X
        IP1MJ = IP1 - J
        WORK(J2) = X - T(IP1MJ)
        J1 = J1 + 1
        J2 = J2 + 1
   70 CONTINUE
      IDERP1 = IDERIV + 1
      DO 90 J=IDERP1,KM1
        KMJ = K - J
        ILO = KMJ
        DO 80 JJ=1,KMJ
          WORK(JJ) = (WORK(JJ+1)*WORK(KPK+ILO)+WORK(JJ)
     1              *WORK(K+JJ))/(WORK(KPK+ILO)+WORK(K+JJ))
          ILO = ILO - 1
   80   CONTINUE
   90 CONTINUE
  100 DBVALU = WORK(1)
      RETURN
C
C
  101 CONTINUE
!      CALL XERROR( ' DBVALU,  N DOES NOT SATISFY N.GE.K',35,2,1)
      print *, ' DBVALU,  N DOES NOT SATISFY N.GE.K'
      RETURN
  102 CONTINUE
!      CALL XERROR( ' DBVALU,  K DOES NOT SATISFY K.GE.1',35,2,1)
      print *, ' DBVALU,  K DOES NOT SATISFY K.GE.1'
      RETURN
  110 CONTINUE
!      CALL XERROR( ' DBVALU,  IDERIV DOES NOT SATISFY 0.LE.IDERIV.LT.K',
      print *, ' DBVALU,  IDERIV DOES NOT SATISFY 0.LE.IDERIV.LT.K'

      RETURN
  120 CONTINUE
!      CALL XERROR( ' DBVALU,  X IS N0T GREATER THAN OR EQUAL TO T(K)'
      print *, ' DBVALU,  X IS N0T GREATER THAN OR EQUAL TO T(K)'
      RETURN
  130 CONTINUE
*      CALL XERROR( ' DBVALU,  X IS NOT LESS THAN OR EQUAL TO T(N+1)',
*     1 47, 2, 1)
      print *,  ' DBVALU,  X IS NOT LESS THAN OR EQUAL TO T(N+1)'
      RETURN
  140 CONTINUE
*      CALL XERROR( ' DBVALU,  A LEFT LIMITING VALUE CANN0T BE OBTAINED A
*     1T T(K)',    58, 2, 1)
      print *,' DBVALU, A LEFT LIMITING VALUE CANT BE OBTAINED AT T(K)'
      RETURN
      END

      SUBROUTINE DINTRV(XT,LXT,X,ILO,ILEFT,MFLAG)
C***BEGIN PROLOGUE  DINTRV
C***DATE WRITTEN   800901   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  E3,K6
C***KEYWORDS  B-SPLINE,DATA FITTING,DOUBLE PRECISION,INTERPOLATION,
C             SPLINE
C***AUTHOR  AMOS, D. E., (SNLA)
C***PURPOSE  Computes the largest integer ILEFT in 1.LE.ILEFT.LE.LXT
C            such that XT(ILEFT).LE.X where XT(*) is a subdivision of
C            the X interval.
C***DESCRIPTION
C
C     Written by Carl de Boor and modified by D. E. Amos
C
C     Reference
C         SIAM J.  Numerical Analysis, 14, No. 3, June 1977, pp.441-472.
C
C     Abstract    **** a double precision routine ****
C         DINTRV is the INTERV routine of the reference.
C
C         DINTRV computes the largest integer ILEFT in 1 .LE. ILEFT .LE.
C         LXT such that XT(ILEFT) .LE. X where XT(*) is a subdivision of
C         the X interval.  Precisely,
C
C                      X .LT. XT(1)                1         -1
C         if  XT(I) .LE. X .LT. XT(I+1)  then  ILEFT=I  , MFLAG=0
C           XT(LXT) .LE. X                         LXT        1,
C
C         That is, when multiplicities are present in the break point
C         to the left of X, the largest index is taken for ILEFT.
C
C     Description of Arguments
C
C         Input      XT,X are double precision
C          XT      - XT is a knot or break point vector of length LXT
C          LXT     - length of the XT vector
C          X       - argument
C          ILO     - an initialization parameter which must be set
C                    to 1 the first time the spline array XT is
C                    processed by DINTRV.
C
C         Output
C          ILO     - ILO contains information for efficient process-
C                    ing after the initial call and ILO must not be
C                    changed by the user.  Distinct splines require
C                    distinct ILO parameters.
C          ILEFT   - largest integer satisfying XT(ILEFT) .LE. X
C          MFLAG   - signals when X lies out of bounds
C
C     Error Conditions
C         None
C***REFERENCES  C. DE BOOR, *PACKAGE FOR CALCULATING WITH B-SPLINES*,
C                 SIAM JOURNAL ON NUMERICAL ANALYSIS, VOLUME 14, NO. 3,
C                 JUNE 1977, PP. 441-472.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  DINTRV
C
C
      INTEGER IHI, ILEFT, ILO, ISTEP, LXT, MFLAG, MIDDLE
      DOUBLE PRECISION X, XT
      DIMENSION XT(LXT)
C***FIRST EXECUTABLE STATEMENT  DINTRV
      IHI = ILO + 1
      IF (IHI.LT.LXT) GO TO 10
      IF (X.GE.XT(LXT)) GO TO 110
      IF (LXT.LE.1) GO TO 90
      ILO = LXT - 1
      IHI = LXT
C
   10 IF (X.GE.XT(IHI)) GO TO 40
      IF (X.GE.XT(ILO)) GO TO 100
C
C *** NOW X .LT. XT(IHI) . FIND LOWER BOUND
      ISTEP = 1
   20 IHI = ILO
      ILO = IHI - ISTEP
      IF (ILO.LE.1) GO TO 30
      IF (X.GE.XT(ILO)) GO TO 70
      ISTEP = ISTEP*2
      GO TO 20
   30 ILO = 1
      IF (X.LT.XT(1)) GO TO 90
      GO TO 70
C *** NOW X .GE. XT(ILO) . FIND UPPER BOUND
   40 ISTEP = 1
   50 ILO = IHI
      IHI = ILO + ISTEP
      IF (IHI.GE.LXT) GO TO 60
      IF (X.LT.XT(IHI)) GO TO 70
      ISTEP = ISTEP*2
      GO TO 50
   60 IF (X.GE.XT(LXT)) GO TO 110
      IHI = LXT
C
C *** NOW XT(ILO) .LE. X .LT. XT(IHI) . NARROW THE INTERVAL
   70 MIDDLE = (ILO+IHI)/2
      IF (MIDDLE.EQ.ILO) GO TO 100
C     NOTE. IT IS ASSUMED THAT MIDDLE = ILO IN CASE IHI = ILO+1
      IF (X.LT.XT(MIDDLE)) GO TO 80
      ILO = MIDDLE
      GO TO 70
   80 IHI = MIDDLE
      GO TO 70
C *** SET OUTPUT AND RETURN
   90 MFLAG = -1
      ILEFT = 1
      RETURN
  100 MFLAG = 0
      ILEFT = ILO
      RETURN
  110 MFLAG = 1
      ILEFT = LXT
      RETURN
      END

      SUBROUTINE DBKNOT(X,N,K,T)
C***BEGIN PROLOGUE  DBKNOT
C***REFER TO  DB2INK,DB3INK
C***ROUTINES CALLED  (NONE)
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***END PROLOGUE  DBKNOT
C
C  --------------------------------------------------------------------
C  DBKNOT CHOOSES A KNOT SEQUENCE FOR INTERPOLATION OF ORDER K AT THE
C  DATA POINTS X(I), I=1,..,N.  THE N+K KNOTS ARE PLACED IN THE ARRAY
C  T.  K KNOTS ARE PLACED AT EACH ENDPOINT AND NOT-A-KNOT END
C  CONDITIONS ARE USED.  THE REMAINING KNOTS ARE PLACED AT DATA POINTS
C  IF N IS EVEN AND BETWEEN DATA POINTS IF N IS ODD.  THE RIGHTMOST
C  KNOT IS SHIFTED SLIGHTLY TO THE RIGHT TO INSURE PROPER INTERPOLATION
C  AT X(N) (SEE PAGE 350 OF THE REFERENCE).
C  DOUBLE PRECISION VERSION OF BKNOT.
C  --------------------------------------------------------------------
C
C  ------------
C  DECLARATIONS
C  ------------
C
C  PARAMETERS
C
      INTEGER
     *        N, K
      DOUBLE PRECISION
     *     X(N), T(*)
C
C  LOCAL VARIABLES
C
      INTEGER
     *        I, J, IPJ, NPJ, IP1
      DOUBLE PRECISION
     *     RNOT
C
C
C  ----------------------------
C  PUT K KNOTS AT EACH ENDPOINT
C  ----------------------------
C
C     (SHIFT RIGHT ENPOINTS SLIGHTLY -- SEE PG 350 OF REFERENCE)
      RNOT = X(N) + 0.10D0*( X(N)-X(N-1) )
      DO 110 J=1,K
         T(J) = X(1)
         NPJ = N + J
         T(NPJ) = RNOT
  110 CONTINUE
C
C  --------------------------
C  DISTRIBUTE REMAINING KNOTS
C  --------------------------
C
      IF (MOD(K,2) .EQ. 1)  GO TO 150
C
C     CASE OF EVEN K --  KNOTS AT DATA POINTS
C
      I = (K/2) - K
      JSTRT = K+1
      DO 120 J=JSTRT,N
         IPJ = I + J
         T(J) = X(IPJ)
  120 CONTINUE
      GO TO 200
C
C     CASE OF ODD K --  KNOTS BETWEEN DATA POINTS
C
  150 CONTINUE
      I = (K-1)/2 - K
      IP1 = I + 1
      JSTRT = K + 1
      DO 160 J=JSTRT,N
         IPJ = I + J
         T(J) = 0.50D0*( X(IPJ) + X(IPJ+1) )
  160 CONTINUE
  200 CONTINUE
C
      RETURN
      END

      SUBROUTINE DBTPCF(X,N,FCN,LDF,NF,T,K,BCOEF,WORK)
C***BEGIN PROLOGUE  DBTPCF
C***REFER TO  DB2INK,DB3INK
C***ROUTINES CALLED  DBINTK,DBNSLV
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***END PROLOGUE  DBTPCF
C
C  -----------------------------------------------------------------
C  DBTPCF COMPUTES B-SPLINE INTERPOLATION COEFFICIENTS FOR NF SETS
C  OF DATA STORED IN THE COLUMNS OF THE ARRAY FCN. THE B-SPLINE
C  COEFFICIENTS ARE STORED IN THE ROWS OF BCOEF HOWEVER.
C  EACH INTERPOLATION IS BASED ON THE N ABCISSA STORED IN THE
C  ARRAY X, AND THE N+K KNOTS STORED IN THE ARRAY T. THE ORDER
C  OF EACH INTERPOLATION IS K. THE WORK ARRAY MUST BE OF LENGTH
C  AT LEAST 2*K*(N+1).
C  DOUBLE PRECISION VERSION OF BTPCF.
C  -----------------------------------------------------------------
C
C  ------------
C  DECLARATIONS
C  ------------
C
C  PARAMETERS
C
      INTEGER
     *        N, LDF, K
      DOUBLE PRECISION
     *     X(N), FCN(LDF,NF), T(*), BCOEF(NF,N), WORK(*)
C
C  LOCAL VARIABLES
C
      INTEGER
     *        I, J, K1, K2, IQ, IW
C
C  ---------------------------------------------
C  CHECK FOR NULL INPUT AND PARTITION WORK ARRAY
C  ---------------------------------------------
C
C***FIRST EXECUTABLE STATEMENT
      IF (NF .LE. 0)  GO TO 500
      K1 = K - 1
      K2 = K1 + K
      IQ = 1 + N
      IW = IQ + K2*N+1
C
C  -----------------------------
C  COMPUTE B-SPLINE COEFFICIENTS
C  -----------------------------
C
C
C   FIRST DATA SET
C
      CALL DBINTK(X,FCN,T,N,K,WORK,WORK(IQ),WORK(IW))
      DO 20 I=1,N
         BCOEF(1,I) = WORK(I)
   20 CONTINUE
C
C  ALL REMAINING DATA SETS BY BACK-SUBSTITUTION
C
      IF (NF .EQ. 1)  GO TO 500
      DO 100 J=2,NF
         DO 50 I=1,N
            WORK(I) = FCN(I,J)
   50    CONTINUE
         CALL DBNSLV(WORK(IQ),K2,N,K1,K1,WORK)
         DO 60 I=1,N
            BCOEF(J,I) = WORK(I)
   60    CONTINUE
  100 CONTINUE
C
C  ----
C  EXIT
C  ----
C
  500 CONTINUE
      RETURN
      END

      SUBROUTINE DBINTK(X,Y,T,N,K,BCOEF,Q,WORK)
C***BEGIN PROLOGUE  DBINTK
C***DATE WRITTEN   800901   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***CATEGORY NO.  E1A
C***KEYWORDS  B-SPLINE,DATA FITTING,DOUBLE PRECISION,INTERPOLATION,
C             SPLINE
C***AUTHOR  AMOS, D. E., (SNLA)
C***PURPOSE  Produces the B-spline coefficients, BCOEF, of the
C            B-spline of order K with knots T(I), I=1,...,N+K, which
C            takes on the value Y(I) at X(I), I=1,...,N.
C***DESCRIPTION
C
C     Written by Carl de Boor and modified by D. E. Amos
C
C     References
C
C         A Practical Guide to Splines by C. de Boor, Applied
C         Mathematics Series 27, Springer, 1979.
C
C     Abstract    **** a double precision routine ****
C
C         DBINTK is the SPLINT routine of the reference.
C
C         DBINTK produces the B-spline coefficients, BCOEF, of the
C         B-spline of order K with knots T(I), I=1,...,N+K, which
C         takes on the value Y(I) at X(I), I=1,...,N.  The spline or
C         any of its derivatives can be evaluated by calls to DBVALU.
C
C         The I-th equation of the linear system A*BCOEF = B for the
C         coefficients of the interpolant enforces interpolation at
C         X(I), I=1,...,N.  Hence, B(I) = Y(I), for all I, and A is
C         a band matrix with 2K-1 bands if A is invertible.  The matrix
C         A is generated row by row and stored, diagonal by diagonal,
C         in the rows of Q, with the main diagonal going into row K.
C         The banded system is then solved by a call to DBNFAC (which
C         constructs the triangular factorization for A and stores it
C         again in Q), followed by a call to DBNSLV (which then
C         obtains the solution BCOEF by substitution).  DBNFAC does no
C         pivoting, since the total positivity of the matrix A makes
C         this unnecessary.  The linear system to be solved is
C         (theoretically) invertible if and only if
C                 T(I) .LT. X(I) .LT. T(I+K),        for all I.
C         Equality is permitted on the left for I=1 and on the right
C         for I=N when K knots are used at X(1) or X(N).  Otherwise,
C         violation of this condition is certain to lead to an error.
C
C         DBINTK calls DBSPVN, DBNFAC, DBNSLV, XERROR
C
C     Description of Arguments
C
C         Input       X,Y,T are double precision
C           X       - vector of length N containing data point abscissa
C                     in strictly increasing order.
C           Y       - corresponding vector of length N containing data
C                     point ordinates.
C           T       - knot vector of length N+K
C                     Since T(1),..,T(K) .LE. X(1) and T(N+1),..,T(N+K)
C                     .GE. X(N), this leaves only N-K knots (not nec-
C                     essarily X(I) values) interior to (X(1),X(N))
C           N       - number of data points, N .GE. K
C           K       - order of the spline, K .GE. 1
C
C         Output      BCOEF,Q,WORK are double precision
C           BCOEF   - a vector of length N containing the B-spline
C                     coefficients
C           Q       - a work vector of length (2*K-1)*N, containing
C                     the triangular factorization of the coefficient
C                     matrix of the linear system being solved.  The
C                     coefficients for the interpolant of an
C                     additional data set (X(I),yY(I)), I=1,...,N
C                     with the same abscissa can be obtained by loading
C                     YY into BCOEF and then executing
C                         CALL DBNSLV(Q,2K-1,N,K-1,K-1,BCOEF)
C           WORK    - work vector of length 2*K
C
C     Error Conditions
C         Improper input is a fatal error
C         Singular system of equations is a fatal error
C***REFERENCES  C. DE BOOR, *A PRACTICAL GUIDE TO SPLINES*, APPLIED
C                 MATHEMATICS SERIES 27, SPRINGER, 1979.
C               D.E. AMOS, *COMPUTATION WITH SPLINES AND B-SPLINES*,
C                 SAND78-1968,SANDIA LABORATORIES,MARCH,1979.
C***ROUTINES CALLED  DBNFAC,DBNSLV,DBSPVN,XERROR
C***END PROLOGUE  DBINTK
C
C
      INTEGER IFLAG, IWORK, K, N, I, ILP1MX, J, JJ, KM1, KPKM2, LEFT,
     1 LENQ, NP1
      DOUBLE PRECISION BCOEF(N), Y(N), Q(*), T(*), X(N), XI, WORK(*)
C     DIMENSION Q(2*K-1,N), T(N+K)
C***FIRST EXECUTABLE STATEMENT  DBINTK
      IF(K.LT.1) GO TO 100
      IF(N.LT.K) GO TO 105
      JJ = N - 1
      IF(JJ.EQ.0) GO TO 6
      DO 5 I=1,JJ
      IF(X(I).GE.X(I+1)) GO TO 110
    5 CONTINUE
    6 CONTINUE
      NP1 = N + 1
      KM1 = K - 1
      KPKM2 = 2*KM1
      LEFT = K
C                ZERO OUT ALL ENTRIES OF Q
      LENQ = N*(K+KM1)
      DO 10 I=1,LENQ
        Q(I) = 0.0D0
   10 CONTINUE
C
C  ***   LOOP OVER I TO CONSTRUCT THE  N  INTERPOLATION EQUATIONS
      DO 50 I=1,N
        XI = X(I)
        ILP1MX = MIN0(I+K,NP1)
C        *** FIND  LEFT  IN THE CLOSED INTERVAL (I,I+K-1) SUCH THAT
C                T(LEFT) .LE. X(I) .LT. T(LEFT+1)
C        MATRIX IS SINGULAR IF THIS IS NOT POSSIBLE
        LEFT = MAX0(LEFT,I)
        IF (XI.LT.T(LEFT)) GO TO 80
   20   IF (XI.LT.T(LEFT+1)) GO TO 30
        LEFT = LEFT + 1
        IF (LEFT.LT.ILP1MX) GO TO 20
        LEFT = LEFT - 1
        IF (XI.GT.T(LEFT+1)) GO TO 80
C        *** THE I-TH EQUATION ENFORCES INTERPOLATION AT XI, HENCE
C        A(I,J) = B(J,K,T)(XI), ALL J. ONLY THE  K  ENTRIES WITH  J =
C        LEFT-K+1,...,LEFT ACTUALLY MIGHT BE NONZERO. THESE  K  NUMBERS
C        ARE RETURNED, IN  BCOEF (USED FOR TEMP.STORAGE HERE), BY THE
C        FOLLOWING
   30   CALL DBSPVN(T, K, K, 1, XI, LEFT, BCOEF, WORK, IWORK)
C        WE THEREFORE WANT  BCOEF(J) = B(LEFT-K+J)(XI) TO GO INTO
C        A(I,LEFT-K+J), I.E., INTO  Q(I-(LEFT+J)+2*K,(LEFT+J)-K) SINCE
C        A(I+J,J)  IS TO GO INTO  Q(I+K,J), ALL I,J,  IF WE CONSIDER  Q
C        AS A TWO-DIM. ARRAY , WITH  2*K-1  ROWS (SEE COMMENTS IN
C        DBNFAC). IN THE PRESENT PROGRAM, WE TREAT  Q  AS AN EQUIVALENT
C        ONE-DIMENSIONAL ARRAY (BECAUSE OF FORTRAN RESTRICTIONS ON
C        DIMENSION STATEMENTS) . WE THEREFORE WANT  BCOEF(J) TO GO INTO
C        ENTRY
C            I -(LEFT+J) + 2*K + ((LEFT+J) - K-1)*(2*K-1)
C                   =  I-LEFT+1 + (LEFT -K)*(2*K-1) + (2*K-2)*J
C        OF  Q .
        JJ = I - LEFT + 1 + (LEFT-K)*(K+KM1)
        DO 40 J=1,K
          JJ = JJ + KPKM2
          Q(JJ) = BCOEF(J)
   40   CONTINUE
   50 CONTINUE
C
C     ***OBTAIN FACTORIZATION OF  A  , STORED AGAIN IN  Q.
      CALL DBNFAC(Q, K+KM1, N, KM1, KM1, IFLAG)
      GO TO (60, 90), IFLAG
C     *** SOLVE  A*BCOEF = Y  BY BACKSUBSTITUTION
   60 DO 70 I=1,N
        BCOEF(I) = Y(I)
   70 CONTINUE
      CALL DBNSLV(Q, K+KM1, N, KM1, KM1, BCOEF)
      RETURN
C
C
   80 CONTINUE
!      CALL XERROR( ' DBINTK,  SOME ABSCISSA WAS NOT IN THE SUPPORT OF TH
!     1E CORRESPONDING BASIS FUNCTION AND THE SYSTEM IS SINGULAR.',109,2,
!     21)
      RETURN
   90 CONTINUE
!      CALL XERROR( ' DBINTK,  THE SYSTEM OF SOLVER DETECTS A SINGULAR SY
!     1STEM ALTHOUGH THE THEORETICAL CONDITIONS FOR A SOLUTION WERE SATIS
!     2FIED.',123,8,1)
      RETURN
  100 CONTINUE
!      CALL XERROR( ' DBINTK,  K DOES NOT SATISFY K.GE.1', 35, 2, 1)
      RETURN
  105 CONTINUE
!      CALL XERROR( ' DBINTK,  N DOES NOT SATISFY N.GE.K', 35, 2, 1)
      RETURN
  110 CONTINUE
!      CALL XERROR( ' DBINTK,  X(I) DOES NOT SATISFY X(I).LT.X(I+1) FOR S
!     1OME I', 57, 2, 1)
      RETURN
      END

      SUBROUTINE DBNFAC(W,NROWW,NROW,NBANDL,NBANDU,IFLAG)
C***BEGIN PROLOGUE  DBNFAC
C***REFER TO  DBINT4,DBINTK
C
C  DBNFAC is the BANFAC routine from
C        * A Practical Guide to Splines *  by C. de Boor
C
C  DBNFAC is a double precision routine
C
C  Returns in  W  the LU-factorization (without pivoting) of the banded
C  matrix  A  of order  NROW  with  (NBANDL + 1 + NBANDU) bands or diag-
C  onals in the work array  W .
C
C *****  I N P U T  ****** W is double precision
C  W.....Work array of size  (NROWW,NROW)  containing the interesting
C        part of a banded matrix  A , with the diagonals or bands of  A
C        stored in the rows of  W , while columns of  A  correspond to
C        columns of  W . This is the storage mode used in  LINPACK  and
C        results in efficient innermost loops.
C           Explicitly,  A  has  NBANDL  bands below the diagonal
C                            +     1     (main) diagonal
C                            +   NBANDU  bands above the diagonal
C        and thus, with    MIDDLE = NBANDU + 1,
C          A(I+J,J)  is in  W(I+MIDDLE,J)  for I=-NBANDU,...,NBANDL
C                                              J=1,...,NROW .
C        For example, the interesting entries of A (1,2)-banded matrix
C        of order  9  would appear in the first  1+1+2 = 4  rows of  W
C        as follows.
C                          13 24 35 46 57 68 79
C                       12 23 34 45 56 67 78 89
C                    11 22 33 44 55 66 77 88 99
C                    21 32 43 54 65 76 87 98
C
C        All other entries of  W  not identified in this way with an en-
C        try of  A  are never referenced .
C  NROWW.....Row dimension of the work array  W .
C        must be  .GE.  NBANDL + 1 + NBANDU  .
C  NBANDL.....Number of bands of  A  below the main diagonal
C  NBANDU.....Number of bands of  A  above the main diagonal .
C
C *****  O U T P U T  ****** W is double precision
C  IFLAG.....Integer indicating success( = 1) or failure ( = 2) .
C     If  IFLAG = 1, then
C  W.....contains the LU-factorization of  A  into a unit lower triangu-
C        lar matrix  L  and an upper triangular matrix  U (both banded)
C        and stored in customary fashion over the corresponding entries
C        of  A . This makes it possible to solve any particular linear
C        system  A*X = B  for  X  by a
C              CALL DBNSLV ( W, NROWW, NROW, NBANDL, NBANDU, B )
C        with the solution X  contained in  B  on return .
C     If  IFLAG = 2, then
C        one of  NROW-1, NBANDL,NBANDU failed to be nonnegative, or else
C        one of the potential pivots was found to be zero indicating
C        that  A  does not have an LU-factorization. This implies that
C        A  is singular in case it is totally positive .
C
C *****  M E T H O D  ******
C     Gauss elimination  W I T H O U T  pivoting is used. The routine is
C  intended for use with matrices  A  which do not require row inter-
C  changes during factorization, especially for the  T O T A L L Y
C  P O S I T I V E  matrices which occur in spline calculations.
C     The routine should NOT be used for an arbitrary banded matrix.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  DBNFAC
C
      INTEGER IFLAG, NBANDL, NBANDU, NROW, NROWW, I, IPK, J, JMAX, K,
     1 KMAX, MIDDLE, MIDMK, NROWM1
      DOUBLE PRECISION W(NROWW,NROW), FACTOR, PIVOT
C
C***FIRST EXECUTABLE STATEMENT  DBNFAC
      IFLAG = 1
      MIDDLE = NBANDU + 1
C                         W(MIDDLE,.) CONTAINS THE MAIN DIAGONAL OF  A .
      NROWM1 = NROW - 1
      if (NROWM1 .lt. 0) then 
         goto 120
      elseif (NROWM1 .eq. 0) then 
         goto 110
      else
         goto 10
      endif
   10 IF (NBANDL.GT.0) GO TO 30
C                A IS UPPER TRIANGULAR. CHECK THAT DIAGONAL IS NONZERO .
      DO 20 I=1,NROWM1
        IF (W(MIDDLE,I).EQ.0.0D0) GO TO 120
   20 CONTINUE
      GO TO 110
   30 IF (NBANDU.GT.0) GO TO 60
C              A IS LOWER TRIANGULAR. CHECK THAT DIAGONAL IS NONZERO AND
C                 DIVIDE EACH COLUMN BY ITS DIAGONAL .
      DO 50 I=1,NROWM1
        PIVOT = W(MIDDLE,I)
        IF (PIVOT.EQ.0.0D0) GO TO 120
        JMAX = MIN0(NBANDL,NROW-I)
        DO 40 J=1,JMAX
          W(MIDDLE+J,I) = W(MIDDLE+J,I)/PIVOT
   40   CONTINUE
   50 CONTINUE
      RETURN
C
C        A  IS NOT JUST A TRIANGULAR MATRIX. CONSTRUCT LU FACTORIZATION
   60 DO 100 I=1,NROWM1
C                                  W(MIDDLE,I)  IS PIVOT FOR I-TH STEP .
        PIVOT = W(MIDDLE,I)
        IF (PIVOT.EQ.0.0D0) GO TO 120
C                 JMAX  IS THE NUMBER OF (NONZERO) ENTRIES IN COLUMN  I
C                     BELOW THE DIAGONAL .
        JMAX = MIN0(NBANDL,NROW-I)
C              DIVIDE EACH ENTRY IN COLUMN  I  BELOW DIAGONAL BY PIVOT .
        DO 70 J=1,JMAX
          W(MIDDLE+J,I) = W(MIDDLE+J,I)/PIVOT
   70   CONTINUE
C                 KMAX  IS THE NUMBER OF (NONZERO) ENTRIES IN ROW  I  TO
C                     THE RIGHT OF THE DIAGONAL .
        KMAX = MIN0(NBANDU,NROW-I)
C                  SUBTRACT  A(I,I+K)*(I-TH COLUMN) FROM (I+K)-TH COLUMN
C                  (BELOW ROW  I ) .
        DO 90 K=1,KMAX
          IPK = I + K
          MIDMK = MIDDLE - K
          FACTOR = W(MIDMK,IPK)
          DO 80 J=1,JMAX
            W(MIDMK+J,IPK) = W(MIDMK+J,IPK) - W(MIDDLE+J,I)*FACTOR
   80     CONTINUE
   90   CONTINUE
  100 CONTINUE
C                                       CHECK THE LAST DIAGONAL ENTRY .
  110 IF (W(MIDDLE,NROW).NE.0.0D0) RETURN
  120 IFLAG = 2
      RETURN
      END

      SUBROUTINE DBNSLV(W,NROWW,NROW,NBANDL,NBANDU,B)
C***BEGIN PROLOGUE  DBNSLV
C***REFER TO  DBINT4,DBINTK
C
C  DBNSLV is the BANSLV routine from
C        * A Practical Guide to Splines *  by C. de Boor
C
C  DBNSLV is a double precision routine
C
C  Companion routine to  DBNFAC . It returns the solution  X  of the
C  linear system  A*X = B  in place of  B , given the LU-factorization
C  for  A  in the work array  W from DBNFAC.
C
C *****  I N P U T  ****** W,B are DOUBLE PRECISION
C  W, NROWW,NROW,NBANDL,NBANDU.....Describe the LU-factorization of a
C        banded matrix  A  of order  NROW  as constructed in  DBNFAC .
C        For details, see  DBNFAC .
C  B.....Right side of the system to be solved .
C
C *****  O U T P U T  ****** B is DOUBLE PRECISION
C  B.....Contains the solution  X , of order  NROW .
C
C *****  M E T H O D  ******
C     (With  A = L*U, as stored in  W,) the unit lower triangular system
C  L(U*X) = B  is solved for  Y = U*X, and  Y  stored in  B . Then the
C  upper triangular system  U*X = Y  is solved for  X  . The calcul-
C  ations are so arranged that the innermost loops stay within columns.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  DBNSLV
C
      INTEGER NBANDL, NBANDU, NROW, NROWW, I, J, JMAX, MIDDLE, NROWM1
      DOUBLE PRECISION W(NROWW,NROW), B(NROW)
C***FIRST EXECUTABLE STATEMENT  DBNSLV
      MIDDLE = NBANDU + 1
      IF (NROW.EQ.1) GO TO 80
      NROWM1 = NROW - 1
      IF (NBANDL.EQ.0) GO TO 30
C                                 FORWARD PASS
C            FOR I=1,2,...,NROW-1, SUBTRACT  RIGHT SIDE(I)*(I-TH COLUMN
C            OF  L )  FROM RIGHT SIDE  (BELOW I-TH ROW) .
      DO 20 I=1,NROWM1
        JMAX = MIN0(NBANDL,NROW-I)
        DO 10 J=1,JMAX
          B(I+J) = B(I+J) - B(I)*W(MIDDLE+J,I)
   10   CONTINUE
   20 CONTINUE
C                                 BACKWARD PASS
C            FOR I=NROW,NROW-1,...,1, DIVIDE RIGHT SIDE(I) BY I-TH DIAG-
C            ONAL ENTRY OF  U, THEN SUBTRACT  RIGHT SIDE(I)*(I-TH COLUMN
C            OF  U)  FROM RIGHT SIDE  (ABOVE I-TH ROW).
   30 IF (NBANDU.GT.0) GO TO 50
C                                A  IS LOWER TRIANGULAR .
      DO 40 I=1,NROW
        B(I) = B(I)/W(1,I)
   40 CONTINUE
      RETURN
   50 I = NROW
   60 B(I) = B(I)/W(MIDDLE,I)
      JMAX = MIN0(NBANDU,I-1)
      DO 70 J=1,JMAX
        B(I-J) = B(I-J) - B(I)*W(MIDDLE-J,I)
   70 CONTINUE
      I = I - 1
      IF (I.GT.1) GO TO 60
   80 B(1) = B(1)/W(MIDDLE,1)
      RETURN
      END

      SUBROUTINE DBSPVN(T,JHIGH,K,INDEX,X,ILEFT,VNIKX,WORK,IWORK)
C***BEGIN PROLOGUE  DBSPVN
C***DATE WRITTEN   800901   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***CATEGORY NO.  E3,K6
C***KEYWORDS  B-SPLINE,DATA FITTING,DOUBLE PRECISION,INTERPOLATION,
C             SPLINE
C***AUTHOR  AMOS, D. E., (SNLA)
C***PURPOSE  Calculates the value of all (possibly) nonzero basis
C            functions at X.
C***DESCRIPTION
C
C     Written by Carl de Boor and modified by D. E. Amos
C
C     Reference
C         SIAM J. Numerical Analysis, 14, No. 3, June, 1977, pp.441-472.
C
C     Abstract    **** a double precision routine ****
C         DBSPVN is the BSPLVN routine of the reference.
C
C         DBSPVN calculates the value of all (possibly) nonzero basis
C         functions at X of order MAX(JHIGH,(J+1)*(INDEX-1)), where T(K)
C         .LE. X .LE. T(N+1) and J=IWORK is set inside the routine on
C         the first call when INDEX=1.  ILEFT is such that T(ILEFT) .LE.
C         X .LT. T(ILEFT+1).  A call to DINTRV(T,N+1,X,ILO,ILEFT,MFLAG)
C         produces the proper ILEFT.  DBSPVN calculates using the basic
C         algorithm needed in DBSPVD.  If only basis functions are
C         desired, setting JHIGH=K and INDEX=1 can be faster than
C         calling DBSPVD, but extra coding is required for derivatives
C         (INDEX=2) and DBSPVD is set up for this purpose.
C
C         Left limiting values are set up as described in DBSPVD.
C
C     Description of Arguments
C
C         Input      T,X are double precision
C          T       - knot vector of length N+K, where
C                    N = number of B-spline basis functions
C                    N = sum of knot multiplicities-K
C          JHIGH   - order of B-spline, 1 .LE. JHIGH .LE. K
C          K       - highest possible order
C          INDEX   - INDEX = 1 gives basis functions of order JHIGH
C                          = 2 denotes previous entry with work, IWORK
C                              values saved for subsequent calls to
C                              DBSPVN.
C          X       - argument of basis functions,
C                    T(K) .LE. X .LE. T(N+1)
C          ILEFT   - largest integer such that
C                    T(ILEFT) .LE. X .LT.  T(ILEFT+1)
C
C         Output     VNIKX, WORK are double precision
C          VNIKX   - vector of length K for spline values.
C          WORK    - a work vector of length 2*K
C          IWORK   - a work parameter.  Both WORK and IWORK contain
C                    information necessary to continue for INDEX = 2.
C                    When INDEX = 1 exclusively, these are scratch
C                    variables and can be used for other purposes.
C
C     Error Conditions
C         Improper input is a fatal error.
C***REFERENCES  C. DE BOOR, *PACKAGE FOR CALCULATING WITH B-SPLINES*,
C                 SIAM JOURNAL ON NUMERICAL ANALYSIS, VOLUME 14, NO. 3,
C                 JUNE 1977, PP. 441-472.
C***ROUTINES CALLED  XERROR
C***END PROLOGUE  DBSPVN
C
C
      INTEGER ILEFT, IMJP1, INDEX, IPJ, IWORK, JHIGH, JP1, JP1ML, K, L
      DOUBLE PRECISION T, VM, VMPREV, VNIKX, WORK, X
C     DIMENSION T(ILEFT+JHIGH)
      DIMENSION T(*), VNIKX(K), WORK(*)
C     CONTENT OF J, DELTAM, DELTAP IS EXPECTED UNCHANGED BETWEEN CALLS.
C     WORK(I) = DELTAP(I), WORK(K+I) = DELTAM(I), I = 1,K
C***FIRST EXECUTABLE STATEMENT  DBSPVN
      IF(K.LT.1) GO TO 90
      IF(JHIGH.GT.K .OR. JHIGH.LT.1) GO TO 100
      IF(INDEX.LT.1 .OR. INDEX.GT.2) GO TO 105
      IF(X.LT.T(ILEFT) .OR. X.GT.T(ILEFT+1)) GO TO 110
      GO TO (10, 20), INDEX
   10 IWORK = 1
      VNIKX(1) = 1.0D0
      IF (IWORK.GE.JHIGH) GO TO 40
C
   20 IPJ = ILEFT + IWORK
      WORK(IWORK) = T(IPJ) - X
      IMJP1 = ILEFT - IWORK + 1
      WORK(K+IWORK) = X - T(IMJP1)
      VMPREV = 0.0D0
      JP1 = IWORK + 1
      DO 30 L=1,IWORK
        JP1ML = JP1 - L
        VM = VNIKX(L)/(WORK(L)+WORK(K+JP1ML))
        VNIKX(L) = VM*WORK(L) + VMPREV
        VMPREV = VM*WORK(K+JP1ML)
   30 CONTINUE
      VNIKX(JP1) = VMPREV
      IWORK = JP1
      IF (IWORK.LT.JHIGH) GO TO 20
C
   40 RETURN
C
C
   90 CONTINUE
!      CALL XERROR( ' DBSPVN,  K DOES NOT SATISFY K.GE.1', 35, 2, 1)
      RETURN
  100 CONTINUE
!      CALL XERROR( ' DBSPVN,  JHIGH DOES NOT SATISFY 1.LE.JHIGH.LE.K',
!     1 48, 2, 1)
      RETURN
  105 CONTINUE
!      CALL XERROR( ' DBSPVN,  INDEX IS NOT 1 OR 2',29,2,1)
      RETURN
  110 CONTINUE
!      CALL XERROR( ' DBSPVN,  X DOES NOT SATISFY T(ILEFT).LE.X.LE.T(ILEF
!     1T+1)', 56, 2, 1)
      RETURN
      END

      DOUBLE PRECISION FUNCTION DB3VAL(XVAL,YVAL,ZVAL,IDX,IDY,IDZ,
     *  TX,TY,TZ,NX,NY,NZ,KX,KY,KZ,BCOEF,WORK)
C***BEGIN PROLOGUE  DB3VAL
C***DATE WRITTEN   25 MAY 1982
C***REVISION DATE  25 MAY 1982
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***CATEGORY NO.  E1A
C***KEYWORDS  INTERPOLATION, THREE-DIMENSIONS, GRIDDED DATA, SPLINES,
C             PIECEWISE POLYNOMIALS
C***AUTHOR  BOISVERT, RONALD, NBS
C             SCIENTIFIC COMPUTING DIVISION
C             NATIONAL BUREAU OF STANDARDS
C             WASHINGTON, DC 20234
C***PURPOSE  DB3VAL EVALUATES THE PIECEWISE POLYNOMIAL INTERPOLATING
C            FUNCTION CONSTRUCTED BY THE ROUTINE B3INK OR ONE OF ITS
C            PARTIAL DERIVATIVES.
C            DOUBLE PRECISION VERSION OF B3VAL.
C***DESCRIPTION
C
C   DB3VAL  evaluates   the   tensor   product   piecewise   polynomial
C   interpolant constructed  by  the  routine  DB3INK  or  one  of  its
C   derivatives  at  the  point  (XVAL,YVAL,ZVAL).  To   evaluate   the
C   interpolant  itself,  set  IDX=IDY=IDZ=0,  to  evaluate  the  first
C   partial with respect to x, set IDX=1,IDY=IDZ=0, and so on.
C
C   DB3VAL returns 0.0D0 if (XVAL,YVAL,ZVAL) is out of range. That is,
C            XVAL.LT.TX(1) .OR. XVAL.GT.TX(NX+KX) .OR.
C            YVAL.LT.TY(1) .OR. YVAL.GT.TY(NY+KY) .OR.
C            ZVAL.LT.TZ(1) .OR. ZVAL.GT.TZ(NZ+KZ)
C   If the knots TX, TY, and TZ were chosen by  DB3INK,  then  this  is
C   equivalent to
C            XVAL.LT.X(1) .OR. XVAL.GT.X(NX)+EPSX .OR.
C            YVAL.LT.Y(1) .OR. YVAL.GT.Y(NY)+EPSY .OR.
C            ZVAL.LT.Z(1) .OR. ZVAL.GT.Z(NZ)+EPSZ
C   where EPSX = 0.1*(X(NX)-X(NX-1)), EPSY =  0.1*(Y(NY)-Y(NY-1)),  and
C   EPSZ = 0.1*(Z(NZ)-Z(NZ-1)).
C
C   The input quantities TX, TY, TZ, NX, NY, NZ, KX, KY, KZ, and  BCOEF
C   should remain unchanged since the last call of DB3INK.
C
C
C   I N P U T
C   ---------
C
C   XVAL    Double precision scalar
C           X coordinate of evaluation point.
C
C   YVAL    Double precision scalar
C           Y coordinate of evaluation point.
C
C   ZVAL    Double precision scalar
C           Z coordinate of evaluation point.
C
C   IDX     Integer scalar
C           X derivative of piecewise polynomial to evaluate.
C
C   IDY     Integer scalar
C           Y derivative of piecewise polynomial to evaluate.
C
C   IDZ     Integer scalar
C           Z derivative of piecewise polynomial to evaluate.
C
C   TX      Double precision 1D array (size NX+KX)
C           Sequence of knots defining the piecewise polynomial in
C           the x direction.  (Same as in last call to DB3INK.)
C
C   TY      Double precision 1D array (size NY+KY)
C           Sequence of knots defining the piecewise polynomial in
C           the y direction.  (Same as in last call to DB3INK.)
C
C   TZ      Double precision 1D array (size NZ+KZ)
C           Sequence of knots defining the piecewise polynomial in
C           the z direction.  (Same as in last call to DB3INK.)
C
C   NX      Integer scalar
C           The number of interpolation points in x.
C           (Same as in last call to DB3INK.)
C
C   NY      Integer scalar
C           The number of interpolation points in y.
C           (Same as in last call to DB3INK.)
C
C   NZ      Integer scalar
C           The number of interpolation points in z.
C           (Same as in last call to DB3INK.)
C
C   KX      Integer scalar
C           Order of polynomial pieces in x.
C           (Same as in last call to DB3INK.)
C
C   KY      Integer scalar
C           Order of polynomial pieces in y.
C           (Same as in last call to DB3INK.)
C
C   KZ      Integer scalar
C           Order of polynomial pieces in z.
C           (Same as in last call to DB3INK.)
C
C   BCOEF   Double precision 2D array (size NX by NY by NZ)
C           The B-spline coefficients computed by DB3INK.
C
C   WORK    Double precision 1D array (size KY*KZ+3*max(KX,KY,KZ)+KZ)
C           A working storage array.
C
C***REFERENCES  CARL DE BOOR, A PRACTICAL GUIDE TO SPLINES,
C                 SPRINGER-VERLAG, NEW YORK, 1978.
C***ROUTINES CALLED  DINTRV,DBVALU
C***END PROLOGUE  DB3VAL
C
C<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
C
C   MODIFICATION
C   ------------
C
C   ADDED CHECK TO SEE IF X OR Y IS OUT OF RANGE, IF SO, RETURN 0.0
C
C   R.F. BOISVERT, NIST
C   22 FEB 00
C
C<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
C  ------------
C  DECLARATIONS
C  ------------
C
C  PARAMETERS
C
      INTEGER
     *        IDX, IDY, IDZ, NX, NY, NZ, KX, KY, KZ
      DOUBLE PRECISION
     *     XVAL, YVAL, ZVAL, TX(*), TY(*), TZ(*), BCOEF(NX,NY,NZ),
     *     WORK(*)
C
C  LOCAL VARIABLES
C
      INTEGER
     *        ILOY, ILOZ, INBVX, INBV1, INBV2, LEFTY, LEFTZ, MFLAG,
     *        KCOLY, KCOLZ, IZ, IZM1, IW, I, J, K
      DOUBLE PRECISION
     *     DBVALU
C
      DATA ILOY /1/,  ILOZ /1/,  INBVX /1/
C     SAVE ILOY    ,  ILOZ    ,  INBVX
C
C
C***FIRST EXECUTABLE STATEMENT
      DB3VAL = 0.0D0
C  NEXT STATEMENT - RFB MOD
      IF (XVAL.LT.TX(1) .OR. XVAL.GT.TX(NX+KX) .OR.
     +    YVAL.LT.TY(1) .OR. YVAL.GT.TY(NY+KY) .OR.
     +    ZVAL.LT.TZ(1) .OR. ZVAL.GT.TZ(NZ+KZ)) GO TO 100
      CALL DINTRV(TY,NY+KY,YVAL,ILOY,LEFTY,MFLAG)
      IF (MFLAG .NE. 0)  GO TO 100
      CALL DINTRV(TZ,NZ+KZ,ZVAL,ILOZ,LEFTZ,MFLAG)
      IF (MFLAG .NE. 0)  GO TO 100
         IZ = 1 + KY*KZ
         IW = IZ + KZ
         KCOLZ = LEFTZ - KZ
         I = 0
         DO 50 K=1,KZ
            KCOLZ = KCOLZ + 1
            KCOLY = LEFTY - KY
            DO 50 J=1,KY
               I = I + 1
               KCOLY = KCOLY + 1
               WORK(I) = DBVALU(TX,BCOEF(1,KCOLY,KCOLZ),NX,KX,IDX,XVAL,
     *                           INBVX,WORK(IW))
   50    CONTINUE
         INBV1 = 1
         IZM1 = IZ - 1
         KCOLY = LEFTY - KY + 1
         DO 60 K=1,KZ
            I = (K-1)*KY + 1
            J = IZM1 + K
            WORK(J) = DBVALU(TY(KCOLY),WORK(I),KY,KY,IDY,YVAL,
     *                           INBV1,WORK(IW))
  60     CONTINUE
         INBV2 = 1
         KCOLZ = LEFTZ - KZ + 1
         DB3VAL = DBVALU(TZ(KCOLZ),WORK(IZ),KZ,KZ,IDZ,ZVAL,INBV2,
     *                  WORK(IW))
  100 CONTINUE
      RETURN
      END

      SUBROUTINE DB3INK(X,NX,Y,NY,Z,NZ,FCN,LDF1,LDF2,KX,KY,KZ,TX,TY,TZ,
     *  BCOEF,WORK,IFLAG)
C***BEGIN PROLOGUE  DB3INK
C***DATE WRITTEN   25 MAY 1982
C***REVISION DATE  25 MAY 1982
C***REVISION HISTORY  (YYMMDD)
C   000330  Modified array declarations.  (JEC)
C
C***CATEGORY NO.  E1A
C***KEYWORDS  INTERPOLATION, THREE-DIMENSIONS, GRIDDED DATA, SPLINES,
C             PIECEWISE POLYNOMIALS
C***AUTHOR  BOISVERT, RONALD, NBS
C             SCIENTIFIC COMPUTING DIVISION
C             NATIONAL BUREAU OF STANDARDS
C             WASHINGTON, DC 20234
C***PURPOSE  DOUBLE PRECISION VERSION OF DB3INK
C            DB3INK DETERMINES A PIECEWISE POLYNOMIAL FUNCTION THAT
C            INTERPOLATES THREE-DIMENSIONAL GRIDDED DATA. USERS SPECIFY
C            THE POLYNOMIAL ORDER (DEGREE+1) OF THE INTERPOLANT AND
C            (OPTIONALLY) THE KNOT SEQUENCE.
C***DESCRIPTION
C
C   DB3INK determines the parameters of a  function  that  interpolates
C   the three-dimensional gridded data (X(i),Y(j),Z(k),FCN(i,j,k))  for
C   i=1,..,NX, j=1,..,NY, and k=1,..,NZ. The interpolating function and
C   its derivatives may  subsequently  be  evaluated  by  the  function
C   DB3VAL.
C
C   The interpolating  function  is  a  piecewise  polynomial  function
C   represented as a tensor product of one-dimensional  B-splines.  The
C   form of this function is
C
C                      NX   NY   NZ
C        S(x,y,z)  =  SUM  SUM  SUM  a   U (x) V (y) W (z)
C                     i=1  j=1  k=1   ij  i     j     k
C
C   where the functions U(i), V(j), and  W(k)  are  one-dimensional  B-
C   spline basis functions. The coefficients a(i,j) are chosen so that
C
C   S(X(i),Y(j),Z(k)) = FCN(i,j,k)  for i=1,..,NX, j=1,..,NY, k=1,..,NZ
C
C   Note that for fixed values of y  and  z  S(x,y,z)  is  a  piecewise
C   polynomial function of x alone, for fixed values of x and z  S(x,y,
C   z) is a piecewise polynomial function of y  alone,  and  for  fixed
C   values of x and y S(x,y,z)  is  a  function  of  z  alone.  In  one
C   dimension a piecewise polynomial may be created by  partitioning  a
C   given interval into subintervals and defining a distinct polynomial
C   piece on each one. The points where adjacent subintervals meet  are
C   called knots. Each of the functions U(i), V(j), and W(k) above is a
C   piecewise polynomial.
C
C   Users of DB3INK choose  the  order  (degree+1)  of  the  polynomial
C   pieces used to define the piecewise polynomial in each of the x, y,
C   and z directions (KX, KY, and KZ). Users also may define their  own
C   knot sequence in x, y, and z separately (TX, TY, and TZ). If IFLAG=
C   0, however, DB3INK will choose sequences of knots that result in  a
C   piecewise  polynomial  interpolant  with  KX-2  continuous  partial
C   derivatives in x, KY-2 continuous partial derivatives in y, and KZ-
C   2 continuous partial derivatives in z. (KX  knots  are  taken  near
C   each endpoint in x, not-a-knot end conditions  are  used,  and  the
C   remaining knots are placed at data points  if  KX  is  even  or  at
C   midpoints between data points if KX is odd. The y and z  directions
C   are treated similarly.)
C
C   After a call to DB3INK, all information  necessary  to  define  the
C   interpolating function are contained in the parameters NX, NY,  NZ,
C   KX, KY, KZ, TX, TY, TZ, and BCOEF. These quantities should  not  be
C   altered until after the last call of the evaluation routine DB3VAL.
C
C
C   I N P U T
C   ---------
C
C   X       Double precision 1D array (size NX)
C           Array of x abcissae. Must be strictly increasing.
C
C   NX      Integer scalar (.GE. 3)
C           Number of x abcissae.
C
C   Y       Double precision 1D array (size NY)
C           Array of y abcissae. Must be strictly increasing.
C
C   NY      Integer scalar (.GE. 3)
C           Number of y abcissae.
C
C   Z       Double precision 1D array (size NZ)
C           Array of z abcissae. Must be strictly increasing.
C
C   NZ      Integer scalar (.GE. 3)
C           Number of z abcissae.
C
C   FCN     Double precision 3D array (size LDF1 by LDF2 by NY)
C           Array of function values to interpolate. FCN(I,J,K) should
C           contain the function value at the point (X(I),Y(J),Z(K))
C
C   LDF1    Integer scalar (.GE. NX)
C           The actual first dimension of FCN used in the
C           calling program.
C
C   LDF2    Integer scalar (.GE. NY)
C           The actual second dimension of FCN used in the calling
C           program.
C
C   KX      Integer scalar (.GE. 2, .LT. NX)
C           The order of spline pieces in x.
C           (Order = polynomial degree + 1)
C
C   KY      Integer scalar (.GE. 2, .LT. NY)
C           The order of spline pieces in y.
C           (Order = polynomial degree + 1)
C
C   KZ      Integer scalar (.GE. 2, .LT. NZ)
C           The order of spline pieces in z.
C           (Order = polynomial degree + 1)
C
C
C   I N P U T   O R   O U T P U T
C   -----------------------------
C
C   TX      Double precision 1D array (size NX+KX)
C           The knots in the x direction for the spline interpolant.
C           If IFLAG=0 these are chosen by DB3INK.
C           If IFLAG=1 these are specified by the user.
C                      (Must be non-decreasing.)
C
C   TY      Double precision 1D array (size NY+KY)
C           The knots in the y direction for the spline interpolant.
C           If IFLAG=0 these are chosen by DB3INK.
C           If IFLAG=1 these are specified by the user.
C                      (Must be non-decreasing.)
C
C   TZ      Double precision 1D array (size NZ+KZ)
C           The knots in the z direction for the spline interpolant.
C           If IFLAG=0 these are chosen by DB3INK.
C           If IFLAG=1 these are specified by the user.
C                      (Must be non-decreasing.)
C
C
C   O U T P U T
C   -----------
C
C   BCOEF   Double precision 3D array (size NX by NY by NZ)
C           Array of coefficients of the B-spline interpolant.
C           This may be the same array as FCN.
C
C
C   M I S C E L L A N E O U S
C   -------------------------
C
C   WORK    Double precision 1D array (size NX*NY*NZ + max( 2*KX*(NX+1),
C                             2*KY*(NY+1), 2*KZ*(NZ+1) )
C           Array of working storage.
C
C   IFLAG   Integer scalar.
C           On input:  0 == knot sequence chosen by B2INK
C                      1 == knot sequence chosen by user.
C           On output: 1 == successful execution
C                      2 == IFLAG out of range
C                      3 == NX out of range
C                      4 == KX out of range
C                      5 == X not strictly increasing
C                      6 == TX not non-decreasing
C                      7 == NY out of range
C                      8 == KY out of range
C                      9 == Y not strictly increasing
C                     10 == TY not non-decreasing
C                     11 == NZ out of range
C                     12 == KZ out of range
C                     13 == Z not strictly increasing
C                     14 == TY not non-decreasing
C
C***REFERENCES  CARL DE BOOR, A PRACTICAL GUIDE TO SPLINES,
C                 SPRINGER-VERLAG, NEW YORK, 1978.
C               CARL DE BOOR, EFFICIENT COMPUTER MANIPULATION OF TENSOR
C                 PRODUCTS, ACM TRANSACTIONS ON MATHEMATICAL SOFTWARE,
C                 VOL. 5 (1979), PP. 173-182.
C***ROUTINES CALLED  DBTPCF,DBKNOT
C***END PROLOGUE  DB3INK
C
C  ------------
C  DECLARATIONS
C  ------------
C
C  PARAMETERS
C
      INTEGER
     *        NX, NY, NZ, LDF1, LDF2, KX, KY, KZ, IFLAG
      DOUBLE PRECISION
     *     X(NX), Y(NY), Z(NZ), FCN(LDF1,LDF2,NZ), TX(*), TY(*), TZ(*),
     *     BCOEF(NX,NY,NZ), WORK(*)
C
C  LOCAL VARIABLES
C
      INTEGER
     *        I, J, LOC, IW, NPK
C
C  -----------------------
C  CHECK VALIDITY OF INPUT
C  -----------------------
C
C***FIRST EXECUTABLE STATEMENT
      IF ((IFLAG .LT. 0) .OR. (IFLAG .GT. 1))  GO TO 920
      IF (NX .LT. 3)  GO TO 930
      IF (NY .LT. 3)  GO TO 970
      IF (NZ .LT. 3)  GO TO 1010
      IF ((KX .LT. 2) .OR. (KX .GE. NX))  GO TO 940
      IF ((KY .LT. 2) .OR. (KY .GE. NY))  GO TO 980
      IF ((KZ .LT. 2) .OR. (KZ .GE. NZ))  GO TO 1020
      DO 10 I=2,NX
         IF (X(I) .LE. X(I-1))  GO TO 950
   10 CONTINUE
      DO 20 I=2,NY
         IF (Y(I) .LE. Y(I-1))  GO TO 990
   20 CONTINUE
      DO 30 I=2,NZ
         IF (Z(I) .LE. Z(I-1))  GO TO 1030
   30 CONTINUE
      IF (IFLAG .EQ. 0)  GO TO 70
         NPK = NX + KX
         DO 40 I=2,NPK
            IF (TX(I) .LT. TX(I-1))  GO TO 960
   40    CONTINUE
         NPK = NY + KY
         DO 50 I=2,NPK
            IF (TY(I) .LT. TY(I-1))  GO TO 1000
   50    CONTINUE
         NPK = NZ + KZ
         DO 60 I=2,NPK
            IF (TZ(I) .LT. TZ(I-1))  GO TO 1040
   60    CONTINUE
   70 CONTINUE
C
C  ------------
C  CHOOSE KNOTS
C  ------------
C
      IF (IFLAG .NE. 0)  GO TO 100
         CALL DBKNOT(X,NX,KX,TX)
         CALL DBKNOT(Y,NY,KY,TY)
         CALL DBKNOT(Z,NZ,KZ,TZ)
  100 CONTINUE
C
C  -------------------------------
C  CONSTRUCT B-SPLINE COEFFICIENTS
C  -------------------------------
C
      IFLAG = 1
      IW = NX*NY*NZ + 1
C
C     COPY FCN TO WORK IN PACKED FOR DBTPCF
      LOC = 0
      DO 510 K=1,NZ
         DO 510 J=1,NY
            DO 510 I=1,NX
               LOC = LOC + 1
               WORK(LOC) = FCN(I,J,K)
  510 CONTINUE
C
      CALL DBTPCF(X,NX,WORK,NX,NY*NZ,TX,KX,BCOEF,WORK(IW))
      CALL DBTPCF(Y,NY,BCOEF,NY,NX*NZ,TY,KY,WORK,WORK(IW))
      CALL DBTPCF(Z,NZ,WORK,NZ,NX*NY,TZ,KZ,BCOEF,WORK(IW))
      GO TO 9999
C
C  -----
C  EXITS
C  -----
C
  920 CONTINUE
!      CALL XERRWV('DB3INK -  IFLAG=I1 IS OUT OF RANGE.',
!     *            35,2,1,1,IFLAG,I2,0,R1,R2)
      IFLAG = 2
      GO TO 9999
C
  930 CONTINUE
      IFLAG = 3
!      CALL XERRWV('DB3INK -  NX=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,NX,I2,0,R1,R2)
      GO TO 9999
C
  940 CONTINUE
      IFLAG = 4
!      CALL XERRWV('DB3INK -  KX=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,KX,I2,0,R1,R2)
      GO TO 9999
C
  950 CONTINUE
      IFLAG = 5
!      CALL XERRWV('DB3INK -  X ARRAY MUST BE STRICTLY INCREASING.',
!     *            46,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
  960 CONTINUE
      IFLAG = 6
!      CALL XERRWV('DB3INK -  TX ARRAY MUST BE NON-DECREASING.',
!     *            42,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
  970 CONTINUE
      IFLAG = 7
!      CALL XERRWV('DB3INK -  NY=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,NY,I2,0,R1,R2)
      GO TO 9999
C
  980 CONTINUE
      IFLAG = 8
!      CALL XERRWV('DB3INK -  KY=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,KY,I2,0,R1,R2)
      GO TO 9999
C
  990 CONTINUE
      IFLAG = 9
!      CALL XERRWV('DB3INK -  Y ARRAY MUST BE STRICTLY INCREASING.',
!     *            46,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
 1000 CONTINUE
      IFLAG = 10
!      CALL XERRWV('DB3INK -  TY ARRAY MUST BE NON-DECREASING.',
!     *            42,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
 1010 CONTINUE
      IFLAG = 11
!      CALL XERRWV('DB3INK -  NZ=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,NZ,I2,0,R1,R2)
      GO TO 9999
C
 1020 CONTINUE
      IFLAG = 12
!      CALL XERRWV('DB3INK -  KZ=I1 IS OUT OF RANGE.',
!     *            32,IFLAG,1,1,KZ,I2,0,R1,R2)
      GO TO 9999
C
 1030 CONTINUE
      IFLAG = 13
!      CALL XERRWV('DB3INK -  Z ARRAY MUST BE STRICTLY INCREASING.',
!     *            46,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
 1040 CONTINUE
      IFLAG = 14
!      CALL XERRWV('DB3INK -  TZ ARRAY MUST BE NON-DECREASING.',
!     *            42,IFLAG,1,0,I1,I2,0,R1,R2)
      GO TO 9999
C
 9999 CONTINUE
      RETURN
      END

