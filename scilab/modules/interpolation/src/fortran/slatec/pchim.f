      SUBROUTINE DPCHIM(N,X,F,D,INCFD)
*
*   This subroutine comes from the slatec / cmlib and has
*   been slightly modified for inclusion in scilab : all
*   the error treatment has been removed (and so the 
*   parameter IERR) because this is done by the scilab
*   interface.
*
C***BEGIN PROLOGUE  DPCHIM
C***DATE WRITTEN   811103   (YYMMDD)
C***REVISION DATE  870707   (YYMMDD)
C***CATEGORY NO.  E1B
C***KEYWORDS  LIBRARY=SLATEC(PCHIP),
C             TYPE=DOUBLE PRECISION(PCHIM-S DPCHIM-D),
C             CUBIC HERMITE INTERPOLATION,MONOTONE INTERPOLATION,
C             PIECEWISE CUBIC INTERPOLATION
C***AUTHOR  FRITSCH, F. N., (LLNL)
C             MATHEMATICS AND STATISTICS DIVISION
C             LAWRENCE LIVERMORE NATIONAL LABORATORY
C             P.O. BOX 808  (L-316)
C             LIVERMORE, CA  94550
C             FTS 532-4275, (415) 422-4275
C***PURPOSE  Set derivatives needed to determine a monotone piecewise
C            cubic Hermite interpolant to given data.  Boundary values
C            are provided which are compatible with monotonicity.  The
C            interpolant will have an extremum at each point where mono-
C            tonicity switches direction.  (See DPCHIC if user control
C            is desired over boundary or switch conditions.)
C***DESCRIPTION
C
C       **** Double Precision version of PCHIM ****
C
C          DPCHIM:  Piecewise Cubic Hermite Interpolation to
C                  Monotone data.
C
C     Sets derivatives needed to determine a monotone piecewise cubic
C     Hermite interpolant to the data given in X and F.
C
C     Default boundary conditions are provided which are compatible
C     with monotonicity.  (See DPCHIC if user control of boundary con-
C     ditions is desired.)
C
C     If the data are only piecewise monotonic, the interpolant will
C     have an extremum at each point where monotonicity switches direc-
C     tion.  (See DPCHIC if user control is desired in such cases.)
C
C     To facilitate two-dimensional applications, includes an increment
C     between successive values of the F- and D-arrays.
C
C     The resulting piecewise cubic Hermite function may be evaluated
C     by DPCHFE or DPCHFD.
C
C ----------------------------------------------------------------------
C
C  Calling sequence:
C
C        PARAMETER  (INCFD = ...)
C        INTEGER  N, IERR
C        DOUBLE PRECISION  X(N), F(INCFD,N), D(INCFD,N)
C
C        CALL  DPCHIM (N, X, F, D, INCFD, IERR)
C
C   Parameters:
C
C     N -- (input) number of data points.  (Error return if N.LT.2 .)
C           If N=2, simply does linear interpolation.
C
C     X -- (input) real*8 array of independent variable values.  The
C           elements of X must be strictly increasing:
C                X(I-1) .LT. X(I),  I = 2(1)N.
C           (Error return if not.)
C
C     F -- (input) real*8 array of dependent variable values to be
C           interpolated.  F(1+(I-1)*INCFD) is value corresponding to
C           X(I).  DPCHIM is designed for monotonic data, but it will
C           work for any F-array.  It will force extrema at points where
C           monotonicity switches direction.  If some other treatment of
C           switch points is desired, DPCHIC should be used instead.
C                                     -----
C     D -- (output) real*8 array of derivative values at the data
C           points.  If the data are monotonic, these values will
C           determine a monotone cubic Hermite function.
C           The value corresponding to X(I) is stored in
C                D(1+(I-1)*INCFD),  I=1(1)N.
C           No other entries in D are changed.
C
C     INCFD -- (input) increment between successive values in F and D.
C           This argument is provided primarily for 2-D applications.
C           (Error return if  INCFD.LT.1 .)
C
C     IERR -- (output) error flag.  --REMOVED IN THIS VERSION--
C
C***REFERENCES  1. F.N.FRITSCH AND R.E.CARLSON, 'MONOTONE PIECEWISE
C                 CUBIC INTERPOLATION,' SIAM J.NUMER.ANAL. 17, 2 (APRIL
C                 1980), 238-246.
C               2. F.N.FRITSCH AND J.BUTLAND, 'A METHOD FOR CONSTRUCTING
C                 LOCAL MONOTONE PIECEWISE CUBIC INTERPOLANTS,' SIAM
C                 J.SCI.STAT.COMPUT.5,2 (JUNE 1984), 300-304.
C***ROUTINES CALLED  DPCHST,XERROR  ( XERROR is not called in this version )
C***END PROLOGUE  DPCHIM
C
C ----------------------------------------------------------------------
C
C  Change record:
C     82-02-01   1. Introduced  DPCHST  to reduce possible over/under-
C                   flow problems.
C                2. Rearranged derivative formula for same reason.
C     82-06-02   1. Modified end conditions to be continuous functions
C                   of data when monotonicity switches in next interval.
C                2. Modified formulas so end conditions are less prone
C                   of over/underflow problems.
C     82-08-03   Minor cosmetic changes for release 1.
C     87-07-07   Corrected XERROR calls for d.p. name(s).
C
C ----------------------------------------------------------------------
C
C  Programming notes:
C
C     1. The function  DPCHST(ARG1,ARG2)  is assumed to return zero if
C        either argument is zero, +1 if they are of the same sign, and
C        -1 if they are of opposite sign.
C     2. To produce a single precision version, simply:
C        a. Change DPCHIM to PCHIM wherever it occurs,
C        b. Change DPCHST to PCHST wherever it occurs,
C        c. Change all references to the Fortran intrinsics to their
C           single precision equivalents,
C        d. Change the double precision declarations to real, and
C        e. Change the constants ZERO and THREE to single precision.
C
C  DECLARE ARGUMENTS.
C
      INTEGER N, INCFD
      DOUBLE PRECISION  X(N), F(INCFD,N), D(INCFD,N)
