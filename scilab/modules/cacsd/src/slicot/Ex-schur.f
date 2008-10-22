
      LOGICAL FUNCTION SB02MV( REIG, IEIG )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the stable eigenvalues
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     REIG    (input) DOUBLE PRECISION
C             The real part of the current eigenvalue considered.
C
C     IEIG    (input) DOUBLE PRECISION
C             The imaginary part of the current eigenvalue considered.
C
C     METHOD
C
C     The function value SB02MV is set to .TRUE. for a stable eigenvalue
C     and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C
C     REVISIONS
C
C     -
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION  IEIG, REIG
C     .. Executable Statements ..
C
      SB02MV = REIG.LT.ZERO
C
      RETURN
C *** Last line of SB02MV ***
      END


      LOGICAL FUNCTION SB02MW( REIG, IEIG )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the stable eigenvalues for discrete-time
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     REIG    (input) DOUBLE PRECISION
C             The real part of the current eigenvalue considered.
C
C     IEIG    (input) DOUBLE PRECISION
C             The imaginary part of the current eigenvalue considered.
C
C     METHOD
C
C     The function value SB02MW is set to .TRUE. for a stable
C     eigenvalue (i.e., with modulus less than one) and to .FALSE.,
C     otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C
C     REVISIONS
C
C     -
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION  IEIG, REIG
C     .. External Functions ..
      DOUBLE PRECISION   DLAPY2
      EXTERNAL           DLAPY2
C     .. Executable Statements ..
C
      SB02MW = DLAPY2( REIG, IEIG ).LT.ONE
C
      RETURN
C *** Last line of SB02MW ***
      END

      LOGICAL FUNCTION SB02OW( ALPHAR, ALPHAI, BETA )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the stable generalized eigenvalues for continuous-time 
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     ALPHAR  (input) DOUBLE PRECISION
C             The real part of the numerator of the current eigenvalue 
C             considered.
C
C     ALPHAI  (input) DOUBLE PRECISION
C             The imaginary part of the numerator of the current 
C             eigenvalue considered.
C
C     BETA    (input) DOUBLE PRECISION
C             The (real) denominator of the current eigenvalue 
C             considered. It is assumed that BETA <> 0 (regular case).
C
C     METHOD
C
C     The function value SB02OW is set to .TRUE. for a stable eigenvalue
C     and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB02CW by P. Van Dooren, Philips
C     Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     -
C
C     ******************************************************************
C
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   ALPHAR, ALPHAI, BETA
C     .. Executable Statements ..
C
      SB02OW = (( ALPHAR.LT.ZERO .AND. BETA.GT.ZERO ) .OR.
     $         ( ALPHAR.GT.ZERO .AND. BETA.LT.ZERO )) .AND.
     $          abs(BETA).GT. abs(ALPHAR)*dlamch('p')
C
      RETURN
C *** Last line of SB02OW ***
      END



      LOGICAL FUNCTION SB02OX( ALPHAR, ALPHAI, BETA )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the stable generalized eigenvalues for 
