      DOUBLE PRECISION FUNCTION spmpar(i)
C-----------------------------------------------------------------------
C     SPMPAR PROVIDES THE SINGLE PRECISION MACHINE CONSTANTS FOR
C     THE COMPUTER BEING USED. IT IS ASSUMED THAT THE ARGUMENT
C     I IS AN INTEGER HAVING ONE OF THE VALUES 1, 2, OR 3. IF THE
C     SINGLE PRECISION ARITHMETIC BEING USED HAS M BASE B DIGITS AND
C     ITS SMALLEST AND LARGEST EXPONENTS ARE EMIN AND EMAX, THEN
C
C        SPMPAR(1) = B**(1 - M), THE MACHINE PRECISION,
C
C        SPMPAR(2) = B**(EMIN - 1), THE SMALLEST MAGNITUDE,
C
C        SPMPAR(3) = B**EMAX*(1 - B**(-M)), THE LARGEST MAGNITUDE.
C
C-----------------------------------------------------------------------
C     RWRITTEN BY JPC to use lapack dlamch 
C-----------------------------------------------------------------------
      DOUBLE PRECISION   DLAMCH
      EXTERNAL DLAMCH 
      goto (1,2,3) i 
      spmpar=0.0
      RETURN
 1    spmpar=dlamch('p')
      return
 2    spmpar=dlamch('u')
      return
 3    spmpar=dlamch('o')
      return
      END
