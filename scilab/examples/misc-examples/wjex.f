      subroutine wjex (neq, t, y, ml, mu, pd, nrpd)
c     -------------------------------------------
c     wfex example continued : we provide here 
c     the jacobian function 
c     jacobian routine jex
c     scilab ode
c     ode([1;0;0],0,[0.4,4],'wfex','wjex')

c     Copyright INRIA
      double precision pd, t, y
      dimension y(3), pd(nrpd,3)
      pd(1,1) = -.040d+0
      pd(1,2) = 1.0d+4*y(3)
      pd(1,3) = 1.0d+4*y(2)
      pd(2,1) = .040d+0
      pd(2,3) = -pd(1,3)
      pd(3,2) = 6.0d+7*y(2)
      pd(2,2) = -pd(1,2) - pd(3,2)
      return
      end



