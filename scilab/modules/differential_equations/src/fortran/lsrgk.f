c     ====================================
c     runge kutta d'ordre 4 adaptatif 
c     l'interface lsrgk a ete fait en s'inspirant de lsode 
c     voir lsode.f pour comprendre le sens des variables 
c     ====================================
      subroutine lsrgk (f, neq, y, t, tout, itol, rtol, atol, itask,
     1            istate, iopt, rwork, lrw, iwork, liw, jac, mf)
     
      external f, jac,rkqc
      integer neq, itol, itask, istate, iopt, lrw, iwork, liw, mf
      double precision y, t, tout, rtol, atol, rwork
      integer nok,nbad
      dimension neq(*), y(*), rtol(*), atol(*), rwork(lrw), iwork(liw)
      integer iero
      common/ierode/iero
      iero=0
      call odeint(y,neq,t,tout,atol(1),1.0d-4,0.0d0,nok,nbad,f,rkqc)
      t=tout
      if (iero.gt.0) istate=-1
      return
      end
c     ====================================
