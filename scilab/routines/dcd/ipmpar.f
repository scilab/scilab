      INTEGER FUNCTION ipmpar(i)
C-----------------------------------------------------------------------
C  INTEGERS.
C
C     ASSUME INTEGERS ARE REPRESENTED IN THE N-DIGIT, BASE-A FORM
C
C               SIGN ( X(N-1)*A**(N-1) + ... + X(1)*A + X(0) )
C
C               WHERE 0 .LE. X(I) .LT. A FOR I=0,...,N-1.
C
C     IPMPAR(1) = A, THE BASE.
C     IPMPAR(2) = N, THE NUMBER OF BASE-A DIGITS.
C     IPMPAR(3) = A**N - 1, THE LARGEST MAGNITUDE.
C
C  FLOATING-POINT NUMBERS.
C
C     IT IS ASSUMED THAT THE SINGLE AND DOUBLE PRECISION FLOATING
C     POINT ARITHMETICS HAVE THE SAME BASE, SAY B, AND THAT THE
C     NONZERO NUMBERS ARE REPRESENTED IN THE FORM
C
C               SIGN (B**E) * (X(1)/B + ... + X(M)/B**M)
C
C               WHERE X(I) = 0,1,...,B-1 FOR I=1,...,M,
C               X(1) .GE. 1, AND EMIN .LE. E .LE. EMAX.
C
C     IPMPAR(4) = B, THE BASE.
C  SINGLE-PRECISION
C     IPMPAR(5) = M, THE NUMBER OF BASE-B DIGITS.
C     IPMPAR(6) = EMIN, THE SMALLEST EXPONENT E.
C     IPMPAR(7) = EMAX, THE LARGEST EXPONENT E.
C  DOUBLE-PRECISION
C     IPMPAR(8) = M, THE NUMBER OF BASE-B DIGITS.
C     IPMPAR(9) = EMIN, THE SMALLEST EXPONENT E.
C     IPMPAR(10) = EMAX, THE LARGEST EXPONENT E.
C-----------------------------------------------------------------------
C     RWRITTEN BY JPC to use lapack dlamch + a small c program 
C     for ipmpar(3)
C-----------------------------------------------------------------------
      DOUBLE PRECISION   DLAMCH
      EXTERNAL DLAMCH 
      include '../stack.h'
      goto (1,1,3,4,1,1,1,1,9,10) i
 1    continue 
      call basout(io,wte,'ipmpar called with wrong argument')
      ipmpar = 0
      return 
 3    ipmpar = largestint()
      return
 4    ipmpar=dlamch('b')
      return
 9    ipmpar=dlamch('m')
      return
 10   ipmpar= dlamch('l')
      END
