c     ====================================
c .   Runge-Kutta driver with adaptive stepsize control.  Integrate
c .   the starting values ystart(1:nvar) from x1 to x2 with accuracy
c .   eps, storing intermediate results in the common block /path/.
c .   h1 should be set as a guessed first stepsize, hmin as the
c .   minimum allowed stepsize (can be zero).
c .   On output, nok and nbad are the number of good and bad (but
c .   retried and fixed) steps taken, and ystart is replaced by
c .   values at the end of the integration interval.
c .   derivs is the user-supplied subroutine for calculating the
c .   right-hand side derivatives, while rkqs is the name of
c .   the stepper routine to be used.
c .   /path/ contains its own information about how often an
c .   intermediate value is to be stored.
c .
c .   The original version has been modified to replace statically
c .   allocated arrays y, yscal and dxdy by parts of rwork argument
c     array + blas use. Serge Steer INRIA- feb 2012

c     ====================================
      subroutine odeint(ystart,nvar,x1,x2,eps,h1,hmin,nok,nbad,derivs,
     $     rkqc,rwork)

      external derivs,rkqc
      integer maxstp,kount,nvar,i,nok,nbad,nstp
      double precision two,zero,tiny,x,h
      parameter (maxstp=10000,two=2.0d0,zero=0.0d0,tiny=1.d-30)
      double precision x1,x2,eps,h1,hmin,hdid,hnext
      double precision ystart(nvar)
      double precision rwork(*)
      character*80 messag
      integer ly,lyscal,ldydx,lwork
c
      ly=1
      lyscal=ly+nvar
      ldydx=lyscal+nvar
      lwork=ldydx+nvar

      ierror=0
      if ( abs(x2-x1).le.tiny) return
      x=x1
      h=sign(h1,x2-x1)
      nok=0
      nbad=0
      kount=0
      call dcopy(nvar,ystart,1,rwork(ly),1)

      do 16 nstp=1,maxstp
         call derivs(nvar,x,rwork(ly),rwork(ldydx))
         if (ierror.gt.0) return
         do 12 i=0,nvar-1
            rwork(lyscal+i)=abs(rwork(ly+i))+abs(h*rwork(ldydx+i))+tiny
 12      continue

         if((x+h-x2)*(x+h-x1).gt.zero) h=x2-x
         call rkqc(rwork(ly),rwork(ldydx),nvar,x,h,eps,rwork(lyscal),
     $        hdid,hnext,derivs,rwork(lwork))
         if(ierror.gt.0) return
         if(hdid.eq.h)then
            nok=nok+1
         else
            nbad=nbad+1
         endif
         if((x-x2)*(x2-x1).ge.zero)then
            call dcopy(nvar,rwork(ly),1,ystart,1)
            return
         endif
         if(abs(hnext).lt.hmin) then
            write(messag, 17) hnext,hmin
            hnext=hmin
         endif
         h=hnext
 16   continue
      ierror=-1
c     print *, 'Trop d''iterations a faire pour la precision demandee.'
      return
 17   format('stepsize ',e10.3,' smaller than minimum ',e10.3)
      end
c     ====================================