C
C  DECLARE LOCAL VARIABLES.
C
      INTEGER I, NLESS1
      DOUBLE PRECISION  DEL1, DEL2, DMAX, DMIN, DRAT1, DRAT2, DSAVE,
     *      H1, H2, HSUM, HSUMT3, THREE, W1, W2, ZERO
      external DPCHST
      DOUBLE PRECISION  DPCHST
      DATA  ZERO /0.D0/, THREE/3.D0/

      NLESS1 = N - 1
      H1 = X(2) - X(1)
      DEL1 = (F(1,2) - F(1,1))/H1
      DSAVE = DEL1
C
C  SPECIAL CASE N=2 -- USE LINEAR INTERPOLATION.
C
C    patch Bruno N was NLESS1
      IF (N .EQ. 2) THEN
         D(1,1) = DEL1
         D(1,N) = DEL1
         return
      endif

C
C  NORMAL CASE  (N .GE. 3).
C
      H2 = X(3) - X(2)
      DEL2 = (F(1,3) - F(1,2))/H2
C
C  SET D(1) VIA NON-CENTERED THREE-POINT FORMULA, ADJUSTED TO BE
C     SHAPE-PRESERVING.
C
      HSUM = H1 + H2
      W1 = (H1 + HSUM)/HSUM
      W2 = -H1/HSUM
      D(1,1) = W1*DEL1 + W2*DEL2
      IF ( DPCHST(D(1,1),DEL1) .LE. ZERO)  THEN
         D(1,1) = ZERO
      ELSE IF ( DPCHST(DEL1,DEL2) .LT. ZERO)  THEN
C        NEED DO THIS CHECK ONLY IF MONOTONICITY SWITCHES.
         DMAX = THREE*DEL1
         IF (DABS(D(1,1)) .GT. DABS(DMAX))  D(1,1) = DMAX
      ENDIF
C
C  LOOP THROUGH INTERIOR POINTS.
C
      DO 50  I = 2, NLESS1
         IF (I .EQ. 2)  GO TO 40
C
         H1 = H2
         H2 = X(I+1) - X(I)
         HSUM = H1 + H2
         DEL1 = DEL2
         DEL2 = (F(1,I+1) - F(1,I))/H2
   40    CONTINUE
