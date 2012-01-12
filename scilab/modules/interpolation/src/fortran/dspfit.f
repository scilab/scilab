*
*   This code comes from the NSWC fortran library with slight
*   modifications from Bruno Pincon 
*
      SUBROUTINE SPFIT (X, Y, WGT, M, BREAK, L, Z, A, WK, IERR)
      implicit none
      integer M, L, IERR
      DOUBLE PRECISION X(M), Y(M), WGT(M), BREAK(L)
      DOUBLE PRECISION Z(*), A(*), WK(*)
C-----------------------------------------------------------------------
C            WEIGHTED LEAST SQUARES CUBIC SPLINE FITTING
C-----------------------------------------------------------------------
      integer N, J, K, LA, LW, LQ, LM1
      DOUBLE PRECISION TEMP(20), DX, B, C
C---------------------
      N = L + 2
C
C                DEFINE THE KNOTS FOR THE B-SPLINES
C
      WK(1) = BREAK(1)
      WK(2) = BREAK(1)
      WK(3) = BREAK(1)
      WK(4) = BREAK(1)
      do J = 2,L               ! the conditions break(k) < break(k+1) are
         WK(J + 3) = BREAK(J)  ! verified in the interface
      enddo
      WK(L + 4) = BREAK(L)
      WK(L + 5) = BREAK(L)
      WK(L + 6) = BREAK(L)
C
C     OBTAIN THE B-SPLINE COEFFICIENTS OF THE LEAST SQUARES FIT
C
      LA = N + 5     ! start indices (in wk) for the others working area
      LW = LA + N    !
      LQ = LW + N    !
      CALL BSLSQ (X, Y, WGT, M, WK(1), N, 4, WK(LA),
     *            WK(LW), WK(LQ), IERR)
*
*     pour BSLSQ : IERR =-1  not enough points for the fit
*                  IERR = 0  OK 
*                  IERR = 1  non uniqness of the solution (but a solution is computed)
*
      IF (IERR .GE. 0) then
C         OBTAIN THE COEFFICIENTS OF THE FIT IN TAYLOR SERIES FORM
         CALL BSPP (WK(1), WK(LA), N, 4, BREAK,
     *              WK(LQ), LM1, TEMP)
         K = LQ
         DO J = 1,LM1
            Z(J) = WK(K)
            A(J) = WK(K + 1)
            K = K + 4
         enddo

         ! a trick to get the spline value (Z(L)) and first derivative
         ! (A(L)) on the last breakpoint : the last polynomial piece
         ! has the form  Z(LM1) + A(LM1)(x- break(l-1)) + B(LM1)(...)
         DX = BREAK(L) - BREAK(L-1)
         B = WK(LQ + 4*(L-2) + 2)
         C = WK(LQ + 4*(L-2) + 3)
         Z(L) = Z(LM1) + DX*(A(LM1) + DX*(B + DX*C))
         A(L) = A(LM1) + DX*( 2.d0*B + DX*3.d0*C)
      endif
      END


      SUBROUTINE BSLSQ (TAU, GTAU, WGT, NTAU, T, N, K, A, WK, Q, IERR)
      implicit none
      integer NTAU, K, N, IERR
      DOUBLE PRECISION TAU(NTAU), GTAU(NTAU), WGT(NTAU)
      DOUBLE PRECISION T(*), A(N), WK(N), Q(K,N)
C-----------------------------------------------------------------------
C
C        BSLSQ PRODUCES THE B-SPLINE COEFFICIENTS OF A PIECEWISE
C              POLYNOMIAL P(X) OF ORDER K WHICH MINIMIZES
C
C                SUM (WGT(J)*(P(TAU(J)) - GTAU(J))**2).
C
C
C     INPUT ...
C
C       TAU   ARRAY OF LENGTH NTAU CONTAINING DATA POINT ABSCISSAE.
C       GTAU  ARRAY OF LENGTH NTAU CONTAINING DATA POINT ORDINATES.
C       WGT   ARRAY OF LENGTH NTAU CONTAINING THE WEIGHTS.
C       NTAU  NUMBER OF DATA POINTS TO BE FITTED.
C       T     KNOT SEQUENCE OF LENGTH N + K.
C       N     DIMENSION OF THE PIECEWISE POLYNOMIAL SPACE.
C       K     ORDER OF THE B-SPLINES.
C
C     OUTPUT ...
C
C       A     ARRAY OF LENGTH N CONTAINING THE B-SPLINE COEFFICIENTS
C             OF THE L2 APPROXIMATION.
C
C       IERR  INTEGER REPORTING THE STATUS OF THE RESULTS ...
C
C             0  THE COEFFICIENT MATRIX IS NONSIGULAR. THE
C                UNIQUE LEAST SQUARES SOLUTION WAS OBTAINED.
C             1  THE COEFFICIENT MATRIX IS SINGULAR. A
C                LEAST SQUARES SOLUTION WAS OBTAINED.
C            -1  INPUT ERRORS WERE DETECTED.
C
C-----------------------------------------------------------------------
C
      integer I, J, JJ, L, LEFT, LEFTMK, MM, ntau_count
      double precision dw
      external isearch
      integer  isearch

