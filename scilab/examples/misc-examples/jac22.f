      subroutine jac22 (t, y, ydot, pd, cj, rpar, ipar)
c     Copyright INRIA
      integer  nrowpd, ipar
      double precision t, y, ydot, pd, cj, rpar
      parameter (nrowpd=2)
      dimension y(2), pd(nrowpd,2)
      pd(1,1) = 0.0d0
      pd(1,2) = 1.0d0
      pd(2,1) = -200.0d0*y(1)*y(2) - 1.0d0
      pd(2,2) = 100.0d0*(1.0d0 - y(1)*y(1))
      pd(1,1) = cj - pd(1,1)
      pd(1,2) =    - pd(1,2)
      pd(2,1) =    - pd(2,1)
      pd(2,2) = cj - pd(2,2)
      end