C
C        SET D(I)=0 UNLESS DATA ARE STRICTLY MONOTONIC.
C
         D(1,I) = ZERO
         CRES=DPCHST(DEL1,DEL2)
         if (CRES .lt. 0) then 
            goto 42
         elseif (CRES .eq. 0) then 
            goto 41
         else
            goto 45
         endif
C
C        COUNT NUMBER OF CHANGES IN DIRECTION OF MONOTONICITY.
C
   41    CONTINUE
         IF (DEL2 .EQ. ZERO)  GO TO 50
         DSAVE = DEL2
         GO TO 50
C
   42    CONTINUE
         DSAVE = DEL2
         GO TO 50
C
C        USE BRODLIE MODIFICATION OF BUTLAND FORMULA.
C
   45    CONTINUE
         HSUMT3 = HSUM+HSUM+HSUM
         W1 = (HSUM + H1)/HSUMT3
         W2 = (HSUM + H2)/HSUMT3
         DMAX = DMAX1( DABS(DEL1), DABS(DEL2) )
         DMIN = DMIN1( DABS(DEL1), DABS(DEL2) )
         DRAT1 = DEL1/DMAX
         DRAT2 = DEL2/DMAX
         D(1,I) = DMIN/(W1*DRAT1 + W2*DRAT2)
C
   50 CONTINUE
C
C  SET D(N) VIA NON-CENTERED THREE-POINT FORMULA, ADJUSTED TO BE
C     SHAPE-PRESERVING.
C
      W1 = -H2/HSUM
      W2 = (H2 + HSUM)/HSUM
      D(1,N) = W1*DEL1 + W2*DEL2
      IF ( DPCHST(D(1,N),DEL2) .LE. ZERO)  THEN
         D(1,N) = ZERO
      ELSE IF ( DPCHST(DEL1,DEL2) .LT. ZERO)  THEN
C        NEED DO THIS CHECK ONLY IF MONOTONICITY SWITCHES.
         DMAX = THREE*DEL2
         IF (DABS(D(1,N)) .GT. DABS(DMAX))  D(1,N) = DMAX
      ENDIF
C
C  NORMAL RETURN.
C
      end

      DOUBLE PRECISION FUNCTION DPCHST(ARG1,ARG2)
C***BEGIN PROLOGUE  DPCHST
C***REFER TO  DPCHCE,DPCHCI,DPCHCS,DPCHIM
C***ROUTINES CALLED  (NONE)
C***REVISION DATE  870707   (YYMMDD)
C***DESCRIPTION
C
C         DPCHST:  DPCHIP Sign-Testing Routine.
C
C
C     Returns:
C        -1. if ARG1 and ARG2 are of opposite sign.
C         0. if either argument is zero.
C        +1. if ARG1 and ARG2 are of the same sign.
C
C     The object is to do this without multiplying ARG1*ARG2, to avoid
C     possible over/underflow problems.
C
C  Fortran intrinsics used:  SIGN.
C
C***END PROLOGUE  DPCHST
C
C ----------------------------------------------------------------------
C
C  Programmed by:  Fred N. Fritsch,  FTS 532-4275, (415) 422-4275,
C                  Mathematics and Statistics Division,
C                  Lawrence Livermore National Laboratory.
C
C  Change record:
C     82-08-05   Converted to SLATEC library version.
C
C ----------------------------------------------------------------------
C
C  Programming notes:
C
C     To produce a single precision version, simply:
C        a. Change DPCHST to PCHST wherever it occurs,
C        b. Change all references to the Fortran intrinsics to their
C           single presision equivalents,
C        c. Change the double precision declarations to real, and
C        d. Change the constants  ZERO  and  ONE  to single precision.
C
C  DECLARE ARGUMENTS.
C
      DOUBLE PRECISION  ARG1, ARG2
C
C  DECLARE LOCAL VARIABLES.
C
      DOUBLE PRECISION  ONE, ZERO
      DATA  ZERO /0.D0/,  ONE/1.D0/
C
C  PERFORM THE TEST.
C
C***FIRST EXECUTABLE STATEMENT  DPCHST
      DPCHST = DSIGN(ONE,ARG1) * DSIGN(ONE,ARG2)
      IF ((ARG1.EQ.ZERO) .OR. (ARG2.EQ.ZERO))  DPCHST = ZERO
C
      RETURN
C------------- LAST LINE OF DPCHST FOLLOWS -----------------------------
      END
