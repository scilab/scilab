      DOUBLE PRECISION FUNCTION D1MACH(I)
C
C  Double-precision machine constants.
C  This implementation for use in MATLAB Fortran Mex-files.
C
C  D1MACH(1) = realmin = B**(EMIN-1), the smallest positive magnitude.
C  D1MACH(2) = realmax = B**EMAX*(1 - B**(-T)), the largest magnitude.
C  D1MACH(3) = eps/2 = B**(-T), the smallest relative spacing.
C  D1MACH(4) = eps = B**(1-T), the largest relative spacing.
C  D1MACH(5) = LOG10(B)
C
c  DLAMCH
*          = 'E' or 'e',   DLAMCH := eps
*          = 'S' or 's ,   DLAMCH := sfmin
*          = 'B' or 'b',   DLAMCH := base
*          = 'P' or 'p',   DLAMCH := eps*base
*          = 'N' or 'n',   DLAMCH := t
*          = 'R' or 'r',   DLAMCH := rnd
*          = 'M' or 'm',   DLAMCH := emin
*          = 'U' or 'u',   DLAMCH := rmin
*          = 'L' or 'l',   DLAMCH := emax
*          = 'O' or 'o',   DLAMCH := rmax
*
*          where
*
*          eps   = relative machine precision
*          sfmin = safe minimum, such that 1/sfmin does not overflow
*          base  = base of the machine
*          prec  = eps*base
*          t     = number of (base) digits in the mantissa
*          rnd   = 1.0 when rounding occurs in addition, 0.0 otherwise
*          emin  = minimum exponent before (gradual) underflow
*          rmin  = underflow threshold - base**(emin-1)
*          emax  = largest exponent before overflow
*          rmax  = overflow threshold  - (base**emax)*(1-eps)
*


      double precision DLAMCH
      IF (I .EQ. 1) D1MACH = DLAMCH('U')
      IF (I .EQ. 2) D1MACH = DLAMCH('O')
      IF (I .EQ. 3) D1MACH = DLAMCH('E')
      IF (I .EQ. 4) D1MACH = DLAMCH('P')
      IF (I .EQ. 5) D1MACH = log10(DLAMCH('B'))   
      RETURN
      END