C     discrete-time
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     ALPHAR  (input) DOUBLE PRECISION
C             The real part of the numerator of the current eigenvalue 
C             considered.
C
C     ALPHAI  (input) DOUBLE PRECISION
C             The imaginary part of the numerator of the current 
C             eigenvalue considered.
C
C     BETA    (input) DOUBLE PRECISION
C             The (real) denominator of the current eigenvalue 
C             considered.
C
C     METHOD
C
C     The function value SB02OX is set to .TRUE. for a stable eigenvalue
C     (i.e., with modulus less than one) and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB02CX by P. Van Dooren, Philips
C     Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     -
C
C     ******************************************************************
C
      DOUBLE PRECISION   ONE
      PARAMETER          ( ONE = 1.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   ALPHAR, ALPHAI, BETA
C     .. External Functions ..
      DOUBLE PRECISION   DLAPY2
      EXTERNAL           DLAPY2
C     .. Intrinsic Functions ..
      INTRINSIC          ABS
C     .. Executable Statements ..
C
      SB02OX = DLAPY2( ALPHAR, ALPHAI ).LT.ABS( BETA )
C
      RETURN
C *** Last line of SB02OX ***
      END


      LOGICAL FUNCTION ZB02MV( EIG )
C
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C
C     PURPOSE
C
C     To select the stable eigenvalues in ordering the Schur form
C     of a matrix.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     EIG    (input) COMPLEX*16
C             The  current eigenvalue considered.
C
C     METHOD
C
C     The function value ZB02MV is set to .TRUE. for a stable eigenvalue
C     and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C    
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC         DREAL
C     .. Executable Statements ..
C
      ZB02MV = DREAL(EIG).LT.ZERO
C
      RETURN
C *** Last line of ZB02MV ***
      END

      LOGICAL FUNCTION ZB02MW( EIG )
C
C     RELEASE 4.0, WGS COPYRIGHT 2001.
C
C     PURPOSE
C
C     To select the eigenvalues inside the unit circle in ordering
C     the Schur form of a matrix.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     EIG    (input) COMPLEX*16
C             The  current eigenvalue considered.
C
C     METHOD
C
C     The function value ZB02MW is set to .TRUE. for an eigenvalue which
C     is inside the unit circle and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C    
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE
      PARAMETER         ( ONE = 1.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16        EIG
C     .. Intrinsic Functions ..
      INTRINSIC ABS
C     .. Executable Statements ..
C
      ZB02MW = ABS(EIG).LT.ONE
C
      RETURN
C *** Last line of ZB02MW ***
      END



      LOGICAL FUNCTION ZB02OW( ALPHA, BETA )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To select the stable generalized eigenvalues for the
C     continuous-time.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     ALPHAR  (input) DOUBLE PRECISION
C             The real part of the numerator of the current eigenvalue 
C             considered.
C
C     ALPHAI  (input) DOUBLE PRECISION
C             The imaginary part of the numerator of the current 
C             eigenvalue considered.
C
C     BETA    (input) DOUBLE PRECISION
C             The (real) denominator of the current eigenvalue 
C             considered. It is assumed that BETA <> 0 (regular case).
C
C     METHOD
C
C     The function value ZB02OW is set to .TRUE. for a stable eigenvalue
C     and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C
C     ******************************************************************
C
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      COMPLEX*16         ALPHA, BETA
      INTRINSIC          DREAL
C     .. Executable Statements ..
C
      if (abs(BETA).ne.ZERO) then
         ZB02OW =  DREAL(ALPHA/BETA).LT.ZERO 
      else
          ZB02OW = .FALSE.
      endif
C
      RETURN
C *** Last line of zb02ow ***
      END


      LOGICAL FUNCTION ZB02OX( ALPHA, BETA )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To select the stable generalized eigenvalues for the
C     discrete-time algebraic.
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     ALPHAR  (input) DOUBLE PRECISION
C             The real part of the numerator of the current eigenvalue 
C             considered.
C
C     ALPHAI  (input) DOUBLE PRECISION
C             The imaginary part of the numerator of the current 
C             eigenvalue considered.
C
C     BETA    (input) DOUBLE PRECISION
C             The (real) denominator of the current eigenvalue 
C             considered.
C
C     METHOD
C
C     The function value ZB02OX is set to .TRUE. for a stable eigenvalue
C     (i.e., with modulus less than one) and to .FALSE., otherwise.
C
C     REFERENCES
C
C     None.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTOR
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, optimal regulator, Schur form.
C
C     ******************************************************************
C
C     .. Scalar Arguments ..
      COMPLEX*16         ALPHA, BETA
C     .. External Functions ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS
C     .. Executable Statements ..
C
      ZB02OX = ABS( ALPHA ).LT.ABS( BETA )
C
      RETURN
C *** Last line of ZB02OX ***
      END