*  some modifs :
*    1/ to avoid the sort on the data points use a dicho search to get
*       the interval LEFT
*    2/ all the data points outside the interval definition of the spline 
*       ([T(K),T(N+1)]) or with a non positive weight are not taken into acount
*       in the fit
*
*
*   Note: the breakpoints goes to T(K) until T(N+1)  (N+1-K+1 points)
*       T(K) is the first break point  (T(K) = X(1), ..., T(I) = X(I-K+1)
*       T(N+1) = T(L+K-1) = X(L)  is the last break point 
C
      do J = 1,N
         A(J) = 0.D0
         do  I = 1,K
            Q(I,J) = 0.d0
         enddo
      enddo
C
      ntau_count = 0
      LEFT = K
      DO  L = 1,NTAU
         if (      TAU(L).ge.T(K) .and. TAU(L).le.T(N+1)    ! added by Bruno 
     $       .and. WGT(L) .gt. 0.d0 ) then
            ntau_count = ntau_count + 1
*           find the index left such that  T(LEFT) <= TAU(L) <= T(LEFT+1) (modified by Bruno)
            LEFT = isearch(TAU(L), T(K), N-K+2) + 3
            JJ = 0
            CALL BSPVB (T, K, K, JJ, TAU(L), LEFT, WK)

            LEFTMK = LEFT - K
            DO MM = 1,K
               DW = WK(MM)*WGT(L)
               J = LEFTMK + MM
               A(J) = DW*GTAU(L) + A(J)
               I = 1
               DO JJ = MM,K
                  Q(I,J) = WK(JJ)*DW + Q(I,J)
                  I = I + 1
               enddo
            enddo
         endif
      enddo

      IF (ntau_count .ge. MAX(2,K)) then
C        SOLVE THE NORMAL EQUATIONS
         CALL BCHFAC (Q, K, N, WK, IERR)
         CALL BCHSLV (Q, K, N, A)
      else
         ierr = -1
      endif
      end


      SUBROUTINE BCHFAC (W, NB, N, DIAG, IFLAG)
      implicit none
      integer NB, N, IFLAG
      DOUBLE PRECISION W(NB,N), DIAG(N)

C-----------------------------------------------------------------------
C  FROM  * A PRACTICAL GUIDE TO SPLINES *  BY C. DE BOOR
C  CONSTRUCTS CHOLESKY FACTORIZATION
C                     C  =  L * D * L-TRANSPOSE
C  WITH L UNIT LOWER TRIANGULAR AND D DIAGONAL, FOR GIVEN MATRIX C OF
C  ORDER  N , IN CASE  C  IS (SYMMETRIC) POSITIVE SEMIDEFINITE
C  AND BANDED, HAVING NB DIAGONALS AT AND BELOW THE MAIN DIAGONAL.
C
C******  INPUT  ******
C
C     N      THE ORDER OF THE MATRIX C.
C
C     NB     THE BANDWIDTH OF C, I.E.,
C               C(I,J) = 0 FOR ABS(I-J) .GT. NB .
C
C     W      WORK ARRAY OF SIZE NB BY N CONTAINING THE NB DIAGONALS
C            IN ITS ROWS, WITH THE MAIN DIAGONAL IN ROW 1. PRECISELY,
C            W(I,J)  CONTAINS  C(I+J-1,J), I=1,...,NB, J=1,...,N.
C            FOR EXAMPLE, THE INTERESTING ENTRIES OF A SEVEN DIAGONAL
C            SYMMETRIC MATRIX C OF ORDER 9 WOULD BE STORED IN W AS
C
C                       11 22 33 44 55 66 77 88 99
C                       21 32 43 54 65 76 87 98
C                       31 42 53 64 75 86 97
C                       41 52 63 74 85 96
C
C            ALL OTHER ENTRIES OF W NOT IDENTIFIED WITH AN ENTRY OF C
C            ARE NEVER REFERENCED.
C
C     DIAG   WORK ARRAY OF LENGTH N.
C
C******  O U T P U T  ******
C                                                         T
C     W      CONTAINS THE CHOLESKY FACTORIZATION C = L*D*L   WHERE
C            W(1,I) = 1/D(I,I) AND W(I,J) = L(I-1+J,J) (I=2,...,NB).
C
C     IFLAG  0 IF C IS NONSINGULAR AND 1 IF C IS SINGULAR.
C
C******  M E T H O D  ******
C
C   GAUSS ELIMINATION, ADAPTED TO THE SYMMETRY AND BANDEDNESS OF  C , IS
C   USED .
C     NEAR ZERO PIVOTS ARE HANDLED IN A SPECIAL WAY. THE DIAGONAL ELE-
C  MENT C(K,K) = W(1,K) IS SAVED INITIALLY IN  DIAG(K), ALL K. AT THE K-
C  TH ELIMINATION STEP, THE CURRENT PIVOT ELEMENT, VIZ.  W(1,K), IS COM-
C  PARED WITH ITS ORIGINAL VALUE, DIAG(K). IF, AS THE RESULT OF PRIOR
C  ELIMINATION STEPS, THIS ELEMENT HAS BEEN REDUCED BY ABOUT A WORD
C  LENGTH, (I.E., IF W(1,K)+DIAG(K) .LE. DIAG(K)), THEN THE PIVOT IS DE-
C  CLARED TO BE ZERO, AND THE ENTIRE K-TH ROW IS DECLARED TO BE LINEARLY
C  DEPENDENT ON THE PRECEDING ROWS. THIS HAS THE EFFECT OF PRODUCING
C   X(K) = 0  WHEN SOLVING  C*X = B  FOR  X, REGARDLESS OF  B. JUSTIFIC-
C  ATION FOR THIS IS AS FOLLOWS. IN CONTEMPLATED APPLICATIONS OF THIS
C  PROGRAM, THE GIVEN EQUATIONS ARE THE NORMAL EQUATIONS FOR SOME LEAST-
C  SQUARES APPROXIMATION PROBLEM, DIAG(K) = C(K,K) GIVES THE NORM-SQUARE
C  OF THE K-TH BASIS FUNCTION, AND, AT THIS POINT,  W(1,K)  CONTAINS THE
C  NORM-SQUARE OF THE ERROR IN THE LEAST-SQUARES APPROXIMATION TO THE K-
C  TH BASIS FUNCTION BY LINEAR COMBINATIONS OF THE FIRST K-1 . HAVING
C  W(1,K)+DIAG(K) .LE. DIAG(K) SIGNIFIES THAT THE K-TH FUNCTION IS LIN-
C  EARLY DEPENDENT TO MACHINE ACCURACY ON THE FIRST K-1 FUNCTIONS, THERE
C  FORE CAN SAFELY BE LEFT OUT FROM THE BASIS OF APPROXIMATING FUNCTIONS
C     THE SOLUTION OF A LINEAR SYSTEM
C                       C*X = B
C   IS EFFECTED BY THE SUCCESSION OF THE FOLLOWING  T W O  CALLS ...
C     CALL BCHFAC (W, NB, N, DIAG, IFLAG)   , TO GET FACTORIZATION
C     CALL BCHSLV (W, NB, N, B, X )            , TO SOLVE FOR X.
C-----------------------------------------------------------------------
C
      integer I, J, K, IMAX, JMAX, KPI, IPJ
      double precision T, RATIO

      IF (N .GT. 1) GO TO 10
         IFLAG = 1
         IF (W(1,1) .EQ. 0.D0) RETURN
         IFLAG = 0
         W(1,1) = 1.D0/W(1,1)
         RETURN
C
C     STORE THE DIAGONAL OF C IN DIAG
C
   10 DO 11 K = 1,N
         DIAG(K) = W(1,K)
   11 CONTINUE
C
C     FACTORIZATION
C
      IFLAG = 0
      DO 60 K = 1,N
         T = W(1,K) + DIAG(K)
         IF (T .NE. DIAG(K)) GO TO 30
            IFLAG = 1
            DO 20 J = 1,NB
               W(J,K) = 0.D0
   20       CONTINUE
            GO TO 60
C
   30    T = 1.D0/W(1,K)
         W(1,K) = T
         IMAX = MIN(NB - 1,N - K)
         IF (IMAX .LT. 1) GO TO 60
         JMAX = IMAX
         DO 50 I = 1,IMAX
            RATIO = T*W(I+1,K)
            KPI = K + I
            DO 40 J = 1,JMAX
               IPJ = I + J
               W(J,KPI) = W(J,KPI) - W(IPJ,K)*RATIO
   40       CONTINUE
            JMAX = JMAX - 1
            W(I+1,K) = RATIO
   50    CONTINUE
   60 CONTINUE
      RETURN
      END

      SUBROUTINE BCHSLV (W, NB, N, B)
      implicit none
      integer NB, N
      DOUBLE PRECISION W(NB,N), B(N)
C-----------------------------------------------------------------------
C
C     BCHSLV SOLVES THE LINEAR SYSTEM C*X = B FOR X WHEN W CONTAINS
C     THE CHOLESKY FACTORIZATION OBTAINED BY THE SUBROUTINE BCHFAC
C     FOR THE BANDED SYMMETRIC POSITIVE DEFINITE MATRIX C.
C
C     INPUT ...
C
C        N   THE ORDER OF THE MATRIX C
C        NB  THE BANDWIDTH OF C
C        W   THE CHOLESKY FACTORIZATION OF C
C        B   VECTOR OF LENGTH N CONTAINING THE RIGHT SIDE
C
C     OUTPUT ...
C
C        B   SOLUTION X OF THE LINEAR SYSTEM C*X = B
C
C                                       T
C     NOTE.  THE FACTORIZATION C = L*D*L  IS USED, WHERE L IS A
C     UNIT LOWER TRIANGULAR MATRIX AND D A DIAGONAL MATRIX.
C
C-----------------------------------------------------------------------
C
      integer J, NBM1, K, JMAX, JPK

      IF (N .GT. 1) GO TO 10
      B(1) = B(1)*W(1,1)
      RETURN
C
C     FORWARD SUBSTITUTION. SOLVE L*Y = B FOR Y AND STORE Y IN B.
C
   10 NBM1 = NB - 1
      DO 30 K = 1,N
         JMAX = MIN(NBM1,N - K)
         IF (JMAX .LT. 1) GO TO 30
         DO 20 J = 1,JMAX
            JPK = J + K
            B(JPK) = B(JPK) - W(J + 1,K)*B(K)
   20    CONTINUE
   30 CONTINUE
C                              T     -1
C     BACKSUBSTITUTION. SOLVE L X = D  Y  FOR X AND STORE X IN B.
C
      K = N
   40    B(K) = B(K)*W(1,K)
         JMAX = MIN(NBM1,N - K)
         IF (JMAX .LT. 1) GO TO 60
         DO 50 J = 1,JMAX
            JPK = J + K
            B(K) = B(K) - W(J + 1,K)*B(JPK)
   50    CONTINUE
   60    K = K - 1
         IF (K .GT. 0) GO TO 40
      RETURN
      END

      SUBROUTINE BSPVB (T, K, JHIGH, J, X, LEFT, BLIST)
      implicit none
      integer K, JHIGH, J, LEFT
      DOUBLE PRECISION T(*), X, BLIST(K)
C-----------------------------------------------------------------------
C
C     BSPVB CALCULATES THE VALUE OF ALL POSSIBLY NONZERO B-SPLINES
C     AT X OF ORDER MAX(JHIGH,J + 1) WHERE T(K) .LE. X .LT. T(N+1).
C
C     DESCRIPTION OF ARGUMENTS
C
C         INPUT
C
C          T       - KNOT VECTOR OF LENGTH N + K.
C          K       - HIGHEST POSSIBLE ORDER OF THE B-SPLINES.
C          JHIGH   - ORDER OF B-SPLINES (1 .LE. JHIGH .LE. K).
C          J       - J .LE. 0  GIVES B-SPLINES OF ORDER JHIGH.
C                    J .GE. 1  ON A PREVIOUS CALL TO BSPVB THE
C                              B-SPLINES OF ORDER J WERE COM-
C                              PUTED AND STORED IN BLIST. IT IS
C                              ASSUMED THAT WORK HAS NOT BEEN
C                              MODIFIED AND THAT J .LT. K.
C          X       - ARGUMENT OF THE B-SPLINES.
C          LEFT    - LARGEST INTEGER SUCH THAT
C                    T(LEFT) .LE. X .LT. T(LEFT+1)
C
C         OUTPUT
C
C          BLIST   - VECTOR OF LENGTH K FOR SPLINE VALUES.
C          J       - B-SPLINES OF ORDER J HAVE BEEN COMPUTED
C                    AND STORED IN BLIST.
C
C-----------------------------------------------------------------------
C     WRITTEN BY CARL DE BOOR (UNIVERSITY OF WISCONSIN) AND MODIFIED
C         BY A.H. MORRIS (NSWC).
C-----------------------------------------------------------------------
C
      integer I, IMJ, L
      double precision S, TIMJ, TI, TERM

      IF (J .GT. 0) GO TO 10
         J = 1
         BLIST(1) = 1.D0
         IF (J .GE. JHIGH) RETURN
C
   10 S = 0.D0
      DO 20 L = 1,J
         I = LEFT + L
         IMJ = I - J
         TIMJ = T(IMJ)
         TI = T(I)
         TERM = BLIST(L)/(TI - TIMJ)
         BLIST(L) = S + (TI - X)*TERM
         S = (X - TIMJ)*TERM
   20 CONTINUE
      J = J + 1
      BLIST(J) = S
      IF (J .LT. JHIGH) GO TO 10
C
      RETURN
      END

      SUBROUTINE BSPP (T, A, N, K, BREAK, C, L, WK)
      implicit none
      integer N, K, L
      DOUBLE PRECISION T(*), A(N), BREAK(*), C(K,*), WK(K,*)
C-----------------------------------------------------------------------
C
C              CONVERSION FROM B-SPLINE REPRESENTATION
C              TO PIECEWISE POLYNOMIAL REPRESENTATION
C
C
C     INPUT ...
C
C       T     KNOT SEQUENCE OF LENGTH N+K
C       A     B-SPLINE COEFFICIENT SEQUENCE OF LENGTH N
C       N     LENGTH OF A
C       K     ORDER OF THE B-SPLINES
C
C     OUTPUT ...
C
C       BREAK BREAKPOINT SEQUENCE, OF LENGTH L+1, CONTAINING
C             (IN INCREASING ORDER) THE DISTINCT POINTS OF THE
C             SEQUENCE T(K),...,T(N+1).
C       C     KXL MATRIX WHERE C(I,J) = (I-1)ST RIGHT DERIVATIVE
C             OF THE PP AT BREAK(J) DIVIDED BY FACTORIAL(I-1).
C       L     NUMBER OF POLYNOMIALS WHICH FORM THE PP
C
C     WORK AREA ...
C
C       WK    2-DIMENSIONAL ARRAY OF DIMENSION (K,K+1)
C
C-----------------------------------------------------------------------
C
      integer I, J, KM1, KP1, LEFT, JJ, JP1, KMJ, IL, ILJ, ILKJ
      double precision TERM, DIFF, R, S, X

      L = 0
      BREAK(1) = T(K)
      IF (K .EQ. 1) GO TO 100
      KM1 = K - 1
      KP1 = K + 1
C
C          GENERAL K-TH ORDER CASE
C
      DO 60 LEFT = K,N
         IF (T(LEFT) .EQ. T(LEFT + 1)) GO TO 60
         L = L + 1
         BREAK(L + 1) = T(LEFT + 1)
         DO 10 J = 1,K
            JJ = LEFT - K + J
            WK(J,1) = A(JJ)
   10    CONTINUE
C
         DO 21 J = 1,KM1
            JP1 = J + 1
            KMJ = K - J
            DO 20 I = 1,KMJ
               IL = I + LEFT
               ILKJ = IL - KMJ
               DIFF = T(IL) - T(ILKJ)
               WK(I,JP1) = (WK(I+1,J) - WK(I,J))/DIFF
   20       CONTINUE
   21    CONTINUE
C
         WK(1,KP1) = 1.D0
         X = T(LEFT)
         C(K,L) = WK(1,K)
         R = 1.D0
         DO 50 J = 1,KM1
            JP1 = J + 1
            S = 0.D0
            DO 30 I = 1,J
               IL = I + LEFT
               ILJ = IL - J
               TERM = WK(I,KP1)/(T(IL) - T(ILJ))
               WK(I,KP1) = S + (T(IL) - X)*TERM
               S = (X - T(ILJ))*TERM
   30       CONTINUE
            WK(JP1,KP1) = S
C
            S = 0.D0
            KMJ = K - J
            DO 40 I = 1,JP1
               S = S + WK(I,KMJ)*WK(I,KP1)
   40       CONTINUE
            R = (R*DBLE(KMJ))/DBLE(J)
            C(KMJ,L) = R*S
   50    CONTINUE
   60 CONTINUE
      RETURN
C
C          PIECEWISE CONSTANT CASE
C
  100 DO 110 LEFT = K,N
         IF (T(LEFT) .EQ. T(LEFT + 1)) GO TO 110
         L = L + 1
         BREAK(L + 1) = T(LEFT + 1)
         C(1,L) = A(LEFT)
  110 CONTINUE
      RETURN
      END
