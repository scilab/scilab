c     ====================================
c     ode Gateway for Adaptative fourth order Runge Kutta
C
c     The original version has been modified to pass the  rwork
c     argument to odeint
c     array + blas use. Serge Steer INRIA- feb 2012

c     ====================================
      subroutine lsrgk (f, neq, y, t, tout, itol, rtol, atol, itask,
     1            istate, iopt, rwork, lrw, iwork, liw, jac, mf)
      external f, jac,rkqc
      integer neq, itol, itask, istate, iopt, lrw, iwork, liw, mf
      double precision y, t, tout, rtol, atol, rwork
      integer nok,nbad
      dimension neq(*), y(*), rtol(*), atol(*), rwork(lrw), iwork(liw)
      ierror=0
      call odeint(y, neq, t,tout,atol(1),1.0d-4,0.0d0,nok,nbad,f,rkqc,
     $     rwork)
      t=tout
      if (ierror.gt.0) istate=-1
      return
      end
c     ====================================
