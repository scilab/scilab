C     Examples for dasrt 
C     ----------------------
C
c     Copyright INRIA
      subroutine gr1 (neq, t, y, ng, groot, rpar, ipar)
      INTEGER neq, ng,ipar(*)
      DOUBLE PRECISION t, y(*), groot(*),rpar(*)
      groot(1) = ((2.0D0*LOG(y(1)) + 8.0D0)/t - 5.0D0)*y(1)
      groot(2) = LOG(y(1)) - 2.2491D0
      RETURN
      END

      subroutine gr2 (neq, t, y, ng, groot, rpar, ipar)
      INTEGER neq, ng, ipar(*)
      DOUBLE PRECISION t, y, groot,rpar(*)
      DIMENSION y(*), groot(*)
      groot(1) = y(1)
      RETURN
      END

